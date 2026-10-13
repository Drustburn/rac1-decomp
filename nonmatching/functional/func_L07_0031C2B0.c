/* functional: high. Written from the retail asm; the compiler merges two identical tails (start effect data+0xD0, state 2) into one call site, folds the 0xFE/0xFD tests into one range test and uses a compare tree for the switch, so every path makes the same reads, writes and calls as retail. */
/* Level 07 update for a moby that moves between two positions, driven by the state (+0x20) of a trigger
 * moby whose index is in data+0xB0 (data at moby+0x78; the index is cleared when the level's table
 * D_0014C150 says the moby's +0xB0 slot is unused). State 0 records the start position in data+0xA0 and goes
 * to 1. State 1 waits until the trigger is in state data+0xB4 (or gone), measures the length of the offset
 * data+0xBC into data+0xAC and goes to 2. State 2 moves along that offset, rotated by the moby's matrix at
 * +0xC0, by data+0xC8 per frame until the length is used up (then it snaps to start+offset and goes to 3),
 * or goes to 4 when the trigger is in state data+0xB8. State 3 waits for data+0xB8 and goes to 4. State 4
 * moves back by data+0xCC per frame while the trigger is not in data+0xB4, snapping to the start and going
 * to 1 when it passes it; if the trigger is in data+0xB4 (or gone) it goes to 2. The effects/sounds in
 * data+0xD0/0xD4 are started with func_0022ED80 and D_L07_001845A8[data+0xD8] is 1 while it is at the far
 * end. Every frame it then passes data+0x60, its motion this frame (new position minus old) and its old
 * and new +0x40 rows to func_L00_002617B0. */
typedef struct { int w[4]; } __attribute__((aligned(16))) Q_31C2B0;

extern void func_001F9C30_31C2B0(void *, void *, float) __asm__("func_001F9C30");
extern float func_001F9CB8_31C2B0(void *) __asm__("func_001F9CB8");
extern int func_0022ED80_31C2B0(int, int, int) __asm__("func_0022ED80");
extern void func_001F9EC0_31C2B0(void *, void *, void *) __asm__("func_001F9EC0");
extern void func_L00_001FF4B0_31C2B0(void *, void *, float) __asm__("func_L00_001FF4B0");
extern void func_001F9BD8_31C2B0(void *, void *, void *) __asm__("func_001F9BD8");
extern void func_L00_002617B0_31C2B0(void *, void *, void *, void *) __asm__("func_L00_002617B0");
extern int D_0015EE84_31C2B0 __asm__("D_0015EE84") MACRO_ADDR;
extern unsigned char D_0014C150_31C2B0[][16] __asm__("D_0014C150");
extern char *D_L07_00160058_31C2B0 __asm__("D_L07_00160058") MACRO_ADDR;
extern int D_L07_001845A8_31C2B0[] __asm__("D_L07_001845A8");
extern float D_0015EE6C_31C2B0 __asm__("D_0015EE6C") MACRO_ADDR;

#define D_31C2B0(off, type) (*(type *)(d + (off)))

