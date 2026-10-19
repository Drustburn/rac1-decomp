/* NON_MATCHING func_L00_00261B00 -- src/overlays/shared/mobyutil_00261B00.c
 * Best so far: SIZE ours 2240 / retail 2252, checked 2026-10-19.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   CAUTION (lead): p30 holds the moby[0xB0] byte in an otherwise dead int to drop one moby reference so `large`
 *   wins $fp; that is scratch-variable reuse and must not land as is. Find a legitimate way to lower moby's priority.
 *   the beqz delay slot as retail.
 *   - float crossings[2] declared after path (not first): spill slots land at 0x78.. as retail.
 *   - Nested 001FA888 call inside the printf argument: string address held in $s1 across both calls as retail.
 *   - Decrement of the pile counter after the call in each arm (`call(...); large--;`): cross-jump then merges the
 *   the last arm as retail (p29, 2260 -> 2240). `int large = 0, small = 0, medium = 0` swaps s6/s7 to retail's.
 *   - Remaining (p30): retail b_large is $s4 and single $s3 (ours reversed; b_large pri .245 beats single .175, so
 *   more references or b_large fewer); retail's extra copies (`daddu s4,s1` for &position, mpos in $s5, B0 index c
 *   count*5 as sll/addu; `3 < count` made no difference (slt vs slti still from hoisting of a constant); one extra
 */
extern int D_L00_0015F678_261B00 __asm__("D_L00_0015F678") MACRO_ADDR;
extern unsigned char D_L00_0015FD48_261B00[] __asm__("D_L00_0015FD48");
extern int D_0015EFA0_261B00 __asm__("D_0015EFA0") MACRO_ADDR;
extern int D_0015EF20_261B00 __asm__("D_0015EF20") MACRO_ADDR;
extern int D_0015EF28_261B00 __asm__("D_0015EF28") MACRO_ADDR;
extern int D_0015EF2C_261B00 __asm__("D_0015EF2C") MACRO_ADDR;
extern float D_0015EE6C_261B00 __asm__("D_0015EE6C") MACRO_ADDR;
extern float D_0015EE70_261B00 __asm__("D_0015EE70") MACRO_ADDR;
extern char *D_L00_001B0830_261B00[] __asm__("D_L00_001B0830");
extern char D_L00_00160160[];
extern char D_L00_00160170[];
extern char D_L00_00160180[];
extern void func_001F9BC0_261B00(void *) __asm__("func_001F9BC0");
extern char *func_L00_0025D390_261B00(char *) __asm__("func_L00_0025D390");
extern int func_002140B0_261B00(int) __asm__("func_002140B0");
extern int func_001F9850_261B00(int) __asm__("func_001F9850");
extern void func_001E9730_261B00(char *, ...) __asm__("func_001E9730");
extern float func_001FA888_261B00(int) __asm__("func_001FA888");
extern long func_00120778_261B00(float) __asm__("func_00120778");
extern float func_00214158_261B00(void) __asm__("func_00214158");
extern float func_001F9F90_261B00(float) __asm__("func_001F9F90");
extern float func_001F9FA8_261B00(float) __asm__("func_001F9FA8");
extern float func_002140F8_261B00(float, float) __asm__("func_002140F8");
extern void func_L00_001FF4B0_261B00(void *, void *, float) __asm__("func_L00_001FF4B0");
extern void func_001F9BD8_261B00(void *, void *, void *) __asm__("func_001F9BD8");
extern void func_001F9BF0_261B00(void *, void *, void *) __asm__("func_001F9BF0");
extern void func_001F9C30_261B00(void *, void *, float) __asm__("func_001F9C30");
extern int func_L00_0025A5D8_261B00(float, float, float, float *, float *) __asm__("func_L00_0025A5D8");
extern int func_L00_00262BC0_261B00(int, void *, void *, void *) __asm__("func_L00_00262BC0");
extern void func_L00_002A86D8_261B00(void *, void *, void *, int, int, int) __asm__("func_L00_002A86D8");

