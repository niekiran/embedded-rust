use crate::reg;

//define addresses of nvic registers
const NVIC_BASE: u32 = 0xE000_E100;

pub const NVIC_ISER: u32 = NVIC_BASE;
pub const NVIC_ICER: u32 = NVIC_BASE + 0x80;


/// Enables the IRQ for the given IRQ number.
pub fn enable_irq(irq_number: u32) {
    let register_offset = (irq_number / 32) * 4;
    let bit_position = irq_number % 32;
    let iser_addr = (NVIC_ISER + register_offset) as *mut u32;
    reg::reg_set_bit(iser_addr, bit_position, true);
}

/// Disables the IRQ for the given IRQ number..
pub fn disable_irq(irq_number: u32) {
    let register_offset = (irq_number / 32) * 4;
    let bit_position = irq_number % 32;
    let icer_addr = (NVIC_ICER + register_offset) as *mut u32;
    reg::reg_set_bit(icer_addr, bit_position, true);
}