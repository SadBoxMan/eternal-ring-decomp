#ifndef COMMON_H
#define COMMON_H

/* ER-003: shared header for the split. Sizes for ee-gcc 2.9-ee-991111 (EABI64): int 32 bits,
   long long 64 bits, pointers 32 bits. The driver defines __LONG_MAX__ as a 64-bit value, so `long` is
   not used for the fixed-size types below. */

#include "include_asm.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif /* COMMON_H */
