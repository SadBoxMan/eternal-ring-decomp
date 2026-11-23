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

void func_0014F480(void *arg0, f32 fparg0, f32 fparg1, f32 fparg2);
extern void func_0012B1B8(void *, s32, s32, s8);
extern void func_00131000(char *, void *);
extern void func_0014E608();
extern void func_0014F160();
extern void func_0014EC58();
extern s32 func_0014EDF8();
extern s32 func_0014EE68();
extern void func_0014EEF0();
extern void func_0014E3E8();

/* ER-100: by hand from the m2c draft (store order by search); names, types and offsets are Unconfirmed readings. */
void func_0014E2E8(char *arg0) {
    *(s8 *)(arg0 + 0xC) = 0;
    *(s32 *)(arg0 + 0x28) = 0;
    *(s32 *)(arg0 + 0x2C) = 0;
    *(s32 *)(arg0 + 0x24) = 0;
    *(s32 *)(arg0 + 0x3C) = 0;
    *(s8 *)(arg0 + 0xE) = -1;
    *(s8 *)(arg0 + 0xF) = -1;
    func_0014F480(arg0, *(f32 *)(arg0 + 0x80), *(f32 *)(arg0 + 0x84), *(f32 *)(arg0 + 0x88));
    func_0012B1B8(*(void **)arg0, 0xA, 0xE, 2);
    func_0012B1B8(*(void **)arg0, 0x1A, 0x1E, 2);
    *(void **)(arg0 + 0xC4) = func_0014F160;
    *(void **)(arg0 + 0xC8) = func_0014F160;
    *(void **)(arg0 + 0xC0) = func_0014EC58;
    *(void **)(arg0 + 0xD0) = func_0014EDF8;
    *(void **)(arg0 + 0xD4) = func_0014EE68;
    *(void **)(arg0 + 0xD8) = func_0014EEF0;
    *(f32 *)(arg0 + 0xE4) = 1000000.0f;
    *(u8 *)(arg0 + 0x5A) |= 8;
    if (*(u16 *)(arg0 + 4) == 0x26) {
        func_00131000(arg0, func_0014E608);
        return;
    }
    func_00131000(arg0, func_0014E3E8);
}

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

void func_0014F490(char *arg0, f32 fparg0, f32 fparg1, f32 fparg2);
s32 func_0014F298(char *arg0, s8 arg1, s32 arg2, s32 arg3, f32 fparg0);
extern s32 func_0014EAD0(s8 *, s32);
int func_0014F618(void *a0);
extern s32 func_00131498();
void func_0014E7A8(char *arg0, char *arg1);
void func_0014E960(void *arg0, void *arg1);

void func_0014E608(char *arg0) {
    s32 r;

    if (func_0014F618(arg0) == 6) {
        func_0014F490(arg0, 0.0f, 0.0f, 50.0f);
    } else {
        func_0014F480(arg0, *(f32 *)(arg0 + 0x80), *(f32 *)(arg0 + 0x84) - 200.0f, *(f32 *)(arg0 + 0x88));
    }
    r = func_0014EAD0((s8 *)arg0, 1);
    if (func_0014F298(arg0, 1, 0xF, 1, 0.2f) == 0) {
        *(s8 *)(arg0 + 0xC) = 1;
        return;
    }
    switch (r) {
    case 0:
    case 1:
        func_00131000(arg0, func_0014E7A8);
        break;
    case 2:
        if (*(s32 *)(arg0 + 0x24) < 0) {
            func_00131000(arg0, func_0014E960);
        }
        break;
    case 3:
        if (*(s32 *)(arg0 + 0x24) < 0) {
            if (func_00131498() < 0x14) {
                func_00131000(arg0, func_0014E960);
            }
        }
        break;
    case 4:
        break;
    }
}

extern void func_0014E740();
extern s32 func_0014F298(char *, s8, s32, s32, f32);

