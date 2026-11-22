#include "common.h"

/* ER-024: this file's .sdata (docs/build.md 3.2) */
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BDD8);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BDDC);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BDE0);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BDE4);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BDE8);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BDF0);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BDF4);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BDF8);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BDFC);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BE00);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BE04);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BE08);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BE0C);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BE10);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BE1C);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BE20);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BE24);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BE28);
INCLUDE_SDATA("asm/nonmatchings/text_0014A968", D_0035BE38);

/* ER-024: this file's .data (docs/build.md 3.2) */
INCLUDE_DATA("asm/nonmatchings/text_0014A968", D_0032FF90);
INCLUDE_DATA("asm/nonmatchings/text_0014A968", D_0032FFA0);

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014A968);

extern void func_0014B0D8(void *arg0, s8 arg1);
extern s32 func_0012B540(void *, s32, s8, s32);
extern void func_00131140(char *, void *, void *, void *, void *, void *);
extern void func_0014AAA8(void **, void *);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014AA18(s8 *arg0, void *arg1) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        func_0014B0D8(arg0, 0);
        func_0012B540(arg1, 0, 0, 0);
        func_0012B540(arg1, 1, 0, 0);
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    func_00131140(arg0, func_0014AAA8, func_0014AAA8, func_0014AAA8, func_0014AAA8, NULL);
}

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014AAA8);

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014ABE8);

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014AD58);

extern void func_0012B218(void *, int);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014AE48(void *arg0) {
    func_0012B218(arg0, 1);
    func_0012B218(arg0, 2);
}

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014AE78);

s8 func_0014B0E0(void *arg0);
extern s32 func_0014FE10(void *, s32);
extern void func_001321E8(char *);

/* ER-064: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014AF58(s8 *arg0, s32 arg1) {
    if (func_0014B0E0(arg0) != 0) {
        func_0014FE10(arg0, arg1);
        return;
    }
    func_001321E8(arg0);
}

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014AFA8);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014B0D8(void *arg0, s8 arg1) {
    *(s8 *)((char *)arg0 + 0xE) = arg1;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s8 func_0014B0E0(void *arg0) {
    return *(s8 *)((char *)arg0 + 0xE);
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014B0E8(void *arg0) {
    *(s32 *)((char *)arg0 + 0x28) = 0;
    *(s32 *)((char *)arg0 + 0x2C) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014B0F8);

extern void func_0012B1B8(void *, s32, s32, s8);
extern void func_0014BC40(void *);
extern void func_0014BC00(void *arg0);
extern void func_00131000(char *, void *);
extern void func_0014B338(void **arg0);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014B2C0(s8 *arg0) {
    *(s8 *)((char *)arg0 + 0xF) = 0;
    *(s32 *)((char *)arg0 + 0x2C) = 0;
    func_0012B1B8(*(void **)arg0, 0xA, 0xE, 4);
    func_0012B1B8(*(void **)arg0, 7, 0x19, 2);
    *(void (**)(void *))((char *)arg0 + 0xC4) = func_0014BC40;
    *(void (**)(void *))((char *)arg0 + 0xC8) = func_0014BC40;
    *(void (**)(void *))((char *)arg0 + 0xC0) = func_0014BC00;
    func_00131000(arg0, func_0014B338);
}

extern s32 func_0012B7C0(void **, u32, s32, s32, s16, s32, f32 fparg0);
extern void func_0014B3A0(s8 *arg0);
extern void func_0014B3E0(s8 *arg0);
extern void func_0014B420(s8 *);
extern void func_0014B4C8(s8 *arg0);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014B338(void **arg0) {
    func_0012B7C0(arg0, 7U, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x14, 1, 0.2f);
    func_00131140((s8 *) arg0, func_0014B3A0, func_0014B3E0, func_0014B420, func_0014B4C8, NULL);
}

extern void func_0014BA78(s8 *, void *);
extern s32 func_0014BC88(void *arg0);
extern void func_0014B7C0();

/* ER-064: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014B3A0(s8 *arg0) {
    if (func_0014BC88(arg0) == 0) {
        func_00131000(arg0, func_0014BA78);
    }
}

/* ER-064: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014B3E0(s8 *arg0) {
    if (func_0014BC88(arg0) == 0) {
        func_00131000(arg0, func_0014B7C0);
    }
}

extern s32 func_0014BD10(s8 *arg0);
extern void func_0014BCC8(s8 *arg0);
extern int func_0012E568(int, int);
extern s16 D_001FFB06[];

/* ER-062: by hand from the m2c draft (the 0x6C / 0x74 compare written with 0x6C first, recipe 2.7); names, types and offsets are Unconfirmed readings. */
void func_0014B420(s8 *arg0) {
    if ((func_0014BC88(arg0) == 0) && (func_0014BD10(arg0) == 0)) {
        if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
            *(s16 *)((char *)arg0 + 0x74) = func_0012E568(0xB4, 0xF0);
            *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
        }
        if ((D_001FFB06[0] != 0) || (*(s16 *)((char *)arg0 + 0x6C) > *(s16 *)((char *)arg0 + 0x74))) {
            func_00131000(arg0, func_0014B7C0);
        }
        func_0014BCC8(arg0);
    }
}

