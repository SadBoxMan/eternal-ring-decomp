#include "common.h"

/* ER-024: this file's .sdata (docs/build.md 3.2) */
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BEC8);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BECC);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BED0);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BED4);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BED8);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BEDC);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BEE0);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BEE4);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BEE8);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BEEC);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BEF0);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BEF4);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BF04);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BF08);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BF0C);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BF10);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BF14);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BF18);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BF1C);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BF20);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BF24);
INCLUDE_SDATA("asm/nonmatchings/text_0014E2E8", D_0035BF28);

/* ER-024: this file's .data (docs/build.md 3.2) */
INCLUDE_DATA("asm/nonmatchings/text_0014E2E8", D_0032FFE0);
INCLUDE_DATA("asm/nonmatchings/text_0014E2E8", D_0032FFF0);
INCLUDE_DATA("asm/nonmatchings/text_0014E2E8", D_00330000);
INCLUDE_DATA("asm/nonmatchings/text_0014E2E8", D_00330050);
INCLUDE_DATA("asm/nonmatchings/text_0014E2E8", D_00330068);
INCLUDE_DATA("asm/nonmatchings/text_0014E2E8", D_00330080);
INCLUDE_DATA("asm/nonmatchings/text_0014E2E8", D_00330190);
INCLUDE_DATA("asm/nonmatchings/text_0014E2E8", D_003301F0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014E2E8);

extern s32 func_0012B540(void *, s32, s8, s32);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014E3E8(void *arg0, void *arg1) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        func_0012B540(arg1, 0, 0, 1);
        func_0012B540(arg1, 1, 0, 1);
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014E450);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014E608);

extern void func_0014E740();
extern s32 func_0014F298(char *, s32, s32, s32, f32);
extern s32 func_0014EAD0(s8 *, s32);
extern void func_00131000(char *, void *);
extern void func_0014E608();

/* ER-083: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014E740(s8 *arg0) {
    func_0014F298(arg0, 0xB, 0xF, 0, 0.1f);
    func_0014EAD0(arg0, 1);
    if (*(s16 *)((char *)arg0 + 0x6C) >= 0x3D) {
        func_00131000(arg0, func_0014E608);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014E7A8);

int func_0014F618(void *a0);
void func_0014EA70(void *arg0);
extern s32 func_0012BA30(void *, s32, s32);
extern s32 func_0012E568(s32, s32);

/* ER-083: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014E960(void *arg0, void *arg1) {
    s16 temp_3;

    func_0014EAD0(arg0, 1);
    temp_3 = *(s16 *)((char *)arg0 + 0x6E);
    switch (temp_3) {                               /* irregular */
    case 0:
        if (func_0014F618(arg0) == 6) {
            func_00131000(arg0, &func_0014E608);
            return;
        }
        if (func_0014F298(arg0, 0xC, 0x23, 0, 0.2f) != 0) {
            if (*(u16 *)((char *)(*(void **)((char *)arg1 + 0xE0)) + 0x62C) == 0xE) {
                func_0014EA70(arg0);
            }
            if (func_0012BA30(arg1, 0, 0xC) != 0) {
                *(s16 *)((char *)arg0 + 0x6C) = 0;
                *(s32 *)((char *)arg0 + 0x24) = func_0012E568(0x1E, 0xB4);
                *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
            }
        }
        return;
    case 1:
        if (*(s16 *)((char *)arg0 + 0x6C) >= 0x15) {
            func_00131000(arg0, &func_0014E608);
        }
        break;
    }
}

