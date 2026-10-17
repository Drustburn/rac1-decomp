#include "common.h"
#include "callees.h"
#include "records.h"
#include "structs.h"

/*
 * freeze.cpp in the original source; text 0x1FBC80-0x1FDF10.
 * Name and boundary from the NTSC split of this game, mapped to PAL by matching function
 * sizes -- see docs/DECOMP_PROGRESS.md. Compiled as C for now.
 */

/* Declarations in scope here before the split. */
extern char D_0013E650[];
extern int D_0015F694;
extern void func_001F9A98(void *, void *, int);
extern char D_00189310[];
extern char D_001899D0[];
extern void *D_001871C0 NOT_SDA;
extern DispatchRec D_001E8F80[];
extern int D_0018A3B0[];
extern void func_001F99B0();
extern void func_001F2BC8(void);
extern int D_0018C434 NOT_SDA;
extern char D_001940C0[];
extern long D_00151888[3];
extern int D_0015F6FC MACRO_ADDR;
extern short D_0015F534;
extern void func_001FB530(void);
extern void func_001F3D78(void);
extern int D_0015F564;
extern int D_0018DD40[];
extern int D_0018DC40[];
extern short D_0015F59C;
extern int func_001F65B0(unsigned char *arg0, int arg1, void *arg2);
extern unsigned char D_001DF3D0[];
extern unsigned char D_001DF770[];
extern unsigned char D_001DFB10[];
extern void func_001F6668(void *, void *, void *, void *, void *, int,
                          unsigned char *);
extern int func_001F6600(unsigned char *, int);
extern int func_001F6620(unsigned char *, int);
extern int func_001F4868(int);
extern void func_001FB498(void);
extern void func_001F3008(void);
extern void func_001F3140(void);
extern int D_0018E840[];
extern long D_00152178 NOT_SDA;

extern void func_0012E528(int);
extern void func_00216EF0(int);
extern int func_001FE540(int);
extern void func_001FF560(int);
extern int func_001F98C0(int);
extern int D_0015F6E8 MACRO_ADDR;
extern int D_0013D440;
extern char D_00193400[];

/* Enters the freeze screen for reason arg0: pauses the music unless the
   game mode (D_0015F6E8) is already 3, saves arg1/arg2 and the old mode in
   the freeze record D_00193400, sets mode 4, then per reason fetches the
   message strings (func_001FE540) or starts the timers (func_001F98C0);
   an unknown reason gets a plain 0x78 countdown. Each case reads the
   record through its own `char *` local (retail keeps only the %hi and
   rebuilds the %lo per case); the case bodies are in retail's layout
   order, and the last store of a block is written last in the source
   because the scheduler issues it first. */
