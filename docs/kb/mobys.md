# Mobys

A **moby** is the game's object instance: Ratchet's model, enemies,
bolts, crates, gadgets and their projectiles, platforms, effects and
invisible level logic are all mobys. This page covers what the engine
does with them. Field tables for `Moby` and `MobyClass` are in
[data-structures.md](data-structures.md#moby-includemobyh).

**This is a byte-exact decompilation of the PAL release.** Everything
here describes what retail code does. A native port has to keep it 1:1:
the slot reuse delay, the update order (list order) and the
pre-pass/post-pass flag tests are all observable in-game, and speedrun
tricks depend on them.

## The record

- **Instance:** `struct Moby`, 0x100 bytes (`include/moby.h`). Key
  fields: `pos` 0x10, `state` 0x20, `pclass` 0x24, `next` 0x28,
  `update_dist`/`visible`/`draw_dist` 0x30-0x33, `flags` 0x34,
  `occlusion` 0x36, `spawn_frame` 0x38, `rot` 0x40, animation 0x50-0x70,
  `update` 0x74, `pvars` 0x78, `oclass` 0xA6, `mission` 0xB0, `save_id`
  0xB2, `parent` 0xB8.
- **Class header:** `struct MobyClass`, reached through `pclass`. It is
  the class's data record from the level's class table (`D_001B3580[slot]`)
  and holds the model, collision, default scale/flags, animation
  sequences (`seqs[]` at +0x48) and the 6-word `callbacks` table at +0x2C.
- **Class id:** `oclass` (s16 at 0xA6) is RaC1's own object-class number,
  the one in debug strings ("Moby 28(Footsoldier)") and in the
  `UpdateMoby_<oClass>` roles. `class_slot` (byte 0x22) is the per-level
  slot that class was registered in. Code maps between the two with
  `D_001B3E40[oClass]` (slot, 0xFF = class not loaded) and
  `D_001B3C80[slot]` (oClass).
- **Do not take offsets from `MobyInstance` in `include/structs.h` or
  from `docs/PVARS.md`.** They put update/pVar/oClass at 0xA8/0xAC/0xBC,
  which the matched code contradicts (0x74/0x78/0xA6). The *pvars class
  layouts* in `include/moby_pvars.h` are a separate matter. They come from
  outside tooling and are unverified per class, so check each one against
  the asm before you rely on it.

## Memory: the moby array and pvars

(`src/game/mobyfunc.c:141-200`)

- All mobys live in one array of 0x100-byte slots. `CreateMoby`
  searches `D_0016001C` .. `D_00160020`. `D_00160018` is the base used to
  compute a moby's slot index (`InitMobyInstance` stores `idx << 16` at
  0xA8 and `idx` at 0xAC). Slots below `D_0016001C` exist (`DeleteMoby`
  treats them differently, see below). They are likely the
  level-placed instances (inferred).
- Each slot owns **0x80 bytes of pvars** at `D_00160028 + idx * 0x80`.
  `CreateMoby` points `moby->pvars` there and zeroes it. pvars are the
  class's private state (path pointers, timers, sub-state, ...). See
  `docs/PVARS.md` for the per-class layout catalogue, with the offset
  caveat above. Level code often reads pvars through a private struct
  cast, e.g. `KillFlags`/`VoiceSlot` in
  `src/overlays/shared/vendor_00298BB8.c`.

## Lifecycle

### Registering a class (level load)

`register_moby_class` (`func_00203E78`, `src/game/loaders.c:973`) takes
the class data, two more arguments and the oClass. It:

1. Puts the class in the next slot `D_00160000`: oClass -> slot in
   `D_001B3E40`, slot -> oClass in `D_001B3C80`, slot -> data in
   `D_001B3580`.
2. Calls `func_00213BB8` (`assign_moby_class_data`,
   `src/game/mobyutil.c:175`), which looks the oClass up in the
   `{key, a, b}` table `D_001E8F00` (key -1 ends it). It stores `a` (the
   update function) in `D_001B3900[slot]` and `b` (the 6-word table) at
   class data +0x2C (`MobyClass.callbacks`).
3. Bumps the slot count. With non-null class data it also keeps its +0x2C
   word in `D_001B6500[slot]` and calls `MobyClassRelocate`
   (`func_00203B70`) to fix up the class's pointers.

### Spawning

`CreateMoby` (`func_0020D348(oClass)`, `src/game/mobyfunc.c:188`):

