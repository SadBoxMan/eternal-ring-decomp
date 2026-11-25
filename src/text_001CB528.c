#include "common.h"

/* ER-237: declarations used from the top of the file (types read from the callees' use, Unconfirmed). */
extern void *D_0035C554;
extern s32 func_001CB808();
extern void func_001CBB88(void);
extern s32 func_001CB918(s32, s32, u32);
extern s32 func_001CBA08(void);
extern s32 func_001CC9B0(u32);
extern s32 func_001CD468(s32);
extern void func_001D03E0(s32);
extern s32 func_001D3308(s32);
extern void func_001D3378(s32);
extern s32 func_001D0520(s32 *, s32);
extern s32 func_001D0500(s32);
extern s32 D_01B73E20[];
extern s32 D_01B73E78[];

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CB528);

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. */
void func_001CB750(void) {
    s32 *e;
    s32 i;

    func_001CBB88();
    func_001CB808(1, 0x3030, 0);
    func_001CB808(1, 0x3030, 1);
    e = (s32 *)((char *)D_0035C554 + 0x200);
    for (i = 0; i < 4; i++, e += 4) {
        if (*e != 0) {
            func_001CD468(i);
        }
    }
    e = (s32 *)D_0035C554;
    for (i = 0; i < 0x10; i++, e += 8) {
        if (*e != 0) {
            func_001CC9B0(i);
        }
    }
    func_001CB808(1, 0x20);
    func_001D03E0(0);
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CB808);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CB918);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CBA08);


