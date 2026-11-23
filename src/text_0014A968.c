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

extern void func_0014B0D8(void *arg0, s8 arg1);
extern void func_0012B1B8(void *, s32, s32, s8);
extern void func_00131000(char *, void *);
extern void func_0014AE78();
extern void func_0014AF58();
extern void func_0014AE48();
extern void func_0014AA18();

/* ER-100: by hand from the m2c draft (store order by search); names, types and offsets are Unconfirmed readings. */
void func_0014A968(char *arg0) {
    char *p;

    func_0014B0D8(arg0, 0);
    *(s8 *)(arg0 + 0xF) = 0;
    *(s32 *)(arg0 + 0x28) = 0;
    *(s32 *)(arg0 + 0x2C) = 0;
    *(void **)(arg0 + 0xC4) = func_0014AE78;
    *(void **)(arg0 + 0xC8) = func_0014AE78;
    *(void **)(arg0 + 0xCC) = func_0014AF58;
    *(void **)(arg0 + 0xC0) = func_0014AE48;
    func_0012B1B8(*(void **)arg0, 3, 0xC, 2);
    p = *(char **)arg0;
    if (*(f32 *)(p + 0x12C) == 0.0f) {
        *(f32 *)(p + 0x12C) = 500.0f;
    }
    *(s8 *)(arg0 + 0x156) = 0x50;
    func_00131000(arg0, func_0014AA18);
}

extern s32 func_0012B540(void *, s32, s8, s32);
extern s32 func_00131140(char *, void *, void *, void *, void *, void *);
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

extern s32 func_0012B890(void **, s32, s32, s32, s16, f32, s32);
extern s32 func_0012F9C0(void **, s32);
extern s32 func_0012BA30(void *, s32, s32);
extern void func_0014ABE8(void **arg0, void *arg1);
extern void func_0014AD58(char *arg0, void *arg1);
extern void func_0014AFA8();

/* ER-120: by hand from the m2c draft (func_0012B890's float parameter before the last int, as in text_00145120: recipes 2.21); names, types and offsets are Unconfirmed readings. */
void func_0014AAA8(void **arg0, void *arg1) {
    char *p = (char *)arg0;
    f32 k;

    if (*(s16 *)(p + 0x6E) == 0) {
        func_0014B0D8(arg0, 0);
        *(u64 *)(p + 0x70) &= ~((u64)0x8000 << 35);
        func_0012F9C0(arg0, 5);
        *(s16 *)(p + 0x6E) += 1;
    }
    k = 0.4f;
    func_0012B890(arg0, 0, 8, *(s16 *)(p + 0x6C), 4, k, 0);
    if (func_0012B890(arg0, 1, 8, *(s16 *)(p + 0x6C), 4, k, 0) != 0 && func_0012BA30(arg1, 1, 8) != 0) {
        func_0014B0D8(arg0, 1);
        *(u64 *)(p + 0x70) |= (u64)0x8000 << 35;
        func_00131140(p, func_0014ABE8, func_0014AD58, func_0014AD58, func_0014AD58, func_0014AFA8);
    }
}

extern s32 func_0012B890(void **, s32, s32, s32, s16, f32 fparg0, s32);
extern int func_0012E568(int, int);
extern s32 func_0012FC38(void **, s32, s32, s32);
extern void func_00130CE0(char *, float);
void func_0014B0E8(void *arg0);
extern s32 func_0014B0F8(char *, void *, s32);
extern void func_0014AD58();

