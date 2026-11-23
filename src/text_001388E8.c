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
extern void func_00138BE0(s8 *, s32);
extern s32 func_0012F8C8(s8 *, s32, s32, s32, s32, f32);

/* ER-072: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001389B8(s8 *arg0) {
    if (func_00139B38(arg0, func_00138D50, func_00138BE0, func_00138BE0, func_00138BE0, NULL) == NULL) {
        func_0012F8C8(arg0, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x14, 1, 0.1f);
    }
}

extern s32 func_00139608(s8 *arg0, s32 arg1);
extern int func_0012E568(int, int);
extern void func_00139640(void *arg0);
extern s32 func_00139650(s8 *, s32);
extern void func_001389B8();
extern void func_00138CB0();
extern void func_00138D50();
extern s32 func_00139AF8(s8 *);
extern void func_00131000(char *, void *);

/* ER-072: two jump tables (5 entries each) by hand from the m2c draft. A state handler (a reading): counts down
   +0x14 each frame and steps the state at +0x6E; state 2 switches on func_00139AF8's choice to leave for
   func_00138D50 / func_00138CB0 / func_001389B8 or go to state 3. Names, types and offsets Unconfirmed. */
void func_00138A20(s8 *arg0, s32 arg1) {
    *(s8 *)(arg0 + 0xB) = 1;
    *(s32 *)(arg0 + 0x14) -= 1;
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        if (func_00139608(arg0, *(s16 *)(arg0 + 0x6C)) != 0) {
            *(s32 *)(arg0 + 0x14) = func_0012E568(0x4B0, 0x708);
            *(s16 *)(arg0 + 0x6E) = 1;
        }
        break;
    case 1:
        *(s32 *)(arg0 + 0x10) = func_0012E568(0x1E, 0x78);
        *(s16 *)(arg0 + 0x6E) = 2;
    case 2:
        *(s32 *)(arg0 + 0x10) -= 1;
        switch (func_00139AF8(arg0)) {
        case 0:
            func_00131000(arg0, func_00138D50);
            break;
        case 1:
            if (*(s16 *)(arg0 + 0x6C) < 0x79) {
                *(s16 *)(arg0 + 0x6E) = 3;
            } else {
                func_00131000(arg0, func_00138CB0);
            }
            break;
        case 2:
            if (*(s32 *)(arg0 + 0x14) < 0) {
                func_00131000(arg0, func_00138CB0);
            } else if (*(s32 *)(arg0 + 0x10) < 0) {
                *(s16 *)(arg0 + 0x6E) = 3;
            }
            break;
        case 3:
            *(s16 *)(arg0 + 0x6E) = 3;
            break;
        case 4:
            func_00131000(arg0, func_001389B8);
            break;
        }
        break;
    case 3:
        if (*(s32 *)(arg0 + 0x44) != 0) {
            func_00131000(arg0, func_00138CB0);
            return;
        }
        func_00139640(arg0);
        *(s16 *)(arg0 + 0x6E) = 4;
    case 4:
        if (func_00139650(arg0, arg1) != 0) {
            *(s16 *)(arg0 + 0x6E) = 1;
        }
        break;
    }
}

void func_00138A20(s8 *arg0, s32 arg1);

/* ER-078: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00138BE0(s8 *arg0, s32 arg1) {
    s16 temp_3;

    *(s8 *)((char *)arg0 + 0xB) = 1;
    temp_3 = *(s16 *)((char *)arg0 + 0x6E);
    switch (temp_3) {                               /* irregular */
    case 0:
        if (func_00139608(arg0, (s32) *(s16 *)((char *)arg0 + 0x6C)) != 0) {
            func_00139640(arg0);
            *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
        }
        return;
    case 1:
        if (func_00139650(arg0, arg1) != 0) {
            if ((func_00139B38(arg0, func_00138D50, func_00138CB0, func_00138A20, NULL, func_001389B8) == NULL) && (*(s32 *)((char *)arg0 + 0x44) != 0)) {
                func_00131000(arg0, func_00138CB0);
            }
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00138CB0);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00138D50);

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00138E90);

