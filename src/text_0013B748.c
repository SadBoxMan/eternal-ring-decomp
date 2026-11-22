#include "common.h"

/* ER-024: this file's .sdata (docs/build.md 3.2) */
INCLUDE_SDATA("asm/nonmatchings/text_0013B748", D_0035BAB0);
INCLUDE_SDATA("asm/nonmatchings/text_0013B748", D_0035BAB4);
INCLUDE_SDATA("asm/nonmatchings/text_0013B748", D_0035BAB8);
INCLUDE_SDATA("asm/nonmatchings/text_0013B748", D_0035BABC);
INCLUDE_SDATA("asm/nonmatchings/text_0013B748", D_0035BAC0);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013B748);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013B7E0);

extern void func_0013C3E8(char *);
extern void func_00131000(char *, void *);
extern float func_00130D00(char *, float);
extern void func_00130CE0(char *, float);
extern int func_00131140(char *, void *, void *, void *, void *, void *);
extern int func_0012E568(int, int);
extern float func_00130758(char *);
extern int func_00130780(char *);
extern int func_00131020(char *, int, void *, void *);
extern void func_0013BBC8(void);
extern void func_0013BA50(void);
extern void func_0013B7E0(void);
extern void func_0013BD90(void);
extern void func_0013C258(void);

/* ER-035. A state handler (a reading): leaves through func_00131140 when func_00130D00 gives at most 70 degrees;
   on its first frame clears the counters at +0x14 / +0x18 and draws a frame count into +0x1C (func_0012E568);
   then counts whether func_00130758's value stays (+0x10), falls (+0x14) or rises (+0x18), and when the count runs
   out picks the next state from the counters through func_00131000 / func_00131020. Names and types Unconfirmed. */
void func_0013B840(char *a0) {
    float f;

    func_0013C3E8(a0);
    if (func_00130D00(a0, 15.0f) <= 1.22173059f) {
        if (func_00131140(a0, func_0013BBC8, func_0013BA50, NULL, NULL, func_0013B7E0)) return;
    }
    func_00130CE0(a0, 20.0f);
    if (*(s16 *)(a0 + 0x6E) == 0) {
        *(int *)(a0 + 0x14) = 0;
        *(int *)(a0 + 0x18) = 0;
        *(int *)(a0 + 0x1C) = func_0012E568(1, 0x14);
        *(float *)(a0 + 0x30) = func_00130758(a0);
        *(s16 *)(a0 + 0x6E) += 1;
        return;
    }
    f = func_00130758(a0);
    if (f == *(float *)(a0 + 0x30)) {
        *(int *)(a0 + 0x10) += 1;
    } else if (f < *(float *)(a0 + 0x30)) {
        *(int *)(a0 + 0x14) += 1;
    } else {
        *(int *)(a0 + 0x18) += 1;
    }
    *(float *)(a0 + 0x30) = f;
    if (--*(int *)(a0 + 0x1C) > 0) return;
    if (func_00130780(a0) < 3) {
        if (*(int *)(a0 + 0x14) > *(int *)(a0 + 0x18)) {
            func_00131000(a0, func_0013B840);
        } else {
            func_00131020(a0, 0x46, func_0013BA50, NULL);
            func_00131020(a0, 0x1E, func_0013BD90, NULL);
        }
    } else if (*(int *)(a0 + 0x14) > *(int *)(a0 + 0x18)) {
        func_00131000(a0, func_0013BD90);
    } else if (*(int *)(a0 + 0x10) < *(int *)(a0 + 0x18)) {
        if (!func_00131020(a0, 10, func_0013BD90, NULL)) {
            func_00131020(a0, 2, func_0013C258, NULL);
        }
    } else {
        func_00131020(a0, 10, func_0013BD90, NULL);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013BA50);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013BBC8);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013BD28);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013BD90);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013C258);

extern void func_0012B238(void);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed. */
void func_0013C318(void) {
    func_0012B238();
}

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013C320);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013C390);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013C3E8);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013C438);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013C450);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013C468);

INCLUDE_ASM("asm/nonmatchings/text_0013B748", func_0013C4E0);

INCLUDE_RODATA("asm/nonmatchings/text_0013B748", D_00354848);
