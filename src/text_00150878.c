#include "common.h"

extern void func_00151640(void **arg0, s32 arg1);
extern void func_0012B1B8(void *, s32, s32, s32);
extern void func_0012F978(void *, s32, s32);
extern void func_00131000(char *, void *);
extern void func_00150EF8(void **, void *);
extern void func_00151198(void **);
extern void func_00150988(void **arg0);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00150878(void **arg0) {
    func_00151640(arg0, 0);
    *(s8 *)((char *)arg0 + 0xD) = 0;
    *(s32 *)((char *)arg0 + 0x24) = 0;
    *(s32 *)((char *)arg0 + 0x2C) = 0;
    *(s32 *)((char *)arg0 + 0x28) = 0;
    *(s32 *)((char *)arg0 + 0x40) = 0;
    *(s32 *)((char *)arg0 + 0x44) = 0;
    *(s32 *)((char *)arg0 + 0x48) = 0;
    *(s32 *)((char *)arg0 + 0x4C) = 0;
    *(s8 *)((char *)arg0 + 0xF) = -1;
    *(s8 *)((char *)arg0 + 0xE) = -1;
    *(void **)((char *)arg0 + 0xC0) = func_00150EF8;
    *(void **)((char *)arg0 + 0xC4) = func_00151198;
    *(void **)((char *)arg0 + 0xC8) = func_00151198;
    func_0012B1B8(*arg0, 0x1A, 0x21, 2);
    func_0012B1B8(*arg0, 0, 1, 4);
    func_0012B1B8(*arg0, 8, 0xA, 4);
    func_0012B1B8(*arg0, 0xC, 0xE, 4);
    func_0012F978(*arg0, 5, 8);
    *(s8 *)(*(char **)((char *)*arg0 + 0xE0) + 0x4E8) = 1;
    *(s8 *)(*(char **)((char *)*arg0 + 0xE0) + 0x768) = 1;
    *(s8 *)(*(char **)((char *)*arg0 + 0xE0) + 0x268) = 1;
    *(u8 *)((char *)arg0 + 0x5A) |= 8;
    func_00131000((char *)arg0, func_00150988);
}

extern s32 func_0012B890(void **, s32, s32, s32, s16, f32 fparg0, s32);
extern s32 func_001512E8(void **arg0, s8 arg1);
extern s32 func_001512A0(void **, s8, s32, f32 fparg0);
extern s32 func_00131140(char *, void *, void *, void *, void *, void *);
extern void func_00150A30(s8 *, void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00150988(void **arg0) {
    f32 temp_f20;

    temp_f20 = 0.15f;
    func_0012B890(arg0, 0, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x14, temp_f20, 0);
    func_0012B890(arg0, 3, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x14, temp_f20, 0);
    func_001512E8(arg0, 0);
    func_001512A0(arg0, 0, 1, 1.0f);
    func_00131140((s8 *) arg0, func_00150A30, func_00150A30, func_00150A30, func_00150A30, NULL);
}

extern s32 func_00151330(void *, s32); extern void func_00151640(void **arg0, s32 arg1); extern s32 func_0012BA30(void *, s32, s32);
extern void func_00131000(char *, void *); extern void func_00150B08(s8 *);
/* ER-096: by hand from ER-080's draft; func_0012B890's float is its sixth parameter (as text_00145120 declares it):
   with the float last, the first call's delay slot differs. Names, types and offsets are Unconfirmed readings. */
void func_00150A30(s8 *arg0, void *arg1) {
    f32 s;
    func_00151330(arg0, 0);
    if (*(s16 *)(arg0 + 0x6E) == 0) {
        s = 0.1f;
        *(s8 *)(arg0 + 0xD) = 0;
        func_0012B890((void **)arg0, 3, 0xE, *(s16 *)(arg0 + 0x6C), 0x14, s, 0);
        if (func_0012B890((void **)arg0, 0, 0xE, *(s16 *)(arg0 + 0x6C), 0x14, s, 0) != 0) {
            func_00151640((void **)arg0, 1);
            if (func_0012BA30(arg1, 0, 0xE) != 0) func_00131000(arg0, func_00150B08);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00150B08);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00150C00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00150D40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00150EF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00151040);

extern s32 func_00150070(void **arg0, s32 arg1, s32 arg2);
extern void func_00150B08(s8 *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00151198(void **arg0) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 0xD) = 0;
    }
    *(f32 *)((char *)arg0 + 0x94) = (f32) (*(f32 *)((char *)arg0 + 0x94) + 0.1f);
    if (func_00150070(arg0, 3, 0xB) != 0) {
        func_00131000((s8 *) arg0, func_00150B08);
    }
}

extern s32 func_0012FAA8();
extern s32 func_0014FE10(void *, s32);
extern s32 func_0012D9B8(void *, f32);
extern s32 func_001321E8(void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00151200(void *arg0, s32 arg1) {
    f32 temp_f0;

    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(f32 *)((char *)arg0 + 0x94) = 0.0f;
        *(f32 *)((char *)arg0 + 0xB0) = (f32) 0.2f;
        func_0012FAA8();
        *(s8 *)((char *)arg0 + 0xD) = 0;
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    func_0014FE10(arg0, arg1);
    temp_f0 = *(f32 *)((char *)arg0 + 0x94) + *(f32 *)((char *)arg0 + 0xB0);
    *(u8 *)((char *)arg0 + 0x5A) = (u8) (*(u8 *)((char *)arg0 + 0x5A) | 8);
    *(f32 *)((char *)arg0 + 0x94) = temp_f0;
    if (func_0012D9B8(arg0, temp_f0) != 0) {
        func_001321E8(arg0);
    }
}

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
s32 func_001512A0(void **arg0, s8 arg1, s32 arg2, f32 arg3) {
    if (*(s8 *)((char *)arg0 + 0xE) != arg1) {
        *(s8 *)((char *)arg0 + 0xE) = arg1;
        *(s32 *)((char *)arg0 + 0x28) = 0;
    }
    return func_0012B890(arg0, 2, arg1 & 0xFF, *(s32 *)((char *)arg0 + 0x28), arg2, arg3, 1);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001512E8(void **arg0, s8 arg1) {
    if (*(s8 *)((char *)arg0 + 0xF) != arg1) {
        *(s8 *)((char *)arg0 + 0xF) = arg1;
        *(s32 *)((char *)arg0 + 0x2C) = 0;
    }
    return func_0012B890(arg0, 1, arg1 & 0xFF, *(s32 *)((char *)arg0 + 0x2C), 0x78, 0.06f, 1);
}

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354A90);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00151330);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00151640(void **arg0, s32 arg1) {
    *(s8 *)((char *)(*(void **)((char *)(*arg0) + 0xE0)) + 0xC58) = (s8) (arg1 == 0);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00151658(void **arg0) {
    return *(u8 *)((char *)(*(void **)((char *)(*arg0) + 0xE0)) + 0xC58) == 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00151670);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001516E8);

extern void func_00151810(s8 *, void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00151800(s8 *arg0) {
    func_00131000(arg0, func_00151810);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00151810);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00151998);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00151AA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00151B60);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00151D00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00151E70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00151F68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001520F0);

extern void func_0012B218(void *, int);
extern void func_0012C788(void *);
extern s32 func_00131088(void *);
extern void func_001520F0();
extern void func_00152560(void *, s32);
extern void func_00152580(void *arg0, void *arg1);
extern void func_00152628(void *, f32);
extern void func_001527B0(void *, void *);
extern void func_001C9FE8(void *, void *, void *);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_001522B8(char *arg0, char *arg1) {
    char *p;

    func_0012B218(arg0, 1);
    func_0012C788(*(void **)arg0);
    if (*(s8 *)(arg0 + 0xF) != 0) {
        func_00152560(arg1, 1);
        func_00152580(arg0, arg1);
    } else {
        func_00152560(arg1, 0);
        func_0012B218(arg0, 4);
    }
    if (!(*(u8 *)(arg0 + 0x5A) & 0x40) && func_00131088(arg0) != (s32)func_001520F0) {
        func_00152628(arg1, 0.0f);
        func_001527B0(arg0, arg1);
    }
    func_0012B218(arg0, 2);
    if (*(s8 *)(arg0 + 0xE) != 0) {
        func_001C9FE8(arg1 + 0x90, arg1 + 0x90, *(char **)(arg1 + 0xE0) + 0x40);
        p = *(char **)(arg1 + 0xE0);
        *(s32 *)(p + 0x40) = 0;
        *(s32 *)(p + 0x44) = 0;
        *(s32 *)(p + 0x48) = 0;
    }
    *(s8 *)(arg0 + 0xE) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001523B0);

extern int func_0012E568(int, int);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001524D8(void) {
    return func_0012E568(0, 2);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001524F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00152548);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00152560);

extern void func_0012DF30(void *, s32, s32, void *, f32 *, f32 fparg0);
extern f32 D_00330228[];
extern f32 D_0035BFC8;
extern f32 D_00330218[];
extern f32 D_0035BFC4;
extern f32 D_00330208[];
extern f32 D_0035BFC0;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00152580(void *arg0, void *arg1) {
    func_0012C788(arg1);
    func_0012DF30(arg1, 0x21, 0x14, *(void **)((char *)arg0 + 0x130), D_00330228, D_0035BFC8);
    func_0012C788(arg1);
    func_0012DF30(arg1, 0x22, 0x21, *(void **)((char *)arg0 + 0x130), D_00330218, D_0035BFC4);
    func_0012C788(arg1);
    func_0012DF30(arg1, 0x23, 0x22, *(void **)((char *)arg0 + 0x130), D_00330208, D_0035BFC0);
    func_0012C788(arg1);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00152628);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00152778);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001527B0);

extern s32 func_0012B180(void *, s32);
extern f32 func_001E3EC8(f32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
f32 func_001528E0(void **arg0) {
    s32 temp_2;
    u16 temp_16;
    void *temp_2_2;
    void *temp_4;

    temp_4 = *arg0;
    temp_2_2 = *(void **)((char *)temp_4 + 0xE0);
    temp_16 = *(u16 *)((char *)temp_2_2 + 0x198C);
    temp_2 = func_0012B180(temp_4, (s32) *(u8 *)((char *)temp_2_2 + 0x1989));
    if ((temp_2 / 2) < (s32) temp_16) {
        return 0.0f;
    }
    return func_001E3EC8((((f32) temp_16 * -3.1415927f) / (f32) temp_2) * 0.5f);
}

extern void func_00152980(s8 *, void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00152970(s8 *arg0) {
    func_00131000(arg0, func_00152980);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00152980);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00152A10);

extern s32 func_00153238();
extern void func_00153428(void *arg0, f32 fparg0);
extern s32 func_001533F0(void *arg0, f32 fparg0);
extern f32 func_001314C8(s16, s32);
extern s32 func_00153380(void *);
extern void *func_0012C360(void *, s32, s32, s32, void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00152AC8(void *arg0, void *arg1) {
    s16 temp_3;
    s32 temp_2;

    func_00153238();
    temp_3 = *(s16 *)((char *)arg0 + 0x6E);
    switch (temp_3) {                               /* irregular */
    case 0:
        temp_2 = func_0012C360(arg0, 0xA9, 0, arg1 + 0x90, arg1 + 0xA0);
        *(s32 *)((char *)arg0 + 0x2C) = temp_2;
        if (temp_2 != 0) {
            *(s32 *)((char *)arg0 + 0x10) = func_0012E568(0x96, 0xB4);
            *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
        case 1:
            func_00153428(arg0, *(f32 *)((char *)arg1 + 0xA4));
            if (func_001533F0(arg0, func_001314C8(*(s16 *)((char *)arg0 + 0x6C), *(s32 *)((char *)arg0 + 0x10))) != 0) {
                func_00153380(arg0);
            }
        }
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00152B88);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00152D58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00152FB0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153060);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001530C8(s8 *arg0) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        func_001533F0(arg0, 0.0f);
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    if (*(s16 *)((char *)arg0 + 0x6C) >= 0x1F) {
        func_00153380(arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153128);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153238);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153380);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001533F0(void *arg0, f32 fparg0) {
    void *temp_2;

    temp_2 = *(void **)((char *)arg0 + 0x2C);
    if (temp_2 != NULL) {
        *(f32 *)((char *)temp_2 + 0x4C) = fparg0;
        if (fparg0 >= 1.0f) {
            *(void **)((char *)arg0 + 0x2C) = NULL;
            return 1;
        }
    }
    return 0;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00153428(void *arg0, f32 fparg0) {
    void *temp_2;

    temp_2 = *(void **)((char *)arg0 + 0x2C);
    if (temp_2 != NULL) {
        *(f32 *)((char *)temp_2 + 0x64) = fparg0;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153440);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001534D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153538);

extern void func_0012F8C8(void **, s32, s32, s32, s32, f32);
extern void func_00130CE0(char *, float);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_001535A8(char *arg0) {
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        *(s32 *)(arg0 + 0x90) = 0;
        *(f32 *)(arg0 + 0x94) = -12.0f;
        *(s32 *)(arg0 + 0x98) = 0;
        *(s32 *)(arg0 + 0x9C) = 0;
        *(s16 *)(arg0 + 0x6E) += 1;
        break;
    case 1:
        func_0012F8C8((void **)arg0, 0, *(s16 *)(arg0 + 0x6C), 0x14, 1, 0.2f);
        *(f32 *)(arg0 + 0x94) += 0.7f;
        if (func_0012D9B8(arg0, *(f32 *)(arg0 + 0x94)) != 0) {
            *(s16 *)(arg0 + 0x6C) = 0;
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 2:
        func_00130CE0(arg0, 30.0f);
        if (*(s16 *)(arg0 + 0x6C) >= 0x1F) {
            *(s16 *)(arg0 + 0x6E) = 0;
        }
        break;
    }
}

extern float func_001114E8(char *, float *);
extern int D_0035B824;
extern float D_003302E0[];

/* ER-034. When D_0035B824 is 0x17: d = func_001114E8(a0 + 0x90, D_003302E0) (a distance: reading); beyond 1750 pulls
   +0x90 / +0x98 in by 1750 / d relative to D_003302E0[0] / [2]; caps +0x94 at 0, -50 or -100 by d. Names, types and the
   reading Unconfirmed. */
void func_001536A0(char *a0) {
    float d;
    float f;

    if (D_0035B824 == 0x17) {
        d = func_001114E8(a0 + 0x90, D_003302E0);
        if (1750.0f < d) {
            *(float *)(a0 + 0x90) = ((*(float *)(a0 + 0x90) - D_003302E0[0]) * 1750.0f) / d;
            *(float *)(a0 + 0x98) = ((*(float *)(a0 + 0x98) - D_003302E0[2]) * 1750.0f) / d;
        }
        f = 0.0f;
        if (d < 1500.0f) {
            f = -50.0f;
        }
        if (d < 750.0f) {
            f -= 50.0f;
        }
        if (f < *(float *)(a0 + 0x94)) {
            *(float *)(a0 + 0x94) = f;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153798);

extern s32 func_00131470(void *, s32);
extern void func_0012FAE8(void *, int, int, int, float);
extern void func_00130FE0(void *, void *);
extern void func_001539A8(void *, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00153910(void *arg0, s32 arg1) {
    if ((func_00131470(arg0, 0x2E) == 0) || (*(u8 *)((char *)(*(void **)((char *)arg0 + 0xE8)) + 7) != 0)) {
        *(u8 *)((char *)arg0 + 0x5A) = (u8) (*(u8 *)((char *)arg0 + 0x5A) & 0xFB);
        func_0012FAE8(arg0, 2, 1, arg1 + 0x90, 2.5e+03f);
        func_00130FE0(arg0, func_001539A8);
        return;
    }
    *(u8 *)((char *)arg0 + 0x5A) = (u8) (*(u8 *)((char *)arg0 + 0x5A) | 4);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001539A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153B08);

extern float D_0035C068;
extern void func_00130B60(void *, float);

/* ER-049: clone of func_00150810, placed by tools/clones.py (callee and global renamed). Arguments Unconfirmed. */
void func_00153CC0(void *a0) {
    func_00130B60(a0, D_0035C068);
}

void func_00153CC8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153CD0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153D10);

extern void func_00131000(char *, void *); extern void func_001539A8();
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_00153DA0(char *a0) {
    if (*(s16 *)(a0 + 0x6E) == 0) { *(s16 *)(a0 + 0x74) = 0x1E; *(s16 *)(a0 + 0x6E) += 1; }
    *(f32 *)(a0 + 0x94) += 0.5f;
    if (*(s16 *)(a0 + 0x6C) > *(s16 *)(a0 + 0x74)) func_00131000(a0, func_001539A8);
}

extern void func_0012FB48(void *arg0, s32 arg1);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00153DF8(void *arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f1;

    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        func_0012FB48(arg0, 0);
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    temp_f0 = func_001314C8((s32) *(s16 *)((char *)arg0 + 0x6C), 0x78);
    temp_f1 = 1.0f - temp_f0;
    *(f32 *)((char *)arg1 + 0xB8) = temp_f1;
    *(f32 *)((char *)arg1 + 0xB4) = temp_f1;
    *(f32 *)((char *)arg1 + 0xB0) = temp_f1;
    if (0.99f <= temp_f0) {
        func_001321E8(arg0);
    }
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00153E88(void *arg0, s32 arg1) {
    *(s32 *)((char *)arg0 + 0x2C) = arg1;
    *(s32 *)((char *)arg0 + 0x28) = 0;
    *(s32 *)((char *)arg0 + 0x24) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153E98);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00153ED0);

extern void func_00154050(char *);
extern void func_00154698(s8 *arg0);
extern void func_001546F8(void *arg0, s32 arg1);
extern void func_00154710(void *, void *);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00153FD0(s8 *arg0) {
    *(s8 *)(arg0 + 0xE) = 0;
    *(s8 *)(arg0 + 0xF) = 0;
    *(s32 *)(arg0 + 0x4C) = 0;
    func_0012B1B8(*(void **)arg0, 2, 8, 2);
    *(void **)(arg0 + 0xC0) = func_00154710;
    *(void **)(arg0 + 0xC4) = func_00154698;
    *(void **)(arg0 + 0xC8) = func_00154698;
    *(void **)(arg0 + 0xCC) = func_001546F8;
    *(s8 *)(arg0 + 0x151) = 0x31;
    func_00131000(arg0, func_00154050);
}

extern s32 func_0012B7C0(void **, u32, s32, s32, s16, s32, f32 fparg0);
extern void func_001540C0(s8 *, void *);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00154050(char *arg0) {
    *(s8 *)(arg0 + 0xF) = 0;
    func_0012B7C0((void **)arg0, 3, 0, *(s16 *)(arg0 + 0x6C), 0x14, 1, 0.2f);
    if (func_00131140(arg0, func_001540C0, func_001540C0, func_001540C0, func_001540C0, 0) != 0) {
        *(s8 *)(arg0 + 0xF) = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001540C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001541C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001543B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00154598);

extern s32 func_0014FF98(void *arg0, s32 arg1, s32 arg2);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00154698(s8 *arg0) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 0xF) = 0;
    }
    if (func_0014FF98(arg0, 3, 3) != 0) {
        *(s8 *)((char *)arg0 + 0xF) = 1;
        func_00131000(arg0, func_001540C0);
    }
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001546F8(void *arg0, s32 arg1) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 0xF) = 0;
    }
    func_0014FE10(arg0, arg1);
}

