/* functional: high. Written from the retail instructions (924 bytes); not a match. The callees' float arguments are passed as retail leaves them in f12. */
/* Finds the point of a closed or open polyline path (shared level code) nearest to a point.
 *
 * path (a0): +0x00 number of points n, +0x10 .. n quadwords (x, y, z, and the length of the segment in w).
 * p (a1): the point to project. out (a2): receives the nearest point on the path.
 * seg (a3): receives the index of the segment start the point lies on; dist (t0): the distance from the
 * path point at that index to the nearest point (with z ignored beyond the range zr).
 * loop (t1): the path wraps. limit (f12): give up (return 0) if the nearest sampled point is farther than
 * limit + step. step (f13): sampling step along the path. zr (f14): z range handed to the distance helpers.
 *
 * Step 1 samples the path every `step` with func_L00_0025E860 and keeps the nearest sample. Step 2 walks
 * one step back and one step forward from it, to the segments that bracket it, and keeps the nearest segment
 * start. Step 3 projects the point (func_L00_0025ED30) onto the segment before and after it and
 * outputs the closer. Returns 1 when found, 0 for an empty path (out is cleared, func_001F9BC0) or too far. */
typedef struct { float v[4]; } Q_EFC0 __attribute__((aligned(16)));
extern float func_L00_0025EAC8_EFC0(Q_EFC0 *, Q_EFC0 *, float) __asm__("func_L00_0025EAC8");
extern int func_L00_0025E860_EFC0(void *, void *, int *, float *, int, float) __asm__("func_L00_0025E860");
extern int func_L00_0025E7F8_EFC0(void *, int, int, int) __asm__("func_L00_0025E7F8");
extern float func_L00_0025ED30_EFC0(void *, void *, void *, void *, float) __asm__("func_L00_0025ED30");
extern void func_001F9BC0_EFC0(void *) __asm__("func_001F9BC0");

int func_L00_0025EFC0(void *path_, void *p_, void *out_, int *seg, float *dist, int loop,
                      float limit, float step, float zr) {
    char *path = (char *)path_;
    Q_EFC0 *p = (Q_EFC0 *)p_;
    Q_EFC0 *out = (Q_EFC0 *)out_;
    Q_EFC0 s0, s1, s2, s3;
    int idx = 0;
    float t = 0.0f;
    int idx2, idx3;
    int best = 0;
    float bestT = 0.0f;
    float bestD;
    float d;
    int ret, n, a, b;
    char *pts = path + 0x10;

    s0 = *(Q_EFC0 *)pts;
    s1 = *p;
    bestD = func_L00_0025EAC8_EFC0(&s0, &s1, zr);
    if (*(int *)path == 0) {
        func_001F9BC0_EFC0(out);
        return 0;
    }
    limit = step + limit;
    do {
        ret = func_L00_0025E860_EFC0(path, &s0, &idx, &t, loop, step);
        s1 = *p;
        d = func_L00_0025EAC8_EFC0(&s0, &s1, zr);
        if (d < bestD) {
            best = idx;
            bestD = d;
            bestT = t;
        }
    } while (ret == 0);
    if (limit < bestD) {
        return 0;
    }
    idx2 = best;
    t = bestT;
    func_L00_0025E860_EFC0(path, &s0, &idx2, &t, loop, -step);
    t = bestT;
    idx3 = best;
    func_L00_0025E860_EFC0(path, &s0, &idx3, &t, loop, step);
    idx3 = func_L00_0025E7F8_EFC0(path, idx3, 1, loop);
    idx = idx2;
    bestD = 1.0e9f;
    for (;;) {
        s1 = *(Q_EFC0 *)(pts + idx * 16);
        s2 = *p;
        d = func_L00_0025EAC8_EFC0(&s1, &s2, zr);
        if (d < bestD) {
            best = idx;
            bestD = d;
        }
        if (idx == idx3) {
            break;
        }
        n = *(int *)path;
        idx = (idx + 1) % n;
    }
    bestD = 1.0e9f;
    n = *(int *)path;
    if (best > 0 || loop != 0) {
        a = (best + (n - 1)) % n;
        bestD = func_L00_0025ED30_EFC0(out, p, pts + a * 16, pts + best * 16, zr);
        *seg = a;
        s1 = *(Q_EFC0 *)(pts + a * 16);
        s2 = *out;
        *dist = func_L00_0025EAC8_EFC0(&s1, &s2, 0.0f);
        n = *(int *)path;
    }
    if (!(best < n - 1) && loop == 0) {
        return 1;
    }
    b = (best + 1) % n;
    d = func_L00_0025ED30_EFC0(&s1, p, pts + best * 16, pts + b * 16, zr);
    if (d < bestD) {
        *out = s1;
        *seg = best;
        s2 = *(Q_EFC0 *)(pts + best * 16);
        s3 = *out;
        *dist = func_L00_0025EAC8_EFC0(&s2, &s3, 0.0f);
    }
    return 1;
}
