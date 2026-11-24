#include "common.h"

typedef struct {
    char pad0[0x120];
    u16 x120;
    u16 x122;
    char pad124[0x40];
    u16 x164;
    char pad166[0x14];
    u8 x17A;
    u8 pad17B;
    u8 x17C;
    u8 pad17D;
    u8 x17E;
    u8 x17F;
    u8 x180;
    char pad181[0x3F];
} R1C0;

typedef struct {
    u8 x0;
    u8 x1;
    u8 x2;
    u8 x3;
} E4;

typedef struct {
    char pad0[0x102];
    u16 x102;
    char pad104[0x1C];
    u16 x120;
    u16 x122;
    E4 e[16];
    u16 x164;
} P1C0;

extern char D_01B72600[];
extern s32 func_001D6308(s32);
extern s32 func_001D6530(s32, s32, void *);

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CA8D8);

/* ER-185: drafted with m2c (tools/m2c_bulk.py, ER-179 run), rewritten by hand; names, types and offsets are Unconfirmed readings. */
void func_001CA9A8(P1C0 *arg0) {
    s32 i;
    E4 *e;
    u16 m;
    u16 on;
    u16 en;

    arg0->x122 = 0;
    i = 0;
    on = arg0->x164;
    en = arg0->x102;
    do {
        e = &arg0->e[i];
        m = 1 << i;
        if (on & m) {
            if (!(en & m)) {
                e->x3 = 0;
            } else if (arg0->x120 & m) {
                arg0->x122 = m | arg0->x122;
                e->x3 = 1;
                e->x2 = e->x0;
            } else if (--e->x2 == 0) {
                arg0->x122 = m | arg0->x122;
                e->x3 = 2;
                e->x2 = e->x1;
            }
            on = arg0->x164;
            en = arg0->x102;
        }
        i++;
    } while (i < 16);
}

/* ER-185: drafted with m2c (tools/m2c_bulk.py, ER-179 run), rewritten by hand; names, types and offsets are Unconfirmed readings. */
void func_001CAA58(s32 arg0) {
    R1C0 *p;

    D_01B72600[0xE00] = 0;
    D_01B72600[0xE01] = 0;
    p = (R1C0 *)D_01B72600;
    do {
        p->x120 = 0;
        p->x122 = 0;
        p->x164 = 0;
        p->x17A = 0;
        p->x17C = 0;
        p->x17E = 0xFF;
        p->x17F = 0xFF;
        p->x180 = 0;
        p++;
    } while ((s32)p < (s32)(D_01B72600 + 0xE00));
    (void)&arg0; /* ruling 3.14 */
    func_001D6308(arg0);
}

/* ER-185: drafted with m2c (tools/m2c_bulk.py, ER-179 run), rewritten by hand; names, types and offsets are Unconfirmed readings. */
s32 func_001CAAC0(s32 arg0, s32 arg1) {
    char *b = D_01B72600;
    char *f = b + 0xE00;

    f[arg0] |= 1 << arg1;
    return func_001D6530(arg0, arg1, b + ((arg0 * 4) + arg1) * 0x1C0);
}

