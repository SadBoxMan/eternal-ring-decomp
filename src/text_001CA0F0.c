#include "common.h"

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA0F0);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA170);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA370);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA408);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA4F0);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA630);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA6B0);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA6E8);

extern s32 D_01B31D80[];

/* ER-159: by hand from m2c; names, types and offsets are Unconfirmed readings (D_01B31D80 read as a word array).
   m2c's `*(s32 *)((char *)&D_01B31D80 + 0x30)` form makes a second base at +0x30; indexing keeps one base. */
void func_001CA8B0(s32 *arg0) {
    D_01B31D80[12] = arg0[0];
    D_01B31D80[13] = arg0[1];
    D_01B31D80[14] = arg0[2];
}
