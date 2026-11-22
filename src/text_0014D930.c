#include "common.h"

/* ER-024: this file's .sdata (docs/build.md 3.2) */
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BE90);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BE94);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BE98);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BE9C);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEA0);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEA4);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEA8);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEAC);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEB0);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEB4);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEB8);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEBC);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEC0);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEC4);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014D930);

void func_0014DA78(void) {
}

extern void func_00130CE0(char *, float);
extern void func_0012D388(s32, f32 fparg0);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014DA80(s8 *arg0, s32 arg1) {
    func_00130CE0(arg0, 40.0f);
    func_0012D388(arg1, 0.1f);
}

void func_0014DAB8(void) {
}

void func_0014DAC0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DAC8);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DC98);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DDD8);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DEA0);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014DEB8(void *arg0, s32 arg1) {
    *(s32 *)((char *)arg0 + 0x3C) = arg1;
    *(s32 *)((char *)arg0 + 0x28) = 0;
    *(s8 *)((char *)arg0 + 9) = 0;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014DEC8(void *arg0, s32 arg1) {
    *(s32 *)((char *)arg0 + 0x38) = arg1;
    *(s32 *)((char *)arg0 + 0x2C) = 0;
    *(s8 *)((char *)arg0 + 8) = 0;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014DED8(void *arg0, s32 arg1) {
    *(s32 *)((char *)arg0 + 0x40) = arg1;
    *(s32 *)((char *)arg0 + 0x30) = 0;
    *(s8 *)((char *)arg0 + 0xA) = 0;
}

extern void func_0014DED8(void *arg0, s32 arg1);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014DEE8(void *arg0, s32 arg1) {
    if (*(s32 *)((char *)arg0 + 0x40) != arg1) {
        func_0014DED8(arg0, arg1);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DF08);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014E0A8);

void func_0014E118(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014E120);

extern void func_00131000(char *, void *);
void func_0014DA78(void);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014E150(s8 *arg0) {
    func_00131000(arg0, func_0014DA78);
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014E160);

void func_0014E248(void) {
}

void func_0014E250(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014E258);

/* ER-014. Types and offsets are Unconfirmed readings. */
void func_0014E2C8(char *a0) {
    char *p = *(char **)(a0 + 0x14);

    if (p != NULL) {
        *(int *)(a0 + 0x14) = 0;
        *(int *)(p + 0x10) = 1;
    }
}

INCLUDE_RODATA("asm/nonmatchings/text_0014D930", D_00354A28);
