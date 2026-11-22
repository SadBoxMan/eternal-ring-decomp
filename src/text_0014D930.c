#include "common.h"

/* ER-024: this file's .sdata (docs/build.md 3.2) */
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BE90);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BE94);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BE98);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BE9C);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEA0);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEA4);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEA8);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEAC);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEB0);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEB4);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEB8);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEBC);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEC0);
INCLUDE_SDATA("asm/nonmatchings/text_0014D930", D_0035BEC4);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014D930);

void func_0014DA78(void) {
}

extern void func_00130CE0(char *, float);
extern void func_0012D388(s32, f32 fparg0);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014DA80(s8 *arg0, s32 arg1) {
    func_00130CE0(arg0, 40.0f);
    func_0012D388(arg1, 0.1f);
}

void func_0014DAB8(void) {
}

void func_0014DAC0(void) {
}

extern double fabs(double);
extern int func_0012EAB8(float *a0, float a1, float a2);
extern void func_0014DEA0(void);
void func_0014DDD8(void *arg0);
void func_0014DEE8(void *arg0, s32 arg1);
extern f32 D_0035BE90;
extern f32 D_0035BE94;
extern f32 D_0035BE98;
extern f32 D_0035BEA4;

void func_0014DAC8(char *a0, char *a1)
{
    float d, lim, v, w, r;

    if (*(s8 *)(a0 + 9) == 0) {
        *(u8 *)(a0 + 9) += 1;
    }
    d = *(float *)(a1 + 0x94) - *(float *)(a0 + 0x84);
    if (fabs(d) > fabs(*(float *)(a1 + 0x124))) {
        v = D_0035BE98 * 3.1415927f / 180.0f;
        if (!(d < 0.0f)) {
            v = -v;
        }
        func_0012EAB8((float *)(a0 + 0x44), v, D_0035BE94 * 3.1415927f / 180.0f);
        func_0014DEE8(a0, (s32)func_0014DEA0);
    } else {
        lim = D_0035BE90 * 3.1415927f / 180.0f;
        w = *(float *)(a0 + 0x44) * D_0035BEA4;
        *(float *)(a0 + 0x44) = w;
        if (lim <= w) {
            r = w;
        } else if (w <= -lim) {
            r = w;
        } else {
            r = (w < 0.0f) ? -lim : lim;
        }
        *(float *)(a0 + 0x44) = r;
        func_0014DEE8(a0, (s32)func_0014DDD8);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DC98);

extern f32 D_0035BEA8;
extern f32 D_0035BEAC;
extern f32 D_0035BEB0[];
extern int func_0012E568(int, int);
extern void func_0012EA28(void *, f32, f32, f32);

/* ER-062: by hand from the m2c draft (plain form; %hi / %lo data as recipe 2.6); names, types and offsets are Unconfirmed readings. */
void func_0014DDD8(void *arg0) {
    s32 temp_2;
    s8 temp_3;

    temp_3 = *(s8 *)((char *)arg0 + 0xA);
    switch (temp_3) {
    case 1:
        break;
    case 0:
        *(s32 *)((char *)arg0 + 0x24) = func_0012E568(0x1E, 0xB4);
        *(s8 *)((char *)arg0 + 0xA) = func_0012E568(1, 3);
        break;
    case 2:
        func_0012EA28(arg0 + 0x4C, D_0035BEA8, 0.01f, D_0035BEB0[0]);
        break;
    case 3:
        func_0012EA28(arg0 + 0x4C, D_0035BEAC, 0.01f, D_0035BEB0[0]);
        break;
    }
    temp_2 = *(s32 *)((char *)arg0 + 0x24) - 1;
    *(s32 *)((char *)arg0 + 0x24) = temp_2;
    if (temp_2 < 0) {
        *(s8 *)((char *)arg0 + 0xA) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DEA0);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014DEB8(void *arg0, s32 arg1) {
    *(s32 *)((char *)arg0 + 0x3C) = arg1;
    *(s32 *)((char *)arg0 + 0x28) = 0;
    *(s8 *)((char *)arg0 + 9) = 0;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014DEC8(void *arg0, s32 arg1) {
    *(s32 *)((char *)arg0 + 0x38) = arg1;
    *(s32 *)((char *)arg0 + 0x2C) = 0;
    *(s8 *)((char *)arg0 + 8) = 0;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014DED8(void *arg0, s32 arg1) {
    *(s32 *)((char *)arg0 + 0x40) = arg1;
    *(s32 *)((char *)arg0 + 0x30) = 0;
    *(s8 *)((char *)arg0 + 0xA) = 0;
}

extern void func_0014DED8(void *arg0, s32 arg1);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014DEE8(void *arg0, s32 arg1) {
    if (*(s32 *)((char *)arg0 + 0x40) != arg1) {
        func_0014DED8(arg0, arg1);
    }
}

extern void func_0012FAE8(void *, int, int, int, float);
extern float func_00131608(float a, float b);
extern s32 func_00131088(void *);
extern void func_0014E0A8(char *);
extern void func_0014E258(char *, char *);

/* ER-077: by hand from the m2c draft. 0.69813174f is 40 degrees in radians; the fabs() compare is done in double,
 * so its constant is gcc's anonymous .rodata double in place of D_00354A28 (ruling 3.10). The clamp reads the field
 * three times (a CLAMP-macro shape; a local copy gives other registers). Names, types and offsets are Unconfirmed. */
void func_0014DF08(char *a0, char *a1)
{
    *(int *)(a0 + 0x28) += 1;
    (*(void (**)(char *, char *))(a0 + 0x3C))(a0, a1);
    *(int *)(a0 + 0x2C) += 1;
    (*(void (**)(char *, char *))(a0 + 0x38))(a0, a1);
    *(int *)(a0 + 0x30) += 1;
    (*(void (**)(char *, char *))(a0 + 0x40))(a0, a1);
    func_0014E0A8(a1);
    *(float *)(a1 + 0xA0) = func_00131608(*(float *)(a1 + 0xA0), *(float *)(a0 + 0x44));
    if (fabs(*(float *)(a1 + 0xA0)) > 0.69813174f) {
        *(float *)(a1 + 0xA0) = (*(float *)(a1 + 0xA0) < -0.69813174f) ? -0.69813174f
                              : (*(float *)(a1 + 0xA0) > 0.69813174f) ? 0.69813174f
                              : *(float *)(a1 + 0xA0);
        *(float *)(a0 + 0x44) *= 0.5f;
    }
    *(float *)(a1 + 0xA4) = func_00131608(*(float *)(a1 + 0xA4), *(float *)(a0 + 0x48));
    func_0012D388((s32)a1, *(float *)(a0 + 0x4C));
    if (func_00131088(a0) != *(int *)(a0 + 0xCC)) {
        func_0014E258(a0, a1);
        func_0012FAE8(a0, 2, 0x7F, *(int *)a0 + 0x90, 1200.0f);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014E0A8);

void func_0014E118(void) {
}

void func_0014E2C8(char *a0);
extern void func_0012FB48(void *arg0, s32 arg1);

/* ER-083: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014E120(s8 *arg0) {
    func_0014E2C8(arg0);
    func_0012FB48(arg0, 0);
}

extern void func_00131000(char *, void *);
void func_0014DA78(void);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014E150(s8 *arg0) {
    func_00131000(arg0, func_0014DA78);
}

void func_0014DEB8(void *arg0, s32 arg1);
void func_0014DAC0(void);
void func_0014DEC8(void *arg0, s32 arg1);
extern s32 func_0012D9B8(void *, f32 fparg0);
extern void func_001321E8(void *arg0);

/* ER-083: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014E160(s8 *arg0) {
    f32 temp_f0;

    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s32 *)((char *)arg0 + 0x44) = 0;
        *(s32 *)((char *)arg0 + 0x48) = 0;
        *(s32 *)((char *)arg0 + 0x4C) = 0;
        func_0014DEB8(arg0, (s32) func_0014DAC0);
        func_0014DEC8(arg0, (s32) func_0014DAC0);
        func_0014DED8(arg0, (s32) func_0014DAC0);
        func_0014E2C8(arg0);
        func_0012FB48(arg0, 1);
        *(u8 *)((char *)arg0 + 0x5A) = (u8) (*(u8 *)((char *)arg0 + 0x5A) | 8);
        *(f32 *)((char *)arg0 + 0xB0) = 1.0f;
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
        *(s32 *)((char *)arg0 + 0x90) = 0;
        *(f32 *)((char *)arg0 + 0x94) = 0.0f;
        *(s32 *)((char *)arg0 + 0x98) = 0;
        *(s32 *)((char *)arg0 + 0x9C) = 0;
    }
    temp_f0 = *(f32 *)((char *)arg0 + 0x94) + *(f32 *)((char *)arg0 + 0xB0);
    *(f32 *)((char *)arg0 + 0x94) = temp_f0;
    if (func_0012D9B8(arg0, temp_f0) != 0) {
        func_001321E8(arg0);
    }
}

void func_0014E248(void) {
}

void func_0014E250(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014E258);

/* ER-014. Types and offsets are Unconfirmed readings. */
void func_0014E2C8(char *a0) {
    char *p = *(char **)(a0 + 0x14);

    if (p != NULL) {
        *(int *)(a0 + 0x14) = 0;
        *(int *)(p + 0x10) = 1;
    }
}
