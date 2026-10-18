# Code conventions

How the C in this repository is written and why it looks the way it does.
Read [architecture.md](architecture.md) first for where things live.

Everything here serves one constraint: **every C function compiles to the
retail bytes** ([BUILD_FIDELITY.md](../BUILD_FIDELITY.md)). Many oddities
(duplicate declarations under other names, `extern short` read as a float,
section attributes on globals) exist only because they decide which
instructions GCC 2.95 emits. Edits to matched code are type, name and
comment changes only, unless they keep identical bytes, and that is proven
with the commands under [Verifying a change](#verifying-a-change). The
behaviour, quirks included, stays 1:1 with the PS2 game, and a PC port
importing this C has to keep it that way.

## 1. Symbol names

### Address names are permanent

| Form | What | Example |
|---|---|---|
| `func_<ADDR>` | executable function (core or game) | `func_001EC7C8` |
| `func_LNN_<ADDR>` | level function, address in its canonical (lowest) level `NN` | `func_L00_002180F8` |
| `D_<ADDR>` | executable or resident data (below 0x15F000, or the executable's own symbols) | `D_0013F450` |
| `D_LNN_<ADDR>` | level data in the replaced `main` range (0x15F000 and up), canonical level `NN` | `D_L17_00162108` |

**They are never renamed.** The build, the linker script, the progress
report and every tool read the address and level out of the name
([NAMES.md](../NAMES.md), [OVERLAYS.md](../OVERLAYS.md) "Names").
Declarations, definitions, `INCLUDE_ASM` lines and `__asm__` labels always
spell the address name.

### Readable names are macros on top

| File | Role |
|---|---|
| `config/names.tsv` | the table. Columns: `symbol`, `name`, `tier`, `source`, `evidence`, `alternatives` |
| `include/names.h` | **generated** by `python3 tools/names.py header`; `#define UpdateAllCameras func_001EC7C8`. Never edit by hand |
| `tools/names.py` | `build`, `header`, `apply` (rewrites bodies to use names), `check`, lookup (`python3 tools/names.py UpdateAllCameras`) |

Tiers ([NAMES.md](../NAMES.md#tiers)):

| Tier | In `names.h` | Meaning |
|---|---|---|
| `recovered` | yes | an original identifier (e.g. from mangled NTSC symbols) |
| `descriptive` | yes | a later name with evidence |
| `candidate` | no | weaker; shown to workers in dossiers only |

Sources are RaC1-only: the NTSC decomp (Lombyte),
ReRAC, and the PAL memory map, each mapped to PAL addresses by function
size alignment or fingerprints. Core/SDK names (`memcpy`, `sceGsSync`) are
in the table but not the header, so they don't shadow library spellings.

Inside function bodies, use the readable name; everywhere else, the
address name. `common.h` includes `names.h`, so both spellings compile to
the same object (verified by hashing every object when it was introduced).

```c
extern void func_001EC7C8(int);     /* declare with the address name */

void func_L00_XXXXXXXX(void) {
    UpdateAllCameras(0);            /* call with the readable name */
}
```

Treat readable names as hints: `func_001E9808` is `showDebugFont` in the
table but its matched body starts a level (`src/game/bmain.c`).

## 2. Declarations that steer codegen

### Per-file aliases

Level source files grew by landing candidates one at a time, each written
against its own view of the globals and callees it uses. C forbids
declaring one symbol twice with different types, so a later candidate that
needs another view declares a **suffixed alias** bound to the same linker
symbol:

```c
/* src/overlays/shared/help_00203E98.c */
extern struct Hero D_0013F450_05278 __asm__("D_0013F450");
extern f32 func_00214358_05278(Vec4f *, s32, f32) __asm__("func_00214358");
```

The suffix is the tail of the function that brought it (here
`func_L00_00205278`), so aliases from different landings don't collide.
The object code is identical to using the plain name; the alias only
changes the C type. Also seen:

- `_r` definitions: a function defined under another C name with an
  `__asm__` label, used when the file's existing prototype of it has
  another type that matched callers rely on
  ([LEVERS.md](../LEVERS.md) "Rules"):
  ```c
  void func_L00_00250478_r(Object_50478p *obj, int i, int j, Packet_50478p *out) __asm__("func_L00_00250478");
  void func_L00_00250478_r(Object_50478p *obj, int i, int j, Packet_50478p *out) { ... }
  ```
  Tools read a definition of `func_X_r` as `func_X`.
- Function-pointer macros in a few large files, e.g.
  `#define d_update_visible ((void(*)(void))(func_00213C78))` in
  `src/overlays/shared/update_0029B6A0.c`.

An alias names exactly one existing symbol, never an expression such as
`"D_0013E633 + 0xF1D"`, and never a number ([QUEUE.md](../QUEUE.md)
"Rules"). Hardware registers are `*(volatile int *)0x10000000`.

**Policy for new C:** these aliases are legacy. The maintainer's current
rule is that new or reworked C adds no `__asm__` at all: no symbol aliases,
no `_r` definitions, no `SDATA()`, no asm statements. Use the file's own
declarations, value casts, real symbol names and struct copies instead.
Existing code at HEAD keeps its aliases until someone tidies it with
identical bytes. (Several repo docs, [LEVERS.md](../LEVERS.md) and
[QUEUE.md](../QUEUE.md), still describe aliases as an allowed lever; for
new work, the stricter rule wins.)

### How a global is addressed: `$gp` vs `lui`

Retail reaches some globals through `$gp` (small data, base 0x166D00) and
others with `lui`/`%lo`. The build uses `-G2`, and three macros from
`include/common.h` pick the form per declaration without changing the use
sites:

| Macro | Expands to | Effect |
|---|---|---|
| `NOT_SDA` | `__attribute__((section(".data")))` | keeps a small global **out** of `$gp` addressing: `lui`/`lo` |
| `MACRO_ADDR` | `__attribute__((section(".sdata")))` | the compiler emits the unsplit assembler macro `lw $2,D`, giving retail's one-register `lui $2` / `lw $2,%lo(D)($2)` load. Loads only; a store can land in a delay slot and warn "macro used after .set nomacro" |
| `SDATA(sym)` | `__asm__(#sym "__gp") MACRO_ADDR` | a scalar of a level file's own small data with its real type, reached through `$gp` (`tools/check_macro_slots.py` equates the `sym__gp` label). Legacy under the no-`__asm__` rule |

The older idiom for a `$gp` access is `extern short D_x;` read as
`*(float *)&D_x` or `*(int *)&D_x`: the declared size (2, within `-G2`)
makes it small data. It looks like a hack, but it is the project's
documented way to get `$gp` at `-G2` ([AGENT_WORKFLOW.md](../AGENT_WORKFLOW.md)).
In a port, read these as plain globals of the cast-to type.

One file can only have one addressing form per symbol: one `MACRO_ADDR`
alias changes the form for the whole file, which is why every landing
re-checks the whole file (`tools/overlay_file_check.py`).

### Assembly stubs

From `include/include_asm.h`:

| Macro | Meaning |
|---|---|
| `INCLUDE_ASM("asm/overlays", func_L00_00205158);` | not yet decompiled: assembles the retail `.s` in place |
| `ASM_FUNC(folder, name)` | original hand-written or VU0 assembly (`config/handwritten_asm.txt`); counts as finished |
| `LINKER_REMNANT(folder, name)` | dead-strip leftovers (`config/linker_remnants.txt`) |
| `INCLUDE_RODATA` | retail rodata included verbatim |

Under `M2CTX` or `PERMUTER` they expand to nothing. The `.s` files are
generated locally from your own baserom (`tools/setup_asm.sh`) and never
committed ([ASM_CLASSIFICATION.md](../ASM_CLASSIFICATION.md)). A port has
to supply its own C (or host code) for every function still behind one of
these; `nonmatching/functional/` has C written for the native port only.

## 3. Shared headers

Use these instead of private struct copies (field-by-field walk-through:
[data-structures.md](data-structures.md)):

| Header | Contents |
|---|---|
| `include/common.h` | `s8`..`u64`, `f32`, `f64`; `NOT_SDA`, `MACRO_ADDR`, `SDATA`; `qcopy`, `qcopy_nc`, `qzero`, `CpuSync`, `CpuSyncP`, `CpuEnableInt`, `CpuDisableInt`; includes `names.h` |
| `include/moby.h` | `struct Moby` (0x100 bytes: `state` 0x20, `pclass` 0x24, `next` 0x28, `flags` 0x34, `update` 0x74, `pvars` 0x78, `oclass` 0xA6 ...) and `struct MobyClass` |
| `include/hero.h` | `struct Hero` (`D_0013F450`) and its records: `HeroMotion`, `HeroState`, `HeroItemSlot`, `HeroManipulator`, `HeroHealth`, `HeroEase`, `HeroRandTimer`, `HeroVelocityKey`, `MobyTrail` |
| `include/pad.h` | `struct PadState` (0x350 bytes, `D_0013CA40`) |
| `include/vector.h` | `Vec4f` (4 floats `xyzw`, 4-byte aligned), `Vec4A` (same, 16-byte aligned), `Vec4` (union `q`/`f[4]`/`i[4]`, moved with one `lq`/`sq`), `VecQuad` (128-bit integer type) |
| `include/records.h` | `DispatchRec` (0x14-byte function-pointer entry), `PadSlots`, `AnimSeq` |
| `include/callees.h` | prototypes of engine functions that three or more files declared the same way |
| `include/structs.h`, `include/moby_pvars.h` | older shared structs; `moby_pvars.h` (class pvars adapted from Wrench) is not included by any source file today |

`moby.h`, `hero.h`, `pad.h` and `vector.h` are adapted from Lombyte's
headers (MIT; see `THIRD_PARTY_NOTICES.md`). Field names that are still
`unkNNN` / `pad_NNN` are unknown, not unused.

### Types vs views: keep the cast on the value

When a function reads a field with a different type than the header gives
it, cast the **value**, not the field's address:

```c
x = (s32)h->motion.unk10;          /* value cast: same code */
h->field = (T)(v);                 /* value cast on store */
*(T *)&D_0013F450.pad_x            /* address cast: avoid */
```

A cast through a member's address folds the member offset into the symbol
(`%hi(D_0013F450+off)`) and can change the generated code; value casts fixed
about 60 hero mismatches in the header pass. If a cast lands in a `pad_`
field, the better fix is to name that field in the header. The underlying
lever ("how an address is spelled": struct symbol vs base register plus
offset, separate symbols for separate blocks) is
[LEVERS.md](../LEVERS.md) lever 15.

### Private views that stay on purpose

A few files keep their own layout because the shared struct does not fit
what the code does with that memory:

| View | File | Why |
|---|---|---|
| `Moby_26ff10` | `src/overlays/shared/partupd_0026A130.c` | not a moby: a particle record (it is passed to `KillPart`, `func_L00_002688A8`), with its own nested vector and pvar layouts |
| `Moby_2E8210`, `Moby_2E8AC8` | `src/overlays/shared/vendor_002E1660.c` | camera-side records (`cam` at 0x70 points to a `Cam_2E8210`), not a moby's layout past the header |
| `HeroPos_2E9978` | `src/overlays/l10_orxon/vendor_002E30F8.c` | a one-field view reached as `D_0013E633 + 0xE9D` (= `gHeroPos`, 0x13F4D0) plus 0x200C: retail derives the address from a register holding that base, which the `Hero` struct symbol would fold away ([LEVERS.md](../LEVERS.md) lever 15) |
| `HeroManip4_5FF0` | `src/overlays/shared/help_00203E98.c` | four more manipulators after `manipulators[7]` at hero+0xEF0, plus a random-delay timer, which `struct Hero` does not lay out that way |
| `Pad_2D9F08` | `src/overlays/shared/vendor_002A5218.c` | reads `held` and `pressed` together as one doubleword (`b1C0`), which `PadState`'s separate `s32` fields cannot express without changing the load |

*(inferred from the definitions and their uses; the pad doubleword reason
is from the header-tidy notes.)* Small `fNN` data views shared by
`help_*.c` files (`S141948`, `Cam`, `Row13E650`, `Rec2C` ...) are also
still private.

## 4. Comments and provenance

- Every function definition gets a comment of one or two sentences above
  it: what the function does and, when needed, why the C has the shape it
  has. Examples:
  ```c
  /* Hero state machine: per-frame movement and physics for the current hero state. */
  void func_L00_002180F8(void) {
  ```
- Name what a number means where it is known (`/* 0x1C: frames left */`
  on struct fields, class names next to oClass numbers).
- **Provenance notes are licence credit and must stay.** Code ported from
  the NTSC decomp (Lombyte) or another project ends its comment with a line
  such as
  `Adapted from Lombyte (MIT) for PAL: src/textbin/world/startlevel.c, startlevel.`
  or `Adapted from Lombyte (MIT) for PAL by OpenRAC's tools/port.py: <file>, <name>.`
  Headers carry the same notes (`include/moby.h`, `include/hero.h`). These
  are referenced from `THIRD_PARTY_NOTICES.md`; never delete or reword
  them, even in a tidy pass. Function names inside such comments
  (`FUN_L04_002aee30`) are the source project's US names, not ours.
- File headers in `src/game/*.c` give the original translation unit
  (`bmain.cpp in the original source; text 0x1E9808-0x1E9E70`).
- Don't trust a stub's old comment blindly; many were wrong
  ([LEVERS.md](../LEVERS.md) "Rules").

## 5. Pure C, no permuter

Rules for new C ([WORKER.md](../WORKER.md),
[QUEUE.md](../QUEUE.md#rules), [AGENT_WORKFLOW.md](../AGENT_WORKFLOW.md),
plus the maintainer's current policy):

- Plain C. No register pins, no artificial barriers
  (`__asm__("" : ...)`, `do { } while (0)`), no `volatile` added only to
  pin an order, no read of a never-assigned local. A function that only
  matches that way is a near miss, not a match.
- No inline assembly inside a function, with these exceptions:
  - the header helpers in `include/common.h`: `qcopy` / `qcopy_nc`
    (retail's `lq $2` / `sq $2` quadword copy, with and without a memory
    clobber), `qzero` (`sq $0`), and `CpuSync`, `CpuSyncP`,
    `CpuEnableInt`, `CpuDisableInt`. Call them; don't copy their asm;
  - since 2026-10-17, functions whose retail code is VU0 (`v*.xyzw`,
    `qmtc2`/`qmfc2`, `lqc2`/`sqc2`, `vcallms`, `ctc2`/`cfc2`) or
    hand-written assembly may contain asm.
- No `__asm__` aliases, `_r` definitions or `SDATA()` in new C (see
  [Per-file aliases](#per-file-aliases)).
- No decomp-permuter. [PERMUTER.md](../PERMUTER.md) documents the setup
  but the project does not use it; decompilation is by hand, led by
  Ghidra and the assembly.
- The build is fixed: no new step that edits compiler output, no
  per-function flags. Options are per file, in `config/file_cflags.txt`.
- Never use Sony SDK source, samples or headers, or leaked material
  ([CONTRIBUTING.md](../../CONTRIBUTING.md) "Sources"). Never write a level
  address as a number: use the symbol (`D_L05_001B24D4`).

For the PC port: `qcopy(dst, src)` is a 16-byte copy, `qzero(p)` a 16-byte
clear, the `Cpu*` helpers are ordering/interrupt instructions with no
effect on game logic. Replace them with host equivalents.

## 6. Verifying a change

All commands run from the repo root, inside the toolchain container
(`bash tools/docker/run.sh ...`):

| Change | Command |
|---|---|
| Level code (`src/overlays/...`) | `bash tools/docker/run.sh python tools/overlay_file_check.py <file> [...]` (no args: files changed since HEAD). Builds each file and checks every C function in it; exits 1 on any build failure or non-EXACT function |
| Executable (`src/game`, `src/core`, `src/libgcc`) | `bash tools/docker/run.sh bash tools/build_sn.sh` (full build, links at retail addresses and audits) |
| One candidate function | `bash tools/docker/run.sh python tools/try_func.py func_X cand.c [--diff]` |
| After pulling | `bash tools/docker/run.sh bash tools/setup_asm.sh` (regenerates `asm/`; stale asm causes mass mismatches) |
| Progress report | `bash tools/docker/run.sh python tools/gen_progress_report.py --no-build`, then `python3 tools/gen_progress_report.py --check` |

A comment-only or rename-only edit still gets the check: a rename that
changes a declaration's type or section can change code.

### Several agents in one checkout

`tools/claims.py` ([AGENT_WORKFLOW.md](../AGENT_WORKFLOW.md) "Several
agents at once"):

```sh
python3 tools/claims.py claim <owner> func_L05_002559DC   # before working on it
python3 tools/claims.py release <owner> func_L05_002559DC # giving up
python3 tools/claims.py lock <owner>                      # before writing src/
python3 tools/claims.py unlock <owner>                    # right after
```

## 7. Reading a typical level file

Take `src/overlays/shared/vendor_002B0068.c`. Top to bottom:

1. **Header line**: `/* Shared code following vendor; generated by
   tools/overlay_src.py, stubs replaced by C as functions are matched. */`.
   "Following vendor" is its link position, not its subject.
2. **Includes**: `common.h`, `include_asm.h`, then shared headers
   (`hero.h`, `moby.h`, often `pad.h`, `callees.h`).
3. **The first landing's block**: aliases, local types and externs it
   needed, e.g. `extern struct Hero D_0013F450_B0068 __asm__("D_0013F450");`,
   a pvars struct `ShotVars_B0068`, `extern float D_0015EE6C_B0068
   __asm__("D_0015EE6C") MACRO_ADDR;`. Then that function.
4. **Each later landing** repeats the pattern: its own declarations (often
   the plain names now, `extern struct Hero D_0013F450;`), then its
   function. Declarations are scattered through the file in landing order;
   a symbol may appear under two C names.
5. **Stubs** for what is still unmatched: `INCLUDE_ASM("asm/overlays",
   func_LNN_XXXXXXXX);` in address order between the C functions.

Function order in the file is retail's address order and must stay so.

### pvars

A moby's class-specific state hangs off `Moby.pvars` (offset 0x78, `u8 *`).
Update functions cast it to a struct local to the file:

```c
/* src/overlays/shared/vendor_002B0068.c */
void func_L04_002B0290(struct Moby *m)
{
    ShotVars *v = (ShotVars *)m->pvars;
    ...
    if (v->owner->oclass == 0xFF) range = 128.0f;
```

Other spellings in the tree: `*(T **)&m->pvars`, and `m->pvars + 0xC0`
for an embedded block. Whichever spelling a matched function uses is the
one that gave retail's bytes; keep it.
[PVARS.md](../PVARS.md) describes the pvars idea and a catalogue of class
layouts from Wrench in `include/moby_pvars.h`; that page's `MobyInstance`
offsets (`pVar` at 0xAC, `pUpdate` at 0xA8, `oClass` at 0xBC) and its level
numbers do not agree with `include/moby.h` and the level table, so use
`moby.h` and [OVERLAYS.md](../OVERLAYS.md#levels) for those.
