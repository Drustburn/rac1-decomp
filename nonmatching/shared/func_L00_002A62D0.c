/* NON_MATCHING func_L00_002A62D0 -- src/overlays/shared/vendor_002A5138.c
 * Best so far: BYTES 71/1896 (96.3% of the bytes match), checked 2026-10-21.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   2026-10-21: Lombyte's draft (FUN_L00_002a5040, camera_object.c, GPL-3.0) with the PAL symbols: BYTES 71/1896,
 *   where the previous staged candidate was 721/1896 (its notes are in this file's history). The tuning floats are
 *   $gp data (SDATA D_L00_0016145C/60); Lombyte's second C name for D_L00_001D76F4 is needed (87 bytes without it).
 *   Left: &D_L00_00166D80 in $a3 where retail has $v0 at the top; the table bases (D_L00_001D7660, 7738, 76F0) and
 *   the sd spill at sp+0x78 in another order around +0x100..+0x148; one add.s/mov.s pair at +0x46c.
 */
typedef struct {
    char p0[0x158];
    float f158;
    char p1[0x180 - 0x15C];
    char *c;
} Cam_2A62D0;
typedef unsigned int Q_2A62D0 __attribute__((mode(TI), aligned(16)));
typedef union { Q_2A62D0 q; float f[4]; } V_2A62D0;
typedef struct {
    float v[4][4];
    int col[4];
    float uv[4][2];
    unsigned long pk[4];
} Quad_2A62D0;
extern Cam_2A62D0 cam_2A62D0 __asm__("D_L00_00166D80") NOT_SDA;
extern float wobble_a_2A62D0 SDATA(D_L00_0016145C);
extern float wobble_b_2A62D0 SDATA(D_L00_00161460);
extern float D_L00_001D7660_2A62D0[][4] __asm__("D_L00_001D7660");
extern float D_L00_001D76F0_2A62D0[][2] __asm__("D_L00_001D76F0");
extern float D_L00_001D76F4_2A62D0[][2] __asm__("D_L00_001D76F4");
/* Fakematch: a second name for the table makes reload re-materialise its address as retail does. */
extern float D_L00_001D76F4_lui_2A62D0[][2] __asm__("D_L00_001D76F4");
extern int D_L00_001D7738_2A62D0[] __asm__("D_L00_001D7738");
extern s32 D_L00_001D7760_2A62D0[] __asm__("D_L00_001D7760");
extern s64 get_effect_texture_2A62D0(s32) __asm__("func_001F4868");
extern void draw_geometry_quad_2A62D0(void *, void *, s32) __asm__("func_L00_001FD1D8");
extern void euler_basis_2A62D0(void *, void *) __asm__("func_001FA1F8");
extern void transform_point_2A62D0(void *, void *, void *) __asm__("func_001F9EC0");
extern void FastVecAdd_2A62D0(void *, void *, void *) __asm__("func_001F9BD8");
extern f32 FastCos_2A62D0(f32) __asm__("func_001F9F90");
extern f32 FastSin_2A62D0(f32) __asm__("func_001F9FA8");
extern void func_L00_00264690_2A62D0(void *, int, float, float) __asm__("func_L00_00264690");

/* Camera-object draw (the wobbling lens quads). Adapted from Lombyte (GPL-3.0) for PAL:
   src/overlays/shared/gameplay/entities/camera_object.c, FUN_L00_002a5040 (a near miss there too). */