- Takes the first slot with `state >= 0xFE` **and**
  `D_0015F6F0 >= spawn_frame`. `D_0015F6F0` is the frame clock. A freed
  slot is not reusable until its `spawn_frame`.
- `0xFE` is a free slot. `0xFF` marks the free tail: taking the tail slot
  writes `0xFF` into the next slot's state, so the mark moves up.
- Calls `InitMobyInstance`, sets and zeroes `pvars`, and decrements
  `D_0015FFFC` if it is nonzero. On failure it prints "... Time: %d,
  oClass: %d" and returns 0.

`InitMobyInstance` (`func_0020D440(moby, oClass)`, `:295`) clears all
0x100 bytes, then sets:

- `alpha = 0x80`, `class_slot = D_001B3E40[oClass]`, `oclass = oClass`.
- `group`, `hit_slot`, `unk71`, `unk72`, `unk7C` and `sound_slot` are all
  0xFF ("none").
- `occlusion = 0x7F80` and the 64-bit word at 0x38 = `0x0040404000000000`.
- The bytes at 0xA0..0xA3 are 0x7F, 0x7F, 0x80, 0x80.
- `update = D_001B3900[slot]`. When that is 0 it sets flag 0x2.
- From the class header, if there is one: `pclass`, `unk72` (class +0x0E),
  `flags |= class.flags`, `collision`, `scale`, `anim_time` 1.0 and
  0x5C 1.0. A glow colour sets flag 0x10. A shadow (class +0x0F) sets
  `shadow_range = 0x18` and flag 0x400. Class +0x06 sets `unk73 = 0x18`.
  Then the sequence rules after `update_moby_animation_state`
  (`func_0020D6D0`).
- With no class header: `pclass = 0`, `flags |= 5`, `collision = 0`.

The 64-bit store at 0x38 suggests that 0x38 holds more than a single
`u64 spawn_frame`: the 0x40 bytes land at 0x3D-0x3F. That is inference;
`DeleteMoby` overwrites the whole 8 bytes.

Spawner helpers then fill in the per-use fields. The canonical example
is `func_00226720` (names.tsv: `spawn_hand_gadget_moby`;
`src/game/pause.c:8919`). It returns 0 when `D_001B3E40[oClass] == 0xFF`
(class not in this level). Otherwise it creates the moby and sets
`update_dist = 0xFF`, `draw_dist = 0xFF`, `state = 0` and `visible = 1`.
It then runs the post pass `func_0020ED48` once, tints the moby
(`func_0020E340(o, 0x202020, ...)`), and sets byte 0x73 = 0x18 when class
+0x06 is set. Spawners set `update_dist`/`draw_dist` to 0x7F for beams
(moby.h).

### Deleting and respawning

- `DeleteMoby` (`func_0020D678`, `:383`) sets `state = 0xFE`, or `0xFD`
  for a slot below `D_0016001C`, sets `spawn_frame = D_0015F6F0 + 2` and
  calls `func_0020EA70(m, 0x80807F7F)`. The two-frame delay means a just
  freed slot cannot be handed out again in the same or the next frame.
- `delete_moby` (`func_002267C0`) calls `DeleteMoby` and then sets
  `spawn_frame = D_0015F6F0`, which makes the slot reusable at once.
