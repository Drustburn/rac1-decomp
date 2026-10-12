/* NON_MATCHING func_L15_002A3B98 -- src/overlays/shared/vendor_00298BB8.c
 * Best so far: BYTES 25/568 (95.6% of the bytes match), checked 2026-10-12.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   func_L15_002A3B98: per-moby draw callback. best.c: SIZE equal (568), BYTES 25/568; all in the first two blocks
 */
typedef int u128_2A3B98 __attribute__((mode(TI)));
extern int func_001F4868(int);
extern void func_001F9BC0(float *);
extern float func_L00_001FF860(float, float);
extern void func_001FA218(float *, float *);
extern void func_L00_001FD1D8(void *, void *, int);
extern float D_L15_001CE490_x[] __asm__("D_L15_001CE490");
extern float D_L15_001CE390_x[] __asm__("D_L15_001CE390");
extern int D_L15_001AC140_x[] __asm__("D_L15_001AC140");
extern char D_L15_00167300_x[] __asm__("D_L15_00167300");
extern int D_L15_00160058_m_x __asm__("D_L15_00160058") MACRO_ADDR;
extern short D_L15_00161544_x __asm__("D_L15_00161544");

// Draws the model matrices for each moby in the room's list, oriented toward the camera.
void func_L15_002A3B98_impl(char *m) __asm__("func_L15_002A3B98");
void func_L15_002A3B98_impl(char *m) {
    float mtx[16];
    int cols[4];
    float t[8];
    long g[4];
    float A[4];
    float B[16];
    short *p;
    char *c;
    float *w;
    int i, j, k;
    g[1] = func_001F4868(8);
    g[2] = 0xFF9000000260L;
    g[3] = 0x8000000044L;
    g[0] = 0;
    {
        float *sp_ = D_L15_001CE490_x;
        float *d1 = t + 1;
        float *d0 = t;
        for (i = 3; i >= 0; i--) {
            *d0 = sp_[0];
            *d1 = sp_[1];
            sp_ += 2;
            d0 += 2;
            d1 += 2;
        }
    }
    p = (short *)D_L15_001AC140_x[*(unsigned char *)(m + 0x21)];
    w = B + 12;
    c = D_L15_00167300_x;
    do {
        char *o = (char *)D_L15_00160058_m_x + ((*(unsigned short *)p & 0x7FFF) << 8);
        if (*(unsigned char *)(o + 0x20) != 1) {
            char *pos = o + 0x10;
            float d;
            func_001F9BC0(A);
            A[2] = func_L00_001FF860(*(float *)(c + 0x140) - *(float *)(o + 0x10), *(float *)(c + 0x144) - *(float *)(o + 0x14));
            d = func_001F9D48((float *)pos, (float *)(c + 0x140));
            A[1] = -func_L00_001FF860(d, *(float *)(c + 0x148) - *(float *)(o + 0x18));
            func_001FA218(B, A);
            qcopy(w, pos);
            B[14] += *(float *)&D_L15_00161544_x;
            B[15] = 1.0f;
            for (j = 0; j < 4; j++) {
                k = 0;
                do {
                    cols[k] = *(int *)(&D_L15_00161544_x + 2 + j * 2);
                    qcopy_nc(mtx + k * 4, D_L15_001CE390_x + j * 16 + k * 4);
                    k++;
                } while (k < 4);
                func_L00_001FD1D8(mtx, B, 0);
            }
        }
    } while (*p++ >= 0);
}
