/* NON_MATCHING func_L01_002F6540 -- src/overlays/shared/vendor_002B90A8.c
 * Best so far: BYTES 12/3984 (99.7% of the bytes match), checked 2026-10-21.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   2026-10-21: Lombyte's near miss (gunship_follow_path) ported to PAL names; BYTES 12/3984. Left: one block,
 *   the case 13/17 call func_0022ED80((rand100 & 1) ? 1 : 2, 0, m). Retail loads $a2 = m before $a1 = 0 (so $a1 lands
 *   in the jal slot) and `addiu $v1,2` before `andi $v0,1`. sched1 breaks those ties by register weight, then luid:
 *   retail's $a2 source must die at the load, i.e. be a pseudo of its own, not m's (a copy the permuter put on the
 *   case 9 path only gave BYTES 7/3984 with it in $s6). A local copy, an inline helper parameter, a local for the
 *   sound id or r, `% 2`, `!= 0`, swapped arms, the int-returning prototype (merges the case 0 tail: SIZE 3968) do not.
 */
typedef unsigned int Q6540 __attribute__((mode(TI), aligned(16)));
typedef union { Q6540 q; f32 f[4]; struct { f32 x, y, z, w; } v; } QVec_6540;

typedef struct {
    s32 count;
    u8 pad4[0xC];
    QVec_6540 node[1];
} PathNodes_6540;

typedef struct {
    s32 idx;
    s8 dir;
    u8 state;
    u8 pad6[2];
    s32 unk8;
    f32 dist;
    PathNodes_6540 *nodes;
    s32 path_id;
    f32 heading;
    s32 unk1C;
} PathCursor_6540;

typedef struct {
    u8 pad0[0x60];
    PathCursor_6540 path;          /* 0x60 */
    QVec_6540 unk80;               /* 0x80 */
    QVec_6540 unk90;               /* 0x90 */
    QVec_6540 unkA0;               /* 0xA0 */
    QVec_6540 unkB0;               /* 0xB0 */
    QVec_6540 unkC0;               /* 0xC0 */
    QVec_6540 unkD0;               /* 0xD0 */
    QVec_6540 unkE0;               /* 0xE0 */
    f32 unkF0;
    f32 unkF4;
    f32 unkF8;
    f32 unkFC;
    f32 unk100;
    f32 unk104;
    f32 unk108;
    f32 unk10C;
    f32 unk110;
    f32 unk114;
    f32 unk118;
    s16 unk11C;
    s16 unk11E;
    u8 unk120;
    u8 unk121;
    s16 unk122;
    f32 unk124;
    f32 unk128;
    f32 unk12C;
    f32 unk130;
    f32 unk134;
    s16 unk138;
    s16 unk13A;
    s16 unk13C;
    s16 unk13E;
    s32 unk140[4];
} Vars_6540;

typedef struct {
    u8 pad0[0x70];
    s32 unk70;
    u8 pad74[0xC];
    QVec_6540 unk80;
} ChildVars_6540;

typedef struct {
    u8 pad0[0x28];
    u16 kind;
} HitInfo_6540;