/* ER-100: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014ABE8(void **arg0, void *arg1) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        func_0014B0E8(arg0);
        *(s8 *)((char *)arg0 + 8) = 0;
        *(s8 *)((char *)arg0 + 9) = 0;
        *(u16 *)((char *)arg0 + 0x74) = func_0012E568(0x1E, 0x78);
        *(s16 *)((char *)arg0 + 0x6E) += 1;
    }
    if (func_0012B890(arg0, 1, 9, *(s16 *)((char *)arg0 + 0x6C), 0x14, 0.15f, 0) != 0) {
        if (*(u16 *)(*(char **)((char *)arg1 + 0xE0) + 0x26C) == 0x13) {
            func_0012F9C0(arg0, 2);
        }
        if (*(s8 *)((char *)arg0 + 8) == 0 && func_0012FC38(arg0, 0, 0x14, 0x1C) != 0) {
            *(s8 *)((char *)arg0 + 8) = 1;
        }
    } else {
        func_00130CE0((char *)arg0, 20.0f);
    }
    if (*(s8 *)((char *)arg0 + 9) == 0) {
        *(s8 *)((char *)arg0 + 9) = func_0014B0F8(arg0, arg1, 0);
    }
    if (func_0012BA30(arg1, 1, 9) != 0) {
        if (--*(s16 *)((char *)arg0 + 0x74) < 0) {
            if (*(s8 *)((char *)arg0 + 9) != 0) {
                func_00131140((char *)arg0, func_0014ABE8, func_0014AD58, func_0014AD58, func_0014AD58, func_0014AFA8);
            }
        }
    }
}

extern s32 func_0012E568(s32 arg0, s32 arg1);
extern void func_0014B0E8(void *arg0);

/* ER-120: by hand from the m2c draft (helper agent); names, types and offsets are Unconfirmed readings. */
void func_0014AD58(char *arg0, void *arg1) {
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        func_0014B0E8(arg0);
        *(s16 *)(arg0 + 0x6E) += 1;
        break;
    case 1:
        if (func_0014B0F8(arg0, arg1, 1) != 0) {
            *(s16 *)(arg0 + 0x6C) = 0;
            *(s16 *)(arg0 + 0x74) = func_0012E568(2, 0x1E);
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 2:
        if (*(s16 *)(arg0 + 0x6C) >= *(s16 *)(arg0 + 0x74)) {
            if (func_00131140(arg0, func_0014ABE8, 0, 0, 0, func_0014AFA8) == 0) {
                *(s16 *)(arg0 + 0x6E) = 1;
            }
        }
        break;
    }
}

extern void func_0012B218(void *, int);

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014AE48(void *arg0) {
    func_0012B218(arg0, 1);
    func_0012B218(arg0, 2);
}

extern f32 D_0035BDE4;
extern f32 D_0035BDE8;
extern s8 func_0014B0E0(void *arg0);
extern s32 func_00131498(void);
extern void func_00130FE0(void *, void *);
extern s32 func_0014FF98(void *arg0, s32 arg1, s32 arg2);

/* ER-120: by hand from the m2c draft (helper agent); names, types and offsets are Unconfirmed readings. */
void func_0014AE78(char *arg0) {
    s32 n;

    if (*(s16 *)(arg0 + 0x6E) == 0) {
        if (func_0014B0E0(arg0) == 0) {
            func_00130FE0(arg0, func_0014AAA8);
            return;
        }
        n = (*(u8 *)(arg0 + 0x60) != 0) ? D_0035BDE4 : D_0035BDE8;
        if (func_00131498() < n) {
            func_00130FE0(arg0, func_0014AFA8);
            return;
        }
    }
    if (func_0014FF98(arg0, 3, 3) != 0) {
        func_00131140(arg0, func_0014ABE8, func_0014AD58, func_0014AD58, func_0014AD58, func_0014AD58);
    }
}

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

extern s32 func_0012F938(s32, s32);

