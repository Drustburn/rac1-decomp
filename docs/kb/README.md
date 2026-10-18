# Knowledge base

A developer's guide to the game code of Ratchet & Clank (PS2, PAL, SCES-50916) as this
decompilation sees it: how the program is laid out, the records everything passes around,
how mobys (game objects) live and die, the naming and declaration conventions, and what the
retail data reveals about the game's development.

It is written for two audiences: contributors who are about to read or match code, and the
developers of the native PC port that imports this C.

## The one rule

This repository is a **byte-exact matching decompilation**. Every function written in C
compiles with the original SN Systems GCC 2.95 toolchain to the retail bytes, so the game
behaves 1:1 with the PS2 original, bugs and quirks included. Speedrun routes depend on exactly
those quirks. Nothing in this knowledge base is a proposal to change behaviour; changes in
`src/` are type, name and comment changes that keep identical bytes, verified with
`tools/overlay_file_check.py` (level code) and `tools/build_sn.sh` (the executable). A port
has to preserve the same behaviour.

## Pages

| Page | What it covers |
|---|---|
| [architecture.md](architecture.md) | The executable's segments and source layout, the 19 level overlays, the per-level dispatch tables (which function updates moby class N), boot and level start, the moby update loop, profiler stages, progress, and a "where do I look for X" table |
| [conventions.md](conventions.md) | Symbol naming, `names.tsv`/`names.h`, per-file aliases and the declarations that steer codegen (`MACRO_ADDR`, `NOT_SDA`, `SDATA`), shared headers and the remaining private views, comments and provenance, verification commands |
| [data-structures.md](data-structures.md) | Offset-accurate field tables for `Moby`, `MobyClass`, `PadState`, the hero records; vector types; pad button bits and input semantics; important globals |
| [mobys.md](mobys.md) | Moby lifecycle (class registration, spawning, update loop, death and respawn), save ids, pvars, animation fields, the class-to-update-function tables, and moby class ids named by retail debug strings |
| [datamining.md](datamining.md) | Findings from the retail executable and level data: debug-menu and profiler leftovers, development paths, per-level NPC names and designer error messages, a hidden button-code decoder, the engine's memory budget, SDK versions |

Older, more detailed documents stay where they are and the pages link to them:
[OVERLAYS.md](../OVERLAYS.md), [ASSETS.md](../ASSETS.md), [NAMES.md](../NAMES.md),
[TOOLCHAIN.md](../TOOLCHAIN.md), [BUILD_FIDELITY.md](../BUILD_FIDELITY.md),
[LEVERS.md](../LEVERS.md), [PVARS.md](../PVARS.md),
[DATAMINED_REVERSE_ENGINEERING.md](../DATAMINED_REVERSE_ENGINEERING.md).

## Known inconsistencies found while writing this

These were found while checking the pages against the code. They are not fixed yet:

- `docs/PVARS.md` and `MobyInstance` in `include/structs.h` put the moby's update pointer,
  pvars and oClass at 0xA8 / 0xAC / 0xBC. The matched code and `include/moby.h` use
  0x74 / 0x78 / 0xA6. Use `moby.h`. PVARS.md's level numbers are also off (Pokitaru is 11).
- Some header comments carry US addresses: `include/hero.h` names the hero `D_0013F350`
  (PAL: `D_0013F450`), and `include/pad.h` names the mirror flag `D_0015EDB4`
  (PAL: `D_0015EEB4`).
- `config/names.tsv` names `func_001E9808` `showDebugFont`, but its matched body starts a level
  (`src/game/bmain.c`).
- `docs/ASM_CLASSIFICATION.md` counts 212 handwritten functions; `config/handwritten_asm.txt`
  lists 245 (the 33 VU0 helpers added on 2026-10-17 are not counted).
- `docs/DATAMINED_REVERSE_ENGINEERING.md` calls the camera unit `camera.c` (no such string
  exists; the header of `src/game/camera.c` says the original was `camera.cpp`), its
  `RaC1GadgetId` enum does not match retail item ids, and its `MobyInstance` layout cannot come
  from debug symbols: this ELF has no debug sections. See [datamining.md](datamining.md) §12.
- String addresses in `config/strings.tsv` for the literal/data ranges are ELF-image
  addresses; each level lays those records out differently, so the xref columns there are
  unreliable.

## Sourcing

Everything here comes from this repository: the matched C, the headers, `config/` tables,
the generated asm, and the retail data in `baserom/`. Statements are marked as verified or
inferred where it matters. The third-party US decompilation is referred to as "the NTSC
decomp". Provenance notes in source comments ("Adapted from ... (MIT)") are licence credit and
stay.
