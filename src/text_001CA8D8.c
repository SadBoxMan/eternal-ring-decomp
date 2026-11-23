#include "common.h"

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CA8D8);

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CA9A8);

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CAA58);

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CAAC0);

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CAB10);

extern char D_01B72600[];

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
s32 func_001CB218(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(s32 *)(p + 0x168);
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
s32 func_001CB240(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(s32 *)(p + 0x16C);
}

extern char D_01B72700[];

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void *func_001CB268(s32 arg0, s32 arg1) {
    return (((arg0 * 4) + arg1) * 0x1C0) + D_01B72700;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
u16 func_001CB288(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(u16 *)(p + 0x102);
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
u16 func_001CB2B0(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(u16 *)(p + 0x120);
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
u16 func_001CB2D8(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(u16 *)(p + 0x122);
}

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CB300);

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CB388);

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CB3C0);

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CB410);


/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001CB450(s32 arg0, s32 arg1, s8 arg2) {
    void *temp_4;

    temp_4 = (((arg0 * 4) + arg1) * 0x1C0) + D_01B72600;
    *(s8 *)((char *)temp_4 + 0x180) = arg2;
    *(s8 *)((char *)temp_4 + 0x17F) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CB478);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CB4C0(s32 arg0, s32 arg1) {
    void *temp_4;

    temp_4 = (((arg0 * 4) + arg1) * 0x1C0) + D_01B72600;
    if (!(*(u8 *)((char *)temp_4 + 0x17D) & 4)) {
        return 0;
    }
    *(s8 *)((char *)temp_4 + 0x17F) = 2;
    return 1;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
u8 func_001CB500(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(u8 *)(p + 0x17E);
}
