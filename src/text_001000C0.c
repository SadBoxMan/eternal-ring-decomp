#include "common.h"

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001000C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00100C30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00100E00);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00100EC0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00100ED0);

extern void func_00101370(void);
extern s32 func_001C1580(s32);
extern s32 func_001C1630(s32, s32);
extern s32 D_001F6B90[];
extern s32 D_001F6C00[];
extern s32 func_001E2D00();
extern s32 func_001D5B68();
extern s32 func_001D5218();
extern s32 func_001D5E98();
extern s32 func_001D56C8();
extern s32 func_001D4740(s32);
extern s32 func_001CE280();
extern s32 func_001CE130(s32, s32, s32, s32);
extern s32 func_001E2788();
extern s32 func_001D3278();

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00100EE0(void) {
    func_00101370();
    func_001C1580((s32) "\\ERDATA.BIN;1");
    func_001C1630(D_001F6B90[0], (s32) D_001F6C00);
    func_001E2D00();
    func_001D5B68();
    func_001D5218();
    func_001D5E98();
    func_001D56C8();
    func_001D4740(1);
    func_001CE280();
    func_001CE130(0, 1, 2, 1);
    func_001E2788();
    func_001D3278();
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00100F70);

void func_00101200(int *p, u32 n);
extern char D_0035C700[];
extern s32 D_0035B860;
extern s32 D_0035B85C;
extern s32 D_0035B84C;
extern s32 D_0035B854;
extern s32 D_0035B858;
extern void func_00101270(void);
extern void *D_0035B878;
extern s32 D_0035B884;
extern s32 D_0185C700[];
extern s32 *D_0035B888;
extern s32 *D_0035B88C;
extern s32 D_018BC700[];
extern s32 D_0021E880[];
extern s32 D_002488C0[];

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00101160(void) {
    func_00101200(D_0035C700, 0x540000U);
    D_0035B860 = 0x30000;
    D_0035B85C = 0x200000;
    D_0035B84C = 0x01000000;
    D_0035B854 = 0x80000;
    D_0035B858 = 0x280000;
    func_00101270();
    D_0035B878 = D_0035B85C + D_0035C700;
    D_0035B884 = (s32) D_0185C700;
    D_0035B888 = D_0185C700;
    D_0035B88C = D_018BC700;
    func_00101200(D_0021E880, 0x4800U);
    func_00101200(D_002488C0, 0x6000U);
}

/* ER-014. Clears n words; n is unsigned (the -1 end test is `li 0xffffffff`). */
void func_00101200(int *p, u32 n) {
    while (n-- != 0) {
        *p++ = 0;
    }
}

/* ER-081: by hand from the m2c draft (word copy); names, types and offsets are Unconfirmed readings. */
void func_00101238(s32 *dst, s32 *src, u32 n) {
    while (n-- != 0) {
        *dst++ = *src++;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00101270);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001012C8);

extern s32 func_001D1BC0(s32);
extern void func_001C1560(void);
extern s32 func_001DA350(s32);
extern s32 func_001D3C38(s32 *);
extern s32 func_001D3BF0();
extern s32 func_001D2BA0();
extern s32 func_001D3770(s32 *, s32, s32 *);
extern s32 D_0035B900[];
extern s32 func_001E9C10(s32 *);
extern s32 D_0035B908[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00101370(void) {
    func_001D1BC0(0);
    func_001C1560();
    func_001DA350(2);
    do {

    } while (func_001D3C38("cdrom0:\\IOPRP165.IMG;1") == 0);
    do {

    } while (func_001D3BF0() == 0);
    func_001D1BC0(0);
    func_001C1560();
    func_001DA350(2);
    func_001D2BA0();
    func_001D3770("cdrom0:\\IOP\\SIO2MAN.IRX;1", 0, D_0035B900);
    func_001D3770("cdrom0:\\IOP\\PADMAN.IRX;1", 0, D_0035B900);
    func_001D3770("cdrom0:\\IOP\\MCMAN.IRX;1", 0, D_0035B900);
    func_001D3770("cdrom0:\\IOP\\MCSERV.IRX;1", 0, D_0035B900);
    func_001D3770("cdrom0:\\IOP\\LIBSD.IRX;1", 0, D_0035B900);
    func_001D3770("cdrom0:\\IOP\\MODMIDI.IRX;1", 0, D_0035B900);
    func_001D3770("cdrom0:\\IOP\\MODHSYN.IRX;1", 0, D_0035B900);
    func_001D3770("cdrom0:\\IOP\\SDRDRV.IRX;1", 0, D_0035B900);
    func_001D3770("cdrom0:\\IOP\\FSIOPSND.IRX;1", 0, D_0035B900);
    func_001E9C10(D_0035B908);
}

extern s32 func_001CAA58(s32);
extern s32 func_001CAAC0(s32, s32);
extern s32 D_0035B818;
extern s32 D_001F6590[];
extern s32 D_0035B828;
extern s32 D_0035B82C;
extern s32 D_0035B830;
extern s32 D_0035B834;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001014A8(void) {
    func_001CAA58(0);
    func_001CAAC0(0, 0);
    D_0035B818 = 1;
    func_00101200(D_001F6590, 9U);
    D_0035B828 = 0;
    D_0035B82C = 0;
    D_0035B830 = 0;
    D_0035B834 = 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001014F8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001016B0(void) {
    D_0035B818 = 0;
}

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

INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003540D0);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003540E0);
INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103C70);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103CD8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103DB8);

/* ER-081: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void* func_00103E30(s32 *arg0, void *arg1) {
    s32 n;
    char *p;

    n = *arg0;
    p = (char *)arg0 + 0x10;
    *(void **)((char *)arg1 + 0x104) = p;
    *(s32 *)((char *)arg1 + 0x100) = n;
    return p + n * 0x10;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103E50);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103EC8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00103F68);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001040C8);

extern s32 D_0035B938;
extern s32 func_001CEBE8(s32);
extern s32 D_0035B890;
extern s32 D_00202C50[];
extern s32 D_00202B60[];
extern s32 func_001CFAD8(s32 *, s32, s32, s16);
extern s32 func_001D03E0(s32);
extern s32 func_001CEB88(s32 *, s32);
extern s32 D_00202B00[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00104300(void) {
    s32 *var_4;

    D_0035B938 = func_001CEBE8(0) == 0;
    if (D_0035B890 != 0) {
        var_4 = D_00202C50;
    } else {
        var_4 = D_00202B60;
    }
    func_001CFAD8(var_4, 0x800, 0x800, (s16) D_0035B938);
    func_001D03E0(0);
    func_001CEB88(D_00202B00, D_0035B890);
    D_0035B890 = D_0035B890 == 0;
}

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

extern void func_001D7778(float *, float *, float);
extern void func_001D76D0(float *, float *, float);
extern void func_001D7820(float *, float *, float);
extern void func_001D7588(float *, float *, void *);
extern float D_001F5780[];

/* ER-081: by hand from the m2c draft (the first argument is not used); names, types and offsets are Unconfirmed readings. */
void func_0010CAF0(void *arg0, char *arg1, float *arg2) {
    float pi;
    float pi2;
    float a;

    pi = 3.1415927f;
    pi2 = 6.2831855f;
    a = *(float *)(arg1 + 0x50);
    a += pi;
    a -= (float)(s32)(a / pi2) * pi2;
    a -= pi;
    func_001D7778(arg2, D_001F5780, a);
    a = *(float *)(arg1 + 0x58);
    a += pi;
    a -= (float)(s32)(a / pi2) * pi2;
    a -= pi;
    func_001D76D0(arg2, arg2, a);
    a = *(float *)(arg1 + 0x54);
    a += pi;
    a -= (float)(s32)(a / pi2) * pi2;
    a -= pi;
    func_001D7820(arg2, arg2, a);
    func_001D7588(arg2, arg2, arg1 + 0x40);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0010CBF8);

extern s32 func_001D75C8(s32, s32 *);
extern s32 D_001F5880[];
extern s32 func_001D74E0(s32, s32, f32);
extern s32 D_001F58C0[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0010CD98(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_16;
    s32 temp_4;

    func_001D75C8(arg1, D_001F5880);
    func_001D74E0(arg1, arg1, *(f32 *)((char *)arg0 + 0xB0));
    temp_4 = arg1 + 0x10;
    func_001D74E0(temp_4, temp_4, *(f32 *)((char *)arg0 + 0xB4));
    temp_16 = arg1 + 0x20;
    func_001D74E0(temp_16, temp_16, *(f32 *)((char *)arg0 + 0xB8));
    func_001D75C8(arg2, D_001F58C0);
}

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

extern float D_0035B94C[];

/* ER-026 (parked by ER-014). Steps *a0 by 0.01 toward a1. Names and types Unconfirmed; D_0035B94C (-0.0 in
   .sdata) is read through %hi / %lo, so it is declared without a size (a sized float would be $gp-relative). */
void func_001115E0(float *a0, float a1) {
    float f = *a0;

    if (f - a1 < D_0035B94C[0]) {
        *a0 = f + 0.01f;
    } else {
        *a0 = f - 0.01f;
    }
}

extern s32 func_001C9B38(s32 *);
extern s32 D_001FF200[];
extern s32 func_001C9B88(s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00111620(s32 arg0, s32 arg1) {
    func_001C9B38(D_001FF200);
    func_001C9B88(arg1, arg0);
}

/* ER-081: by hand from the m2c draft (index of the largest of seven s16); names, types and offsets are Unconfirmed readings. */
s32 func_00111660(void *arg0) {
    s16 *t;
    s16 *p;
    s16 max;
    s32 i;
    s32 best;

    t = *(s16 **)((char *)arg0 + 0x70);
    best = 0;
    i = 1;
    max = t[0];
    p = t + 1;
    do {
        if (max < *p) {
            max = *p;
            best = i;
        }
        i++;
        p++;
    } while (i < 7);
    return best;
}

extern s32 D_0035B89C;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001116A8(void) {
    D_0035B89C += 1;
    *(s32 *)0x10000010 |= 0x400;
}

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

INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354140);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354150);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354160);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354170);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354180);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354190);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003541A0);
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

/* Unofficial layout: only tab (0x80) and sel (0x101) are read here. */
typedef struct {
    char pad0[0x80];
    char *tab[32];
    u8 x100;
    u8 sel;
} Anm;

extern s32 func_00114450(void *, s32);
extern void func_001C9F60(void *, void *, void *);
extern void func_001C9B18(void *, void *);