/* ER-083: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014E740(s8 *arg0) {
    func_0014F298(arg0, 0xB, 0xF, 0, 0.1f);
    func_0014EAD0(arg0, 1);
    if (*(s16 *)((char *)arg0 + 0x6C) >= 0x3D) {
        func_00131000(arg0, func_0014E608);
    }
}

extern s32 func_0012F9C0(void **, s32);
extern s32 func_0012FC38(void **, s32, s32, s32);
extern s32 func_0012BA30(void *, s32, s32);

/* ER-120: by hand from the m2c draft (helper agent); names, types and offsets are Unconfirmed readings. */
void func_0014E7A8(char *arg0, char *arg1) {
    s32 r;
    u16 t;

    func_0014F490(arg0, 0.0f, 0.0f, 50.0f);
    r = func_0014EAD0((s8 *)arg0, 0);
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        if (r == 0) {
            *(s16 *)(arg0 + 0x6E) += 1;
        } else if (r >= 2) {
            func_00131000(arg0, func_0014E608);
        }
        break;
    case 1:
        if (func_0014F298(arg0, 0xA, 0xF, 0, 0.2f) != 0) {
            if (r != 0) {
                func_00131000(arg0, func_0014E608);
            } else {
                *(s8 *)(arg0 + 8) = 0;
                *(s8 *)(arg0 + 9) = 0;
                *(s16 *)(arg0 + 0x6E) += 1;
            }
        }
        break;
    case 2:
        t = *(u16 *)(*(char **)(arg1 + 0xE0) + 0x62C);
        if (t == 4 || t == 0x11) {
            func_0012F9C0((void **)arg0, 4);
        }
        if (*(s8 *)(arg0 + 9) == 0) {
            if (*(s8 *)(arg0 + 8) == 0) {
                if (func_0012FC38((void **)arg0, 0, 5, 0xE) != 0) {
                    *(s8 *)(arg0 + 8) = 1;
                }
            }
            if (func_0012FC38((void **)arg0, 0, 0x12, 0x1C) != 0) {
                *(s8 *)(arg0 + 9) = 1;
            }
        }
        if (func_0012BA30(arg1, 0, 0xA) != 0) {
            func_00131000(arg0, func_0014E608);
        }
        break;
    }
    func_0014EAD0((s8 *)arg0, 1);
}

void func_0014EA70(void *arg0);
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

extern void func_00130CE0(char *, float);
s32 func_0014F2E0(char *arg0, s8 arg1, s32 arg2, s32 arg3, f32 fparg0);
f32 func_0014F550(void **arg0, s32 arg1);
extern void func_00130180(float *, char *, float *);
extern void func_001CA048(float *, float *, float);
extern void func_001CA0C0(void *, void *);
extern void func_00130B18(char *, f32, f32, f32);
extern void func_00130BF8(char *, f32);
extern float func_0012D470(char *a0, float *d, float max);
extern int func_00130780(char *);
extern f32 D_0035BED4;
extern f32 D_0035BED8;
extern f32 D_0035BEDC;
extern f32 D_0035BEE0;
extern f32 D_0035BEE4;

s32 func_0014EAD0(s8 *arg0, s32 arg1) {
    f32 v[4];
    f32 d[4];
    f32 w[4];
    char *p;
    f32 f;
    s32 r;

    p = *(char **)arg0;
    func_00130CE0((char *)arg0, 40.0f);
    *(s8 *)(arg0 + 0xC) = 0;
    r = func_0014F2E0((char *)arg0, 1, 0xF, 1, 0.2f);
    if (r != 0) {
        f = func_0014F550((void **)arg0, arg1);
        v[0] = *(f32 *)(arg0 + 0x40) - *(f32 *)(p + 0x90);
        v[1] = *(f32 *)(arg0 + 0x44) - *(f32 *)(p + 0x94);
        v[2] = *(f32 *)(arg0 + 0x48) - *(f32 *)(p + 0x98);
        v[3] = 0.0f;
        func_00130180(v, p, v);
        if (f < 0.0f) {
            func_001CA0C0(w, v);
            w[1] = 0.0f;
            func_001CA048(w, w, -f * D_0035BEDC);
            func_00130B18((char *)arg0, w[0], w[2], D_0035BEE4);
        } else {
            func_00130BF8((char *)arg0, D_0035BEE0);
        }
        d[0] = 0.0f;
        d[1] = 0.0f;
        d[2] = 0.0f;
        d[3] = 0.0f;
        if (v[1] < d[0] && f < d[0]) {
            *(s8 *)(arg0 + 0xC) = 1;
            d[1] = -D_0035BED4;
        }
        func_0012D470((char *)arg0, d, D_0035BED8);
        r = func_00130780((char *)arg0);
    }
    return r;
}

