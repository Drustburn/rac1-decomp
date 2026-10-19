/* NON_MATCHING func_L15_002D7C00 -- src/overlays/shared/vendor_002D7C00.c
 * Best so far: SIZE ours 3996 / retail 4012, checked 2026-10-19.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   - Length scan is `len = 8; for (ri = 0; ri < 8; ri++) if (best[ri] == -1) { len = ri; break; }` (retail peels 
 *   - armed: `int armed = 1;` at the top; case 3 tests `if (pv->armed != 0) {...}` then `armed = 0;` once at the e
 *   - Light store order f0=1.0; [0x1C]=8.0; f4=1.0; f8=0.5; iC=0.
 *   - Final loop: `char *pos = D_0013F4D0; g = (Hero*)(pos - 0x80)` so s3 = s2 - 0x80 is derived from the register
 *   - Case 2 sw loop: first test `D + sw*256` (mult, index-first addu as retail) instead of `<<8`.
 *   Remaining differences (all in two spots):
 *   1. Case 2 switch-press loop: retail reloads pv->sw[k] and re-tests -1 in the `||` arm (lw/beq/sll before func_
 *   2. Case 3 owner release: ours `beql` (annulled) where retail has `beqz` with the same `daddu s3,s2` slot.
 */
/* fcheck: unchecked (level/overlay function; fcheck runs boot-executable functions only; OpenRAC tools/fcheck_gate.py, 2026-10-18). */
/* func_L15_002D7C00 -- src/overlays/shared/vendor_002D7C00.c (functional C for the port, not a match)
 * Update of the electrified water (class 655, levels 15 and 17). Out of its area cuboid (any state
 * but 0) it releases its hum and its point light. State 0 resets its switches (floor switches 0x33E:
 * their done bytes and save bits cleared; others through func_L15_002DA990) and turns on. State 1
 * (on): the on timer running out turns it to 2; Ratchet in the water and in one of its 12 water
 * cuboids is shocked (hum moved onto him, fog saved and whitened, a bolt from his body, a hit);
 * else sparks jump between two points of the line nearest him (bolt, point light). States 1 and 2:
 * the lamps flicker (steady in 2), the hum follows the lamp nearest the camera, the off timer
 * running out in 2 turns it back on, a pressed switch turns it off (state 3) for pvars +0x18
 * seconds. State 3: lamps steady, light and hum off, countdown drawn; standing on a floor switch
 * when re-armed tops the time up; the time out or Ratchet in a reset cuboid turns it back on.
 * Every frame: Ratchet in a water-level cuboid gets its water level.
 * Adapted with ReRAC's water_shock.rs (ISC), which documents the US twin (level15 0x2d6810).
 * equiv: DIFFERENT, but by code shape only as far as read: the remaining items are values the
 * tool cannot place (a base address rebuilt after a label or in a delay slot in retail: D_L15_001BB334,
 * D_0013F4D0, the hero block; the 0xFF in m[0x30] set before a label), branch polarity and loop
 * induction (the line-length count, the line loop's counter), and one reload of pv->sw[k] that retail
 * repeats before func_L11_00310738 and this CSEs. Not yet proven by the run-time check. */
typedef unsigned int Q_2D7C00 __attribute__((mode(TI), aligned(16)));
typedef struct {
    int sw[4];
    short onT;
    short offT;
    int voice0;
    float offSecs;
    int shocks;
    int water[12];
    float fog[4];
    int levels[4];
    int resetA;
    int lamps[3];
    int voice1;
    unsigned char *owner;
    unsigned char fogRGB[4];
    int armed;
    int lines[3][8];
    Q_2D7C00 start;
    Q_2D7C00 end;
    Q_2D7C00 aim;
    int light;
    int area;
    int resetB;
} Pv_2D7C00;
typedef struct {
    float v[4];
    unsigned char *attacker;
    int flags;
    unsigned char b28;
    unsigned char b29;
    unsigned short oclass;
    float damage;
    char reserved[16];
} Hit_2D7C00;
typedef struct {
    unsigned char pad0[0xD0];
    Q_2D7C00 body;
    unsigned char padE0[0x2F0 - 0xE0];
    float waterLevel;
    unsigned char pad2F4[0x2FC - 0x2F4];
    unsigned char *standing;
    unsigned char pad300[0x30E - 0x300];
    short airTicks;
    unsigned char pad310[0x2080 - 0x310];
    unsigned char *moby;
} Hero_2D7C00;
typedef struct {
    float f0, f4, f8;
    int iC;
    Q_2D7C00 pos;
    unsigned char pad20[0x1C - 0x20 + 0x20];
} Light_2D7C00;

