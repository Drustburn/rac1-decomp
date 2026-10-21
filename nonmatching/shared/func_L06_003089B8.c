/* NON_MATCHING func_L06_003089B8 -- src/overlays/shared/vendor_002FF000.c
 * Best so far: BYTES 10/1740 (99.4% of the bytes match), checked 2026-10-21.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   From Lombyte's non-matching draft of FUN_L06_00307578 (bot_door_lock.c, GPL-3.0), PAL symbols.
 *   Left: the constants 1.4 and 1.1 of the z offsets swap $f4/$f5 (retail 1.4 in $f5). Corner, copy and
 *   rotation statement orders, operand orders and a 900 s permuter run did not move them (2026-10-21, m3).
 *   Fixing the file's prototype (void (char *)) lets the plain name be defined; the alias avoids that here.
 */
typedef u32 VecQ_89B8 __attribute__((mode(TI), aligned(16)));
typedef union {
    VecQ_89B8 q;
    float f[4];
    int i[4];
} Vec4q_89B8;
/* The quad the level draw helper func_L00_001FD1D8 takes. */
typedef struct {
    Vec4q_89B8 positions[4];
    unsigned int colors[4];
    float texture_coordinates[4][2];
    union {
        unsigned long value;
        volatile unsigned long ordered;
    } reserved;
    unsigned long texture;
    unsigned long texture_state;
    unsigned long primitive;
} GeometryQuad_89B8;
extern unsigned long f_tex_89B8(int) __asm__("func_001F4868");
extern void f_rotbasis_89B8(void *, void *) __asm__("func_001FA1F8");
extern void f_xform_89B8(void *, void *, void *) __asm__("func_001F9EC0");
extern void f_draw_89B8(void *, void *, int) __asm__("func_L00_001FD1D8");
extern float D_L06_00167500_89B8[] __asm__("D_L06_00167500");
extern Vec4q_89B8 D_L06_001FEC20_89B8[] __asm__("D_L06_001FEC20");
extern float D_L06_001FECB0_89B8[][2] __asm__("D_L06_001FECB0");
extern unsigned int D_L06_001FECF8_89B8[] __asm__("D_L06_001FECF8");
extern int D_L06_001FED20_89B8[][4] __asm__("D_L06_001FED20");
void func_L06_003089B8_r(char *moby) __asm__("func_L06_003089B8");