/* ER-089: by hand from the asm (ER-063 / ER-086 drafts); names, types and offsets are Unconfirmed readings.
   One variable (id) is the part index and, in the i == n scan, the vertex pointer: the original's register
   choice ($a1 for both) comes only from that reuse. buf: ruling 3.13 (an unused local that only sizes the
   0x80 stack frame; nothing reads sp+0..0x3F). */
void func_00113C30(char *a0, s32 a1, void *a2) {
    s32 n, id, i, k, cnt, c, j, w;
    Anm *t;
    char *q, *p, *m, *rec;
    float buf[16]; /* ruling 3.13: sizes the frame only */

    n = func_00114450(a0, a1);
    if (*(u16 *)(a0 + 0xE8) & 1) {
        Anm *t0 = *(Anm **)(a0 + 0xE4);
        id = *(u16 *)(t0->tab[t0->sel] + n * 64 + 4);
        if (id == 0xFFFF) {
            return;
        }
    } else {
        id = n;
    }
    if (!(*(u16 *)(a0 + 0xE8) & 0x8000)) {
        rec = (char *)(id * 0x50 + *(s32 *)(a0 + 8));
        m = *(char **)(rec + 0x3C);
        cnt = *(u16 *)(rec + 0x30);
        p = *(char **)(rec + 0x34);
        do {
            k = *(u16 *)p;
            p += 0x20;
            do {
                k--;
                if (254.9f < *(float *)(p + 0x10) && *(float *)(p + 0x14) < 0.1f && *(float *)(p + 0x18) < 0.1f) {
                    func_001C9F60(a2, *(char **)(a0 + 0xE0) + n * 0xA0, m + *(u16 *)p * 16);
                    return;
                }
                p += 0x20;
            } while (k != 0);
        } while (--cnt != 0);
        return;
    }
    q = *(char **)(a0 + 0xEC);
    for (i = 0; i < *(u16 *)(a0 + 4); i++) {
        if (*(u16 *)(a0 + 0xE8) & 1) {
            t = *(Anm **)(a0 + 0xE4);
            w = *(u16 *)(t->tab[t->sel] + i * 64 + 4);
            if (w == 0xFFFF) {
                continue;
            }
            id = w;
        } else {
            id = i;
        }
        if (i != n) {
            q += 0x30;
            c = *(u16 *)(*(char **)(a0 + 0xF0) + id * 16 + 0xA);
            do {
                w = *(s32 *)q & 0xEFFFFFFF;
                c--;
                j = (w - 5) / 4;
                q += 0x80;
                q += j * 0x40;
                q += 0x10;
            } while (c != 0);
        } else {
            q += 0x30;
            c = *(u16 *)(*(char **)(a0 + 0xF0) + id * 16 + 0xA);
            do {
                w = *(s32 *)q & 0xEFFFFFFF;
                id = (s32)(q + 0x80);
                for (j = (w - 5) / 4 - 1; j != -1; j--) {
                    if (254.9f < *(float *)(id + 0x30)) {
                        func_001C9B18(a2, (void *)id);
                        goto end;
                    }
                    id += 0x40;
                }
                c--;
                q = (char *)id + 0x10;
            } while (c != 0);
        }
    }
end:
    func_001C9F60(a2, *(char **)(a0 + 0xE0) + n * 0xA0, a2);
}

/* ER-115: by hand from the asm (ER-089's draft; as func_00113C30, with a count of matches to skip); names, types and
   offsets are Unconfirmed readings. id is the part index and, in the i == n scan, the vertex pointer (as
   func_00113C30); k holds the record pointer before it counts vertices (one variable reused, recipes 2.23). The
   first found path is a goto to a call after its loop: a call in tail position outside a loop keeps the sibling-call
   pass's placeholder, which stops cross-jumping from merging it with the i == n path's call (recipes 2.25).
   buf: ruling 3.13 (an unused local that only sizes the 0x90 stack frame; nothing reads sp+0..0x3F). */
void func_00113ED8(char *a0, s32 a1, void *a2, s32 skip) {
    s32 n, id, i, k, cnt, c, j, w;
    Anm *t;
    char *q, *p, *m;
    float buf[16]; /* ruling 3.13: sizes the frame only */

    n = func_00114450(a0, a1);
    if (*(u16 *)(a0 + 0xE8) & 1) {
        Anm *t0 = *(Anm **)(a0 + 0xE4);
        id = *(u16 *)(t0->tab[t0->sel] + n * 64 + 4);
        if (id == 0xFFFF) {
            return;
        }
    } else {
        id = n;
    }
    if (!(*(u16 *)(a0 + 0xE8) & 0x8000)) {
        k = id * 0x50 + *(s32 *)(a0 + 8);
        m = *(char **)(k + 0x3C);
        cnt = *(u16 *)(k + 0x30);
        p = *(char **)(k + 0x34);
        do {
            k = *(u16 *)p;
            p += 0x20;
            do {
                if (254.9f < *(float *)(p + 0x10) && *(float *)(p + 0x14) < 0.1f && *(float *)(p + 0x18) < 0.1f) {
                    break;
                }
                k--;
                p += 0x20;
            } while (k != 0);
            if (k != 0) {
                if (skip == 0) {
                    goto found;
                }
                skip--;
                p += k * 0x20;
            }
        } while (--cnt != 0);
        return;
found:
        func_001C9F60(a2, *(char **)(a0 + 0xE0) + n * 0xA0, m + *(u16 *)p * 16);
        return;
    }
    q = *(char **)(a0 + 0xEC);
    for (i = 0; i < *(u16 *)(a0 + 4); i++) {
        if (*(u16 *)(a0 + 0xE8) & 1) {
            t = *(Anm **)(a0 + 0xE4);
            w = *(u16 *)(t->tab[t->sel] + i * 64 + 4);
            if (w == 0xFFFF) {
                continue;
            }
            id = w;
        } else {
            id = i;
        }
        if (i != n) {
            q += 0x30;
            c = *(u16 *)(*(char **)(a0 + 0xF0) + id * 16 + 0xA);
            do {
                w = *(s32 *)q & 0xEFFFFFFF;
                c--;
                j = (w - 5) / 4;
                q += 0x80;
                q += j * 0x40;
                q += 0x10;
            } while (c != 0);
        } else {
            q += 0x30;
            c = *(u16 *)(*(char **)(a0 + 0xF0) + id * 16 + 0xA);
            do {
                w = *(s32 *)q & 0xEFFFFFFF;
                id = (s32)(q + 0x80);
                j = (w - 5) / 4;
                do {
                    if (*(float *)(id + 0x34) < 0.1f && 254.9f < *(float *)(id + 0x30)) {
                        break;
                    }
                    j--;
                    id += 0x40;
                } while (j != 0);
                if (j != 0) {
                    if (skip == 0) {
                        func_001C9B18(a2, (void *)id);
                        func_001C9F60(a2, *(char **)(a0 + 0xE0) + n * 0xA0, a2);
                        return;
                    }
                    skip--;
                    id += j * 0x40;
                }
                c--;
                q = (char *)id + 0x10;
            } while (c != 0);
        }
    }
}

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

INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003541E0);
INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001163F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00116450);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00117418);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00117630);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00117F88);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00117FC0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00118FF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00119318);

extern s32 func_00111CB0(s32);
extern void *D_0035B848;
extern s32 func_00111F50(s32);
extern s8 D_001FFAA9[];

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00119458(s32 arg0) {
    if (arg0 == 1) {
        func_00111CB0(*(s32 *)((char *)D_0035B848 + 0x18));
        func_00111F50(0);
        D_001FFAA9[0] = 0;
        return;
    }
    func_00111CB0(*(s32 *)((char *)D_0035B848 + 0x1C));
    func_00111F50(0);
    D_001FFAA9[0] = 1;
}

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

extern float func_00111578(float *, float *);
extern float D_0035B958[];
extern float D_0035B95C[];

/* ER-035. The turn toward the heading func_00111548 gives (an angle difference d = heading - a), limited to lim
   per call: d itself when |d| < lim, else lim with d's sign, taking the short way round past pi (a reading).
   D_0035B958 (-0.0 in .sdata) is read through %hi / %lo as in func_001115E0. Names and types Unconfirmed. */
float func_0011B9D0(float *a0, float *a1, float a, float lim) {
    float d = func_00111548(a0, a1) - a;
    float ad = d;
    float r;

    if (!(0.0f < d)) ad = -d;
    if (ad < lim) {
        r = d;
    } else {
        if (ad < 3.14159274f) {
            r = lim;
            if (d < D_0035B958[0]) r = -r;
        } else if (d < D_0035B958[0]) {
            ad = d + 6.28318548f;
            if (ad < lim) r = ad;
            else r = lim;
        } else {
            ad = 6.28318548f - d;
            if (ad < lim) r = -ad;
            else r = -lim;
        }
    }
    return r;
}

/* ER-035. func_0011B9D0 with func_00111578 and D_0035B95C (also -0.0). Names and types Unconfirmed. */
float func_0011BAB0(float *a0, float *a1, float a, float lim) {
    float d = func_00111578(a0, a1) - a;
    float ad = d;
    float r;

    if (!(0.0f < d)) ad = -d;
    if (ad < lim) {
        r = d;
    } else {
        if (ad < 3.14159274f) {
            r = lim;
            if (d < D_0035B95C[0]) r = -r;
        } else if (d < D_0035B95C[0]) {
            ad = d + 6.28318548f;
            if (ad < lim) r = ad;
            else r = lim;
        } else {
            ad = 6.28318548f - d;
            if (ad < lim) r = -ad;
            else r = -lim;
        }
    }
    return r;
}

/* Unofficial layout of D_001FF390 (a camera / view work area, a reading): fields named by offset; the struct form
   (not char-array offsets) gives the game's base register, a symbol plus field offset. fc and bits are read by
   func_00120EF0. */
typedef struct {
    char pad0[0x60];
    float x60;
    char pad64[4];
    float x68;
    char pad6C[4];
    float x70;
    float x74;
    char pad78[8];
    float x80;
    char pad84[4];
    float x88;
    char pad8C[4];
    float x90;
    float x94;
    char pad98[8];
    s32 xA0;
    char padA4[4];
    s32 xA8;
    char padAC[4];
    s32 xB0;
    s32 xB4;
    char padB8[0xC];
    u8 xC4;
    char padC5[0x1B];
    float xE0;
    float xE4;
    float xE8;
    char padEC[0x10];
    char *fc;
    u8 x100;
    u8 x101;
    char pad102[6];
    float x108;
    char pad10C[4];
    float x110[4];
    char pad120[0x20];
    u8 bits[64];
} G390;
extern G390 D_001FF390;
extern float D_001FF340[];
extern char D_001FFAA4[];
extern float D_001FF180[];
extern float D_001FF100[];
extern void func_001E9B0C(void *, s32, s32);
extern void func_001CA048(float *, float *, float);
extern void func_001D73D0(float *, float *);
extern float func_001114E8(char *, float *);
extern void func_001C9FE8(void *, void *, void *);
extern void func_001D7450(void *, float *);
extern void func_001D7340(void *, void *, void *);

