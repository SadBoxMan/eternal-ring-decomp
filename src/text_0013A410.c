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

extern f32 func_00130BF8(char *, f32);
extern float func_0012E820(char *, float, float);
extern s32 func_0012FAA8(void *);
extern s32 func_0012D9B8(void *, f32);
extern void func_001321E8(char *);

/* ER-158: by hand from ER-154's draft; names, types and offsets are Unconfirmed readings. The callees' return types
   (func_0012FAA8 s32, func_0012E820 float, both read from the game) give the game's $v1 counter and $f1 sum. */
void func_0013B2B0(s8 *arg0, s32 arg1) {
    if (*(s16 *)(arg0 + 0x6E) == 0) {
        *(f32 *)(arg0 + 0x94) = 0.0f;
        *(f32 *)(arg0 + 0xB0) = 0.35f;
        func_0012FAA8(arg0);
        *(s16 *)(arg0 + 0x6E) += 1;
    }
    func_00130BF8(arg0, 0.95f);
    func_0012F8C8(arg0, 0x14, *(s16 *)(arg0 + 0x6C), 0xA, 0, 0.2f);
    func_0012E820((char *)arg1 + 0xA0, 3.1415927f, 15.0f);
    *(f32 *)(arg0 + 0x94) += *(f32 *)(arg0 + 0xB0);
    if (func_0012D9B8(arg0, *(f32 *)(arg0 + 0x94)) != 0) {
        func_001321E8(arg0);
    }
}

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

extern s32 func_001313A8(char *);
extern s32 *func_0013B5B0(char *, s32, f32 *);
extern float func_00130378(char *, s32);
extern float func_0012E7B0(float, float);
extern f32 D_0035BA7C;
extern f32 D_0035BA84;

/* ER-198: by hand from the m2c draft; both early exits written first (2.43), 500.0f is v[0] of a stack vector passed
   to func_0013B5B0 (2.46; its prototype s32 *(char *, s32, f32 *) is a reading), the step stored in each arm (gcc
   merges the tails, so x48 is reloaded) and a fresh local for the clamp; names, types and offsets are Unconfirmed
   readings. */
s32 func_0013B438(char *arg0, char *arg1) {
    f32 v[4];
    s32 t;
    s32 *r;
    f32 a;
    f32 d;
    f32 lim;
    f32 x;
    f32 y;

    t = func_001313A8(arg0);
    if (t == 0) {
        return 0;
    }
    v[0] = 500.0f;
    r = func_0013B5B0(arg0, t, v);
    if (r == NULL) {
        return 0;
    }
    a = func_00130378(arg1 + 0x90, *r + 0x90) + 6.2831855f;
    d = func_0012E7B0((a - (f32)(s32)(a / 6.2831855f) * 6.2831855f) - 3.1415927f, *(f32 *)(arg1 + 0xA4));
    x = *(f32 *)(arg0 + 0x48);
    if (d >= 0.0f) {
        *(f32 *)(arg0 + 0x48) = x + -((D_0035BA84 * 3.1415927f) / 180.0f) * 4.0f;
    } else {
        *(f32 *)(arg0 + 0x48) = x + ((D_0035BA84 * 3.1415927f) / 180.0f) * 4.0f;
    }
    lim = (D_0035BA7C * 3.1415927f) / 180.0f;
    y = *(f32 *)(arg0 + 0x48);
    if (y < -lim) {
        y = -lim;
    } else if (lim < y) {
        y = lim;
    }
    *(f32 *)(arg0 + 0x48) = y;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/text_0013A410", func_0013B5B0);

extern void func_0012F9C0(void **, s32);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0013B700(void **arg0) {
    if (*(s32 *)((char *)arg0 + 0x24) == 0) {
        func_0012F9C0(arg0, 2);
        *(s32 *)((char *)arg0 + 0x24) = func_0012E568(0xB4, 0x12C);
    }
}