INCLUDE_ASM("asm/nonmatchings/text_001CA8D8", func_001CAB10);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
s32 func_001CB218(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(s32 *)(p + 0x168);
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
s32 func_001CB240(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(s32 *)(p + 0x16C);
}

extern char D_01B72700[];

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void *func_001CB268(s32 arg0, s32 arg1) {
    return (((arg0 * 4) + arg1) * 0x1C0) + D_01B72700;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
u16 func_001CB288(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(u16 *)(p + 0x102);
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
u16 func_001CB2B0(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(u16 *)(p + 0x120);
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
u16 func_001CB2D8(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(u16 *)(p + 0x122);
}

/* ER-179: drafted with m2c (tools/m2c_bulk.py, narrow parameters arg2-arg4 u16, ER-172), rewritten as plain C by hand (`|= arg2` gives the game's `or` operand order); names, types and offsets are Unconfirmed readings. */
void func_001CB300(s32 arg0, s32 arg1, u16 arg2, u16 arg3, u16 arg4) {
    s32 temp_2;
    s32 var_11;
    s8 *var_10;
    s8 *var_3;
    s8 *var_5;
    s8 *var_9;
    char *temp_4;

    var_11 = 0;
    temp_4 = (((arg0 * 4) + arg1) * 0x1C0) + D_01B72600;
    var_10 = temp_4 + 0x124;
    var_9 = temp_4 + 0x127;
    var_5 = temp_4 + 0x126;
    var_3 = temp_4 + 0x125;
    *(u16 *)(temp_4 + 0x164) |= arg2;
    do {
        temp_2 = arg2 >> var_11;
        var_11 += 1;
        if (temp_2 & 1) {
            *var_10 = arg3;
            *var_3 = arg4;
            *var_5 = 0;
            *var_9 = 0;
        }
        var_9 += 4;
        var_5 += 4;
        var_3 += 4;
        var_10 += 4;
    } while (var_11 < 0x10);
}

/* ER-179: drafted with m2c (tools/m2c_bulk.py --plain, narrow parameter arg2 u16, ER-172); names, types and offsets are Unconfirmed readings. */
void func_001CB388(s32 arg0, s32 arg1, u16 arg2) {
    void *temp_4;

    temp_4 = (((arg0 * 4) + arg1) * 0x1C0) + ((char *)D_01B72600);
    *(u16 *)((char *)temp_4 + 0x164) = (u16) (*(u16 *)((char *)temp_4 + 0x164) & ~arg2);
}

/* ER-185: drafted with m2c (tools/m2c_bulk.py, ER-179 run), rewritten by hand; names, types and offsets are Unconfirmed readings. */
void func_001CB3C0(s32 arg0, s32 arg1, u8 *arg2) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    char *d = p + 0x174;
    s32 i;

    for (i = 0; i < 6; i++) {
        d[i] = arg2[i];
    }
}

/* ER-185: drafted with m2c (tools/m2c_bulk.py, ER-179 run), rewritten by hand; names, types and offsets are Unconfirmed readings. */
void func_001CB410(s32 arg0, s32 arg1, u8 arg2) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;

    if (arg2 != 0) {
        p[0x174] = 1;
    } else {
        p[0x174] = 0;
    }
    p[0x175] = arg2;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001CB450(s32 arg0, s32 arg1, s8 arg2) {
    void *temp_4;

    temp_4 = (((arg0 * 4) + arg1) * 0x1C0) + D_01B72600;
    *(s8 *)((char *)temp_4 + 0x180) = arg2;
    *(s8 *)((char *)temp_4 + 0x17F) = 0;
}

/* ER-185: drafted with m2c (tools/m2c_bulk.py, ER-179 run), rewritten by hand; names, types and offsets are Unconfirmed readings. */
s32 func_001CB478(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_4;

    temp_4 = (((arg0 * 4) + arg1) * 0x1C0) + D_01B72600;
    if (!(*(u8 *)((char *)temp_4 + 0x17D) & 1)) {
        return 0;
    }
    *(s8 *)((char *)temp_4 + 0x180) = arg2;
    *(s8 *)((char *)temp_4 + 0x17F) = 1;
    return 1;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001CB4C0(s32 arg0, s32 arg1) {
    void *temp_4;

    temp_4 = (((arg0 * 4) + arg1) * 0x1C0) + D_01B72600;
    if (!(*(u8 *)((char *)temp_4 + 0x17D) & 4)) {
        return 0;
    }
    *(s8 *)((char *)temp_4 + 0x17F) = 2;
    return 1;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), base pointer by hand (recipes 2.25); names, types and offsets are Unconfirmed readings. */
u8 func_001CB500(s32 arg0, s32 arg1) {
    char *p = D_01B72600 + ((arg0 * 4) + arg1) * 0x1C0;
    return *(u8 *)(p + 0x17E);
}
