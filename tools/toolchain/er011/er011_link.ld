/* ER-011: link test only. Callee addresses come from their names (ER-002's reading of the ELF); the two
   small-data variables are placed at _gp + the game's gp offsets, so those two words only test ld's arithmetic. */
_gp = 0x00360B70;
func_0012B540 = 0x0012B540;
func_001139C8 = 0x001139C8;
func_0012D318 = 0x0012D318;
D_gp_5328 = _gp - 0x5328;
D_gp_52D0 = _gp - 0x52D0;
SECTIONS
{
  .text 0x00200000 : { *(.text) }
  .data : { *(.data) }
  .bss : { *(.bss) }
  /DISCARD/ : { *(.mdebug) *(.reginfo) }
}
