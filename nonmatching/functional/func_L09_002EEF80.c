/* functional: high. Exact functional implementation of retail func_L09_002EEF80 (Gaspar).
 * Handles countdown and detonation/burst effect of timed explosive moby classes 0x512/0x513/0x514.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef short s16;
typedef int s32;
typedef int openrac_ti __attribute__((mode(TI)));

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec4_2EEF80;

extern unsigned char D_0014171B_2EEF80[] __asm__("D_0014171B");
extern int D_0015EE84_2EEF80 __asm__("D_0015EE84");
extern float D_L09_00166FC0_2EEF80[] __asm__("D_L09_00166FC0");
extern float D_0015EE6C_2EEF80 __asm__("D_0015EE6C");

extern void func_0020D678_2EEF80(void *) __asm__("func_0020D678");
extern void *func_L00_0025B478_2EEF80(void *, int, int) __asm__("func_L00_0025B478");
extern int func_L00_001FEF78_2EEF80(void *) __asm__("func_L00_001FEF78");
extern void func_L00_002584A8_2EEF80(void *, int, int) __asm__("func_L00_002584A8");
extern char *func_L09_002EEEB0_2EEF80(char *, int) __asm__("func_L09_002EEEB0");
extern int func_001F9850_2EEF80(int) __asm__("func_001F9850");
extern void func_L00_00260958_2EEF80(void *, float) __asm__("func_L00_00260958");
extern int func_0022ED80_2EEF80(int, int, void *) __asm__("func_0022ED80");
extern void func_001F9BF0_2EEF80(void *, void *, void *) __asm__("func_001F9BF0");
extern void func_L00_001FF4B0_2EEF80(void *, void *, float) __asm__("func_L00_001FF4B0");
extern float func_002140F8_2EEF80(float, float) __asm__("func_002140F8");
extern void func_001F9EC0_2EEF80(void *, void *, void *) __asm__("func_001F9EC0");
extern void func_001F9BD8_2EEF80(void *, void *, void *) __asm__("func_001F9BD8");
extern float func_001F9CB8_2EEF80(void *) __asm__("func_001F9CB8");
extern float func_001F9B88_2EEF80(float) __asm__("func_001F9B88");
extern int func_002140B0_2EEF80(int) __asm__("func_002140B0");
extern char *func_L09_002F0BB8_2EEF80(char *, void *, void *, int, float) __asm__("func_L09_002F0BB8");
extern void func_L00_00260108_2EEF80(void *, void *, int, float, float) __asm__("func_L00_00260108");
extern void func_001F9C30_2EEF80(void *, void *, float) __asm__("func_001F9C30");
extern void func_L00_0025F4A8_2EEF80(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("func_L00_0025F4A8");

static inline void qcopy_2EEF80(void *dst, const void *src) {
    *(openrac_ti *)dst = *(const openrac_ti *)src;
}

void func_L09_002EEF80(char *moby) {
    u8 b0 = *(u8 *)(moby + 0xB0);
    void *hit;
    int spawned_or_nonzero;
    s16 cls;
    Vec4_2EEF80 v20;
    Vec4_2EEF80 v30;
    Vec4_2EEF80 v40;
    Vec4_2EEF80 v50;
    Vec4_2EEF80 v60;
    Vec4_2EEF80 v70;
    int i;
    int div_val;
    float max_speed;
    float min_speed;
    float dist;
    float progress;

    if (b0 != 0xFF) {
        int idx = (int)b0 + (D_0015EE84_2EEF80 << 4);
        if (D_0014171B_2EEF80[0xAA35 + idx] == 0xFF) {
            func_0020D678_2EEF80(moby);
            return;
        }
    }

    hit = func_L00_0025B478_2EEF80(moby, 0x40000, 0);
    spawned_or_nonzero = 0;

    if (*(u8 *)(moby + 0x20) == 0) {
        *(u8 *)(moby + 0x20) = 1;
    }

    if (func_L00_001FEF78_2EEF80(moby + 0xBC) == 0) {
        return;
    }

    if (hit == 0) {
        return;
    }

    cls = *(s16 *)(moby + 0xA6);
    if (cls < 0x514) {
        if (cls == 0x512) {
            char *sp = func_L09_002EEEB0_2EEF80(moby, 0x513);
            if (sp != 0) spawned_or_nonzero = 1;
        } else if (cls == 0x513) {
            qcopy_2EEF80(&v20, moby + 0x10);
            *(float *)(moby + 0x18) += 4.0f;
            func_L00_002584A8_2EEF80(moby, 0, -1);
            qcopy_2EEF80(moby + 0x10, &v20);
            char *sp = func_L09_002EEEB0_2EEF80(moby, 0x514);
            if (sp != 0) spawned_or_nonzero = 1;
        }
    }

    *(u8 *)(moby + 0xBC) = (u8)func_001F9850_2EEF80(10);

    qcopy_2EEF80(&v30, moby + 0x10);
    v30.z += 1.25f;
    qcopy_2EEF80(&v20, &v30);

    func_L00_00260958_2EEF80(&v20, 0.4f);

    if (*(s16 *)(moby + 0xA6) < 0x513) {
        func_0022ED80_2EEF80(0, 0, moby);
    } else {
        func_0022ED80_2EEF80(1, 0, moby);
    }

    *(u8 *)(moby + 0xA4) = 0xFF;

    func_001F9BF0_2EEF80(&v40, &v20, D_L09_00166FC0_2EEF80);
    func_L00_001FF4B0_2EEF80(&v40, &v40, 2.0f);

    max_speed = 4.0f;
    if (*(s16 *)(moby + 0xA6) != 0x114) {
        max_speed = 2.0f;
    }
    min_speed = -max_speed;

    for (i = 0; i < 20; i++) {
        v60.x = func_002140F8_2EEF80(-0.5f, 0.5f);
        v60.y = func_002140F8_2EEF80(-2.0f, 2.0f);
        v60.z = func_002140F8_2EEF80(min_speed, max_speed);
        v60.w = 0.0f;

        qcopy_2EEF80(&v50, &v60);
        func_001F9EC0_2EEF80(&v50, &v50, moby + 0xC0);
        func_001F9BD8_2EEF80(&v50, &v50, &v30);
        func_001F9BF0_2EEF80(&v60, &v50, &v20);

        dist = func_001F9CB8_2EEF80(&v60);
        progress = func_001F9B88_2EEF80(dist - 3.0f) / 3.0f;

        func_L00_001FF4B0_2EEF80(&v70, (char *)hit + 0x10, progress * progress * 8.0f);
        func_001F9BD8_2EEF80(&v60, &v60, &v70);

        {
            float speed_rand = func_002140F8_2EEF80(6.0f, 10.0f) * D_0015EE6C_2EEF80;
            func_L00_001FF4B0_2EEF80(&v60, &v60, progress * speed_rand);
        }

        func_001F9BD8_2EEF80(&v50, &v50, &v40);

        div_val = 3;
        if (*(s16 *)(moby + 0xA6) != 0x514) {
            if (func_002140B0_2EEF80(4) != 0) {
                div_val = 3;
            } else {
                div_val = 3;
                goto skip_spawn_part;
            }
        }

        {
            float f_rand = func_002140F8_2EEF80(1.0f, 2.0f);
            func_L09_002F0BB8_2EEF80(moby, &v50, &v60, i & 1, f_rand);
        }

skip_spawn_part:
        if ((i % div_val) == 0) {
            float f12 = func_002140F8_2EEF80(1.0f, 1.6f);
            func_L00_00260108_2EEF80(moby, &v50, -1, f12, 0.0f);
        }
    }

    func_001F9C30_2EEF80(&v50, (char *)hit + 0x10, 0.5f);
    func_001F9BD8_2EEF80(&v20, &v20, &v40);

    func_L00_0025F4A8_2EEF80(moby, (char *)hit + 0x10, &v20,
                             0.0f, 0.0f,
                             10, 3, 16,
                             3.0f, 2.0f, 1.0f, 1.0f,
                             -1, 15.0f,
                             1, 1, -1, 0);

    if (*(s16 *)(moby + 0xA6) == 0x514 || spawned_or_nonzero != 0) {
        func_0020D678_2EEF80(moby);
    }
}
