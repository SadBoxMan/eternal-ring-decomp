#include "common.h"

/* ER-024: this file's .sdata (docs/build.md 3.2) */
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAC8);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BACC);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAD0);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAD4);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAD8);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BADC);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAE0);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAE4);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAF0);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAF4);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAF8);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAFC);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BB00);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BB04);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013C510);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013C658);

extern void func_0013D480(void *arg0, s8 arg1);
extern s32 func_00131140(char *, void *, void *, void *, void *, void *);
extern s32 func_0012B930(char *, s32, s32, s32, s16, s32, f32);
void func_0013C750(void **arg0, char *arg1);
extern void func_0013C9A8(s8 *);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), fixed by hand; names, types and offsets are Unconfirmed readings. */
void func_0013C6D8(s8 *arg0) {
    func_0013D480(arg0, 2);
    if (func_00131140(arg0, func_0013C750, func_0013C750, 0, 0, func_0013C9A8) == 0) {
        func_0012B930(arg0, 0, 1, *(s16 *)((char *)arg0 + 0x6C), 0x5A, 1, 0.1f);
    }
}

extern void func_00131000(char *, void *);
extern void func_0013C6D8(s8 *);
extern s32 func_0012BA30(void *, s32, s32);
extern s32 func_0012F9C0(void *, s32);
extern s32 func_0012B890(void *, s32, s32, s32, s16, f32, s32);
extern int func_00130780(char *);
extern s32 func_0012FC38(void **, s32, s32, s32);

/* ER-072 / ER-078: jump table (5 entries) by hand from the m2c draft; the explicit `return;` at the end gives the
   game's sibling `j func_0012F9C0` (recipes 2.22). A state handler (a reading); state 2 switches on
   func_00130780's result. Names, types and offsets Unconfirmed. */
void func_0013C750(void **arg0, char *arg1) {
    char *p = (char *)arg0;

    switch (*(s16 *)(p + 0x6E)) {
    case 0:
        func_0012F9C0(arg0, 3);
        *(s16 *)(p + 0x6E) += 1;
    case 1:
        if (func_0012B890(arg0, 0, 0xD, *(s16 *)(p + 0x6C), 0x5A, 0.1f, 0) != 0) {
            *(u8 *)(arg1 + 0x147) &= 0xFE;
            *(s32 *)(p + 0x10) = 0;
            *(s32 *)(p + 0x14) = 0;
            *(s16 *)(p + 0x6E) += 1;
        }
        func_0013D480(arg0, 2);
        return;
    case 2:
        *(s32 *)(p + 0x10) += 1;
        switch (func_00130780(p)) {
        case 0:
        case 1:
        case 2:
            *(s16 *)(p + 0x6E) += 1;
            break;
        case 3:
            if (*(s32 *)(p + 0x10) >= 0x79) {
                func_00131000(p, func_0013C6D8);
                return;
            }
            break;
        case 4:
            if (++*(s32 *)(p + 0x14) >= 2) {
                func_00131000(p, func_0013C9A8);
                return;
            }
            break;
        }
        break;
    case 3:
        *(u8 *)(arg1 + 0x147) |= 1;
        *(s8 *)(p + 8) = 0;
        func_0013D480(arg0, 1);
        *(s16 *)(p + 0x74) = 0;
        *(s16 *)(p + 0x6E) += 1;
    case 4:
        if (func_0012BA30(arg1, 0, 0xD) != 0) {
            if (++*(s16 *)(p + 0x74) >= 0x1F) {
                func_00131000(p, func_0013C6D8);
                return;
            }
        } else {
            if (*(s8 *)(p + 8) == 0 && func_0012FC38(arg0, 0, 1, 0x11) != 0) {
                *(s8 *)(p + 8) = 1;
            }
            if (*(u16 *)(*(char **)(arg1 + 0xE0) + 0x26C) == 0x14) {
                func_0012F9C0(arg0, 4);
                return;
            }
        }
        break;
    }
    return;
}

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013C9A8);

void func_0013CAE8(s8 *arg0);

