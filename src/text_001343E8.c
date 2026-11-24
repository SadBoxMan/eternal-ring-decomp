#include "common.h"

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001343E8);

/* ER-014. Types and offsets are Unconfirmed readings. */
int func_00134460(char *a0) {
    char *p = *(char **)(a0 + 0xE8);

    if (p == NULL) {
        return 0;
    }
    return *(u16 *)(p + 4) == 3;
}

void func_00134480(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134488);

extern s32 func_00131088(void *arg0);
extern void func_00134070(s8 *arg0);
extern void func_00134090(void);
extern void func_00131000(char *, void *);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00134618(void *arg0, s32 arg1) {
    s32 temp_2;
    s8 *temp_16;

    (void)&arg0; /* ruling 3.14 */
    temp_16 = *(s8 **)((char *)arg0 + 0xE8);
    if (temp_16 != NULL) {
        temp_2 = func_00131088(temp_16);
        if ((temp_2 == func_00134070) || (temp_2 == func_00134090)) {
            if (*(s32 *)((char *)temp_16 + 0x2C) != arg1) {
                *(s32 *)((char *)temp_16 + 0x2C) = arg1;
                func_00131000(temp_16, func_00134090);
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134690);

extern s32 func_00111660(void *);

/* ER-159: Unofficial type for func_001346D0's object: a word array at 0x10 (element count Unconfirmed). */
typedef struct {
    s32 unk0[4];
    s32 unk10[1];
} Count346D0;

/* ER-159: by hand from m2c; names, types and offsets are Unconfirmed readings. The array as a struct member gives the
   game's `addiu base, 0x10; addu base, index` (recipes 2.35); every pointer-sum spelling is a word shorter. */
s32 func_001346D0(Count346D0 *arg0, void *arg1) {
    arg0->unk10[func_00111660(arg1)]++;
    return 1;
}

extern void func_001321D8(void *arg0);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00134718(void *arg0, void *arg1) {
    *(s8 *)((char *)arg0 + 0x5A) = 0;
    *(u16 *)((char *)arg0 + 0x64) = (u16) *(u16 *)((char *)(*(void **)((char *)arg1 + 0x11C)) + 0x1A2);
    func_001321D8(arg0);
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134730);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134790);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001348B8);

extern s32 func_00131140(char *, void *, void *, void *, void *, void *);
extern s32 func_0012E568(s32, s32);
extern s32 func_0012E5C0(s32 lo, s32 hi);
extern s32 func_0012B6B0(void **arg0, s32 arg1, s32 arg2, f32 fparg0, s32 arg3);
extern float func_0012E820(char *, float, float);
extern void *func_00134B60(void *);
extern s32 func_0012E3B0(void **, s32 *, s32);
extern void func_00134BB0(s8 *, void *);
extern void func_00134DB0(s8 *, void *);
extern void func_001352B8(void **arg0);

/* ER-198: by hand from the m2c draft; the double abs as in recipes 2.46 and the D_003546D8 compare written as the
   float literal 0.017453292f promoted (ruling 3.10: = 0x3F91DF46A0000000; drop its INCLUDE_RODATA line); the x30 store
   before the x6E step; func_0012B6B0 with the float before the last int (its definition in text_001000C0 still has
   it last, as func_0012B890 had) and func_0012E3B0 declared as text_00150878 does (s32 third parameter: the game
   passes func_0012E568's value unextended); names, types and offsets are Unconfirmed readings. */
void func_00134958(void **arg0, void *arg1) {
    f32 f;
    f32 t;
    double d;
    void **p;

    *(s8 *)((char *)arg0 + 0xE) = 0;
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(u16 *)((char *)arg0 + 0x74) = func_0012E568(0x78, 0x12C);
        t = (f32) func_0012E5C0(0xA, 0x2D) * 3.1415927f / 180.0f;
        *(f32 *)((char *)arg0 + 0x30) = *(f32 *)((char *)arg1 + 0xA4) + t;
        *(s16 *)((char *)arg0 + 0x6E) += 1;
    }
    f = 0.1f;
    func_0012B6B0(arg0, 1, 0, f, 1);
    if (--*(s16 *)((char *)arg0 + 0x74) < 0) {
        *(u16 *)((char *)arg0 + 0x74) = 0;
        func_0012B6B0(arg0, 0, 0xC, 0.5f, 0);
        d = func_0012E820((char *)arg1 + 0xA4, *(f32 *)((char *)arg0 + 0x30), 10.0f);
        if (d < 0.0) {
            d = 0.0 - d;
        }
        if (d <= 0.017453292f) {
            func_00131000((char *)arg0, func_00134958);
        }
    } else {
        func_0012B6B0(arg0, 0, 0, f, 1);
    }
    if (func_00131140((char *)arg0, func_00134BB0, func_00134BB0, func_00134DB0, func_00134DB0, NULL) != 0) {
        *(s8 *)((char *)arg0 + 0xE) = 1;
        return;
    }
    p = arg0;
    while (*(void ***)((char *)p + 0xE8) != NULL) {
        p = *(void ***)((char *)p + 0xE8);
    }
    if (func_00134B60(p) != NULL) {
        func_0012E3B0(arg0, (s32 *) func_001352B8, func_0012E568(0x14, 0x3C));
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134B60);

extern s32 func_0012B890(void **, s32, s32, s32, s16, f32, s32);
extern s32 func_0012F9C0(void **, s32);
extern s32 func_0012FC38(void **, s32, s32, s32);
extern s32 func_00130780(void *arg0);
extern u8 D_001FF451[];
extern float func_00130CE0(char *arg0, f32 fparg0);
extern s32 func_0012F938(void *arg0, s32 arg1);
extern s32 func_001E9C70();
extern s32 func_0012BF08(void *out, char *a1, float *pos);
extern void func_00135378();
extern void func_00135610(void **arg0, void *arg1);

/* ER-198: by hand from the m2c draft; the func_0012BF08 test as if / else with a call in each arm (a return after
   the first gives a sibling j); func_00130CE0 declared float as its definition and func_0012F9C0 s32 as
   text_001000C0 has it (this file's externs say s32 / void and change with them; the x6E step after the call takes
   $v1); names, types and offsets are Unconfirmed readings. */
void func_00134BB0(s8 *arg0, void *arg1) {
    f32 f;

    *(s8 *)(arg0 + 0xE) = 1;
    if (*(s16 *)(arg0 + 0x6E) == 0) {
        *(s8 *)(arg0 + 8) = 0;
        *(s16 *)(arg0 + 0x6E) += 1;
        *(s32 *)(arg0 + 0x90) = 0;
        *(s32 *)(arg0 + 0x94) = 0;
        *(s32 *)(arg0 + 0x98) = 0;
        *(s32 *)(arg0 + 0x9C) = 0;
    }
    if (*(s16 *)(arg0 + 0x6C) < 0x14) {
        func_00130CE0(arg0, 10.0f);
        if (*D_001FF451 != 0 && *(u16 *)(arg0 + 4) == 6 && func_001E9C70() % 100 < 11 && func_00130780(arg0) == 0) {
            func_00131000(arg0, func_00135610);
            return;
        }
    }
    f = 0.1f;
    func_0012B890((void **)arg0, 0, 0xB, *(s16 *)(arg0 + 0x6C), 0x14, f, 0);
    if (func_0012B890((void **)arg0, 1, 0xB, *(s16 *)(arg0 + 0x6C), 0x14, f, 0) != 0
        && *(u16 *)(*(char **)((char *)arg1 + 0xE0) + 0x8C) == 0x3C) {
        func_0012F9C0((void **)arg0, 2);
        *(s16 *)(arg0 + 0x6E) += 1;
    }
    if (*(s8 *)(arg0 + 8) == 0) {
        if (func_0012FC38((void **)arg0, 1, 0x32, 0x46) != 0) {
            *(s8 *)(arg0 + 8) = 1;
        }
    }
    if (func_0012F938(arg1, 0xB) != 0) {
        if (func_0012BF08(NULL, arg0, *(float **)(arg0 + 0x130)) != 0) {
            func_00131000(arg0, func_00135378);
        } else {
            func_00131140(arg0, func_00134BB0, func_00134BB0, func_00134DB0, func_00134DB0, func_001352B8);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134DB0);

extern s32 func_0012B930(char *, s32, s32, s32, s16, f32, s32);
s32 func_0012BA30(void *, s32, s32);
extern void func_0012B540(void *, s32, s32, s32);
extern s32 func_00135810(s8 *, void *);
extern f32 func_00135958(void *arg0, f32 fparg0);
extern void func_00135158();

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134F70);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135158);

extern s32 func_0012B890(void **arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, f32 fparg0, s32 arg5);
extern float func_00130D00(char *, float);
extern void func_00134958(void **, void *);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001352B8(void **arg0) {
    f32 temp_f20;

    temp_f20 = 0.1f;
    *(s8 *)((char *)arg0 + 0xE) = 0;
    func_0012B890(arg0, 1, 0xD, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x14, temp_f20, 1);
    func_0012B930((s8 *) arg0, 0, 0xC, (s32) *(s16 *)((char *)arg0 + 0x6C), 0xA, temp_f20, 1);
    if (func_00130D00((s8 *) arg0, 10.0f) <= 0.2617994f) {
        func_00131140((s8 *) arg0, func_00134BB0, func_00134BB0, func_00134DB0, func_00134DB0, func_00134958);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135378);

extern void func_0012B218(void *arg0, s32 arg1);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00135540(void *arg0) {
    func_0012B218(arg0, 2);
    func_0012B218(arg0, 1);
}

extern s32 func_0014FF38(void *arg0, s32 arg1);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00135570(s8 *arg0) {
    if (func_0014FF38(arg0, 3) != 0) {
        func_00131000(arg0, func_001352B8);
    }
}

extern s32 func_0012FA68(void *arg0);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001355B8(s8 *arg0) {
    if (func_00131088(arg0) == func_00134958) {
        func_0012FA68(arg0);
        func_00131000(arg0, func_001352B8);
    }
}

extern s32 func_0012B7C0(void **, u32, s32, s32, s16, s32, f32 fparg0);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00135610(void **arg0, void *arg1) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    *(s8 *)((char *)arg0 + 0xF) = 1;
    if (func_0012B7C0(arg0, 3U, 0xF, (s32) *(s16 *)((char *)arg0 + 0x6C), 0xA, 0, 0.3f) != 0) {
        *(u8 *)((char *)arg1 + 0x147) = (u8) (*(u8 *)((char *)arg1 + 0x147) & 0xFD);
    }
    if (*D_001FF451 == 0) {
        func_00131000((s8 *) arg0, func_001352B8);
        *(s8 *)((char *)arg0 + 0xF) = 0;
    }
}

extern f32 func_00135900(void *, s32, f32 fparg0);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001356B0(void **arg0, s32 arg1) {
    func_0012B7C0(arg0, 3U, 0xA, (s32) *(s16 *)((char *)arg0 + 0x6C), 5, 0, 0.3f);
    *(s8 *)((char *)arg0 + 0xF) = 1;
    if (func_00135900(arg0, arg1, 0.8f) <= 0.05f) {
        *(s8 *)((char *)arg0 + 0xF) = 0;
        func_00131000((s8 *) arg0, func_001352B8);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135740);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135810);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135900);

