//! # LED Control Module
//!
//! This module provides basic functions to initialize and control an LED
//! connected to a GPIO pin

use crate::gpio::*;

/// Initializes an LED connected to the given GPIO port and pin.
///
/// This function performs the following steps:
/// 1. Enables the peripheral clock for the specified GPIO port.
/// 2. Configures the GPIO pin as an output pin.
/// 3. Sets the output type to push-pull.
/// 4. Optionally sets the output speed.
///
/// # Parameters
/// - *port* : The GPIO port address to which the LED is connected
/// - **pin** : The GPIO pin number to which the LED is connected
/// - `arg`: ??
///
/// # Warning
/// # Note
/// # Example
/// ```
/// led_init(GPIOA_BASE, GPIO_PIN_0);
/// ```
pub fn led_init(port: u32, pin: u32) {
    //1. enable the peripheral clock
    enable_gpio_clock(port);

    //2. Set the gpio pin mode = output mode
    set_gpio_mode_output(port, pin);

    //3. Set the ouput type = pushpull
    set_gpio_output_type_push_pull(port, pin);

    //4. Set the output speed (optional)
}

pub fn led_on(port: u32, pin: u32) {
    set_gpio_pin_state(port, pin, PinState::High);
}

pub fn led_off(port: u32, pin: u32) {
    set_gpio_pin_state(port, pin, PinState::Low);
}

pub fn led_toggle(port: u32, pin: u32) {
    set_gpio_pin_state(port, pin, PinState::Toggle);
}