void func_L06_003089B8_r(char *moby) {
    GeometryQuad_89B8 quad;
    Vec4q_89B8 result[4];
    float rotation[4];
    float matrix[12];
    Vec4q_89B8 pos[4];
    Vec4q_89B8 corners[4];
    int i;
    Vec4q_89B8 *pos0, *pos1, *pos2, *pos3;
    Vec4q_89B8 *result0, *result1, *result2, *result3;
    float *data = *(float **)(moby + 0x78);
    rotation[0] = 0.0f; rotation[1] = 0.0f; rotation[2] = D_L06_00167500_89B8[86];
    f_rotbasis_89B8(matrix, rotation);
    data[7] += 0.01f;
    if (1.0f < data[7]) data[7] -= 1.0f;
    quad.texture_state = 1;
    quad.primitive = 0x4000000044UL;
    quad.reserved.value = 0;
    quad.texture = f_tex_89B8(0x18);
    result0 = &result[0];
    result1 = &result[1];
    result2 = &result[2];
    result3 = &result[3];
    pos0 = &pos[0];
    pos1 = &pos[1];
    pos2 = &pos[2];
    pos3 = &pos[3];
    for (i = 0; i < 4; i++) {
        f_xform_89B8(result0, &D_L06_001FEC20_89B8[D_L06_001FED20_89B8[i][0]], matrix);
        f_xform_89B8(result1, &D_L06_001FEC20_89B8[D_L06_001FED20_89B8[i][1]], matrix);
        f_xform_89B8(result2, &D_L06_001FEC20_89B8[D_L06_001FED20_89B8[i][2]], matrix);
        f_xform_89B8(result3, &D_L06_001FEC20_89B8[D_L06_001FED20_89B8[i][3]], matrix);
        quad.positions[0].f[0] = *(float *)(moby + 16) + result[0].f[0] * data[9];
        quad.positions[0].f[1] = *(float *)(moby + 20) + result[0].f[1] * data[9];
        quad.positions[0].f[2] = *(float *)(moby + 24) + result[0].f[2];
        quad.positions[1].f[0] = *(float *)(moby + 16) + result[1].f[0] * data[9];
        quad.positions[1].f[1] = *(float *)(moby + 20) + result[1].f[1] * data[9];
        quad.positions[1].f[2] = *(float *)(moby + 24) + result[1].f[2];
        quad.positions[2].f[0] = *(float *)(moby + 16) + result[2].f[0] * data[9];
        quad.positions[2].f[1] = *(float *)(moby + 20) + result[2].f[1] * data[9];
        quad.positions[2].f[2] = *(float *)(moby + 24) + result[2].f[2];
        quad.positions[3].f[0] = *(float *)(moby + 16) + result[3].f[0] * data[9];
        quad.positions[3].f[1] = *(float *)(moby + 20) + result[3].f[1] * data[9];
        quad.positions[3].f[2] = *(float *)(moby + 24) + result[3].f[2];
        quad.colors[0] = D_L06_001FECF8_89B8[D_L06_001FED20_89B8[i][0]];
        quad.colors[1] = D_L06_001FECF8_89B8[D_L06_001FED20_89B8[i][1]];
        quad.colors[2] = D_L06_001FECF8_89B8[D_L06_001FED20_89B8[i][2]];
        quad.colors[3] = D_L06_001FECF8_89B8[D_L06_001FED20_89B8[i][3]];
        quad.texture_coordinates[0][0] = D_L06_001FECB0_89B8[D_L06_001FED20_89B8[i][0]][0];
        quad.texture_coordinates[0][1] = D_L06_001FECB0_89B8[D_L06_001FED20_89B8[i][0]][1] + data[7];
        quad.texture_coordinates[1][0] = D_L06_001FECB0_89B8[D_L06_001FED20_89B8[i][1]][0];
        quad.texture_coordinates[1][1] = D_L06_001FECB0_89B8[D_L06_001FED20_89B8[i][1]][1] + data[7];
        quad.texture_coordinates[2][0] = D_L06_001FECB0_89B8[D_L06_001FED20_89B8[i][2]][0];
        quad.texture_coordinates[2][1] = D_L06_001FECB0_89B8[D_L06_001FED20_89B8[i][2]][1] + data[7];
        quad.texture_coordinates[3][0] = D_L06_001FECB0_89B8[D_L06_001FED20_89B8[i][3]][0];
        quad.texture_coordinates[3][1] = D_L06_001FECB0_89B8[D_L06_001FED20_89B8[i][3]][1] + data[7];
        f_draw_89B8(&quad, 0, 1);
    }
    quad.texture = f_tex_89B8(0x1b);
    data[8] += 0.01f;
    if (1.0f < data[8]) data[8] = 0.0f;
    corners[0].q = 0;
    corners[0].f[0] = -data[8] * 1.2f * data[9];
    corners[0].f[1] = -data[8] * 1.2f * data[9];
    corners[0].f[2] = data[8] * 1.4f + 1.1f;
    corners[1].q = 0;
    corners[1].f[0] = data[8] * 1.2f * data[9];
    corners[1].f[1] = -data[8] * 1.2f * data[9];
    corners[1].f[2] = data[8] * 1.4f + 1.1f - 0.2f;
    corners[2].q = 0;
    corners[2].f[0] = -data[8] * 1.2f * data[9];
    corners[2].f[1] = data[8] * 1.2f * data[9];
    corners[2].f[2] = data[8] * 1.4f + 1.1f;
    corners[3].q = 0;
    corners[3].f[0] = data[8] * 1.2f * data[9];
    corners[3].f[1] = data[8] * 1.2f * data[9];
    corners[3].f[2] = data[8] * 1.4f + 1.1f - 0.2f;
    pos[0].q = corners[0].q;
    pos[1].q = corners[1].q;
    pos[2].q = corners[2].q;
    rotation[0] = 0.0f; rotation[1] = 0.0f; rotation[2] = D_L06_00167500_89B8[86];
    pos[3].q = corners[3].q;
    f_rotbasis_89B8(matrix, rotation);
    f_xform_89B8(result0, pos0, matrix);
    f_xform_89B8(result1, pos1, matrix);
    f_xform_89B8(result2, pos2, matrix);
    f_xform_89B8(result3, pos3, matrix);
    quad.positions[0].f[0] = *(float *)(moby + 16) + result[0].f[0];
    quad.positions[0].f[1] = *(float *)(moby + 20) + result[0].f[1];
    quad.positions[0].f[2] = *(float *)(moby + 24) + result[0].f[2];
    quad.positions[1].f[0] = *(float *)(moby + 16) + result[1].f[0];
    quad.positions[1].f[1] = *(float *)(moby + 20) + result[1].f[1];
    quad.positions[1].f[2] = *(float *)(moby + 24) + result[1].f[2];
    quad.positions[2].f[0] = *(float *)(moby + 16) + result[2].f[0];
    quad.positions[2].f[1] = *(float *)(moby + 20) + result[2].f[1];
    quad.positions[2].f[2] = *(float *)(moby + 24) + result[2].f[2];
    quad.positions[3].f[0] = *(float *)(moby + 16) + result[3].f[0];
    quad.positions[3].f[1] = *(float *)(moby + 20) + result[3].f[1];
    quad.positions[3].f[2] = *(float *)(moby + 24) + result[3].f[2];
    quad.colors[0] = ((unsigned int)(int)((1.0f - data[8]) * 128.0f) << 24) | 0x808080;
    quad.colors[3] = quad.colors[2] = quad.colors[1] = quad.colors[0];
    quad.texture_coordinates[0][0] = 0.0f; quad.texture_coordinates[0][1] = 0.0f;
    quad.texture_coordinates[1][0] = 1.0f; quad.texture_coordinates[1][1] = 0.0f;
    quad.texture_coordinates[2][0] = 0.0f; quad.texture_coordinates[2][1] = 1.0f;
    quad.texture_coordinates[3][0] = 1.0f; quad.texture_coordinates[3][1] = 1.0f;
    f_draw_89B8(&quad, 0, 1);
}