extern void func_0012EA28(void *, f32, f32, f32);
extern void func_0012B218(void *, int);
extern void func_0012B1F0(void *arg0, s32 arg1, s32 arg2);
extern void func_0012E820(char *, float, float);
extern void func_0012C788(void *);
extern void func_0012DF30(void *, s32, s32, void *, f32 *, f32 fparg0);
extern void func_0014F328(char *, f32);
extern f32 D_0032FFE0[];
extern f32 D_0035BECC;
extern const f32 D_0035BED0;
extern const f32 D_0035BEE8;
extern const f32 D_0035BEEC;
extern f32 D_0035BEF0;
extern f32 D_0035BEF4;

/* ER-120: by hand from the m2c draft (recipes 2.21: D_0035BED0, D_0035BEE8, D_0035BEEC never stored, extern const; sdata_consts lists them as near an address-taken symbol, so Unconfirmed); names, types and offsets are Unconfirmed readings. */
void func_0014EC58(char *arg0, char *arg1) {
    f32 f;

    f = *(f32 *)(arg0 + 0x94) + D_0035BECC;
    *(f32 *)(arg0 + 0x94) = f;
    if (D_0035BED0 < f) {
        *(f32 *)(arg0 + 0x94) = D_0035BED0;
    }
    if (*(f32 *)(arg0 + 0x94) < -D_0035BED8) {
        *(f32 *)(arg0 + 0x94) = -D_0035BED8;
    }
    *(f32 *)(arg1 + 0x94) += *(f32 *)(arg0 + 0x94);
    func_0012EA28(arg1 + 0x90, *(f32 *)(arg0 + 0x40), D_0035BEE8, D_0035BEEC);
    func_0012EA28(arg1 + 0x94, *(f32 *)(arg0 + 0x44), D_0035BEE8, D_0035BEEC);
    func_0012EA28(arg1 + 0x98, *(f32 *)(arg0 + 0x48), D_0035BEE8, D_0035BEEC);
    *(u8 *)(*(char **)(arg1 + 0xE0) + 0xD08) = *(u8 *)(arg0 + 0xD);
    func_0012B218(arg0, 1);
    func_0012B1F0(arg0, 2, D_0035BEF0);
    *(f32 *)(*(char **)(arg1 + 0xE0) + 0xF8) = *(f32 *)(arg0 + 0x3C);
    if (*(f32 *)(arg0 + 0x3C) != 0.0f) {
        func_0012E820(arg0 + 0x3C, 0.0f, 20.0f);
    }
    if (*(s8 *)(arg0 + 0xD) != 0) {
        func_0012C788(arg1);
        func_0012DF30(arg1, 0x14, 9, *(void **)(arg0 + 0x130), D_0032FFE0, 10.0f);
    }
    func_0014F328(arg1, D_0035BEF4 * 3.1415927f / 180.0f);
    *(s32 *)(arg0 + 0x28) += 1;
    *(s32 *)(arg0 + 0x2C) += 1;
    *(s32 *)(arg0 + 0x24) -= 1;
}

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
extern void func_0012D8B8(char *, double, double, double);
extern void func_001D76D0(float *, float *, float);
extern void func_001C9EE8(float *, float *, char *);
extern void func_001C9F60(void *, void *, void *);
extern void func_001C9FE8(void *, void *, void *);
extern float D_001F5780[];

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

/* ER-120: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
s32 func_0014F240(void *arg0) {
    s32 t = func_00131088(arg0);

    if (t == (s32)func_0014E450 || t == *(s32 *)((char *)arg0 + 0xCC) || t == (s32)func_0014F1D8) {
        return 2;
    }
    return 1;
}

extern s32 func_0012B890(void **, s32, s32, s32, s16, f32, s32);

/* ER-100: by hand from the m2c draft: arg1 is s8 (re-extended on entry), arg2 is narrowed at the call;
   names, types and offsets are Unconfirmed readings. */
s32 func_0014F298(char *arg0, s8 arg1, s32 arg2, s32 arg3, f32 fparg0) {
    if (*(s8 *)(arg0 + 0xE) != arg1) {
        *(s8 *)(arg0 + 0xE) = arg1;
        *(s32 *)(arg0 + 0x28) = 0;
    }
    return func_0012B890((void **)arg0, 0, (u8)arg1, *(s32 *)(arg0 + 0x28), arg2, fparg0, arg3);
}

