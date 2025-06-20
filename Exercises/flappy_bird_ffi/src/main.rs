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
mod mpu6050;
mod i2c_adapter;

use core::pin::Pin;

use game::{Game, InputDevice};
use i2c_adapter::I2cAdapter;
use panic_halt as _;
use stm32f3xx_hal::flash::FlashExt;
use stm32f3xx_hal::gpio::GpioExt;
use stm32f3xx_hal::i2c::I2c as hal_i2c;
use stm32f3xx_hal::time::rate::{Hertz, Kilohertz};
use stm32f3xx_hal::{pac, time};
use stm32f3xx_hal::rcc::RccExt;
use stm32f3xx_hal::gpio::{
    PB6,PB7,AF4,OpenDrain
};



use config::*;

type HalI2cType = hal_i2c<pac::I2C1, (PB6<AF4<OpenDrain>>, PB7<AF4<OpenDrain>>)>;

extern "C" {
    fn c_main();
    static gc9a01a_driver: display::DisplayDriver;
    fn HAL_Delay(delay: u32);
}

// #[entry]
#[no_mangle]
extern "C" fn main() -> ! {

    let i2c = I2cAdapter::new(i2c_init());

    c_init();

    display_init();
    
    let sensor_input = mpu6050::SensorInput::new(MPU6050_DEV_ADDR, i2c);


    let mut game = 
        game_init(sensor_input).expect("Game init failed");

    loop {
        if !game.is_over() {
            unsafe {
                //HAL_Delay(1);
            }
            
            game.input_device.log_data();
            game.update();
        }
    }
}



fn c_init() {
    unsafe {
        c_main();
    }
}


fn display_init() {
    unsafe {
        display::register_driver(&gc9a01a_driver);
    }

    display::init();
}

fn game_init<T: InputDevice>(input_device: T) -> Result<Game<T>, T::Error> {
    let game = Game::init(input_device)?;
    Game::<T>::draw_start_screen();
    Ok(game)
}






fn i2c_init()  ->  HalI2cType {

    //gives you ownership of the entire peripheral block from the PAC 
    let dp = pac::Peripherals::take().unwrap();
   
   //1. configure the gpio pins for the i2c functionality
   //pins  pb6(SCL) and pb7(SDA) should configured for i2c functionality

  
    let mut rcc = dp.RCC.constrain();
    let mut flash = dp.FLASH.constrain();
    let mut gpiob = dp.GPIOB.split(&mut rcc.ahb);

    let mut scl = gpiob.pb6.into_af_open_drain::<4>
    ( 
        &mut gpiob.moder , &mut gpiob.otyper, &mut gpiob.afrl
    );

    let mut sda = gpiob.pb7.into_af_open_drain::<4>
    ( 
        &mut gpiob.moder , &mut gpiob.otyper, &mut gpiob.afrl
    );

    scl.internal_pull_up(&mut gpiob.pupdr, true);
    sda.internal_pull_up(&mut gpiob.pupdr, true);


   let i2c_freq = Hertz::try_from(Kilohertz::new(100_u32)).unwrap();

   let clocks = rcc.cfgr.freeze(&mut flash.acr);


   //2. configure the I2C pheripheral
    hal_i2c::new
    (
        dp.I2C1, 
        (scl, sda), 
        i2c_freq, 
        clocks, 
        &mut rcc.apb1
    )


}