extern void func_00154780(void *arg0, void *arg1);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00154710(void *arg0, void *arg1) {
    if (*(s8 *)((char *)arg0 + 0xF) != 0) {
        char *p;
        func_00154780(arg0, arg1);
        p = *(char **)((char *)arg1 + 0xE0);
        *(s8 *)(p + 0x628) = 1;
        *(s8 *)(p + 0x768) = 1;
    } else {
        char *q;
        q = *(char **)((char *)arg1 + 0xE0);
        *(s8 *)(q + 0x628) = 0;
        *(s8 *)(q + 0x768) = 0;
    }
    func_0012B218(arg0, 2);
    func_0012B218(arg0, 1);
}

extern void func_0012DDA0(void *, s32, s32, void *, void *, f32 fparg0);
extern s32 D_00330310[];
extern f32 D_0035C074;
extern s32 D_00330320[];
extern f32 D_0035C070;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00154780(void *arg0, void *arg1) {
    func_0012C788(arg1);
    func_0012DDA0(arg1, 9, 2, *(void **)((char *)arg0 + 0x130), D_00330310, D_0035C074);
    func_0012C788(arg1);
    func_0012DDA0(arg1, 0xB, 0xA, *(void **)((char *)arg0 + 0x130), D_00330320, D_0035C070);
    func_0012C788(arg1);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00154800);

extern s32 func_00154800(s32, void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00154840(void *arg0) {
    void *temp_8;

    if (func_00154800((s32) arg0, *(void **)((char *)arg0 + 0x4C)) == 0) {
        temp_8 = *(void **)arg0;
        *(void **)((char *)arg0 + 0x4C) = func_0012C360(arg0, 0xAE, 2, *(s32 *)((char *)temp_8 + 0xE0) + 0x710, temp_8 + 0xA0);
    }
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00154898(void *arg0) {
    *(s32 *)((char *)arg0 + 0x24) = 0;
    *(s32 *)((char *)arg0 + 0x28) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001548A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00154A70);

extern s32 func_00155BF0(void **, s32 *, s32);
extern void func_00154BD0();
extern s8 func_00155B58(void *arg0);
extern s32 func_0012E3B0(void **, s32 *, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00154B38(void **arg0) {
    *(s8 *)((char *)arg0 + 0xF) = 0;
    func_0012B7C0(arg0, 3U, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x5A, 1, 0.1f);
    if (func_00155BF0(arg0, &func_00154BD0, 0) != 0) {
        *(s8 *)((char *)arg0 + 0xF) = 1;
        return;
    }
    if (func_00155B58(arg0) != 0) {
        func_0012E3B0(arg0, &func_00154BD0, func_0012E568(0x14, 0x1E));
    }
}

extern s32 func_0012F9C0(void **, s32);
extern s32 func_0012F938(void *, s32);
extern void func_00155820(); extern void func_00154DD8(); extern void func_00154CA8();
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_00154BD0(void **arg0, void *arg1) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        func_0012F9C0(arg0, 3);
        *(s16 *)((char *)arg0 + 0x6E) += 1;
    }
    if (func_0012B7C0(arg0, 3, 9, *(s16 *)((char *)arg0 + 0x6C), 0xA, 0, 0.1f) != 0) {
        if (func_0012F938(arg1, 9) != 0) {
            func_00131140((char *)arg0, func_00155820, func_00154DD8, func_00154DD8, func_00154DD8, func_00154CA8);
        }
        if (*(u16 *)(*(char **)((char *)arg1 + 0xE0) + 0x8C) >= 0x28) {
            func_00130CE0((char *)arg0, 20.0f);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00154CA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00154DD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001550C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001551E8);

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354AF8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354B00);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001554F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00155820);

extern void func_00154CA8(s8 *, void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00155930(s8 *arg0) {
    if (func_0014FF98(arg0, 3, 3) != 0) {
        func_00131000(arg0, func_00154CA8);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00155978);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001559F0(void *arg0) {
    if (*(s32 *)((char *)arg0 + 0x4C) != 0) {
        if (*(u8 *)((char *)arg0 + 0x5A) & 0x80) {
            *(s32 *)((char *)arg0 + 0x4C) = 0;
        }
    }
    func_0012B218(arg0, 1);
    func_0012B218(arg0, 2);
}

extern s32 D_00330330[];
extern f32 D_0035C0E4;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00155A40(void *arg0, void *arg1) {
    func_0012C788(arg1);
    func_0012DDA0(arg1, 4, 3, *(void **)((char *)arg0 + 0x130), D_00330330, D_0035C0E4);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00155A90);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s8 func_00155B58(void *arg0) {
    void *temp_2;

    temp_2 = *(void **)((char *)arg0 + 0x4C);
    if (temp_2 == NULL) {
        return 0;
    }
    return *(s8 *)((char *)temp_2 + 0xF);
}


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00155B78(void *arg0) {
    s32 var_2;
    void *temp_4;

    temp_4 = *(void **)((char *)arg0 + 0x4C);
    var_2 = 0;
    if (temp_4 != NULL) {
        var_2 = func_00131088(temp_4);
    }
    return var_2;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00155BA0);

void func_00155BE0(void) {
}

void func_00155BE8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00155BF0);

extern void func_00155CA8(s8 *, void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00155C98(s8 *arg0) {
    func_00131000(arg0, func_00155CA8);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00155CA8);

extern int func_00130780(char *);
extern void func_00155F50(s8 *);
extern void func_00156178(s8 *, void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00155ED8(s8 *arg0) {
    s32 temp_2;

    *(s8 *)((char *)arg0 + 0xF) = 1;
    temp_2 = func_00130780(arg0);
    if (temp_2 != 0) {
        if (temp_2 >= 0) {
            if (temp_2 < 5) {
                func_00130FE0(arg0, func_00155F50);
            }
        }
    } else {
        func_00130FE0(arg0, func_00156178);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00155F50);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00156178);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00156420);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00156678);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00156890);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00156A10);

extern void func_00156E08(void *);
extern void func_00156CA0(void **arg0);
extern void func_00156F80(void *arg0);
extern void func_00157708(void *arg0, s32 arg1);
extern void func_00156D18(void **);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00156AA0(void *arg0) {
    s32 temp_3;

    temp_3 = *(s32 *)((char *)arg0 + 0x4C);
    if (temp_3 == func_00156E08) {
        return 2;
    }
    if ((temp_3 == func_00156CA0) || (temp_3 == func_00156F80)) {
        func_00157708(arg0, (s32) func_00156D18);
    }
    return 1;
}

extern void func_00155ED8(s8 *arg0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00156AF8(void **arg0) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 0xF) = 0;
        func_00157708(arg0, (s32) func_00156F80);
    }
    if (func_00150070(arg0, 3, 0xB) != 0) {
        func_00131000((s8 *) arg0, func_00155ED8);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00156B60);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00156C88(void *arg0, void *arg1) {
    if (*(s32 *)((char *)arg1 + 8) != arg0) {
        *(s8 *)((char *)arg0 + 0xC) = 1;
    }
}

extern s32 func_0012B540(void *, s32, s8, s32);
extern void func_001573D0(void *, s32, f32 fparg0, f32 fparg1);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00156CA0(void **arg0) {
    func_0012B540(*arg0, 2, 0, 0);
    func_001573D0(arg0, 0x17, 0.0f, 4.0f);
    func_001573D0(arg0, 0x18, 0.0f, 4.0f);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00156D18);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00156E08);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00156F28(void **arg0) {
    if (func_0012B890(arg0, 2, 0, *(s32 *)((char *)arg0 + 0x28), 0x3C, 0.08f, 0) != 0) {
        func_00157708(arg0, (s32) func_00156CA0);
    }
}

extern void func_00156AF8(void **arg0);
extern s32 func_0012FA68(void *);
extern void func_00156F28(void **arg0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00156F80(void *arg0) {
    if (*(s8 *)((char *)arg0 + 0xE) == 0) {
        if (func_00131088(arg0) != (s32) &func_00156AF8) {
            func_0012FA68(arg0);
        }
        *(s8 *)((char *)arg0 + 0xE) = (s8) ((u8) *(s8 *)((char *)arg0 + 0xE) + 1);
    }
    if (func_0012B890(arg0, 2, 0x11, *(s32 *)((char *)arg0 + 0x28), 0x28, 0.3f, 0) != 0) {
        func_00157708(arg0, &func_00156F28);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157018);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001570F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157258);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001573D0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00157440(void *arg0) {
    *(s32 *)((char *)arg0 + 0x24) = 0;
    *(s8 *)((char *)arg0 + 0xD) = 0;
    *(s8 *)((char *)arg0 + 0xB) = 0xE;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157458);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001575D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001576C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001576D8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00157708(void *arg0, s32 arg1) {
    *(s32 *)((char *)arg0 + 0x4C) = arg1;
    *(s32 *)((char *)arg0 + 0x28) = 0;
    *(s8 *)((char *)arg0 + 0xE) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157718);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157760);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001577C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001578D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157A50);

extern void func_00157B50(void *arg0, s32 arg1);
extern s32 D_01ADE5B8[];
extern void func_00157B60(void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00157AF0(void) {
    func_00157B50(D_01ADE5B8, (s32) func_00157B60);
}

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00157B08(void **arg0) {
    void *temp_7;

    temp_7 = *arg0;
    ((s32 (*)(s32 *, void **, s32, void *)) *(s32 *)D_01ADE5B8)(D_01ADE5B8, arg0, *(s32 *)((char *)temp_7 + 0x11C), temp_7);
    *(s32 *)((char *)D_01ADE5B8 + 8) = (s32) (*(s32 *)((char *)D_01ADE5B8 + 8) + 1);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00157B50(void *arg0, s32 arg1) {
    *(s32 *)arg0 = arg1;
    *(s32 *)((char *)arg0 + 4) = 0;
    *(s32 *)((char *)arg0 + 8) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157B60);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157C58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157CF0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157E40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157F38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00157FD8);

extern s32 D_0035B830;
extern s32 D_0035B834;
extern void func_0012F5F8(u16, s32, s32, s32 *, s32, s32, s32, s32);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00158130(char *arg0, char *arg1) {
    if (*(s32 *)(arg0 + 4) == 0) {
        *(s32 *)(arg0 + 0xC) = 0;
        *(s32 *)(arg0 + 4) = 1;
    }
    func_0012F5F8(D_0035B830 & D_0035B834, 0x20, 0x80, (s32 *)(arg0 + 0xC), 1, 0, 0xA, 1);
    if (*(s32 *)(arg0 + 0xC) < *(s32 *)(arg0 + 8) && *(s32 *)(arg0 + 0xC) != 0xA) {
        *(u16 *)(arg1 + 0x7A) = 0;
        *(s32 *)(arg0 + 8) = 0;
    } else {
        *(u16 *)(arg1 + 0x7A) = 0xFFFF;
    }
    if (D_0035B830 & D_0035B834 & 0x4000) {
        func_00157B50(arg0, (s32)func_00157B60);
    }
}

extern void func_001C9B18(s32, void *);
extern s32 func_001321D8(void *);
void func_001583F0(void);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00158200(s32 arg0, void *arg1) {
    void *temp_5;
    void *temp_5_2;
    void *temp_5_3;

    *(s16 *)((char *)arg1 + 4) = 0x17;
    temp_5 = *(void **)arg1;
    func_001C9B18(*(s32 *)((char *)temp_5 + 0xFC), temp_5 + 0x90);
    temp_5_2 = *(void **)arg1;
    func_001C9B18(*(s32 *)((char *)temp_5_2 + 0xFC) + 0x10, temp_5_2 + 0xA0);
    temp_5_3 = *(void **)arg1;
    func_001C9B18(*(s32 *)((char *)temp_5_3 + 0xFC) + 0x20, temp_5_3 + 0xB0);
    func_001321D8(arg1);
    func_00157B50(arg0, func_001583F0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158280);

void func_001583F0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001583F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158480);

void func_00158590(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00158598(void *arg0) {
    *(s32 *)((char *)arg0 + 0x30) = (s32) *(s32 *)((char *)arg0 + 0xC);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001585A8(void *arg0) {
    *(s32 *)((char *)arg0 + 0x30) = (s32) *(s32 *)((char *)arg0 + 0xC);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001585B8(void *arg0) {
    *(s32 *)((char *)arg0 + 0x30) = (s32) *(s32 *)((char *)arg0 + 0xC);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001585C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158680);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158788);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158890);

extern s8 D_00205B70[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00158988(void) {
    s32 var_3;
    s8 *var_2;

    var_3 = 0x1F;
    var_2 = D_00205B70;
    do {
        *var_2 = 0;
        var_3 -= 1;
        var_2 += 0x40;
    } while (var_3 >= 0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001589C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158AD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158C38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158C70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158CF0);

void func_00158D38(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158D40);

typedef struct { f32 v[4]; } __attribute__((aligned(16))) Vec4A;
extern Vec4A D_00354D70;
/* ER-098: by hand from the m2c draft (recipes 2.20); D_00354D70 copied as a 16-byte aligned float vector (ld / sd).
   Names, types and offsets are Unconfirmed readings. */
void func_00158DC0(s32 arg0) {
    Vec4A v;
    v = D_00354D70;
    func_001C9B18(arg0 + 0x40, &v);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158DF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158F18);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00158FC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00159070);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00159188);

void func_00159230(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00159238);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00159290);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001592E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00159450);

extern f32 func_001E3DE0(f32);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00159578(char *a0) {
    *(s32 *)(a0 + 0xC) = 0x3C;
    *(f32 *)(a0 + 0x70) = *(f32 *)(a0 + 0x30);
    *(f32 *)(a0 + 0x74) = *(f32 *)(a0 + 0x34);
    *(f32 *)(a0 + 0x78) = *(f32 *)(a0 + 0x28);
    *(f32 *)(a0 + 0x7C) = func_001E3DE0(*(f32 *)(a0 + 0x20));
    *(f32 *)(a0 + 0x80) = func_001E3EC8(*(f32 *)(a0 + 0x20));
    *(f32 *)(a0 + 0x84) = func_001E3DE0(*(f32 *)(a0 + 0x24));
    *(f32 *)(a0 + 0x88) = func_001E3EC8(*(f32 *)(a0 + 0x24));
    *(f32 *)(a0 + 0x30) = *(f32 *)(a0 + 0x34) = *(f32 *)(a0 + 0x38);
    *(s8 *)(a0 + 2) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00159600);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00159748);

void func_001597C8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001597D0);

void func_00159800(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00159808(void *arg0) {
    *(s8 *)((char *)arg0 + 2) = 0;
    *(s32 *)((char *)arg0 + 0xC) = (s32) *(s32 *)((char *)arg0 + 0x50);
    *(f32 *)((char *)arg0 + 0x20) = (f32) -*(f32 *)((char *)arg0 + 0x20);
}

void func_00159828(void) {
}

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_00159830(char *a0) {
    f32 d;
    *(s8 *)(a0 + 2) = 0;
    *(s32 *)(a0 + 0xC) = *(s32 *)(a0 + 0x50);
    d = (f32)(*(s32 *)(a0 + 0x50) - *(s32 *)(a0 + 0x54));
    *(f32 *)(a0 + 0x84) = (*(f32 *)(a0 + 0x84) - *(f32 *)(a0 + 0x40)) / d;
    *(f32 *)(a0 + 0x88) = (*(f32 *)(a0 + 0x88) - *(f32 *)(a0 + 0x44)) / d;
    *(f32 *)(a0 + 0x8C) = (*(f32 *)(a0 + 0x8C) - *(f32 *)(a0 + 0x48)) / d;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001598A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00159B20);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00159C70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00159E88);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00159F58);

extern void func_00162D80(void *);
extern s32 func_00162D40(void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015A0F8(void *arg0) {
    s32 temp_2;

    *(s32 *)((char *)arg0 + 0xC) = (s32) *(s32 *)((char *)arg0 + 0x50);
    *(s8 *)((char *)arg0 + 2) = 1;
    func_00162D80(*(void **)((char *)arg0 + 4));
    temp_2 = func_00162D40(*(void **)((char *)arg0 + 4));
    *(s32 *)((char *)arg0 + 0x58) = temp_2;
    if (temp_2 == 0x270F) {
        *(s16 *)arg0 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015A148);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015A228(void *arg0) {
    f32 temp_f3;
    s32 temp_3;

    temp_3 = *(s32 *)((char *)arg0 + 0x50);
    temp_f3 = (f32) (temp_3 - *(s32 *)((char *)arg0 + 0x54));
    *(s8 *)((char *)arg0 + 2) = 1;
    *(s32 *)((char *)arg0 + 0xC) = temp_3;
    *(s32 *)((char *)arg0 + 0x34) = 0;
    *(f32 *)((char *)arg0 + 0x84) = (f32) ((*(f32 *)((char *)arg0 + 0x84) - *(f32 *)((char *)arg0 + 0x40)) / temp_f3);
    *(f32 *)((char *)arg0 + 0x88) = (f32) ((*(f32 *)((char *)arg0 + 0x88) - *(f32 *)((char *)arg0 + 0x44)) / temp_f3);
    *(f32 *)((char *)arg0 + 0x8C) = (f32) ((*(f32 *)((char *)arg0 + 0x8C) - *(f32 *)((char *)arg0 + 0x48)) / temp_f3);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015A2A8);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_0015A440(char *arg0) {
    s32 n;
    f32 v;
    f32 d;

    n = *(s32 *)(arg0 + 0x50);
    v = *(f32 *)(arg0 + 0x30);
    d = (f32)(n - *(s32 *)(arg0 + 0x54));
    *(s8 *)(arg0 + 2) = 1;
    *(s32 *)(arg0 + 0xC) = n;
    *(f32 *)(arg0 + 0x70) = (*(f32 *)(arg0 + 0x34) - v) / (f32)n;
    *(f32 *)(arg0 + 0x34) = v;
    *(f32 *)(arg0 + 0x38) = v;
    *(f32 *)(arg0 + 0x4C) = 128.0f;
    *(f32 *)(arg0 + 0x84) = (*(f32 *)(arg0 + 0x84) - *(f32 *)(arg0 + 0x40)) / d;
    *(f32 *)(arg0 + 0x88) = (*(f32 *)(arg0 + 0x88) - *(f32 *)(arg0 + 0x44)) / d;
    *(f32 *)(arg0 + 0x8C) = (*(f32 *)(arg0 + 0x8C) - *(f32 *)(arg0 + 0x48)) / d;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015A4F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015A6E0);

void func_0015A7F8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015A800);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015A8B8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015AB28(void *arg0) {
    *(s32 *)((char *)arg0 + 0xC) = 2;
    *(s8 *)((char *)arg0 + 2) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015AB40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015ACF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015AE00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015B070);

void func_0015B178(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015B180);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015B250);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015B638(void *arg0) {
    *(s8 *)((char *)arg0 + 2) = 1;
    *(s32 *)((char *)arg0 + 0xC) = (s32) *(s32 *)((char *)arg0 + 0x50);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015B650);

void func_0015B7D0(void) {
}