/* ER-100: by hand from the m2c draft: arg1 is s8 (re-extended on entry), arg2 is narrowed at the call;
   names, types and offsets are Unconfirmed readings. */
s32 func_0014F2E0(char *arg0, s8 arg1, s32 arg2, s32 arg3, f32 fparg0) {
    if (*(s8 *)(arg0 + 0xF) != arg1) {
        *(s8 *)(arg0 + 0xF) = arg1;
        *(s32 *)(arg0 + 0x2C) = 0;
    }
    return func_0012B890((void **)arg0, 1, (u8)arg1, *(s32 *)(arg0 + 0x2C), arg2, fparg0, arg3);
}

extern void func_001CA078(void *, void *, void *);
extern void func_001D7778(float *, float *, float);
extern void func_0012EAF8(void *, f32 (*)[4]);
extern void func_0012EEB0(float *, float *);

void func_0014F328(char *arg0, f32 arg1) {
    f32 m[4][4];
    f32 b[4][4];
    f32 r[4][4];
    f32 v[4];
    f32 out[4];
    s32 i;

    func_0012C788(arg0);
    v[0] = *(f32 *)(arg0 + 0x70);
    v[1] = 0.0f;
    v[2] = *(f32 *)(arg0 + 0x78);
    v[3] = 0.0f;
    func_001CA0C0(m[2], v);
    m[1][0] = 0.0f;
    m[1][1] = 1.0f;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;
    func_001CA078(m, m[1], m[2]);
    func_001D7778(r[0], D_001F5780, arg1);
    func_001C9EE8(m[0], m[0], (char *)r);
    func_0012EAF8(b, (f32 (*)[4])(*(char **)(arg0 + 0xE0) + 0x5A0));
    func_001C9EE8(m[0], b[0], (char *)m);
    func_0012EEB0(out, m[0]);
    for (i = 0; i < 4; i++) {
        func_0012E820(*(char **)(arg0 + 0xE0) + 0x1090 + i * 4, out[i], 1.0f);
        func_0012E820(*(char **)(arg0 + 0xE0) + 0x690 + i * 4, out[i], 1.0f);
    }
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014F480(void *arg0, f32 fparg0, f32 fparg1, f32 fparg2) {
    *(f32 *)((char *)arg0 + 0x40) = fparg0;
    *(f32 *)((char *)arg0 + 0x44) = fparg1;
    *(f32 *)((char *)arg0 + 0x48) = fparg2;
}

extern f32 func_001E3DE0(f32);
extern f32 func_001E3EC8(f32);
extern s32 func_0012FB90(s32, char *, char *);

/* ER-120: by hand from the m2c draft (helper agent); names, types and offsets are Unconfirmed readings. */
void func_0014F490(char *arg0, f32 fparg0, f32 fparg1, f32 fparg2) {
    f32 v[3];
    f32 a;
    f32 *p;
    f32 *q;

    a = 1.5707964f - fparg0;
    p = *(f32 **)(arg0 + 0x130);
    v[0] = p[0] + fparg2 * func_001E3DE0(a);
    q = *(f32 **)(arg0 + 0x130);
    v[1] = q[1] + fparg1;
    v[2] = q[2] + fparg2 * func_001E3EC8(a);
    func_0012FB90((s32)v, arg0, (char *)v);
    func_0014F480(arg0, v[0], v[1], v[2]);
}

extern s32 func_0012F9C0(void **, s32);
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

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014F5F8(s8 *arg0) {
    *(u8 *)((char *)arg0 + 0x5A) |= 0x40;
    func_00131000(arg0, func_0014F1D8);
}

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

extern void func_001501D8(void *);
void func_0014FBA0(void *arg0, f32 fparg0);
extern void func_0012B0D0(void *);
extern void func_0014FA18();
extern void func_0014F728();

/* ER-100: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014F690(s8 *arg0, void *arg1) {
    func_0012B0D0(arg1);
    *(s8 *)(arg0 + 0xE) = 0;
    *(s8 *)(arg0 + 0xF) = 0;
    func_0014FBA0(arg0, 0.0f);
    func_0012B1B8(*(void **)arg0, 4, 8, 2);
    *(void **)(arg0 + 0xC0) = func_0014FA18;
    *(void **)(arg0 + 0xE0) = func_001501D8;
    *(u64 *)(arg0 + 0x70) |= 0x2000000000000ULL;
    *(u8 *)(arg0 + 0x5A) |= 0xA;
    func_00131000(arg0, func_0014F728);
}

extern s32 func_0012B890(void **, s32, s32, s32, s16, f32 fparg0, s32);
extern s32 func_0012FA18(void *, s32, float);
extern void func_0014F7F8();

/* ER-100: by hand from the m2c draft; func_0014FBA0 gets arg0 passed through and 0.0f, func_0012FA18 is
   (void *, s32, float) (its asm reads $a1); names, types and offsets are Unconfirmed readings. */
void func_0014F728(char *arg0, char *arg1) {
    func_0014FBA0(arg0, 0.0f);
    if (*(s16 *)(arg0 + 0x6E) == 0) {
        *(s16 *)(arg0 + 0x74) = func_0012E568(0x96, 0xF0);
        *(s16 *)(arg0 + 0x6E) += 1;
    }
    func_0012B890((void **)arg0, 0, 0, *(s16 *)(arg0 + 0x6C), 0x14, 0.3f, 0);
    if (*(u16 *)(*(char **)(arg1 + 0xE0) + 0x26C) == 0x78) {
        func_0012FA18(arg0, 2, 100000.0f);
    }
    if (func_0012BA30(arg1, 0, 0) != 0) {
        func_00131000(arg0, func_0014F7F8);
    }
}

extern void func_0012DAF8(void *, void *, f32);
extern f32 func_001314C8(s32 arg0, s32 arg1);
extern void func_0012FB48(void *, int);
void func_0014FBD0(char *arg0, char *arg1);
extern f32 D_0035BF04;
extern f32 D_0035BF08;
extern s32 func_0012FAE8(void *, int, int, int, float);
extern const f32 D_0035BF10;

void func_0014F7F8(char *arg0, char *arg1) {
    f32 v[4];
    s32 t;

    t = *(u16 *)(*(char **)(arg1 + 0xE0) + 0x26C);
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        func_0014FBA0(arg0, 0.0f);
        if (func_0012B890((void **)arg0, 0, 2, *(s16 *)(arg0 + 0x6C), 2, 0.5f, 0) != 0) {
            func_0012FA18(arg0, 3, 100000.0f);
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 1:
        if (t >= 0x1F) {
            *(s16 *)(arg0 + 0x6E) += 1;
            *(f32 *)(arg0 + 0x48) = -D_0035BF10;
        }
        break;
    case 2:
        v[0] = *(f32 *)(arg0 + 0x3C);
        v[1] = *(f32 *)(arg0 + 0x40);
        v[2] = *(f32 *)(arg0 + 0x44);
        v[3] = 0.0f;
        func_0012DAF8(arg0, v, 25.0f);
        if (t >= 0x46) {
            *(f32 *)(arg0 + 0x48) = 0.0f;
            if (t >= 0x64) {
                func_0012FAE8(arg0, 4, 0x7F, (int)(*(char **)(arg1 + 0xE0) + 0x530), 10000.0f);
                *(s16 *)(arg0 + 0x6C) = 0;
                *(s16 *)(arg0 + 0x6E) += 1;
            }
            *(f32 *)(arg0 + 0x4C) = func_001314C8(t - 0x46, 0x1E);
        }
        break;
    case 3:
        if ((s32)D_0035BF04 < *(s16 *)(arg0 + 0x6C)) {
            *(s8 *)(arg0 + 0xF) = 1;
            func_0014FBA0(arg0, 0.0f);
        }
        if (*(s16 *)(arg0 + 0x6C) < (s32)D_0035BF08) {
            func_0014FBD0(arg0, arg1);
        } else {
            func_0012EA28(arg0 + 0x4C, 0.0f, 0.2f, 999.0f);
            func_0012FB48(arg0, 1);
            *(s8 *)(arg0 + 0xF) = 0;
        }
        if (func_0012BA30(arg1, 0, 2) != 0) {
            func_00131000(arg0, func_0014F728);
        }
        break;
    }
}

extern void func_0012DF90(void *, s32, s32, void *, f32 *, f32 fparg0, f32 fparg1);
extern void func_0012C7B8(f32 *, f32 *);
extern f32 D_00330000[][4];
extern f32 D_00330050[];
extern f32 D_00330068[];
extern f32 D_0035BF0C;

void func_0014FA18(char *arg0, char *arg1) {
    f32 m[4];
    f32 *tbl;
    s32 i;
    s32 a;
    s32 b;
    char *e;

    if (*(s8 *)(arg0 + 0xE) != 0) {
        a = 4;
        b = 3;
        m[0] = *(f32 *)(arg0 + 0x3C);
        m[1] = *(f32 *)(arg0 + 0x40) + *(f32 *)(arg0 + 0x48);
        m[2] = *(f32 *)(arg0 + 0x44);
        m[3] = 0.0f;
        func_0012C788(arg1);
        if (*(s8 *)(arg0 + 0xF) != 0) {
            tbl = D_00330068;
        } else {
            tbl = D_00330050;
        }
        for (i = 0; i < 4; i++, a++, b++) {
            func_0012DF30(arg1, a, b, m, D_00330000[i], tbl[i]);
            e = *(char **)(arg1 + 0xE0);
            func_0012C7B8((f32 *)(e + a * 0xA0), (f32 *)(e + b * 0xA0));
        }
        func_0012DF90(arg1, a, b, m, D_00330000[i], tbl[i], -D_0035BF0C * *(f32 *)(arg0 + 0x4C));
    } else {
        func_0012B218(arg0, 2);
    }
    func_0012B218(arg0, 1);
}

/* ER-085: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014FBA0(void *arg0, f32 fparg0) {
    void *temp_3 = *(void **)((char *)arg0 + 0x130);
    *(s8 *)((char *)arg0 + 0xE) = 1;
    *(f32 *)((char *)arg0 + 0x3C) = *(f32 *)temp_3;
    *(f32 *)((char *)arg0 + 0x40) = *(f32 *)((char *)temp_3 + 4);
    *(f32 *)((char *)arg0 + 0x44) = *(f32 *)((char *)temp_3 + 8);
    *(f32 *)((char *)arg0 + 0x48) = fparg0;
}

extern f32 D_0035BF14;
extern f32 D_0035BF18;
extern void func_0012EB60(float *, s32, float);
extern void func_0012FD70(void *, s32, s32, s32, float *, float, float);

/* ER-100: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014FBD0(char *arg0, char *arg1) {
    float buf[16]; /* unused: sizes the frame only (ruling 3.13) */
    float m[16];
    float v[4];

    func_0012EB60(m, *(s32 *)(arg1 + 0xE0) + 0x500, ((D_0035BF0C * 3.1415927f) / 180.0f) * *(f32 *)(arg0 + 0x4C));
    func_001CA048(v, &m[8], -1.0f);
    func_0012FD70(arg0, 0xA8, 0, 8, v, D_0035BF14, D_0035BF18);
}

