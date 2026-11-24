#include "common.h"

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA0F0);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA170);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA370);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA408);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA4F0);

extern s32 D_01B31D80[];
extern void func_001EBAA8(char *, void *, void *);
extern s32 func_001E9E8C(char *);
extern void *func_001E9A5C(void *, void *, s32);

/* ER-189: by hand: format into a 0x400 buffer, then append it to the 0x800-byte text area after D_01B31D80[19] (a
   reading: func_001EBAA8 / func_001E9E8C / func_001E9A5C look like vsprintf / strlen / memcpy, and func_001E9A5C
   returns a value: $v1 after it). Names, types and offsets are Unconfirmed readings. */
void func_001CA630(void *arg0, void *arg1) {
    char buf[0x400];
    s32 n;

    func_001EBAA8(buf, arg0, arg1);
    n = func_001E9E8C(buf);
    if (n + D_01B31D80[19] > 0x800) {
        n = 0x800 - D_01B31D80[19];
    }
    func_001E9A5C((char *)&D_01B31D80[20] + D_01B31D80[19], buf, n);
    D_01B31D80[19] += n;
}

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA6B0);

INCLUDE_ASM("asm/nonmatchings/text_001CA0F0", func_001CA6E8);


/* ER-159: by hand from m2c; names, types and offsets are Unconfirmed readings (D_01B31D80 read as a word array).
   m2c's `*(s32 *)((char *)&D_01B31D80 + 0x30)` form makes a second base at +0x30; indexing keeps one base. */
void func_001CA8B0(s32 *arg0) {
    D_01B31D80[12] = arg0[0];
    D_01B31D80[13] = arg0[1];
    D_01B31D80[14] = arg0[2];
}
