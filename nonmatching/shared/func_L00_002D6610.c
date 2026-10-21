/* NON_MATCHING func_L00_002D6610 -- src/overlays/shared/vendor_002D1168.c
 * Best so far: BYTES 9/868 (99.0% of the bytes match), checked 2026-10-21.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   From Lombyte's non-matching draft of FUN_L00_002d5160 (veldin_ship.c, GPL-3.0), PAL symbols.
 *   Left: store order of the hit reaction block: retail stores +0x80 first and +0x94/+0x90 last;
 *   756 orders of the nine stores (with and without volatile on +0x94/+0x90) and a permuter run
 *   found nothing closer than this (2026-10-21, m3).
 */
extern s32 D_L00_0015F6A8_6610 __asm__("D_L00_0015F6A8") MACRO_ADDR;
extern f32 D_0015EE6C_6610 __asm__("D_0015EE6C") MACRO_ADDR;
extern f32 D_0015EE70_6610 __asm__("D_0015EE70") MACRO_ADDR;
extern f32 D_L00_00161A40 SDATA(D_L00_00161A40);
extern f32 D_L00_00161A44 SDATA(D_L00_00161A44);
extern f32 D_L00_00161A48 SDATA(D_L00_00161A48);
extern f32 D_L00_00161A4C SDATA(D_L00_00161A4C);
extern f32 D_L00_00161A58 SDATA(D_L00_00161A58);
extern f32 D_L00_00161A5C SDATA(D_L00_00161A5C);
extern char D_0013F450_6610[] __asm__("D_0013F450");
extern unsigned char *D_L00_001B0830_6610[] __asm__("D_L00_001B0830");
extern void *f_B478_6610(void *, s32, s32) __asm__("func_L00_0025B478");
extern void f_B4D0_6610(void *, void *, void *, s32, s32 *, f32 *, s32, s32) __asm__("func_L00_0025B4D0");
extern void f_BBA0_6610(void *, f32 *, void *, void *) __asm__("func_L00_0025BBA0");
extern void f_D5B0_6610(void *, void *, f32, s32, s32, s32) __asm__("func_L00_0025D5B0");
extern void f_E4B0_6610(void *, void *) __asm__("func_L00_0025E4B0");
extern void f_E590_6610(void *, void *) __asm__("func_L00_0025E590");
extern void f_0FB0_6610(f32, void *, void *, s32, s32, void *, s32) __asm__("func_L00_00260FB0");
extern void f_84A8_6610(void *, s32, s32) __asm__("func_L00_002584A8");
extern s32 f_randi_6610(s32) __asm__("func_002140B0");
extern f32 f_randf_6610(f32, f32) __asm__("func_002140F8");
extern void f_9938_6610(void *) __asm__("func_001F9938");
extern f32 f_9878_6610(f32) __asm__("func_001F9878");
extern s16 f_ftoi_6610(f32) __asm__("func_001FA898");
extern s32 f_frames_6610(s32) __asm__("func_001F9850");
extern void f_3DE0_6610(void *, s32, s32, s32) __asm__("func_00213DE0");
void func_L00_002D6610_r(unsigned char *m) __asm__("func_L00_002D6610");

void func_L00_002D6610_r(unsigned char *m) {
    unsigned char *v = *(unsigned char **)(m + 0x78);
    unsigned char *hit;
    s32 result;
    f32 damage;
    f32 angle;
    f32 vec[4] __attribute__((aligned(16)));
    f32 forceX, forceY, forceZ, forceW, health, duration;
    f32 reactionC0, reactionC4;
    unsigned char *row;
    s32 index;
    unsigned char *effect;

    if (m[0x20] == 0)
        return;
    if (D_L00_0015F6A8_6610 == 2) {
        m[0x31] = 0;
        *(u16 *)(m + 0x34) |= 1;
    } else {
        m[0x31] = 1;
        *(u16 *)(m + 0x34) &= ~1;
    }
    damage = 0.0f;
    hit = f_B478_6610(m, 0x330000, 0);
    f_B4D0_6610(m, hit, v + 0x20, 0, &result, &damage, 0, 4);
    if (result != 1 && m[0x20] != 12 && damage != 0.0f) {
        forceX = D_L00_00161A40 * D_0015EE70_6610;
        forceY = D_L00_00161A44 * D_0015EE70_6610;
        forceZ = D_L00_00161A48 * D_0015EE6C_6610;
        forceW = D_L00_00161A4C * D_0015EE6C_6610;
        health = *(f32 *)(v + 0x20) - damage;
        v[0xAD] = 0;
        duration = D_0015EE6C_6610 + D_0015EE6C_6610;
        *(volatile s32 *)(v + 0x94) = 0x29;
        *(volatile s32 *)(v + 0x90) = 0x200;
        *(f32 *)(v + 0x98) = 0.75f;
        *(f32 *)(v + 0x20) = health;
        *(f32 *)(v + 0x84) = forceY;
        *(f32 *)(v + 0x88) = forceZ;
        *(f32 *)(v + 0x8C) = forceW;
        *(f32 *)(v + 0x80) = forceX;
        *(f32 *)(v + 0xBC) = duration;
        *(u16 *)(m + 0x34) &= ~0x1000;
        *(VecQuad *)vec = *(VecQuad *)(hit + 0x10);
        f_BBA0_6610(vec, &angle, v + 0x88, v + 0x8C);
        f_D5B0_6610(m, v + 0x70, angle, f_randi_6610(2) + 6, 1, 0);
        reactionC0 = D_L00_00161A5C;
        reactionC4 = D_L00_00161A58;
        *(f32 *)(v + 0xC0) = reactionC0;
        *(f32 *)(v + 0xC4) = reactionC4;
        m[0x20] = 12;
        v[0x67] = 0x78;
        f_E4B0_6610(m, v + 0x60);
        f_84A8_6610(m, 0, -1);
    }
    m[0xA4] = 0xFF;
    f_E590_6610(m, v + 0x60);
    if (m[0x20] == 3) {
        index = *(s32 *)(v + 0x1E4);
        effect = v + 0x180;
        f_0FB0_6610(64.0f, m, effect, 0, 0, D_L00_001B0830_6610[index] + 0x10, *(s32 *)D_L00_001B0830_6610[index]);
    } else {
        index = *(s32 *)(v + 0x1E0);
        effect = v + 0x180;
        f_0FB0_6610(24.0f, m, effect, 0, 0, D_L00_001B0830_6610[index] + 0x10, *(s32 *)D_L00_001B0830_6610[index]);
    }
    if (*(s32 *)(v + 0x1C0) == 0) {
        *(s32 *)(v + 0x1C0) = (s32)((struct Hero *)D_0013F450_6610)->moby;
        qcopy(effect, &((struct Hero *)D_0013F450_6610)->motion.pos);
    }
    f_9938_6610(v + 0x276);
    index = *(s32 *)(v + 0x38);
    if (index != 0) {
        *(s16 *)(v + 0x276) = f_ftoi_6610(f_9878_6610(f_randf_6610(180.0f, 240.0f)));
    }
    *(s32 *)(v + 0x38) = 0;
    if ((m[0x20] == 5 || m[0x20] == 8) && *(s16 *)(v + 0x276) != 0) {
        m[0x20] = 10;
        if (m[0x53] != 4)
            f_3DE0_6610(m, 4, 0, f_frames_6610(20));
    }
}