extern void *func_0012C630(void *, s32, s32, s32, s32 *);
extern s32 D_0032FFF0[];

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014EA70(void *arg0) {
    s32 temp_2;
    void *temp_2_2;

    temp_2 = func_0014F618(arg0);
    if (temp_2 != 6) {
        temp_2_2 = func_0012C630(arg0, (temp_2 + 0x9B) & 0xFFFF, 1, 0x14, D_0032FFF0);
        if (temp_2_2 != NULL) {
            *(s32 *)((char *)temp_2_2 + 0x24) = temp_2;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014EAD0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014EC58);

extern s32 func_0014F210(void *arg0);
extern void func_0014F5F8(s8 *);
extern void func_0014E450(void **, void *);
extern s32 func_0014F240(void *);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014EDF8(s8 *arg0) {
    if (func_0014F210(arg0) != 0) {
        if (func_0014F618(arg0) == 6) {
            func_0014F5F8(arg0);
            return 2;
        }
        func_00131000(arg0, func_0014E450);
        return 2;
    }
    return func_0014F240(arg0);
}

extern s32 func_00111660(void *);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014EE68(s8 *arg0, void *arg1) {
    s32 temp_16;

    if (func_0014F210(arg0) != 0) {
        temp_16 = func_0014F618(arg0);
        if (temp_16 == func_00111660(arg1)) {
            func_0014F5F8(arg0);
            return 2;
        }
        func_00131000(arg0, func_0014E450);
        return 2;
    }
    return func_0014F240(arg0);
}

extern double fabs(double);
extern s32 func_00131088(void *);
extern void func_0012CB70(float *, char *, char *);
extern float func_00130758(char *);
extern void func_001CA018(float *, char *, char *);
extern void func_00130180(float *, char *, float *);
extern void func_0012D8B8(char *, double, double, double);
extern void func_001D76D0(float *, float *, float);
extern void func_001C9EE8(float *, float *, char *);
extern void func_001C9F60(void *, void *, void *);
extern void func_001C9FE8(void *, void *, void *);
void func_0014F480(void *arg0, f32 fparg0, f32 fparg1, f32 fparg2);
extern float D_001F5780[];
extern f32 D_0035BEE4;

/* ER-077: by hand from the m2c draft. fabs(v[0]) is compared in double, so 300.0 is gcc's anonymous .rodata
 * double in place of D_00354A68 (ruling 3.10); 0.69813174f is 40 degrees in radians. The callees' meanings
 * (vector, matrix helpers) are not read. Names, types and offsets are Unconfirmed. */
void func_0014EEF0(char *a0, char *a1, char *a2)
{
    float v[4];
    float w[4];
    float u[4];
    float p[4];
    float m[16];
    float r, s;

    if (func_00131088(a0) != (s32)func_0014E608) {
        return;
    }
    if (*(char **)(a1 + 8) == a0) {
        return;
    }
    func_0012CB70(v, *(char **)a0, a1 + 0x50);
    if (fabs(v[0]) > 300.0f) {
        return;
    }
    if (v[1] > 100.0f || v[1] < -100.0f) {
        return;
    }
    if (v[2] > 800.0f || v[2] <= 0.0f) {
        return;
    }
    if (func_00130758(a0) <= 400.0f) {
        return;
    }
    func_001CA018(w, a1 + 0x50, a2 + 0x10);
    func_00130180(w, *(char **)a0, w);
    if (0.0f <= v[0]) {
        r = 0.69813174f;
        s = -D_0035BEE4;
    } else {
        s = D_0035BEE4;
        r = -0.69813174f;
    }
    *(float *)(a0 + 0x3C) = r;
    func_0012D8B8(a0, s, 0.0, D_0035BEE4);
    func_001D76D0(m, D_001F5780, r);
    func_001C9EE8(m, m, *(char **)a0 + 0x50);
    u[1] = -300.0f;
    u[2] = 200.0f;
    u[0] = 0.0f;
    u[3] = 0.0f;
    func_001C9F60(p, m, u);
    func_001C9FE8(p, *(char **)a0 + 0x90, p);
    func_0014F480(a0, p[0], p[1], p[2]);
    func_0014F298(a0, 0xB, 0xF, 0, 0.1f);
    func_00131000(a0, func_0014E740);
}

extern f32 D_0035BECC;
extern s32 func_00150070(void **arg0, s32 arg1, s32 arg2);
extern void func_0014E608();

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014F160(s8 *arg0) {
    *(f32 *)((char *)arg0 + 0x94) = (f32) (*(f32 *)((char *)arg0 + 0x94) + D_0035BECC);
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 0xD) = 0;
    }
    *(f32 *)((char *)arg0 + 0x94) = (f32) (*(f32 *)((char *)arg0 + 0x94) + 0.1f);
    if (func_00150070(arg0, 3, 1) != 0) {
        func_00131000(arg0, &func_0014E608);
    }
}

