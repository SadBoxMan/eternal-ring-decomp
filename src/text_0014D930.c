#include "common.h"

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014D930);

void func_0014DA78(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DA80);

void func_0014DAB8(void) {
}

void func_0014DAC0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DAC8);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DC98);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DDD8);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DEA0);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DEB8);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DEC8);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DED8);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DEE8);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014DF08);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014E0A8);

void func_0014E118(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014E120);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014E150);

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014E160);

void func_0014E248(void) {
}

void func_0014E250(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_0014D930", func_0014E258);

/* ER-014. Types and offsets are Unconfirmed readings. */
void func_0014E2C8(char *a0) {
    char *p = *(char **)(a0 + 0x14);

    if (p != NULL) {
        *(int *)(a0 + 0x14) = 0;
        *(int *)(p + 0x10) = 1;
    }
}
