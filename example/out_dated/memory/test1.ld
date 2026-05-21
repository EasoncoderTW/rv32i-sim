ENTRY(main)

MEMORY
{
  IM (rx) : ORIGIN = 0x10000000, LENGTH = 1K
  DM (rwx) : ORIGIN = 0x10000400, LENGTH = 1K
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
  } > DM

  .data : {
    *(.data)
    *(.data.*)
  } > DM

  .bss : {
    *(.bss)
    *(.bss.*)
    *(COMMON)
  } > DM

  /* 堆疊區域（可選） */
  . = ALIGN(8);
  _stack_top = . + 4K;
  PROVIDE(_sp = _stack_top);
}