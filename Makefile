# ER-003: byte-identical build of Eternal Ring (USA) SLUS_200.15 from the splat split.
# Run from the repo root with MSYS2 make (PowerShell: C:\msys64\usr\bin\make.exe, or MSYS2 bash). See docs/build.md.
#
#   make split     extract the load image, run splat, write the non-load ELF sections
#   make           build build/SLUS_200.15.elf (stripped, the comparison target)
#   make check     SHA-1 of the built ELF against the original (prints "build/SLUS_200.15.elf: OK")
#   make regress   clean split + clean build + SHA-1 check + per-function byte check (tools/regress.py)
#   make clean     remove build/;  make distclean also removes the split output (asm/, linker/, baserom/)

.SUFFIXES:
.SECONDARY:
.DELETE_ON_ERROR:
SHELL := /usr/bin/bash
.SHELLFLAGS := -eo pipefail -c

# --- toolchain (built by tools/toolchain/*.sh; docs/toolchain.md 4, docs/build.md) -----------------------
TOOLCHAIN ?= C:/rs/toolchain
# ER-034: -r1 = ee-gcc with tools/toolchain/patches/ee-gcc_r5900_fdiv_nops.patch (ruling docs/decomp_recipes.md
# 3.7: div.s / sqrt.s as "nop; nop; div.s", never in a delay slot). The unpatched ee-gcc-2.9-991111 stays as a fallback.
GCC_DIR   := $(TOOLCHAIN)/ee-gcc-2.9-991111-r1
# ER-070: -r4 = ee-as with the four ee-as_*.patch files in tools/toolchain/patches/ (rulings docs/decomp_recipes.md
# 3.4, 3.5, 3.8, 3.12). ER-034's -r3 (first three), ER-035's -r2 (first two), ER-026's -r1 (first only) and the
# unpatched ER-011 build (ee-binutils-2.9-991111) stay as fallbacks.
BIN_DIR   ?= $(TOOLCHAIN)/ee-binutils-2.9-991111-r4
CC        := $(GCC_DIR)/gcc.exe
AS        := $(BIN_DIR)/ee-as.exe
LD        := $(BIN_DIR)/ee-ld.exe
PYTHON    ?= .venv/Scripts/python.exe
PY        := $(PYTHON) -I

# --- inputs and outputs ---------------------------------------------------------------------------------
BASENAME  := SLUS_200.15
BASEELF   ?= disc/extract/$(BASENAME)
ROM       := baserom/$(BASENAME).rom
YAML      := slus_200.15.yaml
BUILD     := build
TARGET    := $(BUILD)/$(BASENAME).elf
FULL_ELF  := $(BUILD)/$(BASENAME).syms.elf
LD_SCRIPT := $(BUILD)/$(BASENAME).ld
MAP       := $(BUILD)/$(BASENAME).map
SPLAT_LD  := linker/$(BASENAME).splat.ld
GP_INC    := $(BUILD)/include/gp_externs.inc
CRT0_O    := $(BUILD)/asm/text_00100000.o

# --- flags (docs/decomp_recipes.md 1.1) -------------------------------------------------------------------
# ER-035: -fno-strict-aliasing (ruling docs/decomp_recipes.md 3.6): the game reloads memory across stores of other
# types, which this compiler's -O2 default (strict aliasing on) does not.
# ER-069: -fno-edge-lcm (ruling docs/decomp_recipes.md 3.11): GCSE without the edge-based LCM (Cygnus, 27 Sep 1999,
# on by default), which removes partially redundant loads at joins; the game reloads there (docs/toolchain.md 5).
CFLAGS    := -O2 -G8 -fno-strict-aliasing -fno-edge-lcm
CPPFLAGS  := -Iinclude
ASFLAGS   := -G8 -O2 -mabi=eabi -Iinclude -I$(BUILD)/include
LDFLAGS   := -G8 -T $(LD_SCRIPT)

# --- objects (asm/ is splat output; nonmatchings are pulled in by INCLUDE_ASM, never assembled alone) ----
# (pad_* subsegments are alignment fill that tools/gen_ld.py leaves out of the link: not assembled)
ASM_S     := $(shell find asm -name '*.s' -not -path 'asm/nonmatchings/*' -not -name 'pad_*' 2>/dev/null | sort)
C_SRC     := $(shell find src -name '*.c' 2>/dev/null | sort)
SRC_S     := $(shell find src -name '*.s' 2>/dev/null | sort)
ASM_O     := $(ASM_S:%.s=$(BUILD)/%.o)
C_O       := $(C_SRC:%.c=$(BUILD)/%.o)
SRC_S_O   := $(SRC_S:%.s=$(BUILD)/%.o)
# crt0 first, and the objects before -T: ld keeps only the first .reginfo it opens (link-once), and it
# opens files in command-line order (a -T script's own file names would come first). tools/gen_ld.py.
ALL_O     := $(CRT0_O) $(filter-out $(CRT0_O),$(ASM_O) $(C_O) $(SRC_S_O))

.PHONY: all split check regress clean distclean
all: $(TARGET)

# --- split ------------------------------------------------------------------------------------------------
$(ROM): $(BASEELF) tools/extract_rom.py tools/regress_baseline.txt
	$(PY) tools/extract_rom.py $(BASEELF) $@

split: $(ROM)
	$(PY) tools/split.py $(YAML)
	$(PY) tools/gen_elf_extras.py $(BASEELF) asm/elf/elf_extras.s

# --- build --------------------------------------------------------------------------------------------------
ifeq ($(ASM_S),)
$(TARGET) $(FULL_ELF):
	@echo "No split output: run 'make split' first." >&2; exit 1
else

$(GP_INC): $(ASM_S) tools/gen_gp_externs.py
	$(PY) tools/gen_gp_externs.py asm $@

$(BUILD)/src/%.s: src/%.c $(wildcard include/*.h)
	@mkdir -p $(@D)
	$(CC) -B$(GCC_DIR)/ $(CFLAGS) $(CPPFLAGS) -S $< -o $@

$(BUILD)/src/%.o: $(BUILD)/src/%.s $(GP_INC) include/macro.inc include/labels.inc
	$(AS) $(ASFLAGS) -o $@ $<

# hand-written asm in src/ (src/elf_layout.s); a pattern rule more specific than the C one above
$(SRC_S_O): $(BUILD)/src/%.o: src/%.s
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD)/asm/%.o: asm/%.s $(GP_INC) include/macro.inc
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) -o $@ $<

$(LD_SCRIPT): $(SPLAT_LD) tools/gen_ld.py
	$(PY) tools/gen_ld.py $(SPLAT_LD) $@ linker/undefined_funcs_auto.txt linker/undefined_syms_auto.txt

# Two links of the same objects: the stripped ELF is the comparison target; the one with symbols feeds
# the per-function check and later tools.
$(FULL_ELF): $(ALL_O) $(LD_SCRIPT)
	$(LD) -o $@ $(ALL_O) $(LDFLAGS) -Map $(MAP)

$(TARGET): $(ALL_O) $(LD_SCRIPT)
	$(LD) -s -o $@ $(ALL_O) $(LDFLAGS)

endif

# --- checks -------------------------------------------------------------------------------------------------
check: $(TARGET)
	@want=$$(awk '$$1=="elf"{print $$2}' tools/regress_baseline.txt); \
	 echo "$$want *$(TARGET)" | sha1sum -c -

regress:
	$(PY) tools/regress.py

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -rf asm linker baserom assets

-include $(BUILD)/src/*.asmproc.d