extern void func_001CA048(float *, float *, float);

extern float sqrtf(float);
/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
f32 func_00135958(void *arg0, f32 fparg0) {
    f32 *temp_4;
    f32 temp_f0;
    f32 temp_f1;

    temp_4 = arg0 + 0x90;
    func_001CA048(temp_4, temp_4, fparg0);
    temp_f0 = *(f32 *)((char *)arg0 + 0x90);
    temp_f1 = *(f32 *)((char *)arg0 + 0x98);
    return sqrtf((temp_f0 * temp_f0) + (temp_f1 * temp_f1));
}

extern void func_0012B1B8(void *, s32, s32, s8);
extern void func_001368A0(void **);
extern void func_001368D8(s8 *);
extern void func_00136920(s8 *);
extern void func_00136968(void **, s8 *);
extern void func_00136B18(s8 *, s32);

extern void func_00135A50(void **arg0);
/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), fixed by hand; names, types and offsets are Unconfirmed readings. */
void func_001359C0(s8 *arg0) {
    *(void (**)(void **))((char *)arg0 + 0xC0) = func_001368A0;
    *(void (**)(s8 *))((char *)arg0 + 0xC4) = func_001368D8;
    *(void (**)(s8 *))((char *)arg0 + 0xC8) = func_00136920;
    *(void (**)(void **, s8 *))((char *)arg0 + 0xCC) = func_00136968;
    *(void (**)(s8 *, s32))((char *)arg0 + 0xD8) = func_00136B18;
    *(f32 *)((char *)arg0 + 0xE4) = (f32) 2.5e+07f;
    *(f32 *)((char *)arg0 + 0xB4) = (f32) 0.4f;
    func_0012B1B8(*(void **)arg0, 3, 0xF, 2);
    func_00131000(arg0, func_00135A50);
}