- State `>= 0xFE` is "dead, waiting to respawn" (moby.h). Level classes
  that respawn (e.g. class 1885, "Moby 1885 UID:%d could not find a moby
  to respawn at time %d!") build on `spawn_frame` and the slot states.

### States

`state` (byte 0x20) is the class's own state machine for values below
0x80. The engine reserves the top values:

| state | meaning |
|---|---|
| 0x00..0x7F | alive; meaning is per class (update functions `switch` on it) |
| >= 0x80 | skipped by the update loop (signed `lb` + `bltz`) |
| 0xFD | deleted, slot below the dynamic pool |
| 0xFE | free / dead; reusable once `D_0015F6F0 >= spawn_frame` |
| 0xFF | free tail mark of the pool |

## The per-frame update loop

`MobyUpdateLoop` = `func_00213C78` (`asm/nonmatchings/text/func_00213C78.s`,
0x94 bytes, not matched yet):

```c
/* Reconstruction of the asm, not matched C. */
for (m = func_0020E6B8(); m != NULL; m = m->next) {   /* head also saved in a $gp global */
    if ((s8)m->state < 0) continue;                    /* state >= 0x80 */
    if (!(m->flags & 0x40)) func_0020E3D0(m);          /* pre pass */
    if (m->update) m->update(m);                       /* the class's update, moby in $a0 */
    if (!(m->flags & 0x4)) func_0020ED48(m);           /* post pass */
}
```

- `func_0020E6B8` (handwritten asm) builds the frame's list and returns
  its head. It walks the array up to the 0xFF tail, skips flag 0x2 (no
  update function), and tests each moby's distance to the point at
  `D_00187040 + 0x140` against `update_dist` (0xFF = always). It then
  links survivors through `next`, using per-class-slot and per-group
  buckets in scratchpad RAM (0x70000000). This is inferred from reading
  `asm/handwritten/text/func_0020E6B8.s`. The exact ordering rules have
  not been written up as C.
- `func_0020E3D0` (pre) and `func_0020ED48` (post) are handwritten
  animation/matrix passes. Both read `frame`/`prev_frame`/`seq`/`prev_seq`
  at 0x50-0x53, and the post pass also reads `unk71` and 0xA8. Their
  exact jobs are inferred, not documented.
- The loop is also called from the pause code
  (`src/overlays/shared/pause_00277208.c:436`, `src/game/pause.c`), so
  some menus keep mobys ticking.
- Order matters: a moby updates in list order, and anything it changes
  in another moby is visible to mobys later in the list in the same
  frame.

### Distances and visibility

- `update_dist` (0x30): the update radius. 0xFF means always update.
  Spawners use 0xFF, or 0x7F for beams.
- `draw_dist` (0x32, s16) and `visible` (0x31, "drawn last frame"; set
  to 1 by spawners) drive drawing (`DrawMobys` `func_0020E2B0`,
  `DrawMobyList` `func_0020E180`). `InitMobyClassDists` /
  `StashMobyClassDists` / `RestoreMobyClassDists`
  (`func_0020E040`/`068`/`098`) manage per-class distances.
- `occlusion` (0x36): occlusion bits, 0x7F80 by default.
- `shadow_range` (0x7F): in units of 1024.

### Save id, mission and the collected/killed tables

- `save_id` (u16, 0xB2) indexes the level's `collected[]`/`killed[]` byte
  tables and the save bits `D_0014C190[level][id >> 5]`. A class that
  should not come back once collected checks `collected[save_id]` at
  init, e.g. `D_L01_001BB9C0.collected[(short)moby->save_id] != 0` in
  `src/overlays/shared/vendor_002B90A8.c:516`. The tables are in each
  level's own BSS (different address per level). `killed[]` entries
  store "spawn slot + 2" (`src/overlays/shared/vendor_00298BB8.c`).
- `mission` (0xB0) indexes the level's `D_0014C050` row (0xFF = not
  spawned). Level code also writes `collected[save_id] = mission + 2`
  (`src/overlays/l01_novalis/vendor_002FABE8.c:849`).
  `SetMissionDone` (`func_L00_002512D8`) and `SetDeathBits`
  (`func_L00_002584A8`) are the shared helpers (candidate names).
- `parent` (0xB8): the spawner. Its byte 0xB1 is a per-level id.
- `command` (0xBC): an external command byte other mobys write.
- `group` (0x21): index into the level moby-list table
  `D_Lxx_001ABCC0` (0xFF = none). Linked groups such as class 871's mine
  groups use it.

### Animation fields

| Field | Off | |
|---|---|---|
| `frame`, `prev_frame` | 0x50, 0x51 | current frame / frame in `prev_seq` (blend source) |
| `seq`, `prev_seq` | 0x52, 0x53 | sequence id into `pclass->seqs[]`; 0xFF = none (frame data then points at `D_001AAF40`) |
| `anim_time`, `anim_speed` | 0x54, 0x58 | `InitMobyInstance` sets 1.0, or `anim_speed = 0` for a still one-frame class |
| `cur_frame_data`, `prev_frame_data` | 0x68, 0x6C | refreshed from the class's sequence table (`func_0020D6D0`) |
| `anim_flags`, `anim_trigger_count` | 0x70, 0x7E | |
| `manips` | 0x64 | joint manipulators |

