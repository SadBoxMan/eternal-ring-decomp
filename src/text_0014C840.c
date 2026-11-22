#include "common.h"

/* ER-024: this file's .sdata (docs/build.md 3.2) */
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE3C);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE40);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE44);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE48);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE4C);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE50);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE54);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE58);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE60);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE68);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE6C);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE70);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE74);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE78);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE7C);

/* ER-024: this file's .data (docs/build.md 3.2) */
INCLUDE_DATA("asm/nonmatchings/text_0014C840", D_0032FFB0);
INCLUDE_DATA("asm/nonmatchings/text_0014C840", D_0032FFC0);
INCLUDE_DATA("asm/nonmatchings/text_0014C840", D_0032FFD0);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014C840);

extern s32 func_0012B890(void **, s32, s32, s32, s16, s32, f32 fparg0);
extern s32 func_0012D9B8(void *, f32);
extern void func_00130CE0(char *, float);

/* ER-100: by hand from the m2c draft (store order by search); names, types and offsets are Unconfirmed readings. */
void func_0014C968(char *arg0) {
    f32 s;

    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        *(s16 *)(arg0 + 0x6E) += 1;
        *(f32 *)(arg0 + 0x94) = -12.0f;
        *(s32 *)(arg0 + 0x98) = 0;
        *(s32 *)(arg0 + 0x9C) = 0;
        *(s32 *)(arg0 + 0x90) = 0;
        break;
    case 1:
        func_0012B890((void **)arg0, 1, 0, *(s16 *)(arg0 + 0x6C), 0x14, 1, 0.2f);
        s = *(f32 *)(arg0 + 0x94) + 0.7f;
        *(f32 *)(arg0 + 0x94) = s;
        if (func_0012D9B8(arg0, s) != 0) {
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

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014CA60);

extern void func_0014D7A0(void **, s32);
extern void func_0014D8F8(void *arg0, s32 arg1);
extern s32 func_0014D068(void **arg0, s32 arg1);
extern s32 func_0014D890(void *arg0);

/* ER-064: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014CC10(void **arg0) {
    func_0014D8F8(arg0, (s32) func_0014D7A0);
    func_0014D068(arg0, (s32) *(s16 *)((char *)arg0 + 0x6C));
    if (*(s16 *)((char *)arg0 + 0x6C) >= 0x1F) {
        func_0014D890(arg0);
    }
}

extern void func_0014D3F8(void **, s32);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014CC68(void **arg0) {
    s16 temp_3;

    temp_3 = *(s16 *)((char *)arg0 + 0x6E);
    switch (temp_3) {                               /* irregular */
    case 0:
        *(s32 *)((char *)arg0 + 0x40) = 0;
        func_0014D8F8(arg0, (s32) func_0014D3F8);
        /* fallthrough */
    case 1:
        func_0014D068(arg0, (s32) *(s16 *)((char *)arg0 + 0x6C));
        func_0014D890(arg0);
        return;
    }
}

extern void func_0014D8F8(void *, s32);
extern void func_0014D5F0();
extern void func_0012E820(char *, float, float);
extern s32 func_0012B7C0(void **, u32, s32, s32, s16, s32, f32 fparg0);
extern f32 D_0035BE3C;

/* ER-085: by hand from the m2c draft; `case 2:` beside the default gives the game's compare tree (ruling 3.9, extended Sun 22 Nov 2026); names, types and offsets are Unconfirmed readings. */
void func_0014CCD0(void **arg0) {
    switch (*(s16 *)((char *)arg0 + 0x6E)) {
    case 0:
        func_0014D8F8(arg0, (s32) func_0014D5F0);
        *(s16 *)((char *)arg0 + 0x6E) += 1;
        /* fallthrough */
    case 1:
        func_0012E820((char *)arg0 + 0x4C, (D_0035BE3C * 3.1415927f) / 180.0f, 10.0f);
        if (func_0012B7C0(arg0, 3U, 1, *(s16 *)((char *)arg0 + 0x6C), 0x28, 0, 0.2f) != 0) {
            *(s16 *)((char *)arg0 + 0x6E) += 1;
        }
        break;
    case 2:
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014CD88);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014CF60);