extern void func_00136D10(void **arg0, s16 arg1);
extern void func_00135AA8(s8 *arg0);
extern void func_00135AF0(s8 *);
extern void func_00135B88(s8 *);
extern void func_00135C50(s8 *);
extern void func_00135D08(s8 *);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00135A50(void **arg0) {
    func_00136D10(arg0, 0x14);
    func_00131140((s8 *) arg0, func_00135AA8, func_00135AF0, func_00135B88, func_00135C50, func_00135D08);
}

extern void func_001363C0(void **, void *);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00135AA8(s8 *arg0) {
    func_00130CE0(arg0, 15.0f);
    func_00136D10((void **) arg0, 0x14);
    func_00131000(arg0, func_001363C0);
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135AF0);

extern void* func_00131020(char *arg0, s32 arg1, void *arg2, void *arg3);
extern void func_00135DC0(s8 *, void *);
extern void func_00135FE0(void);

/* ER-189: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00135B88(s8 *arg0) {
    func_00130CE0(arg0, 15.0f);
    func_00136D10((void **) arg0, 0x14);
    if (func_00131140(arg0, func_00135AA8, func_00135AF0, NULL, func_00135C50, func_00135D08) == 0) {
        func_00136D10((void **) arg0, 0x14);
        if ((*(s16 *)((char *)arg0 + 0x6C) >= 0x14) && (func_00131020(arg0, 0x14, func_00135DC0, NULL) == NULL)) {
            func_00131020(arg0, 5, func_00135FE0, NULL);
        }
    }
}

extern void func_00136760(s8 *, void *);

/* ER-189: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00135C50(s8 *arg0) {
    func_00130CE0(arg0, 15.0f);
    if (func_00131140(arg0, func_00135AA8, func_00135AF0, func_00135B88, NULL, func_00135D08) == 0) {
        func_00136D10((void **) arg0, 0x14);
        if ((*(s16 *)((char *)arg0 + 0x6C) >= 0x14) && (func_00131020(arg0, 0x14, func_00136760, NULL) == NULL)) {
            func_00131020(arg0, 0x1E, func_00135DC0, NULL);
        }
    }
}

/* ER-189: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00135D08(s8 *arg0) {
    func_00130CE0(arg0, 15.0f);
    if (func_00131140(arg0, func_00135AA8, func_00135AF0, func_00135B88, func_00135C50, NULL) == 0) {
        func_00136D10((void **) arg0, 0x14);
        if ((*(s16 *)((char *)arg0 + 0x6C) >= 0x14) && (func_00131020(arg0, 0x1E, func_00135DC0, NULL) == NULL)) {
            func_00131020(arg0, 0x14, func_00136760, NULL);
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/text_001343E8", D_003546F8);
INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135DC0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135FE0);

extern s32 func_00137028(void **, s8 *, void *);
extern s32 D_0032FD40[];
extern s32 func_00130FE0(char *arg0, void *arg1);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00136058(void **arg0, s8 *arg1) {
    if (func_00137028(arg0, arg1, D_0032FD40) != 0) {
        func_00130FE0((s8 *) arg0, func_00135A50);
    }
}

extern s32 D_0032FD60[];

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001360A0(void **arg0, s8 *arg1) {
    if (func_00137028(arg0, arg1, D_0032FD60) != 0) {
        func_00130FE0((s8 *) arg0, func_00135A50);
    }
}


/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001360E8(void **arg0, void *arg1) {
    f32 temp_f20;

    temp_f20 = 0.1f;
    func_0012B890(arg0, 0, 0xF, (s32) *(s16 *)((char *)arg0 + 0x6C), 0xA, temp_f20, 1);
    if (func_0012B890(arg0, 1, 0xF, (s32) *(s16 *)((char *)arg0 + 0x6C), 0xA, temp_f20, 1) != 0) {
        if (*(u16 *)((char *)(*(void **)((char *)arg1 + 0xE0)) + 0x8C) == 0x5A) {
            func_0012F9C0(arg0, 4);
        }
    }
    if (func_0012F938(arg1, 0xF) != 0) {
        func_00130FE0((s8 *) arg0, func_00135A50);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001361B0);

extern s32 func_001371C0(char **a0);
extern void func_001365A0();

/* ER-198: by hand from the m2c draft; the two exits after func_0012F9C0 / func_001371C0 written as early returns, the
   func_0012B890 == 0 arm first; func_00130CE0 declared float as its definition (this file's extern says s32 and
   changes with it; the file's other C functions compile the same: s32 takes $v0 from the D_001FF451 load); the two
   func_00131020 tests joined with && around the last call (separate early returns give a sibling j); names, types and offsets are Unconfirmed readings. */
