#![no_std]
#![no_main]
#![allow(dead_code)]

mod assets;
mod color;
mod config;
mod display;
mod game;
mod obstacle;
mod player;

use game::Game;
use panic_halt as _;


extern "C" {
    fn c_main();
    static gc9a01a_driver: display::DisplayDriver;
    fn HAL_Delay(delay: u32);
}

// #[entry]
#[no_mangle]
extern "C" fn main() -> ! {
    unsafe {
        c_main();
        display::register_driver(&gc9a01a_driver);
    }

    display::init();
    let mut game = Game::init();
    Game::draw_start_screen();

    loop {

        if !game.is_over() {
            unsafe {
                HAL_Delay(100);
            }
            game.update();
        }

 
    }
}