extern void func_0022ED80_v_6540(int, int, char *) __asm__("func_0022ED80");
extern f32 func_001FA888_6540(s32) __asm__("func_001FA888");
extern s32 func_L01_00276680_6540(struct Moby *, f32) __asm__("func_L01_00276680");
extern s32 func_L00_0025A208_6540(struct Moby **, s32, s32, s32) __asm__("func_L00_0025A208");
extern s32 func_L00_0025A2F0_6540(struct Moby **, struct Moby *, s32, s32) __asm__("func_L00_0025A2F0");
extern s32 func_001F9850_6540(s32) __asm__("func_001F9850");
extern s32 func_L00_0028EB98_6540(struct Moby *, s32) __asm__("func_L00_0028EB98");
extern s32 func_001F9938_6540(s16 *) __asm__("func_001F9938");
extern f32 func_001F9D10_6540(void *, void *) __asm__("func_001F9D10");
extern s32 func_002140B0_6540(s32) __asm__("func_002140B0");
extern s32 func_L00_00258BC8_6540(s32, s32) __asm__("func_L00_00258BC8");
extern s32 func_001FA898_6540(f32) __asm__("func_001FA898");
extern s32 func_001E9730_6540(const char *, ...) __asm__("func_001E9730");
extern f32 func_002140F8_6540(f32, f32) __asm__("func_002140F8");
extern void func_L01_0028C1D8_6540(PathCursor_6540 *) __asm__("func_L01_0028C1D8");
extern s32 func_L01_0028C2D8_6540(void *, PathNodes_6540 *, f32) __asm__("func_L01_0028C2D8");
extern f32 func_L00_001FF860_6540(f32, f32) __asm__("func_L00_001FF860");
extern void func_L01_0028C690_6540(QVec_6540 *, PathCursor_6540 *, s32, f32, f32) __asm__("func_L01_0028C690");
extern f32 func_001F9CE8_6540(void *) __asm__("func_001F9CE8");
extern f32 func_00214158_6540(void) __asm__("func_00214158");
extern f32 func_L01_0028C958_6540(f32, f32, PathCursor_6540 *, s32, struct Moby *) __asm__("func_L01_0028C958");
extern void func_L00_00250800_6540(struct Moby *, s32, QVec_6540 *) __asm__("func_L00_00250800");
extern void func_001F9BF0_6540(void *, void *, void *) __asm__("func_001F9BF0");
extern void func_L00_001FF4B0_6540(void *, void *, f32) __asm__("func_L00_001FF4B0");
extern void func_L00_0028EBF0_6540(s32) __asm__("func_L00_0028EBF0");
extern s32 func_L01_0028C640_6540(PathCursor_6540 *) __asm__("func_L01_0028C640");
extern s32 func_L01_0028C5F0_6540(PathCursor_6540 *, s32) __asm__("func_L01_0028C5F0");
extern f32 func_001F9FA8_6540(f32) __asm__("func_001F9FA8");
extern f32 func_001F9F90_6540(f32) __asm__("func_001F9F90");
extern f32 func_001FA748_6540(f32, f32) __asm__("func_001FA748");
extern f32 func_001FA790_6540(f32, f32) __asm__("func_001FA790");
extern void func_L01_0028C848_6540(f32, QVec_6540 *, QVec_6540 *, QVec_6540 *, QVec_6540 *, QVec_6540 *) __asm__("func_L01_0028C848");
extern f32 func_L00_00259148_6540(f32 *, f32, f32, f32, f32, f32) __asm__("func_L00_00259148");
extern f32 func_L00_0025F368_6540(f32) __asm__("func_L00_0025F368");
extern void func_001F9BC0_6540(void *) __asm__("func_001F9BC0");
extern void func_L01_00280428_6540(QVec_6540 *, QVec_6540 *) __asm__("func_L01_00280428");
extern void func_001F9C30_6540(void *, void *, f32) __asm__("func_001F9C30");
extern void func_001F9BD8_6540(void *, void *, void *) __asm__("func_001F9BD8");
extern void func_L00_0025F4A8_6540(struct Moby *, QVec_6540 *, QVec_6540 *, f32, f32, s32, s32, s32, f32, f32, f32, f32, s32, f32, s32, s32, s32, s32) __asm__("func_L00_0025F4A8");
extern int play_sound_6540(int, int, void *) __asm__("func_0022ED80");
extern void func_L01_002F6418_6540(char *, float *) __asm__("func_L01_002F6418");
extern char * func_L00_0025B478_6540(void *, int, int) __asm__("func_L00_0025B478");
extern s32 gunships_off_6540 SDATA(D_L01_00161B90);
extern f32 frame_scale_6540 __asm__("D_0015EE60") MACRO_ADDR;
extern f32 frame_time_6540 __asm__("D_0015EE6C") MACRO_ADDR;
extern f32 frame_time_sq_6540 __asm__("D_0015EE70") MACRO_ADDR;
extern s32 current_level_6540 __asm__("D_0015EE84") MACRO_ADDR;
extern void * last_update_6540 __asm__("D_L01_001607F0") MACRO_ADDR;
extern char * mobys_6540 __asm__("D_L01_00160058") MACRO_ADDR;
extern char target_pos_6540[] __asm__("D_L01_001672C0");
extern char * paths_6540[] __asm__("D_L01_001B0C30");
extern char path_warning_6540[] __asm__("D_L01_0020B6D0");
extern char voice_pool_6540[] __asm__("D_0013E650");
extern s32 level_frame_count_6540 __asm__("D_L01_0015F6B0") MACRO_ADDR;

