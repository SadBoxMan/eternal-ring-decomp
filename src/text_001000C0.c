#include "common.h"

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001000C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00100C30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00100E00);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00100EC0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00100ED0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00100EE0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00100F70);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101160);

/* ER-014. Clears n words; n is unsigned (the -1 end test is `li 0xffffffff`). */
void func_00101200(int *p, u32 n) {
    while (n-- != 0) {
        *p++ = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101238);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101270);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001012C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101370);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001014A8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001014F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001016B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001016B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101740);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001017B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101898);

void func_00101A50(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101A58);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101C00);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101CC0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101DC8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101ED0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101F20);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103160);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001032D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001033F0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103470);

void func_00103510(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103518);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103598);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001036D0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001037F8);

/* ER-014. Two float quads (0, 0, 0, 1) at word offsets 0x24 and 0x28; types Unconfirmed. */
void func_00103848(float *a0) {
    a0[0x24] = 0;
    a0[0x25] = 0;
    a0[0x26] = 0;
    a0[0x27] = 1.0f;
    a0[0x28] = 0;
    a0[0x29] = 0;
    a0[0x2A] = 0;
    a0[0x2B] = 1.0f;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103878);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001038F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103A28);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103B50);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103BA0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103C20);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103C70);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103CD8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103DB8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103E30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103E50);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103EC8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103F68);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001040C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00104300);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00104378);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00104478);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001044D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00104548);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001045E8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00104D48);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00106230);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00106A38);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00107A60);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00107BE0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00107BF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00107E48);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00107F50);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001082A0);

void func_00108508(void) {
}

void func_00108510(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00108518);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00108798);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00108A18);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00108B40);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00108E00);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00109090);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001091B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00109398);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00109610);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00109738);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00109B88);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00109FD8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010A530);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010AAA0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010B188);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010C090);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010C4C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010C630);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010C6C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010C790);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010C8C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010C970);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010CA48);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010CAF0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010CBF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010CD98);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010CE20);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010D340);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010D468);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010D5E8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010D770);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010D8F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010DB58);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010DC60);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010DD88);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010DFD0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010E028);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010E078);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010E128);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010E248);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010E350);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010E3C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010E468);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010E550);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010E600);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010E690);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010EC20);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010F1A0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010F488);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010F768);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010F898);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010F9C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010FB18);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010FE20);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010FEB0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010FF30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00110198);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00110418);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00110588);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00110678);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00110748);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001107F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001108B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00110948);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001109C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00110A50);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00110B00);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00110BB0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00110C00);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00110CB0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111348);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111480);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001114E8);

extern void func_001CA018(void *, void *, void *);
extern float func_001E41B8(float, float);

/* ER-014. Types Unconfirmed; the return type (float) is a reading: the code is the same if nothing is returned. */
float func_00111548(float *a0, float *a1) {
    float v[4];

    func_001CA018(v, a0, a1);
    return func_001E41B8(v[0], v[2]);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111578);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001115E0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111620);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111660);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001116A8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001116D0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001116E0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111700);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111740);

void func_00111778(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111780);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001119B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111AB0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111B58);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111C10);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111CB0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111D28);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00111F50);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00112160);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001122E8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001124E8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00112638);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001126A0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001129B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00112CC8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00112D30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00112E18);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00113110);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001131C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00113238);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00113288);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001132B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00113460);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00113688);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00113920);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001139C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00113AC8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00113C30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00113ED8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001141D0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00114450);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001144B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001145F8);

void func_00114C90(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00114C98);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00114E30);

extern int D_0035B810;
extern float D_001F0380[];

/* ER-014. D_001F0380: rows of 0x40 floats (Unconfirmed). */
void func_00115000(char *a0) {
    *(float *)(a0 + 0xCC) += D_001F0380[(D_0035B810 & 0x3F) * 0x40] * 4.0f;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115038);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001150C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001152D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115408);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001155C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115670);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115700);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115800);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115968);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115AC8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115B78);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115C08);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115C78);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115CE8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115E48);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00115FE0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00116140);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00116298);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001163F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00116450);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00117418);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00117630);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00117F88);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00117FC0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00118FF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00119318);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00119458);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001194B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00119510);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00119568);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00119670);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001197B8);

void func_001198B0(void) {
}

void func_001198B8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001198C0);

void func_001198E0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001198E8);

void func_00119C00(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00119C08);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00119D10);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00119DF0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00119F30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00119FE0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011B9D0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011BAB0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011BB90);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011C3A8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011C518);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011C598);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011C620);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011C7A0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011C858);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011CB58);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011CE58);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011CEF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011CF80);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011D9E0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011E488);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0011F818);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00120B30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00120BE0);