void func_0015B7D8(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015B7E0(void *arg0) {
    *(s8 *)((char *)arg0 + 2) = 1;
    *(s32 *)((char *)arg0 + 0xC) = (s32) *(s32 *)((char *)arg0 + 0x50);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015B7F8);

void func_0015B8C8(void) {
}

void func_0015B8D0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015B8D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015BA28);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015BAA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015BAE0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015BE00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015C020);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015C0E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015C510);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015C5E0(void *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f1;
    f32 temp_f6;
    f32 temp_f6_2;

    temp_f0 = *(f32 *)((char *)arg0 + 0x78);
    temp_f6 = (f32) *(s32 *)((char *)arg0 + 0x5C);
    temp_f1 = *(f32 *)((char *)arg0 + 0x14) - temp_f0;
    *(f32 *)((char *)arg0 + 0x78) = (f32) (temp_f0 + 0.88199997f);
    *(f32 *)((char *)arg0 + 0x14) = temp_f1;
    *(f32 *)((char *)arg0 + 0x10) = (f32) (*(f32 *)((char *)arg0 + 0x10) - *(f32 *)((char *)arg0 + 0x74));
    *(f32 *)((char *)arg0 + 0x18) = (f32) (*(f32 *)((char *)arg0 + 0x18) - *(f32 *)((char *)arg0 + 0x7C));
    if (temp_f1 < temp_f6) {
        *(f32 *)((char *)arg0 + 0x14) = temp_f6;
        *(s32 *)((char *)arg0 + 0xC) = 1;
    }
    temp_f6_2 = *(f32 *)((char *)arg0 + 0x40) - 2.5f;
    *(f32 *)((char *)arg0 + 0x30) = (f32) (*(f32 *)((char *)arg0 + 0x30) * 0.95f);
    *(f32 *)((char *)arg0 + 0x34) = (f32) (*(f32 *)((char *)arg0 + 0x34) * 0.9f);
    *(f32 *)((char *)arg0 + 0x38) = (f32) (*(f32 *)((char *)arg0 + 0x38) * 0.95f);
    if (temp_f6_2 > 16.0f) {
        *(f32 *)((char *)arg0 + 0x40) = temp_f6_2;
    }
    temp_f0_2 = *(f32 *)((char *)arg0 + 0x44) - 2.5f;
    if (temp_f0_2 > 16.0f) {
        *(f32 *)((char *)arg0 + 0x44) = temp_f0_2;
    }
    temp_f0_3 = *(f32 *)((char *)arg0 + 0x48) - 2.5f;
    if (temp_f0_3 > 16.0f) {
        *(f32 *)((char *)arg0 + 0x48) = temp_f0_3;
    }
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015C6C0(void *arg0) {
    f32 temp_f1;
    u32 temp_3;

    temp_3 = *(u32 *)((char *)arg0 + 0x50);
    temp_f1 = *(f32 *)((char *)arg0 + 0x70);
    *(s8 *)((char *)arg0 + 2) = 0;
    *(f32 *)((char *)arg0 + 0x78) = temp_f1;
    *(u32 *)((char *)arg0 + 0xC) = temp_3;
    *(f32 *)((char *)arg0 + 0x7C) = (f32) (temp_f1 / (f32) ((s32) (temp_3 + (temp_3 >> 0x1F)) >> 1));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015C700);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015C740);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015C818);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015CB00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015CBF8);

extern float func_001E41B8(float, float);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015CDB8(void *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f1;
    f32 temp_f20;

    temp_f1 = *(f32 *)((char *)arg0 + 0x74);
    temp_f0 = *(f32 *)((char *)arg0 + 0x7C);
    temp_f20 = sqrtf((temp_f1 * temp_f1) + (temp_f0 * temp_f0));
    *(f32 *)((char *)arg0 + 0x24) = func_001E41B8(*(f32 *)((char *)arg0 + 0x74), *(f32 *)((char *)arg0 + 0x7C));
    temp_f0_2 = func_001E41B8(-*(f32 *)((char *)arg0 + 0x78), temp_f20);
    *(f32 *)((char *)arg0 + 0x20) = temp_f0_2;
    if (!(*(s32 *)((char *)arg0 + 0x6C) & 0x100)) {
        *(f32 *)((char *)arg0 + 0x20) = (f32) (temp_f0_2 + 1.5707964f);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015CE60);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015CFA8(void *arg0) {
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f20_3;

    *(s32 *)((char *)arg0 + 0x28) = 0;
    temp_f20 = *(f32 *)((char *)arg0 + 0x70) * func_001E3EC8(*(f32 *)((char *)arg0 + 0x24));
    *(f32 *)((char *)arg0 + 0x74) = (f32) (temp_f20 * func_001E3DE0(*(f32 *)((char *)arg0 + 0x20)));
    *(f32 *)((char *)arg0 + 0x78) = (f32) (*(f32 *)((char *)arg0 + 0x70) * func_001E3EC8(*(f32 *)((char *)arg0 + 0x20)));
    temp_f20_2 = *(f32 *)((char *)arg0 + 0x70) * func_001E3DE0(*(f32 *)((char *)arg0 + 0x24));
    temp_f20_3 = temp_f20_2 * func_001E3DE0(*(f32 *)((char *)arg0 + 0x20));
    *(s8 *)((char *)arg0 + 2) = 1;
    *(s32 *)((char *)arg0 + 0xC) = (s32) *(s32 *)((char *)arg0 + 0x50);
    *(f32 *)((char *)arg0 + 0x7C) = temp_f20_3;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015D038);

extern s32 D_00248880[];

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015D210(void *arg0) {
    *(s32 *)((char *)arg0 + 0xC) = 2;
    *(s8 *)((char *)arg0 + 2) = 1;
    *(u8 *)((char *)arg0 + 3) = (u8) (*(u8 *)((char *)arg0 + 3) & (*D_00248880 | ~1));
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015D240(void *arg0) {
    *(s32 *)((char *)arg0 + 0xC) = 2;
}

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015D250(void *arg0) {
    *(s8 *)((char *)arg0 + 2) = 0;
    *(s32 *)((char *)arg0 + 0xC) = 2;
    *(u8 *)((char *)arg0 + 3) = (u8) (*(u8 *)((char *)arg0 + 3) & (*D_00248880 | ~1));
}

void func_0015D280(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015D288);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015D2E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015D388);

void func_0015D3E0(void) {
}

extern f32 func_001E3EC8(f32); extern f32 func_001E3DE0(f32);
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_0015D3E8(char *a0) {
    f32 t, t2, t3;
    *(s32 *)(a0 + 0xC) = *(s32 *)(a0 + 0x50);
    t = *(f32 *)(a0 + 0x70) * func_001E3EC8(*(f32 *)(a0 + 0x24));
    *(f32 *)(a0 + 0x74) = t * func_001E3DE0(*(f32 *)(a0 + 0x20));
    *(f32 *)(a0 + 0x78) = *(f32 *)(a0 + 0x70) * func_001E3EC8(*(f32 *)(a0 + 0x20));
    t2 = *(f32 *)(a0 + 0x70) * func_001E3DE0(*(f32 *)(a0 + 0x24));
    t3 = t2 * func_001E3DE0(*(f32 *)(a0 + 0x20));
    *(s8 *)(a0 + 2) = 1;
    *(f32 *)(a0 + 0x84) = *(f32 *)(a0 + 0x4C);
    *(s32 *)(a0 + 0x4C) = 0;
    *(f32 *)(a0 + 0x7C) = t3;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015D480);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015D600(void *arg0) {
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f20_3;

    temp_f20 = *(f32 *)((char *)arg0 + 0x70) * func_001E3DE0(*(f32 *)((char *)arg0 + 0x20));
    *(f32 *)((char *)arg0 + 0x10) = (f32) (*(f32 *)((char *)arg0 + 0x74) + (temp_f20 * func_001E3EC8(*(f32 *)((char *)arg0 + 0x24))));
    *(f32 *)((char *)arg0 + 0x14) = (f32) (*(f32 *)((char *)arg0 + 0x78) + (*(f32 *)((char *)arg0 + 0x70) * func_001E3EC8(*(f32 *)((char *)arg0 + 0x20))));
    temp_f20_2 = *(f32 *)((char *)arg0 + 0x70) * func_001E3DE0(*(f32 *)((char *)arg0 + 0x20));
    temp_f20_3 = temp_f20_2 * func_001E3DE0(*(f32 *)((char *)arg0 + 0x24));
    *(s8 *)((char *)arg0 + 2) = 1;
    *(f32 *)((char *)arg0 + 0x18) = (f32) (*(f32 *)((char *)arg0 + 0x7C) + temp_f20_3);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015D6A0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015D7C0(void *arg0) {
    f32 temp_f2;

    *(s32 *)((char *)arg0 + 0xC) = (s32) *(s32 *)((char *)arg0 + 0x50);
    *(f32 *)((char *)arg0 + 0x10) = (f32) (*(f32 *)((char *)arg0 + 0x74) + (*(f32 *)((char *)arg0 + 0x70) * func_001E3EC8(*(f32 *)((char *)arg0 + 0x24))));
    temp_f2 = *(f32 *)((char *)arg0 + 0x70) * func_001E3EC8(*(f32 *)((char *)arg0 + 0x24));
    *(s8 *)((char *)arg0 + 2) = 1;
    *(f32 *)((char *)arg0 + 0x18) = (f32) (*(f32 *)((char *)arg0 + 0x78) + temp_f2);
    *(f32 *)((char *)arg0 + 0x8C) = (f32) ((*(f32 *)((char *)arg0 + 0x8C) - *(f32 *)((char *)arg0 + 0x88)) / (f32) (*(s32 *)((char *)arg0 + 0x50) - *(s32 *)((char *)arg0 + 0x58)));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015D858);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015DA68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015DB38);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_0015DDB8(char *arg0) {
    *(s8 *)(arg0 + 2) = 0;
    *(s32 *)(arg0 + 0xC) = *(s32 *)(arg0 + 0x50);
    *(f32 *)(arg0 + 0x80) = *(f32 *)(arg0 + 0x20);
    *(f32 *)(arg0 + 0x20) = -*(f32 *)(arg0 + 0x80);
    *(s32 *)(arg0 + 0x54) += 1;
    *(s32 *)(arg0 + 0x58) %= 3;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015DE00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015E118);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015E2C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015E4B8);

extern void func_0015E968(void *);
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_0015E628(char *a0) {
    f32 d; f32 h; s32 t;
    t = *(s32 *)(a0 + 0x50);
    h = *(f32 *)(a0 + 0x30);
    d = (f32)(t - *(s32 *)(a0 + 0x54));
    *(s32 *)(a0 + 0xC) = t;
    *(f32 *)(a0 + 0x7C) = (*(f32 *)(a0 + 0x34) - h) / (f32)t;
    *(f32 *)(a0 + 0x3C) = 1.0f;
    *(f32 *)(a0 + 0x34) = h;
    *(f32 *)(a0 + 0x38) = h;
    *(f32 *)(a0 + 0x84) = (*(f32 *)(a0 + 0x84) - *(f32 *)(a0 + 0x40)) / d;
    *(s32 *)(a0 + 0x68) = (s32)((*(f32 *)(a0 + 0x24) * 4096.0f) / 6.2831855f) & 0xFFF;
    *(f32 *)(a0 + 0x88) = (*(f32 *)(a0 + 0x88) - *(f32 *)(a0 + 0x44)) / d;
    *(f32 *)(a0 + 0x8C) = (*(f32 *)(a0 + 0x8C) - *(f32 *)(a0 + 0x48)) / d;
    func_0015E968(a0);
    *(s8 *)(a0 + 2) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015E728);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015E968(void *arg0) {
    *(f32 *)((char *)arg0 + 0x14) = (f32) ((f32) *(s32 *)((char *)arg0 + 0x58) - (*(f32 *)((char *)arg0 + 0x80) * *(f32 *)((char *)arg0 + 0x34)));
}

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_0015E988(char *a0) {
    f32 d; f32 h; s32 t;
    t = *(s32 *)(a0 + 0x50);
    h = *(f32 *)(a0 + 0x30);
    d = (f32)(t - *(s32 *)(a0 + 0x54));
    *(s32 *)(a0 + 0xC) = t;
    *(f32 *)(a0 + 0x7C) = (*(f32 *)(a0 + 0x34) - h) / (f32)t;
    *(f32 *)(a0 + 0x3C) = 1.0f;
    *(f32 *)(a0 + 0x34) = h;
    *(f32 *)(a0 + 0x38) = h;
    *(f32 *)(a0 + 0x84) = (*(f32 *)(a0 + 0x84) - *(f32 *)(a0 + 0x40)) / d;
    *(s32 *)(a0 + 0x68) = (s32)((*(f32 *)(a0 + 0x24) * 4096.0f) / 6.2831855f) & 0xFFF;
    *(f32 *)(a0 + 0x88) = (*(f32 *)(a0 + 0x88) - *(f32 *)(a0 + 0x44)) / d;
    *(f32 *)(a0 + 0x8C) = (*(f32 *)(a0 + 0x8C) - *(f32 *)(a0 + 0x48)) / d;
    func_0015E968(a0);
    *(s8 *)(a0 + 2) = 0;
}

void func_0015EA88(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015EA90);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015EB70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015ED58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015EF70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015F000);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015F1A0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015F2B0);

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354B88);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354B90);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354BA0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354BB0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354BC0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354BD0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354BE0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354BF0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354C00);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354C10);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354C20);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354C30);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354C40);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354C50);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354C60);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354C70);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354C80);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354C90);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354CA0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354CB0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354CC0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354CD0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354CE0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354CF0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354D00);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354D10);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354D20);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354D30);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354D40);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354D50);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354D60);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354D70);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354D80);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354D90);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354DA0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354DB0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354DC0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354DC8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354DD0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354DE0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354DE8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354DF0);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015F408);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0015F868(void *arg0) {
    *(s32 *)((char *)arg0 + 0xC) = 0x96;
    *(s8 *)((char *)arg0 + 2) = 1;
    *(s32 *)((char *)arg0 + 0x50) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015F880);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015F958);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0015FD28);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_001602A0(char *arg0) {
    f32 t;
    f32 t2;

    *(s32 *)(arg0 + 0xC) = *(s32 *)(arg0 + 0x50);
    t = *(f32 *)(arg0 + 0x70) * func_001E3EC8(*(f32 *)(arg0 + 0x24));
    *(f32 *)(arg0 + 0x74) = t * func_001E3DE0(*(f32 *)(arg0 + 0x20));
    *(f32 *)(arg0 + 0x78) = *(f32 *)(arg0 + 0x70) * func_001E3EC8(*(f32 *)(arg0 + 0x20));
    t2 = *(f32 *)(arg0 + 0x70) * func_001E3DE0(*(f32 *)(arg0 + 0x24));
    *(f32 *)(arg0 + 0x7C) = t2 * func_001E3DE0(*(f32 *)(arg0 + 0x20));
    *(s8 *)(arg0 + 2) = 1;
    *(u8 *)(arg0 + 3) |= 4;
    *(f32 *)(arg0 + 0x84) = *(f32 *)(arg0 + 0x4C);
    *(s32 *)(arg0 + 0x4C) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00160348);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_001604C8(char *arg0) {
    *(s8 *)(arg0 + 2) = 0;
    *(s32 *)(arg0 + 0xC) = *(s32 *)(arg0 + 0x50) + 0x1E;
    *(f32 *)(arg0 + 0x80) = *(f32 *)(arg0 + 0x20);
    *(f32 *)(arg0 + 0x20) = -*(f32 *)(arg0 + 0x80);
    *(s32 *)(arg0 + 0x54) += 1;
    *(s32 *)(arg0 + 0x58) %= 3;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00160518);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001608F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00160AE8);

extern void func_001E9B0C(void *, s32, s32);
extern float func_00111480(float *, char *);
extern void func_001CA018(void *, void *, void *);
extern s32 func_001E9C70();

/* ER-096: by hand from ER-080's draft; the two sin products are separate temporaries (one shared temporary swaps
   $f21 / $f22). Names, types and offsets are Unconfirmed readings. */
void func_00160CE0(char *a0, char *a1) {
    float v[4];
    float r;
    float a;
    float b;
    float e;
    float t;
    float t2;

    func_001E9B0C(v, 0, 0x10);
    v[0] = *(float *)(a1 + 4);
    v[3] = 1.0f;
    v[1] = *(float *)(a1 + 8);
    v[2] = *(float *)(a1 + 0xC);
    r = func_00111480(v, a0 + 0x10) * 0.75;
    func_001CA018(v, v, a0 + 0x10);
    a = func_001E41B8(-v[0], -v[2]);
    do {
        a += 0.5235988f - (float)(func_001E9C70() & 0xFFFF) * (1.0f / 65536.0f) * 1.0471976f;
        b = a - ((3.1415927f < a) ? 6.2831855f : ((a < -3.1415927f) ? -6.2831855f : 0.0f));
        a = b;
    } while (-0.7853982f < b || b < -2.3561945f);
    e = (float)(func_001E9C70() & 0xFFFF) * (1.0f / 65536.0f) * 6.2831855f + -3.1415927f;
    t = r * func_001E3DE0(b);
    *(float *)(a1 + 4) = *(float *)(a0 + 0x10) - t * func_001E3EC8(e);
    *(float *)(a1 + 8) = *(float *)(a0 + 0x14) - r * func_001E3EC8(b);
    t2 = r * func_001E3DE0(b);
    *(float *)(a1 + 0xC) = *(float *)(a0 + 0x18) - t2 * func_001E3DE0(e);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00160F28);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00161010(void *arg0) {
    if ((u32) (*(u32 *)((char *)arg0 + 0x50) + *(s32 *)((char *)arg0 + 0x54)) < (u32) *(u32 *)((char *)arg0 + 0xC)) {
        *(f32 *)((char *)arg0 + 0x10) = (f32) (*(f32 *)((char *)arg0 + 0x10) - *(f32 *)((char *)arg0 + 0x74));
        *(f32 *)((char *)arg0 + 0x14) = (f32) (*(f32 *)((char *)arg0 + 0x14) - *(f32 *)((char *)arg0 + 0x78));
        *(f32 *)((char *)arg0 + 0x18) = (f32) (*(f32 *)((char *)arg0 + 0x18) - *(f32 *)((char *)arg0 + 0x7C));
    }
    if ((u32) *(u32 *)((char *)arg0 + 0xC) < (u32) *(u32 *)((char *)arg0 + 0x50)) {
        *(f32 *)((char *)arg0 + 0x10) = (f32) (*(f32 *)((char *)arg0 + 0x10) + *(f32 *)((char *)arg0 + 0x74));
        *(f32 *)((char *)arg0 + 0x14) = (f32) (*(f32 *)((char *)arg0 + 0x14) + *(f32 *)((char *)arg0 + 0x78));
        *(f32 *)((char *)arg0 + 0x18) = (f32) (*(f32 *)((char *)arg0 + 0x18) + *(f32 *)((char *)arg0 + 0x7C));
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001610B0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00161160(void *arg0) {
    s32 temp_f1;

    temp_f1 = (s32) (*(f32 *)((char *)arg0 + 0x30) * 128.0f);
    *(f32 *)((char *)arg0 + 0x20) = (f32) (*(f32 *)((char *)arg0 + 0x10) + (f32) (temp_f1 * 0x10));
    *(f32 *)((char *)arg0 + 0x24) = (f32) (*(f32 *)((char *)arg0 + 0x14) + (f32) (temp_f1 * 8));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001611B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00161328);

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00161568(void *arg0) {
    f32 temp_f0;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 temp_f8;

    *(s8 *)((char *)arg0 + 2) = 1;
    *(s32 *)((char *)arg0 + 0xC) = (s32) *(s32 *)((char *)arg0 + 0x58);
    *(u8 *)((char *)arg0 + 3) = (u8) (*(u8 *)((char *)arg0 + 3) | 0x20);
    func_001C9B18(arg0 + 0x20, arg0 + 0x10);
    temp_f1 = *(f32 *)((char *)arg0 + 0x30);
    temp_f1_2 = (f32) (*(s32 *)((char *)arg0 + 0x58) - *(s32 *)((char *)arg0 + 0x5C));
    temp_f8 = *(f32 *)((char *)arg0 + 0x10);
    temp_f0 = (f32) ((s32) (temp_f1 * 128.0f) * 0x10);
    *(f32 *)((char *)arg0 + 0x34) = (f32) ((*(f32 *)((char *)arg0 + 0x34) - temp_f1) / temp_f1_2);
    *(f32 *)((char *)arg0 + 0x24) = (f32) (*(f32 *)((char *)arg0 + 0x14) + temp_f0);
    *(f32 *)((char *)arg0 + 0x20) = (f32) (temp_f8 + temp_f0);
    *(f32 *)((char *)arg0 + 0x84) = (f32) ((*(f32 *)((char *)arg0 + 0x84) - *(f32 *)((char *)arg0 + 0x40)) / temp_f1_2);
    *(f32 *)((char *)arg0 + 0x88) = (f32) ((*(f32 *)((char *)arg0 + 0x88) - *(f32 *)((char *)arg0 + 0x44)) / temp_f1_2);
    *(f32 *)((char *)arg0 + 0x8C) = (f32) ((*(f32 *)((char *)arg0 + 0x8C) - *(f32 *)((char *)arg0 + 0x48)) / temp_f1_2);
    if (*(s32 *)((char *)arg0 + 0x68) != 0) {
        *(s32 *)((char *)arg0 + 0x68) = (s32) temp_f8;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00161680);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00161898);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00161968);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00161BC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00161CB8);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00161E00(void *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f1;
    f32 temp_f20;

    temp_f1 = *(f32 *)((char *)arg0 + 0x74);
    temp_f0 = *(f32 *)((char *)arg0 + 0x7C);
    temp_f20 = sqrtf((temp_f1 * temp_f1) + (temp_f0 * temp_f0));
    *(f32 *)((char *)arg0 + 0x24) = func_001E41B8(*(f32 *)((char *)arg0 + 0x74), *(f32 *)((char *)arg0 + 0x7C));
    temp_f0_2 = func_001E41B8(-*(f32 *)((char *)arg0 + 0x78), temp_f20);
    *(f32 *)((char *)arg0 + 0x20) = temp_f0_2;
    if (!(*(s32 *)((char *)arg0 + 0x6C) & 0x100)) {
        *(f32 *)((char *)arg0 + 0x20) = (f32) (temp_f0_2 + 1.5707964f);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00161EA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00161FC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00162108);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001621A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001621E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001624A0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00162698);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001627A0);