Each `seqs[i]` points at an `AnimSeq` (`include/records.h`) whose u8
`nframes` is at +0x10. `MobyClass.seq_count` (+0x0C) bounds `seq`. The
main entry points are `set_moby_animation` (`func_00213D28`),
`blend_moby_animation[_ex]` (`func_00213DE0`/`func_00213F28`),
`MobyAnimProc` (`func_00212578`) and the level helpers `MobyAnimAdvance`
(`func_L00_002514B8`) and `MobyAnimEval` (`func_L00_00252018`). Names
are from `config/names.tsv` (tiers vary).

## Binding classes to update functions

Each level overlay carries a `vtbl` record (`docs/OVERLAYS.md`, "Roles"):
12-byte entries `{oClass, update function, pointer to a 6-word table}`,
one per moby class the level has, ended by oClass -1, with the 6-word
tables after it. Registration (above) copies the update function into
`D_001B3900[slot]` and the table pointer into `MobyClass.callbacks`.
`InitMobyInstance` then puts `D_001B3900[slot]` in `moby->update`.
Update functions are called with the moby in `$a0`.

`func_00213BB8` addresses the table as `D_001E8F00` in the executable,
while the baserom manifests put each level's `vtbl` record at its own
address (0x1EA680 in level 0, 0x20BE00 in level 1). How the two connect
(copy or relocation at load) has not been verified.

`python3 tools/overlays.py names` decodes all levels' tables into
`config/overlays/names.tsv`. The second column lists roles as
`UpdateMoby_<oClass>:<levels>` (levels `00`..`18`, slash-separated).
One function can serve several classes; a function with many roles is
usually a generic one. `tools/extract/moby_classes.tsv` has class names
for editor labels (from outside tooling, not verified against the game).

### The most widespread classes

1167 classes have an update function in at least one level. 916 appear
in one level only, and **79 are in all 19 levels** (00..18), so the
"top 25 by level count" is all ties. The table lists the 25
lowest-numbered of the 79. All their update functions are in the shared
level code (`src/overlays/shared/`). Descriptions are quoted or condensed
from the comment above the function's definition and left blank where
there is none. "Shared" counts how many classes use the same function.

| oClass | Update function | Shared | File (`src/overlays/shared/`) | Description (from the source comment) |
|---|---|---|---|---|
| 11 | `func_L00_002A6A38` | 1 | `vendor_002A5138.c` | pulsing light-beam moby: spawns a child, fades it with distance to the player, draws it and may trigger the player's grab |
| 13, 14, 15, 16 | `func_L00_002A7078` (BoltUpdate) | 4 | `vendor_002A5138.c` | bolt: lands where dropped, hovers and spins, bounces, flies to the hero when near (or sucked in), leaves a trail, collected on arrival |
| 71 | `func_L00_002A9768` | 1 | `vendor_002A5138.c` | normal wrench swings and its thrown/returning collision sweep |
| 74 | `func_L00_002AB910` | 1 | `vendor_002AB910.c` | mine: falls and bounces, sticks to what it hits, homes on a nearby target and explodes |
| 112 | `func_L00_002ADD78` | 2 (112, 1192) | `vendor_002AB910.c` | decaying moby: spins down its position fields and fades out |
| 121 | `func_L00_002AEDD0` | 1 | `vendor_002AB910.c` | thrown bomb: flies and spins, bounces once, explodes on a hit or timeout, blast ring grows for 15 frames |
| 122 | `func_L00_002B08A0` | 1 | `vendor_002AB910.c` | falling spark: emits a burst particle, moves, fades, dies out of bounds |
| 149 | `func_L00_002B0D30` | 11 (149, 348, 349, 354-359, 363, 364) | `vendor_002AB910.c` | |
| 153 | `func_L00_002B1688` | 1 | `vendor_002AB910.c` | |
| 157 | `func_L00_002B33E8` | 1 | `vendor_002B33E8.c` | Devastator: tracks the aimed target, draws the reticle, fires, keeps usage statistics, moves the muzzle light |
| 163 | `func_L00_002B4340` | 1 | `vendor_002B33E8.c` | arms a pickup-style reaction when the player state allows it |
| 166 | `func_L00_002B4748` | 1 | `vendor_002B33E8.c` | |
| 167 | `func_L00_002B5488` | 1 | `vendor_002B33E8.c` | multi-state timed object: random delays pace the stages, which adjust step/scale in its pvars and call the shared callback |
| 168 | `func_L00_002B5C98` | 1 | `vendor_002B33E8.c` | Blaster: aims (reticle in first person, auto-aim otherwise), fires a shot with sparks, statistics, muzzle light |
| 172 | `func_L00_002B7430` | 1 | `vendor_002B33E8.c` | visibomb missile: drift, homing toward the target, arming countdown |
| 175 | `func_L00_002B8208` | 1 | `vendor_002B33E8.c` | |
| 176 | `func_L00_002B8AE8` | 1 | `vendor_002B33E8.c` (asm) | |
| 177 | `func_L00_002B9AD8` | 1 | `vendor_002B33E8.c` | Tesla claw: tracks up to two targets while fire is held, extends the claw tip, firing states, statistics, light |
| 179 | `func_L00_002BC708` | 1 | `vendor_002BA7C8.c` | follows its parent's flag and spins |
| 180 | `func_L00_002BC860` | 1 | `vendor_002BA7C8.c` | scatters ten fading sparkle quads around the hero |
| 185 | `func_L00_002BDAF0` | 1 | `vendor_002BA7C8.c` | Morph-o-Ray: aims it, transforms what it hits over time, keeps its glow light and sound going while held |
| 186 | `func_L00_002C12B0` | 1 | `vendor_002C12B0.c` (asm) | (no comment; see "M186" below) |

