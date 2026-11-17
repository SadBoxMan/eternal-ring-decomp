# ER-003: ELF layout pieces of SLUS_200.15 that carry no game bytes (hand-written; docs/build.md 3).
#
# 1. The empty VU sections .vutext and .vubss: SCE's linker script keeps them in the output
#    (docs/toolchain.md 2.2); without an input section of that name ee-ld leaves them out.
# 2. Section alignments. ee-ld sets an output section's sh_addralign to the largest alignment among its
#    input sections. The game ELF has .text 64, .data 128, .vudata 16, .rodata 16, .sdata 16, .sbss 16 and
#    .bss 128; splat's objects ask for less (8, 8, 4, 8, 8, 1, 1). These empty input sections, placed
#    first in each output section by tools/gen_ld.py, give the original values. Which original objects
#    asked for them (SDK libraries, aligned DMA buffers, crt0) is not known (Unconfirmed); they add no bytes,
#    since each output section already starts on that boundary.

.section .vutext,"ax"
.section .vubss,"aw",@nobits

.section .text
.align 6
.section .data
.align 7
.section .vudata,"aw"
.align 4
.section .rodata
.align 4
.section .sdata,"aw"
.align 4
.section .sbss,"aw",@nobits
.align 4
.section .bss
.align 7
