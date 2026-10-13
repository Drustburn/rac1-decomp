/* functional: high. Same reads, writes and calls with the same arguments in the same order as retail; only registers, addressing and loop shape differ, plus the printf of an empty slot also passes the slot id (-1) that retail leaves in $a3. */
/* func_L10_002CB030 -- src/overlays/l10_orxon/vendor_00296BD8.c (functional C for the port, not a match)
 * Level 10 update of moby class 794 (pvars +0x78). The pvars link one moby (index at +0x10 into the
 * D_L10_00160058 table) and a list of 5 (id, short, signed char) records at +0x18. Unless the moby is
 * hidden by the stage table, in states != 0 it counts the list records whose moby exists, has the same
 * short at +0xA6, is not in state 0xFE/0xFD, whose stage bit is clear and whose state differs from the
 * wanted one (or has no wanted state). State 0 initialises (copies pose quads to/from the linked moby
 * per flags 0x10/0x40 and 0x20/0x80, logs the records, sets +0x2C = max(data[0], data[1]) * scale) and goes to 1.
 * State 1 with a zero count sets the linked moby's state, copies the timer and goes to 2. State 2 scales
 * the linked moby's byte +0x23 by the timer ratio, may go to 3 on the linked state or the timer ending.
 * State 3 frees the linked moby (unless flag 1) and itself. States 1 and 2 call func_L10_002CAD18. */
typedef unsigned int Q_2CB030 __attribute__((mode(TI), aligned(16)));

extern int func_001E9730_2CB030(char *, ...) __asm__("func_001E9730");
extern float func_001FA888_2CB030(int) __asm__("func_001FA888");
extern int func_001FA898_2CB030(float) __asm__("func_001FA898");
extern int func_001F9908_2CB030(int *) __asm__("func_001F9908");
extern void func_0020D678_2CB030(void *) __asm__("func_0020D678");
extern void func_L10_002CAD18_2CB030(void *, void *) __asm__("func_L10_002CAD18");
extern unsigned char D_0014171B_2CB030[] __asm__("D_0014171B");
extern int D_0015EE84_2CB030 __asm__("D_0015EE84") MACRO_ADDR;
extern char *D_L10_00160058_2CB030 __asm__("D_L10_00160058") MACRO_ADDR;
extern char D_L10_001DD0A0_2CB030[] __asm__("D_L10_001DD0A0");
extern char D_L10_001DD0D0_2CB030[] __asm__("D_L10_001DD0D0");
extern char D_L10_001DD100_2CB030[] __asm__("D_L10_001DD100");