void func_00162940(void) {
}

void func_00162948(void) {
}

void func_00162950(void) {
}

void func_00162958(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00162960);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00162A40);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00162BF8(void *arg0, void *arg1, f32 *arg2, f32 *arg3) {
    f32 temp_f20;
    f32 temp_f21;
    f32 temp_f22;

    temp_f21 = *(f32 *)((char *)arg1 + 8) - *(f32 *)((char *)arg0 + 8);
    temp_f20 = *(f32 *)arg1 - *(f32 *)arg0;
    temp_f22 = -(*(f32 *)((char *)arg1 + 4) - *(f32 *)((char *)arg0 + 4));
    *arg3 = func_001E41B8(-temp_f20, -temp_f21);
    *arg2 = func_001E41B8(temp_f22, sqrtf((temp_f20 * temp_f20) + (temp_f21 * temp_f21)));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00162CB8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00162D40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00162D80);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00162E28);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00163158);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00163300(void *arg0, s8 *arg1) {
    *(f32 *)((char *)arg0 + 0x24) = func_001E41B8(-(*(f32 *)arg1 - *(f32 *)((char *)arg0 + 0x10)), -(*(f32 *)((char *)arg1 + 8) - *(f32 *)((char *)arg0 + 0x18)));
    *(f32 *)((char *)arg0 + 0x80) = func_001114E8(arg1, arg0 + 0x10);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00163368);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001634A0(void *arg0) {
    *(f32 *)((char *)arg0 + 0x10) = (f32) (*(f32 *)((char *)arg0 + 0x10) - *(f32 *)((char *)arg0 + 0x74));
    *(f32 *)((char *)arg0 + 0x14) = (f32) (*(f32 *)((char *)arg0 + 0x14) - *(f32 *)((char *)arg0 + 0x78));
    *(f32 *)((char *)arg0 + 0x18) = (f32) (*(f32 *)((char *)arg0 + 0x18) - *(f32 *)((char *)arg0 + 0x7C));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001634D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001635E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001635F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001636A0);

extern s32 *D_0035B848;
extern void *D_0035C4A8;
extern s32 D_001FF890[];
extern s32 func_001CC0C8(s32, s32, s32, s32, s32, s32);
extern s32 func_001CBA08(void);
extern void func_001CC848(s32, s32);
extern s32 func_001CC988(s32);
extern void func_001CCAB0(s32);
extern s32 func_001D03E0(s32);
extern s32 func_001CEBE8(s32);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00163790(void) {
    char *t;
    char *g;

    t = *(char **)D_0035B848;
    *(s32 *)((char *)D_0035C4A8 + 0x34) = func_001CC0C8(5, *(s32 *)(t + 0x128), *(s32 *)(t + 0x148), *(s32 *)(t + 0x12C), *(s32 *)(t + 0x14C), 0);
    while (func_001CBA08() == 0) {
    }
    if (*(s32 *)((char *)D_0035C4A8 + 0x34) < 0) {
        g = (char *)D_001FF890;
        g[0x164] = 1;
    }
    func_001CC848(*(s32 *)((char *)D_0035C4A8 + 0x34), 0);
    while (func_001CBA08() == 0) {
    }
    while (func_001CC988(0) == 0) {
    }
    func_001CCAB0(*(s32 *)((char *)D_0035C4A8 + 0x34));
    func_001D03E0(0);
    func_001CEBE8(0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00163840);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00163A58);

extern s32 func_001CC9B0(u32);
extern void func_001649C0(void);
extern s8 D_001FF9F6[];
extern void func_00164960(void);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00163C08(void) {
    s32 temp_16;

    temp_16 = func_001CC9B0(*(u32 *)((char *)D_0035C4A8 + 0x30));
    func_001649C0();
    if (temp_16 != 0) {
        D_001FF9F6[0] = 1;
    }
    func_001D03E0(0);
    func_001CEBE8(0);
    func_00164960();
    func_001CEBE8(0);
}

extern void func_00163C68(void);
extern void func_00164C08(void);

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00163C68(void) {
    if ((*(s32 *)((char *)D_0035C4A8 + 0x60) != -1) && (*(s32 *)((char *)D_0035C4A8 + 0x64) == 0)) {
        *(s32 *)((char *)D_0035C4A8 + 0x64) = 1;
    }
    func_00164C08();
    func_00164960();
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00163CA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00163E50);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00163FC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00164068);

extern s32 func_00163CA8();
extern s32 func_001CB808();
extern s32 D_01B7BA00[];
extern s32 func_001649F8();
extern s32 func_001CDC08();

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001640C8(void) {
    func_00163CA8();
    func_001CB808(1, 0x81020, D_01B7BA00);
    func_001649F8();
    func_001CDC08();
    *(s32 *)((char *)D_0035C4A8 + 0x6C) = (s32) (*(s32 *)((char *)D_0035C4A8 + 0x6C) + 1);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00164120);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00164198);

extern void func_00164120(void);

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00164220(void) {
    if (*(s32 *)((char *)D_0035C4A8 + 0x60) != -1) {
        *(s32 *)((char *)D_0035C4A8 + 0x64) = 1;
    }
    func_00164120();
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00164240);

extern s32 D_0035C4B4;
extern s32 D_0035C4AC;
extern s32 D_0035C4B0;
extern s32 D_0035C4B8;
extern s32 D_0035C4BC;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00164378(s32 arg0, s32 arg1) {
    D_0035C4B4 = arg0;
    D_0035C4AC = arg1;
    D_0035C4B0 = 1;
    D_0035C4B8 = 0;
    D_0035C4BC = 0;
}

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354E18);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354E20);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354E28);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354E30);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00164398);

extern s32 func_001E28D0(s32, s32, s32, s32);
extern void func_00163C08(void);
extern s32 func_001CB750();
extern s32 func_00163FC8();
extern s32 func_001636A0();

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00164760(void) {
    func_001E28D0(1, 0x8010, 0x800, 0xFCC);
    func_001E28D0(1, 0x8010, 0x801, 0xFCF);
    func_001E28D0(1, 0x8010, 0xF80, 0);
    func_001E28D0(1, 0x8010, 0x1080, 0);
    func_001E28D0(1, 0x8010, 0xF81, 0);
    func_001E28D0(1, 0x8010, 0x1081, 0);
    func_001E28D0(1, 0x8010, 0x980, 0x3FFF);
    func_001E28D0(1, 0x8010, 0xA80, 0x3FFF);
    func_001E28D0(1, 0x8010, 0x981, 0x3FFF);
    func_001E28D0(1, 0x8010, 0xA81, 0x3FFF);
    func_00163C08();
    func_001CB750();
    func_00163FC8();
    func_001636A0();
    func_001D03E0(0);
    func_001CEBE8(0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00164868);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00164890);

extern s32 func_001CB808();

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00164928(void) {
    return func_001CB808(1, 0x30B0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00164948);

extern s32 D_0035C1CC;
extern s32 func_001CB808();

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00164960(void) {
    if (D_0035C1CC != 0) {
        func_001CB808(1, 0x3050, 1);
        func_001CB808(1, 0x3030, 1);
        D_0035C1CC = 0;
    }
}

extern s32 *D_0035C4C0;
extern s32 D_00340960[];
extern s32 *D_0035C4C4;
extern s32 D_00342960[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001649A0(void) {
    D_0035C4C0 = D_00340960;
    D_0035C4C4 = D_00342960;
}

extern s8 D_003409E0[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001649C0(void) {
    s8 *var_3;
    u32 var_4;

    var_4 = 0;
    var_3 = D_003409E0;
    do {
        *var_3 = 0;
        var_4 += 1;
        var_3 += 1;
    } while (var_4 < 0x2000U);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001649F8);

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00164C08(void) {
    s32 var_5;
    s8 *var_3;

    var_5 = 0x3E;
    var_3 = D_003409E0;
    do {
        var_5 -= 1;
        if ((*(s8 *)var_3 != 0) && (*(s8 *)((char *)var_3 + 1) != 0x64)) {
            *(s8 *)((char *)var_3 + 1) = 0x64;
            *(s32 *)((char *)var_3 + 0x28) = 0;
        }
        var_3 += 0x80;
    } while (var_5 >= 0);
}

extern s32 func_001CD8D8(s16);

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00164C50(void) {
    s32 var_17;
    s8 *var_16;

    var_17 = 0x3E;
    var_16 = D_003409E0;
    do {
        var_17 -= 1;
        func_001CD8D8(*(s16 *)((char *)var_16 + 0x36));
        *(s8 *)var_16 = 0;
        *(s16 *)((char *)var_16 + 0x30) = 0;
        var_16 += 0x80;
    } while (var_17 >= 0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00164CA0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165018);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165080);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001650E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165128);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165170);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001651C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165218);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165280);

extern s8 D_001FFA26[];
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_001652E8(s32 arg0, f32 a, f32 b, f32 c, f32 d) {
    char *p;

    if (arg0 >= 0x3F) {
        D_001FFA26[0] = 1;
        return;
    }
    p = (char *)D_003409E0 + (arg0 << 7);
    if (*(s8 *)p != 0 && *(s8 *)(p + 1) == 2) {
        *(f32 *)(p + 0x10) = a;
        *(f32 *)(p + 0x14) = b;
        *(f32 *)(p + 0x18) = c;
        *(f32 *)(p + 8) = d;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165340);

extern s32 func_001E9D08();
extern char D_0035C1D8[];
extern void func_00177858(s32, s32, u8 *);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00165388(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 buf[0x80];
    func_001E9D08(buf, D_0035C1D8, arg3, arg2);
    func_00177858(arg0, arg1, buf);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001653E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165420);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165580);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165670);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165B48);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165CA0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165D68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165E08);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165EA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00165F68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00166130);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00166260);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00166370);

extern char D_01ADE810[];
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_001663D0(void) {
    char *s = D_01ADE810;
    s32 i;

    *(s32 *)(s + 4) = 0;
    for (i = 0x3F; i >= 0; i--) {
        ((s16 *)(s + 0x90))[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00166410);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001665C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001666D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00166788);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00166838);

extern void func_001766F8();
/* ER-098: by hand from the m2c draft (recipes 2.20); the 0x80-byte frame is an unused local (ruling 3.13).
   Names, types and offsets are Unconfirmed readings. */
void func_001668A8(s32 arg0) {
    char buf[0x80]; /* ruling 3.13: unused; only sizes the stack frame */
    if (arg0 == 0x7F) {
        func_001766F8(0x12C);
    } else {
        func_001766F8(arg0 + 0x12C);
    }
}

extern s32 D_0035B884;
/* ER-098: by hand from the m2c draft (recipes 2.20); the 0x80-byte frame is an unused local (ruling 3.13).
   Names, types and offsets are Unconfirmed readings. */
void func_001668E0(s32 arg0) {
    char buf[0x80]; /* ruling 3.13: unused; only sizes the stack frame */
    func_001766F8(arg0 + 0x10, D_0035B884);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00166900);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00166A60);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00166AE8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00166BE8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00166CD0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001671A0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001672D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00167410);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001676C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00167858);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00167998);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00167B20);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001680F0);

extern void func_00168328(s32, s32, s32);
extern s32 D_01ADE948[];
extern s32 D_0035C504;
extern s32 D_0035C508;

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00168328(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)D_01ADE948 = (s32) D_0035C504;
    *(s32 *)((char *)D_01ADE948 + 4) = (s32) D_0035C508;
    *(s32 *)((char *)D_01ADE948 + 0x1C) = arg2;
    *(s32 *)((char *)D_01ADE948 + 8) = arg0;
    *(s32 *)((char *)D_01ADE948 + 0xC) = arg1;
    *(s32 *)((char *)D_01ADE948 + 0x18) = 0;
}

extern s32 D_01ADE948[]; extern s32 D_0035C504; extern s32 D_0035C508; extern char D_00342E80[];
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_00168358(s32 arg0, s32 arg1, s32 arg2) {
    char *t;
    *(s32 *)D_01ADE948 = D_0035C504;
    t = D_00342E80 + arg0 * 0x2C8;
    *(s32 *)((char *)D_01ADE948 + 4) = D_0035C508;
    *(s32 *)((char *)D_01ADE948 + 0x1C) = arg2;
    *(s32 *)((char *)D_01ADE948 + 8) = *(s16 *)(t + 8) - 0x10;
    *(s32 *)((char *)D_01ADE948 + 0xC) = *(s16 *)(t + 0xA) + arg1 * 0x10;
    *(s32 *)((char *)D_01ADE948 + 0x18) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001683B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001686A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001689C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00168CD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00168E28);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00169020);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
s32 func_00169480(s32 arg0) {
    return (arg0 > 0) ? 0 : ((arg0 == 0) ? 1 : 2);
}

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354EA0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354EC0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354ED0);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001694A0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00169D28);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00169E80);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00169FC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016A808);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016AB80);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016ACF0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016AE58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016AFB8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016B438);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016B5D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016B5E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016B6C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016B6F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016BC38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016BFB0);

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354FD0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354FE0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00354FF8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355020);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016C2D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016C760);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016C9A0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016C9F0);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0016CA38(void *arg0) {
    s32 temp_17;

    *(s16 *)((char *)arg0 + 6) = 0;
    *(s16 *)((char *)arg0 + 4) = 0;
    *(s16 *)((char *)arg0 + 0x18) = 0;
    *(s32 *)((char *)arg0 + 0x1C) = 0;
    temp_17 = (*(s16 *)((char *)arg0 + 0x14) * 0xE) + 8;
    func_00168328(*(s16 *)((char *)arg0 + 8) - 0x10, *(s16 *)((char *)arg0 + 0xA) + temp_17, 8);
    if (*(s16 *)((char *)arg0 + 0x12) == 0) {
        func_00168328(-0x60, *(s16 *)((char *)arg0 + 0xA) + temp_17, 8);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016CAC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016D7A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016DA60);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016DFF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016E180);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016E368);

extern u16 *D_0035C518;

/* ER-014. Types and offsets are Unconfirmed readings. */
void func_0016E390(void) {
    int i;
    u16 *p = D_0035C518;

    p[3] = 0;
    for (i = 7; i >= 0; i--) {
        p[8 + i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016E3C8);

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355160);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016E408);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016E638);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016E7C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016E910);

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003551F8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355210);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355228);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355240);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355250);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355268);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016EBF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016FB60);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0016FD70);

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355290);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003552A8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003552B8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003552D0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003552E0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003552F8);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00170AC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00170D18);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001717A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00171880);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00171AA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00171D38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00172390);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001727D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001729E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00172C20);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00172E30);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001731F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001734F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00173738);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00173A20);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001741C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001746C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001748E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00174AD0);

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003558C0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003558D8);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00174C30);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00174DC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00174F08);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001750F0);

extern s32 func_0016B6F0(s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00175D38(s32 arg0) {
    s32 temp_2;
    s32 var_16;

    var_16 = 0;
    temp_2 = func_0016B6F0(0x11, arg0);
    if (temp_2 == 8) {
        var_16 = -1;
    } else if (temp_2 & 0x100) {
        var_16 = (temp_2 & 0xFF) + 1;
    }
    return var_16;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00175D90);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00175DA8);

extern void func_00167B20(s32, s32, s32, s32, s32);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed.
   ER-095: widened to pass five arguments through (func_001C46C8 sets $a0-$a4 before calling it); same bytes. */
void func_00175DD0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_00167B20(arg0, arg1, arg2, arg3, arg4);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00175DD8);