/* ER-064: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014B4C8(s8 *arg0) {
    if ((func_0014BC88(arg0) == 0) && (func_0014BD10(arg0) == 0)) {
        func_0012B7C0((void **) arg0, 3U, 0xE, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x14, 1, 0.2f);
    }
}

extern void func_00130CE0(char *, float);
extern f32 func_001302C8(void *);
extern float func_00130378(char *, s32);
extern f32 func_0012E7B0(f32, f32);
extern s32 func_0012DA68(void *, f32, s32, s32, f32);

/* ER-100: by hand; names, types and offsets are Unconfirmed readings. func_0012DA68's float parameter first
   (as in recipes 2.21: same registers, other scheduling); `case 2:` beside the default (ruling 3.9); the
   address-taken local only stops the sibling call (ruling 3.14: the game has jal where plain C gives a tail j). */
void func_0014B528(void **arg0, char *arg1) {
    f32 t;
    f32 g;
    (void)&g; /* ruling 3.14 */

    switch (*(s16 *)((char *)arg0 + 0x6E)) {
    case 0:
        if (func_001302C8(arg0) <= 0.17453294f) {
            func_00130CE0((char *)arg0, 15.0f);
            *(s16 *)((char *)arg0 + 0x6E) = 2;
        } else {
            *(s32 *)((char *)arg0 + 0x34) = 0;
            if (func_0012B7C0(arg0, 3, 0xF, *(s16 *)((char *)arg0 + 0x6C), 0x14, 0, 0.2f) != 0) {
                *(f32 *)((char *)arg0 + 0x30) = *(f32 *)(arg1 + 0xA4);
                *(s16 *)((char *)arg0 + 0x6E) = 1;
                *(s16 *)((char *)arg0 + 0x6C) = 0;
            }
        }
        break;
    case 1:
        t = func_00130378(arg1 + 0x90, *(s32 *)((char *)arg0 + 0x130)) + 6.2831855f;
        g = func_0012E7B0((t - ((f32)(s32)(t / 6.2831855f) * 6.2831855f)) - 3.1415927f, *(f32 *)((char *)arg0 + 0x30));
        if (func_0012DA68(arg0, *(f32 *)((char *)arg0 + 0x30), *(s16 *)((char *)arg0 + 0x6C), 0x1E, g) != 0) {
            *(s16 *)((char *)arg0 + 0x6E) = 2;
        }
        break;
    case 2:
    default:
        break;
    }
    if (*(s16 *)((char *)arg0 + 0x6E) == 2 && func_0014BC88(arg0) == 0) {
        func_00131000((char *)arg0, func_0014B338);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014B688);

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014B7C0);

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014B898);

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014BA78);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014BC00(void *arg0) {
    func_0012B218(arg0, 2);
    func_0012B218(arg0, 1);
    *(s32 *)((char *)arg0 + 0x2C) = (s32) (*(s32 *)((char *)arg0 + 0x2C) - 1);
}

extern s32 func_0014FF98(void *arg0, s32 arg1, s32 arg2);
extern void func_00130FE0(void *, void *);

