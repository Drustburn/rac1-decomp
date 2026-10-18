# Datamined findings

What the retail data of Ratchet & Clank (PAL, SCES-50916) says about how the
game was built, and what it contains that players never see. Everything here is
documentation: this repository is a byte-exact decompilation, its behaviour is
1:1 with retail (speedrun tricks depend on that), and nothing below is meant to
change any code.

Earlier, broader notes (source file list, `CardState` enum, save chunk ids, hub
functions) are in [`docs/DATAMINED_REVERSE_ENGINEERING.md`](../DATAMINED_REVERSE_ENGINEERING.md).
This page does not repeat them; [section 12](#12-corrections-to-datamined_reverse_engineeringmd)
lists corrections to that page.

**Evidence labels** used on every finding:

- **verified**: read directly from retail bytes (`baserom/`) or from matched C
  that compiles to those bytes.
- **inferred**: a reasonable reading of verified facts, not proven.
- **untested in game**: derived statically; nobody has tried it on hardware or
  an emulator.

Addresses: `func_L00_*` / `D_L00_*` are level-00 addresses. Shared overlay code
sits at a different address in each level; `config/overlays/functions.tsv`
gives every level's address. Strings from `config/strings.tsv` in the ranges
0x15F000-0x161FFF and 0x165580-0x1E8FFF are addresses in the **executable's own
copy** of the main segment. Each level overlay has its own layout, so the same
string is somewhere else at run time (for example the debug-menu strings are
0x40 lower in every level's `lit.bin`, and the memory-map strings are at
0x1E8B50 in level 00 against 0x1E8080 in the ELF). The xref/sample-function
columns of `strings.tsv` for those ranges are unreliable for the same reason.

## Summary

1. **Hidden 20-button input code system** (verified code, untested in game).
   While the pause/page menu runs with exactly R2+L1 held, the game records 20
   presses of D-pad/Square/Circle and checks them against 145 codes generated
   from one 256-byte table. 91 of them do something: they give any of 37 items,
   unlock any of the 18 planets, set 6 special-item flags, or award any of the
   30 skill points. All 91 sequences are decoded in [section 1](#1-hidden-button-code-system).
2. **The "I-5" debug menu is still in every level** (verified). Game mode -1 of
   the main loop runs `func_L00_001F4918` / `func_L00_001F5C60`, which draw a menu
   titled `I-5` with UPDATE / MODE / DRAW / AMMO / FOG / MISC pages (Invinc,
   Drawdist, profiler bars, collision views, fog editor, occlusion sampling). A
   static scan found no retail code that stores -1 into the game-mode word
   (inferred: it can only be reached by poking memory).
3. **`printf` is an empty stub in retail** (verified): `STUB_printf`
   (`func_001E9730`, `src/game/stub.c`) has no body. All diagnostics,
   including a full `*** MEMORY MAP ***` report printed after every level load,
   are still formatted by the code and then thrown away.
4. **Development environment**: the project code `I5`/`I-5` (menu title,
   `cdrom0:\CODE\I5\PARAM.TXT`, `host0:z:/i5/levels/...`), levels authored in
   Maya ("maya instance" in designer errors), original header `walk.h`, the
   developer spelling `Ratchett`, and SDK release 2.5.x libraries (inferred
   readings of verified strings).
5. **Region and save-folder name come from `SYSTEM.CNF`** at boot (verified):
   `InitOnce` reads LBA 0x121 (`SYSTEM.CNF`), takes PAL/NTSC from the `VMODE`
   line and builds the memory-card folder `BESCES-50916RATCHET` from the boot
   file name.
6. **Designer error strings name about 25 moby class ids** with their in-house
   names (Footsoldier 28, hoverboat 557, wind-control 610, MineBot 923/1401,
   Flying Car 1417, Equilizer Ammo Spawner 1142, ...), see [section 6](#6-moby-class-ids-named-by-strings).

---

## 1. Hidden button-code system

**Where:** `func_L00_00284410` in `src/overlays/shared/pause_00277208.c`
(matched C, 512 bytes, identical in all 19 levels). It is called once per
frame from the page-menu update `func_L00_00277A88` (same file, call at line
356). The page menu is case 3 of the main loop's game-mode switch in
`func_L00_002465F8` (`src/overlays/shared/map_002465F8.c`), the pause / map /
planet-select screens. **verified**

### 1.1 Input

The function reads the controller state `D_0013CA40` (`D_0013A5E0 + 0x2460`,
`struct PadState` in `include/pad.h`):

- `held_unmasked` (0x1C0) `& 0xF` must equal `6`. With the DualShock 2 button
  word that `func_002181F0_r` in `src/game/pad.c` builds
  (`((buf[0] << 8) | buf[1]) ^ 0xFFFF`), the low nibble is L2 0x1, R2 0x2,
  L1 0x4, R1 0x8, so `6` means **R2 and L1 held, L2 and R1 released**.
  The bit meanings come from the standard SCE pad data layout (**inferred**);
  the directions are confirmed by `pad.c`, which ORs the left stick into
  0x8000/0x2000 (x) and 0x1000/0x4000 (y).
- each frame, `pressed_unmasked` (0x1C4) `& 0xF0A0` is mapped to a symbol:
  Up 0x1000 → 0, Down 0x4000 → 1, Left 0x8000 → 2, Right 0x2000 → 3,
  Square 0x80 → 4, otherwise (Circle 0x20) → 5. If several are pressed in the
  same frame, the first in that order wins. Triangle and Cross are ignored.
- `pressed` includes left-stick directions (pad.c ORs them into `held`), so a
  stick flick also counts as a D-pad press. The `_unmasked` copies have the
  mirror-mode left/right swap undone, so the code is always entered with the
  physical buttons, mirrored levels or not. **verified (code)**
- symbols go into the 20-entry buffer `D_L00_001BA838` (`short[20]`); the
  count `D_L00_001604B8` is reset to 0 **only when the R2+L1 chord is not
  held**. After 20 presses no further input is taken, so each chord hold gets
  exactly one attempt. **verified**

### 1.2 Matching

When the 20th symbol arrives, the function tries `index = 2 .. 0x92`. Code
`index` matches if `buf[j] == T[((j + 1) * index) & 0xFF]` for all 20 `j`,
where `T` is the 256-byte table `D_L00_001B9F20` (values 0-5). The first match
wins and `code = index - 2`. The table is byte-identical in all 19 level
overlays (at 0x1B9F20 in levels 00/07, 0x1BA320 in 01/10/12/16, and so on).
Of the 145 generated sequences, four (codes 0x7D, 0x7F, 0x88, 0x8C) repeat a
lower code and can never match, but all of them are in the range that does
nothing. **verified**

### 1.3 Effects

| Codes | Effect | Target (verified) | Meaning |
|---|---|---|---|
| 0x00-0x24 | `D_0013D5C8[c] = 1; D_0013D5F0[c] = 1` | 37-byte per-item arrays | item owned / item obtained (**verified**: `GiveItem` `func_L00_002618D8` in `mobyutil_00258BC8.c` sets the same two arrays; `func_0020C940` in `src/game/missionfunc.c` tests them as condition types 2 and 3) |
| 0x25-0x36 | `UnlockPlanet(c - 0x24)` (`func_L00_00261848`) | `D_0013DE48[1..18]` + `gGalaxyList` | planets 1-18, Novalis to Veldin (return) (**verified** call; planet order from `docs/OVERLAYS.md`) |
| 0x37-0x3C | `gSpecialItems[c - 0x37] = 1` (`D_0013D490`) | first 6 special-item flags | index 0 is the Zoomerator in the memory map that `config/names.tsv` cites (**inferred** for 1-5) |
| 0x3D-0x5A | if `gSkillPoints[c - 0x3D] == 0`: set it, `func_0022EE28(1,0,0)`, `ShowBanner(0x53DB, -1)` | 30 skill-point flags (`D_0013D510`) | the same three steps the game uses for every real skill-point award (e.g. `l08_batalia/vendor_002EAF48.c:1338`, `shared/help_00237B00.c:1469`) (**verified**) |
| 0x5B-0x90 | nothing | | |

Notes:

- The item codes write the raw flags only. `GiveItem` also shows the pick-up
  banner, sets starting ammo (`D_0013D530`), adds weapons to the quick-select
  list (`D_00141FA0`) and equips the item. The codes do none of that.
  **verified (code comparison)**
- Item names, by id. † marks ids pinned by `config/names.tsv` memory-map
  anchors (`gHaveHeliPack` 0x13D5CA = id 2, `gHaveMorphORay` id 0x15,
  `gHaveMagneboots` id 0x1C, `gHaveHologuise` id 0x1F, `gHaveMapOMatic` id
  0x21, `gHavePersuader` id 0x23). The other names follow the usual RaC1
  item-id order (**inferred**). Two tables in level 00 data support that order:
  the item-type field of `D_L00_00179BC0` (stride 0x4C: ids 2-4 type 3 =
  back, 5-7 type 2 = head, 0x1C-0x1E type 1 = feet, 8-0x20 type 0 = hand)
  and the ammo table `D_L00_001C43B0`, where exactly the 11 ammo weapons
  (0xA, 0xB, 0xD, 0xF, 0x10, 0x11, 0x13, 0x14, 0x17, 0x18, 0x19) have
  nonzero entries. The pick-up banner text ids `D_L00_001B06C0[id]` look like
  `level × 1000 + n` (3015 Heli-Pack → Kerwan, 11012 Thruster-Pack → Pokitaru,
  12006 Hydro-Pack → Hoven, 10011 Magneboots → Orxon, 5014 R.Y.N.O. → Rilgar),
  which fits where those items are found (**inferred**).
- This is probably a QA/debug unlock system that was left in, not a
  player-facing cheat. It shares nothing with the real cheat flags `gCheats`
  (`D_0015EEB0`) or the save's cheat blocks (**inferred**). It has **not been
  tried in game**. Entering one of these codes would also change save data
  (the flags are saved), so try it on a throwaway save.

### 1.4 All 91 working codes

Hold **R2 + L1** (and not L2/R1) in the pause menu, then press the 20 buttons
in order. ↑↓←→ = D-pad (or left stick), □ = Square, ○ = Circle. Derived by
static analysis, **untested in game**.

| Code | Index | Buttons (hold R2+L1, then press) | Effect |
|---|---|---|---|
| 0x00 | 2 | `□ ○ ← ← → → → □ ↓ □ ↑ ↑ ↓ ← → → ○ ↑ ○ ↑` | item 0x00 (id 0, no item type): owned + obtained flags |
| 0x01 | 3 | `↓ ← ↓ → ← ↓ ↓ ↑ ↑ → □ ↑ □ ↑ ○ ↓ ↑ □ ← ←` | item 0x01 (id 1, back-slot item, unidentified): owned + obtained flags |
| 0x02 | 4 | `○ ← → □ □ ↑ ← → ↑ ↑ ↑ ↓ ↓ → ← ↓ ○ ↑ → ←` | item 0x02 Heli-Pack †: owned + obtained flags |
| 0x03 | 5 | `□ → ← □ □ → □ ↑ ○ ○ ← ← ↑ ↑ ○ ← → ↑ → □` | item 0x03 Thruster-Pack: owned + obtained flags |
| 0x04 | 6 | `← → ↓ ↑ → ↑ ↑ ↓ □ ← → ↑ → ↑ ↑ ↓ ↑ ↓ ○ ↓` | item 0x04 Hydro-Pack: owned + obtained flags |
| 0x05 | 7 | `↓ → ↓ ← □ ↑ → → ↓ ↑ → ↑ ↓ ↑ → → ← □ □ ↓` | item 0x05 Sonic Summoner: owned + obtained flags |
| 0x06 | 8 | `← □ ↑ → ↑ ↓ → ↓ ↑ ← ← ↓ ← → ↓ → □ □ → ○` | item 0x06 O2 Mask: owned + obtained flags |
| 0x07 | 9 | `↓ ↓ ↑ ↑ ○ □ ↓ ↑ ↓ ↑ ← ↓ ○ □ ↓ □ → ○ ↑ ↓` | item 0x07 Pilot's Helmet: owned + obtained flags |
| 0x08 | 10 | `→ □ → ↑ ○ ← ↑ ← ↑ □ □ ↓ ↓ ↓ ↑ ○ ↑ ↓ □ →` | item 0x08 OmniWrench: owned + obtained flags |
| 0x09 | 11 | `← ↑ □ ↑ ← → → ← ← □ ↑ ↓ ← ○ □ ○ ↑ ← ← ○` | item 0x09 Suck Cannon: owned + obtained flags |
| 0x0a | 12 | `→ ↑ ↑ ↓ ← ↑ ↑ ↓ ↓ ↓ ↓ □ ↑ ← ↓ ← ← ↑ ↓ ←` | item 0x0a Bomb Glove: owned + obtained flags |
| 0x0b | 13 | `↓ ↓ □ ↓ ↑ → ↓ ← ○ ↓ ← ↑ ○ ○ ← ↓ ← → ↓ ○` | item 0x0b Devastator: owned + obtained flags |
| 0x0c | 14 | `→ ← ↑ → ↑ ↑ ↑ → □ ↓ ○ ← ○ ↓ □ → ↓ ← → ↑` | item 0x0c Swingshot: owned + obtained flags |
| 0x0d | 15 | `← → ○ ← ○ ↑ → ↓ ↓ ↑ □ ↓ ← □ ↑ ← → → ○ ↑` | item 0x0d Visibomb Gun: owned + obtained flags |
| 0x0e | 16 | `□ → ↓ ↓ ← ↓ → → □ ○ ○ ← ↓ → ← → □ → ↓ ↓` | item 0x0e Taunter: owned + obtained flags |
| 0x0f | 17 | `→ ○ ↑ ○ → ↑ ← □ → ↑ ↑ ← ← ↓ → □ □ ○ □ ↑` | item 0x0f Blaster: owned + obtained flags |
| 0x10 | 18 | `↓ ↑ □ ↑ ↑ ↓ □ □ ○ ↓ ← ↑ → ← → → ○ ○ ← ←` | item 0x10 Pyrocitor: owned + obtained flags |
| 0x11 | 19 | `→ ○ ← → → ○ □ → ↑ □ ← ↓ ↓ → ○ ↓ □ ← → □` | item 0x11 Mine Glove: owned + obtained flags |
| 0x12 | 20 | `□ ↑ ← ← □ ↓ ↓ ○ ↓ → ○ ← ○ ↑ ↑ ↓ ↑ ← □ □` | item 0x12 Walloper: owned + obtained flags |
| 0x13 | 21 | `↓ ↑ ↓ ↑ → □ ← ← ○ □ ○ ← → ○ ↑ ← ○ ← ← □` | item 0x13 Tesla Claw: owned + obtained flags |
| 0x14 | 22 | `↑ ↑ → ← □ ↓ ○ ○ ← ○ → ← → ↓ ○ ↓ ← ↓ ○ □` | item 0x14 Glove of Doom: owned + obtained flags |
| 0x15 | 23 | `↓ □ □ ↑ ○ → ↑ □ ← → ○ □ ↓ → ↓ → ↓ → → ←` | item 0x15 Morph-o-Ray †: owned + obtained flags |
| 0x16 | 24 | `↑ ↓ ↑ ↓ ↓ □ ← ← ↑ ← ← → → ← ← → → ○ → →` | item 0x16 Hydrodisplacer: owned + obtained flags |
| 0x17 | 25 | `□ ○ ○ □ □ ↑ □ → ↑ □ → ↑ □ ← ← □ ○ ↑ ← ○` | item 0x17 R.Y.N.O.: owned + obtained flags |
| 0x18 | 26 | `↓ ↓ → ← ↓ ↑ ○ ↓ → ○ → → ↑ ↓ ↑ ○ → ↑ ↓ ←` | item 0x18 Drone Device: owned + obtained flags |
| 0x19 | 27 | `↑ □ ↓ ↓ ↓ ○ ○ ↑ ← → ← ○ → ← □ ○ ↑ → ↓ ←` | item 0x19 Decoy Glove: owned + obtained flags |
| 0x1a | 28 | `← → ↑ → ↓ ← ↓ → ← ↑ ↓ ← ↓ □ □ ← ○ → □ ↓` | item 0x1a Trespasser: owned + obtained flags |
| 0x1b | 29 | `○ → → ○ ↓ ↓ ↑ → □ ○ ↓ ↑ ↑ ↑ □ ↓ → → □ ○` | item 0x1b Metal Detector: owned + obtained flags |
| 0x1c | 30 | `→ ← ↑ ↓ ↑ ↓ □ ← → ↑ ○ ← ↑ □ ↑ → ○ ← → ←` | item 0x1c Magneboots †: owned + obtained flags |
| 0x1d | 31 | `○ ↓ → □ ↑ → → → ↓ □ → ○ ← □ ← ← ← □ → ↓` | item 0x1d Grindboots: owned + obtained flags |
| 0x1e | 32 | `→ ↓ ↓ → ○ ← → → → ↓ ↓ → ○ ← → → → ↓ ↓ →` | item 0x1e Hoverboard: owned + obtained flags |
| 0x1f | 33 | `□ → ← ↓ □ ← ○ ← ← ○ ← ↓ → ○ → □ → ↑ ○ ↑` | item 0x1f Hologuise †: owned + obtained flags |
| 0x20 | 34 | `○ ○ ↑ □ ↑ ← ↓ □ ○ ↑ ← → → ○ ○ → → □ ↑ ←` | item 0x20 PDA: owned + obtained flags |
| 0x21 | 35 | `□ ↑ → ↓ □ □ □ ↑ ↑ ← → □ → → ↓ ↓ ↓ ← → ←` | item 0x21 Map-o-Matic †: owned + obtained flags |
| 0x22 | 36 | `↑ ↑ ↓ □ ↓ ↑ ← → ○ ← ↓ ○ ↑ → ← ↓ □ □ ↓ ↓` | item 0x22 Bolt Grabber: owned + obtained flags |
| 0x23 | 37 | `□ ○ ← ↑ → ↓ ↓ ↑ → ○ ↓ ← ↑ ← ↓ ← ○ ○ → ↓` | item 0x23 Persuader †: owned + obtained flags |
| 0x24 | 38 | `○ → ○ → □ ↓ → ↓ ← □ ○ → ↓ □ → ↓ ↑ ↓ □ →` | item 0x24 (id 0x24, unidentified): owned + obtained flags |
| 0x25 | 39 | `□ → ○ ↑ ← → → → → ↑ → ↑ ↑ ○ ← → ↓ □ □ →` | UnlockPlanet(1) — Novalis |
| 0x26 | 40 | `↑ ← ↓ ○ → ← ↑ ↓ ← □ □ → ← ↓ ← → ← ↓ → →` | UnlockPlanet(2) — Aridia |
| 0x27 | 41 | `← ↑ → □ ↓ □ ○ ↑ ↑ ○ ← ← ↓ ↓ □ □ → → ← ↓` | UnlockPlanet(3) — Kerwan |
| 0x28 | 42 | `↑ ↑ □ ← □ ← ○ ← ← □ ○ → ○ → ← ○ ○ ○ → ↑` | UnlockPlanet(4) — Eudora |
| 0x29 | 43 | `↓ ← → ↓ ← □ ○ ← ↓ ↓ → ○ → ↑ □ ○ ← ← → ↑` | UnlockPlanet(5) — Rilgar |
| 0x2a | 44 | `↑ ← ↓ ○ ○ ← ↓ ↓ ↓ □ ↓ □ ← ← ↑ ← ← ↑ ○ →` | UnlockPlanet(6) — Blarg Station |
| 0x2b | 45 | `○ ↑ ↓ ↓ ↑ → ↑ ← □ ↑ → ← ← ← □ ↓ ○ ↑ → ↓` | UnlockPlanet(7) — Umbris |
| 0x2c | 46 | `□ ↑ → □ → □ → → → ← □ ↑ ← ↓ □ → → ← ↑ →` | UnlockPlanet(8) — Batalia |
| 0x2d | 47 | `→ ← □ ← □ ↓ ← ↓ ○ → □ ↓ ← ↑ ↑ ← ○ → □ ↓` | UnlockPlanet(9) — Gaspar |
| 0x2e | 48 | `↓ ↓ □ ← ← → ← → ○ → □ ↓ → ○ ↓ → ↓ ↓ □ ←` | UnlockPlanet(10) — Orxon |
| 0x2f | 49 | `→ ↑ ← ↓ □ ○ → □ → → ↑ → □ ↓ ↓ □ ↑ ○ □ ↑` | UnlockPlanet(11) — Pokitaru |
| 0x30 | 50 | `○ □ ↑ → □ ↑ ← □ ↑ ○ ○ ← → ← ↓ → ↑ ↓ ○ →` | UnlockPlanet(12) — Hoven |
| 0x31 | 51 | `↑ ↑ → ← → ○ ○ → ↑ ○ → □ ↓ ○ ○ ↓ ← ↑ ↑ ←` | UnlockPlanet(13) — Gemlik Base |
| 0x32 | 52 | `↓ ← ↑ ↓ ○ → ↓ ○ ↑ ← ← → □ ↑ → ↓ ○ ← ○ □` | UnlockPlanet(14) — Oltanis |
| 0x33 | 53 | `○ ↑ ↓ ↑ ↓ ↓ ○ ← ← ↓ ↓ □ ↑ → ↑ ← □ → → ↑` | UnlockPlanet(15) — Quartu |
| 0x34 | 54 | `□ ↓ ○ ↑ → ○ ← ○ → ← ↑ □ □ ○ ↑ ↓ ↑ ← □ →` | UnlockPlanet(16) — Kalebo III |
| 0x35 | 55 | `← □ □ ○ → ○ → □ → ○ → ↑ ↑ □ ← → ○ ↓ ↓ →` | UnlockPlanet(17) — Drek's Fleet |
| 0x36 | 56 | `→ → ← → ↑ ← □ ← → ↓ ← ○ ↑ □ ↑ → □ ← ↑ ↓` | UnlockPlanet(18) — Veldin (return) |
| 0x37 | 57 | `← ○ ↑ ↓ ○ ← ← → ↓ → ○ ↓ □ → → □ ↑ □ ↑ ○` | special-item flag 0 (Zoomerator †) |
| 0x38 | 58 | `→ ○ ↓ → ○ ↑ ↑ ↓ → ○ □ □ → ↑ ↑ ○ ↓ □ → □` | special-item flag 1 |
| 0x39 | 59 | `↑ ← ↑ ← □ ↑ ○ ↑ → → ○ ↓ → → ○ ○ □ ○ ↓ ↑` | special-item flag 2 |
| 0x3a | 60 | `← ↓ ↓ ← ↑ ← □ → ← ← ↑ ↓ → ↑ ↓ ← ← → ○ ○` | special-item flag 3 |
| 0x3b | 61 | `→ ← ↑ ○ → □ ↑ → □ ↑ ↓ ○ □ ← ← ↓ ↓ ○ ↓ ↓` | special-item flag 4 |
| 0x3c | 62 | `↓ □ → → □ ○ □ ← □ ↓ ↑ → ○ □ ○ → → ↑ ○ ↑` | special-item flag 5 |
| 0x3d | 63 | `↓ □ ○ ← ↑ ← → → ← ← → ○ ↑ ○ ↑ ← → □ → ←` | skill point 0 (Take Aim †) |
| 0x3e | 64 | `↓ → ← → ↓ → ← → ↓ → ← → ↓ → ← → ↓ → ← →` | skill point 1 |
| 0x3f | 65 | `↑ ↓ ← ○ □ ↑ → ← ← → ↑ → → □ ← □ ↓ ↑ → □` | skill point 2 |
| 0x40 | 66 | `→ ↓ ← ← ○ ↓ ○ □ ↑ ↑ → ↑ ↑ ↑ ↓ → ↑ □ → ↑` | skill point 3 |
| 0x41 | 67 | `□ ↑ ↑ → ↓ ↑ ○ ↑ ↓ → ↑ ↑ □ ↑ → ↓ ○ ○ □ ←` | skill point 4 |
| 0x42 | 68 | `○ □ ← □ ↑ → ○ → □ ← ← ↓ ○ □ ← ↓ ↓ → → ←` | skill point 5 |
| 0x43 | 69 | `□ → ← □ ↓ → ↓ ↑ ○ □ ↓ ← → ← ← ← □ ↓ ○ □` | skill point 6 |
| 0x44 | 70 | `↑ ↓ □ ↑ ← □ → ↓ ← ← □ ↑ □ ↑ ↓ ↓ □ ← ○ ↓` | skill point 7 |
| 0x45 | 71 | `↓ □ ○ ← ← ↑ → → → ← ↓ ↑ ↑ → ← → ↑ ○ □ ↓` | skill point 8 |
| 0x46 | 72 | `↑ □ ↑ → ← ○ → ↓ □ ↓ ↑ ↓ ← ← → → → □ ← ○` | skill point 9 |
| 0x47 | 73 | `← ↑ ← ↑ ○ ○ → ↑ ↓ ↓ □ ↓ → ○ ↓ □ → ○ ← ↓` | skill point 10 |
| 0x48 | 74 | `○ ↑ ↓ ↑ ○ ← ← ← ○ ↓ □ ↓ ↑ → ← ○ → ↓ □ →` | skill point 11 |
| 0x49 | 75 | `○ ↑ ↑ ↑ ← ↑ ↓ ← □ ↓ ← ↓ ← ↓ ○ ○ ↑ ↑ ↓ ○` | skill point 12 |
| 0x4a | 76 | `→ → ↓ ↓ □ → □ ↓ ↓ → ○ □ ○ ↑ ○ ← → ← □ ←` | skill point 13 |
| 0x4b | 77 | `→ ○ ○ ↓ → ○ ↑ ← → □ ↓ ↑ □ □ ↓ ↓ ○ ↑ ↑ ○` | skill point 14 |
| 0x4c | 78 | `→ ↑ → → ↑ ↑ ○ → □ → ↑ ← □ ○ ↑ → □ □ ○ ↑` | skill point 15 |
| 0x4d | 79 | `↓ → → ← ↑ ↓ ← ↓ → ↑ ○ ↓ ↓ ↑ ↑ ← ↓ □ ← ↑` | skill point 16 |
| 0x4e | 80 | `← ○ ← ↓ □ → ↓ → ↓ → → ← □ ↓ ○ → ← ○ ← ↓` | skill point 17 |
| 0x4f | 81 | `↓ ○ ← ○ □ → ← □ → ↑ → ← ○ □ → □ ↓ □ ↓ ↑` | skill point 18 |
| 0x50 | 82 | `↑ □ □ ↑ ○ ← ↓ □ → ↓ ↑ ↑ ↑ □ ○ → ○ ↓ ↑ ←` | skill point 19 |
| 0x51 | 83 | `↓ □ □ → ↓ → □ → □ ↓ ↓ ↓ ← → ← ↓ ↓ → ← □` | skill point 20 |
| 0x52 | 84 | `↑ ← ← ← □ → → ○ ○ ↑ ↑ ← ○ → ← ↓ ↑ → ← □` | skill point 21 |
| 0x53 | 85 | `→ ↑ → ↑ ○ ○ ↓ ← ○ ↑ ○ ← ↓ □ ↑ ← □ □ ↓ □` | skill point 22 |
| 0x54 | 86 | `← ↓ □ ← ↓ ○ ↑ ○ ← ↑ □ ← ← ↓ → ↓ ○ → ↑ □` | skill point 23 |
| 0x55 | 87 | `→ ↓ □ ↑ □ → ↓ □ ← ↑ ○ □ ← ↑ □ → → → ○ ←` | skill point 24 |
| 0x56 | 88 | `← ○ ← ↓ □ □ ← ← ↑ → → → ↓ ↓ ↑ → ↑ ↓ □ →` | skill point 25 |
| 0x57 | 89 | `↓ □ ← □ ○ ↑ ← → □ ← → ↑ □ ↓ ← □ □ → ↑ ○` | skill point 26 |
| 0x58 | 90 | `↑ ↓ → ← ↑ ← ← ↓ ↑ ↓ ↓ → ↑ ← ↑ ○ □ ↑ ↓ ←` | skill point 27 |
| 0x59 | 91 | `↓ ○ → ↓ → ○ □ ↑ ↑ □ □ ○ ↓ □ → ○ ← ↑ ↑ ←` | skill point 28 |
| 0x5a | 92 | `↑ □ □ → ← ↑ ↓ → ← → ○ ← ↓ ← □ ← ← ↓ ↑ ↓` | skill point 29 |

Reproduce it with this script (`python3 -I`, from the repo root):

```python
import json
m = json.load(open('baserom/overlays/level_00/manifest.json'))
d = next(r for r in m['records'] if r['name'] == 'data')
T = open('baserom/overlays/level_00/data.bin', 'rb').read()[0x1B9F20 - d['address']:][:256]
for index in range(2, 0x93):
    print(index - 2, ''.join('UDLRSC'[T[((j + 1) * index) & 0xFF]] for j in range(20)))
```

---

## 2. The "I-5" debug menu (game mode -1)

**verified:**

- The main loop `func_L00_002465F8` switches on the game-mode word
  `D_L00_0015F6A8` (`gGameMode`). `case -1` calls `func_00216270`,
  `func_L00_001F4918` (4932 bytes) and `func_L00_001F5C60` (4884 bytes). Both
  functions are in all 19 levels. Their names in `config/names.tsv`,
  `DebugCameraUpdate` / `DebugCameraRender`, are candidates.
- `func_L00_001F5C60` (matched C, `src/overlays/shared/draw_001F3A78.c:523`)
  draws the menu. It has 71 lui/addiu references into the level `lit` strings
  0x15F108-0x15F3B8 and `func_L00_001F4918` has 3 more. Nothing in the
  executable references these strings, which is why `strings.tsv` lists them
  with 0 xrefs. The title, centred at (0x100, 0x10), is `I-5`.
- Menu state: `debug_menu60` = `D_L00_0016C158`, initialised by the main loop
  on every level start (`map_002465F8.c`: fields 0x8 = 15, 0x38 = -1, ...).

The pages and their rows, from the draw code and the value tables in level 00
data (row → value strings):

| Page | Rows (value strings) |
|---|---|
| UPDATE | Ratchett, Mobys, Parts, Camera, Step (`off`/`on`, bits of `draw_bits`) |
| MODE | Control (`normal`/`camera`/`cam+chr`/`volume`), Profile (`off`/`bars`/`render`/`mobys`/...), Occl (`off`/`freeze`/`active`), Bookmark, Invinc (`off`/`on`), Drawdist (`normal`/`near`), Coll (`hotspt`/`camera`/`sound`), Tv Mode (`ntsc`/`pal`), Screen (`normal`/`small`/...), Scene (`%d`/`none`), Segment (`%d = %d`) |
| DRAW | Darken, Env Coll, Moby Coll, Hero Coll, Bspheres, Cameras, Cuboids, Samples, Text |
| AMMO | SuckCannon, Morpha-Ray, Inflato R, R.Y.N.O., D. Droids, Bomb, Devastator, Visibomb, Agents, Pyrocitor, Tesla Claw, Mine Glove, Blaster, Bolts |
| FOG | NearDst, NearInt, FarDst, FarInt, Color_R/G/B (`%.02f`) |
| MISC | BattleCam, Actuator, AddSample, AddStart |

Value tables: `D_L00_00160CD8` (off/on), `D_L00_00160D00` (control modes),
`D_L00_00160D68`, `D_L00_00160D80`, `D_L00_00160DA0`, `D_L00_00160DB0`,
`D_L00_00160DC8`, `D_L00_001C4128` (profiler modes).

Observations:

- The AMMO page uses development weapon names: `Morpha-Ray` (retail
  Morph-o-Ray), `Inflato R`, `D. Droids` (Drone Device, **inferred**),
  `Agents` (Decoy Glove? **inferred**). **verified** strings.
- `AddSample` / `AddStart` and `occ_samp.dat` / `occ_sample_deltas.dat`
  (referenced from `func_L00_001F4918`, two and one code references in every
  level) point to an in-game tool that recorded occlusion sample points
  (**inferred**). `TJB - No env sample point found!` (`func_L00_002510F0`) is
  the run-time lookup into those samples (**inferred**).
- A five-entry pointer table at level-00 data `0x16C140` lists the sound
  groups `    sfx`, `  music`, ` dialog`, `ambient`, `  shell` (padded to
  seven characters, which looks like a volume-mixer column header,
  **inferred**). No lui/addiu reference to that table was found.
- **Reachability (inferred):** a scan of all 19 level texts for stores to
  `0x15F6A8` found constants 0, 1, 3, 4, 5, 6, 7 and values restored from a
  saved-mode field (`func_001FD3E8`'s level copy at L00 0x202E48, and
  `func_L00_00299B68`). No store of the constant -1 turned up. The ELF's own
  main segment has no store either. `config/names.tsv` cites a TCRF page for
  "0xFFFFFFFF: Debug Menu", which fits entering it by poking memory. The menu
  code is live and kept up to date (it reads the current state structures),
  but no button path to it was found. **untested in game**

---

## 3. Debug leftovers

### 3.1 `printf` is a stub

`func_001E9730` (`STUB_printf`) is `void f(int, ...) {}` in `src/game/stub.c`
(matched). It has 22 callers in the ELF alone, and every level calls it from
the memory-map report, `Transition to level %d` (`func_L00_00277A88`),
`loaded irx modules`, `Unsupported language %d`, and others. In emulator
terms, a breakpoint on `func_001E9730` shows every diagnostic the developers
saw (**inferred**, untested).

### 3.2 Diagnostics and where they are used

Code references were counted in each level (lui/addiu pairs into that level's
copy of the string) and in the ELF. "19/19" means referenced in every level.

| String (verbatim) | Referenced from | Status |
|---|---|---|
| `\n*** MEMORY MAP ***\n\n` + 22 lines | `func_L00_00244AE0` (loaders), 19/19 | live, printed into the stub |
| `TJB - No env sample point found!\n` | `func_L00_002510F0`, 19/19 | live |
| `Camera_CollPrimTest WARNING! - grid out of bounds! (RAR)\n` | 19/19 | live |
| `!!!! Too many directional lights !!!! - Lighting will look bogus as a penalty.\n` | 19/19 + ELF | live |
| `Tried to create moby and couldn't. Time: %d, oClass: %d\n` | `func_0020D348` copy, 19/19 | live |
| `vu chain overflow - mobys dropped\n` | `func_0020E2B0` copy, 19/19 | live |
| `Why doesn't this work?` | `func_00235218` copy, 19/19 + ELF | live |
| `Paradox: This message does not exist` | `func_001FE540` (33 callers), 19/19 + ELF | live: fallback text for a missing message id (**inferred**) |
| `Transition to level %d\n` | `func_L00_00277A88`, 19/19 | live |
| `main loop squirm! should never get here!` | 19/19 | live |
| `camera update`, `sound update`, `system misc` | 19/19 | profiler bar labels (**inferred**) |
| `sizeof(w_2legData) = %d bytes` | level 04 (Eudora) only | live in one level |
| `assertion "%s" failed: file "%s", line %d\n` | none, ELF 0x1E7A90 | dead (library `__assert` format, **inferred**) |
| `cdrom0:\CODE\I5\PARAM.TXT;1` | none | dead |
| `host0:z:/i5/levels/level%d/npcs/scene_%d/scene.dat`, `scenep.dat` | none in 18 levels; level 07 shows numeric matches that look coincidental (not confirmed) | dead (**inferred**) |
| `*** frame %d ***\n`, `tried to draw path -1 \n` | none | dead |

### 3.3 File/line literals

No `__FILE__`/`__LINE__` pair from an assert survives. The only source-file
literals still used by code are `hud.cpp`, `loaders.cpp` and `map.cpp` (each
referenced in every level; level 14 has five `map.cpp` references). They are
passed with line numbers to the heap allocator, see
`DATAMINED_REVERSE_ENGINEERING.md` §1. `/usr/local/989snd/ee/989snd.c` comes
from the sound library. **verified**

---

## 4. Development environment

- **Project code "I5"** (**inferred**): the debug-menu title `I-5`,
  `cdrom0:\CODE\I5\PARAM.TXT;1` and `host0:z:/i5/levels/level%d/...`. A
  `host0:` path is a file on the development PC served through the devkit, and
  `z:` was a mapped drive on it. These paths are dead in retail (section 3.2).
- **Maya** (**inferred**): designer errors say `maya instance` (e.g.
  `Moby class %d maya instance %d needs a rocketPath!`, level 01;
  `Moby Maya Index %d deleted.`, level 10), so placed instances were
  identified by their Maya scene index.
- **Original names** (**verified** strings): header `walk.h` and function
  `walk_init2LegAnim` (`Invalid anim detected on pLD->anims[%d]->anim in
  walk_init2LegAnim.\nSee walk.h and check your initialization function for
  moby class %d`, all levels), struct `w_2legData`, state `SUCKSTATE_DEATH`,
  function `M774_ShipChallengeDone`, `MB_Sucking`, `Camera_CollPrimTest`.
- **"Ratchett"** with two t's is the in-house spelling: debug row `Ratchett`,
  memory-map line `ratchett seqs:`. **verified**
- Designer strings are signed with tags (`- RAR`, `TJB -`) and one asks the
  reader to `Please tell Jason you saw this.` (levels 01 and 08). They are
  quoted here verbatim, and this page makes no claim about who wrote them.
- **VU microcode as DVP overlays** (**verified**): the ELF has 43
  `.DVP.overlay..<vu addr>.<id>.<line>.<n>` sections, 12 groups of 1-8 chunks
  of up to 0x800 bytes. `.DVP.ovlytab` gives each chunk's EE address in
  `.vutext` (0x100080, 0x12290 bytes) and its VU address. That is the
  `dvp-as` overlay mechanism for VU1 micro memory. The `<id>` and `<line>`
  fields look like a source hash and a source line (**inferred**).
- **ELF layout** (**verified**, `readelf -S`): resident sections are named
  `core.text`, `core.data`, `core.rdata`, `core.bss` and `core.lit`. The
  replaceable main segment is `.lit`, `.bss`, `.data`, `lvl.vtbl`,
  `lvl.camvtbl`, `lvl.sndvtbl` and `.text`. The ELF's own `.text` is only
  0x556B0 bytes and `lvl.vtbl` only 0xC bytes, against about 1.1 MB of text
  and about 0x800 bytes of vtbl in each level, so the ELF carries a reduced
  boot program (**inferred**). There is no `.mdebug` or other debug section.

---

## 5. Per-level strings and NPC names

Strings that appear in one or a few level overlays and not in the ELF.
Leading stray bytes from the extraction (`?`, `@`, `=`, `A`) are dropped, and
random-looking float data is left out. Presence in an overlay means the code
is linked into that level, not necessarily that the NPC appears there.
**verified** (strings), level names from `docs/OVERLAYS.md`.

| Level | Names and notable strings |
|---|---|
| 00 Veldin | none of its own. The overlay has only 11 level-unique functions (8 KB), against 42-136 for the other levels |
| 01 Novalis | `Big Turret Guy`, `Water Pump Worker`, `Mystery 774`, `?mouse already completed`, `WARNING! - Moby 774 wasn't expecting a M774_ShipChallengeDone call.  Please tell Jason you saw this.` |
| 02 Aridia | `Surfer Agent`, `McGuffin Surfer`, `Shark with no path. UID: %d`, `Ship with no path. UID: %d` |
| 03 Kerwan | `Helga`, `train end path has no STOP NODE!!!\n  Bailing out of the end cutscene before the train goes out of control and kills us all!`, `train has no master!`, `train engine has no cars!` |
| 04 Eudora | `Moby 1120 %d has no segment switch!`, `moby 427, instance %d requires both path fields to run!!! DELETING MOBY FROM LEVEL!!`, `sizeof(w_2legData)` (only referenced here) |
| 05 Rilgar | `Bouncer`, `Race Girl`, `Shifty Salesman` |
| 06 Blarg | `Scrawny Scientist` |
| 07 Umbris | `ERROR! Moby 1142 Equilizer Ammo Spawner has no trigger cuboid!`, moby 1059 and 871 errors, `hey, shouldn't be in init!` |
| 08 Batalia | `Big Turret Guy`, `Commando`, `Deserter`, `Water Pump Worker`, `Water Worker`, `Mystery 774`, `Paratrooper with an invalid jumpPath. UID = %d`, `M671 Warning: texture w count (%f) is greater than max texture tile count (8)` |
| 09 Gaspar | `Bigship moby %d (oClass:%d) can't find parent! dist:%f`, `Floating ship piece UID:%d oclass:%d missing trigger moby! Will just float there.`, `Kamikaze with an invalid boundPath.` |
| 10 Orxon | `Moby947 maya instance %d HAS NO PATH!!!\n\n\nMoron.\nMobyDeleted`, `moby class 1067 REQUIRES a link to dataProcessor class 1073!!`, `Moby Class %d Instance %d has no pVars!  May be in the art file.` |
| 11 Pokitaru | `Get Pilot Hat!`, `Raritanium Acquired`, `I'm really sorry, but you can't have more than 128 water tiles in this level` |
| 12 Hoven | `Raritanium Acquired`, the 128 water tiles message |
| 13 Gemlik Base | `tractor off screen, abort`, `tractor too long, abort`, `25s min adj`, `moby:%d(%d) opened at %d` / `closed` / `failed` |
| 14 Oltanis | `Captain Qwark`, `Scrap Merchant`, Footsoldier, hoverboat, wind-control, Enemy Fighter, Flying Car, MineBot and Hovertank errors |
| 15 Quartu | Footsoldier errors, `Get Pilot Hat!` |
| 16 Kalebo III | `MineBot (moby1401) ...`, `Kamikaze with an invalid boundPath.` |
| 17 Drek's Fleet | `magnacut cuboid error: detect:%d hero:%d init:%d end:%d`, `no final place for hero during victory, so just fall` |
| 18 Veldin (return) | none of note |

In every level: `Already got pilot helmet... deleting.`,
`?SUCKSTATE_DEATH: should not be called any more!`,
`Hey, M186 init should be earlier!`,
`Moby 1290 %d needs a cut scene!`, `MB_Sucking: Tried to suck a NULL pointer!`,
`cleaned up moby:%d (oclass:%d)`, `jumpdist = %f`,
`mpeg memory alloc error (%d bytes)`.

---

## 6. Moby class ids named by strings

The designer error strings pair class ids with names (**verified** strings;
the identification is the string's own wording). `docs/PVARS.md` already
names 774 as the Plumber (Novalis).

| oClass | Name in string | Level(s) |
|---|---|---|
| 6 | (trigger user) `No trigger for moby 6` | 13 |
| 28 | Footsoldier (`bound/fly/intro path`, `hatch index`) | 14, 15 |
| 111 | `no path for moby 111` | 13 |
| 186 | `M186 init should be earlier` | all |
| 427 | needs both path fields | 04 |
| 557 | hoverboat | 14 |
| 610 | wind-control (cuboids: wind pushing hero area, fxAct, fxArea) | 14 |
| 671 | `M671`: tiled texture (max 8 tiles) | 08 |
| 774 | `Mystery 774`, `M774_ShipChallengeDone` | 01, 08 |
| 794 | needs an effect moby | 10 |
| 871 | mine group controller | 07 |
| 899 | path data | 03 |
| 908, 921 | Enemy Fighter | 14 |
| 923, 1401 | MineBot | 14, 16 |
| 947 | needs a path (`Moron.`) | 10 |
| 1059 | bounding path, hero-detect path, guard-point cuboid, water moby | 07 |
| 1067 → 1073 | requires a link to `dataProcessor` class 1073 | 10 |
| 1120, 1290 | need a segment switch (1290 also a cut scene) | 04 / all |
| 1142 | Equilizer Ammo Spawner (sic) | 07 |
| 1181 | doubly linked chain (`could not link next/prev`) | 09 |
| 1417 | Flying Car | 14 |
| 1885 | respawner (`could not find a moby to respawn`) | 09, 13 |

Strings that name kinds without class ids: `Hovertank`, `Turret`,
`Paratrooper`, `Kamikaze`, `Shark`, `Ship`, `Bigship`, `train`.

---

## 7. Memory budget (the MEMORY MAP report)

`func_L00_00244AE0` (loaders, all levels, still `INCLUDE_ASM`) is called by
the main loop with `(1, 0)` right after each level load, and with `(0, full)`
on quit. When the first argument is nonzero it prints, through the stub
`printf` (**verified**):

```
*** MEMORY MAP ***
code mem / dead space / vu chain bufs / gadget buffer / tfrag geom (or "unknown")
occlusion / collision / shared vram / particle vram / effects vram
mobys / ties / shrubs / ratchett seqs / tie insts / shrub insts / moby insts
moby pvars / paths / part insts
end of mem: %.1fM
estimated mem: %.1fM
```

All lines but the last two are `%.1fK`. The order is how a level's memory is
laid out after load (**inferred**). The labels name the engine's asset
classes: tfrags (terrain fragments), ties, shrubs, mobys, the VU chain
buffers, a dedicated gadget buffer and "ratchett seqs" (the hero's animation
sequences). Hooking the stub would print the real numbers per level
(**untested**).

---

## 8. Libraries and versions

**verified** (version tags in the ELF):

| Tag | Library |
|---|---|
| `PsIIlibkernl2540`, `PsIIlibmc   2540` | kernel, memory card (2.5.4) |
| `PsIIlibcdvd 2530` | CD/DVD (2.5.3) |
| `PsIIlibdma`, `libgraph`, `libipu`, `libmpeg`, `libpad2`, `libdbc`, `libscf`, `libvib` (`2500`) | 2.5.0 libraries |
| `/usr/local/989snd/ee/989snd.c` | 989 Studios sound library (EE side, RPC to the IOP) |
| `cdrom0:\IOPRP243.IMG;1` | IOP replacement image loaded by `InitOnce` (IOP 2.4.3, **inferred** from the name) |

The pad stack is libpad2 with libdbc (`scePad2CreateSocket`, see `pad.h`) and
libvib, not the older libpad (**inferred** from the tags plus `pad.h`).

---

## 9. File system, region and language

- **Boot reads `SYSTEM.CNF`** (**verified**): `InitOnce` (`func_00201E88`,
  `src/game/initonce.c`) reads LBA 0x121 into a buffer. On this disc that
  sector is `BOOT2 = cdrom0:\SCES_509.16;1\r\nVER = 2.00\r\nVMODE = PAL`.
  Byte 0x33 (`P` of `VMODE = PAL`) sets the PAL flag
  `D_0015EE80 = D_0015EF90 = (buf[0x33] != 'N')`. `func_00209A60`
  (`src/game/memcard.c`) then fills the template `/BA****-*****RATCHET`: byte
  0x12 (`E` of `SCES`) replaces the `A` (`P` gives `I`), and the product
  letters and digits are copied in. The PAL save folder is therefore
  `BESCES-50916RATCHET`, holding `icon.sys`, `static.ico` and `save%d.bin`.
- **IRX loading quirk** (**verified**, matched C): `InitOnce` calls
  `LoadIRXModule` on table entries 0x1C, 0x1E, 0x20, 0x22, 0x24, **0x26 twice**,
  0x2A, 0x2C, then 0x28. One module is loaded twice and 0x28 is loaded last.
- **Language** (**verified** code, **inferred** library identity):
  `func_0012D380` reads the OSD config word (bits 16-20, or bit 4 when
  bits 13-15 are zero), which is how libscf's `sceScfGetLanguage` behaves. `InitOnce` maps the
  result into `D_0015EE88`: English 1 → 0, French 2 → 2, Spanish 3 → 4,
  German 4 → 3, Italian 5 → 5. Anything else (Japanese, Dutch, Portuguese)
  prints `Unsupported language %d\n` and falls back to English 0. Per-language
  tables have six slots and are indexed with `language - 1` clamped to 0
  (`src/game/space.c:1842`).
- **Movies** (**verified** names; pointer references in every level's data):
  `\DEMOLOGO.NTS`, `\INFOBOT2.PSS` to `\INFOBOT4.PSS`, `\REPORTER.PSS;1`,
  `\INFOBOT7.PSS;1`, and the four PAL language variants
  `\IBOT5_F/G/S/I.PSS;1` (French/German/Spanish/Italian, **inferred** from the
  letters). See `docs/MOVIE.md`.
- **Level data** is opened as `cdrom0:\DATA\LEVELS\LEVEL` + number (3 code
  references per level).

---

## 10. Unused code

A rough scan (scratch script, not in the repo) marked every function in
`config/overlays/functions.tsv` that has, in no level, a `jal`, a lui/addiu
or lui/ori pair (400-instruction window), or a 32-bit word anywhere in that
level's lit/data/vtbl/text or in the resident ELF code pointing at its
address. **319 of 4180** functions (33 KB) came out unreferenced: 241 shared,
51 level-only, 27 exe-kind. This is an **upper bound, not a list of dead
code**:

- The level's entry point (`func_L00_002465F8`, the main loop, reached through
  `manifest.json` `entry_point`) and `InitOnce` both show up as false
  positives.
- Moby update functions (e.g. the 5448-byte Batalia turret-bolt update
  `func_L08_002E4BE8`) are probably bound through class tables that the level
  loads from disc (**inferred**).
- Odd-aligned entries such as `func_L00_001F080C` are catalogue boundary
  fragments (see `nonmatching/shared/func_L00_001F8750.c` for one).

Real dead code needs a per-function check. Verified dead **data** is easier
to show: see section 3.2 (`PARAM.TXT`, the assert format, `host0` paths, frame
markers).

Functions per level (`functions.tsv`, all kinds / level-unique / unique
bytes): level 00 2336/11/8 KB is the smallest. The largest are 14 Oltanis
2556/136/151 KB, 18 Veldin (return) 2489/127/156 KB and 08 Batalia
2480/86/156 KB.

---

## 11. Open questions

- What sets `gGameMode` to -1 in development builds (a pad combo stripped
  from retail?), and does anything in the current PAL build still do it
  indirectly through the saved-mode field?
- The names of item ids 1 and 0x24, special items 1-5 and skill points 1-29
  (needs the help-text archive, which is not in the overlays).
- Whether the code system works in game as derived (pad 0 only, while
  `unk18C` is zero).

---

## 12. Corrections to `DATAMINED_REVERSE_ENGINEERING.md`

- **"RaC1 Gadget Inventory Enum (`RaC1GadgetId`)"**: these 29 values do not
  match the retail item ids. The ownership array `D_0013D5C8` and `GiveItem`
  use ids 0-0x24 with Heli-Pack = 2, Morph-o-Ray = 0x15, Magneboots = 0x1C,
  Hologuise = 0x1F, Persuader = 0x23 (section 1.3). The enum in
  `include/structs.h` is used by no source file.
- **"Recovered from unstripped debug types"**: the retail ELF has
  no `.mdebug` or any other debug section (section 4), so the `MobyInstance`
  layout there does not come from this executable's debug data. Treat its
  field names as reconstructed. `include/moby.h` is the reference.
- **`camera.c` (0x001E7A50)**: no string `camera.c` exists. That row is the
  `Camera_CollPrimTest` warning, and the file name is inferred. `hud.cpp`,
  `loaders.cpp` and `map.cpp` do exist as strings.
- The string addresses there are ELF-image addresses. In a running level the
  same strings are elsewhere (see the note at the top of this page).