extern s32 D_0035C4E8;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00176370(void) {
    D_0035C4E8 = 0;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00176378(void) {
    D_0035C4E8 = 1;
}

extern void func_00167998(s32, s32, s32, s32);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00176388(s32 arg0) {
    switch (arg0) {
    case 0:
        func_00167998(1, 0, 0x80, 0xCA);
        func_00167998(0, 1, 0xD0, 0xCA);
        func_00167998(5, 2, 0x138, 0xCA);
        break;
    case 1:
        func_00167998(0, 1, 0xD0, 0xCA);
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00176410);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001765B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001766D0);

extern s32 D_01ADED90[];

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001766E0(void) {
    *(s32 *)D_01ADED90 = 0;
    *(s32 *)((char *)D_01ADED90 + 4) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001766F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00176738);

void func_00176808(void) {
}

void func_00176810(void) {
}

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00176818(s32 *arg0) {
    f32 x0;
    f32 y0;
    f32 dx;
    f32 dy;
    f32 k;

    if (arg0[7] <= arg0[6]) {
        arg0[4] = arg0[2];
        arg0[5] = arg0[3];
        return;
    }
    x0 = arg0[0];
    y0 = arg0[1];
    dx = arg0[2] - x0;
    dy = arg0[3] - y0;
    k = func_001E3EC8(((f32)arg0[6] * 1.5707963f) / (f32)arg0[7]);
    arg0[4] = x0 + dx * k;
    arg0[5] = y0 + dy * k;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001768F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00176A98);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00176BA0);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00176CE0(void *arg0) {
    *(s32 *)((char *)arg0 + 0x28) = 0x3C;
    *(f32 *)((char *)arg0 + 0x50) = (f32) (6.0f - ((f32) func_001E9C70() * 12.0f * 4.656613e-10f));
    *(f32 *)((char *)arg0 + 0x54) = (f32) (((f32) func_001E9C70() * 12.0f * 4.656613e-10f) + 6.0f);
    *(f32 *)((char *)arg0 + 0x58) = (f32) (6.0f - ((f32) func_001E9C70() * 12.0f * 4.656613e-10f));
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00176D90(void *arg0) {
    f32 temp_f0;

    *(f32 *)((char *)arg0 + 0x50) = (f32) *(f32 *)arg0;
    *(f32 *)((char *)arg0 + 0x54) = (f32) *(f32 *)((char *)arg0 + 4);
    *(f32 *)((char *)arg0 + 0x58) = (f32) *(f32 *)((char *)arg0 + 8);
    *(f32 *)arg0 = (f32) (*(f32 *)arg0 + (1024.0f - ((f32) func_001E9C70() * 2048.0f * 4.656613e-10f)));
    *(f32 *)((char *)arg0 + 4) = (f32) (*(f32 *)((char *)arg0 + 4) + (1024.0f - ((f32) func_001E9C70() * 2048.0f * 4.656613e-10f)));
    temp_f0 = (f32) func_001E9C70();
    *(s32 *)((char *)arg0 + 0x28) = 0x30;
    *(f32 *)((char *)arg0 + 0xC) = 1.0f;
    *(f32 *)((char *)arg0 + 8) = (f32) (*(f32 *)((char *)arg0 + 8) + (1024.0f - (temp_f0 * 2048.0f * 4.656613e-10f)));
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00176E78(void *arg0) {
    f32 temp_f0;

    *(f32 *)arg0 = (f32) (*(f32 *)arg0 + (64.0f - ((f32) func_001E9C70() * 128.0f * 4.656613e-10f)));
    temp_f0 = (f32) func_001E9C70();
    *(s32 *)((char *)arg0 + 0x28) = 0x3C;
    *(f32 *)((char *)arg0 + 8) = (f32) (*(f32 *)((char *)arg0 + 8) + (64.0f - (temp_f0 * 128.0f * 4.656613e-10f)));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00176F18);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00176F98);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00176FF8(void *arg0) {
    f32 temp_f3;
    f32 temp_f4;
    f32 temp_f5;

    temp_f3 = *(f32 *)arg0;
    temp_f4 = *(f32 *)((char *)arg0 + 4);
    temp_f5 = *(f32 *)((char *)arg0 + 8);
    *(s32 *)((char *)arg0 + 0x1C) = (s32) (*(s32 *)((char *)arg0 + 0x1C) - 2);
    *(f32 *)arg0 = (f32) (temp_f3 + ((*(f32 *)((char *)arg0 + 0x50) - temp_f3) / 30.0f));
    *(f32 *)((char *)arg0 + 4) = (f32) (temp_f4 + ((*(f32 *)((char *)arg0 + 0x54) - temp_f4) / 30.0f));
    *(f32 *)((char *)arg0 + 8) = (f32) (temp_f5 + ((*(f32 *)((char *)arg0 + 0x58) - temp_f5) / 30.0f));
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00177070(void *arg0) {
    *(s32 *)((char *)arg0 + 0x1C) = (s32) (*(s32 *)((char *)arg0 + 0x1C) - 2);
    *(f32 *)((char *)arg0 + 4) = (f32) (*(f32 *)((char *)arg0 + 4) - 4.0f);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00177098);

extern s32 D_0035C544;
extern s32 D_0035C33C;
extern s32 D_0035C548;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001772D0(void) {
    D_0035C544 = 0;
    D_0035C33C = 0x270F;
    D_0035C548 = 0;
}

extern void * func_00111B58(s32, s32, s32);
extern s32 func_001C16C0(s32, s32, s32, s32);
extern s32 D_01ADEEB0[];
extern void func_001C1880(void);
extern s32 D_0035C540;
extern s32 D_0035C530;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001772E8(s32 arg0) {
    func_00111B58(6, D_0035B884, 1);
    func_001C16C0(5, (s32) D_01ADEEB0, 0, 0);
    func_001C1880();
    D_0035C540 = arg0;
    D_0035C548 = 0;
    D_0035C530 = arg0 + 0x108C00;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00177350);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00177480);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001775B0);

extern u8 D_01ADEDB0[];
extern s32 D_0035C340[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings.
   ER-095: the third argument passes through untouched in $a2 to func_001E9D08 (func_001C46C8 passes a string there);
   s32 and falls off the end after the call (no return statement): only that shape gives the sibling call
   `j func_00177858` here and the $v1 its caller func_001C46C8 uses after the call (recipes 2.20).
   PORT NOTE (ruling 3.4, applied to this function 22 Nov 2026): the caller gets whatever func_00177858 left in $v0;
   a PC build must not rely on the missing return (undefined in C) and returns that as-shipped value explicitly on
   its side. */
s32 func_001776D8(s32 arg0, s32 arg1, char *arg2) {
    func_001E9D08(D_01ADEDB0, D_0035C340, arg2);
    func_00177858(arg0, arg1, D_01ADEDB0);
}

extern s32 D_0035C348[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00177730(s32 arg0, s32 arg1) {
    func_001E9D08(D_01ADEDB0, D_0035C348);
    func_00177858(arg0, arg1, D_01ADEDB0);
}

extern s32 func_001E9D08();
extern s32 D_003559D0[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00177788(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_001E9D08(D_01ADEDB0, D_003559D0, arg3, arg2);
    func_00177858(arg0, arg1, D_01ADEDB0);
}

extern s32 D_003559E0[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001777F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_001E9D08(D_01ADEDB0, D_003559E0, arg3, arg2);
    func_00177858(arg0, arg1, D_01ADEDB0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00177858);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00178028);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00178700);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001789C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00178C60);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00179A78);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00179AE8);

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003559D0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003559E0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003559F0);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00179B30);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017A298);

extern s32 D_003496C0[];
extern u8 D_001FFAF1[];

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0017A2E0(void) {
    *(s32 *)D_003496C0 = 0;
    *(s32 *)((char *)D_003496C0 + 4) = (s32) D_001FFAF1[0];
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017A300);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017A898);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017AA60);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017AB90);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017ADC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017B000);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017B380);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017B708);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017B788);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017B810);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017BBB8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017BC40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017BCB8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017C0C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017C1D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017C2C0);

void func_0017C3D0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017C3D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017C500);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017C738);

extern char D_002029D0[];
extern char D_00202A50[];
extern s32 D_00202AD0[];
extern void func_0017D6F0(s32 arg0, void *arg1);
extern void func_0017D6B8(s32, void *, void *);
extern void func_001BB2C8(void);
extern void func_001320B8(void *arg0, u16 arg1);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
s32 func_0017CA10(char *arg0, char *arg1, f32 scale) {
    s32 flag;
    s32 n;
    u32 kind;

    flag = 0;
    kind = (u32)(*(s32 *)(arg0 + 0xC) & 0x300) >> 8;
    if (kind == 1) {
        flag = 1;
        func_0017D6F0(*(s32 *)(arg0 + 0x70), D_002029D0);
    } else if (kind == 2) {
        func_0017D6B8(*(s32 *)(arg0 + 0x70), D_002029D0, arg1);
    }
    func_0017D6B8(*(s32 *)(arg0 + 0x70), D_00202A50, arg1);
    func_001BB2C8();
    n = (s32)((f32)*(s16 *)D_00202AD0 * scale);
    *(u16 *)(arg1 + 0x66) += n;
    func_001320B8(arg1, *(u16 *)((char *)D_00202AD0 + 2));
    if (flag != 0) {
        *(u16 *)(arg1 + 0x68) += n;
    }
    return n != 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017CB10);

extern s32 func_00120D08(void **, void *, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0017CBD0(void *arg0, void **arg1) {
    return func_00120D08(arg1, arg0, 0);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0017CBF8(void *arg0, void **arg1) {
    return func_00120D08(arg1, arg0, 1);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0017CC20(void) {
    return 0;
}

extern char D_0034BFB0[];
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_0017CC28(f32 *arg0, s32 arg1, f32 s) {
    f32 *p; f32 v;
    p = (f32 *)(D_0034BFB0 + arg1 * 0x10);
    v = p[0] * s; if (v > 255.0f) v = 255.0f; arg0[0] = v;
    v = p[1] * s; if (v > 255.0f) v = 255.0f; arg0[1] = v;
    v = p[2] * s; if (v > 255.0f) v = 255.0f; arg0[2] = v;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017CC98);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017CD10);

extern char D_002608C0[];
extern int func_0017D930(int a0);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
char* func_0017CD20(char *arg0, float *arg1, u32 arg2, f32 *arg3) {
    char *p;
    f32 d;
    f32 best;
    s32 i;
    s32 found;

    found = -1;
    i = 0;
    p = D_002608C0;
    best = 1e+07f;
    do {
        if (*(u16 *)p != 0 && arg0 != p && (u32)(*(s32 *)(p + 0xC) & 0x300) >> 8 == arg2 && func_0017D930(*(u16 *)(p + 2)) == 0) {
            d = func_00111480(arg1, p + 0x50);
            if (d < best) {
                best = d;
                found = i;
            }
        }
        i += 1;
        p += 0xA0;
    } while (i < 0x200);
    *arg3 = best;
    if (found == -1) {
        return NULL;
    }
    return D_002608C0 + found * 0xA0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017CE38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017CEC0);

void func_0017CF08(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017CF10);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017CFF8);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
u16* func_0017D0B8(s32 *arg0) {
    s32 i;
    char *p;

    for (i = 0, p = (char *)D_002608C0; i < 0x200; i++, p += 0xA0) {
        if (*(u16 *)p == 0) {
            break;
        }
    }
    if (i >= 0x200) {
        return NULL;
    }
    *arg0 = i + 1;
    return (u16 *)p;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D108);

extern char D_0034C0D0[];
extern s32 D_001FF340[];
extern f32 D_001FF474[];

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0017D290(void) {
    func_001C9B18(D_0034C0D0, D_001FF340);
    *(f32 *)((char *)D_0034C0D0 + 4) = (f32) D_001FF474[0];
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D2D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D328);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0017D398(s32 arg0, void *arg1) {
    u8 temp_3;

    temp_3 = *(u8 *)((char *)arg1 + 6);
    return ((s32) *(u8 *)((char *)((arg0 + (temp_3 >> 3))) + 0x80) >> (temp_3 & 7)) & 1;
}

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_0017D3B8(char *arg0, char *arg1) {
    u8 n = *(u8 *)(arg1 + 6);
    u8 *b = (u8 *)(arg0 + 0x80);
    b[n >> 3] |= 1 << (n & 7);
}

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
u64 func_0017D3E8(char *arg0) {
    u64 v = *(u8 *)(arg0 + 0x87);
    return v >> 7;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0017D3F8(void *arg0) {
    *(u8 *)((char *)arg0 + 0x87) = (u8) (*(u8 *)((char *)arg0 + 0x87) | 0x80);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D408);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D598);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
s32 func_0017D608(char *arg0) {
    char **p = *(char ***)(arg0 + 0x20);
    if (p == NULL) {
        return -1;
    }
    func_001C9B18((s32)(arg0 + 0x50), *p + 0x90);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D640);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D670);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D6B8);

extern char D_001FF8B0[];
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_0017D6F0(s32 arg0, void *arg1) {
    *(s32 *)arg1 = arg0;
    *(s16 *)((char *)arg1 + 8) = 0;
    *(void **)((char *)arg1 + 4) = D_001FF8B0;
    *(void **)((char *)arg1 + 0xC) = D_001FF8B0 + 0x252;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0017D710(void *arg0, s32 arg1) {
    if ((arg0 != NULL) && (arg1 >= (s32) *(u16 *)((char *)arg0 + 0x7A))) {
        *(u16 *)((char *)arg0 + 0x7A) = (u16) arg1;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D730);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D768);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
f32 func_0017D788(void **arg0, s32 arg1) {
    s16 *t = *(s16 **)((char *)*arg0 + 0x11C);
    t += arg1;
    return t[0xC8];
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D7A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D7F0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0017D900(void *arg0, s32 arg1) {
    if (arg0 != NULL) {
        *(s32 *)((char *)arg0 + 0xC) = (s32) ((*(s32 *)((char *)arg0 + 0xC) & 0xFFFFFCFF) | ((arg1 & 0xFF) << 8));
    }
}

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355A50);
/* ER-024 (parked by ER-014 for its jump table). Returns 1 for 20 of the codes 29..117, else 0; what the codes
 * are is Unconfirmed. */
int func_0017D930(int a0) {
    switch (a0) {
    case 29:
    case 59:
    case 64:
    case 65:
    case 68:
    case 70:
    case 71:
    case 72:
    case 73:
    case 74:
    case 75:
    case 76:
    case 85:
    case 89:
    case 90:
    case 91:
    case 92:
    case 93:
    case 116:
    case 117:
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017D968);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017DA38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017DA70);

extern void func_0017CF10(void *, f32 fparg0);
extern void func_0017D328(void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0017DB98(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xA;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017DBE8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017DC30);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017DCC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017E1C8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0017E578(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xA;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017E5C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017E610);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017E6A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017ED40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017F3C0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0017F570(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017F580);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017FAC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017FAF0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0017FCA8(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 0x32;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017FCE8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0017FE28(void *arg0) {
    *(s32 *)((char *)arg0 + 4) = 0xB4;
    *(s32 *)((char *)arg0 + 0x10) = 0x96;
    *(s32 *)((char *)arg0 + 0x14) = 0x20;
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0017FE48);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00180200);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00180228);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001808A0(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    *(s32 *)((char *)arg0 + 4) = 0xC8;
    *(s32 *)((char *)arg0 + 0x14) = 0x20;
    *(s32 *)((char *)arg0 + 0x10) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001808E8);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00181210(char *arg0) {
    f32 t;
    f32 t2;

    func_0017D328(arg0);
    t = func_001E3EC8(*(f32 *)(arg0 + 0x64)) * 32.0f;
    *(f32 *)(arg0 + 0x50) -= t * func_001E3DE0(*(f32 *)(arg0 + 0x60));
    *(f32 *)(arg0 + 0x54) -= func_001E3EC8(*(f32 *)(arg0 + 0x60)) * 32.0f;
    t2 = func_001E3DE0(*(f32 *)(arg0 + 0x64)) * 32.0f;
    *(f32 *)(arg0 + 0x58) -= t2 * func_001E3DE0(*(f32 *)(arg0 + 0x60));
    *(s32 *)(arg0 + 4) = 0x3C;
    *(s32 *)(arg0 + 0x10) = 0x3C;
    *(s32 *)(arg0 + 0x14) = 0;
    *(s32 *)(arg0 + 0x18) = 0;
    *(s8 *)(arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001812C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00181310);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00181460);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00181578);

extern void func_0017D900(void *arg0, s32 arg1);
extern void func_00162CB8(s32 *, s32);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001817F8(void *arg0) {
    func_0017D900(arg0, 0);
    func_00162CB8(D_00248880, (s32) (arg0 + 0x10));
    *(s32 *)((char *)arg0 + 4) = 1;
}

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355BD0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355BE0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355BF0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C00);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C08);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C10);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C18);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C20);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C30);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00181838);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00181A58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00181AA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00181AF0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00181CA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00181EF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001822C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00182310);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00182358);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001823F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00182720);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00182ED8(void *arg0) {
    *(s32 *)((char *)arg0 + 0x10) = (s32) *(s32 *)D_00248880;
    *(s32 *)((char *)arg0 + 0x14) = (s32) *(s32 *)((char *)D_00248880 + 4);
    *(s32 *)((char *)arg0 + 4) = 5;
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00182F00);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00183150(void *arg0) {
    *(s32 *)((char *)arg0 + 0x10) = (s32) D_00248880[0];
    *(s32 *)((char *)arg0 + 4) = 5;
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00183170);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001833D0(void *arg0) {
    *(s32 *)((char *)arg0 + 0x10) = (s32) D_00248880[0];
    *(s32 *)((char *)arg0 + 4) = 0x14;
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001833F0);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001835E0(char *a0) {
    *(f32 *)(a0 + 0x30) = 25.0f;
    *(f32 *)(a0 + 0x38) = 1.0f;
    *(f32 *)(a0 + 0x4C) = 0.1f;
    *(s32 *)(a0 + 0x34) = 0;
    *(s8 *)(a0 + 0x78) = 1;
    *(s32 *)(a0 + 4) = 0xC8;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00183618);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00183930);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00183948);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00183D78(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00183D88);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00183E90(void *arg0) {
    f32 temp_f1;

    func_00162CB8(D_00248880, arg0 + 0x10);
    temp_f1 = *(f32 *)((char *)arg0 + 0x60);
    *(s32 *)((char *)arg0 + 4) = 2;
    *(s8 *)((char *)arg0 + 0x78) = 1;
    *(f32 *)((char *)arg0 + 0x40) = temp_f1;
    *(f32 *)((char *)arg0 + 0x60) = (f32) -temp_f1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00183EE0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00184328(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 0x1E;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00184338);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00184588(void *arg0) {
    *(s32 *)((char *)arg0 + 0x10) = 8;
    *(s32 *)((char *)arg0 + 0x14) = 0x3C;
    *(f32 *)((char *)arg0 + 0x30) = 2.0f;
    *(s32 *)((char *)arg0 + 4) = 1;
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001845B8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00184820(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 0x14;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00184830);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00184930);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00184C18);

void func_00184E30(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00184E38(void *arg0) {
    *(s32 *)((char *)arg0 + 4) = 0x28;
    *(f32 *)((char *)arg0 + 0x30) = 1.0f;
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(f32 *)((char *)arg0 + 0x54) = (f32) (*(f32 *)((char *)arg0 + 0x54) - 5.0f);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00184E68);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001856C0(void *arg0) {
    *(s32 *)((char *)arg0 + 4) = 0x28;
    *(f32 *)((char *)arg0 + 0x30) = 1.0f;
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(f32 *)((char *)arg0 + 0x54) = (f32) (*(f32 *)((char *)arg0 + 0x54) - 5.0f);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001856F0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00185D60(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0x190;
    *(s32 *)((char *)arg0 + 0x10) = 0x190;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00185DB0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00186258);

void func_001866B8(void) {
}

void func_001866C0(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001866C8(void *arg0) {
    *(s32 *)((char *)arg0 + 4) = 0x28;
    *(f32 *)((char *)arg0 + 0x30) = 1.0f;
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(f32 *)((char *)arg0 + 0x54) = (f32) (*(f32 *)((char *)arg0 + 0x54) - 5.0f);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001866F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00186D70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00186DD8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00186E10(void *arg0) {
    *(s32 *)((char *)arg0 + 0x18) = 1;
    *(s32 *)((char *)arg0 + 4) = 0x12D;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00186E28);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00187150);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00187640);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00187BD8);

extern void func_0017CF10(void *, f32);
/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00187F78(char *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)(arg0 + 4) = 0xA;
    *(s32 *)(arg0 + 0x10) = 0xA;
    *(s32 *)(arg0 + 0x14) = 0;
    *(s32 *)(arg0 + 0x18) = 0;
    *(s32 *)(arg0 + 0x24) = 0;
    *(f32 *)(arg0 + 0x4C) = *(f32 *)(arg0 + 0x64);
    *(s8 *)(arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00187FD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00188020);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001880B8);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_001882B8(char *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)(arg0 + 4) = 0xA;
    *(s32 *)(arg0 + 0x10) = 0xA;
    *(s32 *)(arg0 + 0x14) = 0;
    *(s32 *)(arg0 + 0x18) = 0;
    *(s8 *)(arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00188308);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00188350);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001883E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00188AA8);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_00189198(char *a0) {
    *(f32 *)(a0 + 0x30) = 25.0f;
    *(f32 *)(a0 + 0x38) = 1.0f;
    *(s32 *)(a0 + 4) = 0x12C;
    *(s32 *)(a0 + 0x34) = 0;
    *(s8 *)(a0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001891C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00189200);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00189360);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001893E0);