void func_001363C0(void **arg0, void *arg1) {
    f32 f;
    char *p;
    s32 a;
    s32 b;

    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 8) = 0;
        *(s16 *)((char *)arg0 + 0x6E) += 1;
    }
    f = 0.08f;
    func_0012B890(arg0, 0, 8, *(s16 *)((char *)arg0 + 0x6C), 0x14, f, 0);
    if (func_0012B890(arg0, 1, 8, *(s16 *)((char *)arg0 + 0x6C), 0x14, f, 0) == 0) {
        func_00130CE0((char *)arg0, 10.0f);
        if (*D_001FF451 != 0 && func_001E9C70() % 101 < 11) {
            if (func_001371C0((char **)arg0) != 0) {
                return;
            }
        }
    } else {
        if (*(s16 *)((char *)arg0 + 0x6C) == 0x14) {
            func_0012F9C0(arg0, 2);
            if (func_00130780(arg0) >= 2) {
                return;
            }
        }
    }
    if (*(s8 *)((char *)arg0 + 8) == 0) {
        if (func_0012FC38(arg0, 0, 0x14, 0x32) != 0) {
            *(s8 *)((char *)arg0 + 8) = 1;
        }
    }
    if (func_0012F938(arg1, 8) == 0) {
        return;
    }
    p = *(char **)arg0;
    *(u8 *)(p + 0x147) &= 0xFE;
    a = 0x50;
    if (*(s8 *)((char *)arg0 + 8) != 0) {
        b = 0xA;
    } else {
        a = 0x28;
        b = 0x32;
    }
    if (func_00131020((char *)arg0, a, func_001365A0, NULL) == NULL && func_00131020((char *)arg0, b, func_00135FE0, NULL) == NULL) {
        func_00131000((char *)arg0, func_00135A50);
    }
}