/* ER-092: by hand from the asm (ER-073's m2c draft as a map); names, types and offsets are Unconfirmed readings.
   Pad bits of D_0035B828 (read once) step the clamped counters at 0xA0 / 0xA8 / 0xB0 / 0xB4, which decay to 0
   when their bit is idle; then angles are wrapped to [-pi, pi] and the view matrix D_001FF180 is rebuilt. */
void func_0011BB90(void) {
    float a[4];
    float v[4];
    float d[4];
    float r[4];
    float m[16];
    float ang;
    float nrx;
    float len;
    s32 x;
    float *p;
    s32 btn;

    func_001E9B0C(v, 0, 0x10);
    btn = D_0035B828;
    v[3] = 1.0f;
    D_001FF390.xC4 = 0;
    D_001FF390.x80 = 0.2f;
    D_001FF390.x88 = 0.2f;
    D_001FF390.x90 = 0.0006f;
    D_001FF390.x94 = 0.0008f;
    if (btn & 0x200) {
        D_001FF390.xB0--;
        if (D_001FF390.xB0 < -25) {
            D_001FF390.xB0 = -25;
        }
        D_001FF390.xC4 |= 8;
        r[0] = -0.006f;
    }
    if (btn & 0x100) {
        D_001FF390.xB0++;
        if (D_001FF390.xB0 > 25) {
            D_001FF390.xB0 = 25;
        }
        D_001FF390.xC4 |= 8;
        r[0] = 0.006f;
    }
    if (btn & 0x20) {
        D_001FF390.xB4++;
        if (D_001FF390.xB4 > 25) {
            D_001FF390.xB4 = 25;
        }
        D_001FF390.xC4 |= 4;
        r[1] = 0.006f;
    }
    if (btn & 0x80) {
        D_001FF390.xB4--;
        if (D_001FF390.xB4 < -25) {
            D_001FF390.xB4 = -25;
        }
        D_001FF390.xC4 |= 4;
        r[1] = -0.006f;
    }
    if (btn & 0x800) {
        D_001FF390.xA0++;
        if (D_001FF390.xA0 > 20) {
            D_001FF390.xA0 = 20;
        }
        D_001FF390.xC4 |= 1;
        d[0] = 1.5f;
    }
    if (btn & 0x400) {
        D_001FF390.xA0--;
        if (D_001FF390.xA0 < -20) {
            D_001FF390.xA0 = -20;
        }
        D_001FF390.xC4 |= 1;
        d[0] = -1.5f;
    }
    if (btn & 0x10) {
        D_001FF390.xA8++;
        if (D_001FF390.xA8 > 20) {
            D_001FF390.xA8 = 20;
        }
        D_001FF390.xC4 |= 2;
        d[2] = 1.5f;
    }
    if (btn & 0x40) {
        D_001FF390.xA8--;
        if (D_001FF390.xA8 < -20) {
            D_001FF390.xA8 = -20;
        }
        D_001FF390.xC4 |= 2;
        d[2] = -1.5f;
    }
    a[1] = 0.0f;
    a[3] = 1.0f;
    if (btn & 0x1000) {
        a[1] = -5.0f;
    }
    if (btn & 0x4000) {
        a[1] += 5.0f;
    }
    if ((D_001FF390.xC4 & 1) && (D_001FF390.xC4 & 2)) {
        D_001FF390.x60 = (d[0] + D_001FF390.x80 * (float)D_001FF390.xA0) * 0.70710677f;
        D_001FF390.x68 = (d[2] + D_001FF390.x88 * (float)D_001FF390.xA8) * 0.70710677f;
    } else {
        if (D_001FF390.xC4 & 1) {
            D_001FF390.x60 = d[0] + D_001FF390.x80 * (float)D_001FF390.xA0;
        } else {
            if (D_001FF390.xA0 > 0) {
                D_001FF390.xA0 = D_001FF390.xA0 - 1;
            } else if (D_001FF390.xA0 < 0) {
                D_001FF390.xA0 = D_001FF390.xA0 + 1;
            }
            D_001FF390.x60 = D_001FF390.x80 * (float)D_001FF390.xA0;
        }
        if (D_001FF390.xC4 & 2) {
            x = D_001FF390.xA8;
            D_001FF390.x68 = d[2] + D_001FF390.x88 * (float)x;
            if (D_001FF390.xC4 & 0x20) {
                if (x > 0) {
                    D_001FF390.xA8 = x - 1;
                } else if (x < 0) {
                    D_001FF390.xA8 = x + 1;
                }
            }
        } else {
            if (D_001FF390.xA8 > 0) {
                D_001FF390.xA8 = D_001FF390.xA8 - 1;
            } else if (D_001FF390.xA8 < 0) {
                D_001FF390.xA8 = D_001FF390.xA8 + 1;
            }
            D_001FF390.x68 = D_001FF390.x88 * (float)D_001FF390.xA8;
        }
    }
    a[0] = D_001FF390.x60;
    a[2] = D_001FF390.x68;
    if (D_001FF390.xC4 & 8) {
        D_001FF390.x70 = r[0] + D_001FF390.x90 * (float)D_001FF390.xB0;
    } else {
        if (D_001FF390.xB0 > 0) {
            D_001FF390.xB0 = D_001FF390.xB0 - 1;
        } else if (D_001FF390.xB0 < 0) {
            D_001FF390.xB0 = D_001FF390.xB0 + 1;
        }
        D_001FF390.x70 = D_001FF390.x90 * (float)D_001FF390.xB0;
    }
    if (D_001FF390.xC4 & 4) {
        x = D_001FF390.xB4;
        D_001FF390.x74 = r[1] + D_001FF390.x94 * (float)x;
        if (D_001FF390.xC4 & 0x40) {
            if (x > 0) {
                D_001FF390.xB4 = x - 1;
            } else if (x < 0) {
                D_001FF390.xB4 = x + 1;
            }
        }
    } else {
        if (D_001FF390.xB4 > 0) {
            D_001FF390.xB4 = D_001FF390.xB4 - 1;
        } else if (D_001FF390.xB4 < 0) {
            D_001FF390.xB4 = D_001FF390.xB4 + 1;
        }
        D_001FF390.x74 = D_001FF390.x94 * (float)D_001FF390.xB4;
    }
    if (D_001FFAA4[0] == 0) {
        D_001FF340[4] += D_001FF390.x70;
        D_001FF340[5] += D_001FF390.x74;
    }
    if (btn & 0x2000) {
        func_001CA048(a, a, 8.0f);
    }
    if (3.1415927f < D_001FF340[4]) {
        D_001FF340[4] -= 6.2831855f;
    }
    if (D_001FF340[4] < -3.1415927f) {
        D_001FF340[4] += 6.2831855f;
    }
    if (3.1415927f < D_001FF340[5]) {
        D_001FF340[5] -= 6.2831855f;
    }
    if (D_001FF340[5] < -3.1415927f) {
        D_001FF340[5] += 6.2831855f;
    }
    ang = 3.1415927f - D_001FF340[5];
    nrx = -D_001FF340[4];
    if (3.1415927f < ang) {
        ang -= 6.2831855f;
    }
    if (ang < -3.1415927f) {
        ang += 6.2831855f;
    }
    func_001D7820(m, D_001F5780, -ang);
    func_001C9F60(v, m, a);
    if (D_001FF390.x100 != 0 && (D_001FF390.x101 & 2)) {
        p = D_001FF390.x110;
        D_001FF390.xC4 |= 0x22;
        func_001CA018(d, D_001FF340, p);
        d[1] = 0.0f;
        func_001D73D0(d, d);
        len = func_001114E8((char *)D_001FF340, p);
        if (len < D_001FF390.x108) {
            func_001CA048(d, d, len);
        } else {
            func_001CA048(d, d, D_001FF390.x108);
        }
        v[0] -= d[0];
        v[2] -= d[2];
    }
    D_001FF390.xE4 += v[1];
    func_001C9FE8(D_001FF340, D_001FF340, v);
    D_001FF340[1] = D_001FF390.xE4 - D_001FF390.xE0 - D_001FF390.xE8;
    func_001D7820(D_001FF180, D_001F5780, ang);
    func_001D7778(D_001FF180, D_001FF180, nrx);
    func_001D76D0(D_001FF180, D_001FF180, D_001FF340[6]);
    func_001D7450(&D_001FF390, D_001FF180);
    a[0] = -D_001FF340[0];
    a[1] = -D_001FF340[1];
    a[2] = -D_001FF340[2];
    func_001C9F60(a, D_001FF180, a);
    func_001D7588(D_001FF180, D_001FF180, a);
    func_001D7340(D_001FF200, D_001FF100, D_001FF180);
}

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

extern void func_00123C28(void *, void *, f32 fparg0, f32 fparg1, f32 fparg2, f32 fparg3);
extern void func_00124370(void *, void *, f32 fparg0, f32 fparg1, f32 fparg2);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00120D08(void **arg0, void *arg1, s32 arg2) {
    f32 var_f14;
    void *temp_3;
    void *temp_4;

    temp_4 = *arg0;
    temp_3 = *(void **)((char *)arg1 + 0x74);
    if (arg2 == 0) {
        var_f14 = *(f32 *)((char *)temp_3 + 4);
        if (*(u8 *)((char *)temp_3 + 2) != 0) {
            func_00123C28(temp_4 + 0x90, arg1 + 0x50, *(f32 *)((char *)temp_4 + 0x114), *(f32 *)((char *)temp_4 + 0x118), var_f14, *(f32 *)((char *)temp_3 + 8));
        } else {
            goto block_4;
        }
    } else {
        var_f14 = *(f32 *)((char *)temp_3 + 0xC);
        if (*(u8 *)((char *)temp_3 + 3) == 0) {
block_4:
            func_00124370(temp_4 + 0x90, arg1 + 0x50, *(f32 *)((char *)temp_4 + 0x114), *(f32 *)((char *)temp_4 + 0x118), var_f14);
        } else {
            func_00123C28(temp_4 + 0x90, arg1 + 0x50, *(f32 *)((char *)temp_4 + 0x114), *(f32 *)((char *)temp_4 + 0x118), var_f14, *(f32 *)((char *)temp_3 + 0x10));
        }
    }
    return *(s32 *)0x70000010;
}

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

