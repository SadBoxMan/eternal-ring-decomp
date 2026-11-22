#include "common.h"

/* ER-024: this file's .sdata (docs/build.md 3.2) */
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA08);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA0C);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA14);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA18);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA1C);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA20);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA24);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA28);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA2C);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA30);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA38);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA3C);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA40);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA44);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA48);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA4C);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA50);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA54);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA58);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA5C);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA60);
INCLUDE_SDATA("asm/nonmatchings/text_001388E8", D_0035BA64);

/* ER-024: this file's .data (docs/build.md 3.2) */
INCLUDE_DATA("asm/nonmatchings/text_001388E8", D_0032FE68);
INCLUDE_DATA("asm/nonmatchings/text_001388E8", D_0032FE78);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_001388E8);

extern void * func_00139B38(s8 *arg0, void *arg1, void *arg2, void *arg3, void *arg4, void *arg5);
extern void func_00138D50(s8 *, void *);
extern void func_00138BE0(s8 **, s32);
extern s32 func_0012F8C8(s8 *, s32, s32, s32, s32, f32);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001389B8(s8 *arg0) {
    if (func_00139B38(arg0, func_00138D50, func_00138BE0, func_00138BE0, func_00138BE0, NULL) == NULL) {
        func_0012F8C8(arg0, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x14, 1, 0.1f);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00138A20);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00138BE0);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00138CB0);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00138D50);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00138E90);

extern void func_00139C98(void *arg0, s32 arg1);
extern void func_00139450(void **, s32);
extern s32 func_0014FF38(void *arg0, s32 arg1);
extern void func_00139170(void **, void *);
extern void func_00138CB0(s8 *, void *);
extern void func_001389B8(s8 *arg0);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00139050(s8 *arg0) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 0xD) = 0;
        *(s8 *)((char *)arg0 + 0xE) = 0;
        func_00139C98(arg0, (s32) func_00139450);
    }
    if (func_0014FF38(arg0, 3) != 0) {
        func_00139C98(arg0, (s32) func_00139170);
        func_00139B38(arg0, func_00138D50, func_00138CB0, func_00138CB0, func_00138CB0, func_001389B8);
    }
}

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001390E8(void *arg0) {
    *(s32 *)((char *)arg0 + 0x44) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_001390F8);

void func_00139148(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139150);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139170);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_001392A8);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139450);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139488);

extern float func_00130378(char *, s32);
extern void func_0012E820(char *, float, float);
extern f32 D_0035BA30;

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
f32 func_00139560(void *arg0, s32 arg1, f32 fparg0) {
    f32 temp_f20;
    f32 temp_f20_2;

    temp_f20_2 = func_00130378(arg1 + 0x90, *(s32 *)((char *)arg0 + 0x130)) + 6.2831855f;
    temp_f20 = (temp_f20_2 - ((f32) (s32) (temp_f20_2 / 6.2831855f) * 6.2831855f)) - 3.1415927f;
    func_0012E820(arg1 + 0xA4, temp_f20 + ((D_0035BA30 * 3.1415927f) / 180.0f), fparg0);
    return temp_f20;
}

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00139608(s8 *arg0, s32 arg1) {
    return func_0012F8C8(arg0, (*(s8 *)((char *)arg0 + 0xC) == 0) ? 0 : 0xA, arg1, 0x14, 1, 0.2f);
}

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00139640(void *arg0) {
    *(s32 *)((char *)arg0 + 0x1C) = 0;
    *(s32 *)((char *)arg0 + 0x20) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139650);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001397D0(void *arg0) {
    *(s32 *)((char *)arg0 + 0x1C) = 0;
    *(s32 *)((char *)arg0 + 0x20) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_001397E0);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_001399B8);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139A70);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139AD0);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139AF8);

INCLUDE_RODATA("asm/nonmatchings/text_001388E8", D_00354788);
extern s32 func_00139AF8(s8 *);
extern void func_00131000(char *, void *);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void* func_00139B38(s8 *arg0, void *arg1, void *arg2, void *arg3, void *arg4, void *arg5) {
    u32 temp_2;

    temp_2 = func_00139AF8(arg0);
    switch (temp_2) {
    case 0:
        func_00131000(arg0, arg1);
        return arg1;
    case 1:
        func_00131000(arg0, arg2);
        return arg2;
    case 2:
        func_00131000(arg0, arg3);
        return arg3;
    case 3:
        func_00131000(arg0, arg4);
        return arg4;
    case 4:
        func_00131000(arg0, arg5);
        return arg5;
    default:
        return NULL;
    }
}

extern void func_0012F9C0(void **, s32);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00139C28(void **arg0, s32 arg1, s32 arg2) {
    u16 temp_17;

    temp_17 = *(u16 *)((char *)(*(void **)((char *)(*arg0) + 0xE0)) + 0x26C);
    if (temp_17 == arg1) {
        func_0012F9C0(arg0, 4);
    }
    if (temp_17 == arg2) {
        func_0012F9C0(arg0, 5);
    }
}

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00139C98(void *arg0, s32 arg1) {
    *(s32 *)((char *)arg0 + 0x4C) = arg1;
    *(s32 *)((char *)arg0 + 0x2C) = 0;
    *(s32 *)((char *)arg0 + 0x28) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139CA8);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139D10);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139D28);

extern float func_00130D00(char *, float);
extern void func_00139EE0(s8 *, void *);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00139E70(s8 *arg0) {
    func_0012F8C8(arg0, 9, (s32) *(s16 *)((char *)arg0 + 0x6C), 6, 1, 0.3f);
    if (func_00130D00(arg0, 20.0f) <= 0.08726647f) {
        func_00131000(arg0, func_00139EE0);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139EE0);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_0013A050);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_0013A1A0);

extern void func_0012B238(void);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed. */
void func_0013A2C0(void) {
    func_0012B238();
}

extern void func_00139E70(s8 *arg0);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013A2C8(s8 *arg0) {
    if (func_0014FF38(arg0, 3) != 0) {
        func_00131000(arg0, func_00139E70);
    }
}

extern s32 func_00131088(void *);
extern void func_00139D10();
extern s32 func_0012FA68(void *);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); the pass-through arg0 to func_00131088 and the callee
   prototypes aligned with the file by hand. Names, types and offsets are Unconfirmed readings. */
void func_0013A310(void *arg0) {
    if (func_00131088(arg0) == (s32) func_00139D10) {
        func_0012FA68(arg0);
        func_00131000(arg0, func_00139E70);
    }
}

extern s32 func_0012FEE0(s32, s32, s32);
extern s32 func_0014FE10(void *, s32);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013A368(void *arg0, s32 arg1) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        func_0012FEE0(arg1, 0x13, 0);
    }
    func_0014FE10(arg0, arg1);
}

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0013A3B8(s8 *arg0) {
    s32 temp_2;
    void *temp_3;

    temp_2 = func_0012F8C8(arg0, 9, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x14, 0, 0.1f);
    if (temp_2 != 0) {
        temp_3 = *(void **)arg0;
        *(u8 *)((char *)temp_3 + 0x147) = (u8) (*(u8 *)((char *)temp_3 + 0x147) & 0xFE);
    }
    return temp_2;
}

INCLUDE_RODATA("asm/nonmatchings/text_001388E8", D_003547C8);
