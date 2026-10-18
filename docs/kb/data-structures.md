# Data structures

The records that most of the game's code reads: vectors, the moby
(object) record and its class header, the hero, the controller state, and
the global singletons around them. Mobys get their own page,
[mobys.md](mobys.md).

**This is a byte-exact decompilation of the PAL release (SCES-50916).** The
layouts on this page are the ones retail code reads and writes; offsets
come from the matched code. A native port that imports this C has to keep
them, and the behaviour built on them, 1:1. Moving a field, widening a
type or "fixing" an odd store order changes behaviour that players depend
on. Speedrun tricks rely on exact behaviour: frame counts, the order mobys
update in, float quirks, which bits a menu tests.

How to read the tables:

- Offsets were computed by clang from the real headers (`include/*.h`),
  not typed by hand. To regenerate them, use the tidy tooling in
  `build-sn/tidy/` (`conv.canon_layouts`).
- Only named fields are listed. Gaps are `pad_*` bytes that no matched
  function has been seen to use.
- `unkXXX` means the field is accessed but its meaning is not known yet.
- Notes come from the header comments, and these cite the function that
  shows the meaning. `FUN_Lxx_...` / lowercase names in those comments are
  the NTSC decomp's labels from the US build. PAL addresses differ (see
  "Addresses" below).
- The headers say what they were adapted from. They are the single source
  of truth: if this page and a header disagree, the header wins, and
  this page should be fixed.

## Vectors (`include/vector.h`)

| Type | Size / align | Layout | Use |
|---|---|---|---|
| `Vec4f` | 16 / 4 | `f32 x, y, z, w` | A float vector embedded in records whose alignment is only 4. `Moby.pos`, `Moby.rot` and `Moby.bsphere` use it. |
| `Vec4A` | 16 / 16 | `f32 x, y, z, w`, `aligned(16)` | The same four floats, aligned so that one `lq`/`sq` moves them. |
| `VecQuad` | 16 / 16 | `u32 __attribute__((mode(TI)))`, a 128-bit integer | The raw quadword. It isn't called `u128` because source files declare their own `u128`. |
| `Vec4` | 16 / 16 | `union { VecQuad q; f32 f[4]; s32 i[4]; }` | The general 16-byte vector: copy it as one quadword (`.q`), read it as floats (`.f`), or zero/test it as ints (`.i`). The hero and its sub-records use it everywhere. |

Why alignment matters:

- The EE moves a whole vector with 128-bit `lq`/`sq` (and the
  `qcopy`/`qzero` helpers, the only inline-asm allowed in new C outside
  VU0/handwritten functions). `lq`/`sq` ignore the low four address bits,
  so the data **must** be 16-byte aligned. A misaligned `Vec4` does not
  fault on the PS2. The access silently rounds down to the containing
  quadword.
- A native port has to keep `Vec4`/`Vec4A` 16-aligned and 16 bytes long,
  with every record that contains them at the same offsets. A plain
  `struct {float x,y,z,w;}` with 4-byte alignment is only correct where
  retail used `Vec4f`.
- Choosing `Vec4f` or `Vec4` changes the generated code (the alignment,
  and so the padding of the enclosing struct, and whether gcc may use
  `lq`). That is why both exist.

## Moby (`include/moby.h`)