extern f32 func_001E40B8(f32);

/* ER-080: by hand (plain double C, recipes 2.11): (float)(d * 0.75) then 0.294 * d / r^2, clamped angle; names and
   types are Unconfirmed readings. */
void func_00189810(char *a0) {
    float t;
    float ang;
    float r;

    t = (float)(func_001114E8(D_0034C0D0, (float *)(a0 + 0x50)) * 0.75) * 0.294f
        / (*(float *)(a0 + 0x30) * *(float *)(a0 + 0x30));
    if (1.0f < t) {
        ang = 0.7853982f;
    } else {
        ang = func_001E40B8(t) * 0.5f;
    }
    r = -*(float *)(a0 + 0x30);
    *(float *)(a0 + 0x34) = r * func_001E3EC8(ang);
    *(float *)(a0 + 0x30) = *(float *)(a0 + 0x30) * func_001E3DE0(ang);
}

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_001898F0(char *arg0) {
    func_0017CF10(arg0, 96.0f);
    *(s32 *)(arg0 + 4) = 0xA;
    *(s32 *)(arg0 + 0x10) = 0x168;
    *(s32 *)(arg0 + 0x14) = 0;
    *(s32 *)(arg0 + 0x18) = 0;
    *(s32 *)(arg0 + 0x48) = 0;
    *(s32 *)(arg0 + 0x4C) = 0;
    *(s8 *)(arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00189948);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00189990);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00189A00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018A3B0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018A468(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

void func_0018A470(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018A478(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

void func_0018A480(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018A488(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xD2;
    *(s32 *)((char *)arg0 + 0x10) = 0xD2;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018A4D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018A8B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018AAB8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018AAE0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018AFD8(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 0xA;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018B010);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018B198(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s32 *)((char *)arg0 + 4) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018B1B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018B1E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018B3E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018B8A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018B8E8);

extern s32 D_00248888[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018BB80(void *arg0) {
    *(s32 *)((char *)arg0 + 0x18) = (s32) D_00248888[0];
    *(s32 *)((char *)arg0 + 4) = 1;
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018BBA0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018BE70(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s32 *)((char *)arg0 + 4) = 0xF0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018BE88);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018BEC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018C030);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018C1F8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018C390(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018C3A0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018C540(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018C550);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_0018C730(void *arg0) {
    *(s32 *)((char *)arg0 + 4) = 2;
    func_00162CB8(D_00248880, (s32)(arg0 + 0x10));
    if (*(s32 *)((char *)arg0 + 0x14) <= 0) {
        *(s32 *)((char *)arg0 + 0x14) = 1;
    }
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018C780);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018C938);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018C9E8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018CA10(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

void func_0018CA18(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018CA20);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018CAD0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018D168);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018D1D8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018D2B0(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s32 *)((char *)arg0 + 4) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018D2C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018D300);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018D500);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_0018D9C8(char *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)(arg0 + 4) = 0x82;
    *(s32 *)(arg0 + 0x10) = 0x82;
    *(s32 *)(arg0 + 0x14) = 0;
    *(s32 *)(arg0 + 0x18) = 0;
    *(s8 *)(arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018DA18);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018E180(void *arg0) {
    func_0017CF10(arg0, 192.0f);
    func_0017D328(arg0);
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s32 *)((char *)arg0 + 4) = 0xA;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018E1C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018E200);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018E290);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018E7B0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0018EDF0(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s32 *)((char *)arg0 + 4) = 0xA;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018EE38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018EE70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018EF08);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_0018F170(char *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)(arg0 + 4) = 0xA;
    *(s32 *)(arg0 + 0x14) = 0;
    *(s32 *)(arg0 + 0x18) = 0;
    *(f32 *)(arg0 + 0x40) = 1.0f;
    *(s8 *)(arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018F1C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018F200);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018F298);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018F6D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018FC10);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0018FC78);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001901A0(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xA;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001901F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00190238);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001902C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001904F8);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00190688(char *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)(arg0 + 4) = 0xA;
    *(s32 *)(arg0 + 0x10) = 0xA;
    *(s32 *)(arg0 + 0x14) = 0;
    *(s32 *)(arg0 + 0x18) = 0;
    *(s8 *)(arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001906D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00190720);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001907B8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00190D78);

extern s32 * func_0017CE38(void *);
extern s32 func_001650E8(s32, s32);
extern void func_00191640(void);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00191440(void *arg0) {
    func_0017D900(arg0, 0);
    if (func_0017CE38(arg0) != NULL) {
        *(s16 *)arg0 = 0;
        return;
    }
    *(s32 *)((char *)arg0 + 4) = 0x30;
    *(s8 *)((char *)arg0 + 0x78) = 0;
    func_001650E8(0x203, 0x3C006C);
    func_00191640();
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001914A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00191640);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_001918B8(char *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)(arg0 + 4) = 0xA;
    *(s32 *)(arg0 + 0x14) = 0;
    *(s32 *)(arg0 + 0x18) = 0;
    *(s32 *)(arg0 + 0x1C) = 0;
    *(s8 *)(arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00191908);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00191950);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001919E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00191AF8);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00191DC8(void *arg0) {
    func_0017D900(arg0, 0);
    if (func_0017CE38(arg0) != NULL) {
        *(s16 *)arg0 = 0;
        return;
    }
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 0x6E;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00191E10);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00191F10);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00192088);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001922C0);

extern void func_001925D8(void);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001923D0(void *arg0) {
    func_0017D900(arg0, 0);
    if (func_0017CE38(arg0) != NULL) {
        *(s16 *)arg0 = 0;
        return;
    }
    *(s32 *)((char *)arg0 + 4) = 0x3C;
    *(s8 *)((char *)arg0 + 0x78) = 0;
    func_001650E8(0x204, 0x3C006C);
    func_001925D8();
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00192438);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001925D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001929F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00192AB0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00192BB0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00192D30);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00192E18);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00192EE8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00192F88);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193020);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193148);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193230);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193300);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193440);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001934E0);

extern s32 func_00193828();

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00193608(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 0x20;
    func_001650E8(0x203, 0x3C006C);
    func_00193828();
    func_0017D900(arg0, 0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193658);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193828);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193AA0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193B30);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193B68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193C10);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00193F68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001940F8);

INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C50);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C60);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C70);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C80);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C90);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355C98);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355CA0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355CA8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355CB0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355CC0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355CD0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355CD8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355CE0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355CF0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355D00);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355D10);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355D20);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355D30);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355D40);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355D50);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355D60);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355D70);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355D80);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355D90);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355DA0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355DB0);
INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001941C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001942C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00194440);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001944E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00194520);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00194858);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00194928(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0x3C;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00194978);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001949B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00194B38);


/* ER-014. Returns -1 on the two early exits and falls off the end after the call (no return value):
   only that shape gives the sibling call `j func_001CA018` (a call whose value is returned gets a frame).
   PORT NOTE (ruling 3.4): on that path the caller gets whatever func_001CA018 left in $v0; a PC build must not
   rely on the missing return (undefined in C) and returns that as-shipped value explicitly on its side. */
int func_00194D08(char *a0) {
    char *p = *(char **)(a0 + 0x20);

    if (p == NULL) {
        return -1;
    }
    if (*(s16 *)(p + 0x64) <= 0) {
        return -1;
    }
    func_001CA018(a0 + 0x40, a0 + 0x50, *(char **)p + 0x90);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00194D38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00194DF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00194E40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00194F58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00195138);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00195318);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001955D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001956A0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00195760);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00195798);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001959D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00195AD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00195BE0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00195DA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00195E78);

extern s32 func_001132B0(void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00195E88(void *arg0) {
    void *temp_4;

    temp_4 = *(void **)((char *)(*(void **)((char *)arg0 + 0x24)) + 4);
    if (*(u16 *)((char *)temp_4 + 0xE8) & 1) {
        func_001132B0(temp_4);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00195EB0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00195F70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00195FA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001960A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001962F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196388);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196458);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196468);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196498);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196538);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196570);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196690);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196818);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001969F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196C00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196CD0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00196CE0(void *arg0) {
    void *temp_4;

    temp_4 = *(void **)((char *)(*(void **)((char *)arg0 + 0x24)) + 4);
    if (*(u16 *)((char *)temp_4 + 0xE8) & 1) {
        func_001132B0(temp_4);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196D08);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196E08);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196E40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00196F40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00197028);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00197288);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00197358);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00197368(void *arg0) {
    void *temp_4;

    temp_4 = *(void **)((char *)(*(void **)((char *)arg0 + 0x24)) + 4);
    if (*(u16 *)((char *)temp_4 + 0xE8) & 1) {
        func_001132B0(temp_4);
    }
}

extern void func_00197590(void);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00197390(void *arg0) {
    func_0017D900(arg0, 0);
    if (func_0017CE38(arg0) != NULL) {
        *(s16 *)arg0 = 0;
        return;
    }
    *(s32 *)((char *)arg0 + 4) = 0x3C;
    *(s8 *)((char *)arg0 + 0x78) = 0;
    func_001650E8(0x204, 0x3C006C);
    func_00197590();
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001973F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00197590);

extern void func_00197B28(void);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001977E8(void *arg0) {
    func_0017D900(arg0, 0);
    if (func_0017CE38(arg0) != NULL) {
        *(s16 *)arg0 = 0;
        return;
    }
    *(s32 *)((char *)arg0 + 4) = 0x70;
    *(s8 *)((char *)arg0 + 0x78) = 0;
    func_001650E8(0x204, 0x3C006C);
    func_00197B28();
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00197850);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00197B28);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00197F40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198158);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198218);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198368);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_001984E8(char *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)(arg0 + 4) = 0xA;
    *(s32 *)(arg0 + 0x14) = 0;
    *(s32 *)(arg0 + 0x18) = 0;
    *(s32 *)(arg0 + 0x1C) = 0;
    *(s8 *)(arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198538);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198580);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198618);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198728);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001989A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198A70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198AA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198BB0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198C98);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00198EA8(void *arg0) {
    if (func_0017CE38(arg0) != NULL) {
        *(s16 *)arg0 = 0;
        return;
    }
    func_0017CF10(arg0, 96.0f);
    *(s32 *)((char *)arg0 + 4) = 0xC8;
    *(s32 *)((char *)arg0 + 0x14) = 0x20;
    *(s32 *)((char *)arg0 + 0x10) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00198F08);

/* ER-098: by hand from the m2c draft (tools/m2c_bulk.py, recipes 2.20); names, types and offsets are Unconfirmed readings. */
void func_00199660(char *arg0) {
    f32 t;
    f32 t2;

    func_0017D328(arg0);
    t = func_001E3EC8(*(f32 *)(arg0 + 0x64)) * 32.0f;
    *(f32 *)(arg0 + 0x50) -= t * func_001E3DE0(*(f32 *)(arg0 + 0x60));
    *(f32 *)(arg0 + 0x54) -= func_001E3EC8(*(f32 *)(arg0 + 0x60)) * 32.0f;
    t2 = func_001E3DE0(*(f32 *)(arg0 + 0x64)) * 32.0f;
    *(f32 *)(arg0 + 0x58) -= t2 * func_001E3DE0(*(f32 *)(arg0 + 0x60));
    *(s32 *)(arg0 + 4) = 0x3C;
    *(s32 *)(arg0 + 0x10) = 0x3C;
    *(s32 *)(arg0 + 0x14) = 0;
    *(s32 *)(arg0 + 0x18) = 0;
    *(s8 *)(arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00199718);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00199760);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00199AE0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00199D48);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00199E08);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00199E40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_00199FC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019A288);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019A300);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019A3D0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0019A3E0(void *arg0) {
    void *temp_4;

    temp_4 = *(void **)((char *)(*(void **)((char *)arg0 + 0x24)) + 4);
    if (*(u16 *)((char *)temp_4 + 0xE8) & 1) {
        func_001132B0(temp_4);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019A408);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019A470);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0019A4A8(void *arg0) {
    *(s32 *)((char *)arg0 + 0x18) = 1;
    *(s32 *)((char *)arg0 + 4) = 0x12D;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019A4C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019A7E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019ACD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019B270);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019B610);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019B6D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019B808);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0019B998(void *arg0) {
    *(s32 *)((char *)arg0 + 4) = 2;
}

void func_0019B9A8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019B9B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019BA58);

extern void func_001A9948(void *);
extern void func_001A9DF8(void *);
extern void func_0019BDB0(void *);
void func_0019BDC0(void *arg0);

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0019BA90(void *arg0) {
    u32 temp_3;
    u32 temp_3_2;

    temp_3 = *(u32 *)((char *)arg0 + 4);
    if (temp_3 >= 0x1FU) {
        func_001A9948(arg0);
    } else if (temp_3 == 0x1E) {
        func_001A9DF8(arg0);
        func_0019BDB0(arg0);
        func_0019BDC0(arg0);
    }
    temp_3_2 = *(u32 *)((char *)arg0 + 4);
    if (temp_3_2 == 1) {
        *(u32 *)((char *)arg0 + 0x18) = temp_3_2;
        *(u32 *)((char *)arg0 + 4) = 0xC7U;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019BB08);

void func_0019BCD8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019BCE0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019BDB0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0019BDC0(void *arg0) {
    void *temp_4;

    temp_4 = *(void **)((char *)(*(void **)((char *)arg0 + 0x24)) + 4);
    if (*(u16 *)((char *)temp_4 + 0xE8) & 1) {
        func_001132B0(temp_4);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019BDE8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019BE90);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019BEC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019BFA0);

extern void func_001AB1C8(void *, s32);
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_0019C078(void *arg0) {
    func_001AB1C8(arg0, *(s32 *)((char *)arg0 + 4));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019C080);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019C150);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019C1C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019C2A0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019C3A0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019C610);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019C7F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019CA20);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019CC70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019CCA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019CE20);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019CF58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019D090);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019D1F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019D380);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019D5C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019D620);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019D7A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019D808);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019DD48);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019E038);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019E1D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019E3B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019E438);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019E590);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019E870);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019EA50);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019EAD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019EB60);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019EBC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019EFC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019F158);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019F440);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019F898);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019F958);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019FA38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_0019FD10);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A0148);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A0208);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A0330);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A04D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A06E0);

void func_001A07F8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A0800);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A0858);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A08E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A0960);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A0C08);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A0C60);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A0D90);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A0DF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A0E68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A10E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A12C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A1490);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A1838);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A1880);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A1B90);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A1E10);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A2000);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A21B8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A2438);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A24A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A2528);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A26E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A2820);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A2BE0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A2F58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A3190);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A33B8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A3490);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A3608);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A36E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A3738);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A37C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A3A40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A3D00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A3DD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A3EC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A3F30);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A3FC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A40A0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A4270);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A4710);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A47B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A4A68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A4BC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A4DC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A4E18);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A4EA0);

extern float D_001F0380[];
/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001A5360(void *arg0, void *arg1) {
    s32 temp_2;

    temp_2 = (s32) ((*(f32 *)((char *)arg0 + 0x64) * 4096.0f) / 6.2831855f) & 0xFFF;
    *(f32 *)arg1 = (f32) (*(f32 *)((char *)arg0 + 0x50) - (D_001F0380[temp_2] * 640.0f));
    *(f32 *)((char *)arg1 + 4) = (f32) *(f32 *)((char *)arg0 + 0x54);
    *(f32 *)((char *)arg1 + 8) = (f32) (*(f32 *)((char *)arg0 + 0x58) - (D_001F0380[temp_2 + 0x400] * 640.0f));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A53E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A58B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A5BA0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A5DE0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A5F00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A6140);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A64E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A65F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A6640);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A66D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A6AD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A6BA0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A6D30);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A7000);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A7130);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A7418);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A75D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A7BD0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A7CA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A7CE0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A7D78);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A7FD0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A81B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A8668);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A8888);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A8A00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A8B38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A8C70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A8DC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A8F58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A91A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A9210);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A9248);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A95C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A9948);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001A9DF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AA0F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AA4D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AA548);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AA8E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AAE40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AB1C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AB410);

extern void func_0017CF10(void *, f32); extern void func_0017D328(void *); extern s32 func_001E9C70();
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001AB480(char *a0) {
    f32 t;
    func_0017CF10(a0, 96.0f);
    func_0017D328(a0);
    *(s32 *)(a0 + 0x14) = 0;
    *(s32 *)(a0 + 0x18) = 0;
    *(s8 *)(a0 + 0x78) = 0;
    *(s32 *)(a0 + 4) = 0x258;
    *(s32 *)(a0 + 0x10) = 0x258;
    *(f32 *)(a0 + 0x60) = 0.7853982f - (f32)(func_001E9C70() & 0xFFFF) * 0.000015258789f * 1.5707964f;
    t = (f32)(func_001E9C70() & 0xFFFF) * 0.000015258789f;
    *(s32 *)(a0 + 0xC) &= 0xFFFEFFFF;
    *(f32 *)(a0 + 0x64) = 3.1415927f - t * 6.2831855f;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AB540);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AB5B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AB6C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AB760);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001ABB20(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xA;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001ABB70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001ABBB8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001ABC48);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001ABE20);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001AC158(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xA;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AC1A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AC1F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AC280);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AC4C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AC938);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AC988);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AC9D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001ACA68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AD0E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AD750);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AD8F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AD948);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AD980);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001ADA10);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001ADC70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AE078);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AE2B0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001AE440(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xA;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AE490);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AE4D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AE568);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AE7A0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AEC58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AEC70);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001AED88(char *a0) {
    *(f32 *)(a0 + 0x30) = 25.0f;
    *(f32 *)(a0 + 0x38) = 1.0f;
    *(s32 *)(a0 + 4) = 0x12C;
    *(s32 *)(a0 + 0x34) = 0;
    *(s8 *)(a0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AEDB8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AEF18);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001AEF90(char *a0) {
    *(f32 *)(a0 + 0x38) = 0.16f;
    *(f32 *)(a0 + 0x3C) = 1.0f;
    *(s32 *)(a0 + 4) = 0x78;
    *(s32 *)(a0 + 0x34) = 0;
    *(s8 *)(a0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AEFC0);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001AF2E8(void *arg0) {
    func_00162CB8(D_00248880, arg0 + 0x10);
    *(s32 *)((char *)arg0 + 4) = *(s32 *)((char *)arg0 + 0x10);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AF320);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001AF658(void *arg0) {
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s32 *)((char *)arg0 + 4) = 0xD7;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AF668);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AF6A0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AF730);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AF870);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AFB30);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001AFBE0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B0270);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B0398);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B0510);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B0600(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xA;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B0650);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B0698);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B0728);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B0C80);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B1198);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B1200);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B1248);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B1578);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B1960);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B19A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B19F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B1BC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B1EA0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B2230(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xA;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B2280);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B22C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B2358);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B2910);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B2E28(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xA;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B2E78);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B2EC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B2F50);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B30D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B3440);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B3498);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B34F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B3590);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B3750);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B39E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B3B08);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B3D80);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B3E70(void *arg0) {
    *(s32 *)((char *)arg0 + 4) = 0x5A;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B3E80);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001B42B0(char *a0) {
    *(f32 *)(a0 + 0x38) = 0.16f;
    *(f32 *)(a0 + 0x3C) = 1.0f;
    *(s32 *)(a0 + 4) = 0x78;
    *(s32 *)(a0 + 0x34) = 0;
    *(s8 *)(a0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B42E0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B4520(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s32 *)((char *)arg0 + 4) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B4538);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B4570);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B4730);