/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001CBB48(void) {
    if (*(u16 *)((char *)D_0035C554 + 0x4D6) != 1) {
        func_001CB808(1, 0x40);
        *(u16 *)((char *)D_0035C554 + 0x4D6) = 1U;
    }
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001CBB88(void) {
    if (*(u16 *)((char *)D_0035C554 + 0x4D6) != 0) {
        func_001CB808(1, 0x50);
        *(u16 *)((char *)D_0035C554 + 0x4D6) = 0U;
    }
}

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. The countdown loops are gcc reversing an up-counting `for` whose index is unused; the record pointer steps only after a call, as in the game */
void func_001CBBC0(void) {
    char *p;
    s32 *e;
    s32 i;
    s32 n;

    if (*(u16 *)((char *)D_0035C554 + 0x4D6) != 0) {
        func_001CB808(1, 0x50);
    }
    *(s16 *)((char *)D_0035C554 + 0x4D4) = 0;
    func_001CB808(1, 0xC0, 0);
    p = D_0035C554;
    for (i = 0; i < 0x10; i++) {
        *(s32 *)(p + 0x1C) = 0;
        if (*(s32 *)p != 0 && *(s32 *)(p + 0xC) != 0 && *(s32 *)(p + 4) != 0) {
            func_001CB808(1, 0x1010, *(s16 *)(p + 0x18), *(s32 *)(p + 0xC), *(s32 *)(p + 4), *(s8 *)(p + 0x1B));
            p += 0x20;
        }
    }
    e = (s32 *)((char *)D_0035C554 + 0x200);
    for (i = 0; i < 4; i++) {
        if (e[0] != 0 && (n = e[2]) != -1) {
            p = (char *)D_0035C554 + (n << 5);
            func_001CB808(1, 0x2070, e[3], *(s32 *)(p + 0xC), *(s32 *)(p + 4), *(s8 *)(p + 0x1B), 0);
            e += 4;
        }
    }
    if (*(u16 *)((char *)D_0035C554 + 0x4D6) != 0) {
        func_001CB808(1, 0x40);
    }
}

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. Twin of func_001CBBC0 (flag 1) */
void func_001CBD00(void) {
    char *p;
    s32 *e;
    s32 i;
    s32 n;

    if (*(u16 *)((char *)D_0035C554 + 0x4D6) != 0) {
        func_001CB808(1, 0x50);
    }
    *(s16 *)((char *)D_0035C554 + 0x4D4) = 1;
    func_001CB808(1, 0xC0, 1);
    p = D_0035C554;
    for (i = 0; i < 0x10; i++) {
        *(s32 *)(p + 0x1C) = 1;
        if (*(s32 *)p != 0 && *(s32 *)(p + 0xC) != 0 && *(s32 *)(p + 4) != 0) {
            func_001CB808(1, 0x1010, *(s16 *)(p + 0x18), *(s32 *)(p + 0xC), *(s32 *)p, *(s8 *)(p + 0x1B));
            p += 0x20;
        }
    }
    e = (s32 *)((char *)D_0035C554 + 0x200);
    for (i = 0; i < 4; i++) {
        if (e[0] != 0 && (n = e[2]) != -1) {
            p = (char *)D_0035C554 + (n << 5);
            func_001CB808(1, 0x2070, e[3], *(s32 *)(p + 0xC), *(s32 *)p, *(s8 *)(p + 0x1B), 1);
            e += 4;
        }
    }
    if (*(u16 *)((char *)D_0035C554 + 0x4D6) != 0) {
        func_001CB808(1, 0x40);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CBE48);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CBE90);

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. 16-byte rounding loops; the second allocation in its own local */
s32 func_001CC0C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    char *p = D_0035C554;
    char *b;
    s32 i;
    s32 slot;
    s32 m;
    s32 m2;
    u32 v;
    s32 w;

    for (i = 0; i < 0x10; i++, p += 0x20) {
        if (*(s32 *)p == 0) {
            slot = i;
            break;
        }
    }
    if (i >= 0x10) {
        return -2;
    }
    if (arg0 == -1) {
        arg0 = slot;
    } else if ((u32)arg0 >= 0x10) {
        return -1;
    }
    *(s16 *)(p + 0x18) = arg0;
    *(s8 *)(p + 0x1B) = arg5;
    while (arg2 & 0xF) {
        arg2++;
    }
    *(s32 *)(p + 0x10) = arg2;
    *(s32 *)p = func_001D3308(arg2);
    m = func_001D3308(*(s32 *)(p + 0x10));
    *(s32 *)(p + 4) = m;
    if (*(s32 *)p == 0 || m == 0) {
        goto fail;
    }
    if ((u32)(*(s32 *)p + *(s32 *)(p + 0x10)) > 0x1FFFFF || (u32)(m + *(s32 *)(p + 0x10)) > 0x1FFFFF) {
        goto fail;
    }
    func_001CB918(*(s32 *)p, arg1, *(s32 *)(p + 0x10));
    while (func_001CBA08() == 0) {
    }
    func_001CB808(1, 0x1070, *(s32 *)(p + 4), *(s32 *)p, *(s32 *)(p + 0x10));
    while (arg4 & 0xF) {
        arg4++;
    }
    b = D_0035C554;
    v = *(u32 *)(p + 0xC);
    *(s32 *)(p + 0x14) = arg4;
    if (v == 0) {
        u32 t = *(u32 *)(b + 0x4C0);
        *(u32 *)(p + 0xC) = t;
        v = t;
    }
    if (v >= *(u32 *)(b + 0x4C0)) {
        *(u32 *)(b + 0x4C0) = v + *(s32 *)(p + 0x14);
    }
    m2 = func_001D3308(*(s32 *)(p + 0x14));
    *(s32 *)(p + 8) = m2;
    if (m2 == 0 || (u32)(m2 + *(s32 *)(p + 0x14)) > 0x1FFFFF) {
        goto fail;
    }
    func_001CB918(m2, arg3, *(s32 *)(p + 0x14));
    while (func_001CBA08() == 0) {
    }
    if ((s32)(0x1E7FBF - *(u32 *)(p + 0xC)) < *(s32 *)(p + 0x14)) {
        goto fail;
    }
    if (*(s32 *)(p + 0x1C) != 0) {
        w = *(s32 *)p;
    } else {
        w = *(s32 *)(p + 4);
    }
    if (func_001CB808(1, 0x1010, *(s16 *)(p + 0x18), *(u32 *)(p + 0xC), w, *(s8 *)(p + 0x1B)) != 0) {
fail:
        func_001CC9B0(slot);
        return -1;
    }
    func_001D03E0(0);
    return slot;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CC348);

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. The 0x4C0 block reads D_0035C554 in both arms (the game loads it on each path) */
s32 func_001CC6A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    char *p = D_0035C554;
    s32 i;
    s32 slot;
    s32 m;
    u32 v;
    s32 w;

    for (i = 0; i < 0x10; i++, p += 0x20) {
        if (*(s32 *)p == 0) {
            slot = i;
            break;
        }
    }
    if (i >= 0x10) {
        return -2;
    }
    if (arg0 == -1) {
        arg0 = slot;
    } else if ((u32)arg0 >= 0x11) {
        return -1;
    }
    *(s16 *)(p + 0x18) = arg0;
    *(s8 *)(p + 0x1B) = arg5;
    *(s32 *)(p + 0x10) = arg2;
    *(s32 *)p = arg1;
    if (arg1 == 0) {
        return -1;
    }
    m = func_001D3308(arg2);
    *(s32 *)(p + 4) = m;
    if (m == 0) {
        return -1;
    }
    if ((u32)(m + *(s32 *)(p + 0x10)) > 0x1FFFFF) {
        return -1;
    }
    func_001CB808(1, 0x1070, m, *(s32 *)p, *(s32 *)(p + 0x10));
    v = *(u32 *)(p + 0xC);
    *(s32 *)(p + 0x14) = arg4;
    *(s32 *)(p + 8) = arg3;
    {
    char *b;
    if (v == 0) {
        u32 t;
        b = D_0035C554;
        t = *(u32 *)(b + 0x4C0);
        *(u32 *)(p + 0xC) = t;
        v = t;
    } else {
        b = D_0035C554;
    }
    if (v >= *(u32 *)(b + 0x4C0)) {
        *(u32 *)(b + 0x4C0) = v + *(s32 *)(p + 0x14);
    }
    }
    if (*(s32 *)(p + 8) == 0) {
        return -1;
    }
    if ((s32)(0x1E7FBF - *(u32 *)(p + 0xC)) < *(s32 *)(p + 0x14)) {
        return -1;
    }
    if (*(s32 *)(p + 0x1C) != 0) {
        w = *(s32 *)p;
    } else {
        w = *(s32 *)(p + 4);
    }
    if (func_001CB808(1, 0x1010, *(s16 *)(p + 0x18), *(u32 *)(p + 0xC), w, *(s8 *)(p + 0x1B)) != 0) {
        return -1;
    }
    func_001D03E0(0);
    return slot;
}

