/* NON_MATCHING func_L00_0020E3B8 -- src/overlays/shared/help_0020CDF0.c
 * Best so far: BYTES 30/1596 (98.1% of the bytes match), checked 2026-10-21.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   2026-10-21: Lombyte's near miss (FUN_L00_0020dd20) ported without its empty do { } while (0) loops
 *   (LEVERS.md 9 bans them): BYTES 30/1596. Left: h (&hero) in $fp and i in $s7's place taken by `enabled` (o + 6).
 *   global.c allocates by floor_log2(refs) * refs / live: h 4*19/594 = 0.128, i 4*19/492 = 0.154, enabled 2*4/47 =
 *   0.170, so enabled gets $s6, i $s7, h $fp; retail is h $s6, i $s7, enabled $fp. Lombyte's empty loops raise the
 *   loop depth of the blocks with the PoseAnimSlot calls and the slot load (flow.c weights refs by the depth at the
 *   block head): h 27 refs (0.182), i 21 (0.1707 > 0.1702), which gives retail's order, then BYTES 6 ($v1/$a0 of the
 *   two lui temporaries). Writing hero_0E3B8.x as h->x (or the reverse) changes the code; a for loop, block-local
 *   `enabled`, or dropping it for o[6 + j] do not help; the permuter (1500 s) found nothing.
 */
struct AngleFields_0E3B8 {
    u8 pad[0x5e4];
    f32 unk5E4;
    u8 pad5E8[0x3c];
    f32 unk624, unk628;
    u8 pad62C[0x68];
    f32 unk694;
    u8 pad698[0x3c];
    f32 unk6D4, unk6D8;
};
extern struct AngleFields_0E3B8 anim_tuning_0E3B8 __asm__("D_L00_0017A780") __attribute__((section(".data")));
extern struct Hero hero_0E3B8 __asm__("D_0013F450");
extern s32 anim_class_ids_0E3B8[2] SDATA(D_L00_0015F780);
extern u8 anim_table_index_0E3B8[] __asm__("D_L00_00197F40") NOT_SDA;
extern s32 anim_table_0E3B8[] __asm__("D_L00_00197680") NOT_SDA;
extern u8 anim_buffers_0E3B8[] __asm__("D_L00_0017CB80") NOT_SDA;
extern u8 *hero_slots_0E3B8[2] __asm__("D_00140150") NOT_SDA;
extern f32 frame_scale_0E3B8 __asm__("D_0015EE60") MACRO_ADDR;
extern f32 frame_scale_sq_0E3B8 __asm__("D_0015EE64") MACRO_ADDR;
extern u8 *AllocAnimSlot_0E3B8(struct Moby *, s32) __asm__("func_L00_00250060");
extern void FreeAnimSlot_0E3B8(struct Moby *, u8 **) __asm__("func_L00_00250120");
extern void PoseAnimSlot_0E3B8(struct Moby *, u8 *) __asm__("func_L00_002501C8");
extern f32 ApproachValue_0E3B8(f32 *, f32, f32) __asm__("func_00214D28");
extern s32 anim_group_0E3B8(s32) __asm__("func_L00_002056D0");
extern s32 special_pose_0E3B8(s32, f32 *) __asm__("func_L00_0020DC68");
extern s32 anim_id_0E3B8(struct Moby *, s32) __asm__("func_L00_0020DC50");
extern s32 scale_ticks_0E3B8(s32) __asm__("func_001F9850");

/* Hero hand-attachment update: per item slot, spawn, fade and free the attachment moby and keep its two
   animation entries in step with the hero moby's. Adapted from Lombyte (GPL-3.0) for PAL:
   src/overlays/shared/gameplay/hero/hero_check_damage.c, FUN_L00_0020dd20 (a near miss there too), without its
   empty do-while loops. */