extern void func_0017D900(void *, s32);
extern void func_001B99B0(void *);
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001B4BE8(char *arg0) {
    func_00162CB8(D_00248880, (s32)(arg0 + 0x10));
    *(s32 *)(arg0 + 4) = 2;
    *(s32 *)(arg0 + 0x10) = 0;
    func_001B99B0(arg0);
    func_0017D900(arg0, 0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B4C38);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B4DA8(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xA;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B4DF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B4E40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B4ED0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B5010);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B5298);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B52F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B5338);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B53D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B5568);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B55E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B5C68);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001B5E68(void *arg0) {
    func_00162CB8(D_00248880, arg0 + 0x10);
    *(s32 *)((char *)arg0 + 0x10) = 0;
    *(s32 *)((char *)arg0 + 4) = 2;
    func_0017D900(arg0, 0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B5EB0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B6288);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B62E8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B6610(void *arg0) {
    func_0017CF10(arg0, 96.0f);
    func_0017D328(arg0);
    *(s32 *)((char *)arg0 + 4) = 0xA;
    *(s32 *)((char *)arg0 + 0x14) = 0;
    *(s32 *)((char *)arg0 + 0x18) = 0;
    *(s8 *)((char *)arg0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B6660);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B66A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B6738);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B6CD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B7230);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B7358);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B74F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B75E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B75F8);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001B77B8(char *a0) {
    *(f32 *)(a0 + 0x38) = 0.16f;
    *(f32 *)(a0 + 0x3C) = 1.0f;
    *(s32 *)(a0 + 4) = 0x78;
    *(s32 *)(a0 + 0x34) = 0;
    *(s8 *)(a0 + 0x78) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B77E8);

void func_001B7B10(void) {
}

void func_001B7B18(void) {
}

void func_001B7B20(void) {
}

void func_001B7B28(void) {
}

void func_001B7B30(void) {
}

void func_001B7B38(void) {
}

void func_001B7B40(void) {
}

void func_001B7B48(void) {
}

void func_001B7B50(void) {
}

void func_001B7B58(void) {
}

void func_001B7B60(void) {
}

void func_001B7B68(void) {
}

void func_001B7B70(void) {
}

void func_001B7B78(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B7B80(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 0x64;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B7B90);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B7D58(void *arg0) {
    *(s32 *)((char *)arg0 + 4) = 1;
    *(s32 *)((char *)arg0 + 0x10) = 0x20;
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B7D70);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B8020(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
    *(s32 *)((char *)arg0 + 4) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B8030);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B8258(void *arg0) {
    func_00162CB8(D_00248880, arg0 + 0x10);
    *(s32 *)((char *)arg0 + 4) = 0x20;
    *(f32 *)((char *)arg0 + 0x54) = (f32) (*(f32 *)((char *)arg0 + 0x54) - *(f32 *)((char *)arg0 + 0x30));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B82A0);

void func_001B85F0(void) {
}

void func_001B85F8(void) {
}

void func_001B8600(void) {
}

void func_001B8608(void) {
}

void func_001B8610(void) {
}

void func_001B8618(void) {
}

void func_001B8620(void) {
}

void func_001B8628(void) {
}

void func_001B8630(void) {
}

void func_001B8638(void) {
}

void func_001B8640(void) {
}

void func_001B8648(void) {
}

void func_001B8650(void) {
}

void func_001B8658(void) {
}

void func_001B8660(void) {
}

void func_001B8668(void) {
}

void func_001B8670(void) {
}

void func_001B8678(void) {
}

void func_001B8680(void) {
}

void func_001B8688(void) {
}

void func_001B8690(void) {
}

void func_001B8698(void) {
}

void func_001B86A0(void) {
}

void func_001B86A8(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001B86B0(void *arg0) {
    *(s8 *)((char *)arg0 + 0x78) = 0;
}

void func_001B86B8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B86C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B8958);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B8AF0);

extern float func_001E3FB8(float);
extern float D_001F0380[];

/* ER-034. 0 outside [-1, 1]; else looks func_001E3FB8(x) (a libm angle function: reading) up in the 4096-entry table
   D_001F0380 (a sine table: reading). func_001B8CD8 and func_001B8FF0 are byte-identical copies. Names Unconfirmed. */
float func_001B8BC0(float x) {
    float a;

    if (x < -1.0f || 1.0f < x) {
        return 0.0f;
    }
    a = func_001E3FB8(x);
    return D_001F0380[(int)((a * 4096.0f) / 6.2831855f) & 0xFFF];
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B8C50);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B8CB0);

/* ER-034. Same code as func_001B8BC0. */
float func_001B8CD8(float x) {
    float a;

    if (x < -1.0f || 1.0f < x) {
        return 0.0f;
    }
    a = func_001E3FB8(x);
    return D_001F0380[(int)((a * 4096.0f) / 6.2831855f) & 0xFFF];
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B8D68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B8DC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B8DF0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B8F18);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B8F58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B8FC8);

/* ER-034. Same code as func_001B8BC0. */
float func_001B8FF0(float x) {
    float a;

    if (x < -1.0f || 1.0f < x) {
        return 0.0f;
    }
    a = func_001E3FB8(x);
    return D_001F0380[(int)((a * 4096.0f) / 6.2831855f) & 0xFFF];
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B9080);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B90E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B9268);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B9338);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B9458);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B96C0);

extern s32 func_001B9938(void *, f32 *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001B9918(void *arg0, f32 *arg1) {
    return func_001B9938(arg0, arg1) != 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B9938);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B99B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B9A38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B9AF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B9C28);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B9DB0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B9E88);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001B9F20);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BA038);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BA848);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BA920);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BAAA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BAB00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BAB58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BABE8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BAC18);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BAD68);

void func_001BADC8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BADD0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BAE58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BAEE0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BAF58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BB030);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BB1D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BB250);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BB2C8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BB750);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BBC00);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001BBD40(void) {
    u32 i;

    for (i = 0; i < 12; i++) {
        D_00202AD0[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BBD78);

extern void func_001BCD30(s32);
extern void func_001BBF40(s32, s32);

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BBEB8(void *arg0) {
    s16 temp_5;
    s32 temp_3;
    s32 var_4;

    func_001BCD30((s32) *(s16 *)((char *)arg0 + 0x1EC));
    temp_5 = *(s16 *)((char *)arg0 + 0x1A0);
    temp_3 = (temp_5 - *(s16 *)D_001FF890) * 4;
    var_4 = (s32) (((temp_5 * 4) + ((temp_3 <= -1) ? 0 : temp_3)) * (*(s16 *)D_001FF890 + 1)) / (s16) *(s16 *)D_001FF890;
    if ((temp_5 == 0x2710) && (*(s16 *)D_001FF890 != 0x63)) {
        var_4 = *(s32 *)((char *)D_001FF890 + 0x18);
    }
    func_001BBF40(var_4, 1);
}
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355DD0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355DE0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355DF0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355E00);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355E10);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355E20);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355E30);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355E40);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355E50);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355E60);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355E70);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355E80);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355E90);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355EA0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355EB0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355EC0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355ED0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355EE0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355EF0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F00);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F10);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F18);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F20);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F28);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F30);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F40);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F50);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F60);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F68);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F70);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F80);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355F90);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355FA0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355FB0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355FB8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355FC0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355FC8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355FD0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355FD8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355FE0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355FE8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355FF0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00355FF8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356000);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356008);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356010);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356018);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356020);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356030);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356040);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356050);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356060);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356070);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356090);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003560A0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003560B0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003560C0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003560D0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003560E0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003560F0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003560F8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356100);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356108);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356110);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356118);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356120);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356130);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356140);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356148);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356150);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356160);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356170);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356180);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356188);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356190);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003561A0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003561B0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003561C0);


INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BBF40);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BC318);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BC810);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BCBF0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BCCF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BCD30);

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001BCD68(s32 *arg0, s32 arg1, s32 arg2) {
    s32 i = arg1 - 1;

    if (arg1 != 0) {
        do {
            *arg0 = arg2;
            i--;
            arg0++;
        } while (i != -1);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BCDA0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BCE20);

extern char D_01B1F2B0[];
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
s32 func_001BCF48(s32 arg0) {
    return *(u16 *)(D_01B1F2B0 + arg0 * 0xC4C) != 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BCF68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BCFB0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BD028);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BD0C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BD158);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BD258);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BD370);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BD4C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BD790);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BDAC0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BDB88);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BDC08);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BDC88);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BDD08);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BDD80);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BDDF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BDE70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BDEE8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BDFA0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE018);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE0B0);

void func_001BE150(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE158(void *arg0) {
    s32 temp_2;

    temp_2 = *(s32 *)((char *)arg0 + 8) - 1;
    *(s32 *)((char *)arg0 + 8) = temp_2;
    if (temp_2 <= 0) {
        *(s32 *)arg0 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE178);

extern s32 D_0034B6E0[];
extern void func_0017BC40(void);

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE1E0(s32 *arg0) {
    *(s32 *)D_0034B6E0 = 0;
    *(s32 *)((char *)D_0034B6E0 + 4) = 0;
    func_0017BC40();
    *arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE218);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE250);

extern void func_00165080(s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE2C8(void *arg0) {
    func_00165080(*(s32 *)((char *)arg0 + 8), *(s32 *)((char *)arg0 + 0x10));
    *(s32 *)arg0 = 0;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE2F8(void *arg0) {
    func_001650E8(*(s32 *)((char *)arg0 + 8), (*(s32 *)((char *)arg0 + 0x18) << 0x10) | *(s32 *)((char *)arg0 + 0x10));
    *(s32 *)arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE338);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE380);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE3D0);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE420(s32 *arg0) {
    func_00163C68();
    *arg0 = 0;
}

extern void func_001652E8(s32, f32 fparg0, f32 fparg1, f32 fparg2, f32 fparg3);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE448(void *arg0) {
    func_001652E8(*(s32 *)((char *)arg0 + 8), *(f32 *)((char *)arg0 + 0x10), *(f32 *)((char *)arg0 + 0x18), *(f32 *)((char *)arg0 + 0x20), *(f32 *)((char *)arg0 + 0x28));
    *(s32 *)arg0 = 0;
}

extern s32 func_00164198();

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE488(s32 *arg0) {
    func_00164198();
    *arg0 = 0;
}

extern void func_00164220(void);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE4B0(s32 *arg0) {
    func_00164220();
    *arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE4D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE5A0);

extern s32 func_00164890(s32, s32, s32);
extern s32 func_00164928(void);
extern s32 func_00164948();

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE678(void *arg0) {
    if (*(s32 *)((char *)arg0 + 0x84) == 0) {
        func_00164890(*(s32 *)((char *)arg0 + 8), *(s32 *)((char *)arg0 + 0x10), *(s32 *)((char *)arg0 + 0x18));
        *(s32 *)((char *)arg0 + 0x84) = 1;
    }
    if (func_00164928() != 0) {
        func_00164948();
        *(s32 *)arg0 = 0;
    }
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE6D8(s32 *arg0) {
    func_00164960();
    *arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE700);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE770);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE7A0(s32 *arg0) {
    *arg0 = 0;
}

extern s32 func_00113238(s32, s32);
extern s32 func_001BE700(s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE7A8(void *arg0) {
    func_00113238(func_001BE700(*(s32 *)((char *)arg0 + 8), *(s32 *)((char *)arg0 + 0x10)), *(s32 *)((char *)arg0 + 0x18));
    *(s32 *)arg0 = 0;
}


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE7E8(void *arg0) {
    s32 temp_3;

    func_001132B0(func_001BE700(*(s32 *)((char *)arg0 + 8), *(s32 *)((char *)arg0 + 0x10)));
    temp_3 = *(s32 *)((char *)arg0 + 0x18) - 1;
    *(s32 *)((char *)arg0 + 0x18) = temp_3;
    if (temp_3 <= 0) {
        *(s32 *)arg0 = 0;
    }
}

extern s32 func_00113288(void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE830(void *arg0) {
    void *temp_2;

    temp_2 = func_001BE700(*(s32 *)((char *)arg0 + 8), *(s32 *)((char *)arg0 + 0x10));
    func_001132B0(temp_2);
    if (func_00113288(temp_2) != 0) {
        *(s32 *)arg0 = 0;
    }
}

extern s8 D_001FFAA4[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE880(s32 *arg0) {
    D_001FFAA4[0] = 1;
    *arg0 = 0;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BE898(s32 *arg0) {
    D_001FFAA4[0] = 0;
    *arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE8A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE8F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE948);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BE9C0);

extern s32 D_0035B828;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BEA20(void *arg0) {
    if (D_0035B828 & *(s32 *)((char *)arg0 + 8)) {
        *(s32 *)arg0 = 0;
    }
}

extern s32 func_00114450(void *, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BEA40(void *arg0) {
    s32 temp_5;
    void *temp_2;

    temp_2 = func_001BE700(*(s32 *)((char *)arg0 + 8), *(s32 *)((char *)arg0 + 0x10));
    temp_5 = func_00114450(temp_2, *(s32 *)((char *)arg0 + 0x18));
    if (!(*(u16 *)((char *)temp_2 + 0xE8) & 0x8000)) {
        *(u8 *)((char *)(((temp_5 * 0xA0) + *(s32 *)((char *)temp_2 + 0xE0))) + 0x78) = (u8) *(u8 *)((char *)arg0 + 0x20);
        *(s32 *)arg0 = 0;
    }
    *(u8 *)((char *)(((temp_5 * 0xA0) + *(s32 *)((char *)temp_2 + 0xE0))) + 0x78) = (u8) *(u8 *)((char *)arg0 + 0x20);
    *(s32 *)arg0 = 0;
}

extern s32 func_00113460(void *, s32, f32 fparg0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BEAD0(void *arg0) {
    if (func_00113460(func_001BE700(*(s32 *)((char *)arg0 + 8), *(s32 *)((char *)arg0 + 0x10)), (s32) *(u8 *)((char *)arg0 + 0x18), *(f32 *)((char *)arg0 + 0x20)) == 1) {
        *(s32 *)arg0 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BEB18);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BEF10(s32 *arg0) {
    *arg0 = 0;
}

extern void func_001BE0B0(void *, double);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); the float-to-double conversion written as a cast (recipes
   2.10). Names, types and offsets are Unconfirmed readings. */
void func_001BEF18(void *arg0) {
    func_001BE0B0((char *)arg0 + 0x14, func_001E41B8(*(f32 *)((char *)arg0 + 8), *(f32 *)((char *)arg0 + 0x10)));
    *(s32 *)arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BEF68);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BEFD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF038);

extern void func_001BE018(void *, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BF150(void *arg0) {
    func_001BE018(arg0 + 0xC, func_001E9C70() % (s32) *(s32 *)((char *)arg0 + 8));
    *(s32 *)arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF198);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF238);

extern s32 D_001FF100[];

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BF260(void *arg0) {
    *(f32 *)D_001FF100 = (f32) *(f32 *)((char *)arg0 + 8);
    *(s32 *)arg0 = 0;
    *(f32 *)((char *)D_001FF100 + 0x14) = (f32) (*(f32 *)((char *)arg0 + 8) * 0.5f);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF290);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF418);

extern s32 D_0035B8F0;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BF590(void *arg0) {
    void *temp_2;

    temp_2 = D_0035B8F0 + 0x80;
    *(s32 *)arg0 = 0;
    *(f32 *)((char *)temp_2 + 0x48) = (f32) *(f32 *)((char *)arg0 + 8);
    *(f32 *)((char *)temp_2 + 0x4C) = (f32) *(f32 *)((char *)arg0 + 0x10);
}

extern s32 D_001FF310[];

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BF5B0(s32 *arg0) {
    func_001C9B18(D_0035B8F0 + 0xC0, D_001FF310);
    *arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF5E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF638);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF688);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF700);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF748);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF7C8);

extern void func_00110C00(s32, s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BF808(void *arg0) {
    func_00110C00(*(s32 *)((char *)arg0 + 8), *(s32 *)((char *)arg0 + 0x10), 0);
    *(s32 *)arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF840);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF8E0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF940);

extern void func_0012A8C8(s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BF990(void *arg0) {
    func_0012A8C8(*(s32 *)((char *)arg0 + 8), *(s32 *)((char *)arg0 + 0x10));
    *(s32 *)arg0 = 0;
}

extern void func_00121720(void);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BF9C0(s32 *arg0) {
    func_00121720();
    *arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BF9E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BFA50);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BFA90);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BFAC8);

extern char D_002488C0[]; extern void func_00165080(s32, s32);
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001BFEB8(char *arg0) {
    char *t;
    t = D_002488C0 + *(s32 *)(arg0 + 8) * 0x180;
    if (*(u8 *)(t + 0x79) < 0x50U) { func_00165080(*(u8 *)(t + 0x79), 1); *(u8 *)(t + 0x79) = 0x50; }
    *(s32 *)arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BFF20);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001BFF40(s32 *arg0) {
    *arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BFF48);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001BFFF8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C00A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C0158);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C0208);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C02B8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C0368);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C0418);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C04D8);

extern s32 func_001C0690(void *, f32 fparg0, f32 fparg1, f32 fparg2);
extern void func_001C0900(void *);

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C0570(void *arg0) {
    if ((*(s32 *)((char *)arg0 + 0x84) & 1) || (func_001C0690(arg0, *(f32 *)((char *)arg0 + 8) - *(f32 *)D_001FF340, *(f32 *)((char *)arg0 + 0x10) - *(f32 *)((char *)D_001FF340 + 4), *(f32 *)((char *)arg0 + 0x18) - *(f32 *)((char *)D_001FF340 + 8)) == 0)) {
        func_001C0900(arg0);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C05E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C0690);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C0900);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C0A10);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C0A60(f32 fparg0) {
    s32 var_2;

    var_2 = -1;
    if (!(fparg0 < 0.0f)) {
        var_2 = 0;
        if (fparg0 > 0.0f) {
            var_2 = 1;
        }
    }
    return var_2;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C0A90);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C0B28);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C0BB8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C0EE8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C10E0);

/* ER-034 (parked by ER-026). Degrees to radians, with a above 180 first taken down by 360. Needs ee-gcc r1
   (ruling docs/decomp_recipes.md 3.7: the nop; nop before div.s). Reading Unconfirmed. */
float func_001C12D0(float a) {
    if (180.0f < a) {
        a -= 360.0f;
    }
    return (a * 3.14159274101257324219f) / 180.0f;
}

/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001C1310(s32 *arg0) {
    func_001BE0B0(arg0 + 1, (f32)(func_001E9C70() & 0xFFFF) / 65535.0f);
    *arg0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C1378);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C1420);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C14C0);

