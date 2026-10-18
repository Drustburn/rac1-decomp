# Architecture of the game code

A map of how *Ratchet & Clank* (PS2, PAL v2.00, `SCES_509.16`) is laid out
in this repository and how its code fits together at runtime. Written for
new contributors and for developers of a native PC port that imports this C.
Conventions for reading and writing the C are in
[conventions.md](conventions.md).

Statements are backed by a file, address or table in this repository.
Anything marked *(inferred)* is a reading of the evidence, not something a
table or matched function states outright.

## The one rule: this is a byte-exact matching decomp

Every C function here compiles, with retail's own compilers, to the exact
bytes the retail game has at that function's address
([BUILD_FIDELITY.md](../BUILD_FIDELITY.md), "What matched means here").
So the C behaves 1:1 like the PS2 original, **including its bugs and
quirks**: uninitialised reads, odd float comparisons, off-by-one tables, the
behaviour speedrunners rely on. None of that is "fixed" here.

Consequences:

- Changes to matched code are type, name and comment changes only, unless
  they keep identical bytes. Proof is mechanical:
  `tools/overlay_file_check.py` for level code, `tools/build_sn.sh` for the
  executable (commands in [conventions.md](conventions.md#verifying-a-change)).
- A name, type or comment can be wrong; the instructions cannot. When a
  comment and the code disagree, the code is what retail does.
- **For the PC port:** preserve behaviour too. If you must change something
  (endianness, pointer width, hardware access), change the mechanism, not
  the observable result, and keep the change outside the matched C where
  you can.

## 1. The binary

`SCES_509.16` is a MIPS R5900 ELF. `config/splat.yaml` splits it into two
code segments:

| Segment | vram | What it holds | Sources |
|---|---|---|---|
| `core` | 0x100080 | VU microcode (`vutext`), then `core_text` from 0x112380: Sony SDK libraries, newlib, the 989snd sound library, libgcc, start-up and the main loop; plus `core_data`, `core_rdata`, `core_bss` | `src/core/` (53 files named by start address, e.g. `src/core/0012AC80.c`), `src/libgcc/` |
| `main` | 0x15ED80 | the game program: `core_lit`, `lit` (0x15F000), `bss`, `data`, the three level vtables (`lvl_vtbl`, `lvl_camvtbl`, `lvl_sndvtbl`) and `text` from 0x1E9080 | `src/game/` |

Object-to-address maps, in retail link order (the Makefile, linker script
and tools all read them):

- `config/core_text.objects`: one object per line with its start address.
  Boundaries come from retail's own `0xCDCDCDCD` linker fill. A third
  column `ee29` marks Sony SDK objects built with Sony's 2.9-ee instead of
  the game's SN GCC 2.95.3 (49 objects).
- `config/text.objects`: the game objects of `main`'s text, e.g.
  `build-sn/game/bmain.o 0x001E9808`, `build-sn/game/mobyutil.o 0x00213A78`.

Two compilers build the code ([TOOLCHAIN.md](../TOOLCHAIN.md),
[LEVERS.md](../LEVERS.md) "Two compilers"): SN Systems GCC 2.95.3 (build
v1.14) for game code, and Sony's 2.9-ee-991111 for SDK objects and libgcc.

### Original source file names

The game's `src/game/*.c` files carry the original translation-unit names.
Each file's header comment states it, e.g. `src/game/bmain.c`:

```c
/*
 * bmain.cpp in the original source; text 0x1E9808-0x1E9E70.
 * Name and boundary from the NTSC split of this game, mapped to PAL by matching function
 * sizes -- see docs/DECOMP_PROGRESS.md. Compiled as C for now.
 */
```

The original was C++ (`hud.cpp`, `loaders.cpp`, `pause.cpp`, `memcard.cpp`
...); this repo compiles it as C, which produces the same code for these
functions. Further evidence from strings and asserts (heap allocation line
numbers in `hud.cpp`/`loaders.cpp`, `/usr/local/989snd/ee/989snd.c`) is in
[DATAMINED_REVERSE_ENGINEERING.md](../DATAMINED_REVERSE_ENGINEERING.md).

The game units, in link order (`config/text.objects`):
actuator, bloaders, stub, bmain, transition, camera, effects, cmecpu,
collproc, draw, drawquad, fastfunc, framebuf, freeze, help, hud, init,
initonce, lights, loaders, map, menu, memcard, miscproc, missionfunc,
mobyfunc, mobyproc, mobyutil, music, stream, pad, partproc, partupd, pause,
shadowproc, shrubfunc, shrubproc, skyfunc, skyproc, sound, space, stash,
tfragfunc, vuchain, tfragproc, tiefunc, tieproc, update, vendor, then
`movie/*` (the MPEG player; see [MOVIE.md](../MOVIE.md) for why it is
special).

## 2. Level overlays

The executable's `main` segment is only the program that runs before the
first level loads. **Each of the 19 levels ships its own, much larger build
of the game program**, which replaces the whole `main` segment when the
level loads ([ASSETS.md](../ASSETS.md) "Code overlays",
[OVERLAYS.md](../OVERLAYS.md)). The `core` segment (SDK, libc, sound) stays
resident and is shared by every level.

A level's code section is seven records; `tools/overlays.py dump` writes
them to `baserom/overlays/level_NN/` with a `manifest.json` of addresses.
Level 00 (`baserom/overlays/level_00/manifest.json`):

| Record | Address | Bytes |
|---|---|---|
| `lit` | 0x15F000 | 11,968 |
| `bss` | 0x161F00 | 16,816 |
| `data` | 0x166100 | 542,000 |
| `vtbl` | 0x1EA680 | 1,284 |
| `camvtbl` | 0x1EAB00 | 160 |
| `sndvtbl` | 0x1EAC00 | 24 |
| `text` | 0x1EAD80 | 1,078,552 (~1.08 MB) |

`lit` starts at 0x15F000 in every level and `$gp` is 0x166D00 everywhere;
the other records are packed at each level's own sizes, so the same
function sits at different addresses in different levels. ParseBin
(`func_0012DA38`) copies the records and returns the entry point; the main
loop (`func_0012DB18`, `main` in `config/names.tsv`) calls it
([ASSETS.md](../ASSETS.md)).

### Level index to planet

A level is known by its index `NN` everywhere: `baserom/overlays/level_NN/`,
`func_LNN_*`, `D_LNN_*`, report category `level_NN`. Only the source
directory carries the planet. Full table and the evidence for the order:
[OVERLAYS.md](../OVERLAYS.md#levels).

| NN | Dir | NN | Dir | NN | Dir |
|---|---|---|---|---|---|
| 00 | `l00_veldin1` | 07 | `l07_umbris` | 14 | `l14_oltanis` |
| 01 | `l01_novalis` | 08 | `l08_batalia` | 15 | `l15_quartu` |
| 02 | `l02_aridia` | 09 | `l09_gaspar` | 16 | `l16_kalebo3` |
| 03 | `l03_kerwan` | 10 | `l10_orxon` | 17 | `l17_fleet` (Drek's Fleet) |
| 04 | `l04_eudora` | 11 | `l11_pokitaru` | 18 | `l18_veldin2` |
| 05 | `l05_rilgar` | 12 | `l12_hoven` | | |
| 06 | `l06_blarg` (Blarg Station) | 13 | `l13_gemlik` | | |

### Function kinds

`config/overlays/functions.tsv` catalogues every distinct function in the
19 level programs, deduplicated by instructions with address fields masked
(`identity()` in `tools/overlays.py`). Columns: name, kind, size,
fingerprint, level count, places (`level:address` for every copy).

| Kind | Count | Meaning | Name | C lives in |
|---|---|---|---|---|
| `exe` | 749 | same code as an executable function | `func_XXXXXXXX` | `src/game/` |
| `shared` | 1,963 | in two or more levels, not in the executable (1,159 of them in all 19) | `func_LNN_XXXXXXXX` | `src/overlays/shared/` (140 files) |
| `level` | 1,468 | in exactly one level | `func_LNN_XXXXXXXX` | `src/overlays/lNN_<planet>/` |

For `func_LNN_XXXXXXXX`, `NN` is the lowest-numbered level that has the
function (its *canonical level*) and `XXXXXXXX` its address there. A shared
function named `func_L04_...` simply does not occur in levels 00-03. Its
copies in other levels are at other addresses; the `places` column lists
them.

Overlay source files are named after the **executable unit they follow in
link order** plus their first function (`hud_00235960.c`), not after what
they do ([OVERLAYS.md](../OVERLAYS.md) "Plan" step 2). So
`src/overlays/shared/help_002180F8.c` holds hero code (it sits after the
`help` unit; 1.17 MB of level code lies between `help` and `hud`), and the
`vendor_*.c` files hold many moby update functions. The grouping is
provisional until original file boundaries are known.

### The dispatch tables: which function updates moby class N

Each level dispatches through three tables in its records
([OVERLAYS.md](../OVERLAYS.md#roles)):

| Record | Entry | Terminator |
|---|---|---|
| `vtbl` | 12 bytes: `{oClass, update function, pointer to a 6-word table}`, one per moby class in the level | oClass -1 |
| `camvtbl` | 20 bytes: `{camera id, init, activate, update, exit}` | id -1 |
| `sndvtbl` | 8 bytes: `{id, function}` | id -1 |

`tools/overlays.py names` writes `config/overlays/names.tsv` from these:
each function a table points at, with roles like `UpdateMoby_<oClass>`,
`InitCamera_<id>`, `ActivateCamera_<id>`, `UpdateCamera_<id>`,
`ExitCamera_<id>`, `SoundFunc_<id>`, each with the levels using it that way.
A function with dozens of roles is generic (e.g. `func_001E9768`, an empty
function in `src/game/stub.c`, used as many classes' "do nothing").

Recipe, with class 726 as the example:

```sh
grep -P '^726\t' tools/extract/moby_classes.tsv          # 726  novalis_elevator
grep -oP '^\S+(?=\t.*UpdateMoby_726:)' config/overlays/names.tsv   # func_L01_002BB068
grep -oP 'UpdateMoby_726:[0-9/]+' config/overlays/names.tsv      # levels 01/03/13
grep -rln func_L01_002BB068 src/overlays                         # shared/vendor_002B90A8.c
```

The update function receives the moby in `$a0` (`void f(Moby *)`). The
6-word per-class table is a set of class callbacks; some are documented in
comments (e.g. "slot 4 of the class tables of large_amoeboid (572)" in
`src/overlays/shared/vendor_002B90A8.c`). `tools/dossier.py` puts the role
in words into a worker's `CONTEXT.md`.

## 3. Frame and engine flow

Only what the evidence shows. Large parts of the top-level frame are still
being named.

### Boot and level start

- `main` (`func_0012DB18`, core) runs the outer loop and loads level code
  through ParseBin (`func_0012DA38`).
- `src/game/bmain.c`: `func_001E99D8` is the start-up sequence before the
  first level (clears level bss, intro and loading images, intro movie,
  sound bank, video mode, starts the level load); `func_001E9808(level)`
  starts a level from the table in `D_00137C80` (an alternate set while
  `D_0015EE80` is set), waits for the loader and loads it. Note:
  `config/names.tsv` calls `func_001E9808` `showDebugFont` (from the NTSC
  decomp's automated naming); the matched body says "start level".
- One-time set-up: `InitOnce` (`func_00201E88`, `src/game/initonce.c`),
  `LoadIRXModule` (`func_00201D58`, `src/game/init.c`), `LoadHudBanks`
  (`func_002032D0`, `src/game/loaders.c`).

### The moby update loop

`func_00213C78` (`MobyUpdateLoop`, candidate tier in `config/names.tsv`)
in `src/game/mobyutil.c`; still `INCLUDE_ASM`, so read
`asm/nonmatchings/text/func_00213C78.s`. Field names are `include/moby.h`'s:

```c
/* Equivalent C reading of func_00213C78 (not matched source). */
Moby *m = func_0020E6B8();              /* first moby; also stored at $gp-0x6CDC */
for (; m != NULL; m = m->next) {        /* next at 0x28 */
    if ((s8)m->state < 0) continue;     /* state at 0x20: >= 0x80 skipped, 0xFE = dead */
    if (!(m->flags & 0x40))             /* flags at 0x34 */
        func_0020E3D0(m);
    if (m->update)                      /* update at 0x74, from the level vtbl */
        m->update(m);
    if (!(m->flags & 4))
        func_0020ED48(m);
}
```

*(inferred)* `func_0020E3D0` and `func_0020ED48` are per-moby pre- and
post-update steps (animation and matrix work, going by the candidate name's
evidence "anim advance; update fn; matrix"); neither has a name yet.

Callers include the vendor-menu world update `func_L00_0029D988`
(`src/overlays/shared/update_0029B6A0.c`), which shows a typical sequence of
world update steps: `func_L00_00299108`, then the moby loop and
`func_0022EF68` (`enqueue_voice_request`, candidate), `func_L00_002076E8`, then `UpdateParts`
(`func_00218A80`), each gated by flags and separated by calls to
`func_001E9768`. That function is empty in retail (`src/game/stub.c`):
*(inferred)* the profiler timing hooks were compiled out of the shipped
build, leaving the labels below as strings only.

### Profiler stage labels

The draw and update stages appear, in this order, as profiler labels in the
executable's string table (`config/strings.tsv`, 0x00160D80-0x00160EA0):

1. render setup
2. sky texs, sky draw
3. tfrag texs, tfrag draw (terrain fragments)
4. tie texs, tie draw (instanced static geometry)
5. shrub texs, shrub draw
6. moby texs, moby draw
7. effects texs, vu effects, moby effects
8. part draw (particles)
9. post effects, screen overlays, aa blur
10. debug text

Update labels: `moby update`, `hero update`, `part update` at 0x00161148
in the executable; `camera update`, `sound update` and `system misc` exist
only in the level programs' `lit` record (level 00 at 0x1610E8, 0x1610F8,
0x15FD78). *(inferred)* The order is the frame's draw order; the labels are
what `DrawDebugProfiler` (`func_001F3D78`) prints.

Subsystems with readable names (`config/names.tsv`, `include/names.h`):

| Subsystem | Functions |
|---|---|
| Camera | `UpdateAllCameras` (`func_001EC7C8`), `UpdateCamera` (`func_001EDE50`), `camera_update` (`func_001ED080`, candidate), `Camera_*` helpers; `src/game/camera.c` |
| Draw | `drawNormalFrame` (`func_001F45F0`), `AddDrawCallback`, `ExecuteDrawCallbacks` (`func_001F4A00`) and variants 2-4, `DrawMobys` (`func_0020E2B0`), `DrawTfrag` (`func_002346C0`), `DrawTies_1/2`, `DrawShrubs`, `SkyDrawShell*`; `src/game/draw.c` and the `*proc.c` units |
| HUD | `HudDraw` (`func_001FFFB8`), `HudSprite`, `Hud_*`; `src/game/hud.c`; `HudFrame` (`func_L00_0023BAB8`) in level code |
| Mobys | `CreateMoby` (`func_0020D348`), `DeleteMoby` (`func_0020D678`), `InitMobyInstance`, `MobyAnimProc`; `src/game/mobyfunc.c`, `mobyproc.c`, `mobyutil.c`; `MobyProc` (`func_L00_002569F8`) in level code |
| Particles | `UpdateParts` (`func_00218A80`), `PartProc` (`func_00218B10`); `src/game/partproc.c`, `partupd.c`; `CreatePart2`, `KillPart` in level code |
| Sound/music | `sound_update` (`func_0022DD68`), `sound_*` in `src/game/sound.c`; `music_*` in `src/game/music.c`; `snd_*` (989snd) in `src/core/` |
| Pad | `UpdatePad` (`func_00218908`), `ClearPadInput`; `src/game/pad.c` |
| Save | `memcard_Save` (`func_0020BFC8`), `memcard_PrepData`, `memcard_Restore*`; `src/game/memcard.c`; `gGameSaveData` (`D_001A05C0`), `gLevelSaveData` (`D_001A08C0`) |
| Menus/pause | `DrawItemsMenu`, `DrawMapScreen`, `DrawCheatsMenu`, `DrawMissionsMenu`, `PauseAllSounds`; `src/game/pause.c`, `menu.c` |

### The hero and the pad

- **Hero** (Ratchet): one global `D_0013F450`, type `struct Hero` from
  `include/hero.h` (records `HeroMotion`, `HeroState`, `HeroItemSlot`,
  `HeroManipulator`, `HeroHealth` ...). `D_0013F4D0` (`gHeroPos`) is the
  hero position block.
- **Pad**: `D_0013CA40`, `struct PadState` (0x350 bytes) from
  `include/pad.h`: `held`, `pressed`, `released`, the `*_unmasked` copies
  menus read, stick axes in `analog[0..3]`.
- **Hero code is level code.** The biggest functions in the game are the
  per-level builds of the hero state machine, all matched C:

| Function | Size | File | Comment in source |
|---|---|---|---|
| `func_L00_002180F8` | 39,324 | `src/overlays/shared/help_002180F8.c` (6 levels) | per-frame movement and physics for the current hero state |
| `func_L17_00214F68` | 36,168 | `src/overlays/l17_fleet/help_00214F68.c` | same, level 17 build |
| `func_L16_00214C88` | 36,052 | `src/overlays/l16_kalebo3/help_00214C88.c` | same, level 16 build |
| `func_L18_0021F1D8` | 33,392 | `src/overlays/l18_veldin2/help_0021F1D8.c` | same, level 18 build |
| `func_L00_0022A340` | 33,312 | `src/overlays/shared/help_0022A340.c` (6 levels) | per-frame transition logic for the current hero state |

`HeroUpdate` (`func_L01_00228E08`) is in
`src/overlays/shared/help_002274A8.c`. Several levels carry their own build
of the hero code (the catalogue counts constants and struct offsets, so two
copies that differ in one number are two functions; [OVERLAYS.md](../OVERLAYS.md)
"Names"). A port must run the build that belongs to the current level.

## 4. Progress snapshot

From `progress/report.json` (regenerated 2026-10-18):

| Measure | Value |
|---|---|
| Functions matched | 4,805 / 5,118 (93.88%) |
| Code matched | 3,486,816 / 3,712,772 bytes (**93.91%**) |
| Executable (core + game + libgcc) | 1,601 / 1,689 functions, 89.62% of code |
| Level code | 3,204 / 3,429 functions, 94.53% of code |
| Shared level code | 1,778 / 1,980, 87.34% |
| Level-specific code | 1,426 / 1,449, 98.75% |

Levels 00, 02, 09, 11, 13, 16, 17 and 18 are at 100%.

Where the remaining work is:

- Still-assembly functions are `INCLUDE_ASM("asm/...", func_X)` stubs in
  their source files (retail `.s` under `asm/`, generated locally by
  `tools/setup_asm.sh`, never committed).
- Near misses (compile, close, not exact) are staged one per file in
  `nonmatching/<dir>/`, listed closest first in `nonmatching/README.md`
  ([NONMATCHING.md](../NONMATCHING.md)). `nonmatching/functional.tsv` and
  `nonmatching/functional/` hold C written for the native port only: it
  does what the retail function does without matching its bytes, and the
  matching build never reads it.
- Original assembly: `config/handwritten_asm.txt` (245 entries, including
  33 VU0 helpers in `fastfunc.o` added 2026-10-17) and
  `config/linker_remnants.txt`. These are included with `ASM_FUNC` /
  `LINKER_REMNANT` and count as finished
  ([ASM_CLASSIFICATION.md](../ASM_CLASSIFICATION.md)).
- `func_00213C78` (the moby loop above) is itself still assembly.

## 5. Where do I look for X

| Looking for | Start here |
|---|---|
| Update function of moby class N | `config/overlays/names.tsv` role `UpdateMoby_N`, class names in `tools/extract/moby_classes.tsv` (recipe in [section 2](#the-dispatch-tables-which-function-updates-moby-class-n)) |
| A moby's private state | `Moby.pvars` (0x78) cast to the class's struct in the update function's file; [PVARS.md](../PVARS.md) |
| Moby lifecycle | `CreateMoby` / `DeleteMoby` / `InitMobyInstance` in `src/game/mobyfunc.c`; loop `func_00213C78` in `src/game/mobyutil.c` |
| Hero movement and states | `src/overlays/shared/help_002180F8.c`, `help_0022A340.c` and their per-level copies (`src/overlays/l1[678]_*/help_*.c`); `include/hero.h` |
| Controller input | `src/game/pad.c`, `include/pad.h` (`D_0013CA40`) |
| HUD | `src/game/hud.c` (`HudDraw`), banks in `src/game/loaders.c` (`LoadHudBanks`); level `HudFrame` |
| Camera | `src/game/camera.c` (`UpdateAllCameras`); per-level cameras via `camvtbl` roles `InitCamera_<id>` / `UpdateCamera_<id>` in `config/overlays/names.tsv`; `src/overlays/shared/camera_*.c` |
| Sound and music | `src/game/sound.c`, `src/game/music.c`, level `sndvtbl` roles `SoundFunc_<id>`; 989snd library in `src/core/` |
| Save / memory card | `src/game/memcard.c`; card states and save chunk ids in [DATAMINED_REVERSE_ENGINEERING.md](../DATAMINED_REVERSE_ENGINEERING.md) |
| Menus and pause | `src/game/pause.c` (`Draw*Menu`), `src/game/menu.c` |
| Particles | `src/game/partproc.c`, `src/game/partupd.c`; level `CreatePart2` / `KillPart` |
| Rendering | `src/game/draw.c` (`drawNormalFrame`), `tfragproc.c`, `tieproc.c`, `shrubproc.c`, `skyproc.c`, `src/game/vuchain.c` |
| Level loading | `src/game/bmain.c` (`func_001E9808`, `func_001E99D8`), `src/game/loaders.c`, ParseBin in `src/core/`; on-disc layout in [ASSETS.md](../ASSETS.md) |
| A symbol's readable name | `python3 tools/names.py func_001EC7C8` or `... UpdateAllCameras` ([NAMES.md](../NAMES.md)) |
| Which file holds function X | `grep -rln func_X src/`; for level code, `config/overlays/functions.tsv` gives kind and places |
| Retail assembly of X | `asm/nonmatchings/{text,core_text}/func_X.s`, `asm/overlays/func_LNN_X.s` (after `tools/setup_asm.sh`) |
| Strings | `config/strings.tsv` (address, symbol, xrefs, value) |

## Further reading

- [OVERLAYS.md](../OVERLAYS.md): catalogue, layout, roles, joined functions.
- [ASSETS.md](../ASSETS.md): disc and level file formats, code overlay records.
- [NAMES.md](../NAMES.md): naming table, tiers and sources.
- [TOOLCHAIN.md](../TOOLCHAIN.md), [BUILD_FIDELITY.md](../BUILD_FIDELITY.md): compilers and what the build may do.
- [WORKFLOW.md](../WORKFLOW.md): set-up, iterate, verify, publish.
- [NONMATCHING.md](../NONMATCHING.md), [ASM_CLASSIFICATION.md](../ASM_CLASSIFICATION.md): what is left and what stays assembly.
