/* NON_MATCHING func_L00_00207220 -- src/overlays/shared/help_00203E98.c
 * Best so far: SIZE ours 244 / retail 240, checked 2026-10-12.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   func_L00_00207220: sound-slot pruner (8 slots at hero+0x2218, mobys at +0x2238, rows of 0x70 at D_0013E650). B
 */
typedef struct { char p0[0x74]; unsigned char f74; char p1[0x13]; Moby *f88; } Row13E650_207220;
extern Hero D_0013F450_207220 __asm__("D_0013F450");
extern char D_0013E650_207220[] __asm__("D_0013E650");
extern void func_L00_0028EBF0_a(void) __asm__("func_L00_0028EBF0");

void func_L00_00207220(void) {
    int i;
    int none;
    int *p;
    Moby *m;
    int k;
    Row13E650_207220 *row;

    none = -1;
    p = &D_0013F450_207220.unk2218;
    for (i = 0; i < 8; i++, p++) {
        k = *p;
        m = D_0013F450_207220.unk2238[i];
        if (m != 0) {
            if (k != none) {
                row = (Row13E650_207220 *)(D_0013E650_207220 + k * 0x70);
                if (row->f88 == m && row->f74 != 0) {
                    func_L00_0028EBF0_a();
                }
            }
        } else {
            if (k != none) {
                m = *(Moby **)((char *)&D_0013F450_207220 + 0x2080);
                row = (Row13E650_207220 *)(D_0013E650_207220 + k * 0x70);
                if (row->f88 == m && row->f74 != 0) {
                    func_L00_0028EBF0_a();
                }
            }
        }
        *p = none;
    }
}
