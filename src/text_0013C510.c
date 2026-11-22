#include "common.h"

/* ER-024: this file's .sdata (docs/build.md 3.2) */
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAC8);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BACC);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAD0);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAD4);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAD8);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BADC);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAE0);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAE4);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAF0);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAF4);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAF8);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BAFC);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BB00);
INCLUDE_SDATA("asm/nonmatchings/text_0013C510", D_0035BB04);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013C510);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013C658);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013C6D8);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013C750);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013C9A8);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CA60);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CAE8);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CB60);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CC20);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CC68);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CCE8);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CD68);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CDD0);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CE28);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013CF78);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D068);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D088);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D378);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D440);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D480);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D488);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D498);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D4A8);

extern s32 func_0013D498(void *);
extern s32 func_0012B540(void *, s32, s8, s32);

/* ER-031 / ER-051: first C jump table in the files cut from S1's (docs/game_seams.md 9.10). Picks 10 for state 9,
   9 for states 8 and 10, else 8, from func_0013D498, and passes it to func_0012B540. The explicit cases 0 and 12
   give the table's 0..12 range (gcc indexes from the lowest case). Names and types Unconfirmed. */
s32 func_0013D4D0(void **a0) {
    s32 v;

    switch (func_0013D498(a0)) {
    case 9:
        v = 10;
        break;
    case 8:
    case 10:
        v = 9;
        break;
    case 0:
    case 12:
    default:
        v = 8;
        break;
    }
    func_0012B540(*a0, 1, v, 0);
    return v;
}

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D550);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D5D0);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D688);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D7A8);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D7D8);

extern void func_0014FE10(void);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed. */
void func_0013D820(void) {
    func_0014FE10();
}

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D828);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D838);

INCLUDE_ASM("asm/nonmatchings/text_0013C510", func_0013D9D0);