A moby is the game's object instance: 0x100 bytes, one per live object
(Ratchet's model, enemies, bolts, crates, platforms, projectiles, ...).
Lifecycle, update loop and per-level class binding are on
[mobys.md](mobys.md).

| Offset | Type | Field | Notes |
|---|---|---|---|
| 0x000 | `Vec4f` | `bsphere` |  |
| 0x010 | `Vec4f` | `pos` |  |
| 0x020 | `u8` | `state` | >= 0xFE: dead, waiting to respawn |
| 0x021 | `u8` | `group` | linked group: index into the level moby-list table D_Lxx_001ABCC0 (0xFF: none) |
| 0x022 | `u8` | `class_slot` | class slot: pclass = D_L00_00197300[class_slot] (FUN_L00_002cf218) |
| 0x023 | `u8` | `alpha` | 0x80 by default; 0x40 for the smoke trail FUN_L09_00307ba8 spawns |
| 0x024 | `struct MobyClass *` | `pclass` |  |
| 0x028 | `struct Moby *` | `next` |  |
| 0x02C | `f32` | `scale` | draw scale (FUN_L01_002fa068 halves it, FUN_L00_00215ef8 divides by it) |
| 0x030 | `u8` | `update_dist` | update distance; 0xFF (always) or 0x7F for beams, set by spawners |
| 0x031 | `u8` | `visible` | drawn last frame; set to 1 by spawners |
| 0x032 | `s16` | `draw_dist` | draw distance; 0xFF (0x7F for beams), set by spawners |
| 0x034 | `u16` | `flags` |  |
| 0x036 | `u16` | `occlusion` | occlusion bits; 0x7F80 set by spawners |
| 0x038 | `u64` | `spawn_frame` | frame count at which it may respawn |
| 0x040 | `Vec4f` | `rot` | z: yaw (FUN_L00_00266448 compares it with atan2 to the hero) |
| 0x050 | `u8` | `frame` | animation frame |
| 0x051 | `u8` | `prev_frame` | frame index in prev_seq |
| 0x052 | `u8` | `seq` | animation sequence id |
| 0x053 | `u8` | `prev_seq` |  |
| 0x054 | `f32` | `anim_time` |  |
| 0x058 | `f32` | `anim_speed` |  |
| 0x064 | `struct Manip *` | `manips` |  |
| 0x068 | `void *` | `cur_frame_data` |  |
| 0x06C | `void *` | `prev_frame_data` |  |
| 0x070 | `u8` | `anim_flags` |  |
| 0x071 | `u8` | `unk71` | set to 0xFF when a moby changes class |
| 0x072 | `u8` | `unk72` |  |
| 0x073 | `u8` | `unk73` |  |
| 0x074 | `void (*)(struct Moby*)` | `update` |  |
| 0x078 | `u8 *` | `pvars` |  |
| 0x07C | `u8` | `unk7C` |  |
| 0x07D | `u8` | `sound_slot` | D_0013E550 slot of its looping sound, 0xFF: none (FUN_L00_00232670) |
| 0x07E | `u8` | `anim_trigger_count` |  |
| 0x07F | `u8` | `shadow_range` | shadow range in units of 1024; set to 0x17 by FUN_L09_002c5990 near D_L09_00166F40 |
| 0x080 | `s32` | `unk80` | colour from the level's moby records, c2 << 16 \| c1 << 8 \| c0 (func_L00_002422D8) |
| 0x086 | `s16` | `unk86` | compared with 5 (func_L02_002D9F08) |
| 0x090 | `s32` | `glow_color` | glow colour (class header + 0x40) |
| 0x094 | `u32` | `collision` | collision data, from the class header (MobyClass.collision) |
| 0x098 | `s32` | `collision_disable` | per-primitive collision disable bits; 1 while a carrier holds the moby (FUN_L00_002c7a58) |
| 0x0A4 | `u8` | `hit_slot` | hit-message slot (0xFF: none) |
| 0x0A6 | `s16` | `oclass` |  |
| 0x0B0 | `u8` | `mission` | index into the level's D_0014C050 row (0xFF: not spawned) |
| 0x0B1 | `u8` | `unkB1` | set from the level's moby records (func_L00_002422D8) |
| 0x0B2 | `u16` | `save_id` | index into the level collected[]/killed[] tables and save bits D_0014C190[level][id >> 5] |
| 0x0B4 | `s16` | `unkB4` |  |
| 0x0B6 | `s16` | `unkB6` | set from the level's moby records (func_L00_002422D8) |
| 0x0B8 | `void *` | `parent` | spawner (bolt source record); its byte 0xB1 is a per-level id (FUN_L00_002a6b70) |
| 0x0BC | `u8` | `command` | external command byte |
| 0x0C0 | `Vec4f` | `rot_row0` | rows of the matrix built from rot (FUN_001fa030) |
| 0x0D0 | `Vec4f` | `rot_row1` |  |
| 0x0E0 | `Vec4f` | `rot_row2` |  |

sizeof = 0x100

Checked by hand against code: `state` at 0x20, `next` at 0x28,
`flags` at 0x34 and `update` at 0x74 are the offsets the update loop
`func_00213C78` loads (`lb 0x20`, `lw 0x28`, `lhu 0x34`, `lw 0x74`). The
writes to 0xA6 (oClass) and 0x78 (pvars) are in `InitMobyInstance` /
`CreateMoby` (`src/game/mobyfunc.c:188`, `:295`). The `class_slot` note
cites a level-side table `D_L00_00197300`. The executable's class
registration keeps slot -> class header in `D_001B3580[slot]`
(`src/game/loaders.c:973`). Whether the two are views of the same data
has not been checked.

**Do not use `MobyInstance` from `include/structs.h` or the struct in
`docs/PVARS.md` for offsets.** They put `pUpdate` at 0xA8, `pVar` at 0xAC
and `oClass` at 0xBC, which contradicts the matched code (update 0x74,
pvars 0x78, oclass 0xA6). Those offsets look like they come from a
different game's layout (inference). `include/moby.h` is the one that
matches retail.

### Moby.flags (0x34) bits known from code

| Bit | Meaning | Evidence |
|---|---|---|
| 0x0002 | no update function for the class (set when `D_001B3900[slot]` is 0); cleared again if the class's first sequence has 2+ frames | `InitMobyInstance`, `src/game/mobyfunc.c:295` |
| 0x0004 | skip the post-update pass `func_0020ED48` | `func_00213C78` (`andi 0x4`) |
| 0x0005 (0x1 and 0x4) | set together when the class has no header (`pclass = 0`) | `InitMobyInstance` (`flags \|= 5`) |
| 0x0010 | has a glow colour (class +0x40 copied to `glow_color`) | `InitMobyInstance` |
| 0x0040 | skip the per-frame pre-pass `func_0020E3D0`; also set on creation when class byte +0x0C is 1, the first sequence has one frame and its byte +0x11 is negative | `func_00213C78` (`andi 0x40`), `InitMobyInstance` |
| 0x0400 | class has a shadow (class byte +0x0F); `shadow_range` set to 0x18 | `InitMobyInstance` |

The class header's `flags` (+0x44) is ORed in on creation.

## MobyClass (`include/moby.h`)

The class header that `Moby.pclass` points at. Its size is unknown, and
only the fields that matched code reads are named.

| Offset | Type | Field | Notes |
|---|---|---|---|
| 0x00C | `u8` | `seq_count` | number of entries in seqs[] (menu previews clamp the sequence to it) |
| 0x010 | `u32` | `collision` | collision data; copied into Moby.collision when a moby is (re)classed |
| 0x01C | `void *` | `retarget_table` | word table, indexed id * 4 + 4 (0024eec0) |
| 0x020 | `struct GifEntry *` | `gifs` | patched by patch_moby_gifs |
| 0x024 | `f32` | `scale` | default draw scale: copied into Moby.scale, divides it |
| 0x028 | `s32` | `sound_defs` |  |
| 0x02C | `void **` | `callbacks` | function-pointer table, called with the moby |
| 0x044 | `u16` | `flags` | initial Moby.flags |
| 0x046 | `s16` | `type` | class type; 5 is tested by targeting code |
| 0x048 | `struct AnimSeq *[1]` | `seqs` | animation sequences, indexed by Moby.seq |

sizeof = 0x4C

`callbacks` (+0x2C) is the "6-word table" of the level's `vtbl` record:
`func_00213BB8` (`src/game/mobyutil.c:175`) stores the table's third word
there (see [mobys.md](mobys.md#binding-classes-to-update-functions)).
`InitMobyInstance` also reads class bytes +0x06 (sets `Moby.unk73` to
0x18), +0x0C (sequence mode: 1 with a one-frame sequence stops the
animation) and +0x0E (copied to `Moby.unk72`).

## Hero (`include/hero.h`)

The player (Ratchet): one 0x2300-byte record, the singleton
**`D_0013F450`** (122 source files reference it). `D_0013F450 + 0x80`
= `D_0013F4D0`, which `config/names.tsv` names `gHeroPos` ("X/Y/Z
coordinate of Ratchet"). That agrees with `motion.pos` at +0x80.

The comment above `struct Hero` in `hero.h` says "D_0013F350". That is
the US address (the US build sits 0x100 lower in this region, e.g. pad
US `D_0013C940` vs PAL `D_0013CA40`). The PAL code uses `D_0013F450`.

Some overlay code reaches the hero as `D_0013E633 + 0xE1D` (=
0x0013F450), e.g. `src/overlays/shared/tieproc_00299108.c:144`. That is
an artifact of how the compiler materialised the address, not a second
object.

Layout overview (sub-records are their own structs, and the offsets are
absolute in `Hero`):

| Offset | Record | Size | What |
|---|---|---|---|
| 0x000 | `f32[16]` x2 | 0x80 | two matrices (unnamed) |
| 0x080 | `HeroMotion motion` | 0x110 | position, rotation, velocities |
| 0xD30 | `HeroManipulator manipulators[7]` | 7 x 0x40 | joint manipulators attached while `rand_timer` runs |
| 0xFF0 | `HeroRandTimer rand_timer` | 0xC | random-interval timer |
| 0x1090 | `HeroItemSlot items[7]` | 7 x 0x50 | item slots; slot 0 = equipped gadget |
| 0x1670 | `MobyTrail trail` | 0x140 | ring of moby copies following the hero moby |
| 0x18F0 | `HeroEase unk18F0[3]` | 3 x 0xB0 | eased manipulator records |
| 0x1D90 | `HeroEase unk1D90` | 0xB0 | manipulator handed to attach_manipulator |
| 0x2080 | `struct Moby *moby` | 4 | the hero's moby |
| 0x2084 | `HeroState state` | 0x20 | state machine |
| 0x22A8 | `HeroHealth health` | 0xC | hit points |

Named (non-`unk`) fields of `Hero`. See `include/hero.h` for all
fields and comments:

| Offset | Type | Field | Notes |
|---|---|---|---|
| 0x080 | `struct HeroMotion` | `motion` |  |
| 0x190 | `f32` | `target_speed` | horizontal speed ground_speed eases toward; the stick handler writes it (0 with the stick centred) |
| 0x194 | `f32` | `ground_speed` | horizontal speed target this frame (0.095 running, i.e. 5.7 u/s at 60 fps; 0 idle) |
| 0x198 | `s32` | `state_timer` | frames in the current state |
| 0x1C0 | `s32` | `invulnerable_frames` |  |
| 0x1E8 | `s16` | `fidget_cooldown` |  |
| 0x220 | `f32` | `capsule_top` |  |
| 0x23C | `u8 *` | `coll_hit_moby` | moby of the last push-out collision hit |
| 0x248 | `f32` | `wall_probe_distance` |  |
| 0x257 | `u8` | `wall_hit` |  |
| 0x290 | `Vec4` | `gravity_dir` | gravity direction (0, 0, -1) |
| 0x2A0 | `Vec4` | `ground_point` | ground probe hit point under the hero |
| 0x2D8 | `union { f32 f; s32 i; }` | `ground_z` | ground_point.z; zeroed as an int |
| 0x2DC | `f32` | `ground_distance` | distance from motion.pos to ground_point |
| 0x2E0 | `union { f32 f; s32 i; }` | `ground_slope` | written as a float and zeroed as an int |
| 0x2E4 | `union { f32 f; s32 i; }` | `ground_pitch` | written as a float and zeroed as an int |
| 0x2EC | `f32` | `downhill_yaw` |  |
| 0x2F0 | `f32` | `water_surface_z` |  |
| 0x2FC | `struct Moby *` | `ground_moby` | moby the ground probe hit (0 = level ground) |
| 0x300 | `s32` | `grounded_frames` | frames on the ground |
| 0x30C | `union { s16 s; u16 u; }` | `frames_off_ground` | read signed, incremented unsigned |
| 0x30E | `union { s16 s; u16 u; }` | `air_frames` | frames off the ground, 0 on landing |
| 0x56C | `s32` | `grind_rail` |  |
| 0x640 | `Vec4 [3]` | `basis` | rotation rows (transform_vector_by_basis) |
| 0x920 | `Vec4` | `move_impulse` | one-frame displacement; the hero update adds it to motion.pos and clears it (some states; zero while running) |
| 0xA60 | `s32` | `attack_record` |  |
| 0xA6C | `f32` | `speed_scale` | multiplier on the run target in some states (target = frame_time * 5.7 * speed_scale); 1.0 normally |
| 0xA74 | `f32` | `aim_yaw` | gadget aim yaw (FUN_L00_00217658) |
| 0xA78 | `f32` | `aim_pitch` | gadget aim pitch |
| 0xA90 | `f32` | `anim_speed` |  |
| 0xAA8 | `f32` | `anim_frame` |  |
| 0xD30 | `struct HeroManipulator [7]` | `manipulators` | attached by FUN_L00_002056a8 while rand_timer runs |
| 0xFF0 | `struct HeroRandTimer` | `rand_timer` |  |
| 0x1090 | `struct HeroItemSlot [7]` | `items` | item slots; slot 0 is the equipped gadget |
| 0x12E0 | `s16` | `ground_type` |  |
| 0x12E4 | `u8` | `base_condition` |  |
| 0x12E5 | `u8` | `selector_1` |  |
| 0x12E6 | `u8` | `selector_3` |  |
| 0x12EA | `u8` | `on_slip_ground` |  |
| 0x12EB | `u8` | `selector_11` |  |
| 0x12EC | `u8` | `selector_13` |  |
| 0x15F0 | `struct Moby *` | `ship_moby` | the ship the hero is flying (l17 FUN_L17_002ed018) |
| 0x15F4 | `s16` | `ship_oclass` | its oclass, -1 when none |
| 0x15F6 | `u8` | `ship_ammo` | shots left in the space levels; firing needs one and takes it (FUN_L11_003126f8) |
| 0x15F7 | `u8` | `ship_ammo_max` | ammo pips the ship HUD draws, lit while below ship_ammo |
| 0x15FC | `f32` | `ship_hp` | ship health; l17 subtracts collision damage and explodes the ship below 0 |
| 0x1600 | `f32` | `ship_hp_max` | full ship_hp; the HUD bar shows ship_hp over it |
| 0x1608 | `s32` | `ship_hp_percent` | ship_hp as a percentage for the HUD |
| 0x160F | `u8` | `ship_flags` | 2 = being steered back from the edge or height limit of the space arena (FUN_L11_00313f60) |
| 0x1628 | `f32` | `glow_phase` | phase of the hero moby's colour pulse (func_L00_002086C8) |
| 0x1670 | `struct MobyTrail` | `trail` | trail of moby copies following hero.moby |
| 0x1D20 | `f32` | `stick_x` | stick vector (d-pad when the stick is idle) |
| 0x1D24 | `f32` | `stick_y` |  |
| 0x1FF6 | `u8` | `ammo_used` |  |
| 0x1FF7 | `u8` | `ammo_capacity` |  |
| 0x2080 | `struct Moby *` | `moby` | the hero's moby |
| 0x2084 | `struct HeroState` | `state` |  |
| 0x20A4 | `u8` | `vehicle_mode` |  |
| 0x20B8 | `s32 [7]` | `pending_item` | per slot: item to switch to |
| 0x20D4 | `s32 [7]` | `selected_item` | per slot: item after a switch |
| 0x20F0 | `s32 [7]` | `saved_item` | per slot: item kept while another is forced in |
| 0x210C | `s32 [7]` | `restore_item` | per slot: 1 puts saved_item back |
| 0x2288 | `f32` | `reach_distance` |  |
| 0x228C | `f32` | `reach_height` |  |
| 0x22A8 | `struct HeroHealth` | `health` |  |

sizeof = 0x2300

### HeroMotion (hero + 0x80)

| Offset | Hero offset | Type | Field | Notes |
|---|---|---|---|---|
| 0x000 | 0x080 | `Vec4` | `pos` | world position |
| 0x010 | 0x090 | `Vec4` | `rot` | rotation; z is the yaw |
| 0x020 | 0x0A0 | `Vec4` | `unkA0` |  |
| 0x030 | 0x0B0 | `Vec4` | `unkB0` |  |
| 0x040 | 0x0C0 | `Vec4` | `unkC0` |  |
| 0x050 | 0x0D0 | `Vec4` | `unkD0` |  |
| 0x060 | 0x0E0 | `Vec4` | `velocity` | per-frame velocity |
| 0x070 | 0x0F0 | `Vec4` | `platform_delta` | platform movement this frame |
| 0x080 | 0x100 | `Vec4` | `displacement` | total displacement this frame |
| 0x090 | 0x110 | `Vec4` | `unk110` |  |
| 0x0A0 | 0x120 | `Vec4` | `unk120` |  |
| 0x0B0 | 0x130 | `Vec4` | `unk130` |  |
| 0x0C0 | 0x140 | `Vec4` | `platform_carry` |  |
| 0x0D0 | 0x150 | `Vec4` | `unk150` | copied from unk110/displacement on hero_set_state |
| 0x0E0 | 0x160 | `f32` | `speed_xy` | current horizontal speed per frame; eases toward ground_speed, capped at frame_time * 52 |
| 0x0E4 | 0x164 | `f32` | `unk164` |  |
| 0x0E8 | 0x168 | `f32` | `unk168` |  |
| 0x0EC | 0x16C | `f32` | `unk16C` |  |
| 0x0F0 | 0x170 | `Vec4` | `unk170` |  |
| 0x100 | 0x180 | `f32` | `move_yaw` | angle, compared with rot.z |
| 0x104 | 0x184 | `f32` | `unk184` |  |
| 0x108 | 0x188 | `f32` | `unk188` |  |

sizeof = 0x110

### HeroState (hero + 0x2084)

| Offset | Type | Field | Notes |
|---|---|---|---|
| 0x000 | `s32` | `current` | state id; hero_set_state sets it |
| 0x004 | `s32` | `step` | sub-step, 0 on state change |
| 0x008 | `s32` | `control_mode` |  |
| 0x00C | `s32` | `prev` | state before the last change |
| 0x010 | `s32` | `prev_control_mode` | control_mode before the last change |
| 0x014 | `s32` | `prev_timer` | state_timer before the last change |
| 0x018 | `s32` | `prev2` | state before prev |
| 0x01C | `s32` | `prev2_control_mode` | control mode before prev_control_mode |

sizeof = 0x20

`hero_set_state` copies `current`/`control_mode`/`state_timer` into the
`prev*` fields, zeroes `step` and `state_timer`, and clears `trail`
(hero.h comment above `struct Hero`).

### HeroHealth (hero + 0x22A8)

| Offset | Type | Field | Notes |
|---|---|---|---|
| 0x000 | `s32` | `hp` | hit points; 0 = dead |
| 0x004 | `s32` | `unk22AC` | swapped with health.hp |
| 0x008 | `s16` | `unk22B0` | s16 copy of health.hp |
| 0x00A | `s16` | `unk22B2` |  |

sizeof = 0xC

### HeroItemSlot (hero + 0x1090 + i * 0x50)

| Offset | Type | Field | Notes |
|---|---|---|---|
| 0x000 | `struct Moby *` | `moby` | item moby |
| 0x004 | `struct Moby *` | `moby2` | second item moby |
| 0x00C | `u8 *` | `unk0C` |  |
| 0x010 | `s32` | `button_mask` | pad buttons that hold the gadget states |
| 0x014 | `s32` | `unk14` | set on release |
| 0x018 | `s16` | `timer` | counts down; a slot change waits for 0 |
| 0x01A | `u8` | `unk1A` | cleared on a slot change |
| 0x01B | `u8` | `unk1B` | counts releases (FUN_L00_00210748, state 3) |
| 0x01C | `u8` | `unk1C` | 2 blocks a slot change, 1 reloads timer |
| 0x01D | `u8` | `timer_reload` | timer reload value |
| 0x020 | `s32` | `unk20` |  |
| 0x024 | `s32` | `state` | 0 empty, 2 ready, 3 released |
| 0x028 | `s32` | `item_id` | item in the slot |
| 0x02C | `f32` | `glow_phase` | phase of the item moby's colour pulse (FUN_L00_002080b0) |
| 0x030 | `f32` | `unk30` |  |
| 0x034 | `f32` | `unk34` |  |
| 0x038 | `s32` | `unk38` |  |
| 0x03C | `s16` | `unk3C` |  |
| 0x03E | `u16` | `unk3E` |  |
| 0x040 | `s32` | `unk40` | items[6]: aim lock (func_L00_002B33E8) |
| 0x048 | `struct Moby *` | `lock_target` | items[6]: the moby the weapons lock on to while unk40 is set (func_L00_002B33E8) |
| 0x04C | `f32` | `unk4C` | items[6]: aim yaw (func_L00_002CDAB8) |

sizeof = 0x50

The hero.h comment says "four item slots", but the array has 7 entries
and matched code uses `items[6]` (weapon aim lock, `func_L00_002B33E8`).
Treat the array as 7 long. The item-switch arrays `pending_item`,
`selected_item`, `saved_item` and `restore_item` (0x20B8..0x2128) are
also 7 long. `FUN_L00_0020fca8(i, v)` releases slot i (state 3), and the
`..._0020d460` / `..._0020d498` helpers return a ready slot's moby / item
id (hero.h comment).

### HeroManipulator (hero + 0xD30 + i * 0x40)

| Offset | Type | Field | Notes |
|---|---|---|---|
| 0x000 | `u8` | `unk0` |  |
| 0x001 | `u8` | `attached` |  |
| 0x002 | `u8` | `unk2` |  |
| 0x003 | `u8` | `unk3` |  |
| 0x00C | `f32` | `unkC` |  |
| 0x010 | `Vec4` | `quat` |  |
| 0x020 | `Vec4` | `scale3` |  |
| 0x030 | `Vec4` | `translation` |  |

sizeof = 0x40

A HeroManipulator is the first 0x40 bytes of the 0xB0-byte `HeroEase`
layout. `HeroEase` (eased by `FUN_L00_00205168`) adds rot/pos, their
velocities and targets, `kind` (+0xA2), `accel`, `max_speed` and
`scale`, and rebuilds `quat`/`scale3`/`translation` from them each frame.
`attach_manipulator` hands that part to the moby. `MobyTrail`,
`HeroRandTimer` and `HeroVelocityKey` are documented inline in hero.h.

## PadState (`include/pad.h`)

The controller state, the singleton **`D_0013CA40`** (71 source files),
0x350 bytes. `func_00218908` (UpdatePad) calls `func_00217F68` (UpdatePad
on that pad) each frame. That function reads the device through the
socket and hands the raw report to `func_002181F0` (process_pad_input),
all in `src/game/pad.c`. Some overlay code addresses it as
`D_0013A5E0 + 0x2460` (= 0x0013CA40), e.g.
`src/overlays/shared/help_00237B00.c:128`.

| Offset | Type | Field | Notes |
|---|---|---|---|
| 0x100 | `f32 [16]` | `analog` | [0..3] stick axes (-1..1), [4..15] pressure (0..1) |
| 0x140 | `f32 [16]` | `analog_prev` | copy of analog |
| 0x180 | `u8 [4]` | `profile` | button profile block UpdatePad reads (func_00124DF0) |
| 0x184 | `u8 [4]` | `act` | actuator block UpdatePad reads (func_00125218) |
| 0x188 | `u8` | `unk188` |  |
| 0x189 | `u8` | `unk189` | copied by FUN_L00_002d2ee8 on spawn |
| 0x18C | `s16` | `unk18C` | counted down by UpdatePad; on pad 0, nonzero suppresses pressed/released (PAL) |
| 0x18E | `s16` | `hist_idx` | next slot of the 30-poll history; cleared with hist_count by ResetGlobalStateFields |
| 0x190 | `s32` | `hist_count` | history entries filled, up to 30 |
| 0x194 | `s32` | `socket` | scePad2CreateSocket(&param, &D_0013C940) result, init_pads |
| 0x198 | `s32` | `profile_state` | 0 query profile, 1 read pad, 2 profile too long |
| 0x19C | `s32` | `device_state` | scePad2GetState result |
| 0x1A0 | `s32` | `held` | buttons down, plus stick directions |
| 0x1A4 | `s32` | `pressed` | newly down this frame (~prev_held & held) |
| 0x1A8 | `s32` | `released` | ~held & prev_held |
| 0x1AC | `s32` | `prev_held` |  |
| 0x1B0 | `s32` | `raw_held` | buttons only, no stick directions |
| 0x1B4 | `s32` | `raw_pressed` | ~prev_held & raw_held; saving_data_menu pad_buttons source |
| 0x1B8 | `s32` | `raw_released` | ~held & prev_raw_held; weapon swap tests 0x10 |
| 0x1BC | `s32` | `prev_raw_held` | raw_held of the previous poll |
| 0x1C0 | `u32` | `held_unmasked` |  |
| 0x1C4 | `u32` | `pressed_unmasked` | menu handlers test 0xD00 (accept) and 0x10 each frame |
| 0x1C8 | `u32` | `released_unmasked` |  |
| 0x1CC | `s32` | `mode` | 1 masks off 0x5030 and the left stick, 2 keeps only 0x900; reset to 0 |
| 0x1D0 | `s32` | `no_buttons` | held == 0 |
| 0x1D4 | `s32` | `no_direction` | (held & 0xF000) == 0 |
| 0x1D8 | `s32` | `stick_moved` | analog[2] or analog[3] nonzero |
| 0x1DC | `s32` | `unk1DC` | 0x79 if the button profile is all ones, else 0; l01 picks pressure input on 0x79 |
| 0x1E0 | `s32 [30]` | `hist_btn` | pressed, per poll, for the stick flick detector |
| 0x258 | `f32 [30]` | `hist_ang` | stick angle, per poll |
| 0x2D0 | `f32 [30]` | `hist_mag` | stick magnitude, per poll |
| 0x348 | `s32` | `unk348` | held, latched by UpdatePad |
| 0x34D | `u8` | `rep_delay` | key repeat: frames before the first repeat |
| 0x34E | `u8` | `rep_rate` | frames between repeats |
| 0x34F | `u8` | `rep_timer` |  |

sizeof = 0x350

### Button bits

`func_002181F0` builds the mask from the first two report bytes
(`src/game/pad.c`, first statement of `func_002181F0_r`):

```c
p->raw_held = p->held = ((buf[0] << 8) | buf[1]) ^ 0xFFFF;   /* active-low -> active-high */
```

| Bit | Button | Bit | Button |
|---|---|---|---|
| 0x0001 | L2 | 0x0100 | Select |
| 0x0002 | R2 | 0x0200 | L3 |
| 0x0004 | L1 | 0x0400 | R3 |
| 0x0008 | R1 | 0x0800 | Start |
| 0x0010 | Triangle | 0x1000 | Up |
| 0x0020 | Circle | 0x2000 | Right |
| 0x0040 | Cross | 0x4000 | Down |
| 0x0080 | Square | 0x8000 | Left |
| 0x10000 | *synthetic*: stick flick (only in `pressed`, see below) | | |

How sure: **high for the d-pad bits, high but inferred for the rest.**
No source comment names the buttons. The table is the standard PS2
digital-report layout (byte 0: Select, L3, R3, Start, Up, Right, Down,
Left from bit 0; byte 1: L2, R2, L1, R1, Triangle, Circle, Cross, Square),
and the code agrees with it wherever it can be checked:

- Left-stick x < 0 ORs 0x8000 and x > 0 ORs 0x2000 (Left/Right). y < 0
  ORs 0x1000 and y > 0 ORs 0x4000 (Up/Down; stick y is negative up).
  `no_direction` tests `held & 0xF000`.
- The mirror option swaps exactly 0x8000 and 0x2000 (left/right).
- `mode` 2 keeps only 0x900 (Start|Select).
- The most common gameplay test is `pressed & 0x40` (69 sites; Cross is
  jump), then 0x10 and 0x20. The weapon-swap code tests 0x10 (Triangle,
  the quick-select button) in `raw_released`.
- Menus test `pressed_unmasked & 0xD00` (Start|R3|Select) to close a
  pause menu when closing is not locked
  (`src/overlays/shared/pause_00277208.c:688`). The header comment calls
  0xD00 "accept", but the call sites read as "close menu" (likely).

### The held/pressed/released family

All of these are set by `func_002181F0` once per poll:

| Field | Meaning |
|---|---|
| `raw_held` | buttons only (after the mirror swap, before stick directions). `mode` 2 masks it. |
| `held` | `raw_held` plus the left-stick directions as d-pad bits (0x8000/0x2000/0x1000/0x4000) |
| `prev_held` | `held` of the previous poll. UpdatePad restores it from the latch `unk348` before polling. |
| `pressed` | `~prev_held & held`. Plus 0x10000 on a stick flick, and overwritten by key repeat. Forced to 0 while `unk18C != 0` (on pad 0 at `D_0013CA40`). |
| `released` | `~held & prev_held`. Forced to 0 like `pressed`. |
| `raw_pressed` / `raw_released` | `~prev_held & raw_held` / `~held & prev_raw_held`. As retail has it, these mix the raw and stick-augmented masks. Do not "fix" that. |
| `held_unmasked`, `pressed_unmasked`, `released_unmasked` | copies taken **before** the `mode` masks, with the mirror swap undone (so menus see the physical left/right). Menus read these. |
| `no_buttons` | `held == 0` |
| `no_direction` | `(held & 0xF000) == 0` |
| `stick_moved` | `analog[2]` or `analog[3]` nonzero |
| `mode` | one-shot input lock, reset to 0 after it is applied. 1 clears 0x5030 (Up, Down, Triangle, Circle) from held/pressed/released and zeroes `analog[0..1]`. 2 keeps only 0x900 in held/pressed/released/raw_held, sets `no_direction` and zeroes `analog[2..3]`. |

`ClearPadInput` (`func_00218188`) zeroes the current masks (not
`prev_held`/`prev_raw_held`, not `mode`), `stick_moved` and both analog
arrays and sets `no_buttons = no_direction = 1`. It runs while the device
is not ready or its profile is being read.

### Analog values

- `analog[0..3]`: report bytes 2..5, centred on 0x7F, with a dead zone of
  0x30 (|d| < 0x30 reads 0), scaled `(d - 0x30) / 0x4C`, clamped to 1 and
  signed. Only when the report is at least 6 bytes.
- `analog[4..15]`: report bytes 6..17 * (1/255), the pressure values.
  Only when the report is at least 0x12 bytes.
- Stick roles: the code derives the d-pad bits and the flick from
  `analog[2..3]`, so those are the movement stick. In the standard
  DualShock 2 report order bytes 2..5 are right-x, right-y, left-x,
  left-y, which makes `analog[0..1]` the right stick and `analog[2..3]`
  the left stick (inferred from the report order; the header comment on
  `mode` 1 says "left stick" for what the code does to `analog[0..1]`, so
  one of the two is mislabelled).
- Pressure order (inferred from the standard report): right, left, up,
  down, triangle, circle, cross, square, L1, R1, L2, R2.
- `analog_prev` is a straight copy of `analog` taken after the mirror
  negation, then re-negated for mirror like the `_unmasked` masks.

### Mirror option

The flag is the byte at `D_0015EEB0 + 4` = `D_0015EEB4`, which
`config/names.tsv` names `gCheatMirroredLevels` ("Levels are mirrored",
cheat flag 4). It is the mirrored-levels cheat, not a pad setting. When
it is set, `analog[0]` and `analog[2]` (the x axes) are negated and
Left/Right (0x8000/0x2000) are swapped in `held`/`raw_held`. The
`_unmasked` copies and `analog_prev` get the swap undone. pad.h's header
comment gives the US address `D_0015EDB4`.

### Stick history and flick detector

Each poll stores the left-stick magnitude and angle in
`hist_mag[hist_idx]` / `hist_ang[hist_idx]` (rings of 30). If the
magnitude is now > 0.9, the poll looks back:

1. The previous `scale_game_frames(4) - 1` polls (`func_001F98C0`; see the
   `#define` in `src/game/space.c:1738`): the first earlier sample with
   magnitude > 0.9 stops the search. A sample < 0.25 before that is a
   flick.
2. If no flick yet: over the previous `scale_game_frames(5) - 1` polls, if
   `FastDiffRots(old_angle, angle)` > 0.9599311 rad (55 degrees), it is a
   flick.

A flick ORs 0x10000 into `pressed`. Then `hist_btn[hist_idx] = pressed`,
`hist_idx` advances mod 30, and `hist_count` saturates at 30.
`ResetGlobalStateFields` clears `hist_idx`/`hist_count`.

### Key repeat

Only when `rep_rate != 0`. If `held` is nonzero and equal to `prev_held`,
`rep_timer` is decremented. When it reaches 0, or wraps to 0xFF,
`rep_timer = rep_rate` and **`pressed = held`** (the whole held mask, not
only the repeated button). Any change of `held` reloads
`rep_timer = rep_delay`.

### Polling (`func_00217F68`)

`profile_state` 0 reads the button profile (`func_00124DF0` -> `profile`,
with `unk1DC = 0x79` when the first word is 0xFFFFFFFF) and the actuator
block (`func_00125218` -> `act`), then goes to 1. If the profile is
longer than 4 bytes it goes to 2 instead, and `unk18C = 2` either way.
`profile_state` 1 reads the report (`func_00124D18`) into
`func_002181F0` and counts `unk18C` down. While `unk18C` is nonzero,
`pressed`/`released` stay 0, so the first two polls after connecting
produce no edges. `profile_state` 2 clears input. A device state other
than 1 clears input and resets to 0.

## Small shared records (`include/records.h`)

| Record | Size | Layout | Used for |
|---|---|---|---|
| `DispatchRec` | 0x14 | function pointers at +0x08 and +0x10 | entries of the camera and mode dispatch tables (e.g. `D_001E8F80[]` in `src/game/pad.c`) |
| `PadSlots` | 0x2B8 | `use[5]`, `flags[5]`, `sel`, `size[5]` after 0x278 bytes | the slot tables of the pad-driven selector state at `D_001A01F0` |
| `AnimSeq` | >= 0x11 | `nframes` (u8) at +0x10 | animation sequence header (`MobyClass.seqs[]`); only the frame count is read |

## Global singletons

Only addresses that code or `config/names.tsv` supports. Names in
quotes are `config/names.tsv` entries (tier "descriptive", from a PAL
memory-map sheet). The rest come from source comments.

| Address | What | Evidence |
|---|---|---|
| `D_0013F450` | the hero (`Hero`, 0x2300 bytes) | hero.h; 122 files |
| `D_0013F4D0` | hero + 0x80 = `motion.pos`, "gHeroPos" | names.tsv |
| `D_0013CA40` | the pad (`PadState`, 0x350 bytes) | pad.h, `src/game/pad.c` |
| `D_0013A5E0` | base symbol overlay code addresses globals from: `+0x2460` = the pad; `+0x2600`, `+0x2604`, `+0x2634`, `+0x2610` are also used (flags at +0x2600 tested for 0x2/0x8 in `func_L00_00229348`, `src/overlays/shared/help_00221A98.c`). What lives at 0x0013A5E0 itself is not established. | grep `D_0013A5E0 +` |
| `D_0015EE84` | current level index (0..18 for the 19 level overlays, inferred; s32, also read as s16) | `#define current_level_index` in `src/game/space.c:1693`; `1 << D_0015EE84` level masks in vendor code; `% 19` table index in `src/game/pause.c` |
| `D_0015EEB0` | cheat flags, 8 bytes ("gCheats"); `D_0015EEB4` = mirrored levels | names.tsv; `src/game/pad.c` |
| `D_0015EE98` | bolts ("gBolts") | names.tsv |
| `D_0015EEA0` | max nanotech ("gMaxNanotech") | names.tsv |
| `D_0015F6A8` | game mode (0 paused, 2 gameplay, -1 debug menu) ("gGameMode") | names.tsv |
| `D_0015F6F0` | frame counter used as the moby clock: `CreateMoby` reuses a slot only when it is >= the slot's `spawn_frame`, and `DeleteMoby` sets `spawn_frame = D_0015F6F0 + 2` | `src/game/mobyfunc.c:188`, `:383` |
| `D_0013D490` | special-item flags (Zoomerator, Raritanium, Codebot, Premium/Ultra Nanotech) ("gSpecialItems") | names.tsv |
| `D_0013D510` | 30 skill-point flags ("gSkillPoints") | names.tsv |
| `D_0013D5CA` .. `D_0013D5EB` | gadget-owned bytes: Heli-Pack 0x5CA, Morph-o-Ray 0x5DD, Magneboots 0x5E4, Hologuise 0x5E7, Map-o-matic 0x5E9, Persuader 0x5EB | names.tsv |
| `D_0013D618` | galaxy list (planets in order of discovery) | names.tsv |
| `D_001A05C0` / `D_001A08C0` | game save data blob / 20 level save records | names.tsv |
| `D_0014C190` | per-level save bits, indexed `[level][save_id >> 5]` | `Moby.save_id` comment |
| `D_0014C050` | per-level mission rows that `Moby.mission` indexes | `Moby.mission` comment |
| `D_0016001C` .. `D_00160020` | the moby array (pool); `D_00160018` is the base used for the slot index, `D_00160028` the 0x80-byte-per-moby pvars area | `src/game/mobyfunc.c:141-200` |
| `D_00160000` | number of registered moby classes (next class slot) | `src/game/loaders.c:973` |
| `D_001B3E40[oClass]`, `D_001B3C80[slot]`, `D_001B3580[slot]`, `D_001B3900[slot]` | class tables: oClass -> slot (0xFF = not loaded), slot -> oClass, slot -> class header, slot -> update function | `src/game/loaders.c:973`, `src/game/mobyutil.c:175` |

The per-level "collected"/"killed" byte tables that `save_id` indexes
live in each level overlay's own BSS, at a different address per level
(e.g. `D_L01_001BB9C0.collected[...]` in level 1,
`D_L09_001BB6C0.collected[...]` in level 9). There is no single global.

## Addresses: PAL vs US

Header comments that cite `FUN_Lxx_...` lowercase names or US addresses
come from the NTSC decomp (US build). The PAL executable is laid out
differently, and in the 0x0013xxxx data region the US address is 0x100
lower (pad US `D_0013C940` / PAL `D_0013CA40`; hero US `D_0013F350` / PAL
`D_0013F450`; mirror US `D_0015EDB4` / PAL `D_0015EEB4`). Always use the
PAL symbol in code.
