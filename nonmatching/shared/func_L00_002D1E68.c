/* NON_MATCHING func_L00_002D1E68 -- src/overlays/shared/vendor_002D1168.c
 * Best so far: BYTES 9/2664 (99.7% of the bytes match), checked 2026-10-19.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   2026-10-19: Lombyte port (break_crate, FUN_L00_002d09b8, matched there). Breaks a crate moby: break sound, rel
 *   the crate stack, smoke puffs, debris and trail mobys by oclass and detail level, sparks and glows when hit. BY
 *   9/2664: two spilled address pseudos swap stack slots (retail keeps &moby->pos at sp+0xE8 and &basis at sp+0xEC
 *   ours the reverse). Lombyte gets retail's slots with two empty `if (!angle) {}` tests on the not-yet-assigned a
 *   which this project's rules forbid (a read of a local never assigned). Tried instead: an early pointer local fo
 *   &moby->pos (size changes), `angle = 0.0f` at those two spots, the self copy at the top, basis passed as the ar
 *   and the position through self (all 9 bytes). Defined as func_L00_002D1E68_r: the file declares func_L00_002D1E
 *   with another type. What would unblock it: a legal way to make the &moby->pos pseudo spill before &basis's.
 */
extern int D_L00_0015F6B0_2D1E68 __asm__("D_L00_0015F6B0") MACRO_ADDR;
extern int D_L00_001619A4_2D1E68 __asm__("D_L00_001619A4") MACRO_ADDR;
extern int D_L00_001619A8_2D1E68 __asm__("D_L00_001619A8") MACRO_ADDR;
extern int D_L00_0016007C_2D1E68 __asm__("D_L00_0016007C") MACRO_ADDR;
extern float D_0015EE6C_2D1E68 __asm__("D_0015EE6C") MACRO_ADDR;
extern float D_L00_0015F6B4_2D1E68 __asm__("D_L00_0015F6B4") MACRO_ADDR;
extern char D_0013E633_2D1E68[] __asm__("D_0013E633");
extern f32 D_L00_00166D80_2D1E68[] __asm__("D_L00_00166D80");
extern char D_L00_00166EC0_2D1E68[] __asm__("D_L00_00166EC0");
extern Vec4 D_L00_001DEA40_2D1E68[] __asm__("D_L00_001DEA40");
extern Vec4 D_L00_001DEAA0_2D1E68[] __asm__("D_L00_001DEAA0");
extern u32 D_L00_001EA200_2D1E68[] __asm__("D_L00_001EA200");
extern u32 D_L00_001EA218_2D1E68[] __asm__("D_L00_001EA218");
extern int func_001F9B70_2D1E68(int) __asm__("func_001F9B70");
extern void func_L00_0028EF68_2D1E68(int, int, void *, int) __asm__("func_L00_0028EF68");
extern void func_0022ED80_2D1E68(int, int, void *) __asm__("func_0022ED80");
extern int func_L00_0025F410_2D1E68(void *) __asm__("func_L00_0025F410");
extern void func_001F9978_2D1E68(void) __asm__("func_001F9978");
extern void func_L00_0026C630_2D1E68(f32, f32, f32, f32, f32, void *, int, int) __asm__("func_L00_0026C630");
extern void func_001F9BF0_2D1E68(void *, void *, void *) __asm__("func_001F9BF0");
extern f32 func_L00_001FF860_2D1E68(f32, f32) __asm__("func_L00_001FF860");
extern f32 func_001FA790_2D1E68(f32, f32) __asm__("func_001FA790");
extern int func_001FA898_2D1E68(f32) __asm__("func_001FA898");
extern f32 func_001FA7D8_2D1E68(f32) __asm__("func_001FA7D8");
extern void func_001F9BC0_2D1E68(void *) __asm__("func_001F9BC0");
extern void func_001FA1F8_2D1E68(void *, void *) __asm__("func_001FA1F8");
extern void func_001F9EC0_2D1E68(void *, void *, void *) __asm__("func_001F9EC0");
extern void func_001F9BD8_2D1E68(void *, void *, void *) __asm__("func_001F9BD8");
extern f32 func_001FA748_2D1E68(f32, f32) __asm__("func_001FA748");
extern f32 func_002140F8_2D1E68(f32, f32) __asm__("func_002140F8");
extern void func_L00_001FF4B0_2D1E68(void *, void *, f32) __asm__("func_L00_001FF4B0");
extern int func_002140B0_2D1E68(int) __asm__("func_002140B0");
extern void *func_L00_002B0B98_2D1E68(void *, void *, void *, void *, int, int, f32, f32) __asm__("func_L00_002B0B98");
extern void func_L00_00251E30_2D1E68(void *) __asm__("func_L00_00251E30");
extern void func_L00_00258DB0_2D1E68(f32, f32, void *) __asm__("func_L00_00258DB0");
extern f32 func_00214158_2D1E68(void) __asm__("func_00214158");
extern u8 func_001F9850_2D1E68(int) __asm__("func_001F9850");
extern f32 func_001F9CB8_2D1E68(void *) __asm__("func_001F9CB8");
extern int func_L00_00258BC8_2D1E68(int, int) __asm__("func_L00_00258BC8");
extern void func_L00_0026B890_2D1E68(void *, void *, int, int, int, int, int, int, f32, f32) __asm__("func_L00_0026B890");
extern void func_L00_002ADBB0_2D1E68(void *, void *, void *, f32, int, int, int, int, int) __asm__("func_L00_002ADBB0");

