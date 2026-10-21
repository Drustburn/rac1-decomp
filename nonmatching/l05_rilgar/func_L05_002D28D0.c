/* NON_MATCHING func_L05_002D28D0 -- src/overlays/l05_rilgar/vendor_002D28D0.c
 * Best so far: BYTES 11/1960 (99.4% of the bytes match), checked 2026-10-21.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   From Lombyte's non-matching draft of FUN_L05_002d1688 (sharkigator_update, GPL-3.0), PAL symbols.
 *   Left: at the first func_L05_002D3220 call retail sets $a1 and stores v[2] before the call and puts
 *   $a0 = moby in the delay slot; ours puts the store in the slot. Argument types, qcopy vs qcopy_nc,
 *   re-reads and a permuter run did not change it (2026-10-21, m3).
 */
extern char *D_L05_001B0CB0_shk[] __asm__("D_L05_001B0CB0");
extern float D_L05_00161450 SDATA(D_L05_00161450);
extern float D_L05_001613A0_shk[4] __asm__("D_L05_001613A0") MACRO_ADDR;
extern char D_L05_001672C0_shk[] __asm__("D_L05_001672C0");
extern float D_0015EE6C_shk __asm__("D_0015EE6C") MACRO_ADDR;
extern float D_0015EE70_shk __asm__("D_0015EE70") MACRO_ADDR;
extern char D_0013F450_shk[] __asm__("D_0013F450");
extern int step_path_2d1688(int) __asm__("func_002140B0");
extern int timer_2d1688(int) __asm__("func_001F9850");
extern void mark_moby_2d1688(char *) __asm__("func_0020D678");
extern void set_action_2d1688(char *, int, int, int) __asm__("func_00213DE0");
extern float angle_2d1688(float, float) __asm__("func_L00_001FF860");
extern float steer_2d1688(float *, float *, float, float, float, float) __asm__("func_L00_0025CE58");
extern void vnorm_2d1688(float, void *, void *) __asm__("func_L00_001FF4B0");
extern void vadd_2d1688(void *, void *, void *) __asm__("func_001F9BD8");
extern void vsub_2d1688(void *, void *, void *) __asm__("func_001F9BF0");
extern float distance_2d1688(void *, void *) __asm__("func_001F9D10");
extern int tick_2d1688(void *) __asm__("func_001F9908");
extern float absolute_2d1688(float) __asm__("func_001F9B88");
extern float distance_to_2d1688(void *, void *) __asm__("func_001F9D48");
extern float move_2d1688(float *, float, float *, float, float, float) __asm__("func_00214D88");
extern float angle_from_2d1688(float, float) __asm__("func_001FA748");
extern void copy_direction_2d1688(void *, void *) __asm__("func_001FA1F8");
extern int check_path_2d1688(void *, void *, int) __asm__("func_L00_0025A778");
extern void set_camera_2d1688(void *, void *, int, int) __asm__("func_L00_00217718");
extern int voice_2d1688(int, int, char *) __asm__("func_0022ED80");
extern void sound_2d1688(void) __asm__("func_L00_00211908");
extern void reset_2d1688(void *, void *, int, int, int) __asm__("func_L00_002EBF50");
extern void f_3078_2d1688(char *) __asm__("func_L05_002D3078");
extern void f_3220_2d1688(int, char *) __asm__("func_L05_002D3220");