The remaining 54 all-level classes: 188 190 192 203 204 208 209 213 214
222 223 225 226 229 230 270 305 348 349 354 355 356 357 358 359 428 433
454 457 479 500 501 511 562 585 607 618 639 775 804 806 849 1006 1007
1138 1143 1192 1251 1289 1290 1438 1447 1449 1900. `func_L00_002C6720`
alone serves 11 of them (204 213 214 222 223 225 226 1006 1438 1447
1449). To regenerate: count, per oClass, the union of levels over all
`UpdateMoby_<oClass>` roles in `config/overlays/names.tsv`.

## Classes named by retail debug strings

The level overlays' `data` records keep the developers' debug and error
strings. They name some classes directly. Strings below are verbatim
(`strings baserom/overlays/level_NN/data.bin`). "Update levels" is
where `config/overlays/names.tsv` gives the class an `UpdateMoby_<id>`
role. In every case it matches the levels whose data holds the string.

| oClass | What the string says | String in levels | Update function (levels) |
|---|---|---|---|
| 28 | `*Error: Moby 28(Footsoldier) with no boundPath / flyPath / introPath / hatch index` | 14, 15 | `func_L14_002B2A28` (14, 15) |
| 557 | `*Error: Moby 557(hoverboat) with no target.` | 14 | `func_L14_002E9038` (14) |
| 610 | `*Error: Moby 610 (wind-control) with no cuboid index (wind pushing hero area)` (also fxAct / fxArea cuboids) | 14 | `func_L14_002EC3E8` (14) |
| 908, 921 | `Enemy Fighter (moby908) with no flyPath.`; same for `moby921` | 14 | `func_L14_002FD578`, `func_L14_002FE038` (14) |
| 923, 1401 | `MineBot (moby923) with no activation cuboid.`; `MineBot (moby1401) ...` (flyPath, exitPath) | 14; 16 | `func_L14_002FF728` (14); `func_L16_002E4C08` (16) |
| 1417 | `Flying Car (moby1417) with no flyPath.` | 14 | `func_L14_00308368` (14) |
| 1059 | `Illegal Bounding path` / `Illegal Hero Detect path` / `Illegal guard point cuboid` / `Illegal water moby for moby 1059 instance #%d.` | 07 | `func_L07_003109D0` (07) |
| 871 | `Moby 871 could not find mines for group %d` (and mine-count vs pVar checks) | 07 | `func_L07_0030C3E0` (07) |
| 1142 | `ERROR! Moby 1142 Equilizer Ammo Spawner has no trigger cuboid!` [sic] | 07 | `func_L07_0031E6B0` (07) |
| 774 | `WARNING! - Moby 774 wasn't expecting a M774_ShipChallengeDone call. ...` | 01, 08 | `func_L01_003004F0` (01, 08) |
| 947 | `Moby947 maya instance %d HAS NO PATH!!!` | 10 | `func_L10_002DA0F0` (10) |
| 1067 | `moby class 1067 REQUIRES a link to dataProcessor class 1073!!` | 10 | `func_L10_002DB688` (10); 1073: `func_L10_002DE018` (10) |
| 1181 | `could not link next / prev on moby type 1181 #%d` (a linked list of instances) | 09 | `func_L09_00305710` (09) |
| 1885 | `Moby 1885 UID:%d could not find a moby to respawn at time %d!` (a respawner) | 09 | `func_L09_0030BF88` (09, 13) |
| 427 | `moby 427, instance %d requires both path fields to run!!! DELETING MOBY FROM LEVEL!!` | 04 | `func_L04_002C5BD0` (04) |
| 1120 | `Moby 1120 %d has no segment switch!` | 04 | `func_L04_002E2DF0` (04) |
| 1290 | `Moby 1290 %d has no segment switch!`, `Moby 1290 %d needs a cut scene!` | all 19 | `func_L00_002E1850` (all 19) |
| 899 | `Bad path data on moby class 899, maya instance %d` | 03 | `func_L03_002DC648` (03) |
| 111 | `no path for moby 111 #%d: killing` | 13 | `func_L13_002C56B0` (13) |
| 6 | `No trigger for moby 6 instance %d. Deleting` | 13 | `func_L13_002B2020` (13) |
| 794 | `M794 %d trigger %d = moby %d (oClass %d)` (an effect/trigger moby) | 10 | `func_L10_002CB030` (10) |
| 671 | `M671 texture w count = 0.0, correcting to 1.0`, `... greater than max texture tile count (8)` (texture tiling) | 08 | `func_L08_002F8560` (08) |
| 186 | `Hey, M186 init should be earlier!` (an init-order check) | all 19 | `func_L00_002C12B0` (all 19) |