void func_00120C78(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00120C80);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00120D08);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00120DA0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00120EF0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00121438);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00121720);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001220A0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00122580);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00122988);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00122B68);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00122FD8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00123708);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001239B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00123BA8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00123C28);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00123D70);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00123DF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00124370);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001245A8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00124BE8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125330);

void func_001254E8(void) {
}

void func_001254F0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001254F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125790);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001257E8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125818);

extern char *D_0035B8A0;

/* ER-014. 33 records of 0x180 bytes from offset 0xA80 (Unconfirmed). */
void func_00125A00(void) {
    int i;
    char *p = D_0035B8A0 + 0xA80;

    for (i = 0x20; i >= 0; i--) {
        *(u16 *)(p + 2) |= 1;
        p += 0x180;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125A38);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125AF0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125B68);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125C10);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125C28);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125C60);

extern char *D_0035B8A8;

/* ER-014. Types and offsets are Unconfirmed readings. */
void func_00125CC0(void) {
    int i;
    char *p = D_0035B8A8;

    for (i = 0; i < *(u16 *)(p + 4); i++) {
        (*(char **)(p + 0xE0))[i * 0xA0 + 0x79] = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125D00);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125EA0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125F18);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126190);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126250);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126338);

void func_00126510(void) {
}

void func_00126518(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126520);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126728);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126828);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001268F0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126968);

void func_001269A0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001269A8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126A78);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126CA0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126E40);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126EB0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00126F00);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001270E0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00127180);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00127198);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001275B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00127658);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001276B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00127840);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00127868);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00127E28);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00128478);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00128928);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00128940);

void func_00128A08(void) {
}

void func_00128A10(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00128A18);

void func_00128EB0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00128EB8);

void func_001298F8(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00129900);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00129C70);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00129ED8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00129F18);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00129F78);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012A140);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012A1A0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012A320);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012A450);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012A658);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012A8C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012AA70);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B020);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B058);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B090);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B0D0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B180);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B1B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B1F0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B208);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B218);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B238);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B240);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B540);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B5F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B6B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B7C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B890);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B930);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012BA30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012BAE0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012BCE0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012BDA8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012BEC8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012BF08);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C020);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C098);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C128);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C190);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C208);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C2E8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C360);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C3E8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C500);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C578);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C630);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C718);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C778);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C788);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C7B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C8D0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C938);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C9D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CA68);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CAD8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CAF0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CB08);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CB70);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CBF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CC68);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CD08);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CE18);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CE88);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CF90);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D138);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D2D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D318);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D388);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D3B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D3E8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D470);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D568);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D600);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D7C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D8B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D9B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DA18);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DA68);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DAF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DB88);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DC10);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DDA0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DF30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DF90);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E198);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E320);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E3B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E3C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E428);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E488);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E568);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E5C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E640);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E6B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E750);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E7B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E820);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E908);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012EA28);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012EA78);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012EAB8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012EAF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012EB60);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012EBB0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012EEB0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F158);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F160);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F1D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F2F0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F418);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F540);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F550);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F5F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F688);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F740);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F7E0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F808);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F838);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F870);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F8A0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F8C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F900);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F938);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F958);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F978);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F990);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F9C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FA18);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FA68);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FAA8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FAE8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FB48);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FB90);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FBE8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FC38);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FCA0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FD10);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FD70);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FE10);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FE48);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FEB8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FEE0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FF38);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FF78);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FFD8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130040);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001300B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130120);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130150);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130180);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001301F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130258);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001302C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130378);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001303A8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130420);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001304C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130590);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130628);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001306C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001306E8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130758);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130780);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001307A0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001307E0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130848);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001308B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130930);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001309C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130A88);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130AD0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130B18);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130B60);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130BF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130C80);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130CE0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130D00);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130D68);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130DF0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130F50);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130FE0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131000);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131020);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131088);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131090);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131140);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131208);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001312D0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001313A8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001313F0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131418);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131430);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131448);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131470);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131498);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001314C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131508);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131548);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001315A8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131608);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131678);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131688);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001316A8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001318A0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001318D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131930);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131D40);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132020);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132060);

void func_001320A8(void) {
}

void func_001320B0(void) {
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001320B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132108);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001321D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001321E8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132200);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132268);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132278);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132340);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132348);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132480);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001325E8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132890);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001328D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132938);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001329F0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132A08);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132CA0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132CC0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132CF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132E80);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132F80);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133098);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133290);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133458);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133648);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001336D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001336E0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133750);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133868);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001339F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133A60);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133F28);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133FC0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00134070);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00134088);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00134090);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00134160);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00134298);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001342C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00134310);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00134358);
