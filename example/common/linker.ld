ENTRY(_start)

MEMORY
{
  IM (rx) : ORIGIN = 0x40000000, LENGTH = 256K
  DM (rwx) : ORIGIN = 0x40020000, LENGTH = 256K
}

SECTIONS
{
  . = ORIGIN(IM);

  .text : {
    *(.text)
    *(.text.*)
  } > IM

  .rodata : {
    *(.rodata)
    *(.rodata.*)
  } > IM

  .data : {
    *(.data)
    *(.data.*)
  } > DM

  .bss : {
    *(.bss)
    *(.bss.*)
    *(COMMON)
  } > DM

  . = ALIGN(8);
  end = .;
  _end = .;
  PROVIDE(end = .);
  PROVIDE(_end = .);

  /* stack top = DM end */
  _stack_top = ORIGIN(DM) + LENGTH(DM);
  PROVIDE(_sp = _stack_top);
}