/* ER-159: by hand from m2c; names, types and offsets are Unconfirmed readings. The game stores arg1's low byte raw and
   sign-extends only the call's copy, so arg1 is a full word here (an `s8` parameter is extended on entry). */
s32 func_001CC848(s32 arg0, s32 arg1) {
    char *p = (char *)D_0035C554 + (arg0 << 5);
    *(s8 *)(p + 0x1A) = arg1;
    if (func_001CB808(1, 0x90, (s8)arg1, 0, *(s32 *)(p + 8), *(s32 *)(p + 0xC), *(s32 *)(p + 0x14)) != *(s32 *)(p + 0x14)) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CC8B0);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CC988(s32 arg0) {
    return func_001CB808(1, 0x80, arg0, 0);
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CC9B0);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CCAB0);


/* ER-233: by hand (ER-202's draft, recipes 2.50; ER-206); plain C with the file's -fedge-lcm (ruling 3.23); names,
   types and offsets are Unconfirmed readings. */
s32 func_001CCB08(void) {
    char *p[16];
    u8 f[16]; /* ruling 3.18: filled with zeros and never read; the game makes the stores */
    char *e;
    char *a;
    char *b;
    char *s;
    char *d;
    s32 i;
    s32 m;
    s32 k;
    s32 gap;
    u32 total;
    u32 c;
    s32 r;
    s32 v;

    for (i = 15; i != -1; i--) {
        p[i] = (char *)D_0035C554 + i * 0x20;
        f[i] = 0;
    }
    for (gap = 8; gap != 0; gap /= 2) {
        for (m = 0; m < gap; m++) {
            for (i = m + gap; i < 16; i += gap) {
                k = i - gap;
                if (k >= m) {
                    do {
                        a = p[k];
                        b = p[k + gap];
                        if (*(u32 *)(b + 0xC) >= *(u32 *)(a + 0xC)) {
                            break;
                        }
                        p[k] = b;
                        p[k + gap] = a;
                        k -= gap;
                    } while (k >= m);
                }
            }
        }
    }
    total = *(u32 *)((char *)D_0035C554 + 0x4BC);
    for (i = 0; i < 16; i++) {
        e = p[i];
        if (*(s32 *)(e + 8) != 0 && (c = *(u32 *)(e + 0xC)) != 0) {
            if (total < c) {
                *(u32 *)(e + 0xC) = total;
                func_001CB808(1, 0x80, *(s8 *)(e + 0x1A), 1);
                r = func_001CB808(1, 0x90, *(s8 *)(e + 0x1A), 0, *(s32 *)(e + 8), *(u32 *)(e + 0xC), *(s32 *)(e + 0x14));
                if (r != *(s32 *)(e + 0x14)) {
                    return -1;
                }
                total += r;
                func_001CB808(1, 0x80, *(s8 *)(e + 0x1A), 1);
                if (*(s32 *)(e + 0x1C) != 0) {
                    v = *(s32 *)e;
                } else {
                    v = *(s32 *)(e + 4);
                }
                if (func_001CB808(1, 0x1010, *(s16 *)(e + 0x18), *(u32 *)(e + 0xC), v, *(s8 *)(e + 0x1B)) != 0) {
                    return -1;
                }
            } else {
                total = c + *(s32 *)(e + 0x14);
            }
        }
    }
    d = D_0035C554;
    *(u32 *)(d + 0x4C0) = total;
    s = d + 0x200;
    for (i = 0; i < 4; i++, s += 0x10) {
        if (*(s32 *)s != 0 && *(s32 *)(s + 8) != -1) {
            e = (char *)D_0035C554 + (*(s32 *)(s + 8) << 5);
            if (*(s32 *)(e + 0x1C) != 0) {
                v = *(s32 *)e;
            } else {
                v = *(s32 *)(e + 4);
            }
            if (func_001CB808(1, 0x1010, *(s32 *)(s + 0xC), *(s32 *)(e + 0xC), v, *(s8 *)(e + 0x1B)) != 0) {
                return -1;
            }
        }
    }
    func_001D03E0(0);
    return 0;
}


