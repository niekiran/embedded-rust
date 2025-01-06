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

PROVIDE(mem_manage_handler = default_handler);
PROVIDE(busfault_handler = default_handler);
PROVIDE(usagefault_handler = default_handler);
PROVIDE(SVCall_Handler = default_handler);
PROVIDE(DebugMon_Handler = default_handler);
PROVIDE(PendSV_Handler = default_handler);
PROVIDE(SysTick_Handler = default_handler);
PROVIDE(WWDG_Handler = default_handler);
PROVIDE(PVD_Handler = default_handler);
PROVIDE(TAMPER_STAMP_Handler = default_handler);
PROVIDE(RTC_WKUP_Handler = default_handler);
PROVIDE(FLASH_Handler = default_handler);
PROVIDE(RCC_Handler = default_handler);
PROVIDE(EXTI0_Handler = default_handler);
PROVIDE(EXTI1_Handler = default_handler);
PROVIDE(EXTI2_TS_Handler = default_handler);
PROVIDE(EXTI3_Handler = default_handler);
PROVIDE(EXTI4_Handler = default_handler);
PROVIDE(DMA1_Channel1_Handler = default_handler);
PROVIDE(DMA1_Channel2_Handler = default_handler);
PROVIDE(DMA1_Channel3_Handler = default_handler);
PROVIDE(DMA1_Channel4_Handler = default_handler);
PROVIDE(DMA1_Channel5_Handler = default_handler);
PROVIDE(DMA1_Channel6_Handler = default_handler);
PROVIDE(DMA1_Channel7_Handler = default_handler);
PROVIDE(ADC1_2_Handler = default_handler);
PROVIDE(USB_HP_CAN_TX_Handler = default_handler);
PROVIDE( USB_LP_CAN_RX0_Handler = default_handler);
PROVIDE(CAN_RX1_Handler = default_handler);
PROVIDE(CAN_SCE_Handler = default_handler);
PROVIDE(EXTI9_5_Handler = default_handler);
PROVIDE(TIM1_BRK_TIM15_Handler = default_handler);
PROVIDE(TIM1_UP_TIM16_Handler = default_handler);
PROVIDE(TIM1_TRG_COM_TIM17_Handler = default_handler);
PROVIDE(TIM1_CC_Handler = default_handler);
PROVIDE(TIM2_Handler = default_handler);
PROVIDE(TIM3_Handler = default_handler);
PROVIDE(TIM4_Handler = default_handler);
PROVIDE(I2C1_EV_Handler = default_handler);
PROVIDE(I2C1_ER_Handler = default_handler);
PROVIDE(I2C2_EV_Handler = default_handler);
PROVIDE(I2C2_ER_Handler = default_handler);
PROVIDE(SPI1_Handler = default_handler);
PROVIDE(SPI2_Handler = default_handler);
PROVIDE(USART1_Handler = default_handler);
PROVIDE(USART2_Handler = default_handler);
PROVIDE(USART3_Handler = default_handler);
PROVIDE(EXTI15_10_Handler = default_handler);
PROVIDE( RTC_Alarm_Handler = default_handler);
PROVIDE(USBWakeUp_Handler = default_handler);
PROVIDE(TIM8_BRK_Handler = default_handler);
PROVIDE(TIM8_UP_Handler = default_handler);
PROVIDE(TIM8_TRG_COM_Handler = default_handler);
PROVIDE(TIM8_CC_Handler = default_handler);
PROVIDE(ADC3_Handler = default_handler);
PROVIDE(SPI3_Handler = default_handler);
PROVIDE(UART4_Handler = default_handler);
PROVIDE(UART5_Handler = default_handler);
PROVIDE(TIM6_DAC_Handler = default_handler);
PROVIDE(TIM7_Handler = default_handler);
PROVIDE(DMA2_Channel1_Handler = default_handler);
PROVIDE(DMA2_Channel2_Handler = default_handler);
PROVIDE(DMA2_Channel3_Handler = default_handler);
PROVIDE(DMA2_Channel4_Handler = default_handler);
PROVIDE(DMA2_Channel5_Handler = default_handler);
PROVIDE(ADC4_Handler = default_handler);
PROVIDE(COMP1_2_3_Handler = default_handler);
PROVIDE(COMP4_5_6_Handler = default_handler);
PROVIDE(COMP7_Handler = default_handler);
PROVIDE(USB_HP_Handler = default_handler);
PROVIDE(USB_LP_Handler = default_handler);
PROVIDE(USB_WakeUp_RMP_Handler = default_handler);
PROVIDE(FPU_Handler = default_handler);