Notes:

- "UID" in these strings is the per-level instance id and "maya instance"
  the instance number from the level editor. Neither is the slot index.
- "Mystery 774" does not appear in any level's data. Only the
  `M774_ShipChallengeDone` string does, so no name for 774 is
  established.
- Level 14 also prints errors for a "Hovertank" (bomb/bound/exit/intro
  paths) and level 09 for a "Kamikaze" and a "Bigship moby", without an
  oClass number in the string.
- Generic strings worth knowing when you read init code: `Moby Class %d
  Instance %d has no pVars!  May be in the art file.` (10),
  `cleaned up moby:%d (oclass:%d)` (00), `internal moby sound mismatch on
  moby %d` (00).

## Tips for decompiling and porting moby code

- **Start from the role.** `config/overlays/names.tsv` tells you which
  class a level function updates and on which levels. A function shared
  by many classes is generic, so don't specialise it to one class.
- **Include `moby.h`, `hero.h`, `pad.h`, `vector.h`** instead of private
  struct copies. The recent refactors replaced dozens of private copies
  (commits d19de34f, d8808743). Private casts are still needed for
  **pvars**: declare a local struct for the class's 0x80 bytes and cast
  `moby->pvars` (u8 *) to it.
- **Watch the signedness and width of the byte fields.** `state` is read
  signed by the update loop (`lb`, `bltz`) but compared unsigned
  (`>= 0xFE`) elsewhere. `save_id` is u16 but used as `(short)` index.
  `draw_dist` is s16, while `update_dist` is u8. The wrong type changes
  `lb`/`lbu`/`lh`/`lhu` and breaks the match, and in a port it changes
  behaviour at 0x80/0x8000.
- **Use the state/flag conventions** rather than inventing new ones:
  delete with `DeleteMoby` (two-frame reuse delay), not by poking
  `state`. Test `D_001B3E40[oClass] != 0xFF` before spawning a class
  the level may not have.
- **Update order and timing are behaviour.** The list is rebuilt every
  frame by `func_0020E6B8` and walked in order. Mobys out of
  `update_dist` don't tick at all, so their timers freeze. Frame counts
  in level code often go through `scale_game_frames()` (`func_001F98C0`),
  and a port must keep the same scaling.
- **Handwritten asm stays asm.** `func_0020E6B8`, `func_0020E3D0`,
  `func_0020ED48`, `func_0020EA70` and others are in
  `config/handwritten_asm.txt` (they use VU0 macro instructions and
  scratchpad RAM). A port needs a C reimplementation of them that
  reproduces the results exactly (list order, float results). Rewrites
  are planned after matching, and they keep the functions marked as asm.
- **Debug strings are free documentation.** Grep the level's `data.bin`
  strings for the oClass number before naming a class or a pvar.
- **Check callees against their asm.** C that matches can still declare
  a callee wrong (argument count, return type). Compare each `extern`
  prototype with the callee's own asm: which argument registers it
  reads, whether it sets `$v0`/`$f0`. Update functions take exactly one
  argument, the moby.