/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CCD98(u32 arg0, s32 arg1) {
    if (arg0 >= 0x10U) {
        return -1;
    }
    *(s32 *)(((char *)D_0035C554 + (arg0 << 5)) + 0xC) = arg1;
    return 0;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001CCDC0(s32 arg0, s32 arg1) {
    func_001CB808(1, 0x1040, *(s16 *)(((char *)D_0035C554 + (arg0 << 5)) + 0x18), arg1 & 0xFF);
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001CCDE0(s32 arg0, s32 arg1) {
    func_001CB808(1, 0x1050, *(s16 *)(((char *)D_0035C554 + (arg0 << 5)) + 0x18), arg1 & 0xFF);
}

/* ER-189: by hand, the early exits first (recipes 2.43: the game's bnel); names, types and offsets are Unconfirmed readings. */
s32 func_001CCE00(u32 arg0, s16 arg1) {
    char *p;

    if (arg0 >= 0x11U) {
        return -1;
    }
    p = (char *)D_0035C554 + (arg0 << 5);
    if (*(s32 *)p == 0) {
        return -1;
    }
    func_001CB808(1, 0x1060, *(s16 *)(p + 0x18), arg1);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CCE58);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CCF60);

extern s32 func_001CE038(s32);
extern s32 func_001CE0D8(s32, s32);

/* ER-233: by hand (ER-198's draft without its `ok` flag; func_001CD360's shape); plain C with the file's -fedge-lcm
   (ruling 3.23); names, types and offsets are Unconfirmed readings. */
s32 func_001CD068(s32 arg0, s32 arg1, s32 arg2) {
    char *b = D_0035C554;
    s32 *e = (s32 *)(b + 0x200);
    char *p;
    s32 i;
    s32 slot;
    s32 v;
    s32 n;
    s32 m;

    for (i = 0; i < 4; i++, e += 4) {
        if (*e == 0) {
            slot = i;
            break;
        }
    }
    if (i >= 4) {
        return -2;
    }
    if (arg0 == -1) {
        arg0 = slot;
    } else if ((u32)arg0 >= 4) {
        return -1;
    }
    p = b + (arg1 << 5);
    e[3] = arg0;
    e[2] = arg1;
    n = func_001CE038(arg2);
    if (n < 0) {
        return -1;
    }
    e[1] = n;
    m = func_001D3308(n);
    e[0] = m;
    if (m == 0) {
        return -1;
    }
    if ((u32)(m + e[1]) > 0x1FFFFF) {
        return -1;
    }
    if (func_001CE0D8(arg2, m) != 0) {
        return -1;
    }
    if (*(s32 *)(p + 0x1C) != 0) {
        v = *(s32 *)p;
    } else {
        v = *(s32 *)(p + 4);
    }
    if (func_001CB808(1, 0x1010, e[3], *(s32 *)(p + 0xC), v, *(s8 *)(p + 0x1B)) != 0) {
        return -1;
    }
    if (func_001CB808(1, 0x2000, e[3], e[0]) != 0) {
        return -1;
    }
    func_001D03E0(0);
    return slot;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD1D0);

/* ER-233: by hand (ER-198's draft; ER-206); plain C with the file's -fedge-lcm (ruling 3.23); names, types and
   offsets are Unconfirmed readings. */
s32 func_001CD360(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    char *b = D_0035C554;
    s32 *e = (s32 *)(b + 0x200);
    char *p;
    s32 i;
    s32 slot;
    s32 v;

    for (i = 0; i < 4; i++, e += 4) {
        if (*e == 0) {
            slot = i;
            break;
        }
    }
    if (i >= 4) {
        return -2;
    }
    if (arg0 == -1) {
        arg0 = slot;
    } else if ((u32)arg0 >= 4) {
        return -1;
    }
    p = b + (arg1 << 5);
    e[3] = arg0;
    e[1] = arg3;
    e[2] = arg1;
    e[0] = arg2;
    if (arg2 == 0) {
        return -1;
    }
    if (*(s32 *)(p + 0x1C) != 0) {
        v = *(s32 *)p;
    } else {
        v = *(s32 *)(p + 4);
    }
    if (func_001CB808(1, 0x1010, e[3], *(s32 *)(p + 0xC), v, *(s8 *)(p + 0x1B)) != 0) {
        return -1;
    }
    if (func_001CB808(1, 0x2000, e[3], e[0]) != 0) {
        return -1;
    }
    func_001D03E0(0);
    return slot;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD468);

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. The two early exits written apart give the game's shared `b; li -1` */
s32 func_001CD4C8(u32 arg0, s32 arg1) {
    char *p;

    if (*(u16 *)((char *)D_0035C554 + 0x4D6) == 0) {
        return -1;
    }
    if (arg0 >= 5U) {
        return -1;
    }
    p = (char *)D_0035C554 + ((arg0 << 4) + 0x200);
    if (*(s32 *)p == 0) {
        return -1;
    }
    return (func_001CB808(1, 0x2030, *(s32 *)(p + 0xC), arg1) == 0) ? 0 : -1;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CD530(u32 arg0) {
    if (arg0 >= 5U) {
        return -1;
    }
    return func_001CB808(1, 0x2010, *(s32 *)(((arg0 * 0x10) + (char *)D_0035C554) + 0x20C));
}

/* ER-189: by hand, the early exits first (recipes 2.43: the game's bnel); names, types and offsets are Unconfirmed readings. */
s32 func_001CD570(u32 arg0) {
    char *p;

    if (arg0 >= 5U) {
        return -1;
    }
    p = (char *)D_0035C554 + ((arg0 << 4) + 0x200);
    if (*(s32 *)p == 0) {
        return -1;
    }
    return (func_001CB808(1, 0x2040, *(s32 *)(p + 0xC)) == 0) ? 0 : -1;
}

/* ER-189: by hand, the early exits first (recipes 2.43: the game's bnel); names, types and offsets are Unconfirmed readings. */
s32 func_001CD5C8(u32 arg0) {
    char *p;

    if (arg0 >= 5U) {
        return -1;
    }
    p = (char *)D_0035C554 + ((arg0 << 4) + 0x200);
    if (*(s32 *)p == 0) {
        return -1;
    }
    return (func_001CB808(1, 0x2050, *(s32 *)(p + 0xC)) == 0) ? 0 : -1;
}

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. */
s32 func_001CD620(u32 arg0) {
    char *p;

    if (*(u16 *)((char *)D_0035C554 + 0x4D6) == 0) {
        return -1;
    }
    if (arg0 >= 5U) {
        return -1;
    }
    p = (char *)D_0035C554 + ((arg0 << 4) + 0x200);
    if (*(s32 *)p == 0) {
        return -1;
    }
    return (func_001CB808(1, 0x2060, *(s32 *)(p + 0xC)) == 0) ? 0 : -1;
}

/* ER-159: by hand from m2c; names, types and offsets are Unconfirmed readings. `arg0 << 4` gives the game's
   `addu $v0, base, index`; m2c's `arg0 * 0x10` in one expression sums the other way round. */
s32 func_001CD680(s32 arg0, s16 arg1) {
    return func_001CB808(1, 0x1060, *(s32 *)((char *)D_0035C554 + (arg0 << 4) + 0x20C), arg1);
}

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. Packet writer into the uncached (| 0x20000000) buffer at +0x4CC; the record stores in the order that matches */
s32 func_001CD6B8(u32 arg0, s32 arg1, s8 arg2, s8 arg3, s8 arg4, s8 arg5, s16 arg6, s8 arg7) {
    s32 *q;
    u8 *w;
    char *r;
    char *e;
    s32 len;

    if (*(u16 *)((char *)D_0035C554 + 0x4D6) == 0) {
        return -1;
    }
    q = (s32 *)(*(s32 *)((char *)D_0035C554 + 0x4CC) | 0x20000000);
    len = (arg3 != -1) ? 0x1B : 0x19;
    if ((u32)(*q + len) >= 0x3FC) {
        return -2;
    }
    r = (char *)D_0035C554 + (arg1 << 5);
    w = (u8 *)((s32)(&((char *)q)[*q + 4]) | 0x20000000);
    if (arg0 >= 0x7E) {
        return -1;
    }
    e = (char *)D_0035C554 + (((arg0 & 0x7F) << 2) + 0x240);
    e[3] = 1;
    e[0] = arg1;
    e[2] = arg4;
    e[1] = arg2;
    *w++ = 0xFF;
    *w++ = *(u8 *)(r + 0x18);
    *w++ = len - 3;
    if (arg3 != -1) {
        *w++ = arg2 - 0x40;
        *w++ = arg3;
    }
    if (*(u16 *)((char *)D_0035C554 + 0x4D4) == 0 || *(s32 *)(r + 0x1C) == 0) {
        arg7 = 0x3F;
    }
    *w++ = 0xF9;
    *w++ = 1;
    *w++ = arg2;
    *w++ = arg7;
    *w++ = 0;
    *w++ = 0xF9;
    *w++ = 2;
    *w++ = arg2;
    *w++ = arg6 & 0x7F;
    *w++ = ((arg6 >> 7) + 0x40) & 0x7F;
    *w++ = 0xF9;
    *w++ = 0;
    *w++ = arg2;
    *w++ = arg5;
    *w++ = 0;
    *w++ = 0xFD;
    *w++ = 0x10;
    *w++ = arg2;
    *w++ = arg4;
    *w++ = arg0;
    w[0] = 0x7F;
    w[1] = 0;
    *q += len;
    return arg0;
}

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. Packet writer (see func_001CD6B8) */
s32 func_001CD8D8(u32 arg0) {
    s32 *q;
    u8 *w;
    char *e;

    if (*(u16 *)((char *)D_0035C554 + 0x4D6) == 0 || arg0 >= 0x7E) {
        return -1;
    }
    q = (s32 *)(*(s32 *)((char *)D_0035C554 + 0x4CC) | 0x20000000);
    if ((u32)(*q + 0xA) >= 0x3FC) {
        return -2;
    }
    w = (u8 *)((s32)(&((char *)q)[*q + 4]) | 0x20000000);
    e = (char *)D_0035C554 + ((arg0 << 2) + 0x240);
    *w++ = 0xFF;
    *w++ = *(u8 *)((char *)D_0035C554 + (*(s8 *)e << 5) + 0x18);
    *w++ = 7;
    *w++ = 0xFD;
    *w++ = 0x10;
    *w++ = *(u8 *)(e + 1);
    *w++ = *(u8 *)(e + 2);
    *w++ = arg0;
    w[0] = 0;
    w[1] = 0;
    *q += 0xA;
    *(s8 *)(e + 3) = -1;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CD9C8);

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. Packet writer (see func_001CD6B8) */
s32 func_001CDB60(s32 arg0, s8 arg1, s8 arg2) {
    s32 *q;
    u8 *w;
    s32 n;

    if (*(u16 *)((char *)D_0035C554 + 0x4D6) == 0) {
        return -1;
    }
    q = (s32 *)(*(s32 *)((char *)D_0035C554 + 0x4CC) | 0x20000000);
    if ((u32)(*q + 5) >= 0x3FC) {
        return -2;
    }
    w = (u8 *)((s32)(&((char *)q)[*q + 4]) | 0x20000000);
    *w++ = 0xFF;
    *w++ = *(u8 *)((char *)D_0035C554 + (arg0 << 5) + 0x18);
    *w++ = 2;
    w[0] = arg1 - 0x40;
    w[1] = arg2;
    *q += 5;
    return 0;
}

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. Sends the buffer through func_001D0520 / func_001D0500 (a DMA send and status poll by shape, Unconfirmed) */
s32 func_001CDC08(void) {
    s32 *q;
    s32 n;
    s32 id;
    s32 a;

    if (*(u16 *)((char *)D_0035C554 + 0x4D6) == 0) {
        return -1;
    }
    q = (s32 *)(*(s32 *)((char *)D_0035C554 + 0x4CC) | 0x20000000);
    n = *q + 4;
    while (n & 0xF) {
        n++;
    }
    a = *(s32 *)((char *)D_0035C554 + 0x4CC);
    D_01B73E78[1] = *(s32 *)((char *)D_0035C554 + 0x4C4);
    D_01B73E78[0] = a;
    D_01B73E78[2] = n;
    D_01B73E78[3] = 0;
    id = func_001D0520(D_01B73E78, 1);
    if (id == 0) {
        return -1;
    }
    while (func_001D0500(id) >= 0) {
    }
    *q = 0;
    func_001CB808(1, 0x1080);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CDCE0);

extern s32 func_001E9D78(s32, s32);
extern s32 D_01B7B900[];
extern s32 func_001D23A8(s32 *, s32, s32, s32 *, s32, s32 *, s32, s32, s32);
extern s32 D_01B73E50[];
extern s32 D_01B73840[];

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CDDA0(s32 arg0, s32 arg1, s32 arg2) {
    if ((func_001CB808(1, 0x3040, arg0, arg2) != 0) || (func_001E9D78((s32) D_01B7B900, arg1), func_001D23A8(D_01B73E50, arg0 | 0x30A0, 0, D_01B7B900, 0x100, D_01B73840, 1, 0, 0), (D_01B73840[0] != 0))) {
        func_001CB808(1, 0x3030, arg0);
        return -1;
    }
    return arg0;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CDE58(s32 arg0, s32 arg1, s32 arg2) {
    return (func_001CB808(1, 0x30C0, arg0, arg1, arg2) == 0) ? arg0 : -1;
}

/* ER-237: by hand; plain C with the file's -fedge-lcm (ruling 3.23); names, types and offsets are Unconfirmed readings. The record stores in offset order (24-byte entries at +0x440, -1 = free) */
s32 func_001CDEA0(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6) {
    char *b = D_0035C554;
    s32 *e = (s32 *)(b + 0x440);
    s32 i;
    s32 slot;

    for (i = 0; i < 4; i++, e += 6) {
        if (*e == -1) {
            slot = i;
            break;
        }
    }
    if (i >= 4) {
        return -2;
    }
    e[0] = 1;
    e[1] = arg0 | 0x100;
    *(s16 *)((char *)e + 8) = arg1;
    *(s16 *)((char *)e + 0xA) = arg2;
    e[3] = arg3;
    e[4] = arg4;
    *(s16 *)((char *)e + 0x14) = arg5;
    *(s16 *)((char *)e + 0x16) = arg6;
    func_001D03E0(0);
    return slot;
}

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CDF78);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CDFD0);

INCLUDE_ASM("asm/nonmatchings/text_001CB528", func_001CE038);

extern s32 func_001D33F0(s32, s32);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CE0D8(s32 arg0, s32 arg1) {
    if (*(s32 *)((char *)D_0035C554 + 0x4B8) == 0) {
        *(s32 *)((char *)D_0035C554 + 0x4B8) = arg1;
    }
    do {

    } while (func_001D33F0(arg0, arg1) < 0);
    return 0;
}
