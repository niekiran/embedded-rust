#![no_std]
#![no_main]
#![allow(dead_code)]

//If you forget to import it, the compiler won't know which 
//entry attribute you are referring to, and you'll get an error.
use cortex_m_rt:: {
    entry, 
    exception, 
};


use panic_halt as _;
use cortex_m::peripheral::Peripherals;
use cortex_m::peripheral::syst;


mod led;
mod gpio;
mod mcu;
mod reg;
mod board;


#[entry]
#[allow(clippy::empty_loop)]
 fn main() -> ! {

    led::led_init(board::BLUE_LED_PORT, board::BLUE_LED_PIN);
    led::led_init(board::RED_LED_PORT, board::RED_LED_PIN);
    led::led_init(board::GREEN_LED_PORT, board::GREEN_LED_PIN);

    let mut peripherals = Peripherals::take().unwrap();
    let systick = &mut peripherals.SYST;
    systick.set_clock_source(syst::SystClkSource::Core);
    systick.set_reload(4_000_000 - 1);
    systick.clear_current();
    systick.enable_interrupt();
    systick.enable_counter();

    loop {
        /* .. */
    }
}


#[exception]
fn SysTick() {
    led::led_toggle(board::BLUE_LED_PORT, board::BLUE_LED_PIN);
    led::led_toggle(board::RED_LED_PORT, board::RED_LED_PIN);
    led::led_toggle(board::GREEN_LED_PORT, board::GREEN_LED_PIN);
}