void func_L07_0031C2B0(unsigned char *moby)
{
    char *d;
    char *o;
    int idx;
    unsigned char st;
    float delta[4];
    Q_31C2B0 oldm;
    float v[4];
    float w[4];
    float len;
    float t;

    d = *(char **)(moby + 0x78);
    func_001F9C30_31C2B0(delta, moby + 0x10, -1.0f);
    oldm = *(Q_31C2B0 *)(moby + 0x40);

    st = moby[0xB0];
    if (st != 0xFF && D_0014C150_31C2B0[D_0015EE84_31C2B0][st] == 0xFF)
        D_31C2B0(0xB0, int) = -1;

    switch (moby[0x20]) {
    case 0:
        *(Q_31C2B0 *)(d + 0xA0) = *(Q_31C2B0 *)(moby + 0x10);
        if (D_31C2B0(0xD8, int) != -1)
            D_L07_001845A8_31C2B0[D_31C2B0(0xD8, int)] = 0;
        moby[0x20] = 1;
        D_31C2B0(0xAC, int) = 0;
        break;

    case 1:
        idx = D_31C2B0(0xB0, int);
        if (idx != -1) {
            o = D_L07_00160058_31C2B0 + (idx << 8);
            if (o != 0) {
                st = ((unsigned char *)o)[0x20];
                if (st != 0xFE && st != 0xFD && st != D_31C2B0(0xB4, int))
                    break;
            }
        }
        w[0] = 0; w[1] = 0; w[2] = 0; w[3] = 0;
        w[0] = D_31C2B0(0xBC, float);
        w[1] = D_31C2B0(0xC0, float);
        w[2] = D_31C2B0(0xC4, float);
        v[0] = w[0]; v[1] = w[1]; v[2] = w[2]; v[3] = w[3];
        D_31C2B0(0xAC, float) = func_001F9CB8_31C2B0(v);
        if (D_31C2B0(0xD0, int) != -1)
            func_0022ED80_31C2B0(D_31C2B0(0xD0, int), 0, (int)moby);
        moby[0x20] = 2;
        break;

    case 2:
        idx = D_31C2B0(0xB0, int);
        if (idx != -1) {
            o = D_L07_00160058_31C2B0 + (idx << 8);
            if (o != 0) {
                st = ((unsigned char *)o)[0x20];
                if (st != 0xFE && st != 0xFD && st == D_31C2B0(0xB8, int)) {
                    if (D_31C2B0(0xD0, int) != -1)
                        func_0022ED80_31C2B0(D_31C2B0(0xD0, int), 0, (int)moby);
                    moby[0x20] = 4;
                    break;
                }
            }
        }
        w[0] = 0; w[1] = 0; w[2] = 0; w[3] = 0;
        w[0] = D_31C2B0(0xBC, float);
        w[1] = D_31C2B0(0xC0, float);
        w[2] = D_31C2B0(0xC4, float);
        v[0] = w[0]; v[1] = w[1]; v[2] = w[2]; v[3] = w[3];
        func_001F9EC0_31C2B0(v, v, moby + 0xC0);
        func_L00_001FF4B0_31C2B0(w, v, D_31C2B0(0xC8, float) * D_0015EE6C_31C2B0);
        func_001F9BD8_31C2B0(moby + 0x10, moby + 0x10, w);
        t = D_31C2B0(0xAC, float) - D_31C2B0(0xC8, float) * D_0015EE6C_31C2B0;
        D_31C2B0(0xAC, float) = t;
        if (t < 0.0f) {
            func_001F9BD8_31C2B0(moby + 0x10, d + 0xA0, v);
            D_31C2B0(0xAC, float) = 0.0f;
            if (D_31C2B0(0xD8, int) != -1)
                D_L07_001845A8_31C2B0[D_31C2B0(0xD8, int)] = 1;
            moby[0x20] = 3;
            if (D_31C2B0(0xD4, int) != -1)
                func_0022ED80_31C2B0(D_31C2B0(0xD0, int), 0, (int)moby);
        }
        break;

    case 3:
        idx = D_31C2B0(0xB0, int);
        if (idx == -1)
            break;
        o = D_L07_00160058_31C2B0 + (idx << 8);
        if (o == 0)
            break;
        st = ((unsigned char *)o)[0x20];
        if (st == 0xFE || st == 0xFD || st != D_31C2B0(0xB8, int))
            break;
        D_31C2B0(0xAC, int) = 0;
        if (D_31C2B0(0xD0, int) != -1)
            func_0022ED80_31C2B0(D_31C2B0(0xD0, int), 0, (int)moby);
        moby[0x20] = 4;
        break;

    case 4:
        idx = D_31C2B0(0xB0, int);
        if (idx != -1) {
            o = D_L07_00160058_31C2B0 + (idx << 8);
            if (o != 0) {
                st = ((unsigned char *)o)[0x20];
                if (st != 0xFE && st != 0xFD && st != D_31C2B0(0xB4, int)) {
                    w[0] = 0; w[1] = 0; w[2] = 0; w[3] = 0;
                    w[0] = D_31C2B0(0xBC, float);
                    w[1] = D_31C2B0(0xC0, float);
                    w[2] = D_31C2B0(0xC4, float);
                    v[0] = w[0]; v[1] = w[1]; v[2] = w[2]; v[3] = w[3];
                    len = func_001F9CB8_31C2B0(v);
                    func_L00_001FF4B0_31C2B0(v, v, -(D_31C2B0(0xCC, float) * D_0015EE6C_31C2B0));
                    func_001F9EC0_31C2B0(w, v, moby + 0xC0);
                    func_001F9BD8_31C2B0(moby + 0x10, moby + 0x10, w);
                    t = D_31C2B0(0xAC, float) + D_31C2B0(0xCC, float) * D_0015EE6C_31C2B0;
                    D_31C2B0(0xAC, float) = t;
                    if (len < t) {
                        *(Q_31C2B0 *)(moby + 0x10) = *(Q_31C2B0 *)(d + 0xA0);
                        if (D_31C2B0(0xD0, int) != -1)
                            func_0022ED80_31C2B0(D_31C2B0(0xD4, int), 0, (int)moby);
                        if (D_31C2B0(0xD8, int) != -1)
                            D_L07_001845A8_31C2B0[D_31C2B0(0xD8, int)] = 0;
                        moby[0x20] = 1;
                    }
                    break;
                }
            }
        }
        if (D_31C2B0(0xD0, int) != -1)
            func_0022ED80_31C2B0(D_31C2B0(0xD0, int), 0, (int)moby);
        moby[0x20] = 2;
        break;
    }

    func_001F9BD8_31C2B0(delta, delta, moby + 0x10);
    func_L00_002617B0_31C2B0(d + 0x60, delta, &oldm, moby + 0x40);
}
#undef D_31C2B0