extern void func_0012C3E8(void *, s32, s32, s32, f32, f32);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain; the pass-through $a0 written out); names, types and offsets are Unconfirmed readings. */
void func_0014D028(void **arg0) {
    func_0012C3E8(arg0, 0xAC, 1, *(s32 *)((char *)*arg0 + 0xE0) + 0x8F0, 0.0f, 10.0f);
}

/* ER-085: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
s32 func_0014D050(s32 arg0) {
    return (arg0 != 9) ? 9 : 8;
}


/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014D068(void **arg0, s32 arg1) {
    return func_0012B890(arg0, 0, 0, arg1, 0x28, 1, 0.08f);
}

extern void func_0012B218(void *, int);
extern void func_0014D138(void *, void *);
extern void func_0014D1E0(void *, float);
extern void func_0014D8D0(void *, int);

/* ER-015 (drafted with m2c, compared with tools/cmpfunc.py, placed with tools/place.py). Field meanings,
 * argument types and the callback's arguments are Unconfirmed readings. */
void func_0014D098(char *a0, void *a1) {
    *(int *)(a0 + 0x28) = *(int *)(a0 + 0x28) + 1;
    (*(void (**)(void *))(a0 + 0x44))(a0);
    func_0012B218(a0, 2);
    if (*(s8 *)(a0 + 0xF) != 0) {
        func_0014D8D0(a1, 1);
        func_0014D138(a0, a1);
    } else {
        func_0014D8D0(a1, 0);
        func_0012B218(a0, 4);
    }
    func_0012B218(a0, 1);
    func_0014D1E0(a1, *(float *)(a0 + 0x4C));
}

extern void func_0012C788(void *);
extern void func_0012DF30(void *, s32, s32, void *, f32 *, f32 fparg0);
extern f32 D_0032FFD0[];
extern f32 D_0035BE74;
extern f32 D_0032FFC0[];
extern f32 D_0035BE70;
extern f32 D_0032FFB0[];
extern f32 D_0035BE6C;

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014D138(void *arg0, void *arg1) {
    func_0012C788(arg1);
    func_0012DF30(arg1, 4, 3, *(void **)((char *)arg0 + 0x130), D_0032FFD0, D_0035BE74);
    func_0012C788(arg1);
    func_0012DF30(arg1, 6, 5, *(void **)((char *)arg0 + 0x130), D_0032FFC0, D_0035BE70);
    func_0012C788(arg1);
    func_0012DF30(arg1, 7, 6, *(void **)((char *)arg0 + 0x130), D_0032FFB0, D_0035BE6C);
    func_0012C788(arg1);
}

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D1E0);

extern s32 func_00150070(void **arg0, s32 arg1, s32 arg2);

/* ER-064: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014D338(void **arg0) {
    if (func_00150070(arg0, 3, 1) != 0) {
        func_0014D890(arg0);
    }
}

void func_0014D378(void) {
}

extern void func_0014D3F0(void);
extern void func_0014FE10(void *, s32);

/* ER-062: by hand from the m2c draft (the pass-through $a0 to func_0014D8F8 written out); names, types and offsets are Unconfirmed readings. */
void func_0014D380(void *arg0, s32 arg1) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 0xF) = 0;
        func_0014D8F8(arg0, (s32) func_0014D3F0);
    }
    func_0014FE10(arg0, arg1);
}

