ENTRY(reset_handler)


/* Define memory regions */
MEMORY
{
    FLASH (rx)    : ORIGIN = 0x08000000, LENGTH = 256K
    RAM (rwx)     : ORIGIN = 0x20000000, LENGTH = 40K
  /*  EEPROM (rwx)  : ORIGIN = 0x08080000, LENGTH = 4K  */
    CCMRAM (rwx)  : ORIGIN = 0x10000000, LENGTH = 8K  
  /*  BATTRAM (rw)  : ORIGIN = 0x40024000, LENGTH =    4K       */
}


_start_of_stack = ORIGIN(RAM) + LENGTH(RAM);
_min_stack_size = 0x400;
_min_heap_size =  0x400;

/*
 * SECTIONS
 * SECTIONS command in a linker script specifies where each section 
 * (e.g., .text, .data, .bss) should be placed in memory. 
*/

SECTIONS
{
    /*
    1. Defines where each program section is placed in the microcontroller's memory.

    2. The linker assigns addresses based on the SECTIONS command, 
         creating an ELF file with exact memory locations.
        For example , if FLASH starts at 0x08000000 and the .text section is placed in FLASH, 
        the linker will assign address starting frm 0x08000000 for the .text section.

    3. The ELF file guides the programming tool to correctly place the sections in the microcontroller's memory, 
          ensuring proper execution of the code

    */

    .text :
    {
        /* here you should collect all executable code */
        . = ORIGIN(FLASH);
        LONG(_start_of_stack);
        KEEP(*(.isr_vector));
        *(.text)
        *(.text*)  
         . = ALIGN(4); 
    } > FLASH


  /* Constant data will go into FLASH */
   .rodata : 
   {
         . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
         . = ALIGN(4);
   } > FLASH



   /* Initialized data will be in FLASH and it will also be in RAM during runtime*/
   .data :
   {
        _sidata = LOADADDR(.data); /* This returns the FLASH (LMA) */
         . = ALIGN(4);
        _sdata = .; /* start of data section in RAM (VMA) */
        *(.data)
        *(.data*)
         . = ALIGN(4);
        _edata = .;
   } > RAM AT> FLASH


/* un-initialized data will be RAM */
   .bss :
   {
         . = ALIGN(4);
        _sbss = .;  /* start of bss section in RAM (VMA) */
        *(.bss)
        *(.bss*)
         . = ALIGN(4);
        _ebss = .;
   } > RAM


   .ram_usage_check :
   {
      . = ALIGN(8);
      . = . + _min_stack_size;
      . = . + _min_heap_size;
      . = ALIGN(8);
   } > RAM
   
}

INCLUDE "device_STM32F303.x"