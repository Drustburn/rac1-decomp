/* NON_MATCHING func_L00_002D3B20 -- src/overlays/shared/vendor_002D1168.c
 * Best so far: BYTES 850/1052 (19.2% of the bytes match), checked 2026-10-15.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   2026-10-15: compiles again (func_L00_002D3608 is now C in the file, so the call uses its prototype).
 *   Rewritten without goto: the release test is one condition over Hero (state.current, unk20A8, unk20AC,
 *   unk1BC, state_timer, items[0].button_mask) and PadState.pressed. Size 1052 = retail, BYTES 850/1052,
 *   almost all from the frame: retail saves s0-s6 and ra in a 0x100 frame with one more 16-byte local (sp+0x70
 *   for the last func_L00_002C48C8 call), and keeps e = d + 0x40 in two registers (s2, then s5). Ours saves s7
 *   and fp as well. Separate locals for the three uses of loc give 1036.
 */
extern void func_L00_00250800(void *, int, void *);
extern void func_001F9BD8(void *, void *, void *);
extern void func_L00_00260878(void *, void *);
extern int func_L00_002C48C8(void *, void *, void *);
extern void func_L00_00222B80(int, int);
extern int func_001F9908(int *);
extern int func_001F9850(int);
extern int func_L00_00234718(int);
extern int func_L00_00217570(int, int);
extern int func_L00_00234638(int, int);
extern void func_L00_002C4B90(float *, char *, void *);
extern short D_L00_001B0BB0[];
extern int D_0015EFA4 MACRO_ADDR;
extern int D_0015EE84 MACRO_ADDR;
extern char D_0013E633[];
extern char D_0013A5E0[];
extern char D_0014171B[];
extern Hero D_0013F450_2D3B20 __asm__("D_0013F450");
extern PadState D_0013CA40_2D3B20 __asm__("D_0013CA40");
extern void func_0020D678_2D3B20(void *) __asm__("func_0020D678");
extern void func_0022ED80(int, int, int);
typedef int u128_D3B20 __attribute__((mode(TI)));

/* Decoy glove update: state machine on moby[0x20], per-object data block at moby[0x78]. Adapted from Lombyte (MIT) for PAL: none. */
void func_L00_002D3B20(unsigned char *moby) {
    char *d;
    char *p;
    char *e;
    float vb[4];
    float va[4];
    float vc[4];
    float vd[4];
    u128_D3B20 loc;

    if (moby == 0) return;
    d = *(char **)(moby + 0x78);
    if (d == 0) return;
    *(u128_D3B20 *)va = 0;
    va[1] = -0.19f;
    va[2] = -0.11f;
    func_L00_00250800(moby, 0, vb);
    e = d + 0x40;
    func_0020DAF8_2d3330(moby, 0, vd);
    func_001F9EE8(vc, va, vd);
    func_001F9BD8(e, vb, vc);

    p = *(char **)(d + 0x50);
    if (p) {
        *(int *)(p + 0x98) = 1;
        if (*(unsigned char *)(D_0013E633 + 6) && *(short *)(*(char **)(d + 0x50) + 0xA6) != 0x76C) {
            qzero(&loc);
            func_L00_00260878(*(char **)(d + 0x50), D_L00_001B0BB0);
            func_0020D678_2D3B20(*(char **)(d + 0x50));
            *(int *)(d + 0x50) = func_L00_002C48C8(moby, e, &loc);
        }
    }
    if (*(char **)(d + 0x50)) {
        if (*(unsigned char *)(*(char **)(d + 0x50) + 0x20) == 0xFE || *(unsigned char *)(*(char **)(d + 0x50) + 0x20) == 0xFD) *(int *)(d + 0x50) = 0;
    }

    if (D_0013F450_2D3B20.state.current == 1) func_L00_00222B80(0x1E, 1);
    if (func_001F9908((int *)(d + 0x54)) && D_0013F450_2D3B20.unk20AC == 0
        && ((D_0013F450_2D3B20.unk20A8 && D_0013F450_2D3B20.unk1BC == func_001F9850(0x11))
            || ((D_0013CA40_2D3B20.pressed & D_0013F450_2D3B20.items[0].button_mask) && D_0013F450_2D3B20.state.current == 0x1E && func_L00_00234718(-1))
            || (D_0013F450_2D3B20.state.current == 0x23 && D_0013F450_2D3B20.state_timer == func_001F9850(0x10)))) {
        func_L00_00217570(0x1A, 0);
        func_L00_00234638(-1, 1);
        moby[0x20] = 3;
    }
    *(u128_D3B20 *)&loc = *(u128_D3B20 *)(d + 0x40);
    func_L00_002D3608((int)moby, (Moby_2D3608 *)d, (float *)&loc);

    switch (moby[0x20]) {
    case 0:
        *(int *)(d + 0x50) = 0;
        moby[0x20] = (moby[0x70] & 2) ? 2 : 1;
    case 1:
        if (moby[0x70] & 2) moby[0x20] = 2;
        break;
    case 2:
        break;
    case 3:
        p = *(char **)(d + 0x50);
        if (p) {
            char *P = D_0014171B + 0x65;
            if (*(unsigned short *)(P + 0xC8) <= 0xFFFE) *(unsigned short *)(P + 0xC8) = *(unsigned short *)(P + 0xC8) + 1;
            if ((int)*(unsigned short *)(P + 0xCA) < func_001F9850(D_0015EFA4) / 600)
                *(unsigned short *)(P + 0xCA) = func_001F9850(D_0015EFA4) / 600;
            *(unsigned int *)(P + 0xCC) = (*(unsigned int *)(P + 0xCC) | (1 << D_0015EE84)) | 0x80000000;
            func_L00_002C4B90((float *)e, p, *(char **)(p + 0x78) + 0x10);
            *(int *)(d + 0x50) = 0;
        } else {
            func_0022ED80(0, 0, (int)moby);
        }
        moby[0x20] = 4;
        *(int *)(d + 0x54) = func_001F9850(0x14);
        break;
    case 4:
        if (D_0013F450_2D3B20.state.current != 0x23 && D_0013F450_2D3B20.unk20A8 == 0) moby[0x20] = 2;
        break;
    case 5:
        moby[0x20] = 6;
        break;
    case 6:
        return;
    default:
        break;
    }

    p = *(char **)(d + 0x50);
    if (p) {
        qcopy(p + 0x10, e);
        return;
    }
    if (func_L00_00234718(-1) || D_0013F450_2D3B20.unk20A8) {
        qzero(&loc);
        *(int *)(d + 0x50) = func_L00_002C48C8(moby, e, &loc);
    }
}