extern void func_001321E8(char *);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014F1D8(s8 *arg0) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    if (*(s16 *)((char *)arg0 + 0x6C) >= 0x3D) {
        func_001321E8(arg0);
    }
}

extern s32 func_00131088(void *);
extern void func_0014E3E8(void *arg0, void *arg1);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014F210(void *arg0) {
    return func_00131088(arg0) == (s32) func_0014E3E8;
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F240);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F298);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F2E0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F328);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014F480(void *arg0, f32 fparg0, f32 fparg1, f32 fparg2) {
    *(f32 *)((char *)arg0 + 0x40) = fparg0;
    *(f32 *)((char *)arg0 + 0x44) = fparg1;
    *(f32 *)((char *)arg0 + 0x48) = fparg2;
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F490);

extern void func_0012F9C0(void **, s32);
extern s32 func_0012B180(void *, s32);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
f32 func_0014F550(void **arg0, s32 arg1) {
    u16 temp_17;
    void *temp_4;

    temp_4 = *(void **)arg0;
    if (*(u8 *)((char *)temp_4 + 0x149) != *(s8 *)((char *)arg0 + 0xF)) {
        return 0.0f;
    }
    temp_17 = *(u16 *)((char *)(*(void **)((char *)temp_4 + 0xE0)) + 0x6CC);
    if ((arg1 != 0) && (temp_17 == 0x14)) {
        func_0012F9C0(arg0, 2);
    }
    return (2.0f * ((f32) temp_17 / (f32) func_0012B180(*(void **)arg0, (s32) *(s8 *)((char *)arg0 + 0xF)))) + -1.0f;
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F5F8);

/* ER-024 (parked by ER-014 for its jump table). Maps the u16 at offset 4 from 0x34..0x39 to 0..5, else 6;
 * the field's meaning is Unconfirmed. */
int func_0014F618(void *a0) {
    switch (*(u16 *)((char *)a0 + 4)) {
    case 0x34:
        return 0;
    case 0x35:
        return 1;
    case 0x36:
        return 2;
    case 0x37:
        return 3;
    case 0x38:
        return 4;
    case 0x39:
        return 5;
    }
    return 6;
}

extern void func_0014F690(s8 *, void *);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014F680(s8 *arg0) {
    func_00131000(arg0, func_0014F690);
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F690);

extern s32 func_0012B890(void **, s32, s32, s32, s16, s32, f32 fparg0);
extern void func_0012FA18(void *, s32, float);
void func_0014FBA0(void *arg0, f32 fparg0);
extern void func_0014F7F8();

/* ER-100: by hand from the m2c draft; func_0014FBA0 gets arg0 passed through and 0.0f, func_0012FA18 is
   (void *, s32, float) (its asm reads $a1); names, types and offsets are Unconfirmed readings. */
void func_0014F728(char *arg0, char *arg1) {
    func_0014FBA0(arg0, 0.0f);
    if (*(s16 *)(arg0 + 0x6E) == 0) {
        *(s16 *)(arg0 + 0x74) = func_0012E568(0x96, 0xF0);
        *(s16 *)(arg0 + 0x6E) += 1;
    }
    func_0012B890((void **)arg0, 0, 0, *(s16 *)(arg0 + 0x6C), 0x14, 0, 0.3f);
    if (*(u16 *)(*(char **)(arg1 + 0xE0) + 0x26C) == 0x78) {
        func_0012FA18(arg0, 2, 100000.0f);
    }
    if (func_0012BA30(arg1, 0, 0) != 0) {
        func_00131000(arg0, func_0014F7F8);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F7F8);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FA18);