/* ER-198: by hand from the m2c draft; both early exits written first (func_00130780 >= 2, func_0012B890 == 0);
   func_00130CE0 declared float as its definition (this file's extern says s32; see func_001363C0); names, types and
   offsets are Unconfirmed readings. */
void func_001365A0(s8 *arg0, void *arg1) {
    f32 f;

    if (*(s16 *)(arg0 + 0x6E) == 0) {
        if (func_00130780(arg0) >= 2) {
            func_00130FE0(arg0, func_00135A50);
            return;
        }
        *(s8 *)(arg0 + 8) = 0;
        *(s16 *)(arg0 + 0x6E) += 1;
    }
    f = 0.08f;
    func_0012B890((void **)arg0, 0, 9, *(s16 *)(arg0 + 0x6C), 0x14, f, 0);
    if (func_0012B890((void **)arg0, 1, 9, *(s16 *)(arg0 + 0x6C), 0x14, f, 0) == 0) {
        func_00130CE0(arg0, 10.0f);
        if (*D_001FF451 != 0 && func_001E9C70() % 101 < 11) {
            func_001371C0((char **)arg0);
        }
        return;
    }
    if (*(s16 *)(arg0 + 0x6C) == 0x14) {
        func_0012F9C0((void **)arg0, 2);
    }
    if (*(s8 *)(arg0 + 8) == 0) {
        if (func_0012FC38((void **)arg0, 1, 1, 0x32) != 0) {
            *(s8 *)(arg0 + 8) = 1;
        }
    }
    if (func_0012F938(arg1, 9) != 0) {
        func_00130CE0(arg0, 10.0f);
        if (*(s8 *)(arg0 + 8) != 0) {
            func_00131000(arg0, func_00135FE0);
        } else {
            func_00131000(arg0, func_00135A50);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136760);

extern void func_00136D88(void **arg0);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001368A0(void **arg0) {
    func_0012B218(arg0, 2);
    func_0012B218(arg0, 1);
    func_00136D88(arg0);
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001368D8(s8 *arg0) {
    if (func_0014FF38(arg0, 3) != 0) {
        func_00131000(arg0, func_00135A50);
    }
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00136920(s8 *arg0) {
    if (func_0014FF38(arg0, 3) != 0) {
        func_00131000(arg0, func_00135A50);
    }
}

extern s32 D_0032FD90[];
s32 func_0012F9C0(void **, s32);
void func_0012FB48(void *arg0, s32 arg1);
s32 func_0012FEB8(s8 arg0, s32 arg1, s32 arg2);
void func_001300B0(void *, s8 *, f32 fparg0, f32 fparg1);
void func_001321E8(void *arg0);
void func_00136EC0(void **, f32 *);
s32 func_001501E0(float *arg0);
void func_001C9FE8(void *, void *, void *);

/* ER-189: m2c draft by hand: func_0012B890 with the float before the last int (as text_0013C510; recipes 2.42), the x74
   counter read back as s16. Names, types and offsets are Unconfirmed readings. */
void func_00136968(void **arg0, s8 *arg1) {
    f32 sp[4];
    s32 *var_17;
    f32 temp_f20;
    u16 temp_20;
    u32 var_18;

    if ((*(s16 *)((char *)arg0 + 0x6E)) == 0) {
        func_0012FB48(arg0, 1);
        (*(u16 *)((char *)arg0 + 0x74)) = 0U;
        (*(s16 *)((char *)arg0 + 0x6E)) = (s16) ((u16) (*(s16 *)((char *)arg0 + 0x6E)) + 1);
    }
    temp_f20 = 0.1f;
    func_0012B890(arg0, 0, 2, (s32) (*(s16 *)((char *)arg0 + 0x6C)), 0x14, temp_f20, 0);
    if (func_0012B890(arg0, 1, 2, (s32) (*(s16 *)((char *)arg0 + 0x6C)), 0x14, temp_f20, 0) != 0) {
        temp_20 = (*(u16 *)((char *)((*(void **)((char *)arg1 + 0xE0))) + 0x8C));
        if (temp_20 == 0x14) {
            func_0012F9C0(arg0, 4);
        }
        var_18 = 0;
        var_17 = D_0032FD90;
        do {
            if (temp_20 == (*(s32 *)((char *)var_17 + 0))) {
                func_001300B0(sp, arg1, (*(f32 *)((char *)var_17 + 4)), (*(f32 *)((char *)var_17 + 8)));
                func_001C9FE8(sp, (char *)arg1 + 0x90, sp);
                if (var_18 != 8) {
                    func_00136EC0(arg0, sp);
                } else if (func_001501E0(sp) != 0) {
                    func_0012FEB8(0x17, (s32) sp, (s32) ((char *)arg1 + 0xA0));
                    func_0012F9C0(arg0, 7);
                }
            }
            var_18 += 1;
            var_17 += 3;
        } while (var_18 < 9U);
    }
    if (func_0012BA30(arg1, 1, 2) != 0) {
        (*(s16 *)((char *)arg0 + 0x74)) += 1;
        if ((*(s16 *)((char *)arg0 + 0x74)) >= 0x5B) {
            func_001321E8(arg0);
        }
    }
}

extern void func_0012CB70(float *, char *, char *);
extern float func_00130758(char *);
extern double fabs(double);
extern s32 func_001372E8(char *, float, float);
extern void func_001360A0(void **, s8 *);
extern void func_00136058(void **, s8 *);
extern void func_001360E8(void **arg0, void *arg1);
extern f32 D_0035B9F0;

/* ER-198: by hand from the m2c draft; every test an early return (the game's bc1tl to the epilogue), the x coordinate
   compared with fabs() in double as func_001371C0 does (200.0 built by ori / dsll32), a local f32 sp[4] for the
   vector; prototype as the file's extern (s8 *, s32); names, types and offsets are Unconfirmed readings. */
void func_00136B18(s8 *arg0, s32 arg1) {
    f32 sp[4];
    s32 s;

    s = func_00131088(arg0);
    if (s != (s32)func_00135A50 && s != (s32)func_00135DC0 && s != (s32)func_001360E8 && s != (s32)func_00135AA8
        && s != (s32)func_00135AF0 && s != (s32)func_00135B88 && s != (s32)func_00135C50 && s != (s32)func_00135D08) {
        return;
    }
    func_0012CB70(sp, *(char **)arg0, (char *)arg1 + 0x50);
    func_001CA048(sp, sp, 1.0f / *(f32 *)(*(char **)arg0 + 0xB0));
    if (fabs(sp[0]) > 200.0) {
        return;
    }
    if (sp[2] > 800.0f) {
        return;
    }
    if (sp[2] <= 0.0f) {
        return;
    }
    if (sp[1] < -250.0f) {
        return;
    }
    if (func_00130758((char *)arg0) <= D_0035B9F0) {
        return;
    }
    if (sp[0] >= 0.0f) {
        if (func_001372E8((char *)arg0, -1.0f, 0.0f) != 0) {
            func_00130FE0((char *)arg0, func_001360A0);
        }
    } else {
        if (func_001372E8((char *)arg0, 1.0f, 0.0f) != 0) {
            func_00130FE0((char *)arg0, func_00136058);
        }
    }
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00136D10(void **arg0, s16 arg1) {
    f32 temp_f20;

    temp_f20 = 0.08f;
    func_0012B890(arg0, 0, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), arg1, temp_f20, 1);
    func_0012B890(arg0, 1, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), arg1, temp_f20, 1);
}

extern void func_00136E18(void **, s32, void *);
extern s32 D_0032FE10[];
extern s32 D_0032FE00[];

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00136D88(void **arg0) {
    void *temp_16;

    (void)&arg0; /* ruling 3.14 */
    temp_16 = *arg0;
    if (*(u8 *)((char *)temp_16 + 0x148) == 1) {
        if (*(u16 *)((char *)(*(void **)((char *)temp_16 + 0xE0)) + 0xDAC) == 0xA) {
            func_00136E18(arg0, 0x15, D_0032FE10);
        }
        if (*(u16 *)((char *)(*(void **)((char *)temp_16 + 0xE0)) + 0xBCC) == 0x28) {
            func_00136E18(arg0, 0x12, D_0032FE00);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136E18);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136EC0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136F50);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00137028);

extern float func_0012CE18(char *, char *);

/* ER-077 (C by ER-041). 0.17453293f is 10 degrees in radians; the fabs() compare is done in double, so its
 * constant is gcc's anonymous .rodata double in place of D_00354718 (ruling 3.10). Names and types are Unconfirmed. */
s32 func_001371C0(char **a0)
{
    float ang = func_0012CE18((char *)a0, *a0 + 0x90);

    if (fabs(ang) <= 0.17453293f && func_001372E8((char *)a0, 0.0f, -1.0f)) {
        func_00131000((char *)a0, func_00135FE0);
        return 1;
    }
    if (0.0f < ang && func_001372E8((char *)a0, -1.0f, 0.0f)) {
        func_00131000((char *)a0, func_001360A0);
        return 1;
    }
    if (func_001372E8((char *)a0, 1.0f, 0.0f)) {
        func_00131000((char *)a0, func_00136058);
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001372E8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001373E0);

extern void func_00130848(s8 **, f32);
extern s32 func_0012F8C8(void **arg0, s32 arg1, s32 arg2, s16 arg3, s32 arg4, f32 fparg0);
extern void func_001374C8(s8 *, void *);
extern void func_001375E8(s8 *arg0);
extern void func_00137658(void **);
extern void func_00137750(s8 *arg0);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00137428(s8 **arg0) {
    (void)&arg0; /* ruling 3.14 */
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        func_00130848(arg0, 0.0f);
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    func_0012F8C8((void **) arg0, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x3C, 1, 0.05f);
    func_00130CE0((s8 *) arg0, 50.0f);
    func_00131140((s8 *) arg0, func_001374C8, func_001375E8, func_00137658, func_00137750, NULL);
}

extern void func_001378D8(s8 *, f32 fparg0);
extern void func_00137428(s8 **arg0);

/* ER-189: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001374C8(s8 *arg0, void *arg1) {
    (void)&arg0; /* ruling 3.14 */
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 8) = 0;
        *(s16 *)((char *)arg0 + 0x6E) = (s16) ((u16) *(s16 *)((char *)arg0 + 0x6E) + 1);
    }
    func_001378D8(arg0, 210.0f);
    if (func_0012F8C8((void **) arg0, 9, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x14, 0, 0.1f) != 0) {
        if (*(u16 *)((char *)(*(void **)((char *)arg1 + 0xE0)) + 0x8C) == 0x23) {
            func_0012F9C0((void **) arg0, 2);
        }
        if (*(s8 *)((char *)arg0 + 8) == 0) {
            if (func_0012FC38((void **) arg0, 0, 0x1E, 0x55) != 0) {
                *(s8 *)((char *)arg0 + 8) = 1;
            }
        }
        if (func_0012F938(arg1, 9) != 0) {
            if (func_00131140(arg0, NULL, func_001375E8, func_00137658, func_00137750, func_00137428) == 0) {
                func_00131020(arg0, 0x3C, func_001375E8, func_001374C8);
            }
        }
    }
}


/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001375E8(s8 *arg0) {
    func_001378D8(arg0, *(f32 *)((char *)arg0 + 0x14C));
    func_0012F8C8((void **) arg0, 8, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x5A, 1, 0.08f);
    func_00131140(arg0, func_001374C8, NULL, func_00137658, func_00137750, func_00137428);
}

void func_001377C8(s8 *, void *);

/* ER-159: by hand from m2c; names, types and offsets are Unconfirmed readings. The call written in each arm keeps the
   x74 store and the x6E counter in separate blocks for the first scheduling pass (the arms are merged afterwards, the
   game's `jal; nop`); one call after the if / else lets the counter's load move above the store. */
void func_00137658(void **arg0) {
    char *a = (char *)arg0;

    if (*(s16 *)(a + 0x6E) == 0) {
        if (*(u16 *)(a + 4) == 0x3F) {
            *(s16 *)(a + 0x74) = func_0012E568(0xA, 0x1E);
        } else {
            *(s16 *)(a + 0x74) = func_0012E568(0x32, 0x50);
        }
        *(s16 *)(a + 0x6E) += 1;
    }
    func_0012F8C8(arg0, 0, *(s16 *)(a + 0x6C), 0x5A, 1, 0.08f);
    func_001378D8((s8 *)a, *(f32 *)(a + 0x14C) - 1.0f);
    if (func_00131140(a, func_001374C8, func_001375E8, 0, func_00137750, func_00137428) == 0 && *(s16 *)(a + 0x6C) >= *(s16 *)(a + 0x74)) {
        func_00131000(a, func_001377C8);
    }
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00137750(s8 *arg0) {
    func_001378D8(arg0, *(f32 *)((char *)arg0 + 0x148) - 1.0f);
    func_0012F8C8((void **) arg0, 1, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x5A, 1, 0.08f);
    func_00131140(arg0, func_001375E8, func_001375E8, func_00137658, NULL, func_00137428);
}

extern s32 func_0012C578(char *, s32, s32, s32);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001377C8(s8 *arg0, void *arg1) {
    func_001378D8(arg0, *(f32 *)((char *)arg0 + 0x148));
    if (func_0012F8C8((void **) arg0, 0xA, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x14, 0, 0.1f) != 0) {
        if (*(u16 *)((char *)(*(void **)((char *)arg1 + 0xE0)) + 0x8C) == 0x12C) {
            func_0012C578(arg0, 0x81, 1, 0x1E);
        }
        if (func_0012F938(arg1, 0xA) != 0) {
            func_00130FE0(arg0, func_00137428);
        }
    }
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00137878(s8 *arg0) {
    *(s32 *)((char *)arg0 + 0x48) = 0;
    *(f32 *)((char *)arg0 + 0xB4) = (f32) 0.9f;
    if (func_0014FF38(arg0, 3) != 0) {
        *(f32 *)((char *)arg0 + 0xB4) = (f32) 0.2f;
        func_00131000(arg0, func_00137428);
    }
}

extern void func_0012B238(void);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed. */
void func_001378D0(void) {
    func_0012B238();
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001378D8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001379A0);

extern s32 func_00138800(void **arg0);
extern void func_00138860(s8 *arg0, s32 arg1);
extern s32 func_0012FB90(void *arg0, s32 *arg1, f32 *arg2);
extern float func_00130378(char *, s32);
extern float func_0012E7B0(float a, float b);
extern void func_00137CE0(s8 *, s8 *);
extern void func_001381D0();
extern void func_00138370();

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00137A10);

INCLUDE_RODATA("asm/nonmatchings/text_001343E8", D_00354720);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00137CE0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001381D0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00138370);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00138540);

