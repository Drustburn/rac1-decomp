#ifndef VECTOR_H
#define VECTOR_H

/*
 * The 16-byte vector the game moves with lq/sq: one quadword (q), four
 * floats or four words. One definition for every source file, in place of
 * the private copies candidates carried to land without clashing with
 * their files. The 4-byte aligned float vector Vec4f is in moby.h.
 *
 * Adapted from Lombyte (MIT), the US decompilation:
 * include/rnc/math/vector.h at 42adb529. See THIRD_PARTY_NOTICES.md.
 */

#include "common.h"

/* The quadword. Not called u128: source files declare their own u128,
   and a second typedef of one name is an error. */
typedef u32 VecQuad __attribute__((mode(TI), aligned(16)));

typedef union {
    VecQuad q;
    f32 f[4];
    s32 i[4];
} Vec4;

#endif /* VECTOR_H */