extern float func_001114E8(char *, float *);
extern void func_0012D3B8(char *, f32, f32);
extern s32 func_0012F8C8(s8 *a0, s32 a1, s32 a2, s32 a3, s32 a4, f32 a5);
extern void func_001C9B18(void *, void *);

s32 func_0014FC68(char *arg0, char *arg1, char *arg2) {
    f32 v[4];
    f32 f;
    u16 s;

    *(s16 *)(arg0 + 0x5C) = 0;
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        *(f32 *)(arg0 + 0x30) = *(f32 *)(arg1 + 0x90);
        *(f32 *)(arg0 + 0x34) = *(f32 *)(arg1 + 0x94);
        *(f32 *)(arg0 + 0x38) = *(f32 *)(arg1 + 0x98);
        func_001C9B18(arg0 + 0x90, arg2);
        *(f32 *)(arg0 + 0xB0) = *(f32 *)(arg2 + 0x10);
        *(s16 *)(arg0 + 0x6E) += 1;
    case 1:
        func_0012F8C8((s8 *)arg0, *(u8 *)(arg2 + 0x14), *(s16 *)(arg0 + 0x6C), 0xA, 0, 0.2f);
        f = *(f32 *)(arg0 + 0x94) + *(f32 *)(arg0 + 0xB0);
        *(f32 *)(arg0 + 0x94) = f;
        *(f32 *)(arg1 + 0x94) += f;
        *(f32 *)(arg0 + 0x50) += *(f32 *)(arg0 + 0x94);
        func_0012D3B8(arg1, *(f32 *)(arg0 + 0x90), *(f32 *)(arg0 + 0x98));
        f = *(f32 *)(arg0 + 0x54);
        if (f <= *(f32 *)(arg1 + 0x94)) {
            s = *(u16 *)(arg0 + 0x6E);
            *(f32 *)(arg1 + 0x94) = f;
            *(s16 *)(arg0 + 0x6C) = 0;
            *(s16 *)(arg0 + 0x6E) = s + 1;
        }
        break;
    case 2:
        if (func_0012F8C8((s8 *)arg0, *(u8 *)(arg2 + 0x15), *(s16 *)(arg0 + 0x6C), 0xA, 0, 0.2f) != 0) {
            v[0] = *(f32 *)(arg0 + 0x30);
            v[1] = *(f32 *)(arg0 + 0x34);
            v[2] = *(f32 *)(arg0 + 0x38);
            v[3] = 0.0f;
            *(f32 *)(arg2 + 0x18) = func_001114E8((char *)v, (float *)(arg1 + 0x90));
            return 1;
        }
        break;
    }
    return 0;
}

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