extern Hero_2D7C00 G_2D7C00 __asm__("D_0013F450");
extern char D_0013F4D0_2D7C00[] __asm__("D_0013F4D0");
extern unsigned char D_0013E650_2D7C00[] __asm__("D_0013E650");
extern unsigned char *D_L15_00160058_2D7C00 __asm__("D_L15_00160058") MACRO_ADDR;
extern char *D_L15_0016016C_2D7C00 __asm__("D_L15_0016016C") MACRO_ADDR;
extern int D_0015EE84_2D7C00 SDATA(D_0015EE84);
extern float D_L15_00161C48_2D7C00 SDATA(D_L15_00161C48);
extern float D_L15_00161C4C_2D7C00 SDATA(D_L15_00161C4C);
extern float D_L15_00161C50_2D7C00 SDATA(D_L15_00161C50);
extern unsigned char D_L15_001BB334_2D7C00[] __asm__("D_L15_001BB334");
extern unsigned char D_L15_001BBF94_2D7C00[] __asm__("D_L15_001BBF94");
extern int D_0014C290_2D7C00[] __asm__("D_0014C290");
extern int D_L15_001BADE0_2D7C00[] __asm__("D_L15_001BADE0");
extern float D_L15_00167440_2D7C00[] __asm__("D_L15_00167440");
extern char D_L15_00180940_2D7C00[] __asm__("D_L15_00180940");
extern unsigned char D_L15_0015F544_2D7C00[4] __asm__("D_L15_0015F544") MACRO_ADDR;
extern float D_L15_0015F548_2D7C00 __asm__("D_L15_0015F548") MACRO_ADDR;
extern float D_L15_0015F54C_2D7C00 __asm__("D_L15_0015F54C") MACRO_ADDR;
extern float D_L15_0015F550_2D7C00 __asm__("D_L15_0015F550") MACRO_ADDR;
extern float D_L15_0015F554_2D7C00 __asm__("D_L15_0015F554") MACRO_ADDR;
extern int D_L15_0015F548_i_2D7C00 SDATA(D_L15_0015F548);
extern int D_L15_0015F554_i_2D7C00 SDATA(D_L15_0015F554);

