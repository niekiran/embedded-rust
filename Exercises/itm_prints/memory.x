
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
