use core::ptr;

pub unsafe fn read_register(addr: *mut u32) -> u32 {
    ptr::read_volatile(addr)
}

pub unsafe fn write_register(addr: *mut u32, value: u32) {
    ptr::write_volatile(addr, value)
}

pub fn reg_set_bits(reg_addr: *mut u32, new_bits_val: u32, bit_position: u32, n_bits: u32) {
    assert!(
        n_bits > 0 && n_bits <= 32,
        "n_bits must be between 1 and 32"
    );
    assert!(bit_position < 32, "bit_position must be less than 32");

    unsafe {
        // Read the current value of the register
        let reg_value = read_register(reg_addr);

        // Create a mask for the bits to clear
        let mask = ((1 << n_bits) - 1) << bit_position;

        // Clear the relevant bits in the register and set the new value
        let updated_value = (reg_value & !mask) | ((new_bits_val << bit_position) & mask);

        // Write the modified value back to the register
        write_register(reg_addr, updated_value);
    }
}

pub fn reg_set_bit(reg_addr: *mut u32, bit_position: u32, bit_val: bool) {
    unsafe {
        // Read the current value of the register
        let reg_value = read_register(reg_addr);

        // Set or clear the specific bit based on `bit_val`
        let updated_value = if bit_val {
            reg_value | (1 << bit_position)
        } else {
            reg_value & !(1 << bit_position)
        };

        // Write the modified value back to the register
        write_register(reg_addr, updated_value);
    }
}

pub fn reg_set_val(reg_addr: *mut u32, new_reg_val: u32) {
    unsafe {
        write_register(reg_addr, new_reg_val);
    }
}

pub fn reg_read_bit(reg_addr: *mut u32, bit: u32) -> bool {
    unsafe {
        let reg_value = read_register(reg_addr);
        (reg_value & (1 << bit)) != 0
    }
}
