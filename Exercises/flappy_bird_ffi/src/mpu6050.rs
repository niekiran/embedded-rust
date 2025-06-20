
//here will keep code which reads the sensor data and enables us to control the birds's movement
//1. read the raw accelerometer data
//2. calculate the roll angle
//3. map the roll angle to screen position( px 0 -- px 239); angle ---> pixel position on y axis

use embedded_hal_mpu6050_driver::mpu6050::Mpu6050;
use embedded_hal_1::i2c::I2c;

use crate::{config::Coord, game::InputDevice};
use rtt_target::{rtt_init_print, rprintln};


pub struct SensorInput<T: I2c> {
    pub mpu6050: Mpu6050<T>
}

impl<T: I2c> SensorInput<T> {

    pub fn new(device_addr: u8, i2c_interface: T) -> Self {
        Self {
           mpu6050:  Mpu6050::new(i2c_interface, device_addr)
        }
    }

}

//trait implementation
impl<T: I2c> InputDevice for SensorInput<T> {
    type Error = T::Error;
    fn init(&mut self) -> Result<(), Self::Error> {

        #[cfg(feature  = "log")]
        rtt_init_print!();

        self.mpu6050.init()?;
        self.mpu6050.set_low_pass_filter(5)?;
        Ok(())
    }

     #[cfg(feature  = "log")]
    fn log_data(&mut self) {
        let data = 
            self.mpu6050.read_accel_data_raw();
        if let Ok(accel) = data {
            rprintln!("acc: X={}, y={}, Z={}", accel[0], accel[1], accel[2])
        }
    }

    fn is_tap(&mut self, y_min: Coord, y_max: Coord) -> Result<(Coord, bool), T::Error> {
        
        let accel_data = self.mpu6050.read_accel_data_raw()?;

        let roll_min = -10_f32;
        let roll_max = 90_f32;


        let roll_angle = 
            get_roll_angle(accel_data[1] as f32, accel_data[2] as f32);
        
        let new_y = 
            map_roll_angle_to_y(roll_angle, roll_min, roll_max, y_min, y_max);

        Ok((new_y as Coord, true))

    }

}


pub fn get_roll_angle(y: f32, z: f32) -> f32 {
    libm::atan2f(y, z) * (180.0_f32 / core::f32::consts::PI)
}


fn map_roll_angle_to_y(roll: f32, roll_min: f32, roll_max: f32, y_min: Coord, y_max: Coord) -> Coord {
    let clamped_roll = if roll < roll_min {
        roll_min
    } else if roll > roll_max {
        roll_max
    } else {
        roll
    };

    let normalized = (clamped_roll - roll_min) / (roll_max - roll_min);
    let y_range = (y_max - y_min) as f32;
    
    // rounding
    let mapped = y_min as f32 + normalized * y_range;
    (mapped + 0.5) as Coord
}