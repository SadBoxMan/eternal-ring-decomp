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

/* ER-179: drafted with m2c (tools/m2c_bulk.py, narrow parameters arg2-arg4 u16, ER-172), rewritten as plain C by hand (`|= arg2` gives the game's `or` operand order); names, types and offsets are Unconfirmed readings. */
void func_001CB300(s32 arg0, s32 arg1, u16 arg2, u16 arg3, u16 arg4) {
    s32 temp_2;
    s32 var_11;
    s8 *var_10;
    s8 *var_3;
    s8 *var_5;
    s8 *var_9;
    char *temp_4;

    var_11 = 0;
    temp_4 = (((arg0 * 4) + arg1) * 0x1C0) + D_01B72600;
    var_10 = temp_4 + 0x124;
    var_9 = temp_4 + 0x127;
    var_5 = temp_4 + 0x126;
    var_3 = temp_4 + 0x125;
    *(u16 *)(temp_4 + 0x164) |= arg2;
    do {
        temp_2 = arg2 >> var_11;
        var_11 += 1;
        if (temp_2 & 1) {
            *var_10 = arg3;
            *var_3 = arg4;
            *var_5 = 0;
            *var_9 = 0;
        }
        var_9 += 4;
        var_5 += 4;
        var_3 += 4;
        var_10 += 4;
    } while (var_11 < 0x10);
}

/* ER-179: drafted with m2c (tools/m2c_bulk.py --plain, narrow parameter arg2 u16, ER-172); names, types and offsets are Unconfirmed readings. */
void func_001CB388(s32 arg0, s32 arg1, u16 arg2) {
    void *temp_4;

    temp_4 = (((arg0 * 4) + arg1) * 0x1C0) + ((char *)D_01B72600);
    *(u16 *)((char *)temp_4 + 0x164) = (u16) (*(u16 *)((char *)temp_4 + 0x164) & ~arg2);
}

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