extern s32 func_0014FE30(void **, void *, s32);

/* ER-083: drafted with m2c (tools/m2c_bulk.py, return-wrapper form); names, types and offsets are Unconfirmed readings. */
s32 func_0014FE10(void *arg0, s32 arg1) {
    return func_0014FE30(arg0, arg1, 2);
}

extern s32 func_0012FAA8(void *);
extern s32 func_0012F938(void *, s32);

/* ER-120: by hand from the m2c draft (helper agent); names, types and offsets are Unconfirmed readings. */
s32 func_0014FE30(void **arg0, void *arg1, s32 arg2) {
    s32 i;

    *(f32 *)((char *)arg0 + 0xB4) = 0.9f;
    *(u8 *)((char *)arg0 + 0x5A) &= 0xF7;
    switch (*(s16 *)((char *)arg0 + 0x6E)) {
    case 0:
        func_0012FB48(arg0, 0);
        func_0012FAA8(arg0);
        *(s16 *)((char *)arg0 + 0x6E) += 1;
    case 1:
        for (i = 0; i < 7; i++) {
            func_0012B890(arg0, i, (u8)arg2, *(s16 *)((char *)arg0 + 0x6C), 0xA, 0.3f, 0);
        }
        if (*(s16 *)((char *)arg0 + 0x6C) > 0x14 && func_0012F938(arg1, arg2) != 0) {
            func_001321E8(arg0);
            return 1;
        }
        return 0;
    }
    return 1;
}

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