/* ER-120: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014AFA8(void **arg0, void *arg1) {
    char *p = (char *)arg0;
    f32 k;

    if (*(s16 *)(p + 0x6E) == 0) {
        *(u64 *)(p + 0x70) &= ~((u64)0x8000 << 35);
        *(s32 *)(p + 0x10) = func_0012E568(0x3C, 0xB4);
        *(s16 *)(p + 0x6E) += 1;
    }
    k = 0.1f;
    func_0012B890(arg0, 1, 2, *(s16 *)(p + 0x6C), 0x14, k, 0);
    if (func_0012B890(arg0, 0, 2, *(s16 *)(p + 0x6C), 0x14, k, 0) != 0 && func_0012F938((s32)arg1, 2) != 0) {
        if (--*(s32 *)(p + 0x10) < 0) {
            func_00131140(p, func_0014AAA8, func_0014AAA8, func_0014AAA8, func_0014AAA8, func_0014AA18);
        }
        func_0014B0D8(arg0, 0);
        *(u64 *)(p + 0x70) |= (u64)0x8000 << 35;
    }
}

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

extern s32 func_0012C190(char *);
extern void func_00130AD0(char *, f32, f32);
extern void func_00130848(s8 **, f32);
extern f32 D_0035BDD8;
extern f32 D_0035BDDC;
extern f32 D_0035BDE0;

/* ER-120: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
s32 func_0014B0F8(char *arg0, void *arg1, s32 arg2) {
    s32 ret = 0;
    s32 st = *(s32 *)(arg0 + 0x28);
    s32 n = (*(s8 *)(arg0 + 0xF) != 0) ? 0xA : 1;
    s32 c = *(s32 *)(arg0 + 0x2C) + 1;

    switch (st) {
    case 0:
        if (func_0012C190(arg0) == 0) {
            return 1;
        }
        st = 1;
    case 1:
        func_00130CE0(arg0, 10.0f);
        if (arg2 != 0) {
            func_0012B890((void **)arg0, 1, n & 0xFF, c, 0x14, 0.08f, 0);
        }
        if (func_0012B890((void **)arg0, 0, n & 0xFF, c, 0x14, 0.08f, 0) != 0) {
            func_00130848((s8 **)arg0, D_0035BDD8);
            st += 1;
        } else if (c == 0xA) {
            func_0012F9C0((void **)arg0, 3);
        }
        break;
    case 2:
        if (*(u16 *)(*(char **)((char *)arg1 + 0xE0) + 0x8C) < 0x1F) {
            func_00130CE0(arg0, 50.0f);
            func_00130AD0(arg0, D_0035BDDC, D_0035BDE0);
        } else if (func_0012BA30(arg1, 0, n) != 0) {
            func_0012F9C0((void **)arg0, 4);
            st = 0;
            c = 0;
            ret = 1;
            *(u8 *)(arg0 + 0xF) ^= 1;
        }
        break;
    }
    *(s32 *)(arg0 + 0x28) = st;
    *(s32 *)(arg0 + 0x2C) = c;
    return ret;
}

extern void func_0014BC40(void *);
extern void func_0014BC00(void *arg0);
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

extern void func_0014BA78(char *, void *);
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

extern s32 func_0012C578(char *, s32, s32, s32);

/* ER-120: by hand from the m2c draft (helper agent) (the 0x2C store in each arm); names, types and offsets are Unconfirmed readings. */
void func_0014B688(char *arg0, void *arg1) {
    func_00130CE0(arg0, 20.0f);
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        *(s32 *)(arg0 + 0x30) = 0;
        *(s16 *)(arg0 + 0x6E) += 1;
        break;
    case 1:
        if (func_0012B7C0((void **)arg0, 3, 0xB, *(s16 *)(arg0 + 0x6C), 0x14, 0, 0.2f) != 0) {
            if (func_0012C578(arg0, 0xA2, 3, 0x12) == 0) {
                *(u8 *)(arg0 + 0xF) += 1;
                *(s32 *)(arg0 + 0x2C) = func_0012E568(1, 0xA);
            } else {
                *(u8 *)(arg0 + 0xF) = 0;
                *(s32 *)(arg0 + 0x2C) = func_0012E568(0x96, 0xF0);
            }
            *(s16 *)(arg0 + 0x6C) = 0;
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 2:
        if (func_0012BA30(arg1, 0, 0xB) != 0) {
            func_00131000(arg0, func_0014B338);
            return;
        }
        break;
    }
    return;
}

extern int func_00130780(char *);
extern s32 func_0014BD70(char *, char *, s32);
extern void func_0014B898();
extern void func_0014BD50(void *);
extern void func_0014BD60(void *);