/* ER-035. Returns 1 when the point a1 (x at 0, z at 8) lies outside radius r of the position at a0 + 0x90 /
   0x98, by a box test first, then by squared distance. Names and types Unconfirmed. */
int func_00123BA8(float *a0, float *a1, float r) {
    float dx = a1[0] - a0[0x90 / 4];
    float dz = a1[2] - a0[0x98 / 4];

    if (r < dx) return 1;
    if (dx < -r) return 1;
    if (r < dz) return 1;
    if (dz < -r) return 1;
    return r * r < dx * dx + dz * dz;
}

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

extern void * func_001139C8(s32, void *);
extern char *D_0035B8A0;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001257E8(void) {
    *(s32 *)((char *)D_0035B848 + 4) = func_001139C8(*(s32 *)((char *)D_0035B848 + 4), D_0035B8A0 + 0x1200);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125818);

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

extern char *D_0035B8A8;

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00125C10(void) {
    void *temp_4;

    temp_4 = *(void **)((char *)D_0035B8A8 + 0xC);
    *(u8 *)((char *)temp_4 + 0x5A) = (u8) (*(u8 *)((char *)temp_4 + 0x5A) | 2);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00125C28(void) {
    *(s32 *)((char *)D_0035B848 + 4) = func_001139C8(*(s32 *)((char *)D_0035B848 + 4), D_0035B8A0 + 0x14700);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00125C60);

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

extern void func_001152D8(void *, s32 *, s32, f32 fparg0);
extern void func_00115408(void *, s32 *, s32, f32 fparg0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00126EB0(void) {
    s8 *temp_16;

    temp_16 = D_0035B8A0 + 0x14700;
    func_001152D8(temp_16, *(s32 **)((char *)temp_16 + 0xEC), 0x80, 0.03f);
    func_00115408(temp_16, *(s32 **)((char *)temp_16 + 0xEC), 0x50, 0.8f);
}

/* Unofficial: a 16-byte aligned float vector, copied by assignment (ld / sd pairs). */
typedef struct {
    float v[4];
} __attribute__((aligned(16))) Vec4;

extern const Vec4 D_003543C0;
extern const Vec4 D_003543B0;
extern const Vec4 D_00354390;
extern s32 func_001E9C70();
extern void func_00158AD8(s32, s32, void *, void *, void *, void *);

/* ER-094: ER-063's draft; the three vector constants are const (they sit in .rodata): only then does the scheduler
   issue each copy's %lo load before its +8 load, as the game does. Names, types and offsets are Unconfirmed
   readings. */
void func_00126F00(void) {
    Vec4 p;
    Vec4 v;
    Vec4 out;
    Vec4 a = D_003543C0;
    Vec4 b = D_003543B0;
    Vec4 c = D_00354390;
    float m[16] __attribute__((aligned(16)));
    float ang;
    float d;
    float sc;
    float dz;
    float h;
    s32 n;
    s32 r;

    sc = -1.0f;
    ang = 3.1415927f - D_001FF340[5];
    if (3.1415927f < ang) {
        ang -= 6.2831855f;
    }
    if (ang < -3.1415927f) {
        ang += 6.2831855f;
    }
    h = 1200.0f;
    n = 2400;
    func_001D7778(m, D_001F5780, -sc);
    func_001D7820(m, m, -ang);
    func_001C9B18(&p, D_001FF340);
    func_001C9F60(&a, m, &a);
    r = func_001E9C70() % 800;
    func_001CA048(v.v, a.v, r);
    func_001C9FE8(&v, &v, &p);
    d = func_001E9C70() % n - h;
    dz = func_001E9C70() % 2400 - h;
    v.v[0] += d;
    v.v[2] += dz;
    func_00158AD8(0x5D, 0x3B, &v, &out, &c, &b);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001270E0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00127180(void) {
    *(s8 *)((char *)(*(void **)((char *)D_0035B8A8 + 0x3E0)) + 0xD98) = 1;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00127198);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001275B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00127658);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001276B0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00127840);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00127868);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00127E28);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00128478);

extern void func_00115E48(s32, s16, s32, s32, s16);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00128928(void) {
    func_00115E48(0x2F40, 0x100, 0x100, 0x13, 8);
}

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

extern char D_002608C0[];

/* ER-081: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_00129ED8(s32 arg0) {
    s32 i;
    char *p;

    i = 0x1FF;
    p = D_002608C0;
    do {
        i--;
        if (*(u16 *)p != 0 && *(u16 *)(p + 2) == arg0) {
            *(s32 *)(p + 4) = 0;
            *(u16 *)p = 0;
        }
        p += 0xA0;
    } while (i >= 0);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00129F18);

INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354360);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354370);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354380);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354390);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003543A0);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003543B0);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003543C0);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003543D0);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003543E0);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003543F0);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354400);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354410);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354470);
INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00129F78);


/* ER-081: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0012A140(void *arg0, void *arg1, void *arg2) {
    func_001C9B18(D_001F58C0, arg0);
    func_001C9B18(D_001F58C0 + 4, arg1);
    func_001C9B18(D_001F58C0 + 8, arg2);
}

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

extern void func_0012B240(void *, s32, s32, f32 fparg0);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0012B1F0(void *arg0, s32 arg1, s32 arg2) {
    void *temp_2;

    temp_2 = *(void **)arg0;
    func_0012B240(temp_2, *(u8 *)((char *)temp_2 + 0x147) & arg1, arg2, *(f32 *)((char *)arg0 + 0xB4));
}

extern void func_0012B1F0(void *arg0, s32 arg1, s32 arg2);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0012B208(void *arg0, s32 arg1) {
    func_0012B1F0(arg0, 0xFF, arg1);
}

extern void func_0012B218(void *, int);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0012B218(void *arg0, s32 arg1) {
    void *temp_2;

    temp_2 = *(void **)arg0;
    func_0012B240(temp_2, *(u8 *)((char *)temp_2 + 0x147) & arg1, 1, *(f32 *)((char *)arg0 + 0xB4));
}


/* ER-081: by hand; $a0 passes through to func_0012B218 (recipes 2.10). Names and types are Unconfirmed readings. */
void func_0012B238(void *arg0) {
    func_0012B218(arg0, 0xFF);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B240);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B540);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B5F8);

extern s32 func_0012BAE0(void *, s32, s32, f32 fparg0);
extern s32 func_0012B540(void *, s32, s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012B6B0(void **arg0, s32 arg1, s32 arg2, s32 arg3, f32 fparg0) {
    s32 temp_19;
    s32 temp_6;
    void *temp_3;
    void *temp_3_2;
    void *temp_4;
    void *temp_5;
    void *temp_5_2;
    void *temp_5_3;

    temp_5 = *arg0;
    temp_19 = (s32) (0x10000 << arg1) >> 0x10;
    if (*(u8 *)((char *)((temp_5 + arg1)) + 0x148) != arg2) {
        if (func_0012BAE0(temp_5, arg1, arg2 & 0xFF, fparg0) != 0) {
            return func_0012B540(*arg0, arg1, arg2, arg3);
        }
        temp_5_2 = *arg0;
        temp_6 = ~temp_19;
        *(u8 *)((char *)temp_5_2 + 0x147) = (u8) (*(u8 *)((char *)temp_5_2 + 0x147) & temp_6);
        temp_4 = *arg0;
        *(u8 *)((char *)temp_4 + 0x146) = (u8) (*(u8 *)((char *)temp_4 + 0x146) & temp_6);
        temp_5_3 = *arg0;
        *(u8 *)((char *)temp_5_3 + 0x146) = (u8) (*(u8 *)((char *)temp_5_3 + 0x146) | temp_19);
        return 0;
    }
    *(u8 *)((char *)temp_5 + 0x147) = (u8) (*(u8 *)((char *)temp_5 + 0x147) | temp_19);
    temp_3 = *arg0;
    *(u8 *)((char *)temp_3 + 0x146) = (u8) (*(u8 *)((char *)temp_3 + 0x146) & ~temp_19);
    if (arg3 != 0) {
        temp_3_2 = *arg0;
        *(u8 *)((char *)temp_3_2 + 0x146) = (u8) (*(u8 *)((char *)temp_3_2 + 0x146) | temp_19);
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B7C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B890);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012B930);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012BA30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012BAE0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012BCE0);

extern float func_0012BEC8(char *, float);

/* ER-063. Twin of func_0012BF08 that first sets v[1] = func_0012BEC8(a1, pos[1]) (returns 1 if that changed it).
   Readings, Unconfirmed: v = pos clamped in x / z to the radius at obj+0x12C around the point at a1+0x80. */
s32 func_0012BDA8(void *out, char *a1, float *pos) {
    float v[4];
    char *obj;
    float d;
    s32 r;

    r = 0;
    obj = *(char **)a1;
    func_001C9B18(v, pos);
    v[1] = func_0012BEC8(a1, pos[1]);
    if (v[1] != pos[1]) {
        r = 1;
    }
    d = func_001114E8(a1 + 0x80, pos);
    if (*(float *)(obj + 0x12C) < d) {
        v[0] = *(float *)(a1 + 0x80);
        v[2] = *(float *)(a1 + 0x88);
        r = 1;
        if (0.0f < *(float *)(obj + 0x12C)) {
            v[0] = *(float *)(a1 + 0x80) + (pos[0] - *(float *)(a1 + 0x80)) * *(float *)(obj + 0x12C) / d;
            v[2] = *(float *)(a1 + 0x88) + (pos[2] - *(float *)(a1 + 0x88)) * *(float *)(obj + 0x12C) / d;
        }
    }
    if (out == NULL) {
        return r;
    }
    func_001C9B18(out, v);
    return r;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012BEC8);

/* ER-063 (parked by ER-034). Copies pos into v; if pos is farther than the radius at obj+0x12C from the point at
   a1+0x80 (func_001114E8: a distance), returns 1 and pulls v's x / z onto that radius; copies v to out when given
   (readings, Unconfirmed). The separate `if (out == NULL) return r;` gives the game's $s3 / $s4 for r and out. */