void func_L10_002CB030(char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *list = data + 0x18;
    char *e = 0;
    int state, cnt = 0, k;
    char *p;
    unsigned char *tb = D_0014171B_2CB030 + 0xAA35;
    if (*(int *)(data + 0x10) != -1) e = D_L10_00160058_2CB030 + (*(int *)(data + 0x10) << 8);
    if (((unsigned char *)moby)[0xB0] == 0xFF || tb[((unsigned char *)moby)[0xB0] + (D_0015EE84_2CB030 << 4)] != 0xFF) {
        if (((unsigned char *)moby)[0x20] != 0) {
            int kw;
            char *pw = list;
            for (kw = 4; kw >= 0; kw--, pw += 8) {
                int id = *(int *)pw;
                char *e2;
                if (id == -1) continue;
                e2 = D_L10_00160058_2CB030 + (id << 8);
                if (e2 == 0) continue;
                if (*(short *)(e2 + 0xA6) != *(short *)(pw + 4)) continue;
                if (((unsigned char *)e2)[0x20] == 0xFE) continue;
                if (((unsigned char *)e2)[0x20] == 0xFD) continue;
                if ((*(int *)(D_0014171B_2CB030 + 0xAB75 + (((short)*(unsigned short *)(e2 + 0xB2) >> 5) * 4 + (D_0015EE84_2CB030 << 8))) >> (*(unsigned short *)(e2 + 0xB2) & 0x1F)) & 1) continue;
                if (*(signed char *)(pw + 6) == -1) { cnt++; continue; }
                if (((unsigned char *)e2)[0x20] == *(signed char *)(pw + 6)) continue;
                cnt++;
            }
        }
    }
    state = ((unsigned char *)moby)[0x20];
    switch (state) {
    case 0: {
        float f1, f2, m;
        if (e != 0) {
            *(short *)(data + 0x14) = *(unsigned short *)(e + 0xA6);
            *(float *)(data + 0x44) = func_001FA888_2CB030(((unsigned char *)e)[0x23]);
            if (*(int *)(data + 8) & 0x10) {
                *(Q_2CB030 *)(moby + 0x10) = *(Q_2CB030 *)(e + 0x10);
            } else if (*(int *)(data + 8) & 0x40) {
                *(Q_2CB030 *)(e + 0x10) = *(Q_2CB030 *)(moby + 0x10);
            }
            if (*(int *)(data + 8) & 0x20) {
                *(Q_2CB030 *)(moby + 0x40) = *(Q_2CB030 *)(e + 0x40);
            } else if (*(int *)(data + 8) & 0x80) {
                *(Q_2CB030 *)(e + 0x40) = *(Q_2CB030 *)(moby + 0x40);
            }
        } else {
            func_001E9730_2CB030(D_L10_001DD0A0_2CB030);
        }
        p = list;
        for (k = 0; k < 5; k++, p += 8) {
            if (*(int *)p == -1) {
                func_001E9730_2CB030(D_L10_001DD100_2CB030, (int)(moby - D_L10_00160058_2CB030) >> 8, k, -1);
            } else {
                char *e2 = D_L10_00160058_2CB030 + (*(int *)p << 8);
                *(short *)(p + 4) = *(unsigned short *)(e2 + 0xA6);
                func_001E9730_2CB030(D_L10_001DD0D0_2CB030, (int)(moby - D_L10_00160058_2CB030) >> 8, k, *(int *)p, *(short *)(e2 + 0xA6));
            }
        }
        f2 = *(float *)data;
        f1 = *(float *)(data + 4);
        m = *(float *)(*(char **)(moby + 0x24) + 0x24);
        if (f2 < f1) *(float *)(moby + 0x2C) = f1 * m;
        else *(float *)(moby + 0x2C) = f2 * m;
        moby[0x20] = 1;
        return;
    }
    case 1:
        if (cnt != 0) {
            func_L10_002CAD18_2CB030(moby, data);
            return;
        }
        if (e != 0 && *(signed char *)(data + 0x16) != -1) ((unsigned char *)e)[0x20] = *(unsigned char *)(data + 0x16);
        *(int *)(data + 0x40) = *(int *)(data + 0xC);
        moby[0x20] = 2;
        func_L10_002CAD18_2CB030(moby, data);
        return;
    case 2: {
        float a, b;
        if (e != 0 && !(*(int *)(data + 8) & 2)) {
            a = func_001FA888_2CB030(*(int *)(data + 0x40));
            b = func_001FA888_2CB030(*(int *)(data + 0xC));
            ((unsigned char *)e)[0x23] = func_001FA898_2CB030(*(float *)(data + 0x44) * a / b);
        }
        if (!(*(int *)(data + 8) & 4)) {
            if (e != 0) {
                signed char c = *(signed char *)(data + 0x17);
                if (c != -1 && ((unsigned char *)e)[0x20] == c) moby[0x20] = 3;
            }
        }
        if (!(*(int *)(data + 8) & 8)) {
            if (func_001F9908_2CB030((int *)(data + 0x40))) moby[0x20] = 3;
        }
        func_L10_002CAD18_2CB030(moby, data);
        return;
    }
    case 3:
        if (e != 0 && !(*(int *)(data + 8) & 1)) func_0020D678_2CB030(e);
        func_0020D678_2CB030(moby);
        return;
    default:
        return;
    }
}
