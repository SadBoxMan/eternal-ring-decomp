#include "common.h"

/* ER-024: this file's .sdata (docs/build.md 3.2) */
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA68);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA6C);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA70);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA74);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA78);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA7C);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA80);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA84);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA88);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA8C);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA90);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA94);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA98);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BA9C);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BAA0);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BAA4);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BAA8);
INCLUDE_SDATA("asm/nonmatchings/text_0013A410", D_0035BAAC);

extern void func_0013B3A0(void *arg0);
extern void func_00131000(char *, void *);
extern void func_0013A490(void **, void *);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013A410(s8 *arg0) {
    void *temp_2;

    *(s8 *)((char *)arg0 + 0xF) = 0;
    *(s32 *)((char *)arg0 + 0x24) = 0;
    *(s32 *)((char *)arg0 + 0x28) = 0;
    *(s32 *)((char *)arg0 + 0x2C) = 0;
    *(s32 *)((char *)arg0 + 0x48) = 0;
    *(s32 *)((char *)arg0 + 0x4C) = 0;
    func_0013B3A0(arg0);
    temp_2 = *(void **)arg0;
    if (*(f32 *)((char *)temp_2 + 0x12C) == 0.0f) {
        *(f32 *)((char *)temp_2 + 0x12C) = 1000.0f;
    }
    *(u8 *)((char *)arg0 + 0x5A) = (u8) ((*(u8 *)((char *)arg0 + 0x5A) | 8) & 0xFD);
    func_00131000(arg0, func_0013A490);
}

INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013A490);

INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013A888);

INCLUDE_RODATA("asm/nonmatchings/text_0013A410", D_003547D0);
INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013AA28);

INCLUDE_RODATA("asm/nonmatchings/text_0013A410", D_00354800);
extern int func_0012E568(int, int);
extern void func_00130CE0(char *, float);
extern s32 func_0012F8C8(s8 *, s32, s32, s32, s32, f32);
extern int func_00130780(char *);
extern void func_0013AEA0(s8 *, s32);
extern int func_00131020(char *, int, void *, void *);
extern void func_0013AFC8(s8 *, void *);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013AD40(s8 *arg0, void *arg1) {
    s32 temp_2_2;
    u32 temp_2;

    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s32 *)((char *)arg0 + 0x10) = func_0012E568(0x78, 0x12C);
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    func_00130CE0(arg0, 20.0f);
    if (func_0012F8C8(arg0, 0xC, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x1E, 0, 0.1f) != 0) {
        *(u8 *)((char *)arg1 + 0x147) = (u8) (*(u8 *)((char *)arg1 + 0x147) & 1);
    }
    temp_2 = func_00130780(arg0);
    switch (temp_2) {
    case 0:
        func_00131000(arg0, func_0013AEA0);
        return;
    case 1:
        func_00131020(arg0, 0x46, func_0013AFC8, func_0013AEA0);
        return;
    case 2:
    case 3:
        temp_2_2 = *(s32 *)((char *)arg0 + 0x10) - 1;
        *(s32 *)((char *)arg0 + 0x10) = temp_2_2;
        if (temp_2_2 < 0) {
            func_00131000(arg0, func_0013AEA0);
            return;
        }
    default:
        return;
    case 4:
        func_00131000(arg0, func_0013AFC8);
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013AEA0);

INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013AFC8);

INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013B170);

extern s32 func_0014FF38(void *arg0, s32 arg1);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013B218(s8 *arg0) {
    if (func_0014FF38(arg0, 3) != 0) {
        func_00131000(arg0, func_0013AFC8);
    }
}

extern void func_0014FE10(void);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed. */
void func_0013B260(void) {
    func_0014FE10();
}

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013B268(s8 *arg0) {
    if (func_0014FF38(arg0, 0x11) != 0) {
        func_00131000(arg0, func_0013A490);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013B2B0);

extern void func_0013B170(void *, void *);
extern void func_0013B218(s8 *arg0);
void func_0013B260(void);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013B370(void *arg0) {
    *(void (**)(void *, void *))((char *)arg0 + 0xC0) = func_0013B170;
    *(void (**)(s8 *))((char *)arg0 + 0xC4) = func_0013B218;
    *(void (**)(s8 *))((char *)arg0 + 0xC8) = func_0013B218;
    *(void (**)())((char *)arg0 + 0xCC) = func_0013B260;
}

extern void func_0013B268(s8 *arg0);
extern void func_0013B2B0(s8 *, s32);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013B3A0(void *arg0) {
    *(void (**)(void *, void *))((char *)arg0 + 0xC0) = func_0013B170;
    *(void (**)(s8 *))((char *)arg0 + 0xC4) = func_0013B268;
    *(void (**)(s8 *))((char *)arg0 + 0xC8) = func_0013B268;
    *(void (**)(s8 *, s32))((char *)arg0 + 0xCC) = func_0013B2B0;
}

INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013B3D0);

INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013B3F0);

INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013B438);

INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013B5B0);

extern void func_0012F9C0(void **, s32);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013B700(void **arg0) {
    if (*(s32 *)((char *)arg0 + 0x24) == 0) {
        func_0012F9C0(arg0, 2);
        *(s32 *)((char *)arg0 + 0x24) = func_0012E568(0xB4, 0x12C);
    }
}