void func_001FBC80(int arg0, int arg1, int arg2) {
    char *s = D_00193400;

    *(int *)(s + 0x30) = arg2;
    if (D_0015F6E8 != 3) {
        func_0012E528(0x1D);
        music_Pause(0);
    }
    *(int *)(s + 0x18) = arg1;
    *(int *)(s + 0x14) = D_0015F6E8;
    D_0015F6E8 = 4;
    *(int *)s = arg0;
    *(int *)(s + 0x2C) = 0;
    switch (arg0) {
    case 0: {
        char *p = D_00193400;
        *(int *)(p + 8) = msg_string(0x4F6E);
        *(int *)(p + 0xC) = msg_string(0x524D);
        *(int *)(p + 0x10) = msg_string(0x524E);
        *(int *)(p + 4) = 0;
        *(int *)(p + 0x1C) = 0;
        *(int *)(p + 0x20) = 0;
        *(int *)(p + 0x24) = 0;
        *(int *)(p + 0x28) = 0;
        break;
    }
    case 2: {
        int v = msg_string(0x524F);
        char *p = D_00193400;
        *(int *)(p + 8) = v;
        *(int *)(p + 0xC) = 0;
        *(int *)(p + 4) = 0;
        break;
    }
    case 1:
    case 4: {
        char *p = D_00193400;
        *(int *)(p + 8) = msg_string(0x522E);
        *(int *)(p + 0xC) = msg_string(0x4EE0);
        *(int *)(p + 0x10) = msg_string(0x524F);
        *(int *)(p + 4) = 0;
        break;
    }
    case 5: {
        char *p = D_00193400;
        func_001FF560(0x4E2B);
        *(int *)(p + 4) = func_001F98C0(0x1E);
        *(int *)(p + 0x20) = 0;
        *(int *)(p + 0x24) = func_001F98C0(0x1E);
        break;
    }
    case 7:
    case 8: {
        char *p = D_00193400;
        *(int *)(p + 4) = func_001F98C0(0x1E);
        *(int *)(p + 0x20) = -1;
        *(int *)(p + 0x24) = func_001F98C0(0x1E);
        break;
    }
    case 3: {
        char *p = D_00193400;
        *(int *)(p + 4) = func_001F98C0(0x1E);
        *(int *)(p + 0x20) = -1;
        *(int *)(p + 0x24) = func_001F98C0(0x1E);
        D_0013D440 = 3;
        *(int *)(p + 0x2C) = func_001F98C0(0xA);
        break;
    }
    case 6: {
        int v = func_001F98C0(0x1E);
        char *p = D_00193400;
        *(int *)(p + 4) = v;
        *(int *)(p + 0x1C) = 0;
        break;
    }
    default:
        *(int *)(s + 4) = 0x78;
        *(int *)(s + 0xC) = 0;
        *(int *)(s + 0x10) = 0;
        *(int *)(s + 8) = 0;
        break;
    }
}
__asm__(".section .text\n\tnop\n");

INCLUDE_ASM("asm/nonmatchings/text", func_001FBE80);

/* The freeze screen's update (game mode 4: the dialogs over a frozen frame). D_00193400 is the
   dialog: +0 its kind (0-8), +4 and +0x2C countdowns, +0x14 the mode to return to, +0x18 a menu
   to show, +0x1C a step, +0x20 a frame counter, +0x24 a delay before input is taken. D_0013CBE4
   holds the buttons pressed this frame (0x40 cross, 0x10 triangle, 0x20 circle); D_0015F6E8 is the
   game's mode, which leaving the dialog sets; D_0015EFB4 the save menu's flags. Kind 3 is the
   memory card dialog, whose step comes from D_0015EFB0. Written from the instructions. */

typedef struct {
    int kind;
    int x4;
    int x8;
    int xC;
    int x10;
    int x14;
    int x18;
    int x1C;
    int x20;
    int x24;
    int x28;
    int x2C;
} Freeze_FD3E8;

extern int D_0013CBE4[];
extern int D_0015EFB4 MACRO_ADDR;
extern int D_0015EE84 MACRO_ADDR;
extern int D_0015EFB0 MACRO_ADDR;
extern int D_0015F6C8 MACRO_ADDR;
extern int D_0015F6CC MACRO_ADDR;
extern int D_001D60C0[];
extern int D_001D5F78[];
extern int D_0015F728 MACRO_ADDR;
extern int D_0015EF38 MACRO_ADDR;
extern int D_0015EF3C MACRO_ADDR;
extern int D_0013D48C[];
extern int D_0015F6E4 MACRO_ADDR;
extern int D_0015F690 MACRO_ADDR;
extern short D_0013E15A[];
extern unsigned char D_0016044C[4] MACRO_ADDR;
extern char D_0013F450[];
extern char D_0013D390[];
extern char D_0013CA40[];
extern char D_00141150[];

extern void func_0020BFC8(int, int);
extern void func_001E9768(void);
extern void func_001E97C0(void *, void *, int, int);
extern void func_001F4E08(int);
extern void func_001FFDA0(int, int);
extern void func_001FFFA0(void);
extern void func_00216D30(int, int);
extern void func_00209DC0(void);
extern void func_0022F4A0(int);
extern void func_00227DB0(int);
extern void func_0012E558(int);
extern void func_00216F28(void);
extern int func_0012DDC0(void);