s32 func_0012BF08(void *out, char *a1, float *pos) {
    float v[4];
    char *obj;
    float d;
    s32 r;

    r = 0;
    obj = *(char **)a1;
    func_001C9B18(v, pos);
    if (*(float *)(obj + 0x12C) != 0.0f) {
        d = func_001114E8(a1 + 0x80, pos);
        if (*(float *)(obj + 0x12C) < d) {
            v[0] = *(float *)(a1 + 0x80);
            v[2] = *(float *)(a1 + 0x88);
            r = 1;
            if (0.0f < *(float *)(obj + 0x12C)) {
                v[0] = *(float *)(a1 + 0x80) + (pos[0] - *(float *)(a1 + 0x80)) * *(float *)(obj + 0x12C) / d;
                v[2] = *(float *)(a1 + 0x88) + (pos[2] - *(float *)(a1 + 0x88)) * *(float *)(obj + 0x12C) / d;
            }
        }
    }
    if (out == NULL) {
        return r;
    }
    func_001C9B18(out, v);
    return r;
}

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

extern void func_0010D5E8(char *, char *, char *, char *);

/* ER-081: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_0012C778(char *arg0) {
    func_0010D5E8(arg0, arg0 + 0x90, arg0 + 0xA0, arg0 + 0xB0);
}

extern void func_0010CBF8(void *);
extern void func_0012C8D0(void *, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0012C788(void *arg0) {
    func_0010CBF8(arg0);
    func_0012C8D0(*(void **)((char *)arg0 + 0xE0), (s32) (arg0 + 0x50));
}

extern void func_001C9EE8(float *, float *, char *);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0012C7B8(f32 *arg0, f32 *arg1) {
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f12_3;
    f32 temp_f20;
    f32 temp_f21;

    temp_f21 = 3.1415927f;
    temp_f20 = 6.2831855f;
    temp_f12 = *(f32 *)((char *)arg0 + 0x50) + temp_f21;
    func_001D7778(arg0, D_001F5780, (temp_f12 - ((f32) (s32) (temp_f12 / temp_f20) * temp_f20)) - temp_f21);
    temp_f12_2 = *(f32 *)((char *)arg0 + 0x58) + temp_f21;
    func_001D76D0(arg0, arg0, (temp_f12_2 - ((f32) (s32) (temp_f12_2 / temp_f20) * temp_f20)) - temp_f21);
    temp_f12_3 = *(f32 *)((char *)arg0 + 0x54) + temp_f21;
    func_001D7820(arg0, arg0, (temp_f12_3 - ((f32) (s32) (temp_f12_3 / temp_f20) * temp_f20)) - temp_f21);
    func_001D7588(arg0, arg0, arg0 + 16);
    func_001C9EE8(arg0, arg1, (s8 *) arg0);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C8D0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C938);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012C9D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CA68);

extern void func_0012CAD8(void *, void *, int);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); the file's prototype kept (casts as its other callers of
   func_001D75C8); names, types and offsets are Unconfirmed readings. */
void func_0012CAD8(void *arg0, void *arg1, int arg2) {
    func_001D75C8((s32)arg0, (s32 *)(*(s32 *)((char *)arg1 + 0xE0) + (arg2 * 0xA0)));
}

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0012CAF0(void *arg0, void *arg1, s32 arg2) {
    func_001C9B18(arg0, *(s32 *)((char *)arg1 + 0xE0) + (arg2 * 0xA0) + 0x30);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CB08);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CB70);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CBF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CC68);

extern void func_0012EAF8(void *, void *);

/* ER-035. Builds two matrices for entry idx of a0 (func_0012CAD8, func_0012EAF8), takes the heading of a2 seen
   from there (atan2 of x and z, a reading) plus the entry's angle at +0x54 (entries of 0xA0 bytes at a0 + 0xE0),
   and wraps it into [-pi, pi]. Names, types and the matrix reading Unconfirmed. */
float func_0012CD08(void *a0, int idx, void *a2) {
    float m0[4][4];
    float m1[4][4];
    float v0[4];
    float v1[4];
    float f;

    func_0012CAD8(m0, a0, idx);
    func_001CA018(v0, a2, m0[3]);
    func_0012EAF8(m1, m0);
    func_001C9F60(v1, m1, v0);
    f = *(float *)(*(char **)((char *)a0 + 0xE0) + idx * 0xA0 + 0x54) + func_001E41B8(v1[0], v1[2]);
    while (3.14159274f < f) f -= 6.28318548f;
    while (f < -3.14159274f) f += 6.28318548f;
    return f;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CE18);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CE88);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012CF90);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D138);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D2D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D318);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D388);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D3B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D3E8);

extern float sqrtf(float);
extern void func_0012C778(char *);

/* ER-063. a0+0x90 (a vector, reading) is moved by d (x and z negated) through the matrix at (*a0)+0x50, then clamped
   to length max (func_0012D568 inline); returns the length. Names and types Unconfirmed. */
float func_0012D470(char *a0, float *d, float max) {
    float m[4];
    float t[4];
    float *v;
    char *o;
    float len;

    v = (float *)(a0 + 0x90);
    o = *(char **)a0;
    t[0] = -d[0];
    t[1] = d[1];
    t[2] = -d[2];
    t[3] = 0.0f;
    func_0012C778(o);
    func_001C9F60(m, o + 0x50, t);
    func_001C9FE8(v, v, m);
    len = sqrtf(*(float *)(a0 + 0x90) * *(float *)(a0 + 0x90) + *(float *)(a0 + 0x94) * *(float *)(a0 + 0x94)
                + *(float *)(a0 + 0x98) * *(float *)(a0 + 0x98));
    if (max < len) {
        func_001CA048(v, v, max / len);
        len = max;
    }
    return len;
}

/* ER-041 (placed by ER-063). Clamps v to length max; returns the length. Names and types Unconfirmed. */
float func_0012D568(float *v, float max) {
    float len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    if (max < len) {
        func_001CA048(v, v, max / len);
        len = max;
    }
    return len;
}


/* ER-063 / ER-086: matched with -fno-edge-lcm (ruling 3.11); func_001CA048 takes its float last (only the order of
   the argument moves shows it). Names, types and offsets are Unconfirmed readings. */
s32 func_0012D600(float *out, float *v, float lo0, float hi0, float lo1, float hi1) {
    float a[2];
    float m[16];
    s32 r;
    float len;
    float *p;

    r = 0;
    a[0] = func_001E41B8(-v[1], sqrtf(v[0] * v[0] + v[2] * v[2]));
    a[1] = func_001E41B8(v[0], v[2]);
    if (a[0] < lo0) {
        a[0] = lo0;
        r = 1;
    }
    if (hi0 < a[0]) {
        a[0] = hi0;
        r = 1;
    }
    if (a[1] < lo1) {
        a[1] = lo1;
        r = 1;
    }
    if (hi1 < a[1]) {
        a[1] = hi1;
        r = 1;
    }
    if (r != 0) {
        func_001D7778(m, D_001F5780, a[0]);
        func_001D7820(m, m, a[1]);
        p = &m[8];
        len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        func_001CA048(out, p, len);
    }
    return r;
}

/* ER-063. As func_0012D470 with d = {0, 0, a, 0} and the length taken in x / z only. Names and types Unconfirmed. */
float func_0012D7C0(char *a0, float a, float max) {
    float m[4];
    float q[4];
    float t[4];
    float *v;
    char *o;
    float len;

    q[0] = 0.0f;
    q[1] = 0.0f;
    q[2] = a;
    q[3] = 0.0f;
    v = (float *)(a0 + 0x90);
    o = *(char **)a0;
    t[0] = -q[0];
    t[1] = q[1];
    t[2] = -q[2];
    t[3] = 0.0f;
    func_0012C778(o);
    func_001C9F60(m, o + 0x50, t);
    func_001C9FE8(v, v, m);
    len = sqrtf(*(float *)(a0 + 0x90) * *(float *)(a0 + 0x90) + *(float *)(a0 + 0x98) * *(float *)(a0 + 0x98));
    if (max < len) {
        func_001CA048(v, v, max / len);
        len = max;
    }
    return len;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D8B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012D9B8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DA18);

extern f32 func_001314C8(s32 arg0, s32 arg1);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain), func_001314C8 declared f32 by hand; names, types and offsets are Unconfirmed readings. */
s32 func_0012DA68(void **arg0, s32 arg1, s32 arg2, f32 fparg0, f32 fparg1) {
    f32 var_f2;
    s32 var_3;

    if (arg1 >= arg2) {
        var_f2 = 1.0f;
    } else {
        var_f2 = func_001314C8(arg1, arg2);
    }
    var_3 = 1;
    *(f32 *)((char *)(*arg0) + 0xA4) = (f32) (fparg0 + (fparg1 * var_f2));
    if (var_f2 != 1.0f) {
        var_3 = 0;
    }
    return var_3;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DAF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DB88);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DC10);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DDA0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DF30);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012DF90);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E198);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E320);