extern s32 func_001D9A58(s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C1560(void) {
    return func_001D9A58(0);
}

extern s32 func_001D8700(s32 *, s32);
extern s32 D_001F6B90[];

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C1580(s32 arg0) {
    do {

    } while (func_001D8700(D_001F6B90, arg0) == 0);
    func_001D03E0(0);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C15D0);

extern s32 func_001D9498();

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C1610(void) {
    return func_001D9498();
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C1630);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C16C0);

extern s32 func_001D97C8(s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C17D0(void) {
    return func_001D97C8(1);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C17F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C1880);

extern void func_00101200(int *p, u32 n);
extern s32 D_001FEC00[];

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C18C8(s32 arg0, s32 arg1) {
    func_00101200(D_001FEC00, 0x140U);
    *(s32 *)((char *)D_001FEC00 + 0x1C) = arg0;
    *(s32 *)((char *)D_001FEC00 + 0x4A8) = arg1;
    *(s32 *)D_001FEC00 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C1928);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C1CA8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C1E38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C20B0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C20E8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C2258);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356530);


INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C2400);

void func_001C2440(void) {
}
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356560);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356570);


INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C2448);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C2B10);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003565C8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003565D8);


INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C2BB8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C2FC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C3030);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C3060);

extern s32 func_001E3228(s32, s32, s32);
extern s32 D_001FF0A0[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C3090(s32 arg0, s32 arg1) {
    return func_001E3228(D_001FF0A0[0], arg0, arg1);
}

extern s32 func_001E3150(s32, s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C30C0(s32 arg0, s32 arg1) {
    return func_001E3150(D_001FF0A0[0], arg0, arg1);
}

extern s32 func_001E2FA0(s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C30F0(void) {
    return func_001E2FA0(D_001FF0A0[0]);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C3118);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356608);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356630);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356640);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356650);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", jtbl_00356660);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", jtbl_00356680);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", jtbl_003566A0);


INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C3980);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C3CF0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C3EE8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C40F0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C4118);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C4240);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003567A8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003567B8);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003567E0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003567F0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356810);


INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C4390);

extern void func_00176388(s32);
extern void func_001C4750(char *, s32, s32);
extern s32 D_0035B82C;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C44E8(void) {
    s32 var_16;

    var_16 = 0;
    func_00176388(1);
    func_001C4750("Format failed.", 0x64, 0xD);
    if (D_0035B828 & 0x2000) {
        var_16 = !(D_0035B82C & 0x2000) ? 0 : -1;
    }
    return var_16;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C4548(void) {
    s32 var_16;

    var_16 = 0;
    func_00176388(1);
    func_001C4750("Load Data failed.", 0x64, 0xF);
    if (D_0035B828 & 0x2000) {
        var_16 = !(D_0035B82C & 0x2000) ? 0 : -1;
    }
    return var_16;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C45A8(void) {
    s32 var_16;

    var_16 = 0;
    func_00176388(1);
    func_001C4750("No saved data.", 0x64, 0xF);
    if (D_0035B828 & 0x2000) {
        var_16 = !(D_0035B82C & 0x2000) ? 0 : -1;
    }
    return var_16;
}

extern s32 D_003567F0[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C4608(void) {
    s32 var_16;

    var_16 = 0;
    func_00176388(1);
    func_001C4750((char *) D_003567F0, 0x64, 0x13);
    if (D_0035B828 & 0x2000) {
        var_16 = !(D_0035B82C & 0x2000) ? 0 : -1;
    }
    return var_16;
}

extern s32 D_00356810[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C4668(void) {
    s32 var_16;

    var_16 = 0;
    func_00176388(1);
    func_001C4750((char *) D_00356810, 0x64, 0x14);
    if (D_0035B828 & 0x2000) {
        var_16 = !(D_0035B82C & 0x2000) ? 0 : -1;
    }
    return var_16;
}

/* ER-095: drafted by ER-080 from m2c; names, types and offsets are Unconfirmed readings. Needs func_001776D8 to be
   s32 (ruling 3.4's shape, see its PORT NOTE): a void callee would let this function reuse $v0 after the call,
   where the game uses $v1 (recipes 2.20). */
s32 func_001C46C8(void) {
    s32 var_16;

    var_16 = 0;
    func_00176388(1);
    func_00175DD0(0x50, 0x64, 0x1A4, 0xC, 0xFF);
    func_001776D8(0xA0, 0x6E, "Not enough space for save.");
    func_001776D8(0xC8, 0x7A, "Min. 100KB needed.");
    if (D_0035B828 & 0x2000) {
        var_16 = !(D_0035B82C & 0x2000) ? 0 : -1;
    }
    return var_16;
}

void func_00175DD0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_001776D8(s32 arg0, s32 arg1, char *arg2);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C4750(char *arg0, s32 arg1, s32 arg2) {
    s32 temp_16;

    temp_16 = (0x1E - arg2) * 8;
    func_00175DD0(temp_16, arg1, ((arg2 + 1) * 0x14) + 0xA, 0, 0xFF);
    func_001776D8(temp_16 + 0xE, arg1 + 0xA, arg0);
}

extern void func_00175DD8(s32, s32, s32, s32, s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C47C0(u8 *arg0, s32 arg1, s32 arg2) {
    s32 temp_16;

    temp_16 = (0x1E - arg2) * 8;
    func_00175DD8(temp_16, arg1, 0xFFFFFA, ((arg2 + 1) * 0x14) + 0xA, 0, 0xFF);
    func_00177858(temp_16 + 0xE, arg1 + 0xA, arg0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C4838);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C49D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C4B40);

void func_001C4DF0(void) {
}

void func_001C4DF8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C4E00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C4E88);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C4EB0);

extern s32 D_0034D178[];

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C4F68(s32 arg0, s32 arg1) {
    *(s32 *)D_0034D178 = arg0;
    *(s32 *)((char *)D_0034D178 + 4) = arg0;
    *(s32 *)((char *)D_0034D178 + 8) = (s32) ((arg0 + arg1) - 1);
}
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003568C0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_003568D0);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356900);
INCLUDE_RODATA("asm/nonmatchings/text_00150878", D_00356930);


INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C4F88);

extern s32 D_0035C3D0;
extern s32 func_001D0050();
extern void func_001C4F68(s32 arg0, s32 arg1);
extern s32 D_0034D188[];
extern u32 func_001C4F88(s32, u32);
s32 func_001C5120(s32 arg0, s32 arg1);

/* ER-080: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C4FF0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    D_0035C3D0 = func_001D0050();
    func_001C4F68(arg1, arg2);
    *(u32 *)D_0034D188 = func_001C4F88(0x800, 0x40U);
    *(u32 *)((char *)D_0034D188 + 4) = func_001C4F88(0x4000, 0x40U);
    *(u32 *)((char *)D_0034D188 + 8) = func_001C4F88(0x13B800, 0x40U);
    *(u32 *)((char *)D_0034D188 + 0xC) = func_001C4F88(0xC000, 0x40U);
    *(u32 *)((char *)D_0034D188 + 0x10) = func_001C4F88(0x40000, 0x40U);
    *(u32 *)((char *)D_0034D188 + 0x14) = func_001C4F88(0x810, 0x40U);
    *(u32 *)((char *)D_0034D188 + 0x18) = func_001C4F88(0x578000, 0x40U);
    *(u32 *)((char *)D_0034D188 + 0x1C) = func_001C4F88(0x29B440, 0x40U);
    *(u32 *)((char *)D_0034D188 + 0x20) = func_001C4F88(0x3C00, 0x40U);
    *(u32 *)((char *)D_0034D188 + 0x24) = func_001C4F88(0x100, 0x40U);
    *(u32 *)((char *)D_0034D188 + 0x28) = func_001C4F88(0x5000C, 0x40U);
    func_001C5120(arg0, arg3);
    return 0;
}

extern s32 func_001E9D08(void *, s32 *, s32);
extern void func_001C53D8(s32);
extern s32 func_001C5188(void *, void *, s32, s32, s32);
extern s32 D_0034D068[];
extern void * D_0034D1B0[];
extern s32 D_0035C3D4;
extern s32 D_0035C3D8;
extern void func_001C56F0(void);
extern s32 D_0035C3E0;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C5120(s32 arg0, s32 arg1) {
    s32 sp[64];
    func_001E9D08(sp, "cdrom0:\\%s;1", arg0);
    func_001C53D8((s32) sp);
    func_001C5188(D_0034D068, D_0034D1B0[0], D_0035C3D4, D_0035C3D8, arg1);
    func_001C56F0();
    return D_0035C3E0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C5188);

extern void func_001D0010(s32);
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001C53D0(void) {
    func_001D0010(0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C53D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C56F0);

extern void func_001C53D0(void);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C5798(void) {
loop_1:
    func_001C53D0();
    goto loop_1;
}

extern s32 func_001E9C10(s32 *, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C57B0(s32 arg0) {
    func_001E9C10("[ Error ] %s", arg0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C57C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C5840);

extern s32 func_001D3378(s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C5908(void *arg0) {
    func_001D3378(*(s32 *)((char *)arg0 + 0x44));
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C5928);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C59C8(void *arg0, u32 arg1) {
    u32 temp_2;
    u32 temp_2_2;
    u32 temp_4;
    u32 temp_4_2;
    u32 var_5;

    var_5 = arg1;
    if (*(s32 *)arg0 == 0) {
        temp_4 = *(u32 *)((char *)arg0 + 0x2C);
        temp_2 = 0x28 - temp_4;
        temp_2_2 = (temp_2 >= var_5) ? var_5 : temp_2;
        temp_4_2 = temp_4 + temp_2_2;
        *(u32 *)((char *)arg0 + 0x2C) = temp_4_2;
        if (temp_4_2 >= 0x28U) {
            *(s32 *)arg0 = 1;
        }
        var_5 -= temp_2_2;
    }
    *(s32 *)((char *)arg0 + 0x34) = (s32) ((s32) (*(s32 *)((char *)arg0 + 0x34) + var_5) % (s32) *(s32 *)((char *)arg0 + 0x3C));
    *(s32 *)((char *)arg0 + 0x38) = (s32) (*(s32 *)((char *)arg0 + 0x38) + var_5);
    *(s32 *)((char *)arg0 + 0x40) = (s32) (*(s32 *)((char *)arg0 + 0x40) + var_5);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C5A48(void *arg0) {
    return *(s32 *)((char *)arg0 + 0x50) >= *(s32 *)((char *)arg0 + 0x48);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C5A60);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C5B38);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C5CD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C5D80);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C5EC8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C5F40);

void func_001C6158(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6160);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C64F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6530);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6660);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6720);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C67D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6828);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6880);

extern void * func_001C6880(void *, s32, s64);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C68B0(void *arg0) {
    return func_001C6880(arg0, 0x3F, 0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C68D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6938);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C69D8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C6A60(void *arg0, s64 arg1, s64 arg2) {
    return func_001C6880(arg0, 3, ((u64) (arg1 << 0x20) >> 0x20) | ((u64) (arg2 << 0x20) >> 0x10));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6A90);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C6AE0(void *arg0, s64 arg1, s64 arg2, s64 arg3) {
    return func_001C6880(arg0, 5, ((u64) (arg1 << 0x20) >> 0x20) | ((u64) (arg2 << 0x20) >> 0x10) | (arg3 << 0x20));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6B18);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6B60);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6BD8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C6C18(void *arg0, s64 arg1, s64 arg2) {
    return func_001C6880(arg0, 0x18, ((u64) (arg1 << 0x20) >> 0x20) | (arg2 << 0x20));
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C6C48(void *arg0, s64 arg1) {
    return func_001C6880(arg0, 0x1A, (s64) ((u64) (arg1 << 0x20) >> 0x20));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6C70);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6CE8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C6D48(void *arg0, s64 arg1, s64 arg2, s64 arg3) {
    return func_001C6880(arg0, 0x50, (arg3 << 0x38) | (arg2 << 0x30) | (arg1 << 0x20));
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C6D78(void *arg0, s64 arg1, s64 arg2, s64 arg3) {
    return func_001C6880(arg0, 0x51, (arg1 << 0x3B) | (arg3 << 0x30) | (arg2 << 0x20));
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C6DA8(void *arg0, s64 arg1, s64 arg2) {
    return func_001C6880(arg0, 0x52, (arg2 << 0x20) | ((u64) (arg1 << 0x20) >> 0x20));
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C6DD8(void *arg0, s64 arg1) {
    return func_001C6880(arg0, 0x53, (s64) ((u64) (arg1 << 0x20) >> 0x20));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6E00);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6E30);

extern s32 D_0035C3E4;
extern s32 D_0035C3EC;
extern void func_001C87C0(void *);
extern s32 D_0034D008[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C6F50(void) {
    if (D_0035C3E4 != 0) {
        if (D_0035C3EC != 0) {
            func_001C87C0(D_0034D008);
            D_0035C3EC = 0;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6F90);

extern s32 D_0035C3DC;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C6FD0(void) {
    D_0035C3E4 = 0;
    D_0035C3DC = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C6FE0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C7108);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C71E0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C7310(void *arg0) {
    *(s32 *)((char *)arg0 + 0x50008) = 0x50000;
    *(s32 *)((char *)arg0 + 0x50000) = 0;
    *(s32 *)((char *)arg0 + 0x50004) = 0;
}

void func_001C7328(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C7330(void *arg0, void **arg1) {
    s32 temp_3;

    temp_3 = *(s32 *)((char *)arg0 + 0x50008) - *(s32 *)((char *)arg0 + 0x50004);
    if (temp_3 != 0) {
        *arg1 = arg0 + *(s32 *)((char *)arg0 + 0x50000);
    }
    return temp_3;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C7360);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C73A8(void *arg0, void **arg1) {
    s32 temp_2;
    s32 temp_4;

    temp_2 = *(s32 *)((char *)arg0 + 0x50004);
    if (temp_2 != 0) {
        temp_4 = *(s32 *)((char *)arg0 + 0x50008);
        *arg1 = arg0 + ((s32) ((*(s32 *)((char *)arg0 + 0x50000) - temp_2) + temp_4) % temp_4);
    }
    return *(s32 *)((char *)arg0 + 0x50004);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C73F0);

extern s32 func_001D3770(void *, s32, s32 *);
extern s32 D_0035C3F8[];
extern s32 func_001CFD80(s32);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C7418(void *arg0) {
    if (func_001D3770(arg0, 0, D_0035C3F8) < 0) {
        func_001E9C10("Cannot load '%s'\n", arg0);
        func_001CFD80(1);
    }
    func_001E9C10("load '%s'\n", arg0);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C7470);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C7538);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C7578);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C75C0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C7608(s64 *arg0, s64 arg1, s64 arg2, s64 arg3) {
    *arg0 = (arg1 << 0x20) | ((u64) (arg2 << 0x20) >> 4) | ((u64) (arg3 << 0x20) >> 0x20);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C7630);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C77A8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C78A0);

extern s32 func_001D01A0(s32);
extern s32 func_001D0180(s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C7988(void *arg0, s32 arg1) {
    func_001D01A0(*(s32 *)((char *)arg0 + 0x40));
    *(s32 *)((char *)arg0 + 0x14) = (s32) (*(s32 *)((char *)arg0 + 0x14) + arg1);
    *(s64 *)((char *)arg0 + 0x48) = (s64) (arg1 + *(s64 *)((char *)arg0 + 0x48));
    func_001D0180(*(s32 *)((char *)arg0 + 0x40));
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C79D8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C7BD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C7CD8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C7FF0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8040(void *arg0) {
    s32 temp_17;

    func_001D01A0(*(s32 *)((char *)arg0 + 0x40));
    temp_17 = *(s32 *)((char *)arg0 + 0x44);
    func_001D0180(*(s32 *)((char *)arg0 + 0x40));
    return temp_17;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8080(void *arg0) {
    s32 temp_17;

    func_001D01A0(*(s32 *)((char *)arg0 + 0x40));
    temp_17 = (*(s32 *)((char *)arg0 + 0x10) << 0xB) + *(s32 *)((char *)arg0 + 0x14);
    func_001D0180(*(s32 *)((char *)arg0 + 0x40));
    return temp_17;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C80D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8120);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C81C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8388);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8460);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8600);

void func_001C8648(void) {
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C8650(void *arg0) {
    *(s32 *)((char *)arg0 + 8) = 0;
    *(s32 *)((char *)arg0 + 0xC) = 0;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8660(void *arg0) {
    return *(s32 *)((char *)arg0 + 0xC) == *(s32 *)((char *)arg0 + 0x10);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8678);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8700);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8748(void *arg0) {
    return *(s32 *)((char *)arg0 + 0xC) == 0;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8758);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C87C0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C87E0);

extern void func_001DBBF8(void);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed. */
void func_001C88F8(void) {
    func_001DBBF8();
}

extern s32 func_001DB008();

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8900(void) {
    func_001DB008();
    return 1;
}

extern void func_001C78A0(void *, s32 *, s32 *, s32 *, s32 *);
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001C8920(void *arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    func_001C78A0((char *)arg0 + 0x48, arg1, arg2, arg3, arg4);
}

extern void func_001C7988(void *arg0, s32 arg1);
/* ER-080: by hand from ER-060's m2c draft (recipes 2.10 / 2.20); names, types and offsets are Unconfirmed readings. */
void func_001C8928(void *arg0, s32 arg1) {
    func_001C7988((char *)arg0 + 0x48, arg1);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C8930(void *arg0) {
    *(s32 *)((char *)arg0 + 0xA8) = 0;
}

extern s32 func_001C7FF0(void *);
extern s32 func_001DBAD8(s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8938(s32 arg0) {
    func_001C7FF0(arg0 + 0x48);
    func_001DBAD8(arg0);
    return 1;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C8970(void *arg0) {
    *(s32 *)((char *)arg0 + 0xA8) = 1;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8980(void *arg0) {
    return *(s32 *)((char *)arg0 + 0xA8);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8988(void *arg0, s32 arg1) {
    s32 temp_2;

    temp_2 = *(s32 *)((char *)arg0 + 0xA8);
    *(s32 *)((char *)arg0 + 0xA8) = arg1;
    return temp_2;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8998);

extern s32 func_001C8080(void *arg0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C89D8(s32 arg0) {
    return func_001C8080(arg0 + 0x48);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C89F8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8A30);

s32 func_001C89D8(s32 arg0);
extern s32 func_001DBC40(s32);

/* ER-098: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8B00(s32 arg0) {
    s32 var_2;

    var_2 = 0;
    if (func_001C89D8(arg0) == 0) {
        var_2 = func_001DBC40(arg0) != 0;
    }
    return var_2;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8B48);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8BB8);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8C80);

extern void func_001C8ED0(void);
extern s32 func_001C79D8(void *);
extern s32 D_0034D0B0[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8CA8(void) {
    func_001C8ED0();
    func_001C53D0();
    func_001C79D8(D_0034D0B0);
    return 1;
}

extern s32 func_001C7BD8(void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8CE0(void) {
    func_001C7BD8(D_0034D0B0);
    return 1;
}

extern s32 func_001C7CD8(void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8D08(void) {
    func_001C7CD8(D_0034D0B0);
    return 1;
}

extern void func_001CAB10(void);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001C8D30(void) {
    func_001CAB10();
    func_001C8ED0();
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8D58);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8DA0);

extern s32 func_001C5B38(void *);
extern s32 D_0034D120[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001C8ED0(void) {
    func_001C5B38(D_0034D120);
}

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C8EE0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C92D0);

INCLUDE_ASM("asm/nonmatchings/text_00150878", func_001C9680);