extern void func_00139C98(void *arg0, s32 arg1);
extern void func_00139450(void **);
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

void func_00139148(void);
extern s32 func_0014FE10(void *, s32);

/* ER-078: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001390F8(void *arg0, s32 arg1) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 0xD) = 0;
        *(s8 *)((char *)arg0 + 0xE) = 0;
        func_00139C98(arg0, (s32) func_00139148);
    }
    func_0014FE10(arg0, arg1);
}

void func_00139148(void) {
}

extern s32 func_0012B890(void **arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s32 arg5, f32 fparg0);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00139150(void **arg0) {
    func_0012B890(arg0, 1, 0, *(s32 *)((char *)arg0 + 0x2C), 0x14, 1, 0.1f);
}

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139170);

extern void func_0012F9C0(void **, s32);
extern void func_0012C3E8(void *, s32, s32, s32, f32, f32);
extern f32 func_0012E6B8(f32, f32);
extern s32 func_0012BA30(void *, s32, s32);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001392A8(void **arg0, void *arg1) {
    s32 var_18;
    s32 var_19;
    u16 temp_3;
    void *temp_16;

    var_18 = *(s32 *)((char *)arg0 + 0x28);
    var_19 = *(s32 *)((char *)arg0 + 0x2C);
    switch (var_18) {                               /* irregular */
    case 0:
        if (*(s8 *)((char *)arg0 + 0xB) == 0) {
            func_00139C98(arg0, (s32) func_00139170);
            return;
        }
        if (func_0012B890(arg0, 1, 0xB, var_19, 0x14, 0, 0.1f) != 0) {
            var_19 = 0;
            var_18 = 1;
        }
    default:
        *(s32 *)((char *)arg0 + 0x28) = var_18;
        *(s32 *)((char *)arg0 + 0x2C) = var_19;
        return;
    case 1:
        temp_16 = *(void **)((char *)arg1 + 0xE0);
        temp_3 = *(u16 *)((char *)temp_16 + 0xB2C);
        if (temp_3 == 0x28) {
            func_0012F9C0(arg0, 3);
        } else if (temp_3 == 0x2D) {
            func_0012C3E8(arg0, 0x84, 2, (s32) (temp_16 + 0xB70), 0.0f, func_0012E6B8(0.0f, 100.0f));
        } else if (func_0012BA30(arg1, 1, 0xB) != 0) {
            var_19 = 0;
            var_18 = 2;
        }
        /* Duplicate return node #24. Try simplifying control flow for better match */
        *(s32 *)((char *)arg0 + 0x28) = var_18;
        *(s32 *)((char *)arg0 + 0x2C) = var_19;
        return;
    case 2:
        if (var_19 >= 0x1E) {
            func_00139C98(arg0, (s32) func_00139170);
            return;
        }
        /* Duplicate return node #24. Try simplifying control flow for better match */
        *(s32 *)((char *)arg0 + 0x28) = var_18;
        *(s32 *)((char *)arg0 + 0x2C) = var_19;
        return;
    }
}

extern s32 func_0012B6B0(void **arg0, s32 arg1, s32 arg2, s32 arg3, f32 fparg0);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00139450(void **arg0) {
    if (*(s32 *)((char *)arg0 + 0x28) == 0) {
        *(s32 *)((char *)arg0 + 0x28) = 1;
        *(s8 *)((char *)arg0 + 0xE) = 0;
        *(u8 *)((char *)arg0 + 0x5A) = (u8) (*(u8 *)((char *)arg0 + 0x5A) & 0xF7);
    }
    func_0012B6B0(arg0, 1, 3, 0, 0.3f);
}

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

extern float func_0012CE18(char *, char *);
extern float func_00130258(char *);
extern float func_00131608(float a, float b);

