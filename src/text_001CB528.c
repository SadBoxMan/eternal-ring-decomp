#include "common.h"

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CB528);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CB750);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CB808);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CB918);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CBA08);

extern void *D_0035C554;
extern s32 func_001CB808();

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001CBB48(void) {
    if (*(u16 *)((char *)D_0035C554 + 0x4D6) != 1) {
        func_001CB808(1, 0x40);
        *(u16 *)((char *)D_0035C554 + 0x4D6) = 1U;
    }
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001CBB88(void) {
    if (*(u16 *)((char *)D_0035C554 + 0x4D6) != 0) {
        func_001CB808(1, 0x50);
        *(u16 *)((char *)D_0035C554 + 0x4D6) = 0U;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CBBC0);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CBD00);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CBE48);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CBE90);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CC0C8);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CC348);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CC6A0);

/* ER-159: by hand from m2c; names, types and offsets are Unconfirmed readings. The game stores arg1's low byte raw and
   sign-extends only the call's copy, so arg1 is a full word here (an `s8` parameter is extended on entry). */
s32 func_001CC848(s32 arg0, s32 arg1) {
    char *p = (char *)D_0035C554 + (arg0 << 5);
    *(s8 *)(p + 0x1A) = arg1;
    if (func_001CB808(1, 0x90, (s8)arg1, 0, *(s32 *)(p + 8), *(s32 *)(p + 0xC), *(s32 *)(p + 0x14)) != *(s32 *)(p + 0x14)) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CC8B0);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CC988(s32 arg0) {
    return func_001CB808(1, 0x80, arg0, 0);
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CC9B0);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CCAB0);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CCB08);


/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CCD98(u32 arg0, s32 arg1) {
    if (arg0 >= 0x10U) {
        return -1;
    }
    *(s32 *)(((char *)D_0035C554 + (arg0 << 5)) + 0xC) = arg1;
    return 0;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001CCDC0(s32 arg0, s32 arg1) {
    func_001CB808(1, 0x1040, *(s16 *)(((char *)D_0035C554 + (arg0 << 5)) + 0x18), arg1 & 0xFF);
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001CCDE0(s32 arg0, s32 arg1) {
    func_001CB808(1, 0x1050, *(s16 *)(((char *)D_0035C554 + (arg0 << 5)) + 0x18), arg1 & 0xFF);
}

/* ER-189: by hand, the early exits first (recipes 2.43: the game's bnel); names, types and offsets are Unconfirmed readings. */
s32 func_001CCE00(u32 arg0, s16 arg1) {
    char *p;

    if (arg0 >= 0x11U) {
        return -1;
    }
    p = (char *)D_0035C554 + (arg0 << 5);
    if (*(s32 *)p == 0) {
        return -1;
    }
    func_001CB808(1, 0x1060, *(s16 *)(p + 0x18), arg1);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CCE58);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CCF60);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD068);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD1D0);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD360);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD468);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD4C8);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CD530(u32 arg0) {
    if (arg0 >= 5U) {
        return -1;
    }
    return func_001CB808(1, 0x2010, *(s32 *)(((arg0 * 0x10) + (char *)D_0035C554) + 0x20C));
}

/* ER-189: by hand, the early exits first (recipes 2.43: the game's bnel); names, types and offsets are Unconfirmed readings. */
s32 func_001CD570(u32 arg0) {
    char *p;

    if (arg0 >= 5U) {
        return -1;
    }
    p = (char *)D_0035C554 + ((arg0 << 4) + 0x200);
    if (*(s32 *)p == 0) {
        return -1;
    }
    return (func_001CB808(1, 0x2040, *(s32 *)(p + 0xC)) == 0) ? 0 : -1;
}

/* ER-189: by hand, the early exits first (recipes 2.43: the game's bnel); names, types and offsets are Unconfirmed readings. */
s32 func_001CD5C8(u32 arg0) {
    char *p;

    if (arg0 >= 5U) {
        return -1;
    }
    p = (char *)D_0035C554 + ((arg0 << 4) + 0x200);
    if (*(s32 *)p == 0) {
        return -1;
    }
    return (func_001CB808(1, 0x2050, *(s32 *)(p + 0xC)) == 0) ? 0 : -1;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD620);

/* ER-159: by hand from m2c; names, types and offsets are Unconfirmed readings. `arg0 << 4` gives the game's
   `addu $v0, base, index`; m2c's `arg0 * 0x10` in one expression sums the other way round. */
s32 func_001CD680(s32 arg0, s16 arg1) {
    return func_001CB808(1, 0x1060, *(s32 *)((char *)D_0035C554 + (arg0 << 4) + 0x20C), arg1);
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD6B8);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD8D8);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD9C8);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CDB60);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CDC08);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CDCE0);

extern s32 func_001E9D78(s32, s32);
extern s32 D_01B7B900[];
extern s32 func_001D23A8(s32 *, s32, s32, s32 *, s32, s32 *, s32, s32, s32);
extern s32 D_01B73E50[];
extern s32 D_01B73840[];

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CDDA0(s32 arg0, s32 arg1, s32 arg2) {
    if ((func_001CB808(1, 0x3040, arg0, arg2) != 0) || (func_001E9D78((s32) D_01B7B900, arg1), func_001D23A8(D_01B73E50, arg0 | 0x30A0, 0, D_01B7B900, 0x100, D_01B73840, 1, 0, 0), (D_01B73840[0] != 0))) {
        func_001CB808(1, 0x3030, arg0);
        return -1;
    }
    return arg0;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CDE58(s32 arg0, s32 arg1, s32 arg2) {
    return (func_001CB808(1, 0x30C0, arg0, arg1, arg2) == 0) ? arg0 : -1;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CDEA0);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CDF78);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CDFD0);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CE038);

extern s32 func_001D33F0(s32, s32);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CE0D8(s32 arg0, s32 arg1) {
    if (*(s32 *)((char *)D_0035C554 + 0x4B8) == 0) {
        *(s32 *)((char *)D_0035C554 + 0x4B8) = arg1;
    }
    do {

    } while (func_001D33F0(arg0, arg1) < 0);
    return 0;
}