void func_00131678(void *arg0, s8 arg1);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0012E3B0(void *arg0, s32 arg1, s16 arg2) {
    *(s32 *)((char *)arg0 + 0xB8) = arg1;
    *(s16 *)((char *)arg0 + 0x74) = arg2;
    func_00131678(arg0, 1);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E3C8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E428);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E488);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012E568(s32 arg0, s32 arg1) {
    s32 temp_17;

    temp_17 = arg1 - arg0;
    if (temp_17 == 0) {
        return arg0;
    }
    return arg0 + (func_001E9C70() % (s32) (temp_17 + 1));
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E5C0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
f32 func_0012E640(f32 fparg0, f32 fparg1) {
    f32 temp_f21;

    temp_f21 = fparg1 - fparg0;
    if (temp_f21 == 0.0f) {
        return fparg0;
    }
    return fparg0 + ((temp_f21 * (f32) (func_001E9C70() & 0x7F)) / 127.0f);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E6B8);

extern void func_00111620(s32 arg0, s32 arg1);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
u32 func_0012E750(s32 arg0, s32 arg1) {
    s32 temp_5;

    func_00111620(arg1, arg0);
    temp_5 = (s32) *(s32 *)((char *)arg0 + 8) >> 4;
    *(s32 *)arg0 = (s32) (((s32) *(s32 *)arg0 >> 4) - 0x800);
    *(s32 *)((char *)arg0 + 4) = (s32) (((s32) *(s32 *)((char *)arg0 + 4) >> 4) - 0x800);
    *(s32 *)((char *)arg0 + 8) = temp_5;
    return (u32) ~temp_5 >> 0x1F;
}

/* ER-026. Wraps the angle a - b into [-pi, pi] (a reading); names and types Unconfirmed. gcc pads each short loop to 6 instructions itself. */
float func_0012E7B0(float a, float b) {
    float f = a - b;

    if (3.14159274f < f) {
        do {
            f -= 6.28318548f;
        } while (3.14159274f < f);
    }
    if (f < -3.14159274f) {
        do {
            f += 6.28318548f;
        } while (f < -3.14159274f);
    }
    return f;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012E820);

/* ER-035. Turns the angle *a0 toward a by step (func_0012E7B0 gives the wrapped difference); stops on a when the
   step would pass it, wraps the result into [-pi, pi] and returns the difference left (0 when already there).
   Names and types Unconfirmed. */
float func_0012E908(float *a0, float a, float step) {
    float cur = *a0;
    float d = func_0012E7B0(a, cur);
    float r;

    if (d == 0.0f) return 0.0f;
    if (d < 0.0f) cur -= step;
    else cur += step;
    r = func_0012E7B0(a, cur);
    if (d * r <= 0.0f) {
        cur = a;
        r = 0.0f;
    }
    if (3.14159274f < cur) {
        do {
            cur -= 6.28318548f;
        } while (3.14159274f < cur);
    }
    if (cur < -3.14159274f) {
        do {
            cur += 6.28318548f;
        } while (cur < -3.14159274f);
    }
    *a0 = cur;
    return r;
}

/* ER-026. Moves *a0 toward a1 by (a1 - *a0) * a2, by at most a3 either way. Names and types Unconfirmed. */
void func_0012EA28(float *a0, float a1, float a2, float a3) {
    float d = (a1 - *a0) * a2;

    if (d < -a3) {
        *a0 = *a0 - a3;
    } else if (a3 < d) {
        *a0 = *a0 + a3;
    } else {
        *a0 = *a0 + d;
    }
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012EA78(s32 *arg0, s32 arg1, s32 arg2) {
    s32 temp_6;
    s32 var_3;
    s32 var_7;

    var_3 = *arg0 + arg1;
    var_7 = 0;
    if (arg2 < var_3) {
        var_3 = arg2;
        var_7 = 1;
    }
    temp_6 = -arg2;
    if (var_3 < temp_6) {
        var_3 = temp_6;
        var_7 = -1;
    }
    *arg0 = var_3;
    return var_7;
}

/* ER-026. *a0 += a1, clamped to [-a2, a2]; returns 1 / -1 when clamped high / low, else 0. Types Unconfirmed. */
int func_0012EAB8(float *a0, float a1, float a2) {
    float f = *a0 + a1;
    int r = 0;

    if (a2 < f) {
        f = a2;
        r = 1;
    }
    a2 = -a2;
    if (f < a2) {
        f = a2;
        r = -1;
    }
    *a0 = f;
    return r;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012EAF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012EB60);


/* ER-086: by hand after func_0012EEB0 (-fno-edge-lcm, ruling 3.11); names, types and offsets are Unconfirmed readings. */
void func_0012EBB0(float *out, float *src) {
    float m[16];
    float e[4];
    float r[4];
    s32 flip;
    float len;

    flip = 1;
    func_001D75C8((s32)m, (s32 *)src);
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 0.0f;
    e[0] = 1.0f;
    e[1] = 0.0f;
    e[2] = 0.0f;
    e[3] = 0.0f;
    func_001C9F60(r, m, e);
    if (!(r[0] < 0.0f)) flip = 0;
    out[2] = func_001E41B8(r[1], r[0]);
    len = sqrtf(r[0] * r[0] + r[1] * r[1]);
    out[1] = func_001E41B8(-r[2], len);
    func_001D76D0(m, m, -out[2]);
    func_001D7820(m, m, -out[1]);
    e[0] = 0.0f;
    e[1] = 0.0f;
    e[2] = 1.0f;
    e[3] = 0.0f;
    func_001C9F60(r, m, e);
    out[0] = func_001E41B8(-r[1], r[2]);
    if (flip) {
        out[0] += 3.1415927f;
        out[1] = 3.1415927f - out[1];
        out[2] += 3.1415927f;
    }
    while (3.1415927f < out[0]) out[0] -= 6.2831855f;
    while (out[0] < -3.1415927f) out[0] += 6.2831855f;
    while (3.1415927f < out[1]) out[1] -= 6.2831855f;
    while (out[1] < -3.1415927f) out[1] += 6.2831855f;
    while (3.1415927f < out[2]) out[2] -= 6.2831855f;
    while (out[2] < -3.1415927f) out[2] += 6.2831855f;
}

/* ER-063 / ER-086: matched with -fno-edge-lcm (ruling 3.11); names, types and offsets are Unconfirmed readings. */
void func_0012EEB0(float *out, float *src) {
    float m[16];
    float e[4];
    float r[4];

    func_001D75C8((s32)m, (s32 *)src);
    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 0.0f;
    e[0] = 1.0f;
    e[1] = 0.0f;
    e[2] = 0.0f;
    e[3] = 0.0f;
    func_001C9F60(r, m, e);
    out[1] = -func_001E41B8(r[2], r[0]);
    out[2] = func_001E41B8(r[1], sqrtf(r[0] * r[0] + r[2] * r[2]));
    func_001D7820(m, m, -out[1]);
    func_001D76D0(m, m, -out[2]);
    e[0] = 0.0f;
    e[1] = 0.0f;
    e[2] = 1.0f;
    e[3] = 0.0f;
    func_001C9F60(r, m, e);
    out[0] = func_001E41B8(-r[1], r[2]);
    while (3.1415927f < out[0]) out[0] -= 6.2831855f;
    while (out[0] < -3.1415927f) out[0] += 6.2831855f;
    while (3.1415927f < out[1]) out[1] -= 6.2831855f;
    while (out[1] < -3.1415927f) out[1] += 6.2831855f;
    while (3.1415927f < out[2]) out[2] -= 6.2831855f;
    while (out[2] < -3.1415927f) out[2] += 6.2831855f;
}

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed.
   ER-086: passes both pointers on to func_0012EEB0 (recipes 2.13); same code. */
void func_0012F158(float *arg0, float *arg1) {
    func_0012EEB0(arg0, arg1);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F160);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F1D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F2F0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F418);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F540);

/* ER-035. Steps *p by +step when a0 & a1 and by -step when a0 & a2 (button bits, a reading), then clamps it to
   [lo, hi], or wraps it to the other end when wrap is set; returns the sign of the change made (truncated to int).
   Names and types Unconfirmed. */
int func_0012F550(u16 a0, u16 a1, u16 a2, float *p, float step, float lo, float hi, int wrap) {
    float f, old;
    int i, r;

    if (p == NULL) return 0;
    f = *p;
    old = f;
    if (a0 & a1) f += step;
    if (a0 & a2) f -= step;
    if (f < lo) {
        if (wrap) f = hi;
        else f = lo;
    }
    if (hi < f) {
        if (wrap) f = lo;
        else f = hi;
    }
    *p = f;
    i = f - old;
    if (i != 0) {
        r = 1;
        if (i < 0) r = -1;
    } else {
        r = 0;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F5F8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F688);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F740);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012F7E0(void *arg0, s32 arg1, s32 arg2) {
    return func_0012B540(arg0, 0, arg1, arg2);
}

extern s32 func_0012B5F8(void *, s32, s32, u32, s32);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012F808(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    return func_0012B5F8(arg0, 0, arg1, arg2, arg3);
}


/* ER-081: by hand from the m2c draft (the third argument is a u8); names, types and offsets are Unconfirmed readings. */
s32 func_0012F838(void *arg0, s32 arg1, u32 arg2) {
    return func_0012B5F8(arg0, arg1, *(u8 *)((char *)arg0 + arg1 + 0x148), arg2, *(u8 *)((char *)arg0 + 0x146) & (1 << arg1));
}

/* ER-081: by hand from the m2c draft (the third argument is a u8); names, types and offsets are Unconfirmed readings. */
s32 func_0012F870(void *arg0, u32 arg1) {
    return func_0012B5F8(arg0, 0, *(u8 *)((char *)arg0 + 0x148), arg1, *(u8 *)((char *)arg0 + 0x146) & 1);
}

extern s32 func_0012F8A0(void **, s32, s32, f32 fparg0);
s32 func_0012B6B0(void **arg0, s32 arg1, s32 arg2, s32 arg3, f32 fparg0);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); returns func_0012B6B0's value, second parameter s32 (the
   file's extern said void and s8); names, types and offsets are Unconfirmed readings. */
s32 func_0012F8A0(void **arg0, s32 arg1, s32 arg2, f32 fparg0) {
    return func_0012B6B0(arg0, 0, arg1, arg2, fparg0);
}