/* ER-064: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014BC40(void *arg0) {
    if (func_0014FF98(arg0, 3, 7) != 0) {
        func_00130FE0(arg0, func_0014B338);
    }
}

extern s32 func_001312D0(void *, s32, s32, s32, s32, s32);
extern void func_0014B528(void **, char *);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014BC88(void *arg0) {
    return func_001312D0(arg0, (s32) func_0014B3A0, (s32) func_0014B3E0, (s32) func_0014B420, (s32) func_0014B4C8, (s32) func_0014B528);
}


/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014BCC8(s8 *arg0) {
    func_00130CE0(arg0, 30.0f);
    func_0012B7C0((void **) arg0, 3U, 0xE, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x14, 1, 0.2f);
}

extern void func_0014B688(s8 *, void *);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014BD10(s8 *arg0) {
    if ((D_001FFB06[0] != 0) || (*(s32 *)((char *)arg0 + 0x2C) > 0)) {
        return 0;
    }
    func_00131000(arg0, func_0014B688);
    return 1;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014BD50(void *arg0) {
    *(s8 *)((char *)arg0 + 0xF) = 0;
    *(s32 *)((char *)arg0 + 0x2C) = 0;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014BD60(void *arg0) {
    *(s32 *)((char *)arg0 + 0x24) = 0;
    *(s32 *)((char *)arg0 + 0x28) = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014BD70);

void func_0014BF40(void) {
}

extern void func_0012F978(void *, s32, s8);
extern void func_0014C560(void *, void *);
extern void func_0014C380(s8 *arg0);
extern void func_0014C3F8(s8 *);
extern s32 func_0014C5E8(void *arg0);
void func_0014C810(char *a0, int a1);
extern void func_0014C500(s8 *arg0, s32 arg1);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014BF48(s8 *arg0) {
    *(s8 *)((char *)arg0 + 0xF) = 0;
    *(s32 *)((char *)arg0 + 0x24) = 0;
    *(s32 *)((char *)arg0 + 0x2C) = 0;
    *(s32 *)((char *)arg0 + 0x48) = 0;
    *(s8 *)((char *)arg0 + 0xE) = 1;
    func_0012F978(*(void **)arg0, 2, 2);
    func_0012B1B8(*(void **)arg0, 0x1B, 0x24, 2);
    *(void (**)(void *, void *))((char *)arg0 + 0xC0) = func_0014C560;
    *(void (**)(s8 *))((char *)arg0 + 0xC4) = func_0014C380;
    *(void (**)(s8 *))((char *)arg0 + 0xC8) = func_0014C380;
    *(void (**)(s8 *))((char *)arg0 + 0xCC) = func_0014C3F8;
    func_0014C5E8(arg0);
    func_0014C810(arg0, (s32) func_0014C500);
}

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014BFE0);

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014C110);

extern s32 func_00131498(void);
extern s32 func_0014C658(void **, s32, s32);
extern s32 func_0014C748(s32);
s32 func_0014C760(void **arg0, s32 arg1);

/* ER-064: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014C220(void **arg0) {
    s16 temp_3;
    s16 temp_6;
    s32 temp_2;

    temp_3 = *(s16 *)((char *)arg0 + 0x6E);
    switch (temp_3) {                               /* irregular */
    case 0:
        *(s32 *)((char *)arg0 + 0x10) = (s32) ((func_00131498() >= 0x33) ? 9 : 8);
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
        /* fallthrough */
    case 1:
        if ((func_0014C658(arg0, *(s32 *)((char *)arg0 + 0x10), (s32) *(s16 *)((char *)arg0 + 0x6C)) != 0) && (func_0014C5E8(arg0) == 0)) {
            temp_2 = func_0014C748(*(s32 *)((char *)arg0 + 0x10));
            *(s16 *)((char *)arg0 + 0x6C) = 0;
            *(s32 *)((char *)arg0 + 0x10) = temp_2;
            *(s16 *)((char *)arg0 + 0x74) = func_0012E568(1, 0x1E);
            *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
        }
        return;
    case 2:
        temp_6 = *(s16 *)((char *)arg0 + 0x6C);
        if (*(s16 *)((char *)arg0 + 0x74) < temp_6) {
            if (func_0014C658(arg0, *(s32 *)((char *)arg0 + 0x10), (s32) temp_6) != 0) {
                *(s16 *)((char *)arg0 + 0x6C) = 0;
                *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
                return;
            }
        } else {
            func_0014C760(arg0, (s32) temp_6);
        }
        break;
    case 3:
        if ((func_0014C760(arg0, (s32) *(s16 *)((char *)arg0 + 0x6C)) != 0) && (func_0014C5E8(arg0) == 0)) {
            func_00131000((s8 *) arg0, func_0014C220);
        }
        break;
    }
}

