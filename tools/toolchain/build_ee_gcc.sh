#!/bin/bash
# ER-002: build ee-gcc 2.9-ee-991111 (SSXModding/ps2-ee-toolchain commit b595ded, GPL source) natively
# on Windows under MSYS2, host x86_64-pc-msys (LP64), target "ee" = mips64r5900-sky-elf.
# Pattern taken from Project Twinblade's tools/build_old_gcc.sh (GCC 2.7.2 under MSYS2).
#
# Source must be cloned with LF line endings (this machine has core.autocrlf=true globally):
#   git -c core.autocrlf=false clone https://github.com/SSXModding/ps2-ee-toolchain.git /c/rs/toolchain/src-lf
# (Two case-colliding files, ee/libgloss/sparc/crt0.[sS] and ee/readline/{CHANGELOG,ChangeLog}, cannot
#  both exist on NTFS; neither is used by this build.)
#
# Run from an MSYS2 MSYS shell:  /c/rs/toolchain/build_ee_gcc.sh
# Output: /c/rs/toolchain/ee-gcc-2.9-991111/{gcc,cpp,cc1}.exe (+ msys-2.0.dll)
set -e
export PATH="/usr/bin:$PATH"
unset CC CXX

ROOT=/c/rs/toolchain
SRC=$ROOT/src-lf/ee
BUILD=$ROOT/b-gcc
OUT=$ROOT/ee-gcc-2.9-991111
LOG=$ROOT/build_ee_gcc.log

# Host compiler flags: GCC 15 rejects much 1990s C by default; these only relax diagnostics.
CFLAGS="-O0 -g0 -std=gnu89 -w -fcommon -Wno-int-conversion -Wno-implicit-function-declaration -Wno-implicit-int -Wno-return-mismatch -Wno-incompatible-pointer-types"
export CFLAGS

cd "$SRC"
# 1. config.sub: accept x86_64 CPUs and the msys OS (idempotent).
if ! grep -q 'ER-002 x86_64' config.sub; then
    sed -i 's/^\tmips64r5900-\* | mips64r5900el-\*) # CYGNUS LOCAL gavin\/r5900$/\tx86_64-*) # ER-002 x86_64\n\t\t;;\n&/' config.sub
    sed -i 's/      | -cygwin\* | -pe\* /      | -cygwin* | -msys* | -pe* /' config.sub
fi
grep -q 'ER-002 x86_64' config.sub
grep -q -- '-msys\*' config.sub

# 2. gcc/configure: an LP64 MSYS host entry at the top of the main machine case
#    (64-bit HOST_WIDE_INT, which the r5900 target requires: target_requires_64bit_host_wide_int).
if ! grep -q 'ER-002 msys host' gcc/configure; then
    awk '!done && /^\t# Support site-specific machine types\.$/ {print "\tx86_64-*-msys*) # ER-002 msys host"; print "\t\txm_file=xm-msys64.h"; print "\t\tfixincludes=Makefile.in"; print "\t\t;;"; done=1}
         {print}' gcc/configure > gcc/configure.tmp
    mv gcc/configure.tmp gcc/configure
    chmod +x gcc/configure
fi
grep -q 'ER-002 msys host' gcc/configure

cat > gcc/config/xm-msys64.h <<'EOF'
/* ER-002: host description for GCC running under MSYS2 (x86_64, LP64). */
#define HOST_BITS_PER_CHAR    8
#define HOST_BITS_PER_SHORT  16
#define HOST_BITS_PER_INT    32
#define HOST_BITS_PER_LONG   64
#define HOST_BITS_PER_LONGLONG 64
#define HOST_BITS_PER_WIDE_INT 64
#define HOST_WIDE_INT long
#define FALSE 0
#define TRUE  1
#define SUCCESS_EXIT_CODE 0
#define FATAL_EXIT_CODE 33
#define HAVE_VPRINTF
#define HAVE_STRERROR
#define POSIX
#define NO_STAB_H
#define BSTRING
#include "tm.h"
EOF

# 3. The tree ships no generated c-parse.c / cexp.c; MSYS2's bison 3.8 rejects the 1999 grammar.
#    Build the tree's own bison 1.25 and use it, with the grammar unchanged.
BISONB=$ROOT/b-bison
rm -rf "$BISONB"; mkdir -p "$BISONB"; cd "$BISONB"
"$SRC/bison/configure" --host=x86_64-pc-msys --build=x86_64-pc-msys > "$ROOT/build_bison.log" 2>&1
make CFLAGS="$CFLAGS" >> "$ROOT/build_bison.log" 2>&1
export BISON_SIMPLE="$SRC/bison/bison.simple" BISON_HAIRY="$SRC/bison/bison.hairy"
"$BISONB/bison.exe" --version

# 4. libiberty, as a sibling of the gcc build dir (gcc links ../libiberty/libiberty.a).
rm -rf "$ROOT/libiberty"; mkdir -p "$ROOT/libiberty"; cd "$ROOT/libiberty"
"$SRC/libiberty/configure" --host=x86_64-pc-msys --build=x86_64-pc-msys --target=ee > "$ROOT/build_libiberty.log" 2>&1
make -j4 CFLAGS="$CFLAGS" >> "$ROOT/build_libiberty.log" 2>&1
test -f libiberty.a

# 5. configure and build cpp, cc1 and the driver only (-j4, project rule).
rm -rf "$BUILD"; mkdir -p "$BUILD"; cd "$BUILD"
"$SRC/gcc/configure" --target=ee --host=x86_64-pc-msys --build=x86_64-pc-msys \
    --prefix=$ROOT/ee-prefix --with-gnu-as --with-gnu-ld --disable-nls > "$LOG" 2>&1
make -j4 cpp cc1 xgcc CFLAGS="$CFLAGS" LANGUAGES=c BISON="$BISONB/bison.exe" >> "$LOG" 2>&1

mkdir -p "$OUT"
cp cpp.exe cc1.exe "$OUT/"
cp xgcc.exe "$OUT/gcc.exe"
cp /usr/bin/msys-2.0.dll "$OUT/"
echo "Done: $OUT"