extern s32 func_0012B890(void **, s32, s32, s32, s16, s32, f32 fparg0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012F8C8(void **arg0, s32 arg1, s32 arg2, s16 arg3, s32 arg4, f32 fparg0) {
    return func_0012B890(arg0, 0, arg1 & 0xFF, arg2, arg3, arg4, fparg0);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F900);

extern s32 func_0012BA30(void *, s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012F938(void *arg0, s32 arg1) {
    return func_0012BA30(arg0, 0, arg1);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012F958(void *arg0, s32 arg1, f32 fparg0) {
    return func_0012BAE0(arg0, 0, arg1 & 0xFF, fparg0);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0012F978(void *arg0, s32 arg1, s8 arg2) {
    *(s8 *)((char *)(((arg1 * 0xA0) + *(s32 *)((char *)arg0 + 0xE0))) + 0x8A) = arg2;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F990);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012F9C0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FA18);

extern s32 func_00165170(s16, s32, void *, f32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012FA68(void *arg0) {
    void *temp_6;

    temp_6 = *(void **)arg0;
    return func_00165170(*(s16 *)((char *)(*(void **)((char *)temp_6 + 0x11C)) + 0x1C4), (*(u8 *)((char *)arg0 + 0x151) << 0x10) | *(u8 *)((char *)arg0 + 0x150), temp_6 + 0x90, 2.5e+03f);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012FAA8(void *arg0) {
    void *temp_6;

    temp_6 = *(void **)arg0;
    return func_00165170(*(s16 *)((char *)(*(void **)((char *)temp_6 + 0x11C)) + 0x1C6), (*(u8 *)((char *)arg0 + 0x153) << 0x10) | *(u8 *)((char *)arg0 + 0x152), temp_6 + 0x90, 2.5e+03f);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FAE8);

extern void func_00165080(s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0012FB48(void *arg0, s32 arg1) {
    if ((u8) *(u8 *)((char *)arg0 + 0x79) < 0x50U) {
        func_00165080((s32) *(u8 *)((char *)arg0 + 0x79), arg1 != 0);
        *(u8 *)((char *)arg0 + 0x79) = 0x50U;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FB90);

extern s32 func_00120DA0(void **, s32);
extern s16 func_0012C208(void **, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012FBE8(void **arg0, s32 arg1) {
    if (func_00120DA0(arg0, arg1) != 0) {
        func_0012C208(arg0, arg1);
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FC38);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FCA0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FD10);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FD70);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FE10);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FE48);

extern void *func_0017D968(s32, s32, s32, s32, s32);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_0012FEB8(s8 arg0, s32 arg1, s32 arg2) {
    return func_0017D968((s32) arg0, arg1, arg2, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_0012FEE0);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_0012FF38(s32 arg0, s32 arg1) {
    void *temp_2;

    temp_2 = func_0017D968(0x7E, arg0, arg0, 0, 0);
    if (temp_2 != NULL) {
        *(s32 *)((char *)temp_2 + 0x10) = arg1;
    }
}

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

extern float func_0012E7B0(float, float);

/* ER-034. Returns s or -s by the sign of func_0012E7B0(angle from a0 + 0x90 to a1, wrapped into [-pi, pi), +0xA4)
   (a turn direction: reading). The sign as a nested `?:` gives the game's registers. Names and reading Unconfirmed. */
float func_001304C8(char *a0, char *a1, float s) {
    float v[4];
    float f;
    float d;
    float sign;

    func_001CA018(v, a1, a0 + 0x90);
    f = func_001E41B8(v[0], v[2]) + 6.28318548202514648438f;
    d = func_0012E7B0((f - (int)(f / 6.28318548202514648438f) * 6.28318548202514648438f) - 3.14159274101257324219f,
                      *(float *)(a0 + 0xA4));
    sign = (d == 0.0f) ? 0.0f : ((d < 0.0f) ? -1.0f : 1.0f);
    return (sign < 0.0f) ? -s : s;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130590);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130628);

extern s32 func_00111480(s32, s32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001306C0(void *arg0) {
    return func_00111480(*(s32 *)arg0 + 0x90, *(s32 *)((char *)arg0 + 0x130));
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001306E8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain), float return set by hand (the game passes func_001114E8's $f0 on); names, types and offsets are Unconfirmed readings. */
float func_00130758(void *arg0) {
    return func_001114E8(*(char **)arg0 + 0x90, *(float **)((char *)arg0 + 0x130));
}

extern s32 func_0012D138(void *, void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00130780(void *arg0) {
    return func_0012D138(arg0, arg0 + 0x138);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001307A0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001307E0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130848);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001308B8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00130930(void *arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f1;

    temp_f0 = *(f32 *)arg1;
    temp_f1 = *(f32 *)((char *)arg1 + 8);
    *(f32 *)arg0 = func_001E41B8(-*(f32 *)((char *)arg1 + 4), sqrtf((temp_f0 * temp_f0) + (temp_f1 * temp_f1)));
    *(f32 *)((char *)arg0 + 4) = func_001E41B8(*(f32 *)arg1, *(f32 *)((char *)arg1 + 8));
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001309C0(void *arg0, void *arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f1;

    temp_f0 = *(f32 *)arg1;
    temp_f1 = *(f32 *)((char *)arg1 + 8);
    *(f32 *)arg0 = func_001E41B8(-*(f32 *)((char *)arg1 + 4), sqrtf((temp_f0 * temp_f0) + (temp_f1 * temp_f1)));
    temp_f0_2 = func_001E41B8(*(f32 *)arg1, *(f32 *)((char *)arg1 + 8)) + 6.2831855f;
    *(f32 *)((char *)arg0 + 4) = (f32) ((temp_f0_2 - ((f32) (s32) (temp_f0_2 / 6.2831855f) * 6.2831855f)) - 3.1415927f);
}

extern float func_0012D470(char *a0, float *d, float max);

/* ER-081: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
float func_00130A88(char *arg0, float *arg1, float fparg0) {
    float r;
    char *p;

    r = func_0012D470(arg0, arg1, fparg0);
    p = *(char **)arg0 + 0x90;
    func_001C9FE8(p, p, arg0 + 0x90);
    return r;
}

extern float func_0012D7C0(char *a0, float a, float max);

/* ER-081: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
float func_00130AD0(char *arg0, float fparg0, float fparg1) {
    float r;
    char *p;

    r = func_0012D7C0(arg0, fparg0, fparg1);
    p = *(char **)arg0 + 0x90;
    func_001C9FE8(p, p, arg0 + 0x90);
    return r;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130B18);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
f32 func_00130B60(void *arg0, f32 fparg0) {
    f32 *temp_17;
    f32 temp_f0;
    f32 temp_f1;
    f32 temp_f20;
    void *temp_4;

    temp_17 = arg0 + 0x90;
    func_001CA048(temp_17, temp_17, fparg0);
    temp_f1 = *(f32 *)((char *)arg0 + 0x90);
    temp_f0 = *(f32 *)((char *)arg0 + 0x98);
    temp_f20 = sqrtf((temp_f1 * temp_f1) + (temp_f0 * temp_f0));
    temp_4 = *(s32 *)arg0 + 0x90;
    func_001C9FE8(temp_4, temp_4, temp_17);
    return temp_f20;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130BF8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130C80);

extern s32 func_0012DAF8(void *, s32, f32);

/* ER-081: drafted with m2c (tools/m2c_bulk.py --plain, pass-through retry); the float in $f12 passes through to
   func_0012DAF8 as its callers in text_001388E8 / text_0013A410 / text_0013B748 pass one (recipes 2.16).
   Names, types and offsets are Unconfirmed readings. */
s32 func_00130CE0(char *arg0, f32 fparg0) {
    return func_0012DAF8(arg0, *(s32 *)(arg0 + 0x130), fparg0);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130D00);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130D68);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00130DF0);

/* ER-035. Wraps *a0 into [-pi, pi], then clamps it to [lo, hi] (a reading). Names and types Unconfirmed. */
void func_00130F50(float *a0, float lo, float hi) {
    float f = *a0;

    if (3.14159274f < f) {
        do {
            f -= 6.28318548f;
        } while (3.14159274f < f);
    }
    if (f < -3.14159274f) {
        do {
            f += 6.28318548f;
        } while (f < -3.14159274f);
    }
    if (f < lo) f = lo;
    if (hi < f) f = hi;
    *a0 = f;
}

extern s32 func_0012E3C8(char *, void *, s32);

/* ER-081: by hand; $a0 / $a1 pass through to func_0012E3C8 (recipes 2.10), its value is returned (jal and a frame,
   recipes 2.1). Names and types are Unconfirmed readings. */
s32 func_00130FE0(char *arg0, void *arg1) {
    return func_0012E3C8(arg0, arg1, 1);
}

/* ER-081: by hand; as func_00130FE0 with 0. Names and types are Unconfirmed readings. */
s32 func_00131000(char *arg0, void *arg1) {
    return func_0012E3C8(arg0, arg1, 0);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131020);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00131088(void *arg0) {
    return *(s32 *)((char *)arg0 + 0xB8);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00131090(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    u32 temp_2;

    temp_2 = func_0012D138(arg0, arg0 + 0x138);
    switch (temp_2) {
    case 0:
        return arg1;
    case 1:
        return arg2;
    case 2:
        return arg3;
    case 3:
        return arg4;
    case 4:
        return arg5;
    default:
        return 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131140);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131208);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001312D0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001313A8);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_001313F0(void *arg0) {
    s32 var_3;

    var_3 = 1;
    if (!(*(f32 *)((char *)arg0 + 0x54) <= *(f32 *)((char *)(*(void **)arg0) + 0x94))) {
        var_3 = 0;
    }
    return var_3;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
u16 func_00131418(void *arg0) {
    u16 var_2;

    var_2 = 0;
    if (arg0 != NULL) {
        var_2 = *(u16 *)((char *)arg0 + 4);
    }
    return var_2;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00131430(void *arg0, s32 arg1) {
    u16 var_2;

    var_2 = 0;
    if (arg0 != NULL) {
        var_2 = *(u16 *)((char *)arg0 + 4);
    }
    return var_2 == arg1;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131448);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131470);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00131498(void) {
    return func_001E9C70() % 101;
}

extern f32 func_001E3EC8(f32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
f32 func_001314C8(s32 arg0, s32 arg1) {
    return func_001E3EC8(((f32) arg0 * 1.5707964f) / (f32) arg1);
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
f32 func_00131508(s32 arg0, s32 arg1) {
    return func_001E3EC8(((f32) arg0 * 3.1415927f) / (f32) arg1);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131548);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001315A8);

/* ER-026. Wraps the angle a + b into [-pi, pi] (a reading); names and types Unconfirmed. gcc pads each short loop to 6 instructions itself. */
float func_00131608(float a, float b) {
    float f = a + b;

    if (3.14159274f < f) {
        do {
            f -= 6.28318548f;
        } while (3.14159274f < f);
    }
    if (f < -3.14159274f) {
        do {
            f += 6.28318548f;
        } while (f < -3.14159274f);
    }
    return f;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00131678(void *arg0, s8 arg1) {
    *(s8 *)((char *)arg0 + 0x5B) = arg1;
    *(s16 *)((char *)arg0 + 0x6E) = 0;
    *(s16 *)((char *)arg0 + 0x6C) = 0;
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00131688(void **arg0, s16 arg1) {
    return arg1 >= *(s16 *)((char *)(*(void **)((char *)(*arg0) + 0x11C)) + 0x19E);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001316A8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001318A0);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001318D8);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131930);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00131D40);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00132020(void *arg0) {
    (*(s32 (**)())((char *)arg0 + 0xB8))();
    return *(s32 *)((char *)arg0 + 0xBC) != (s32) *(s32 (**)())((char *)arg0 + 0xB8);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132060);

void func_001320A8(void) {
}

void func_001320B0(void) {
}

extern void func_00132108(void *);

/* ER-081: by hand from the m2c draft (a u16 parameter); names, types and offsets are Unconfirmed readings. */
void func_001320B8(void *arg0, u16 arg1) {
    void (*fn)(void *, s32);

    if (arg1 & 0x10) {
        func_00132108(arg0);
    }
    fn = *(void (**)(void *, s32))((char *)arg0 + 0x164);
    if (fn != NULL) {
        fn(arg0, arg1);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132108);

extern void func_00131930(void *, s32, u16);

/* ER-081: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
void func_001321D8(void *arg0) {
    func_00131930(arg0, *(s32 *)arg0, *(u16 *)((char *)arg0 + 4));
}

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001321E8(void *arg0) {
    *(s16 *)((char *)arg0 + 0x64) = 0;
    *(u8 *)((char *)arg0 + 0x5A) = (u8) (*(u8 *)((char *)arg0 + 0x5A) | 0x80);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132200);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00132268(void *arg0) {
    *(s16 *)((char *)arg0 + 0x64) = 0x7D00;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132278);

extern void func_00132A08(void);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed. */
void func_00132340(void) {
    func_00132A08();
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132348);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132480);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_001325E8);

extern s32 func_00132938(void *);
extern s32 func_00131000(char *, void *);
extern void func_001325E8(s8 *, void *);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00132890(s8 *arg0) {
    if (func_00132938(arg0) == 0) {
        return 0;
    }
    func_00131000(arg0, func_001325E8);
    return 1;
}

extern void func_00132278();

/* ER-081: by hand from the m2c draft; names, types and offsets are Unconfirmed readings. */
s32 func_001328D8(s8 *arg0, void *arg1) {
    if (((*(u32 *)((char *)arg1 + 0xC) >> 8) & 3) == 2) {
        return 0;
    }
    if (func_00132938(arg0) == 0) {
        return 0;
    }
    func_00131000(arg0, func_00132278);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132938);

void func_001321D8(void *arg0);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001329F0(void *arg0, void *arg1) {
    *(s16 *)((char *)arg0 + 4) = (s16) *(u8 *)((char *)(*(void **)((char *)arg1 + 0x11C)) + 0xA);
    func_001321D8(arg0);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132A08);

extern s32 func_00131088(void *arg0);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
s32 func_00132CA0(void *arg0) {
    *(s16 *)((char *)arg0 + 0x64) = 0x7530;
    return func_00131088(arg0);
}

void func_001336E0(s8 *arg0);
void func_001336D8(void *arg0);
void func_00132CF8(s8 *arg0);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00132CC0(s8 *arg0) {
    *(void (**)(s8 *))((char *)arg0 + 0xC4) = func_001336E0;
    *(void (**)(s8 *))((char *)arg0 + 0xC8) = func_001336E0;
    *(void (**)(void *))((char *)arg0 + 0xC0) = func_001336D8;
    *(f32 *)((char *)arg0 + 0xB4) = (f32) 0.2f;
    func_00131000(arg0, func_00132CF8);
}

INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354570);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354580);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354598);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003545A8);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003545B8);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003545C8);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003545D8);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003545E8);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_003545F8);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354608);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354618);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354628);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354638);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354648);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354658);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354668);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354678);
INCLUDE_RODATA("asm/nonmatchings/text_001000C0", D_00354688);
extern s32 func_00130780(void *arg0);
extern void func_00133458(s8 *, void *);
extern void func_00132F80(void **, s8 *);
extern void func_00133290(void);
extern void func_00132E80(void);
void func_00133098(char *a0, char *a1);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00132CF8(s8 *arg0) {
    s32 temp_17;
    u32 temp_2;

    temp_17 = func_001E9C70() % 100;
    temp_2 = func_00130780(arg0);
    switch (temp_2) {
    case 0:
        if (temp_17 < 0x1F) {
            func_00131000(arg0, func_00133458);
            return;
        }
        func_00131000(arg0, func_00132F80);
        return;
    case 1:
        if (temp_17 < 0x15) {
            func_00131000(arg0, func_00132F80);
            return;
        }
        if (temp_17 < 0x33) {
            func_00131000(arg0, func_00133458);
            return;
        }
        func_00131000(arg0, func_00133290);
        return;
    case 2:
        func_00131000(arg0, func_00133098);
        return;
    case 3:
        func_00131000(arg0, func_00132E80);
        return;
    case 4:
        func_0012F8A0((void **) arg0, 0, 1, 0.1f);
        return;
    default:
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132E80);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00132F80);