void func_0014C4F8(void);
extern s32 func_00150070(void **arg0, s32 arg1, s32 arg2);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014C380(s8 *arg0) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 0xE) = 0;
        func_0014C810(arg0, (s32) func_0014C4F8);
    }
    if (func_00150070(arg0, 3, 3) != 0) {
        func_0014C5E8(arg0);
        func_0014C810(arg0, (s32) &func_0014C500);
        *(s8 *)((char *)arg0 + 0xE) = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014C3F8);

extern s32 func_0012B890(void **, s32, s32, s32, s16, s32, f32 fparg0);

/* ER-085: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014C4D8(void **arg0) {
    func_0012B890(arg0, 1, 0, *(s32 *)((char *)arg0 + 0x24), 0x14, 1, 0.2f);
}

void func_0014C4F8(void) {
}

extern f32 D_0035BE20;
extern void func_0012D388(s32, f32 fparg0);
extern f32 D_0035BE1C;

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014C500(s8 *arg0, s32 arg1) {
    func_00130CE0(arg0, D_0035BE20);
    func_0012D388(arg1, D_0035BE1C);
    func_0012B890((void **) arg0, 1, 1, *(s32 *)((char *)arg0 + 0x24), 0x14, 1, 0.2f);
}

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014C560);

extern void func_0014BFE0(void **, s32);
extern void func_0014C110(void **, void *);
extern void func_0014C220(void **);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014C5E8(void *arg0) {
    return func_001312D0(arg0, (s32) func_0014BFE0, (s32) func_0014BFE0, (s32) func_0014C110, (s32) func_0014C220, (s32) func_0014C220);
}

extern void func_0012FD10(void *, s32, s32, s32);

/* ER-062: by hand from the m2c draft (the pass-through $a0 written out); names and types are Unconfirmed readings. */
void func_0014C620(void *arg0) {
    func_0012FD10(arg0, 0xA6, 2, 5);
}

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014C630);

INCLUDE_ASM("asm/nonmatchings/text_0014A968", func_0014C658);

/* ER-085: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
s32 func_0014C748(s32 arg0) {
    return (arg0 != 9) ? 9 : 8;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014C760(void **arg0, s32 arg1) {
    return func_0012B890(arg0, 0, 0, arg1, 0x28, 1, 0.08f);
}

extern void func_0012C788(void *);
extern void func_0012DDA0(void *, s32, s32, void *, void *, f32 fparg0);
extern s32 D_0032FF90[];
extern f32 D_0035BE24;
extern s32 D_0032FFA0[];
extern f32 D_0035BE28;

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014C790(void *arg0, void *arg1) {
    func_0012C788(arg1);
    func_0012DDA0(arg1, 3, 2, *(void **)((char *)arg0 + 0x130), D_0032FF90, D_0035BE24);
    func_0012C788(arg1);
    func_0012DDA0(arg1, 5, 4, *(void **)((char *)arg0 + 0x130), D_0032FFA0, D_0035BE28);
    func_0012C788(arg1);
}

/* ER-014. Types and offsets are Unconfirmed readings. */
void func_0014C810(char *a0, int a1) {
    *(int *)(a0 + 0x2C) = a1;
    *(int *)(a0 + 0x24) = 0;
    a0[0xF] = 0;
}

/* ER-014. Types and offsets are Unconfirmed readings. */
void func_0014C820(char *a0, int a1) {
    if (*(int *)(a0 + 0x2C) != a1) {
        func_0014C810(a0, a1);
    }
}