extern s32 func_001387C0(void *, f32 fparg0);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001385E8(void *arg0) {
    func_001387C0(arg0, 0.0f);
}

extern void func_00137A10(void **, void *);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001385F8(s8 *arg0) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 0xF) = 0;
        *(f32 *)((char *)arg0 + 0xB4) = 1.0f;
        func_001387C0(arg0, 0.0f);
    }
    if (func_0014FF38(arg0, 3) != 0) {
        *(s8 *)((char *)arg0 + 0xF) = 1;
        *(f32 *)((char *)arg0 + 0xB4) = (f32) 0.6f;
        func_00131000(arg0, func_00137A10);
        func_00138860(arg0, func_00130780(arg0));
    }
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00138690(void *arg0) {
    *(s8 *)((char *)arg0 + 0xF) = 1;
}

extern s32 func_0014FE10(void *arg0, s32 arg1);

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001386A0(void *arg0, s32 arg1) {
    if (*(s16 *)((char *)arg0 + 0x6E) == 0) {
        *(s8 *)((char *)arg0 + 0xF) = 0;
        func_001387C0(arg0, 0.0f);
    }
    func_0014FE10(arg0, arg1);
}

extern void func_001386E8(void *, s8 *);
extern void func_00113C30(char *a0, s32 a1, void *a2);
extern void func_001C9FE8(void *, void *, void *);