/* Drops bolts from MOBY: picks a count between LOWER and UPPER (doubled in the bonus modes, scaled by the level's
   bolt rate when the moby is a crate the player may be farming), splits it into 50/20/5/1 bolt mobys (at most three
   piles when FLAGS has bit 3), and throws each pile from the moby with a random velocity, aimed at path PATH_INDEX
   when one is given, through func_L00_002A86D8. */
void func_L00_00261B00(char *moby, int lower, int upper, int flags, int path_index) {
    Vec4 base, position, velocity, offset, adjusted, difference, quarter;
    float time;
    float scale;
    int large = 0, small = 0, medium = 0, single;
    int count, multiplier;
    int remaining;
    int near_large, near_medium, near_small, near_single;
    int far_large, far_medium, far_small, far_single;
    int near_value, far_value, gated;
    char *path;
    float crossings[2];

    func_001F9BC0_261B00(&base);
    if (!(flags & 0x20) && (remaining = (unsigned char)moby[0xB0]) != 0xFF &&
        (unsigned int)((unsigned short)*(unsigned short *)(moby + 0xA6) - 500) > 40) {
        D_L00_0015F678_261B00 = D_L00_0015FD48_261B00[remaining] == 0xFF;
    }
    gated = 0;
    if (D_L00_0015F678_261B00) {
        int bit = flags & 0x20;
        gated = bit == 0;
    }
    if (!flags || (!lower && !upper))
        return;
    if (flags & 4) {
        path = func_L00_0025D390_261B00(moby);
        if (path)
            qcopy(&base, path + 0x20);
    }
    if (lower < 1)
        lower = 1;
    count = func_002140B0_261B00(upper - lower + 1) + lower;
    if (D_0015EFA0_261B00 || D_0015EF20_261B00)
        count *= 2;
    if (count < 500 && gated && D_0015EF28_261B00) {
        multiplier = (int)((float)func_001F9850_261B00(60) * 60.0f) * D_0015EF2C_261B00 / D_0015EF28_261B00;
        if (D_0015EFA0_261B00 || D_0015EF20_261B00)
            multiplier /= 2;
        if (multiplier < 150) {
            if (multiplier < 91) {
                func_001E9730_261B00(D_L00_00160160);
                count *= 5;
            } else {
                lower = 190;
                lower -= multiplier;
                func_001E9730_261B00(D_L00_00160170, lower / 20);
                count *= lower / 20;
            }
            if (D_0015EFA0_261B00) {
                if (count > 1000) count = 1000;
            } else if (count > 500) count = 500;
        } else if (multiplier >= 301) {
            if (multiplier >= 352) {
                func_001E9730_261B00(D_L00_00160180, (long)0x3FB99999A0000000);
                count /= 10;
            } else {
                int span = 360 - multiplier;
                func_001E9730_261B00(D_L00_00160180, func_00120778_261B00(func_001FA888_261B00(span) / 20.0f));
                count *= span / 80;
            }
            if (count < 1) count = 1;
        }
    }
    remaining = count;
    if (count >= 300) {
        do {
            remaining -= 50;
            large++;
        } while (remaining >= 300);
    }
    for (; remaining >= 200; remaining -= 20) medium++;
    for (; remaining >= 30; remaining -= 25) { medium++; small++; }
    for (; remaining >= 7; remaining -= 5) small++;
    single = remaining;
    if (large + medium + small + single > 3 && (flags & 8)) {
        int a_single = 0, a_small = 0, a_medium = 0, a_large = 3;
        int b_large = 0, b_medium = 0, b_small = 0, b_single = 3;
        near_value = 150;
        far_value = 3;
        if (single + small + medium + large > 3) {
            if (count < 150) {
                int value_large = 150, value_medium = 0, value_small = 0, value_single = 0;
                do {
                    if (a_large) {
                        value_large -= 50; a_large--; value_medium += 20; a_medium++;
                    } else if (a_medium) {
                        value_medium -= 20; a_medium--; value_small += 5; a_small++;
                    } else if (a_small) {
                        value_small -= 5; a_small--; a_single++;
                    }
                    near_value = value_large + value_medium + value_small + a_single;
                } while (count < near_value && near_value > 3);
            }
            if (3 < count) {
                int value_large = b_large * 50, value_medium = b_medium * 20, value_small = b_small * 5;
                do {
                    if (b_single) {
                        b_single--; value_small += 5; b_small++;
                    } else if (b_small) {
                        value_small -= 5; b_small--; value_medium += 20; b_medium++;
                    } else if (b_medium) {
                        value_medium -= 20; b_medium--; value_large += 50; b_large++;
                    }
                    far_value = value_large + value_medium + value_small + b_single;
                } while (far_value < count && far_value < 150);
            }
        }
        if (near_value - count < count - far_value) {
            single = a_single; small = a_small; medium = a_medium; large = a_large;
        } else {
            single = b_single; small = b_small; medium = b_medium; large = b_large;
        }
    }
    goto check_units;
    do {
        char *mpos = moby + 0x10;
        qcopy(&position, mpos);
        position.f[2] += 0.5f;
        time = func_00214158_261B00();
        scale = func_001F9F90_261B00(time);
        velocity.f[0] = scale * (func_002140F8_261B00(0.0f, 3.0f) * D_0015EE6C_261B00);
        scale = func_001F9FA8_261B00(time);
        velocity.f[1] = scale * (func_002140F8_261B00(0.0f, 3.0f) * D_0015EE6C_261B00);
        velocity.f[2] = 0.0f;
        velocity.f[2] = func_002140F8_261B00(3.7f, 6.0f) * D_0015EE6C_261B00;
        offset.f[0] = velocity.f[0];
        offset.f[1] = velocity.f[1];
        offset.f[2] = 0.0f;
        func_L00_001FF4B0_261B00(&offset, &offset, 0.5f);
        func_001F9BD8_261B00(&position, &position, &offset);
        if (path_index >= 0) {
            float t2;
            float half = D_0015EE70_261B00 * 10.8f * -0.5f;
            t2 = 120.0f;
            if ((float)func_L00_0025A5D8_261B00(half, velocity.f[2] - half,
                position.f[2] - *(float *)(D_L00_001B0830_261B00[path_index] + 0x18),
                &crossings[0], &crossings[1]) >= 1.0f && crossings[0] > 0.0f && crossings[0] < 120.0f)
                t2 = crossings[0];
            func_001F9C30_261B00(&adjusted, &velocity, t2);
            func_001F9BD8_261B00(&adjusted, &adjusted, mpos);
            func_L00_00262BC0_261B00(path_index, mpos, &adjusted, &difference);
            func_001F9BF0_261B00(&adjusted, &difference, &position);
            adjusted.f[2] = 0.0f;
            func_L00_001FF4B0_261B00(&quarter, &adjusted, 0.25f);
            func_001F9BF0_261B00(&adjusted, &adjusted, &quarter);
            func_001F9C30_261B00(&adjusted, &adjusted, 1.0f / t2);
            velocity.f[0] = adjusted.f[0];
            velocity.f[1] = adjusted.f[1];
        }
        func_001F9BD8_261B00(&velocity, &velocity, &base);
        if (large) {
            func_L00_002A86D8_261B00(moby, &position, &velocity, flags, 50, gated);
            large--;
        } else if (medium) {
            func_L00_002A86D8_261B00(moby, &position, &velocity, flags, 20, gated);
            medium--;
        } else if (small) {
            func_L00_002A86D8_261B00(moby, &position, &velocity, flags, 5, gated);
            small--;
        } else {
            func_L00_002A86D8_261B00(moby, &position, &velocity, flags, 1, gated);
            single--;
        }
check_units:;
    } while (single || small || medium || large);
}
