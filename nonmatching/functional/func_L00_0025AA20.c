/* SPDX-License-Identifier: GPL-3.0-or-later */
/* Native PAL hit spheres: func_L00_0025A8E8 (0x134 bytes) and func_L00_0025AA20
 * (0xA0 bytes, linked on levels 0, 6, 10, 16 and 18 only), which the decompilation
 * still has as assembly. Every attack that is a sphere goes through the first: the
 * wrench's head, the hero's ground slams, a creature's bite. Not a PS2 match.
 *
 * Adapted from ReRAC (https://github.com/re-rac/rerac),
 * crates/rc-game/src/moby_update/creature/attack.rs (sphere_hit, sphere_template,
 * joint_hit): ISC License, Copyright (c) 2026 ReRAC contributors. ReRAC describes the
 * US release (NTSC-U level 1's 0x26e830 and level 0's 0x2599e8); the calls here are
 * PAL's own, read from the matched C around them: the template's set-up
 * (func_L00_0025A8C0), the sphere against mobys (func_L00_001F2BE8) and the joint's
 * point (func_L00_00250800). */
extern void func_L00_00250800(char *moby, int list, void *out);
extern int func_L00_0025A8E8(char *moby, float radius, void *centre, int flags, float damage, float push,
                             int type, int subtype, int sphere_flags);

/* The same sphere at the last joint of the attacker's joint list `list`, as it is
 * posed now; `n` is both the damage and the template's flags (one register serves
 * both in the retail code). The second float is not read. */
int func_L00_0025AA20(char *moby, float radius, float unused, float push, int list, int n, int type,
                      int subtype, int sphere_flags) {
    float point[4] __attribute__((aligned(16)));

    (void)unused;
    func_L00_00250800(moby, list, point);
    return func_L00_0025A8E8(moby, radius, point, n, (float)n, push, type, subtype, sphere_flags);
}