/* ER-085: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014FBA0(void *arg0, f32 fparg0) {
    void *temp_3 = *(void **)((char *)arg0 + 0x130);
    *(s8 *)((char *)arg0 + 0xE) = 1;
    *(f32 *)((char *)arg0 + 0x3C) = *(f32 *)temp_3;
    *(f32 *)((char *)arg0 + 0x40) = *(f32 *)((char *)temp_3 + 4);
    *(f32 *)((char *)arg0 + 0x44) = *(f32 *)((char *)temp_3 + 8);
    *(f32 *)((char *)arg0 + 0x48) = fparg0;
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FBD0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FC68);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014FDF8(void) {
    return 2;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014FE00(void) {
    return 2;
}

extern s32 func_0014FE10(void *, s32);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed. */
/* ER-083: passes its arguments on to func_0014FE10 (recipes 2.13; same `j`). */
void func_0014FE08(void *arg0, s32 arg1) {
    func_0014FE10(arg0, arg1);
}

extern s32 func_0014FE30(void *, s32, s32);

/* ER-083: drafted with m2c (tools/m2c_bulk.py, return-wrapper form); names, types and offsets are Unconfirmed readings. */
s32 func_0014FE10(void *arg0, s32 arg1) {
    return func_0014FE30(arg0, arg1, 2);
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FE30);

extern s32 func_0014FF98(void *arg0, s32 arg1, s32 arg2);

/* ER-062: drafted with m2c (tools/m2c_bulk.py --plain, own run); names, types and offsets are Unconfirmed readings. */
s32 func_0014FF38(void *arg0, s32 arg1) {
    return func_0014FF98(arg0, arg1, 1);
}

/* ER-062: drafted with m2c (tools/m2c_bulk.py --plain, own run); names, types and offsets are Unconfirmed readings. */
s32 func_0014FF58(void **arg0, s32 arg1) {
    return func_00150070(arg0, arg1, 1);
}

extern s32 func_001500C8(void **, s32, s32);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014FF78(void **arg0, s32 arg1) {
    return func_001500C8(arg0, arg1, 1);
}

extern s32 func_001307A0(void *, s32);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014FF98(void *arg0, s32 arg1, s32 arg2) {
    f32 temp_f3;
    void *temp_5;

    temp_5 = *(void **)arg0;
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(f32 *)((char *)arg0 + 0x30) = 10.0f;
        func_001307A0(arg0 + 0x90, temp_5 + 0x90);
    } else {
        temp_f3 = *(f32 *)((char *)arg0 + 0x30) * 0.9f;
        *(f32 *)((char *)arg0 + 0x30) = temp_f3;
        if (0.01f <= temp_f3) {
            *(f32 *)((char *)temp_5 + 0x90) = (f32) (*(f32 *)((char *)temp_5 + 0x90) + (temp_f3 * *(f32 *)((char *)arg0 + 0x90)));
            *(f32 *)((char *)temp_5 + 0x94) = (f32) (*(f32 *)((char *)temp_5 + 0x94) + (*(f32 *)((char *)arg0 + 0x30) * *(f32 *)((char *)arg0 + 0x94)));
            *(f32 *)((char *)temp_5 + 0x98) = (f32) (*(f32 *)((char *)temp_5 + 0x98) + (*(f32 *)((char *)arg0 + 0x30) * *(f32 *)((char *)arg0 + 0x98)));
        }
    }
    return func_00150070(arg0, arg1, arg2);
}

extern void func_0012FA68(void);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00150070(void **arg0, s32 arg1, s32 arg2) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        func_0012FA68();
    }
    return func_001500C8(arg0, arg1, arg2);
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_001500C8);