extern s32 func_001E9C70();

/* ER-100: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
s32 func_001500C8(void **arg0, s32 arg1, s32 arg2) {
    s32 r;
    s32 i;
    s32 n;

    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        s32 s = *(s32 *)((char *)arg0 + 0xBC);
        if (s != *(s32 *)((char *)arg0 + 0xC4) && s != *(s32 *)((char *)arg0 + 0xC8)) {
            n = func_001E9C70();
        } else {
            n = *(s32 *)((char *)arg0 + 0x10) + 1;
        }
        *(s32 *)((char *)arg0 + 0x10) = n % 3;
        *(s16 *)((char *)arg0 + 0x6E) += 1;
        func_0012FB48(arg0, 0);
        return 0;
    }
    r = 1;
    for (i = 0; i < 7; i++) {
        if ((arg2 >> i) & 1) {
            if (func_0012B890(arg0, i, (u8)(*(s32 *)((char *)arg0 + 0x10) + arg1), *(s16 *)((char *)arg0 + 0x6C), 0x1E, 0.1f, 0) == 0) {
                r = 0;
            }
        }
    }
    return r;
}


/* ER-062: by hand from the m2c draft (the pass-through $a0 written out); names and types are Unconfirmed readings. */
void func_001501D8(void *arg0) {
    func_0012FB48(arg0, 0);
}

extern char D_0032FD80[];