void func_L05_002D28D0(char *moby) {
    char *data = *(char **)(moby + 0x78);
    int state = *(unsigned char *)(moby + 0x20);
    int one = 1;
    char *path;
    char *point;
    float v[4] __attribute__((aligned(16)));
    float w[4] __attribute__((aligned(16)));
    float z[4] __attribute__((aligned(16)));
    float speed;
    float height;
    int index;
    int duration;

    switch (state) {
    case 0: goto state_zero;
    case 1: goto state_one;
    case 2: goto state_two;
    case 3: goto state_three;
    default: return;
    }
state_zero:
    {
        float height;
        if (*(int *)(data + 0x64) != 0) {
            *(unsigned char *)(moby + 0x20) = 2;
            *(unsigned short *)(moby + 0x34) |= 1;
            *(unsigned char *)(moby + 0x30) = 0xff;
            *(float *)(data + 0x74) = *(float *)(moby + 0x18);
            *(int *)(data + 0x7c) = timer_2d1688(30);
            return;
        }
        index = *(int *)(data + 0x60);
        if (index == -1) {
            mark_moby_2d1688(moby);
            return;
        }
        *(int *)(data + 0x68) = step_path_2d1688(*(int *)D_L05_001B0CB0_shk[index]);
        *(int *)(data + 0x6c) = timer_2d1688(600);
        if (*(unsigned char *)(moby + 0x53) != 1) {
            set_action_2d1688(moby, 1, 0, timer_2d1688(10));
        }
        height = *(float *)(D_L05_001B0CB0_shk[*(int *)(data + 0x60)] + 0x18) - D_L05_00161450;
        *(float *)(moby + 0x18) = height;
        *(float *)(data + 0x80) = height;
        *(unsigned char *)(moby + 0x20) = one;
        return;
    }
state_one:
    {
        path = D_L05_001B0CB0_shk[*(int *)(data + 0x60)];
        qcopy(v, path + *(int *)(data + 0x68) * 16 + 0x10);
        point = moby + 0x48;
        {
            float y = v[1];
            float x = v[0];
            height = angle_2d1688(x - *(float *)(moby + 0x10), y - *(float *)(moby + 0x14));
        }
        {
            float pi = 3.1415927f;
            float turn_step = D_0015EE70_shk * pi;
            steer_2d1688((float *)(point), (float *)(data + 0x70), height, turn_step, turn_step, D_0015EE6C_shk * pi);
        }
        vnorm_2d1688(D_0015EE6C_shk * 3.0f, w, moby + 0xc0);
        vadd_2d1688(moby + 0x10, moby + 0x10, w);
        height = distance_2d1688(moby + 0x10, v);
        if (height < 3.0f || tick_2d1688(data + 0x6c) != 0) {
            index = step_path_2d1688(*(int *)D_L05_001B0CB0_shk[*(int *)(data + 0x60)]);
            *(int *)(data + 0x68) = (*(int *)(data + 0x68) + index) % *(int *)D_L05_001B0CB0_shk[*(int *)(data + 0x60)];
            *(int *)(data + 0x6c) = timer_2d1688(600);
        }
        if ((((struct Hero *)D_0013F450_shk)->state.control_mode == 0x12 || ((struct Hero *)D_0013F450_shk)->state.current == 0x12) &&
            distance_to_2d1688(moby + 0x10, &((struct Hero *)D_0013F450_shk)->motion.pos) < 8.0f) {
            speed = (D_L05_001613A0_shk[2] - D_L05_00161450) - 2.0f;
        } else {
            speed = (D_L05_001613A0_shk[2] - D_L05_00161450) - 0.5f;
            f_3078_2d1688(moby);
        }
        move_2d1688((float *)(moby + 0x18), speed, (float *)(data + 0x78), D_0015EE70_shk + D_0015EE70_shk, D_0015EE70_shk + D_0015EE70_shk, D_0015EE6C_shk + D_0015EE6C_shk);
        return;
    }
state_two:
    {
        char *path;
        tick_2d1688(data + 0x7c);
        height = absolute_2d1688(((struct Hero *)D_0013F450_shk)->water_surface_z - *(float *)(data + 0x74));
        if (height < 0.33f && ((struct Hero *)D_0013F450_shk)->state.control_mode == 0x12) {
            char *point;
            if (*(int *)(data + 0x7c) != 0) return;
            point = D_L05_001672C0_shk - 0x140;
            *(float *)(moby + 0x44) = -1.5533431f;
            *(float *)(moby + 0x48) = angle_from_2d1688(*(float *)(point + 0x158), 1.5707964f);
            copy_direction_2d1688(moby + 0xc0, moby + 0x40);
            qcopy(moby + 0x10, &((struct Hero *)D_0013F450_shk)->motion.pos);
            vnorm_2d1688(D_L05_00161450, v, moby + 0xe0);
            vsub_2d1688(moby + 0x10, moby + 0x10, v);
            {
                float speed = D_0015EE6C_shk * 10.0f;
                *(float *)(moby + 0x18) -= 2.0f;
                *(float *)(data + 0x78) = speed;
            }
            *(unsigned short *)(moby + 0x34) &= 0xfffe;
            if (*(unsigned char *)(moby + 0x53) != state)
                set_action_2d1688(moby, 2, 0, timer_2d1688(5));
            *(unsigned char *)(moby + 0xbc) = 0;
            *(unsigned char *)(moby + 0x20) = 3;
            reset_2d1688(point + 0x140, point + 0x150, 0, 0, 0);
            return;
        }
        if (((struct Hero *)D_0013F450_shk)->state.current == 0x12) return;
        path = D_L05_001B0CB0_shk[*(int *)(data + 0x60)];
        if (check_path_2d1688(&((struct Hero *)D_0013F450_shk)->motion.pos, path + 0x10, *(int *)path) != 0)
            *(int *)(data + 0x7c) = timer_2d1688(600);
        else
            *(int *)(data + 0x7c) = timer_2d1688(180);
        return;
    }
state_three:
    {
        float speed;
        {
            struct Hero *h = (struct Hero *)D_0013F450_shk;
            if (h->water_surface_z < *(float *)(moby + 0x18) || *(float *)(data + 0x78) < 0.0f) {
                if (h->state.current != 0x72) {
                    qcopy_nc(v, &h->motion.pos);
                    v[2] = h->water_surface_z;
                    f_3220_2d1688((int)moby, (char *)v);
                    set_camera_2d1688(&h->motion.pos, &h->motion.rot, 0x72, 0);
                } else {
                    qcopy(&h->motion.pos, moby + 0x10);
                }
                h->unk20A5 = one;
                h->motion.pos.f[2] += 20.0f;
                speed = *(float *)(data + 0x78);
            } else {
                speed = *(float *)(data + 0x78);
            }
        }
        {
            float acceleration = D_0015EE70_shk * 10.0f;
            float threshold = D_0015EE6C_shk + D_0015EE6C_shk;
            *(float *)(moby + 0x18) = *(float *)(moby + 0x18) + speed;
            {
                float speed = *(float *)(data + 0x78) - acceleration;
                *(float *)(data + 0x78) = speed;
                if (speed < threshold) {
                    vnorm_2d1688(D_L05_00161450, v, moby + 0xe0);
                    steer_2d1688((float *)(moby + 0x44), (float *)(data + 0x70), 1.5533431f, D_0015EE70_shk * 12.566371f, D_0015EE70_shk * 12.566371f, D_0015EE6C_shk * 25.132742f);
                    copy_direction_2d1688(moby + 0xc0, moby + 0x40);
                    vnorm_2d1688(D_L05_00161450, w, moby + 0xe0);
                    vsub_2d1688(z, w, v);
                    vsub_2d1688(moby + 0x10, moby + 0x10, z);
                }
            }
        }
        if (*(float *)(data + 0x78) < 0.0f) {
            if (*(float *)(moby + 0x18) < ((struct Hero *)D_0013F450_shk)->water_surface_z && *(unsigned char *)(moby + 0xbc) == 0) {
                *(unsigned char *)(moby + 0xbc) = 1;
                voice_2d1688(0, 0, moby);
                vnorm_2d1688(D_L05_00161450, v, moby + 0xe0);
                vadd_2d1688(v, v, moby + 0x10);
                v[2] = ((struct Hero *)D_0013F450_shk)->water_surface_z;
                f_3220_2d1688((int)moby, (char *)v);
            }
        }
        if (*(float *)(data + 0x78) < 0.0f &&
            *(float *)(moby + 0x18) < *(float *)(data + 0x74) - 8.0f) sound_2d1688();
        if ((*(unsigned char *)(moby + 0x70) & 2) != 0 && *(unsigned char *)(moby + 0x53) != 1)
            set_action_2d1688(moby, 1, 0, timer_2d1688(10));
    }
}