#define R (*(Freeze_FD3E8 *)D_00193400)
#define PAD (D_0013CBE4[0])
void func_001FD3E8(void) {
    Freeze_FD3E8 *q;
    Freeze_FD3E8 *r5;
    char *dp1, *dp2;
    char *h1, *h2, *h3, *h4;
    int t;
    int p1, p2, p3, p4, p5, p6, p7, p8, p9;
    char *m, *m2;
    char *c;
    int v;

    func_0022DD68();
    q = &R;
    if (q->x4 != 0) {
        q->x4--;
    }
    if (q->x2C != 0) {
        q->x2C--;
    }
    switch (R.kind) {
    case 5:
        R.x20++;
        t = func_001F98C0(0x5A);
        if (t < R.x20 && R.x24 != 0) {
            R.x24--;
        }
        r5 = &R;
        t = func_001F98C0(0x78);
        if (t < r5->x20 && (PAD & 0x40)) {
            D_0015F6E8 = 0;
            if (D_0015EE84 == 1) {
                func_0020BFC8(0, -1);
            }
        }
        break;
    case 8:
        R.x20++;
        t = func_001F98C0(0x1E);
        if (t < R.x20 && R.x24 != 0) {
            R.x24--;
        }
        if (R.x24 == 0 && (PAD & 0x40)) {
            D_0015EFB4 &= ~0x40;
            D_0013D48C[0] = 0;
            func_00227DB0(-1);
            func_0022F4A0(0);
            D_0013E15A[0] = 1;
        }
        break;
    case 7:
        R.x20++;
        t = func_001F98C0(0x1E);
        if (t < R.x20 && R.x24 != 0) {
            R.x24--;
        }
        if (R.x24 == 0 && (PAD & 0x40)) {
            D_0013D48C[0] = 0;
            func_00227DB0(-1);
            func_0022F4A0(0);
            D_0013E15A[0] = 1;
        }
        break;
    case 4:
        p1 = PAD;
        if (p1 & 0x10) {
            h1 = D_0013F450;
            m = *(char **)(h1 + 0x2080);
            m[0x31] = 0;
            *(int *)(m + 0x94) = 0;
            *(unsigned short *)(m + 0x34) |= 1;
            func_001E9768();
            func_001E97C0(h1 + 0x1D00, h1 + 0x1D10, 0, 1);
            D_0015F6E8 = 0;
        } else if (p1 & 0x40) {
            D_0015F6E8 = 0;
        }
        break;
    case 6:
        switch (R.x1C) {
        case 0:
            if (R.x4 == 0) {
                R.x1C = 1;
            }
            break;
        case 1:
            p2 = PAD;
            if (p2 & 0x10) {
                D_0015F6E8 = R.x14;
            } else if (p2 & 0x40) {
                func_001F4E08(4);
                D_0016044C[0] = 0;
                R.x1C = 2;
                R.x20 = func_001F98C0(0x258);
            }
            break;
        case 2:
            if (R.x20 == 0 || --R.x20 == 0) {
                R.x1C = 3;
                D_0016044C[0] = 1;
                break;
            }
            p3 = PAD;
            if (p3 & 0x10) {
                func_001F4E08(4);
                D_0015F6E8 = R.x14;
                D_0016044C[0] = 1;
            } else if (p3 & 0x40) {
                D_0015F6E8 = R.x14;
                D_0016044C[0] = 0;
            }
            break;
        case 3:
            if (PAD & 0x40) {
                D_0015F6E8 = R.x14;
            }
            break;
        default:
            D_0015F6E8 = R.x14;
            break;
        }
        break;
    case 1:
        p4 = PAD;
        D_0015F728 = 2;
        if (p4 & 0x10) {
            D_0015F6E8 = 0;
            h2 = D_0013F450;
            h2[0x160F] |= 1;
        } else if (p4 & 0x40) {
            D_0015F6E8 = 0;
        }
        break;
    case 2:
        p5 = PAD;
        D_0015F728 = 2;
        if (p5 & 0x40) {
            D_0015F6E8 = 0;
        }
        break;
    case 0:
        switch (R.x1C) {
        case 0:
            if (R.x20 < 8) {
                R.x20++;
            } else if (R.x24 < 8) {
                R.x24++;
            } else {
                R.x1C = 1;
            }
            break;
        case 1:
            p6 = PAD;
            if (p6 & 0x40) {
                R.x1C = 2;
            } else if (p6 & 0x820) {
                R.x1C = 3;
            }
            break;
        case 2:
        case 3:
            if (R.x24 != 0) {
                R.x24--;
                break;
            }
            if (R.x20 != 0) {
                R.x20--;
                break;
            }
            v = R.x1C;
            if (v == 2) {
                h3 = D_0013F450;
                v = *(int *)(h3 + 0x880);
                if (v != -1) {
                    func_001FFDA0(v, 0);
                    *(int *)(h3 + 0x880) = -1;
                }
                func_001FFFA0();
                t = func_001F98C0(0x1068);
                if (t < *(int *)(h3 + 0x19C)) {
                    if (D_0015EE84 == 5) {
                        D_0015EF38 = D_0015EF38 + 1;
                    } else if (D_0015EE84 == 0x10) {
                        D_0015EF3C = D_0015EF3C + 1;
                    }
                }
                D_0015F6E8 = 0;
                func_00216D30(0, 8);
                func_001E97C0(D_00141150, D_00141150 + 0x10, 0, 1);
                break;
            }
            D_0015F6E8 = 0;
            h4 = D_0013F450;
            if (*(short *)(h4 + 0x89A) >= 3) {
                *(short *)(h4 + 0x89A) = *(unsigned short *)(h4 + 0x89A) - 1;
                m2 = *(char **)(h4 + 0x890);
                m2[0xBC] = 3;
            }
            break;
        }
        break;
    case 3:
        dp1 = D_0013D390;
        if (*(int *)(dp1 + 0xB0) == 1) {
            break;
        }
        R.x20++;
        t = func_001F98C0(0x1E);
        if (t < R.x20 && R.x24 != 0) {
            R.x24--;
        }
        switch (D_0015EFB0 - 1) {
        case 1:
            if (R.x24 == 0 && (PAD & 0x40)) {
                D_0015EFB4 &= ~1;
                D_0015F6E8 = R.x14;
            }
            break;
        case 5:
            if (R.x24 == 0 && (PAD & 0x20)) {
                D_0015EFB4 |= 8;
                break;
            }
            if (R.x4 == 0 && (PAD & 0x10)) {
                { int fl = D_0015EFB4; fl |= 0x20; fl &= ~2; fl &= ~4; D_0015EFB4 = fl; }
                if (D_0015F6C8 == 0) {
                    D_0015F6E8 = R.x14;
                }
            }
            break;
        case 12:
            if (R.x24 == 0 && (PAD & 0x20)) {
                D_0015EFB4 |= 0x10;
                break;
            }
            if (R.x4 == 0 && (PAD & 0x10)) {
                { int fl = D_0015EFB4; fl |= 0x20; fl &= ~2; fl &= ~4; D_0015EFB4 = fl; }
                if (D_0015F6C8 == 0) {
                    D_0015F6E8 = R.x14;
                }
            }
            break;
        case 19:
            if (R.x24 == 0 && (PAD & 0x40)) {
                { int fl = D_0015EFB4; fl &= ~0x40; fl &= ~0x400; fl &= ~2; fl &= ~4; D_0015EFB4 = fl; }
                if (D_0015F6C8 != 0) {
                    D_0015F6E8 = R.x14;
                } else {
                    dp2 = D_0013D390;
                    *(int *)(dp2 + 0xFC) = 0;
                    D_0015F6E4 = -1;
                    D_0015F690 = 1;
                    D_0015F6FC = 1;
                }
            }
            break;
        case 16:
        case 17:
        case 20:
            if (R.x24 == 0 && (PAD & 0x40)) {
                D_0015F6E8 = R.x14;
                { int fl = D_0015EFB4; fl &= ~0x40; fl &= ~0x400; fl &= ~2; fl &= ~4; D_0015EFB4 = fl; }
            }
            break;
        case 0:
        case 15:
            D_0015F6E8 = R.x14;
            D_001D5F78[0] = R.x18;
            break;
        case 11:
            if (D_001D60C0[0] == 0) {
                break;
            }
            if (R.x24 == 0 && (PAD & 0x10)) {
                { int fl = D_0015EFB4; fl &= ~2; fl &= ~4; D_0015EFB4 = fl; }
                D_0015F6E8 = R.x14;
            }
            break;
        case 18:
            if (D_0015F6C8 != 0 && D_001D60C0[0] == 0) {
                if (R.x24 != 0) {
                    break;
                }
                p9 = PAD;
                if (p9 & 0x40) {
                    func_00209DC0();
                    { int fl = D_0015EFB4; fl &= ~2; fl &= ~4; fl |= 0x20; D_0015EFB4 = fl; }
                    func_0022F4A0(0);
                    D_0013E15A[0] = 1;
                    break;
                }
                if (p9 & 0x10) {
                    { int fl = D_0015EFB4; fl &= ~2; fl &= ~4; D_0015EFB4 = fl; }
                    D_0015F6E8 = R.x14;
                }
                break;
            }
        case 2:
        case 4:
            if (R.x24 == 0 && (PAD & 0x10)) {
                { int fl = D_0015EFB4; fl &= ~2; fl &= ~4; D_0015EFB4 = fl; }
                D_0015F6E8 = R.x14;
            }
            break;
        case 22:
        case 23:
            if (R.x24 != 0) {
                break;
            }
            p7 = PAD;
            if (p7 & 0x40) {
                func_00209DC0();
                { int fl = D_0015EFB4; fl &= ~2; fl &= ~4; fl |= 0x20; D_0015EFB4 = fl; }
                func_0022F4A0(0);
                D_0013E15A[0] = 1;
                break;
            }
            if (p7 & 0x10) {
                D_0015EFB4 |= 0x20;
                D_0015F6E8 = R.x14;
            }
            break;
        case 3:
            if (D_0015F6C8 == 0) {
                goto cardb;
            }
            if (D_001D60C0[0] != 0) {
                goto set20;
            }
            if (R.x24 != 0) {
                break;
            }
            c = D_0013CA40;
            if (*(int *)(c + 0x1A4) & 0x40) {
                func_00209DC0();
                { int fl = D_0015EFB4; fl &= ~2; fl &= ~4; D_0015EFB4 = fl; }
                func_0022F4A0(0);
                D_0013E15A[0] = 1;
            }
            if (R.x24 != 0) {
                break;
            }
            if (*(int *)(c + 0x1A4) & 0x10) {
                { int fl = D_0015EFB4; fl |= 0x20; fl &= ~2; fl &= ~4; D_0015EFB4 = fl; }
                D_0015F6E8 = R.x14;
            }
            break;
        cardb:
            if (D_0015F6CC == 0) {
                goto set20;
            }
            if (R.x24 != 0) {
                break;
            }
            p8 = PAD;
            if (p8 & 0x10) {
                D_0015F6E8 = R.x14;
                { int fl = D_0015EFB4; fl |= 0x20; fl &= ~2; fl &= ~4; D_0015EFB4 = fl; }
            }
            if (p8 & 0x20) {
                func_00227DB0(-1);
                func_0022F4A0(0);
                D_0013E15A[0] = 1;
            }
            break;
        set20:
            if (R.x24 != 0) {
                break;
            }
            if (PAD & 0x10) {
                { int fl = D_0015EFB4; fl |= 0x20; fl &= ~2; fl &= ~4; D_0015EFB4 = fl; }
                D_0015F6E8 = R.x14;
            }
            break;
        case 8:
            if (D_0015F6C8 == 0 || (D_0015EFB4 & 6) != 0) {
                break;
            }
            D_0015F6E8 = R.x14;
            break;
        }
        break;
    }
    if ((unsigned int)(D_0015F6E8 - 3) >= 2) {
        func_0012E558(0x1D);
        func_00216F28();
        func_0012DDC0();
    }
}