/* Breaks a crate moby: plays the break sound (once per frame, bomb crates through func_L00_0028EF68), relinks the
   crate's neighbours in its stack, puffs smoke at it, picks the debris class for its oclass (500..511), throws
   count/trail debris mobys (D_L00_0016007C sets how many) spun toward the camera, marks the crate broken (state 3,
   flags bit 0, command = 15 frames), and when HIT also sprays sparks and glows scaled by the distance to the camera.
   Adapted from Lombyte (MIT) for PAL: src/overlays/shared/gameplay/entities/veldin_ship.c, break_crate (FUN_L00_002d09b8). */
void func_L00_002D1E68_r(Moby *moby, s32 hit, s32 source) __asm__("func_L00_002D1E68");
void func_L00_002D1E68_r(Moby *moby, s32 hit, s32 source) {
    u8 *vars = moby->pvars;
    Vec4 position, away, rotation, basis[3], launch, spawn_pos, spin;
    s32 kind;
    Moby *self;
    s32 close, count, trail_count, particle, i, choice;
    s32 first, second, first_color, second_color;
    f32 angle, distance, height, speed, rise;
    u32 colors[6], other_colors[6];
    void *spawned;
    u32 *effect, *velocity;

    close = 0;
    if (func_001F9B70_2D1E68(D_L00_0015F6B0_2D1E68 - D_L00_001619A4_2D1E68) > 1) {
        if (moby->oclass == 0x1FA)
            func_L00_0028EF68_2D1E68(0, 0, moby, 500);
        else
            func_0022ED80_2D1E68(0, 0, moby);
        D_L00_001619A4_2D1E68 = D_L00_0015F6B0_2D1E68;
    }
    if (func_001F9B70_2D1E68(D_L00_0015F6B0_2D1E68 - D_L00_001619A8_2D1E68) >= 2)
        D_L00_001619A8_2D1E68 = D_L00_0015F6B0_2D1E68;
    else
        close = 1;
    if (*(u8 **)(vars + 0xA0)) {
        *(u8 **)(*(u8 **)(*(u8 **)(vars + 0xA0) + 0x78) + 0xA4) = *(u8 **)(vars + 0xA4);
        *(u32 *)(*(u8 **)(*(u8 **)(vars + 0xA0) + 0x78) + 0xAC) |= 4;
    }
    if (*(u8 **)(vars + 0xA4)) {
        if (!func_L00_0025F410_2D1E68(*(void **)(*(u8 **)(*(u8 **)(vars + 0xA4) + 0x78) + 0xA0)))
            func_001F9978_2D1E68();
        *(u8 **)(*(u8 **)(*(u8 **)(vars + 0xA4) + 0x78) + 0xA0) = *(u8 **)(vars + 0xA0);
        *(u32 *)(*(u8 **)(*(u8 **)(vars + 0xA4) + 0x78) + 0xAC) |= 4;
    }
    self = moby;
    {
        s32 i;
        qcopy(&position, &moby->pos);
        position.f[2] += 0.3f;
        func_L00_0026C630_2D1E68(0.3f, 1.01f, 1.075f, 0.05f, 50000.0f, &position, 1, 0x40808080);
        for (i = 4; i >= 0; i--)
            func_L00_0026C630_2D1E68(0.3f, 1.01f, 1.07f, 0.05f, 50000.0f, &position, 1, 0x40808080);
        if (source)
            qcopy(&away, (u8 *)source + 0x10);
        else
            func_001F9BF0_2D1E68(&away, &moby->pos, (*(u8 **)(D_0013E633_2D1E68 + 0x2E9D)) + 0x10);
        switch ((s16)((u16)moby->oclass - 500)) {
        case 1: kind = 0x15C; break;
        case 3: kind = 0x15F; break;
        case 2: kind = 0x162; break;
        case 0: case 11: kind = 0x165; break;
        case 4: kind = 0x168; break;
        case 5: kind = 0x16B; break;
        case 6: kind = 0x16E; break;
        case 7: case 8: case 9: case 10: kind = 0x171; break;
        default: kind = 0x95; break;
        }
        if (D_L00_0016007C_2D1E68 >= 201) { count = 4; trail_count = 4; }
        else if (D_L00_0016007C_2D1E68 >= 151) { count = 2; trail_count = 2; }
        else if (D_L00_0016007C_2D1E68 >= 101) { count = 2; trail_count = 1; }
        else {
            if (D_L00_0016007C_2D1E68 >= 51) { count = 1; trail_count = 0; }
            else { count = 1; trail_count = 0; }
        }
        angle = D_L00_00166D80_2D1E68[80] - moby->pos.x;
        speed = D_L00_00166D80_2D1E68[81] - moby->pos.y;
        angle = func_L00_001FF860_2D1E68(angle, speed);
        angle = func_001FA790_2D1E68(angle, moby->rot.z);
        angle += 3.1415925f;
        choice = func_001FA898_2D1E68(angle / 1.57f);
        angle = func_001FA7D8_2D1E68((f32)choice * 1.57f);
        func_001F9BC0_2D1E68(&rotation);
        rotation.f[2] = angle;
        func_001FA1F8_2D1E68(&basis, &rotation);
        for (i = 0; i < count; ++i) {
            func_001F9EC0_2D1E68(&spawn_pos, &D_L00_001DEA40_2D1E68[i], &basis);
            func_001F9BD8_2D1E68(&spawn_pos, &spawn_pos, &moby->pos);
            qcopy(&spin, &D_L00_001DEAA0_2D1E68[i]);
            spin.f[2] = func_001FA748_2D1E68(spin.f[2], rotation.f[2]);
            func_001F9BF0_2D1E68(&launch, &spawn_pos, &moby->pos);
            launch.f[2] = 0.0f;
            speed = func_002140F8_2D1E68(1.0f, 5.0f);
            func_L00_001FF4B0_2D1E68(&launch, &launch, speed * D_0015EE6C_2D1E68);
            launch.f[2] = func_002140F8_2D1E68(4.0f, 9.0f) * D_0015EE6C_2D1E68;
            particle = func_002140B0_2D1E68(2);
            spawned = func_L00_002B0B98_2D1E68(moby, &launch, &spawn_pos, &spin, kind, particle, 1.0f, 1.0f);
            if (spawned) {
                qcopy((u8 *)spawned + 0x40, &D_L00_001DEAA0_2D1E68[i]);
                *(f32 *)((u8 *)spawned + 0x48) = func_001FA748_2D1E68(*(f32 *)((u8 *)spawned + 0x48), rotation.f[2]);
                func_L00_00251E30_2D1E68(spawned);
            }
        }
        for (i = 0; i < trail_count; i++) {
            spawn_pos.f[0] = func_002140F8_2D1E68(-0.5f, 0.5f);
            spawn_pos.f[1] = func_002140F8_2D1E68(-0.5f, 0.5f);
            spawn_pos.f[2] = func_002140F8_2D1E68(0.0f, 1.0f);
            func_001F9BD8_2D1E68(&spawn_pos, &spawn_pos, &moby->pos);
            speed = func_002140F8_2D1E68(6.0f, 12.0f) * D_0015EE6C_2D1E68;
            func_L00_00258DB0_2D1E68(speed, speed, &launch);
            if (launch.f[2] < 0.0f) launch.f[2] = -launch.f[2];
            spin.f[0] = func_00214158_2D1E68();
            spin.f[2] = func_00214158_2D1E68();
            particle = 1;
            if (func_002140B0_2D1E68(6) == 0)
                particle = func_001FA898_2D1E68(func_002140F8_2D1E68(3.0f, 6.0f));
            func_L00_002B0B98_2D1E68(moby, &launch, &spawn_pos, &spin, kind + 1, particle, 0.25f, 0.5f);
        }
    }
    self->collision = 0;
    moby->state = 3;
    moby->flags |= 1;
    moby->command = func_001F9850_2D1E68(15);
    if (hit) {
        if (*(s32 *)(D_0013E633_2D1E68 + 0x2EA9) == 22)
            vars[0xCA] = func_001F9850_2D1E68(5);
        else
            vars[0xCA] = 1;
        func_L00_001FF4B0_2D1E68(&launch, &moby->rot_row2, 0.5f);
        func_001F9BD8_2D1E68(&launch, &launch, &moby->pos);
        func_001F9BF0_2D1E68(&spawn_pos, D_L00_00166EC0_2D1E68, &moby->pos);
        func_001F9BC0_2D1E68(&spin);
        distance = func_001F9CB8_2D1E68(&spawn_pos);
        choice = 10;
        if (distance < 8.0f) choice = func_001FA898_2D1E68(distance) + 2;
        height = 0.0f;
        if (distance < 7.0f) height = 7.0f - distance;
        if (close && choice > 6) choice = 6;
        for (i = 0; i < choice; i++) {
            speed = func_002140F8_2D1E68(8.0f, 10.0f);
            __builtin_memcpy(colors, D_L00_001EA200_2D1E68, sizeof(colors));
            __builtin_memcpy(other_colors, D_L00_001EA218_2D1E68, sizeof(other_colors));
            rise = speed * D_0015EE6C_2D1E68 - height * D_0015EE6C_2D1E68;
            effect = colors + func_002140B0_2D1E68(6);
            velocity = other_colors + func_002140B0_2D1E68(6);
            first = func_001F9850_2D1E68(15);
            second = func_001F9850_2D1E68(20);
            first_color = func_L00_00258BC8_2D1E68(first, second);
            first = func_001F9850_2D1E68(25);
            second = func_001F9850_2D1E68(30);
            second_color = func_L00_00258BC8_2D1E68(first, second);
            func_L00_0026B890_2D1E68(&launch, &spin, *effect, *velocity, first_color, second_color, 0, 0, 400000.0f, rise);
        }
        if (D_L00_0015F6B4_2D1E68 < 0.95f && distance > 9.0f && !close) {
            func_L00_002ADBB0_2D1E68(moby, &launch, &spin, 4.0f, func_001F9850_2D1E68(15), 127, 127, 127, 32);
            func_L00_002ADBB0_2D1E68(moby, &launch, &spin, 4.0f, func_001F9850_2D1E68(24), 127, 32, 0, 32);
            func_L00_002ADBB0_2D1E68(moby, &launch, &spin, 4.0f, func_001F9850_2D1E68(20), 127, 64, 0, 48);
        }
        func_L00_002ADBB0_2D1E68(moby, &launch, &spin, 3.5f, func_001F9850_2D1E68(27), 96, 16, 0, 64);
        func_L00_002ADBB0_2D1E68(moby, &launch, &spin, 3.0f, func_001F9850_2D1E68(29), 32, 0, 0, 32);
    }
}
