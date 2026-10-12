/* NON_MATCHING func_L00_00227F48 -- src/overlays/shared/help_00221A98.c
 * Best so far: SIZE ours 552 / retail 564, checked 2026-10-12.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   func_L00_00227F48: hero help-state timer tick (control_mode 0x11 decays hero.unk22A0, signals func_L00_00222B8
 */
#include "pad.h"
extern Hero D_0013F450_227F48 __asm__("D_0013F450");
extern struct PadState D_0013CA40_227F48 __asm__("D_0013CA40");
extern unsigned char D_0013D5CA_227F48[] __asm__("D_0013D5CA") NOT_SDA;
extern float D_0015EE6C_227F48 __asm__("D_0015EE6C") MACRO_ADDR;
extern int D_L00_00167114_227F48 __asm__("D_L00_00167114") NOT_SDA;
extern int func_001F9850_227F48(int) __asm__("func_001F9850");
extern int func_001FFB38_227F48(int, int, void (*)(void), void (*)(void), void (*)(void), int *, int) __asm__("func_001FFB38");
extern int func_L00_001EFFF0_227F48(void *, void *, int, void *, int) __asm__("func_L00_001EFFF0");
extern void func_L00_00236BF8(void);
extern void func_L00_00236DE8(void);
extern void func_L00_00236F38(void);

int func_L00_00227F48(void) {
    float v[4] __attribute__((aligned(16)));
    float w[4] __attribute__((aligned(16)));
    int dec;
    int st;

    if (D_0013F450_227F48.state.control_mode == 0x11) {
        if (D_0013D5CA_227F48[4] != 0) {
            dec = 0;
        } else {
            dec = 10000 / (func_001F9850_227F48(0x3C) * 15);
            func_001FFB38_227F48(4, 0x753F, func_L00_00236BF8, func_L00_00236DE8, func_L00_00236F38, &D_0013F450_227F48.unk22A0, 10000);
        }
        D_0013F450_227F48.unk22A0 = D_0013F450_227F48.unk22A0 - dec;
        if (D_0013F450_227F48.unk22A0 < 0) {
            D_0013F450_227F48.unk22A0 = 0;
        }
        if (D_0013F450_227F48.unk22A0 == 0) {
            if (!(D_0013F450_227F48.height_threshold - 2.0f < D_0013F450_227F48.motion.pos.f[2])
                || !(D_0013F450_227F48.motion.rot.f[1] < -0.8742f)
                || !(D_0015EE6C_227F48 < D_0013F450_227F48.motion.unk100.f[2])) {
                st = 0x6A;
                goto signal;
            }
        }
        if (D_0013F450_227F48.height_threshold + 0.4f < D_0013F450_227F48.motion.pos.f[2]) {
            D_L00_00167114_227F48 = 0;
            st = 6;
        signal:
            func_L00_00222B80(st, 1);
            return 1;
        }
    } else if (D_0013F450_227F48.state.current != 0x6A && D_0013F450_227F48.state.current != 0x76) {
        D_0013F450_227F48.unk22A0 = 10000;
    }
    if (D_0013F450_227F48.state.control_mode == 0x12) {
        qcopy(v, &D_0013F450_227F48.motion.pos);
        v[2] = D_0013F450_227F48.height_threshold + 0.01f;
        qcopy(w, v);
        w[2] = w[2] + 0.3f;
        if (func_L00_001EFFF0_227F48(v, w, 2, D_0013F450_227F48.moby, 0) == 0) {
            return 0;
        }
        if (D_0013D5CA_227F48[2] != 0 && (D_0013CA40_227F48.held & 0xA) != 0) {
            st = 0x35;
            goto signal;
        }
        func_L00_00222B80(0x33, 1);
        return 1;
    }
    return 0;
}