/* ER-085: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014D3D0(void **arg0) {
    func_0012B890(arg0, 1, 0, *(s32 *)((char *)arg0 + 0x28), 0x14, 1, 0.2f);
}

void func_0014D3F0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D3F8);

extern s32 func_0012FB90(s32, char *, char *);
extern void func_001CA018(float *, char *, char *);
extern float func_0012CE18(char *, char *);

float func_0014D510(char *a0, char *a1)
{
    float v[4];

    if (func_0012FB90(0, a0, a1 + 0x90)) {
        func_001CA018(v, a0 + 0x80, a1 + 0x90);
        return (0.0f <= v[0] * *(float *)(a1 + 0x50) + v[2] * *(float *)(a1 + 0x58)) ? -1.0f : 1.0f;
    }
    return (func_0012CE18(a0, a1 + 0x90) < 0.0) ? 1.0f : -1.0f;
}

extern f32 D_0035BE7C;

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014D5F0(s8 *arg0) {
    if (*(s8 *)((char *)arg0 + 0xE) == 0) {
        func_00130CE0(arg0, D_0035BE7C);
        func_0012B890((void **) arg0, 1, 0, *(s32 *)((char *)arg0 + 0x28), 0x14, 1, 0.2f);
    }
}

extern void func_0012D388(s32, f32 fparg0);
extern f32 D_0035BE78;

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014D650(s8 *arg0, s32 arg1) {
    func_00130CE0(arg0, D_0035BE7C);
    func_0012D388(arg1, D_0035BE78);
    func_0012B890((void **) arg0, 1, 0, *(s32 *)((char *)arg0 + 0x28), 0x14, 1, 0.2f);
}

extern f32 D_0035BE50;
extern f32 D_0035BE68;
extern s32 func_0012BA30(void *, s32, s32);
extern s32 func_0012B540(void *, s32, s8, s32);
extern void func_0012EA28(void *, f32, f32, f32);

/* ER-100: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014D6B0(void **arg0, void *arg1) {
    s8 st;
    s32 t;

    st = *(s8 *)((char *)arg0 + 0xE);
    t = *(s32 *)((char *)arg0 + 0x28);
    switch (st) {
    case 0:
        if (func_0012B890(arg0, 1, 0xD, t, 8, 1, 0.35f) != 0) {
            *(s16 *)((char *)arg0 + 0x6C) = 0;
            st = 1;
        }
        break;
    case 1:
        func_0012EA28((char *)arg1 + 0x94, *(f32 *)(*(char **)((char *)arg0 + 0x130) + 4) - D_0035BE50, 0.1f, D_0035BE68);
        if (*(s16 *)((char *)arg0 + 0x6C) >= 0x1F && func_0012BA30(arg1, 1, 0xD) != 0) {
            func_0012B540(arg1, 1, 0, 1);
        }
        break;
    }
    *(s8 *)((char *)arg0 + 0xE) = st;
    *(s32 *)((char *)arg0 + 0x28) = t;
}

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D7A0);

extern s32 func_001312D0(void *, s32, s32, s32, s32, s32);
extern void func_0014CA60(s8 *, void *);
extern void func_0014CD88(void **);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014D890(void *arg0) {
    *(s8 *)((char *)arg0 + 0xF) = 1;
    return func_001312D0(arg0, (s32) func_0014CA60, (s32) func_0014CA60, (s32) func_0014CD88, (s32) func_0014CD88, (s32) func_0014CD88);
}

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D8D0);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014D8E8(void *arg0, s32 arg1) {
    *(s32 *)((char *)arg0 + 0x44) = arg1;
    *(s32 *)((char *)arg0 + 0x28) = 0;
    *(s8 *)((char *)arg0 + 0xE) = 0;
}

extern void func_0014D8E8(void *arg0, s32 arg1);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014D8F8(void *arg0, s32 arg1) {
    if (*(s32 *)((char *)arg0 + 0x44) != arg1) {
        func_0014D8E8(arg0, arg1);
    }
}

/* ER-014. Types and offsets are Unconfirmed readings. */
float func_0014D918(char *a0) {
    return *(float *)(*(char **)(a0 + 0x130) + 4) - *(float *)(*(char **)a0 + 0x94);
}