/* ER-189: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001386E8(void *arg0, s8 *arg1) {
    s32 sp[8];

    func_00113C30(arg1, 0x1D, sp);
    func_00113C30(arg1, 0x1A, &sp[4]);
    func_001C9FE8(arg0, sp, &sp[4]);
    *(f32 *)arg0 = (f32) (*(f32 *)arg0 * 0.5f);
    *(f32 *)((char *)arg0 + 4) = (f32) (*(f32 *)((char *)arg0 + 4) * 0.5f);
    *(f32 *)((char *)arg0 + 8) = (f32) (*(f32 *)((char *)arg0 + 8) * 0.5f);
}


/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00138780(void *arg0) {
    s32 temp_2;

    temp_2 = *(s32 *)((char *)arg0 + 0x48);
    if (temp_2 != 0) {
        func_001386E8(temp_2 + 0x50, *(s8 **)arg0);
    }
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
f32 func_001387A0(void *arg0) {
    f32 var_f0;
    void *temp_2;

    temp_2 = *(void **)((char *)arg0 + 0x48);
    var_f0 = 1.0f;
    if (temp_2 != NULL) {
        var_f0 = *(f32 *)((char *)temp_2 + 0x4C);
    }
    return var_f0;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain), fixed by hand; names, types and offsets are Unconfirmed readings. */