/* ER-120: by hand from the m2c draft; names and types are Unconfirmed readings. */
s32 func_001501E0(float *arg0) {
    return func_001114E8(D_0032FD80, arg0) <= 1750.0f;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00150220(void *arg0, void *arg1) {
    *(f32 *)((char *)arg0 + 0x40) = (f32) *(f32 *)arg1;
    *(f32 *)((char *)arg0 + 0x44) = (f32) *(f32 *)((char *)arg1 + 4);
    *(f32 *)((char *)arg0 + 0x48) = (f32) *(f32 *)((char *)arg1 + 8);
}

extern s32 func_00131470(void *, s32);

/* ER-120: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
s8 func_00150240(void *arg0) {
    if (func_00131470(arg0, 0x11) == 0) {
        return 0;
    }
    return *(s8 *)(*(char **)((char *)arg0 + 0xE8) + 0xF);
}

/* ER-120: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
s32 func_00150280(void *arg0) {
    if (func_00131470(arg0, 0x11) == 0) {
        return 0;
    }
    return *(s16 *)(*(char **)((char *)arg0 + 0xE8) + 0x64) < 1;
}

extern void func_00150658(char *, char *);
extern s32 func_001503C0(s8 *arg0);
extern s32 func_001503E0(s8 *arg0);
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

extern f32 D_0035BF24;
void func_00150810(void *a0, s32 a1);
void func_001505F8(void **arg0);
extern void func_0012C3E8(void *, s32, s32, s32, f32, f32);

/* ER-120: by hand from the m2c draft (helper agent); names, types and offsets are Unconfirmed readings. */
void func_00150400(char *arg0, void *arg1) {
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        func_00150810(arg0, 1);
        *(s16 *)(arg0 + 0x6E) += 1;
    case 1:
        if (func_0012B890((void **)arg0, 2, 8, *(s16 *)(arg0 + 0x6C), 0xA, 0.3f, 0) != 0) {
            *(s8 *)(arg0 + 0xF) = 1;
        }
        if (func_0012B890((void **)arg0, 0, 8, *(s16 *)(arg0 + 0x6C), 0x14, 0.3f, 0) != 0 && func_0012BA30(arg1, 0, 8) != 0) {
            func_00150810(arg0, 2);
            *(s16 *)(arg0 + 0x6C) = 0;
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 2:
        *(f32 *)(arg0 + 0x4C) = func_001314C8(*(s16 *)(arg0 + 0x6C), 0x28);
        if (func_0012B890((void **)arg0, 2, 0xA, *(s16 *)(arg0 + 0x6C), 0x28, 0.1f, 0) != 0) {
            func_00150818((int **)arg0, 3, ((s32)D_0035BF24 << 16) | 0x7F);
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 3:
        if (func_00130780(arg0) == 4) {
            func_0012FB48(arg0, 1);
            func_00131000(arg0, func_001505F8);
        } else if (*(s16 *)(arg0 + 0x6C) >= 2) {
            s32 p = *(s32 *)(*(char **)arg0 + 0xE0);
            *(s16 *)(arg0 + 0x6C) = 0;
            func_0012C3E8(arg0, 0xA7, 0, p + 0x5D0, 0.0f, 0.0f);
        }
        break;
    }
}


/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001505F8(void **arg0) {
    *(s8 *)((char *)arg0 + 0xF) = 0;
    if (func_0012B7C0(arg0, 7U, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x78, 1, 0.1f) != 0) {
        func_00131000((s8 *) arg0, func_00150350);
    }
}

extern f32 D_00330190[][4];
extern f32 D_003301F0[];
extern f32 D_0035BF28;

/* ER-120: by hand from the m2c draft (helper agent) (func_0012DF90 read from this call: Unconfirmed); names, types and offsets are Unconfirmed readings. */
void func_00150658(char *arg0, char *arg1) {
    f32 m[4];
    s32 i;
    s32 a;
    s32 b;
    char *e;

    if (*(s8 *)(arg0 + 0xF) != 0) {
        a = 4;
        b = 3;
        func_001C9B18(m, *(void **)(arg0 + 0x130));
        func_0012C788(arg1);
        for (i = 0; i < 5; i++, a++, b++) {
            func_0012DF30(arg1, a, b, m, D_00330190[i], D_003301F0[i]);
            e = *(char **)(arg1 + 0xE0);
            func_0012C7B8((f32 *)(e + a * 0xA0), (f32 *)(e + b * 0xA0));
        }
        func_0012DF90(arg1, a, b, m, D_00330190[i], D_003301F0[i], -D_0035BF28 * *(f32 *)(arg0 + 0x4C));
    } else {
        func_0012B218(arg0, 2);
        func_0012B218(arg0, 4);
    }
    func_0012B218(arg0, 1);
}

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