/* ER-041 (proven under make regress, put back for the file's owner), placed by ER-072: picks a turn of at most 60
   degrees toward a1 (a reading). Names and types Unconfirmed. */
float func_001399B8(char *a0, char *a1, float f) {
    float s, d;
    s = (func_0012CE18(a0, a1 + 0x90) < 0.0) ? -1.0f : 1.0f;
    d = func_00131608(func_00130258(a0), s * f);
    d = (d < -1.0471976f) ? -1.0471976f : ((1.0471976f < d) ? 1.0471976f : d);
    return func_00131608(*(float *)(a1 + 0xA4), d);
}

INCLUDE_ASM("asm/nonmatchings/text_001388E8", func_00139A70);

extern void func_0012DF30(void *arg0, s32 arg1, s32 arg2, void *arg3, f32 *arg4, f32 arg5);
extern f32 D_0032FE78[];
extern f32 D_0035BA0C;

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00139AD0(void *arg0, void *arg1) {
    func_0012DF30(arg1, 0x10, 3, *(void **)((char *)arg0 + 0x130), D_0032FE78, D_0035BA0C);
}

extern s32 func_00130780(s8 *);

/* ER-078: from the m2c draft, the two flag stores in game order. Names, types and offsets Unconfirmed. */
s32 func_00139AF8(s8 *arg0) {
    s32 n;

    n = func_00130780(arg0);
    if (n < 4) {
        *(s8 *)(arg0 + 0xD) = 1;
        *(s8 *)(arg0 + 0xE) = 1;
    }
    return n;
}

INCLUDE_RODATA("asm/nonmatchings/text_001388E8", D_00354788);

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

extern void func_00139D10();
extern s32 func_0012F900(char *arg0, s32 arg1, s32 arg2, s16 arg3, s32 arg4, f32 arg5);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00139D10(s8 *arg0) {
    func_0012F900(arg0, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x270F, 0, 0.01f);
}

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

extern int func_00131140(char *, void *, void *, void *, void *, void *);
extern void func_00130CE0(char *, float);
extern void func_00130848(s8 **, f32);
extern f32 func_00130AD0(s8 *, f32, f32);
extern void func_0013A050();
extern void func_0013A1A0();
extern f32 D_0035BA5C;
extern f32 D_0035BA60;
extern f32 D_0035BA64;

/* ER-072: jump table (5 entries) by hand from the m2c draft. A state handler (a reading): unless func_00131140
   leaves, steps the counter at +0x6E through five states; states 1 and 3 fall through into 2 and 4, which wait for
   the value 11 / 44 at +0x8C of arg1's +0xE0 object. Names, types and offsets Unconfirmed. */
void func_00139EE0(s8 *arg0, void *arg1) {
    if (func_00131140(arg0, func_0013A1A0, NULL, NULL, NULL, func_0013A050) != 0) {
        return;
    }
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        func_00130CE0(arg0, 25.0f);
        if (func_0012F8C8(arg0, 1, *(s16 *)(arg0 + 0x6C), 10, 1, 0.25f) != 0) {
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 1:
        func_00130848((s8 **)arg0, D_0035BA5C);
        *(s16 *)(arg0 + 0x6E) += 1;
    case 2:
        func_00130CE0(arg0, 25.0f);
        func_00130AD0(arg0, D_0035BA60, D_0035BA64);
        if (*(u16 *)(*(char **)((char *)arg1 + 0xE0) + 0x8C) == 11) {
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 3:
        func_00130848((s8 **)arg0, D_0035BA5C);
        *(s16 *)(arg0 + 0x6E) += 1;
    case 4:
        func_00130CE0(arg0, 25.0f);
        func_00130AD0(arg0, D_0035BA60, D_0035BA64);
        if (*(u16 *)(*(char **)((char *)arg1 + 0xE0) + 0x8C) == 44) {
            *(s16 *)(arg0 + 0x6E) = 1;
        }
        break;
    }
}

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