extern int func_00215570_2D7C00(void *, int) __asm__("func_00215570");
extern void func_L00_0028EBF0_2D7C00(int) __asm__("func_L00_0028EBF0");
extern void func_L00_0023F1D0_2D7C00(int) __asm__("func_L00_0023F1D0");
extern void func_L15_002DA990_2D7C00(void *) __asm__("func_L15_002DA990");
extern float func_001F9878_2D7C00(float) __asm__("func_001F9878");
extern int func_001FA898_2D7C00(float) __asm__("func_001FA898");
extern int func_001F9850_2D7C00(int) __asm__("func_001F9850");
extern int func_L15_002D7B60_2D7C00(void) __asm__("func_L15_002D7B60");
extern int func_001F9938_2D7C00(void *) __asm__("func_001F9938");
extern void func_L00_00251328_2D7C00(void *, int, int, int) __asm__("func_L00_00251328");
extern int func_L00_00258BC8_2D7C00(int, int) __asm__("func_L00_00258BC8");
extern int func_002140B0_2D7C00(int) __asm__("func_002140B0");
extern float func_001F9D10_2D7C00(void *, void *) __asm__("func_001F9D10");
extern int func_L00_0023F0D0_2D7C00(void *, float, float, float, float, float) __asm__("func_L00_0023F0D0");
extern void func_L15_002D8DD8_2D7C00(void *) __asm__("func_L15_002D8DD8");
extern void func_L15_002D8FA0_2D7C00(void *) __asm__("func_L15_002D8FA0");
extern void func_L15_002D9B00_2D7C00(void) __asm__("func_L15_002D9B00");
extern void func_L15_002D8BB0_2D7C00(void) __asm__("func_L15_002D8BB0");
extern void func_001F49B0_2D7C00(void (*)(void), void *) __asm__("func_001F49B0");
extern int func_L00_0028EF68_2D7C00(int, int, void *, int) __asm__("func_L00_0028EF68");
extern float func_002140F8_2D7C00(float, float) __asm__("func_002140F8");
extern float func_00214158_2D7C00(void) __asm__("func_00214158");
extern void func_00215C00_2D7C00(void *, float, float, float) __asm__("func_00215C00");
extern void func_L00_001FF240_2D7C00(void *, void *, void *) __asm__("func_L00_001FF240");
extern float func_001F9F90_2D7C00(float) __asm__("func_001F9F90");
extern float func_001F9FA8_2D7C00(float) __asm__("func_001F9FA8");
extern void func_L00_001FF500_2D7C00(void *, void *, float) __asm__("func_L00_001FF500");
extern void func_L00_0025AAC0_2D7C00(void *, void *) __asm__("func_L00_0025AAC0");
extern float func_001F9D48_2D7C00(void *, void *) __asm__("func_001F9D48");
extern int func_L00_0028EB98_2D7C00(void *, int) __asm__("func_L00_0028EB98");
extern int func_L11_00310738_2D7C00(void *) __asm__("func_L11_00310738");
extern int func_L00_0020DC00_2D7C00(void) __asm__("func_L00_0020DC00");

