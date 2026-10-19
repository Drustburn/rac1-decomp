/* fcheck: unchecked (never called in fcheck's capture run of the title, menu and level 0; OpenRAC tools/fcheck_gate.py, 2026-10-18). */
/* Bitstream peek: the top n bits of the 64-bit accumulator at +0x0, as an
   int. The shift count is taken mod 64 as the MIPS dsrlv does. */
int func_0012AAA8(void *arg0, int n) {
    unsigned long acc = *(unsigned long *)arg0;
    return (int)(acc >> ((64 - n) & 63));
}
