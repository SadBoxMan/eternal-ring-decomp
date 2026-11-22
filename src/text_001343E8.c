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

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134618);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134690);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001346D0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134718);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134730);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134790);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001348B8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134958);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134B60);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134BB0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134DB0);

INCLUDE_RODATA("asm/nonmatchings/text_001343E8", D_003546D8);
INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00134F70);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135158);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001352B8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135378);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135540);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135570);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001355B8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135610);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001356B0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135740);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135810);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135900);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135958);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001359C0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135A50);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135AA8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135AF0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135B88);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135C50);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135D08);

INCLUDE_RODATA("asm/nonmatchings/text_001343E8", D_003546F8);
INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135DC0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00135FE0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136058);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001360A0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001360E8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001361B0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001363C0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001365A0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136760);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001368A0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001368D8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136920);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136968);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136B18);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136D10);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136D88);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136E18);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136EC0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00136F50);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00137028);

extern double fabs(double);
extern float func_0012CE18(char *, char *);
extern s32 func_001372E8(char *, float, float);
extern void func_00131000(char *, void *);
extern void func_00135FE0(void);
extern void func_001360A0(void);
extern void func_00136058(void);

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

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00137428);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001374C8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001375E8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00137658);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00137750);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001377C8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00137878);

extern void func_0012B238(void);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed. */
void func_001378D0(void) {
    func_0012B238();
}

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001378D8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001379A0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00137A10);

INCLUDE_RODATA("asm/nonmatchings/text_001343E8", D_00354720);
INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00137CE0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001381D0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00138370);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00138540);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001385E8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001385F8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00138690);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001386A0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001386E8);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00138780);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001387A0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_001387C0);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00138800);

INCLUDE_ASM("asm/nonmatchings/text_001343E8", func_00138860);

INCLUDE_RODATA("asm/nonmatchings/text_001343E8", D_00354748);