extern float func_0012CE18(char *, char *);
extern float func_00130D00(char *, float);
extern void func_0012F900(char *, int, int, int, int, float);
extern void func_0012D3B8(char *, float, float);
extern void func_00131140(char *, void *, void *, void *, void *, void *);

/* ER-035. A state handler (a reading): on its first frame (+0x6E == 0) sets step 0.08 and limit 1.5; leaves through
   func_0012F900 (state 0, 8) when func_00130D00 gives more than 20 degrees; otherwise picks state 10 or 11 from the
   angle func_0012CE18 gives (+-2 degrees of hysteresis), moves +0x30 by +-step within +-limit, counts frames in
   +0x14, and passes +0x30 to func_0012D3B8. Names, types and the state reading Unconfirmed. */
void func_00133098(char *a0, char *a1) {
    float ang;
    float s;
    float lim;
    float f;
    int old;

    if (*(s16 *)(a0 + 0x6E) == 0) {
        *(float *)(a0 + 0x34) = 0.08f;
        *(float *)(a0 + 0x38) = 1.5f;
        *(int *)(a0 + 0x10) = -1;
        *(s16 *)(a0 + 0x6E) += 1;
        *(float *)(a0 + 0x30) = 0.0f;
        *(int *)(a0 + 0x14) = 0;
    }
    ang = func_0012CE18(a0, a1 + 0x90);
    if (0.34906587f < func_00130D00(a0, 15.0f)) {
        func_0012F900(a0, 0, *(s16 *)(a0 + 0x6C), 8, 1, 0.1f);
        return;
    }
    old = *(int *)(a0 + 0x10);
    if (old == 10) {
        s = -1.0f;
        if (ang <= 0.034906585f) s = 1.0f;
    } else {
        s = 1.0f;
        if (-0.034906585f <= ang) s = -1.0f;
    }
    if (1.0f <= s) *(int *)(a0 + 0x10) = 10;
    else *(int *)(a0 + 0x10) = 11;
    f = *(float *)(a0 + 0x30) + s * *(float *)(a0 + 0x34);
    *(float *)(a0 + 0x30) = f;
    lim = *(float *)(a0 + 0x38);
    if (f < -lim) {
        f = -lim;
    } else if (lim < f) {
        f = lim;
    }
    *(float *)(a0 + 0x30) = f;
    if (*(int *)(a0 + 0x10) != old) *(int *)(a0 + 0x14) = 0;
    *(int *)(a0 + 0x14) += 1;
    func_0012F900(a0, *(int *)(a0 + 0x10), *(int *)(a0 + 0x14), 10, 1, 0.1f);
    func_0012D3B8(a1, *(float *)(a0 + 0x30), 1.0f);
    func_00131140(a0, func_00133290, func_00133290, NULL, func_00132E80, func_00132CF8);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133290);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133458);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133648);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed.
   ER-081: passes $a0 on to func_0012B238 (recipes 2.13); same code. */
void func_001336D8(void *arg0) {
    func_0012B238(arg0);
}

extern s32 func_0014FF38(void *arg0, s32 arg1);
void func_00133098(char *a0, char *a1);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001336E0(s8 *arg0) {
    *(f32 *)((char *)arg0 + 0xB4) = 1.0f;
    if (func_0014FF38(arg0, 3) != 0) {
        *(f32 *)((char *)arg0 + 0xB4) = (f32) 0.2f;
        func_00131140(arg0, func_00132CF8, func_00132CF8, func_00133098, func_00132E80, func_00132CF8);
    }
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133750);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133868);

extern void func_001501D8(void *arg0);
extern void func_00133F28(void *, s32);
extern void func_00133A60(void **, void *);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001339F8(s8 *arg0) {
    *(s8 *)((char *)(*(void **)((char *)(*(void **)arg0) + 0xE0)) + 0xD08) = 1;
    *(s8 *)((char *)(*(void **)((char *)(*(void **)arg0) + 0xE0)) + 0xDA8) = 1;
    *(s8 *)((char *)(*(void **)((char *)(*(void **)arg0) + 0xE0)) + 0xEE8) = 1;
    *(s8 *)((char *)(*(void **)((char *)(*(void **)arg0) + 0xE0)) + 0xE48) = 1;
    *(void (**)(void *))((char *)arg0 + 0xE0) = func_001501D8;
    *(void (**)(void *, s32))((char *)arg0 + 0xCC) = func_00133F28;
    *(u8 *)((char *)arg0 + 0x5A) = (u8) (*(u8 *)((char *)arg0 + 0x5A) | 8);
    func_00131000(arg0, func_00133A60);
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133A60);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133F28);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00133FC0);

extern void func_00134070(s8 *);

/* ER-112: drafted with m2c (tools/m2c_bulk.py --plain); one parameter (the file's extern said two; $a1 is not read);
   names, types and offsets are Unconfirmed readings. */
void func_00134070(s8 *arg0) {
    func_0012F900((char *)arg0, 0, (s32) *(s16 *)((char *)arg0 + 0x6C), 0x1E, 1, 0.1f);
}

extern void func_00134090(void);

/* ER-049: clone of func_0014A940, placed by tools/clones.py (callee name is the only change). Tail call (`j`); reading Unconfirmed. */
void func_00134088(void) {
    func_00134090();
}

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00134090);

INCLUDE_ASM("asm/nonmatchings/text_001000C0", func_00134160);


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00134298(void *arg0) {
    func_0012B218(arg0, 1);
    func_0012B218(arg0, 2);
}

extern s32 func_0014FF58(void **arg0, s32 arg1);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_001342C8(void **arg0) {
    if (func_0014FF58(arg0, 3) != 0) {
        func_00131000((s8 *) arg0, func_00134090);
    }
}


/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
void func_00134310(s8 *arg0) {
    if (func_00131088(arg0) == func_00134070) {
        func_00131000(arg0, func_00134090);
    }
}

float func_0012CD08(void *a0, int idx, void *a2);
extern f32 func_001315A8(s32, f32, f32);

/* ER-060: drafted with m2c (tools/m2c_bulk.py --plain); names, types and offsets are Unconfirmed readings. */
f32 func_00134358(void *arg0, f32 fparg0) {
    f32 temp_f0;
    void *temp_16;
    void *temp_6;

    temp_6 = *(void **)((char *)arg0 + 0x2C);
    temp_16 = *(void **)arg0;
    if (temp_6 == NULL) {
        return 99999.0f;
    }
    temp_f0 = func_0012CD08(temp_16, 0, temp_6) + 6.2831855f;
    return func_001315A8(*(s32 *)((char *)temp_16 + 0xE0) + 0xF4, (temp_f0 - ((f32) (s32) (temp_f0 / 6.2831855f) * 6.2831855f)) - 3.1415927f, fparg0);
}