/* ER-078: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013CA60(void *arg0) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        func_0012F9C0(arg0, 3);
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    if (func_0012B890(arg0, 0, 0xE, *(s16 *)((char *)arg0 + 0x6C), 0x14, 0.15f, 0) != 0) {
        *(s8 *)((char *)arg0 + 0xC) = 1;
        func_00131000(arg0, &func_0013CAE8);
    }
    func_0013D480(arg0, 2);
}


/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013CAE8(s8 *arg0) {
    s32 temp_2;

    func_0013D480(arg0, 2);
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s32 *)((char *)arg0 + 0x10) = 0;
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    temp_2 = *(s32 *)((char *)arg0 + 0x10) + 1;
    *(s32 *)((char *)arg0 + 0x10) = temp_2;
    if (temp_2 >= 0xB) {
        *(s8 *)((char *)arg0 + 0xC) = 0;
        func_00131000(arg0, func_0013C6D8);
    }
}

extern s32 func_0013CF78(void *, s32);
extern void func_0013D068(void *arg0, s32 arg1);
extern s32 func_0013D088(void *, s32);
extern s32 func_0013D378(void *, s32);

extern void func_0012B218(void *, int);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain), declarations aligned with the file by hand; names, types and
   offsets are Unconfirmed readings. */
void func_0013CB60(void *arg0, s32 arg1) {
    s8 temp_4;

    temp_4 = *(s8 *)((char *)arg0 + 0xD);
    *(s32 *)((char *)arg0 + 0x2C) = (s32) (*(s32 *)((char *)arg0 + 0x2C) + 1);
    switch (temp_4) {
    case 0:
        func_0013CF78(arg0, arg1);
        break;
    case 1:
        func_0013D068(arg0, arg1);
        break;
    case 2:
        func_0013D088(arg0, arg1);
        break;
    }
    func_0013D378(arg0, arg1);
    func_0012B218(arg0, 1);
    func_0012B218(arg0, 2);
}

extern s32 func_00131088(void *);
extern void func_0013C658(s8 *);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0013CC20(s8 *arg0) {
    if (func_00131088(arg0) == (s32) func_0013C658) {
        func_00131000(arg0, func_0013C9A8);
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CC68);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CCE8);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CD68);

extern s32 func_0012FA68(void *);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); the pass-through arg0 to func_00131088 and the callee
   prototypes aligned with the file by hand. Names, types and offsets are Unconfirmed readings. */
void func_0013CDD0(void *arg0) {
    *(s8 *)((char *)arg0 + 0xC) = 0;
    if (func_00131088(arg0) == (s32) func_0013C658) {
        func_0012FA68(arg0);
        func_00131000(arg0, func_0013C9A8);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CE28);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CF78);

extern void func_0013D488(void *arg0, s8 arg1);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013D068(void *arg0, s32 arg1) {
    s8 temp_5;

    temp_5 = *(s8 *)((char *)arg0 + 0xF);
    if (temp_5 != 1) {
        func_0013D488(arg0, temp_5);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D088);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D378);

extern void func_0012F978(void *, s32, s8);
extern void func_0012B1B8(void *, s32, s32, s8);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013D440(void **arg0) {
    func_0012F978(*arg0, 2, 2);
    func_0012B1B8(*arg0, 0x25, 0x2A, 2);
}

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013D480(void *arg0, s8 arg1) {
    *(s8 *)((char *)arg0 + 0xF) = arg1;
}

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013D488(void *arg0, s8 arg1) {
    *(s8 *)((char *)arg0 + 0xD) = arg1;
    *(s32 *)((char *)arg0 + 0x28) = 0;
    *(s32 *)((char *)arg0 + 0x2C) = 0;
}

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain), declarations aligned with the file by hand; names, types and
   offsets are Unconfirmed readings. */
s32 func_0013D498(void *arg0) {
    return *(u8 *)(*(char **)arg0 + 0x149);
}


/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0013D4A8(void **arg0) {
    void *temp_4;

    temp_4 = *arg0;
    return func_0012BA30(temp_4, 1, (s32) *(u8 *)((char *)temp_4 + 0x149));
}