void func_L15_002D7C00(unsigned char *m) {
    float tmp[4];
    Hit_2D7C00 hit;
    Pv_2D7C00 *pv = *(Pv_2D7C00 **)(m + 0x78);
    int armed = 1;
    int wet, inside, n, r, len, ri;
    int k1, k2, k3, k4, k5, k6, k7, k8, k9, k10, k11;
    int *best, *line;
    unsigned char *near, *o;
    unsigned short u;
    float d1, a, b;

    if (m[0x20] != 0 && func_00215570_2D7C00(D_0013F4D0_2D7C00, pv->area) == 0) {
        if (pv->owner != 0) {
            if ((pv->voice0) != -1) {
                unsigned char *e_ = D_0013E650_2D7C00 + (pv->voice0) * 0x70;
                if (*(unsigned char **)(e_ + 0x88) == (pv->owner) && e_[0x74] != 0) {
                    func_L00_0028EBF0_2D7C00(pv->voice0);
                }
            }
            pv->voice0 = -1;
            if ((pv->voice1) != -1) {
                unsigned char *e_ = D_0013E650_2D7C00 + (pv->voice1) * 0x70;
                if (*(unsigned char **)(e_ + 0x88) == (pv->owner) && e_[0x74] != 0) {
                    func_L00_0028EBF0_2D7C00(pv->voice1);
                }
            }
            pv->voice1 = -1;
        }
        if (pv->light != -1) {
            func_L00_0023F1D0_2D7C00(pv->light);
            pv->light = -1;
        }
        return;
    }

    switch (m[0x20]) {
    case 0:
        m[0x30] = 0xFF;
        for (k1 = 0; k1 < 4; k1++) {
            if (pv->sw[k1] == -1) {
                continue;
            }
            if (*(short *)((D_L15_00160058_2D7C00 + ((pv->sw[k1]) << 8)) + 0xA6) == 0x33E) {
                D_L15_001BB334_2D7C00[*(short *)((D_L15_00160058_2D7C00 + ((pv->sw[k1]) << 8)) + 0xB2)] = 0;
                D_L15_001BBF94_2D7C00[*(short *)((D_L15_00160058_2D7C00 + ((pv->sw[k1]) << 8)) + 0xB2)] = 0;
                u = *(unsigned short *)((D_L15_00160058_2D7C00 + ((pv->sw[k1]) << 8)) + 0xB2);
                *(int *)((char *)D_0014C290_2D7C00 + (((short)u >> 5) << 2) + (D_0015EE84_2D7C00 << 8)) &=
                    ~(1 << (u & 0x1F));
                u = *(unsigned short *)((D_L15_00160058_2D7C00 + ((pv->sw[k1]) << 8)) + 0xB2);
                *(int *)((char *)D_L15_001BADE0_2D7C00 + (((short)u >> 5) << 2)) &= ~(1 << (u & 0x1F));
            } else {
                func_L15_002DA990_2D7C00((D_L15_00160058_2D7C00 + ((pv->sw[k1]) << 8)));
            }
        }
        pv->onT = func_001FA898_2D7C00(func_001F9878_2D7C00((D_L15_00161C48_2D7C00) * 60.0f));
        pv->offT = func_001F9850_2D7C00(200);
        pv->shocks = 0;
        pv->light = -1;
        m[0x20] = 1;
        pv->voice1 = -1;
        pv->voice0 = -1;
        break;

    case 1:
        inside = 0;
        wet = func_L15_002D7B60_2D7C00();
        if (func_001F9938_2D7C00(&pv->onT) != 0) {
            pv->offT = func_001FA898_2D7C00(func_001F9878_2D7C00((D_L15_00161C4C_2D7C00) * 60.0f));
            m[0x20] = 2;
            if (pv->owner != 0) {
                if ((pv->voice1) != -1) {
                    unsigned char *e_ = D_0013E650_2D7C00 + (pv->voice1) * 0x70;
                    if (*(unsigned char **)(e_ + 0x88) == (pv->owner) && e_[0x74] != 0) {
                        func_L00_0028EBF0_2D7C00(pv->voice1);
                    }
                }
                pv->voice1 = -1;
            }
        }
        for (k2 = 0; k2 < 12; k2++) {
            if (func_00215570_2D7C00(D_0013F4D0_2D7C00, pv->water[k2]) != 0) {
                inside = 1;
                break;
            }
        }
        if (m[0x20] == 2) {
            for (k3 = 0; k3 < 3; k3++) {
                if (pv->lamps[k3] != -1) {
                    func_L00_00251328_2D7C00((D_L15_00160058_2D7C00 + ((pv->lamps[k3]) << 8)), 0x40, 0x40, 0x40);
                }
            }
        } else {
            for (k4 = 0; k4 < 3; k4++) {
                r = func_L00_00258BC8_2D7C00(0x40, 0x7F);
                if (pv->lamps[k4] != -1) {
                    func_L00_00251328_2D7C00((D_L15_00160058_2D7C00 + ((pv->lamps[k4]) << 8)), r, r, r);
                }
            }
        }

        if ((!inside || !wet) && func_002140B0_2D7C00(1) == 0 && D_L15_00161C50_2D7C00 == 0.0f) {
            best = 0;
            for (k5 = 0; k5 < 24; k5 += 8) {
                line = pv->lines[0] + k5;
                if (line[0] == -1) {
                    continue;
                }
                if (best != 0) {
                    d1 = func_001F9D10_2D7C00((D_L15_0016016C_2D7C00 + ((best[0]) << 7)) + 0x30, D_0013F4D0_2D7C00);
                    if (!(func_001F9D10_2D7C00((D_L15_0016016C_2D7C00 + ((line[0]) << 7)) + 0x30, D_0013F4D0_2D7C00) < d1)) {
                        continue;
                    }
                }
                best = line;
            }
            if (best != 0) {
                len = 8;
                for (ri = 0; ri < 8; ri++) {
                    if (best[ri] == -1) {
                        len = ri;
                        break;
                    }
                }
                if (len >= 2) {
                    best += func_L00_00258BC8_2D7C00(0, len - 2);
                    pv->start = *(Q_2D7C00 *)((D_L15_0016016C_2D7C00 + ((best[0]) << 7)) + 0x30);
                    pv->end = pv->aim = *(Q_2D7C00 *)((D_L15_0016016C_2D7C00 + ((best[1]) << 7)) + 0x30);
                    if (pv->light == -1) {
                        pv->light = func_L00_0023F0D0_2D7C00(&pv->start, 8.0f, 0.0f, 1.0f, 1.0f, 0.5f);
                    } else {
                        Light_2D7C00 *l = (Light_2D7C00 *)(D_L15_00180940_2D7C00 + (pv->light << 5));
                        qcopy(&l->pos, &pv->start);
                        l->f0 = 1.0f;
                        *(float *)((char *)l + 0x1C) = 8.0f;
                        l->f4 = 1.0f;
                        l->f8 = 0.5f;
                        l->iC = 0;
                    }
                    func_L15_002D8DD8_2D7C00(m);
                    func_L15_002D8FA0_2D7C00(m);
                    func_001F49B0_2D7C00(func_L15_002D9B00_2D7C00, m);
                }
            }
        }

        if (inside && wet) {
            if (pv->owner != 0 && pv->owner != G_2D7C00.moby) {
                if ((pv->voice0) != -1) {
                    unsigned char *e_ = D_0013E650_2D7C00 + (pv->voice0) * 0x70;
                    if (*(unsigned char **)(e_ + 0x88) == (pv->owner) && e_[0x74] != 0) {
                        func_L00_0028EBF0_2D7C00(pv->voice0);
                    }
                }
                pv->voice0 = -1;
                if ((pv->voice1) != -1) {
                    unsigned char *e_ = D_0013E650_2D7C00 + (pv->voice1) * 0x70;
                    if (*(unsigned char **)(e_ + 0x88) == (pv->owner) && e_[0x74] != 0) {
                        func_L00_0028EBF0_2D7C00(pv->voice1);
                    }
                }
                pv->voice1 = -1;
            }
            pv->owner = G_2D7C00.moby;
            pv->voice0 = func_L00_0028EF68_2D7C00(0, 4, G_2D7C00.moby, 0x28F);
            pv->voice1 = func_L00_0028EF68_2D7C00(1, 4, G_2D7C00.moby, 0x28F);
            if (pv->shocks == 0) {
                pv->fog[0] = D_L15_0015F548_2D7C00;
                pv->fog[1] = D_L15_0015F54C_2D7C00;
                pv->fog[2] = D_L15_0015F550_2D7C00;
                pv->fog[3] = D_L15_0015F554_2D7C00;
                pv->fogRGB[0] = D_L15_0015F544_2D7C00[0];
                pv->fogRGB[1] = D_L15_0015F544_2D7C00[1];
                pv->fogRGB[2] = D_L15_0015F544_2D7C00[2];
            }
            pv->shocks = pv->shocks + 1;
            D_L15_0015F544_2D7C00[2] = 0x7F;
            D_L15_0015F544_2D7C00[0] = 0x7F;
            D_L15_0015F544_2D7C00[1] = 0x7F;
            D_L15_0015F54C_2D7C00 = 131072.0f;
            D_L15_0015F548_i_2D7C00 = 0;
            D_L15_0015F550_2D7C00 = func_002140F8_2D7C00(128.0f, 255.0f);
            D_L15_0015F554_i_2D7C00 = 0;
            d1 = func_002140F8_2D7C00(5.0f, 8.0f);
            a = func_00214158_2D7C00();
            b = func_00214158_2D7C00();
            func_00215C00_2D7C00(&pv->aim, d1, a, b);
            func_L00_001FF240_2D7C00(tmp, &pv->aim, &G_2D7C00.body);
            pv->end = pv->aim;
            pv->start = G_2D7C00.body;
            func_L15_002D8DD8_2D7C00(m);
            func_L15_002D8FA0_2D7C00(m);
            func_001F49B0_2D7C00(func_L15_002D9B00_2D7C00, m);
            hit.attacker = m;
            hit.flags = 0x10001;
            hit.damage = 1.0f;
            hit.v[0] = func_001F9F90_2D7C00(*(float *)(m + 0x48)) * 0.2f;
            hit.v[1] = func_001F9FA8_2D7C00(*(float *)(m + 0x48)) * 0.2f;
            *(int *)&hit.v[2] = 0;
            func_L00_001FF500_2D7C00(hit.v, hit.v, 1.0f);
            hit.b29 = 1;
            hit.oclass = *(unsigned short *)(m + 0xA6);
            hit.v[2] = 1.0f;
            hit.v[3] = 5627.9248f;
            hit.b28 = 0;
            func_L00_0025AAC0_2D7C00(G_2D7C00.moby, &hit);
            if (m[0x20] == 2) {
                D_L15_0015F548_2D7C00 = pv->fog[0];
                D_L15_0015F54C_2D7C00 = pv->fog[1];
                D_L15_0015F550_2D7C00 = pv->fog[2];
                D_L15_0015F554_2D7C00 = pv->fog[3];
                D_L15_0015F544_2D7C00[0] = pv->fogRGB[0];
                D_L15_0015F544_2D7C00[1] = pv->fogRGB[1];
                D_L15_0015F544_2D7C00[2] = pv->fogRGB[2];
            }
        }
        /* fall through: the hum and the switches, as in state 2 */
    case 2:
        near = 0;
        for (k6 = 0; k6 < 3; k6++) {
            if (pv->lamps[k6] == -1) {
                continue;
            }
            if (near != 0) {
                d1 = func_001F9D48_2D7C00(near + 0x10, D_L15_00167440_2D7C00);
                if (!(func_001F9D48_2D7C00((D_L15_00160058_2D7C00 + ((pv->lamps[k6]) << 8)) + 0x10, D_L15_00167440_2D7C00) < d1)) {
                    continue;
                }
            }
            near = (D_L15_00160058_2D7C00 + ((pv->lamps[k6]) << 8));
        }
        if (pv->owner != 0) {
            d1 = func_001F9D48_2D7C00(pv->owner + 0x10, D_L15_00167440_2D7C00);
            if (func_001F9D48_2D7C00(near + 0x10, D_L15_00167440_2D7C00) < d1) {
                if (pv->owner != 0) {
                    if ((pv->voice0) != -1) {
                        unsigned char *e_ = D_0013E650_2D7C00 + (pv->voice0) * 0x70;
                        if (*(unsigned char **)(e_ + 0x88) == (pv->owner) && e_[0x74] != 0) {
                            func_L00_0028EBF0_2D7C00(pv->voice0);
                        }
                    }
                    pv->voice0 = -1;
                    if ((pv->voice1) != -1) {
                        unsigned char *e_ = D_0013E650_2D7C00 + (pv->voice1) * 0x70;
                        if (*(unsigned char **)(e_ + 0x88) == (pv->owner) && e_[0x74] != 0) {
                            func_L00_0028EBF0_2D7C00(pv->voice1);
                        }
                    }
                    pv->voice1 = -1;
                }
                pv->owner = near;
            }
        } else {
            pv->owner = near;
        }
        if (func_L00_0028EB98_2D7C00(pv->owner, pv->voice0) == 0) {
            pv->voice0 = func_L00_0028EF68_2D7C00(0, 5, pv->owner, 0x28F);
        }
        if (m[0x20] == 1 && func_L00_0028EB98_2D7C00(pv->owner, pv->voice1) == 0) {
            pv->voice1 = func_L00_0028EF68_2D7C00(1, 5, pv->owner, 0x28F);
        }
        if (func_001F9938_2D7C00(&pv->offT) != 0 && m[0x20] == 2) {
            pv->onT = func_001FA898_2D7C00(func_001F9878_2D7C00((D_L15_00161C48_2D7C00) * 60.0f));
            pv->offT = func_001F9850_2D7C00(200);
            pv->shocks = 0;
            m[0x20] = 1;
        }
        for (k7 = 0; k7 < 4; k7++) {
            if (pv->sw[k7] == -1) {
                continue;
            }
            if (!(*(short *)((D_L15_00160058_2D7C00 + pv->sw[k7] * 256) + 0xA6) == 0x33E && (D_L15_00160058_2D7C00 + pv->sw[k7] * 256)[0x20] == 2)) {
                if (pv->sw[k7] == -1) {
                    continue;
                }
                if (func_L11_00310738_2D7C00((D_L15_00160058_2D7C00 + ((pv->sw[k7]) << 8))) == 0) {
                    continue;
                }
            }
            pv->offT = func_001FA898_2D7C00(func_001F9878_2D7C00((pv->offSecs) * 60.0f));
            m[0x20] = 3;
            func_L00_0028EF68_2D7C00(2, 0, (D_L15_00160058_2D7C00 + ((pv->sw[k7]) << 8)), 0x28F);
            pv->armed = 0;
        }
        break;

    case 3:
        for (k8 = 0; k8 < 3; k8++) {
            if (pv->lamps[k8] != -1) {
                func_L00_00251328_2D7C00((D_L15_00160058_2D7C00 + ((pv->lamps[k8]) << 8)), 0x40, 0x40, 0x40);
            }
        }
        if (pv->light != -1) {
            func_L00_0023F1D0_2D7C00(pv->light);
            pv->light = -1;
        }
        if (pv->offT != 0) {
            func_001F49B0_2D7C00(func_L15_002D8BB0_2D7C00, m);
        }
        if (pv->owner != 0) {
            if ((pv->voice0) != -1) {
                unsigned char *e_ = D_0013E650_2D7C00 + (pv->voice0) * 0x70;
                if (*(unsigned char **)(e_ + 0x88) == (pv->owner) && e_[0x74] != 0) {
                    func_L00_0028EBF0_2D7C00(pv->voice0);
                }
            }
            pv->voice0 = -1;
            if ((pv->voice1) != -1) {
                unsigned char *e_ = D_0013E650_2D7C00 + (pv->voice1) * 0x70;
                if (*(unsigned char **)(e_ + 0x88) == (pv->owner) && e_[0x74] != 0) {
                    func_L00_0028EBF0_2D7C00(pv->voice1);
                }
            }
            pv->voice1 = -1;
            pv->owner = 0;
        }
        for (k9 = 0; k9 < 4; k9++) {
            if (pv->sw[k9] == -1) {
                continue;
            }
            if (G_2D7C00.standing != (D_L15_00160058_2D7C00 + ((pv->sw[k9]) << 8)) || *(short *)(G_2D7C00.standing + 0xA6) != 0x33E || G_2D7C00.airTicks != 0) {
                continue;
            }
            if (pv->armed != 0) {
                n = pv->offT;
                if (n < func_001FA898_2D7C00(func_001F9878_2D7C00((pv->offSecs) * 60.0f)) - func_001F9850_2D7C00(0x3C)) {
                    pv->offT = func_001FA898_2D7C00(func_001F9878_2D7C00((pv->offSecs) * 60.0f));
                    func_L00_0028EF68_2D7C00(2, 0, (D_L15_00160058_2D7C00 + ((pv->sw[k9]) << 8)), 0x28F);
                }
            }
            armed = 0;
        }
        pv->armed = armed;
        if (func_001F9938_2D7C00(&pv->offT) == 0) {
            if (func_00215570_2D7C00(D_0013F4D0_2D7C00, pv->resetA) == 0 || *(short *)(D_0013F4D0_2D7C00 + 0x28E) != 0) {
                if (func_00215570_2D7C00(D_0013F4D0_2D7C00, pv->resetB) == 0) {
                    break;
                }
            }
        }
        for (k10 = 0; k10 < 4; k10++) {
            if (pv->sw[k10] == -1) {
                continue;
            }
            if (*(short *)((D_L15_00160058_2D7C00 + ((pv->sw[k10]) << 8)) + 0xA6) == 0x33E) {
                (D_L15_00160058_2D7C00 + ((pv->sw[k10]) << 8))[0x20] = 1;
            } else {
                func_L15_002DA990_2D7C00((D_L15_00160058_2D7C00 + ((pv->sw[k10]) << 8)));
            }
        }
        pv->onT = func_001FA898_2D7C00(func_001F9878_2D7C00((D_L15_00161C48_2D7C00) * 60.0f));
        pv->offT = func_001F9850_2D7C00(200);
        pv->shocks = 0;
        m[0x20] = 1;
        break;
    }

    {
        char *pos = D_0013F4D0_2D7C00;
        Hero_2D7C00 *g = (Hero_2D7C00 *)(pos - 0x80);
        for (k11 = 0; k11 < 4; k11++) {
            if (func_00215570_2D7C00(pos, pv->levels[k11]) != 0 && func_L00_0020DC00_2D7C00() != 0) {
                g->waterLevel = *(float *)((D_L15_0016016C_2D7C00 + ((pv->levels[k11]) << 7)) + 0x38);
            }
        }
    }
}
