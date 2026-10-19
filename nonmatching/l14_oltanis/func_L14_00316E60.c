/* NON_MATCHING func_L14_00316E60 -- src/overlays/l14_oltanis/vendor_002FF358.c
 * Best so far: BYTES 10/880 (98.9% of the bytes match), checked 2026-10-19.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   camera paths into D_L14_001F6FC0, resets the saved state, loads the default angles, places the camera behind t
 *   defaults every key duration to 10000. The 1.0f flag at D_L14_00162578 must be an SDATA float (a short cast swa
 *   two hoisted constants, 36 bytes); the third zone-table read is a second alias of D_L14_0015F7EC (Lombyte uses 
 *   volatile alias). BYTES 10/880: the key loop. Retail fills the `if (*r == 0)` slot with the store (beql) and ke
 *   counter decrement after the join; our compiler steals the decrement into the slot (bne/addiu/sw) whatever the 
 *   written as: for/do-while with the decrement before or after the pointer step, post-decrement test (bgtz, 17 by
 *   count-up with dbra conversion (12 bytes), Lombyte's goto layout (size). What would unblock it: a loop shape wh
 *   starts with the pointer step (it conflicts with the store, so reorg would not steal it).
 */
extern char *D_L14_0015F7EC_16E60 __asm__("D_L14_0015F7EC") MACRO_ADDR;
/* the third path read through a second alias: retail reloads the zone table there */
extern char *D_L14_0015F7EC_16E60b __asm__("D_L14_0015F7EC") MACRO_ADDR;
extern float D_L14_001F6FC0[][1000];
extern float D_L14_001F67C8_16E60[] __asm__("D_L14_001F67C8");
extern float D_L14_00162578_16E60 SDATA(D_L14_00162578);
extern void func_L00_001ED900_16E60(void *, void *, void *) __asm__("func_L00_001ED900");

/* Starts the Oltanis vendor camera CAM: measures its record's three camera paths into the per-path length tables
   D_L14_001F6FC0, resets the camera's blend state (+0x88 and +0x74 of the saved state), loads the default angles
   from D_L14_001F67C8 (degrees to radians), places the camera behind the hero looking at the hero, and gives every
   camera key without a duration the default 10000.
   Adapted from Lombyte (MIT) for PAL: src/overlays/l14/gameplay/entities/morphoray.c, FUN_L14_003159d8. */
void func_L14_00316E60(struct UpdateCam *cam) {
    float ang[4];
    float pad[4]; /* unused; keeps the frame layout */
    float dir[4];
    float up[4];
    char *rec = *(char **)(D_L14_0015F050 + cam->descriptor_index * 32 + 0x1C);
    char *path;
    char *tbl;
    char *p;
    float *v;
    int *q;
    int j;
    int *w;
    int *r;
    float sum;
    float f;
    int i;

    tbl = D_L14_0015F7EC_16E60;
    *(int *)(rec + 0x20) = 2;
    *(short *)(rec + 0x30) = 0;
    *(short *)(rec + 0x32) = 0;
    path = *(char **)(tbl + *(int *)(rec + 0x34) * 32 + 0x10);
    sum = 0.0f;
    for (i = 0; i < *(int *)path - 1; i++) {
        D_L14_001F6FC0[0][i] = sum;
        sum += *(float *)(path + i * 16 + 0x1C);
    }
    D_L14_001F6FC0[0][i] = sum;
    path = *(char **)(D_L14_0015F7EC_16E60 + *(int *)(rec + 0x38) * 32 + 0x10);
    sum = 0.0f;
    for (i = 0; i < *(int *)path - 1; i++) {
        D_L14_001F6FC0[1][i] = sum;
        sum += *(float *)(path + i * 16 + 0x1C);
    }
    D_L14_001F6FC0[1][i] = sum;
    path = *(char **)(D_L14_0015F7EC_16E60b + *(int *)(rec + 0x3C) * 32 + 0x10);
    sum = 0.0f;
    for (i = 0; i < *(int *)path - 1; i++) {
        D_L14_001F6FC0[2][i] = sum;
        sum += *(float *)(path + i * 16 + 0x1C);
    }
    D_L14_001F6FC0[2][i] = sum;
    p = cam->saved_state;
    *(int *)(p + 0x88) = 1;
    q = (int *)(p + 0x88);
    q[1] = 0;
    q[2] = 0;
    q[3] = 0;
    q[4] = 0;
    D_L14_00162578_16E60 = 1.0f;
    v = (float *)((char *)cam->saved_state + 0x60);
    v[0] = D_L14_001F67C8_16E60[0] * 0.017453292f;
    v[1] = D_L14_001F67C8_16E60[1] * 0.017453292f;
    v[2] = D_L14_001F67C8_16E60[2];
    v[3] = D_L14_001F67C8_16E60[3] * 0.017453292f;
    v[4] = D_L14_001F67C8_16E60[4] * 0.017453292f;
    w = (int *)((char *)cam->saved_state + 0x74);
    *w = 0;
    w[1] = 0;
    w[2] = 0;
    w[3] = 0;
    w[4] = 0;
    f = func_L00_001FF860(D_0013F450.moby->rot_row1.x, D_0013F450.moby->rot_row1.y);
    ang[1] = v[1];
    ang[0] = f;
    ang[2] = v[2];
    func_L00_001ED900_16E60(&cam->pos, ang, &D_0013F450.motion);
    func_001F9BF0(dir, &D_0013F450.motion, &cam->pos);
    func_001F9C30(up, &D_0013F450.gravity_dir, -1.5f);
    func_001F9BD8(dir, dir, up);
    func_L00_001FF4B0(&cam->m0, dir, 1.0f);
    func_001F9CA0(&cam->m1, &cam->m0, up);
    func_L00_001FF4B0(&cam->m1, &cam->m1, 1.0f);
    func_001F9CA0(&cam->m2, &cam->m1, &cam->m0);
    /* give every key without a duration (+0x44) the default 10000 */
    r = (int *)&D_L14_001F67C8_16E60[0x11];
    j = 38;
    do {
        if (*r == 0)
            *r = 10000;
        j--;
        r += 12;
    } while (j >= 0);
}
