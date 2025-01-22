use crate::mcu::*;
use crate::reg::*;

pub mod gpio {

    use super::*; //import everthing from parent

    pub enum EdgeTrigger {
        Rising,
        Falling,
    }

    pub fn set_edge(pin: u32, edge: EdgeTrigger) {
        let exti_rtsr1_addr = (EXTI_BASE + 0x08) as *mut u32;
        let exti_ftsr1_addr = (EXTI_BASE + 0x0c) as *mut u32;

        match edge {
            EdgeTrigger::Falling => {
                reg_set_bit(exti_ftsr1_addr, pin, true);
            }

            EdgeTrigger::Rising => {
                reg_set_bit(exti_rtsr1_addr, pin, true);
            }
        }
    }

    pub fn configure_syscfg(port: u32, pin: u32) {
        let reg_offset = (pin / 4) * 4;
        let bit_position = (pin % 4) * 4;
        let syscfg_reg_addr = (SYSCFG_BASE + 0x08 + reg_offset) as *mut u32;

        match port {
            GPIOA_BASE => {
                reg_set_bits(syscfg_reg_addr, 0, bit_position, 4);
            }

            GPIOB_BASE => {
                reg_set_bits(syscfg_reg_addr, 1, bit_position, 4);
            }

            //include more match arms realted to other gpio ports like GPIOC, D, E, etc

            _ => (),
        }
    }

}



pub enum ExtiLine {
    Line0 = 0,
    Line1,
    Line2,
    Line3,
}

impl ExtiLine {
    pub fn from_pin(pin: u32) -> Option<ExtiLine> {
        match pin {
            0 => Some(ExtiLine::Line0),
            1 => Some(ExtiLine::Line1),
            //create similar arms from 0 to 15
            _ => None,
        }
    }
}

fn configure_interrupt(exti_line: ExtiLine, is_enable: bool) {
    let exti_imr1_addr = (EXTI_BASE + 0x00) as *mut u32;
    let exti_imr2_addr = (EXTI_BASE + 0x20) as *mut u32;
    let line = exti_line as u32;
    match line {
        0..=31 => {
            reg_set_bit(exti_imr1_addr, line, is_enable);
        }

        32..=35 => {
            reg_set_bit(exti_imr2_addr, line, is_enable);
        }

        _ => (),
    }
}
pub fn enable_interrupt(exti_line: ExtiLine) {
    configure_interrupt(exti_line, true);
}

pub fn disable_interrupt(exti_line: ExtiLine) {
    configure_interrupt(exti_line, false);
}

pub fn clear_pending_interrupt(exti_line: ExtiLine) {
    let exti_pr1_reg_addr = (EXTI_BASE + 0x14) as *mut u32;
    let exti_pr2_reg_addr = (EXTI_BASE + 0x34) as *mut u32;

    let line = exti_line as u32;

    match line {
        0..=31 => {
            reg_set_bit(exti_pr1_reg_addr, line, true);
        }

        32..=33 => {
            reg_set_bit(exti_pr2_reg_addr, line, true);
        }

        _ => (),
    }
}