s32 func_001387C0(void *arg0, f32 fparg0) {
    void *temp_3;
    f32 t;

    temp_3 = *(void **)((char *)arg0 + 0x48);
    if (temp_3 != NULL) {
        t = *(f32 *)((char *)(*(void **)arg0) + 0xA4);
        *(f32 *)((char *)temp_3 + 0x4C) = fparg0;
        *(f32 *)((char *)temp_3 + 0x64) = t;
        if (fparg0 >= 1.0f) {
            *(void **)((char *)arg0 + 0x48) = NULL;
        }
        return 1;
    }
    return 0;
}

/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00138800(void **arg0) {
    s32 temp_2;
    void *temp_3;

    temp_2 = func_0012F8C8(arg0, (*(s8 *)((char *)arg0 + 0xE) != 0) ? 8 : 9, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x3C, 0, 0.05f);
    if (temp_2 != 0) {
        temp_3 = *(void **)arg0;
        *(u8 *)((char *)temp_3 + 0x147) = (u8) (*(u8 *)((char *)temp_3 + 0x147) & 0xFE);
    }
    return temp_2;
}


/* ER-154: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00138860(s8 *arg0, s32 arg1) {
    f32 temp_f0;

    temp_f0 = func_00130378(*(s32 *)arg0 + 0x90, *(s32 *)((char *)arg0 + 0x130)) + 6.2831855f;
    *(s32 *)((char *)arg0 + 0x28) = arg1;
    *(f32 *)((char *)arg0 + 0x4C) = (f32) ((temp_f0 - ((f32) (s32) (temp_f0 / 6.2831855f) * 6.2831855f)) - 3.1415927f);
    func_00131000(arg0, func_00137CE0);
}

INCLUDE_RODATA("asm/nonmatchings/text_001343E8", D_00354748);