/* Gunship path follower (shared by the levels that fly gunships). Adapted from Lombyte (GPL-3.0) for PAL:
   src/overlays/shared/gameplay/entities/novalis_elevator.c, gunship_follow_path (FUN_L01_002f5168), itself a
   near miss there. PAL calls the level copies of atan2 and normalize, func_001F9850 for the frame scaling and
   func_L00_0028EBF0 to stop a voice; D_L01_001607F0 is stored with the lui $at macro (MACRO_ADDR). */
void func_L01_002F6540(struct Moby *m) {
    QVec_6540 d20;
    QVec_6540 zero;
    QVec_6540 rot40;
    QVec_6540 pos50;
    QVec_6540 off[4];
    QVec_6540 dA0;
    struct Moby *it;
    Vars_6540 *v;
    PathCursor_6540 *p;
    s32 frozen;
    s32 i;
    s32 never = 0;
    f32 path_speed;
    f32 f24;

    zero.q = 0;
    frozen = 0;
    v = (Vars_6540 *)m->pvars;
    if (m->oclass != 0x336) {
        frozen = func_L01_00276680_6540(m, func_001FA888_6540(m->draw_dist)) == -1;
    }
    if (v == 0 || gunships_off_6540 != 0) {
        return;
    }
    p = &v->path;
    if (m->group != 0xFF && m->state != 0 && m->state != 3) {
        s32 bad = 0;
        func_L00_0025A208_6540(&it, m->group, 0, 0);
        if (it == m) {
            goto check;
            do {
                func_L00_0025A2F0_6540(&it, it, 0, 0);
            check:
                if (it == 0)
                    goto done;
            } while (func_L01_00276680_6540(it, func_001FA888_6540(it->draw_dist)) == -1);
            bad = 1;
        done:
            if (bad == 0) {
                func_L00_0025A208_6540(&it, m->group, 0, 0);
                while (it != 0) {
                    Vars_6540 *iv = (Vars_6540 *)it->pvars;
                    bad = 3;
                    it->state = bad;
                    iv->unk11C = func_001F9850_6540(5);
                    func_L00_0025A2F0_6540(&it, it, 0, 0);
                }
            }
        }
    }
    if ((m->oclass == 0x33 || m->oclass == 0x46A || m->oclass == 0x473 || m->oclass == 0x474)
        && (level_frame_count_6540 & 7) == (((s32)m >> 8) & 7) && v->unk13A <= 0 && m->state != 0
        && func_L00_0028EB98_6540(m, v->unk11E) == 0) {
        v->unk11E = play_sound_6540(0, 4, m);
    }
    switch ((s16)(m->oclass - 0x33)) {
    case 0:
    case 9:
    case 23:
        if (v->unk138 != -1) {
            if (!func_001F9938_6540(&v->unk138))
                break;
            if (func_001F9D10_6540(&m->pos, target_pos_6540) < 30.0f)
                play_sound_6540(1, 0, m);
        }
        v->unk138 = func_001F9850_6540(func_L00_00258BC8_6540(v->unk13C, v->unk13C + v->unk13E));
        break;
    case 13:
    case 17:
        if (v->unk138 != -1) {
            if (!func_001F9938_6540(&v->unk138))
                break;
            func_0022ED80_v_6540((func_002140B0_6540(100) & 1) ? 1 : 2, 0, (char *)m);
        }
        v->unk138 = func_001F9850_6540(func_L00_00258BC8_6540(v->unk13C, v->unk13C + v->unk13E));
        break;
    }

    switch (m->state) {
    case 0:
        if (p->path_id != -1) {
            PathNodes_6540 *pn = (PathNodes_6540 *)paths_6540[p->path_id];
            if (func_001F9D10_6540(&pn->node[0], &pn->node[pn->count - 1]) < 0.5f)
                pn->count--;
        } else {
            func_001E9730_6540(path_warning_6540, m->oclass, func_001FA898_6540((f32)(m - (struct Moby *)mobys_6540)));
        }
        m->update_dist = 0xFF;
        v->unk11E = -1;
        if (500.0f < v->unk130)
            v->unk130 = func_002140F8_6540(-1.0f, 1.0f) * v->unk130 / 1000.0f;
        if (500.0f < v->unk134)
            v->unk134 = func_002140F8_6540(-1.0f, 1.0f) * v->unk134 / 1000.0f;
        if (p->path_id == -1) {
            m->state = 2;
            return;
        }
        v->unk121 = (m - (struct Moby *)mobys_6540) / 2 % 7;
        func_L01_0028C1D8_6540(p);
        if (p->state == 0) {
            if (p->idx <= 0) {
                s32 r = func_L01_0028C2D8_6540(&m->pos, p->nodes, 0.0f);
                p->idx = r;
                if ((p->dir >= 0 && r == p->nodes->count - 1) || (p->dir < 0 && r == 0))
                    p->idx = 0;
            }
            p->dist = func_001F9D10_6540(&m->pos, &p->nodes->node[p->idx]);
            p->heading = func_L00_001FF860_6540(p->nodes->node[p->idx].v.x - m->pos.x,
                                      p->nodes->node[p->idx].v.y - m->pos.y);
            p->state = 2;
            p->unk8 = 0;
            p->unk1C = 0;
        }
        v->unkE0.q = (p->nodes->node + p->idx)->q;
        v->unkF0 = 1.0f;
        func_L01_0028C690_6540(&v->unkA0, p, 0, v->unkF4, v->unkF8);
        v->unkB0.q = v->unkC0.q = (p->nodes->node + p->idx)->q;
        *(s32 *)&m->rot.x = 0;
        m->rot.y = -func_L00_001FF860_6540(func_001F9CE8_6540(&v->unkA0), v->unkA0.v.z);
        m->rot.z = func_L00_001FF860_6540(v->unkA0.v.x - zero.v.x, v->unkA0.v.y - zero.v.y);
        v->unk104 = 0;
        v->unk10C = 0;
        *(u128 *)&m->pos = v->unkE0.q;
        func_L01_002F6418_6540((char *)m, (float *)&m->pos);
        v->unk121 = 7;
        if (6.2831855f < v->unk12C) {
            v->unk12C = v->unk12C * 0.017453292f;
        } else if (v->unk124 != 0.0f && v->unk12C == -1.0f) {
            v->unk12C = func_00214158_6540();
        }
        v->unkD0.q = v->unkC0.q;
        func_L01_0028C958_6540(v->unkF4, v->unkF8, p, v->unk120 & 0x20, m);
        if ((v->unk120 & 0x1E) && !(v->unk120 & 1)) {
            s32 i;
            for (i = 0; i < 4; i++) {
                struct Moby *c;
                ChildVars_6540 *cv;
                if (!(v->unk120 & (2 << i)) || v->unk140[i] == -1)
                    continue;
                c = (struct Moby *)mobys_6540 + v->unk140[i];
                if (c == 0 || (cv = (ChildVars_6540 *)c->pvars) == 0)
                    continue;
                cv->unk70 |= 0x1000;
                func_L00_00250800_6540(m, i, &off[i]);
                func_001F9BF0_6540(&dA0, &off[i], &m->pos);
                func_L00_001FF4B0_6540(&dA0, &dA0, 1.0f);
                *(u128 *)&c->pos = off[i].q;
                cv->unk80.q = dA0.q;
            }
        }
        if (v->unk11C > 0) {
            m->state = 3;
            return;
        }
        if (current_level_6540 == 3 || current_level_6540 == 9)
            v->unk13A = 0;
        m->state = 1;
        return;

    case 2:
        if (p->path_id == -1)
            return;
        m->state = 0;
        if (v->unk11E != -1) {
            u8 *e = (u8 *)voice_pool_6540 + v->unk11E * 0x70;
            if (*(struct Moby **)(e + 0x88) == m && e[0x74] != 0)
                func_L00_0028EBF0_6540(v->unk11E);
        }
        v->unk11E = -1;
        return;

    case 3:
        if (func_001F9938_6540(&v->unk11C)) {
            v->unk11C = 0;
            m->state = 1;
        }
        return;

    case 1:
        break;

    default:
        return;
    }

    rot40.q = *(u128 *)&m->rot;
    pos50.q = *(u128 *)&m->pos;
    if (1.0f <= v->unkF0) {
        s32 n;
        v->unkF0 -= 1.0f;
        n = func_L01_0028C640_6540(p);
        v->unk90.q = v->unkA0.q;
        func_L01_0028C690_6540(&v->unkA0, p, 1, v->unkF4, v->unkF8);
        v->unkB0.q = v->unkC0.q;
        v->unkC0.q = (p->nodes->node + func_L01_0028C5F0_6540(p, n + 1))->q;
    }
    {
        f32 s = func_001F9FA8_6540(v->unk12C);
        path_speed = v->unkFC + v->unk124 * frame_scale_6540 * s * frame_scale_6540;
        f24 = path_speed / v->unkB0.v.w;
        v->unk12C = func_001FA748_6540(v->unk12C, v->unk128 * frame_scale_6540 * 0.017453292f);
    }
    v->unkE0.q = v->unkD0.q;
    func_L01_0028C848_6540(v->unkF0 + f24, &v->unkD0, &v->unkB0, &v->unkC0, &v->unk90, &v->unkA0);
    if (!(v->unk120 & 0x40)) {
        f32 dd = func_001F9D10_6540(&v->unkE0, &v->unkD0);
        if (dd != 0.0f) {
            f24 = f24 * (path_speed / dd);
            func_L01_0028C848_6540(v->unkF0 + f24, &v->unkD0, &v->unkB0, &v->unkC0, &v->unk90, &v->unkA0);
        }
    }
    if (!frozen) {
        f32 k = 0.017453292f;
        f32 a;
        f32 turn;
        f32 lim = (f32)v->unk122 * k;
        func_001FA790_6540(m->rot.z, lim);
        a = func_L00_001FF860_6540(v->unkD0.v.x - v->unkE0.v.x, v->unkD0.v.y - v->unkE0.v.y);
        a = func_001FA748_6540(a, lim);
        a = func_001FA790_6540(a, m->rot.z);
        turn = a * (1.0f / v->unk110 * frame_scale_6540);
        m->rot.z = func_001FA748_6540(turn, m->rot.z);
        func_001F9BF0_6540(&d20, &v->unkD0, &v->unkE0);
        a = -func_L00_001FF860_6540(func_001F9CE8_6540(&d20), d20.v.z);
        m->rot.y = func_L00_00259148_6540(&v->unk10C, m->rot.y, a,
                                            v->unk108 * 10.0f * k * frame_time_sq_6540,
                                            v->unk108 * 5.0f * k * frame_time_sq_6540,
                                            v->unk108 * k * frame_time_6540);
        a = func_L00_0025F368_6540(turn * v->unk114);
        a = func_001FA790_6540(m->rot.x, a);
        a = func_L00_0025F368_6540(a * v->unk118);
        m->rot.x = func_L00_00259148_6540(&v->unk104, m->rot.x, a,
                                            v->unk100 * 10.0f * k * frame_time_sq_6540,
                                            v->unk100 * 5.0f * k * frame_time_sq_6540,
                                            v->unk100 * k * frame_time_6540);
    } else {
        func_001F9BC0_6540(&m->rot);
        m->rot.z = func_L00_001FF860_6540(v->unkD0.v.x - v->unkE0.v.x, v->unkD0.v.y - v->unkE0.v.y);
    }
    v->unkF0 += f24;
    *(u128 *)&m->pos = v->unkE0.q;
    if (!frozen)
        func_L01_002F6418_6540((char *)m, (float *)&m->pos);
    if (v->unk120 & 0x1F) {
        s32 n2 = 0;
        s32 i;
        for (i = 0; i < 5; i++) {
            if (!((v->unk120 >> i) & 1))
                continue;
            func_L00_00250800_6540(m, n2, &off[0]);
            n2++;
            func_001F9BF0_6540(&off[1], &off[0], &m->pos);
            if (!(v->unk120 & 1)) {
                struct Moby *c;
                ChildVars_6540 *cv;
                if (v->unk140[i - 1] == -1)
                    continue;
                c = (struct Moby *)mobys_6540 + v->unk140[i - 1];
                if (c == 0 || (cv = (ChildVars_6540 *)c->pvars) == 0)
                    continue;
                func_L00_001FF4B0_6540(&off[1], &off[1], 1.0f);
                cv->unk70 |= 0x1000;
                *(u128 *)&c->pos = off[0].q;
                cv->unk80.q = off[1].q;
            } else if (v->unk121 == 0) {
                func_L00_001FF4B0_6540(&off[1], &off[1], func_002140F8_6540(0.016f, 0.031f) * frame_scale_6540);
                func_L01_00280428_6540(&off[0], &off[1]);
                func_L00_001FF4B0_6540(&off[1], &off[1], func_002140F8_6540(0.016f, 0.031f) * frame_scale_6540);
                func_L01_00280428_6540(&off[0], &off[1]);
            }
        }
        if (v->unk121 == 0)
            v->unk121 = func_001F9850_6540(7);
        v->unk121--;
    }
    if (current_level_6540 != 3 && current_level_6540 != 9)
        return;
    {
        HitInfo_6540 *hit = (HitInfo_6540 *)func_L00_0025B478_6540(m, 0x210000, 0);
        if (hit != 0 && hit->kind != 0x102) {
            QVec_6540 *src;
            s32 k;
            QVec_6540 *dir;
            func_001F9BC0_6540(&off[0]);
            if (m->oclass == 0x33) {
                off[0].v.x = func_001F9F90_6540(m->rot.z) * -2.7470f;
                off[0].v.y = func_001F9FA8_6540(m->rot.z) * -2.7470f;
                *(s32 *)&off[0].v.z = 0;
            }
            src = (QVec_6540 *)&m->pos;
            func_001F9BF0_6540(&off[2], src, &pos50);
            off[1].q = off[2].q;
            func_001F9C30_6540(&off[1], &off[1], 0.5f);
            func_001F9BD8_6540(&off[0], &off[0], src);
            v->unk13A = func_001F9850_6540(600);
            dir = &off[1];
            /* Fakematch: the never-taken call keeps gcse from propagating &off[1] into the call, so dir
               gets a1 before the class tests and &off[1] gets s0, as in retail. */
            if (never)
                dir = (QVec_6540 *)func_001F9850_6540(0);
            k = -1;
            if (m->oclass == 0x532 || m->oclass == 0x53B || m->oclass == 0x53C)
                k = 1;
            func_L00_0025F4A8_6540(m, dir, &off[0], 0.0f, 0.0f, 0x14, 3, 4, 4.0f, 2.0f, 100000.0f, 3.0f, k, 15.0f, 1,
                             1, -1, 0);
        }
    }
    last_update_6540 = (void *)func_L01_002F6540;
    m->hit_slot = 0xFF;
    if (v->unk13A == 0)
        return;
    if (func_001F9938_6540(&v->unk13A) && frozen) {
        m->collision = ((u32 *)m->pclass)[4];
        m->flags = (m->flags & 0xFFBE) | 0x1000;
        return;
    }
    m->collision = 0;
    m->flags |= 0x41;
    if (v->unk11E != -1) {
        u8 *e = (u8 *)voice_pool_6540 + v->unk11E * 0x70;
        if (*(struct Moby **)(e + 0x88) == m && e[0x74] != 0)
            func_L00_0028EBF0_6540(v->unk11E);
    }
    v->unk11E = -1;
    m->flags &= 0xEFFF;
}