void func_L00_002A62D0_r(char *m) __asm__("func_L00_002A62D0");
void func_L00_002A62D0_r(char *m) {
    char *p = *(char **)(m + 0x78);
    Quad_2A62D0 qd;
    float q[4][4];
    float d0[4];
    float mtx[3][4];
    V_2A62D0 M[4];
    V_2A62D0 T[4];
    float off, zf;
    int k, i, i0, i1, i2, i3, col;
    Cam_2A62D0 *cam;
    float rz;

    cam = &cam_2A62D0;
    rz = cam->f158;
    d0[0] = 0.0f;
    d0[1] = 0.0f;
    d0[2] = rz;
    euler_basis_2A62D0(mtx, d0);

    wobble_a_2A62D0 = wobble_a_2A62D0 + 0.01f;
    if (1.0f < wobble_a_2A62D0) {
        wobble_a_2A62D0 = wobble_a_2A62D0 - 1.0f;
    }

    qd.pk[2] = 1;
    qd.pk[3] = 0x4000000044UL;
    qd.pk[0] = 0;
    qd.pk[1] = get_effect_texture_2A62D0(0x18);

    for (k = 0; k < 4; k++) {
        float (*uvt)[2] = D_L00_001D76F4_2A62D0;
        float (*uvt2)[2] = D_L00_001D76F4_lui_2A62D0;
        transform_point_2A62D0(q[0], D_L00_001D7660_2A62D0[((int (*)[4])D_L00_001D7760_2A62D0)[k][0]], mtx);
        transform_point_2A62D0(q[1], D_L00_001D7660_2A62D0[((int (*)[4])D_L00_001D7760_2A62D0)[k][1]], mtx);
        transform_point_2A62D0(q[2], D_L00_001D7660_2A62D0[((int (*)[4])D_L00_001D7760_2A62D0)[k][2]], mtx);
        transform_point_2A62D0(q[3], D_L00_001D7660_2A62D0[((int (*)[4])D_L00_001D7760_2A62D0)[k][3]], mtx);

        qd.v[0][0] = *(float *)(m + 0x10) + q[0][0] * *(float *)(p + 0x90);
        qd.v[0][1] = *(float *)(m + 0x14) + q[0][1] * *(float *)(p + 0x90);
        qd.v[0][2] = *(float *)(m + 0x18) + q[0][2];
        qd.v[1][0] = *(float *)(m + 0x10) + q[1][0] * *(float *)(p + 0x90);
        qd.v[1][1] = *(float *)(m + 0x14) + q[1][1] * *(float *)(p + 0x90);
        qd.v[1][2] = *(float *)(m + 0x18) + q[1][2];
        qd.v[2][0] = *(float *)(m + 0x10) + q[2][0] * *(float *)(p + 0x90);
        qd.v[2][1] = *(float *)(m + 0x14) + q[2][1] * *(float *)(p + 0x90);
        qd.v[2][2] = *(float *)(m + 0x18) + q[2][2];
        qd.v[3][0] = *(float *)(m + 0x10) + q[3][0] * *(float *)(p + 0x90);
        qd.v[3][1] = *(float *)(m + 0x14) + q[3][1] * *(float *)(p + 0x90);
        qd.v[3][2] = *(float *)(m + 0x18) + q[3][2];

        i0 = ((int (*)[4])D_L00_001D7760_2A62D0)[k][0];
        i1 = ((int (*)[4])D_L00_001D7760_2A62D0)[k][1];
        i2 = ((int (*)[4])D_L00_001D7760_2A62D0)[k][2];
        i3 = ((int (*)[4])D_L00_001D7760_2A62D0)[k][3];
        off = wobble_a_2A62D0;
        qd.col[0] = D_L00_001D7738_2A62D0[i0];
        qd.col[1] = D_L00_001D7738_2A62D0[i1];
        qd.col[2] = D_L00_001D7738_2A62D0[i2];
        qd.col[3] = D_L00_001D7738_2A62D0[i3];
        qd.uv[0][0] = D_L00_001D76F0_2A62D0[i0][0];
        qd.uv[0][1] = uvt[i0][0] + off;
        qd.uv[1][0] = D_L00_001D76F0_2A62D0[i1][0];
        qd.uv[1][1] = uvt[i1][0] + off;
        qd.uv[2][0] = D_L00_001D76F0_2A62D0[i2][0];
        qd.uv[2][1] = uvt2[i2][0] + off;
        qd.uv[3][0] = D_L00_001D76F0_2A62D0[i3][0];
        qd.uv[3][1] = uvt[i3][0] + off;
        draw_geometry_quad_2A62D0(&qd, 0, 1);
    }

    qd.pk[1] = get_effect_texture_2A62D0(0x1B);

    wobble_b_2A62D0 = wobble_b_2A62D0 + 0.01f;
    if (1.0f < wobble_b_2A62D0) {
        wobble_b_2A62D0 = 0.0f;
    }

    {
        float g = wobble_b_2A62D0;
        float n = -g * 1.2f;
        float pp = g * 1.2f;
        float z = g * 1.4f + 1.1f;
        float zz = z - 0.2f;

        T[0].q = 0;
        T[0].f[0] = n * *(float *)(p + 0x90);
        T[0].f[1] = n * *(float *)(p + 0x90);
        T[0].f[2] = z;
        T[1].q = 0;
        T[1].f[0] = pp * *(float *)(p + 0x90);
        T[1].f[1] = n * *(float *)(p + 0x90);
        T[1].f[2] = zz;
        T[2].q = 0;
        T[2].f[0] = n * *(float *)(p + 0x90);
        T[2].f[1] = pp * *(float *)(p + 0x90);
        T[2].f[2] = z;
        T[3].q = 0;
        T[3].f[0] = pp * *(float *)(p + 0x90);
        T[3].f[1] = pp * *(float *)(p + 0x90);
        T[3].f[2] = zz;
        M[0].q = T[0].q;
        M[1].q = T[1].q;
        M[2].q = T[2].q;
        M[3].q = T[3].q;
    }

    /* Fakematch: setting cam a second time hides its base from alias analysis, so the
       yaw load waits for the orbit stores as in retail. */
    cam = &cam_2A62D0;
    rz = cam->f158;
    d0[0] = 0.0f;
    d0[1] = 0.0f;
    d0[2] = rz;
    euler_basis_2A62D0(mtx, d0);
    transform_point_2A62D0(q[0], M[0].f, mtx);
    transform_point_2A62D0(q[1], M[1].f, mtx);
    transform_point_2A62D0(q[2], M[2].f, mtx);
    transform_point_2A62D0(q[3], M[3].f, mtx);

    col = ((int)((1.0f - wobble_b_2A62D0) * 128.0f) << 24) | 0x808080;
    qd.v[0][0] = *(float *)(m + 0x10) + q[0][0];
    qd.v[0][1] = *(float *)(m + 0x14) + q[0][1];
    qd.v[0][2] = *(float *)(m + 0x18) + q[0][2];
    qd.v[1][0] = *(float *)(m + 0x10) + q[1][0];
    qd.v[1][1] = *(float *)(m + 0x14) + q[1][1];
    qd.v[1][2] = *(float *)(m + 0x18) + q[1][2];
    qd.v[2][0] = *(float *)(m + 0x10) + q[2][0];
    qd.v[2][1] = *(float *)(m + 0x14) + q[2][1];
    qd.v[2][2] = *(float *)(m + 0x18) + q[2][2];
    qd.v[3][0] = *(float *)(m + 0x10) + q[3][0];
    qd.v[3][1] = *(float *)(m + 0x14) + q[3][1];
    qd.v[3][2] = *(float *)(m + 0x18) + q[3][2];
    qd.col[0] = col;
    qd.col[1] = col;
    qd.col[2] = col;
    qd.col[3] = col;
    qd.uv[0][0] = 0.0f;
    qd.uv[0][1] = 0.0f;
    qd.uv[1][0] = 1.0f;
    qd.uv[1][1] = 0.0f;
    qd.uv[2][0] = 0.0f;
    qd.uv[2][1] = 1.0f;
    qd.uv[3][0] = 1.0f;
    qd.uv[3][1] = 1.0f;
    draw_geometry_quad_2A62D0(&qd, 0, 1);

    zf = 0.0f;
    for (i = 0; i < 4; i++) {
        float ang;
        /* Fakematch: re-setting zf in the loop keeps the 0.0 in one register for the whole tail. */
        zf = 0.0f;
        ang = (float)i * 3.14159f * 0.5f - 3.14159f;
        T[0].f[0] = FastCos_2A62D0(ang) * 1.1f;
        T[0].f[1] = FastSin_2A62D0(ang) * 1.1f;
        T[0].f[2] = 0.59f;
        transform_point_2A62D0(T[0].f, T[0].f, m + 0xC0);
        FastVecAdd_2A62D0(T[0].f, T[0].f, m + 0x10);
        func_L00_00264690_2A62D0(T[0].f, *(int *)(m + 0x90) & 0xFFFF0000, 0.1333f, zf);
    }
}
