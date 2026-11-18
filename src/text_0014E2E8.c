#include "common.h"

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014E2E8);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014E3E8);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014E450);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014E608);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014E740);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014E7A8);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014E960);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014EA70);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014EAD0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014EC58);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014EDF8);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014EE68);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014EEF0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F160);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F1D8);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F210);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F240);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F298);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F2E0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F328);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F480);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F490);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F550);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F5F8);

INCLUDE_RODATA("asm/nonmatchings/text_0014E2E8", D_00354A68);
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

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F680);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F690);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F728);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014F7F8);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FA18);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FBA0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FBD0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FC68);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FDF8);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FE00);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FE08);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FE10);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FE30);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FF38);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FF58);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FF78);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_0014FF98);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_00150070);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_001500C8);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_001501D8);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_001501E0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_00150220);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_00150240);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_00150280);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_001502C0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_00150350);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_001503C0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_001503E0);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_00150400);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_001505F8);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_00150658);

INCLUDE_ASM("asm/nonmatchings/text_0014E2E8", func_001507B8);

extern float D_0035BF20;
extern void func_0012FA18(void *, float);
extern void func_0012FB48(void *, int);
extern void func_0012FAE8(void *, int, int, int, float);

/* ER-014. Arguments Unconfirmed ($a0-$a3 pass through unchanged). */
void func_00150810(void *a0) {
    func_0012FA18(a0, D_0035BF20);
}

/* ER-014. Types and offsets are Unconfirmed readings. */
void func_00150818(int **a0, int a1, int a2) {
    func_0012FB48(a0, 0);
    func_0012FAE8(a0, a1, a2, *(int *)((char *)*a0 + 0xE0) + 0x5D0, D_0035BF20);
}
