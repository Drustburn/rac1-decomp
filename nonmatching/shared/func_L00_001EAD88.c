/* NON_MATCHING func_L00_001EAD88 -- src/overlays/shared/start_001EAD88.c
 * Best so far: BYTES 2/1044 (99.8% of the bytes match), checked 2026-10-15.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   2026-10-15: func_L00_001EAD88 is one function of 1044 bytes in six catalogue pieces (joined: EADE8 EADEC EB0A0
 *   Needed: tools/jump_tables.py and tools/overlay_check.py read a joined function's pieces for its jump table; an
 *   best.c: BYTES 2/1044. The pointer to the channel is set after the type test. Left: count and sum_count sit in 
 */
#include "common.h"
extern float func_001FA888(int);
extern int func_001FA898(float);
extern float FastCos(float);

typedef struct {
    short type;
    unsigned char slot;
    unsigned char weighted;
    short delay;
    short life;
    short time;
    short rise;
    short fall;
    unsigned char amp;
    unsigned char base;
} Pulse;
extern Pulse D_L00_00166100[8];

int func_L00_001EAD88(int *out) {
    int sum[2];
    int count[2];
    int sum_count[2];
    int i;
    int active = 0;
    for (i = 0; i < 2; i++) {
        out[i] = 0;
        sum[i] = 0;
        sum_count[i] = 0;
        count[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        Pulse *p;
        int v, r;
        if (D_L00_00166100[i].type == 0) continue;
        p = D_L00_00166100 + i;
        active++;
        if (D_L00_00166100[i].life-- <= 0) p->type = 0;
        if (D_L00_00166100[i].delay != 0) {
            D_L00_00166100[i].delay--;
            continue;
        }
        D_L00_00166100[i].time++;
        v = 0;
        r = p->time % (p->rise + p->fall);
        switch (p->type) {
        case 0:
            break;
        case 1:
            if (r < p->rise) v = p->amp + p->base;
            else v = p->base;
            break;
        case 2:
            if (r < p->rise) v = p->amp * r / p->rise;
            else v = p->amp * (p->fall + p->rise - r) / p->fall;
            v += p->base;
            break;
        case 3:
            if (r < p->rise) v = p->amp * r / p->rise + p->base;
            break;
        case 4:
            if (r >= p->rise) v = p->amp * (p->fall + p->rise - r) / p->fall + p->base;
            break;
        case 5: {
            float a, amp;
            if (r < p->fall) {
                a = func_001FA888(r) * 3.1415927f / func_001FA888(p->fall);
            } else {
                a = 3.1415927f - func_001FA888(r - p->fall) * 3.1415927f / func_001FA888(p->rise);
            }
            a = FastCos(a);
            amp = func_001FA888(p->amp);
            v = func_001FA898((a * amp + amp) * 0.5f) + p->base;
            if (v >= 0x100) v = 0xFF;
            break;
        }
        }
        if (p->weighted) {
            sum[p->slot] += v;
            sum_count[p->slot]++;
        } else {
            out[p->slot] += v;
            count[p->slot]++;
        }
    }
    for (i = 0; i < 2; i++) {
        if (count[i] != 0) out[i] /= count[i];
        if (sum_count[i] != 0) {
            sum[i] /= sum_count[i];
            out[i] = out[i] * sum[i] / 0xFF;
        }
    }
    return active;
}