/* ER-120: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014B7C0(s8 *arg0, s32 arg1) {
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        func_0014BD50(arg0);
        func_0014BD60(arg0);
        *(s16 *)(arg0 + 0x6E) += 1;
    case 1:
        if (func_0014BD70((char *)arg0, (char *)arg1, 1) == 0) {
            break;
        }
        switch (func_00130780((char *)arg0)) {
        case 0:
            func_00131000((char *)arg0, func_0014B898);
            break;
        case 1:
            func_00131000((char *)arg0, func_0014B898);
            break;
        }
        break;
    }
}

extern f32 D_0035BDFC;
extern f32 func_00130BF8(char *, f32);

/* ER-120: by hand from the m2c draft (helper agent); names, types and offsets are Unconfirmed readings. */
void func_0014B898(char *arg0, void *arg1) {
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        func_00130BF8(arg0, D_0035BDFC);
        func_00130CE0(arg0, 50.0f);
        if (func_0012B7C0((void **)arg0, 3, 8, *(s16 *)(arg0 + 0x6C), 8, 0, 0.35f) != 0) {
            func_0012F9C0((void **)arg0, 2);
            *(s8 *)(arg0 + 8) = 0;
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 1:
        func_00130BF8(arg0, D_0035BDFC);
        if (*(s8 *)(arg0 + 8) == 0) {
            if (func_0012FC38((void **)arg0, 0, 2, 0x10) != 0) {
                *(s8 *)(arg0 + 8) = 1;
            }
        }
        if (func_0012BA30(arg1, 0, 8) != 0) {
            *(s16 *)(arg0 + 0x6C) = 0;
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 2:
        func_00130CE0(arg0, 20.0f);
        if (func_0012B7C0((void **)arg0, 3, 9, *(s16 *)(arg0 + 0x6C), 0xA, 0, 0.3f) != 0) {
            func_0012F9C0((void **)arg0, 3);
            *(s8 *)(arg0 + 9) = 0;
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 3:
        if (*(s8 *)(arg0 + 9) == 0) {
            if (func_0012FC38((void **)arg0, 1, 1, 0xD) != 0) {
                *(s8 *)(arg0 + 9) = 1;
            }
        }
        if (func_0012BA30(arg1, 0, 9) != 0) {
            func_00131000(arg0, func_0014BA78);
            return;
        }
        break;
    }
    return;
}

extern f32 D_0035BE00;
extern f32 D_0035BE04;
extern f32 D_0035BE08;
extern f32 D_0035BE0C;
extern f32 D_0035BE10;

/* ER-120: by hand from the m2c draft (func_00130BF8 returns a float here: its result is compared); names, types and offsets are Unconfirmed readings. */
void func_0014BA78(char *arg0, void *arg1) {
    s16 s = *(s16 *)(arg0 + 0x6E);

    switch (s) {
    case 0:
        func_00130CE0(arg0, 10.0f);
        if (func_0012B7C0((void **)arg0, 3, 0xA, *(s16 *)(arg0 + 0x6C), 0x1E, 0, 0.2f) != 0) {
            *(s8 *)(arg0 + 8) = 0;
            func_0012F9C0((void **)arg0, 4);
            func_00130AD0(arg0, D_0035BE00, D_0035BE08);
            *(s16 *)(arg0 + 0x6E) += 1;
        } else {
            func_00130BF8(arg0, 0.95f);
        }
        break;
    case 1:
        func_00130AD0(arg0, D_0035BE04, D_0035BE08);
        if (*(s8 *)(arg0 + 8) == 0 && func_0012FC38((void **)arg0, 2, 8, 0x3E7) != 0) {
            *(s8 *)(arg0 + 8) = 1;
        }
        if (func_0012BA30(arg1, 1, 0xA) != 0) {
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 2:
        if (func_00130BF8(arg0, D_0035BE0C) < D_0035BE10) {
            func_00131000(arg0, func_0014B338);
        }
        break;
    }
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0014BC00(void *arg0) {
    func_0012B218(arg0, 2);
    func_0012B218(arg0, 1);
    *(s32 *)((char *)arg0 + 0x2C) = (s32) (*(s32 *)((char *)arg0 + 0x2C) - 1);
}


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

extern void func_0014B688(char *, void *);

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

extern void func_001CA018(float *, char *, char *);
extern void func_001CA0C0(float *, float *);
extern void func_0012D568(char *, f32);
extern f32 D_0035BDF0;
extern const f32 D_0035BDF4;
extern f32 D_0035BDF8;

s32 func_0014BD70(char *arg0, char *arg1, s32 arg2) {
    f32 v[4];
    s32 ret = 0;
    u16 k;
    s32 n;

    func_00130378(arg1 + 0x90, *(s32 *)(arg0 + 0x130));
    func_001CA018(v, *(char **)(arg0 + 0x130), arg1 + 0x90);
    v[1] = 0.0f;
    func_001CA0C0(v, v);
    n = *(s32 *)(arg0 + 0x28) + 1;
    k = *(u16 *)(*(char **)(arg1 + 0xE0) + 0x8C);
    *(s32 *)(arg0 + 0x28) = n;
    func_0012B7C0((void **)arg0, 3, 1, n, 8, 1, 0.3f);
    switch (*(s32 *)(arg0 + 0x24)) {
    case 0:
        *(s32 *)(arg0 + 0x24) = 1;
    case 1:
        func_00130CE0(arg0, 5.0f);
        {
            f32 m = D_0035BDF0;
            f32 a = v[0] * m;
            f32 b = v[2] * m;

            *(f32 *)(arg0 + 0x90) += a;
            *(f32 *)(arg0 + 0x98) += b;
        }
        func_0012D568(arg0 + 0x90, D_0035BDF4);
        *(s32 *)(arg0 + 0x24) = 2;
        break;
    case 2:
        if (k == 1) {
            ret = 1;
        }
        if (k == 0x10) {
            ret = -1;
        }
        if (ret != 0) {
            *(s32 *)(arg0 + 0x24) = 3;
        }
        break;
    case 3:
        func_00130CE0(arg0, 10.0f);
        {
            f32 m = D_0035BDF8;
            f32 x = *(f32 *)(arg0 + 0x90);
            f32 y = *(f32 *)(arg0 + 0x98);

            *(f32 *)(arg0 + 0x90) = x * m;
            *(f32 *)(arg0 + 0x98) = y * m;
        }
        if (k == 7 || k == 0x15) {
            *(s32 *)(arg0 + 0x24) = 1;
        }
        break;
    }
    *(f32 *)(arg1 + 0x90) += *(f32 *)(arg0 + 0x90);
    *(f32 *)(arg1 + 0x98) += *(f32 *)(arg0 + 0x98);
    return ret;
}

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

extern void func_0012CBF8(void *, void *, s32, void *);
void func_0014C620(void *arg0);

/* ER-100: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014BFE0(void **arg0, s32 arg1) {
    f32 v[4];
    s32 n;

    switch (*(s16 *)((char *)arg0 + 0x6E)) {
    case 0:
        *(s32 *)((char *)arg0 + 0x10) = 0;
        *(s32 *)((char *)arg0 + 0x30) = 0;
        *(s16 *)((char *)arg0 + 0x6E) += 1;
        func_0012F9C0(arg0, 4);
    case 1:
        func_0012CBF8(v, (void *)arg1, 5, *(void **)((char *)arg0 + 0x130));
        if (func_0012B890(arg0, 0, 0xB, *(s16 *)((char *)arg0 + 0x6C), 0x14, 0.2f, 0) != 0 && func_0012BA30((void *)arg1, 0, 0xB) != 0) {
            func_0012B540((void *)arg1, 0, 0x11, 1);
            *(u16 *)((char *)arg0 + 0x6C) = 0;
            *(s16 *)((char *)arg0 + 0x6E) += 1;
        }
        break;
    case 2:
        if ((*(u16 *)((char *)arg0 + 0x6C) & 0xF) == 0) {
            func_0014C620(arg0);
            n = *(s32 *)((char *)arg0 + 0x10) + 1;
            *(s32 *)((char *)arg0 + 0x10) = n;
            if (n >= 5) {
                func_0014C5E8(arg0);
            }
        }
        break;
    }
}

extern void func_0014C630(void **);
s32 func_0014C760(void **arg0, s32 arg1);
extern void func_0014C110(void **, void *);

/* ER-100: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. The address-taken
   parameter only stops the sibling call (ruling 3.14: the game has jal where plain C gives a tail j). */
void func_0014C110(void **arg0, void *arg1) {
    (void)&arg0; /* ruling 3.14 */

    switch (*(s16 *)((char *)arg0 + 0x6E)) {
    case 0:
        if (func_0012B890(arg0, 0, 0xA, *(s16 *)((char *)arg0 + 0x6C), 0x28, 0.2f, 0) != 0) {
            if (*(u16 *)(*(char **)((char *)arg1 + 0xE0) + 0xB2C) == 1) {
                func_0012F9C0(arg0, 3);
                func_0014C630(arg0);
            } else if (func_0012BA30(arg1, 0, 0xA) != 0) {
                *(s16 *)((char *)arg0 + 0x6C) = 0;
                *(s16 *)((char *)arg0 + 0x74) = func_0012E568(0x3C, 0xB4);
                *(s16 *)((char *)arg0 + 0x6E) += 1;
            }
        }
        break;
    case 1:
        func_0014C760(arg0, *(s16 *)((char *)arg0 + 0x6C));
        if (func_0014C5E8(arg0) == 0 && *(s16 *)((char *)arg0 + 0x74) < *(s16 *)((char *)arg0 + 0x6C)) {
            func_00131000((char *)arg0, func_0014C110);
        }
        break;
    }
}

extern s32 func_0014C658(void **, s32, s32);
extern s32 func_0014C748(s32);

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

extern s32 func_0012FAA8(void *);

/* ER-120: by hand from the m2c draft (func_0012FAA8 returns s32, as its definition in text_001000C0); names, types and offsets are Unconfirmed readings. */
void func_0014C3F8(s8 *arg0) {
    switch (*(s16 *)(arg0 + 0x6E)) {
    case 0:
        *(s8 *)(arg0 + 0xE) = 0;
        func_0014C810((char *)arg0, (s32)func_0014C4F8);
        func_0012FAA8(arg0);
        *(s16 *)(arg0 + 0x6E) += 1;
    case 1:
        if (func_0012B7C0((void **)arg0, 3, 2, *(s16 *)(arg0 + 0x6C), 0xA, 0, 0.3f) != 0) {
            *(s16 *)(arg0 + 0x6C) = 0;
            *(s16 *)(arg0 + 0x6E) += 1;
        }
        break;
    case 2:
        if (*(s16 *)(arg0 + 0x6C) >= 0xB5) {
            func_001321E8((char *)arg0);
        }
        break;
    }
}


/* ER-085: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014C4D8(void **arg0) {
    func_0012B890(arg0, 1, 0, *(s32 *)((char *)arg0 + 0x24), 0x14, 0.2f, 1);
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
    func_0012B890((void **) arg0, 1, 1, *(s32 *)((char *)arg0 + 0x24), 0x14, 0.2f, 1);
}

extern void func_0014C790(void *, void *);

/* ER-100: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0014C560(void *arg0, void *arg1) {
    char *p;
    char *q;

    *(s32 *)((char *)arg0 + 0x24) += 1;
    (*(void (**)(void))((char *)arg0 + 0x2C))();
    func_0012B218(arg0, 2);
    if (*(s8 *)((char *)arg0 + 0xE) != 0) {
        func_0014C790(arg0, arg1);
        p = *(char **)((char *)arg1 + 0xE0);
        *(s8 *)(p + 0x3A8) = 1;
        *(s8 *)(p + 0x268) = 1;
    } else {
        q = *(char **)((char *)arg1 + 0xE0);
        *(s8 *)(q + 0x3A8) = 0;
        *(s8 *)(q + 0x268) = 0;
    }
    func_0012B218(arg0, 1);
}

extern void func_0014BFE0(void **, s32);
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

extern void func_0012C3E8(void *, s32, s32, s32, f32, f32);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain; the pass-through $a0 written out); names, types and offsets are Unconfirmed readings. */
void func_0014C630(void **arg0) {
    func_0012C3E8(arg0, 0xA5, 1, *(s32 *)((char *)*arg0 + 0xE0) + 0xAD0, 0.0f, 10.0f);
}

extern f32 D_0035BE38;
extern f32 func_0012E6B8(f32, f32);

/* ER-120: by hand from the m2c draft (helper agent) (ruling 3.13); names, types and offsets are Unconfirmed readings. */
s32 func_0014C658(void **arg0, s32 arg1, s32 arg2) {
    s32 buf[4]; /* unused: sizes the frame only (ruling 3.13) */
    s32 n;
    f32 f;

    if (func_0012B890(arg0, 0, arg1 & 0xFF, arg2, 8, 0.4f, 0) != 0) {
        if (*(u16 *)((char *)*(void **)((char *)*arg0 + 0xE0) + 0xB2C) == 0xC) {
            func_0012F9C0(arg0, 2);
            n = (arg1 == 8) ? 0x11 : 0x17;
            f = func_0012E6B8(10.0f, D_0035BE38);
            func_0012C3E8(arg0, 0xA4, 0, *(s32 *)((char *)*arg0 + 0xE0) + n * 0xA0 + 0x30, 0.0f, f);
        }
        if (func_0012BA30(*arg0, 0, arg1) != 0) {
            return 1;
        }
    }
    return 0;
}

/* ER-085: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
s32 func_0014C748(s32 arg0) {
    return (arg0 != 9) ? 9 : 8;
}

/* ER-054: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0014C760(void **arg0, s32 arg1) {
    return func_0012B890(arg0, 0, 0, arg1, 0x28, 0.08f, 1);
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
