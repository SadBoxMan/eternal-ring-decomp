#include "common.h"

/* ER-024: this file's .sdata (docs/build.md 3.2) */
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE3C);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE40);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE44);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE48);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE4C);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE50);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE54);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE58);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE60);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE68);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE6C);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE70);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE74);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE78);
INCLUDE_SDATA("asm/nonmatchings/text_0014C840", D_0035BE7C);

/* ER-024: this file's .data (docs/build.md 3.2) */
INCLUDE_DATA("asm/nonmatchings/text_0014C840", D_0032FFB0);
INCLUDE_DATA("asm/nonmatchings/text_0014C840", D_0032FFC0);
INCLUDE_DATA("asm/nonmatchings/text_0014C840", D_0032FFD0);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014C840);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014C968);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014CA60);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014CC10);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014CC68);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014CCD0);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014CD88);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014CF60);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D028);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D050);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D068);

extern void func_0012B218(void *, int);
extern void func_0014D138(void *, void *);
extern void func_0014D1E0(void *, float);
extern void func_0014D8D0(void *, int);

/* ER-015 (drafted with m2c, compared with tools/cmpfunc.py, placed with tools/place.py). Field meanings,
 * argument types and the callback's arguments are Unconfirmed readings. */
void func_0014D098(char *a0, void *a1) {
    *(int *)(a0 + 0x28) = *(int *)(a0 + 0x28) + 1;
    (*(void (**)(void *))(a0 + 0x44))(a0);
    func_0012B218(a0, 2);
    if (*(s8 *)(a0 + 0xF) != 0) {
        func_0014D8D0(a1, 1);
        func_0014D138(a0, a1);
    } else {
        func_0014D8D0(a1, 0);
        func_0012B218(a0, 4);
    }
    func_0012B218(a0, 1);
    func_0014D1E0(a1, *(float *)(a0 + 0x4C));
}

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D138);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D1E0);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D338);

void func_0014D378(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D380);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D3D0);

void func_0014D3F0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D3F8);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D510);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D5F0);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D650);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D6B0);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D7A0);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D890);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D8D0);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D8E8);

INCLUDE_ASM("asm/nonmatchings/text_0014C840", func_0014D8F8);

/* ER-014. Types and offsets are Unconfirmed readings. */
float func_0014D918(char *a0) {
    return *(float *)(*(char **)(a0 + 0x130) + 4) - *(float *)(*(char **)a0 + 0x94);
}