extern s32 func_0013D498(void *);
extern s32 func_0012B540(void *, s32, s8, s32);

/* ER-031 / ER-051: first C jump table in the files cut from S1's (docs/game_seams.md 9.10). Picks 10 for state 9,
   9 for states 8 and 10, else 8, from func_0013D498, and passes it to func_0012B540. The explicit cases 0 and 12
   give the table's 0..12 range (gcc indexes from the lowest case). Names and types Unconfirmed. */
s32 func_0013D4D0(void **a0) {
    s32 v;

    switch (func_0013D498(a0)) {
    case 9:
        v = 10;
        break;
    case 8:
    case 10:
        v = 9;
        break;
    case 0:
    case 12:
    default:
        v = 8;
        break;
    }
    func_0012B540(*a0, 1, v, 0);
    return v;
}

/* ER-072: jump table (13 entries, 0..12) by hand, same shape as func_0013D4D0: 0 for states 0, 11 and 12, 12 for
   state 9, else 11, from func_0013D498, passed to func_0012B540. gcc needs five case labels for a table (four
   give a compare tree), so state 10 is listed beside default (ruling 3.9); which value the original listed there
   is Unconfirmed (any of 1..8 or 10 gives the same bytes). Names and types Unconfirmed. */
s32 func_0013D550(void **a0) {
    s32 v;

    switch (func_0013D498(a0)) {
    case 0:
    case 11:
    case 12:
        v = 0;
        break;
    case 9:
        v = 12;
        break;
    case 10:
    default:
        v = 11;
        break;
    }
    func_0012B540(*a0, 1, v, 0);
    return v;
}

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D5D0);

extern void func_0012FAE8(void *, int, int, int, float);
extern void func_0013D828(void *);
extern s32 func_0013D838(void *, void *);
extern s32 func_0012E568(s32, s32);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), fixed by hand; names, types and offsets are Unconfirmed readings. */
void func_0013D688(void **arg0, void *arg1) {
    f32 temp_f20;
    s32 temp_2;

    switch (*(s16 *)((char *)arg0 + 0x6E)) {
    case 0:
        temp_f20 = 0.08f;
        func_0012FAE8(arg0, 3, 0, (s32) (arg0 + 32), 2500.0f);
        func_0012B890(arg0, 0, 0, *(s16 *)((char *)arg0 + 0x6C), 0x3C, temp_f20, 0);
        if (func_0012B890(arg0, 1, 0, *(s16 *)((char *)arg0 + 0x6C), 0x3C, temp_f20, 0) != 0) {
            func_0013D828(arg0);
            *(s16 *)((char *)arg0 + 0x6E) = *(s16 *)((char *)arg0 + 0x6E) + 1;
        }
        return;
    case 1:
        if (func_0013D838(arg0, arg1) != 0) {
            *(s32 *)((char *)arg0 + 0x10) = func_0012E568(0, 3);
            *(s16 *)((char *)arg0 + 0x6E) = *(s16 *)((char *)arg0 + 0x6E) + 1;
        }
        break;
    case 2:
        temp_2 = *(s32 *)((char *)arg0 + 0x10) - 1;
        *(s32 *)((char *)arg0 + 0x10) = temp_2;
        if (temp_2 < 0) {
            *(s16 *)((char *)arg0 + 0x6E) = 1;
        }
        break;
    }
}


/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013D7A8(void *arg0) {
    func_0012B218(arg0, 1);
    func_0012B218(arg0, 2);
}

extern s32 func_00150070(void **arg0, s32 arg1, s32 arg2);
extern void func_0013D688(void **, void *);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013D7D8(void **arg0) {
    if (func_00150070(arg0, 3, 3) != 0) {
        func_00131000((s8 *) arg0, func_0013D688);
    }
}

extern void func_0014FE10(void);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed. */
void func_0013D820(void) {
    func_0014FE10();
}

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013D828(void *arg0) {
    *(s32 *)((char *)arg0 + 0x28) = 0;
    *(s32 *)((char *)arg0 + 0x2C) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D838);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D9D0);