extern void func_0012FB48(void *, int);

/* ER-062: by hand from the m2c draft (the pass-through $a0 written out); names and types are Unconfirmed readings. */
void func_001501D8(void *arg0) {
    func_0012FB48(arg0, 0);
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_001501E0);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00150220(void *arg0, void *arg1) {
    *(f32 *)((char *)arg0 + 0x40) = (f32) *(f32 *)arg1;
    *(f32 *)((char *)arg0 + 0x44) = (f32) *(f32 *)((char *)arg1 + 4);
    *(f32 *)((char *)arg0 + 0x48) = (f32) *(f32 *)((char *)arg1 + 8);
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_00150240);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_00150280);

extern void func_00150658(void *, void *);
extern s32 func_001503C0(s8 *arg0);
extern s32 func_001503E0(s8 *arg0);
extern void func_001501D8(void *);
extern void func_0012B1B8(void *, s32, s32, s8);
extern void func_00150350(s32 **);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001502C0(s8 *arg0) {
    *(s8 *)((char *)arg0 + 0xF) = 0;
    *(s32 *)((char *)arg0 + 0x4C) = 0;
    *(void (**)(void *, void *))((char *)arg0 + 0xC0) = func_00150658;
    *(s32 (**)(s8 *))((char *)arg0 + 0xD0) = func_001503C0;
    *(s32 (**)(s8 *))((char *)arg0 + 0xD4) = func_001503E0;
    *(void (**)())((char *)arg0 + 0xE0) = func_001501D8;
    func_0012B1B8(*(void **)arg0, 4, 9, 2);
    func_0012B1B8(*(void **)arg0, 0xA, 0xB, 4);
    func_00131000(arg0, func_00150350);
}

void func_00150818(int **a0, int a1, int a2);
extern s32 func_0012B7C0(void **, u32, s32, s32, s16, s32, f32 fparg0);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00150350(s32 **arg0) {
    if ((*(s16 *)((char *)arg0 + 0x6E) == 0) && (*(s16 *)((char *)arg0 + 0x6C) >= 0x79)) {
        func_00150818(arg0, 0, 0x7F);
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    func_0012B7C0((void **) arg0, 7U, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0xA, 1, 0.3f);
}

extern void func_001507B8(void *arg0);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001503C0(s8 *arg0) {
    func_001507B8(arg0);
    return 1;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001503E0(s8 *arg0) {
    func_001507B8(arg0);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_00150400);


/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001505F8(void **arg0) {
    *(s8 *)((char *)arg0 + 0xF) = 0;
    if (func_0012B7C0(arg0, 7U, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x78, 1, 0.1f) != 0) {
        func_00131000((s8 *) arg0, func_00150350);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_00150658);

extern void func_00150350();
extern void func_00150400();
extern void func_0012FB48(void *, int);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain), declarations aligned with this file by hand (m2c left
 * out arg0, which passes through in $a0); names, types and offsets are Unconfirmed readings. */
void func_001507B8(void *arg0) {
    if (func_00131088(arg0) == (s32) func_00150350) {
        func_0012FB48(arg0, 0);
        func_00131000(arg0, func_00150400);
    }
}

extern float D_0035BF20;
extern void func_0012FB48(void *, int);
extern void func_0012FAE8(void *, int, int, int, float);

/* ER-014. Arguments Unconfirmed ($a0-$a3 pass through unchanged). ER-100: func_0012FA18 reads $a1, so the
   wrapper passes its second argument on. */
void func_00150810(void *a0, s32 a1) {
    func_0012FA18(a0, a1, D_0035BF20);
}

/* ER-014. Types and offsets are Unconfirmed readings. */
void func_00150818(int **a0, int a1, int a2) {
    func_0012FB48(a0, 0);
    func_0012FAE8(a0, a1, a2, *(int *)((char *)*a0 + 0xE0) + 0x5D0, D_0035BF20);
}
