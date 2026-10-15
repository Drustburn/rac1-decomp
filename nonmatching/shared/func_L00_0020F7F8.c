/* NON_MATCHING func_L00_0020F7F8 -- src/overlays/shared/help_0020CDF0.c
 * Best so far: BYTES 558/1056 (47.2% of the bytes match), checked 2026-10-15.
 * Not built into anything: the retail assembly stays in the source file
 * until a candidate is EXACT (docs/NONMATCHING.md). Start from this one.
 * What the last attempts found:
 *   2026-10-15: the ring of 16 hero entries (0x30 bytes each at hero + 0x12F0; index 0x21B8, count 0x21BC,
 *   length 0x21C0) through a local struct, the hand-built sign fix replaced by `% 16`, the target stored
 *   straight into D_L00_0015F774 in each arm (retail merges the stores and reads the global back) and the
 *   length stepped on the field. Size 1056 = retail, BYTES 558/1056. Retail writes the index as ring_index -
 *   (i - 15); that form gives 1048. Left: retail's frame is 0x20 larger (fa sits at sp+0x40, one more 16-byte
 *   local before it, and the -1 of the sign fix is spilled with sq to sp+0x90), and its loops over n test with
 *   blez and count down.
 */
typedef struct {
    char pad0[0x12F0];
    float ring[16][12];
    char pad15F0[0x2084 - 0x15F0];
    int state;
    int step;
    int control_mode;
    char pad2090[0x21B8 - 0x2090];
    int ring_index;
    int ring_count;
    int ring_len;
} HeroRing_20F7F8;
extern HeroRing_20F7F8 D_0013F450_20F7F8 __asm__("D_0013F450");
extern short D_L00_0015F774;
extern void func_001FA480();
extern void func_00214F78(float *);
extern void func_001F9BC0(void *);
extern float func_L00_0020F750(float *a, float *b);
extern void func_001F9BD8(void *, void *, void *);
extern void func_001F9C30(void *, void *, float);
extern void func_002153E8(void *, void *);

typedef struct { float x, y, z, w; } __attribute__((aligned(16))) V4_F7F8;

/* Per-frame bookkeeping for the 16-slot ring at D_0013F450: advances the slot index, scores pairs of slots and writes the normalized weights to arg+0x40. */
void func_L00_0020F7F8(char *arg) {
    V4_F7F8 vv[3];
    float fa[16];
    float *fp;
    float *p;
    float *out;
    V4_F7F8 *sp4;
    V4_F7F8 *vq;
    int cnt, n, i, j, k, m, t, c, ra, rb, acc;
    float mx, thr;

    func_001FA480(D_0013F450_20F7F8.ring[D_0013F450_20F7F8.ring_index]);
    func_00214F78(D_0013F450_20F7F8.ring[D_0013F450_20F7F8.ring_index]);

    t = D_0013F450_20F7F8.ring_index + 1;
    c = D_0013F450_20F7F8.ring_count + 1;
    D_0013F450_20F7F8.ring_count = c;
    D_0013F450_20F7F8.ring_index = t % 16;
    if (c > 16) {
        D_0013F450_20F7F8.ring_count = 16;
    }

    {
        V4_F7F8 *w = vv;
        for (k = 2; k >= 0; k--) {
            func_001F9BC0(w);
            w++;
        }
    }

    if (D_0013F450_20F7F8.control_mode == 0x16) {
        *(int *)&D_L00_0015F774 = 0xE;
    } else if (D_0013F450_20F7F8.state == 2) {
        *(int *)&D_L00_0015F774 = 5;
    } else {
        *(int *)&D_L00_0015F774 = 0xB;
    }

    if (D_0013F450_20F7F8.ring_len < *(int *)&D_L00_0015F774) {
        D_0013F450_20F7F8.ring_len++;
    } else if (*(int *)&D_L00_0015F774 < D_0013F450_20F7F8.ring_len) {
        D_0013F450_20F7F8.ring_len--;
    }

    n = D_0013F450_20F7F8.ring_len;
    if (!(1 < n)) {
        n = 2;
    }
    if (D_0013F450_20F7F8.ring_count < n) {
        return;
    }

    out = (float *)(arg + 0x40);
    cnt = 0;
    fp = fa;
    if (n > 0) for (i = 0; i < n; i++) {
        ra = (D_0013F450_20F7F8.ring_index - i + 15) % 16;
        for (j = 0; j < n; j++) {
            if (i != j) {
                rb = (D_0013F450_20F7F8.ring_index - j + 15) % 16;
                fp[i] += func_L00_0020F750(D_0013F450_20F7F8.ring[ra],
                                           D_0013F450_20F7F8.ring[rb]);
            }
        }
    }

    mx = 0.0f;
    p = fa;
    for (k = n; k != 0; k--) {
        if (mx < *p) {
            mx = *p;
        }
        p++;
    }

    thr = mx * 0.2f;
    p = fa;
    for (k = n; k > 0; k--) {
        if (thr < *p) {
            cnt++;
        }
        p++;
    }

    acc = 0;
    thr = mx * 0.2f;
    for (k = 0; n > 0 && k < n; k++) {
        if (thr < fp[k] && cnt > 0 && cnt < 3) {
            continue;
        }
        acc++;
        ra = (D_0013F450_20F7F8.ring_index - k + 15) % 16;
        vq = (V4_F7F8 *)D_0013F450_20F7F8.ring[ra];
        sp4 = vv;
        for (m = 2; m >= 0; m--) {
            func_001F9BD8(sp4, sp4, vq);
            sp4++;
            vq++;
        }
    }

    {
        V4_F7F8 *q = vv;
        for (m = 2; m >= 0; m--) {
            func_001F9C30(q, q, 1.0f / (float)acc);
            q++;
        }
    }
    func_002153E8(vv, out);
}