void func_L00_0020E3B8(void) {
    struct Hero *h = &hero_0E3B8;
    s32 i;

    i = 0;
    do {
        u8 *o;
        u8 *t;
        s32 diff;
        struct { s16 v; } w;
        s32 r;
        s32 k;
        s32 j;
        u8 *ids;
        f32 out;
        struct Hero *p6;

        t = hero_slots_0E3B8[i];

        if (t != 0 && *(s32 *)(t + 0x34) != 0) {
            if (ApproachValue_0E3B8((f32 *)(t + 8), 0.0f, frame_scale_0E3B8 * 0.1f) == 0.0f) {
                FreeAnimSlot_0E3B8(h->moby, &hero_slots_0E3B8[i]);
            }
        } else {
            s32 c = h->unk20AB;
            s32 want = 0;

            if (c != 0 && (i == 0 || c == 2)) {
                want = 1;
            }
            if (hero_0E3B8.unk20AF != 0) {
                u8 *slot;
                want = 0;
                slot = hero_slots_0E3B8[i];
                if (slot != 0) {
                    *(s32 *)(slot + 0x34) = 1;
                }
            }
            if (want) {
                struct Hero *p4 = &hero_0E3B8;
                if (hero_slots_0E3B8[i] == 0) {
                    u8 *slot = AllocAnimSlot_0E3B8(p4->moby, anim_class_ids_0E3B8[i]);
                    hero_slots_0E3B8[i] = slot;
                    *(f32 *)(slot + 8) = 0.0f;
                    *(s32 *)(hero_slots_0E3B8[i] + 0x18) = anim_table_0E3B8[anim_table_index_0E3B8[i + 1]];
                }
            } else {
                struct Hero *p5 = &hero_0E3B8;
                u8 *slot = hero_slots_0E3B8[i];
                if (slot == 0) {
                    continue;
                }
                if (ApproachValue_0E3B8((f32 *)(slot + 8), 0.0f, frame_scale_0E3B8 * 0.1f) == 0.0f) {
                    FreeAnimSlot_0E3B8(p5->moby, &hero_slots_0E3B8[i]);
                }
            }
        }

        o = hero_slots_0E3B8[i];
        p6 = &hero_0E3B8;
        if (o == 0) {
            continue;
        }
        {
            u8 *mb = (u8 *)p6->moby;
            diff = mb[0x52] != mb[0x53];
        }
        if (p6->unkD08[0] != 0 && p6->unkD14 == 0) {
            ApproachValue_0E3B8((f32 *)(o + 8), 0.0f, frame_scale_0E3B8 * 0.1f);
        } else if (*(s32 *)(o + 0x34) == 0) {
            ApproachValue_0E3B8((f32 *)(o + 8), 1.0f, frame_scale_0E3B8 * 0.1f);
        }

        w.v = 0;
        if (diff && hero_0E3B8.unkA9C != 0) {
            s32 a = anim_group_0E3B8(o[0x23]);
            s32 b = anim_group_0E3B8(((u8 *)hero_0E3B8.moby)[0x53]);
            if (a != b) {
                w.v = o[0x22] != o[0x23];
            }
        }
        *(s16 *)(o + 0x30) = w.v;

        r = special_pose_0E3B8(((u8 *)h->moby)[0x53], &out);
        if (r != 0 || o[0x32] != 0) {
            if (out != 0.0f) {
                f32 x = frame_scale_sq_0E3B8;
                u8 *tbl = (u8 *)&anim_tuning_0E3B8;
                *(f32 *)(tbl + 0x694) = out;
                *(f32 *)(tbl + 0x5E4) = out;
                *(f32 *)(tbl + 0x6D4) = x * 0.05f;
                *(f32 *)(tbl + 0x6D8) = x * 0.3f;
                *(f32 *)(tbl + 0x624) = x * 0.05f;
                *(f32 *)(tbl + 0x628) = x * 0.3f;
            }
            if (r != 0 && (o[0x32] == 0 || o[0x23] != 0)) {
                {
                    if (o[0x22] == o[0x23] || 0.5f < *(f32 *)(o + 0x24)) {
                        o[0x22] = o[0x23];
                        o[0x20] = o[0x21];
                    }
                    *(f32 *)(o + 0x24) = 0.0f;
                    o[0x23] = 0;
                    o[0x21] = 0;
                    *(f32 *)(o + 0x28) = 1.0f;
                    *(f32 *)(o + 0x2C) = 1.0f / (f32)scale_ticks_0E3B8(0xF);
                    o[0x32] = 1;
                    o[0x33] = 0;
                }
            }
            if (r == 0) {
                if (o[0x33] == 0) {
                    t = (u8 *)h->moby;
                    o[0x23] = anim_id_0E3B8((struct Moby *)t, t[0x53]);
                    o[0x21] = 0;
                    o[0x33] = 1;
                    *(f32 *)(o + 0x24) = 0.0f;
                    *(f32 *)(o + 0x28) = 0.0f;
                } else {
                    f32 f = *(f32 *)(o + 0x24) + frame_scale_0E3B8 * 0.1f;
                    *(f32 *)(o + 0x24) = f;
                    if (1.0f < f) {
                        *(f32 *)(o + 0x24) = 1.0f;
                    }
                    if (*(f32 *)(o + 0x24) == 1.0f) {
                        o[0x32] = 0;
                    }
                }
            } else {
                *(f32 *)(o + 0x28) = 1.0f;
            }
            for (j = 0; j < 2; j++) {
                ids = o + 0x22;
                if (ids[j] < 0x17) {
                    o[6 + j] = 1;
                    *(u8 **)(o + 0x38 + j * 4) = anim_buffers_0E3B8 + (i * 2 + j) * 0x400;
                } else {
                    o[6 + j] = 0;
                }
            }
            PoseAnimSlot_0E3B8(h->moby, o);
        } else {
            u8 *enabled;

            if (*(s16 *)(o + 0x30) == 0) {
                for (k = 0; k < 2; k++) {
                    if (!diff || k == 1) {
                        u8 *p = o + 0x22;
                        t = (u8 *)hero_0E3B8.moby;
                        p[k] = anim_id_0E3B8((struct Moby *)t, *(t + k + 0x52));
                    }
                }
            }
            if (*(s16 *)(o + 0x30) != 0) {
                f32 f = *(f32 *)(o + 0x24) + frame_scale_0E3B8 * 0.15f;
                *(f32 *)(o + 0x24) = f;
                if (1.0f <= f) {
                    o[0x22] = o[0x23];
                    o[0x20] = o[0x21];
                    *(s16 *)(o + 0x30) = 0;
                }
            } else {
                if (!diff) {
                    o[0x20] = ((u8 *)h->moby)[0x50];
                }
                o[0x21] = ((u8 *)h->moby)[0x51];
                *(f32 *)(o + 0x24) = *(f32 *)((u8 *)h->moby + 0x54);
            }
            enabled = o + 6;
            for (j = 0; j < 2; j++) {
                ids = o + 0x22;
                if (ids[j] < 0x17) {
                    enabled[j] = 1;
                    *(u8 **)(o + 0x38 + j * 4) = anim_buffers_0E3B8 + (i * 2 + j) * 0x400;
                } else {
                    enabled[j] = 0;
                }
            }
            PoseAnimSlot_0E3B8(h->moby, o);
        }
    } while (++i < 2);
}
