
//import the trait or interface
use embedded_hal::i2c::I2c;
use crate::reg;

pub struct Mpu6050<T> {
    device_address: u8,
    i2c_interface: T,
   
}

impl<T: I2c> Mpu6050<T>
{

   pub fn new(i2c_interface: T, device_address: u8) -> Self {
        Self {
            device_address,
            i2c_interface, 
          
        }
    }

    //minimal init code 
    // here you have to clear SLEEP bit of the register PWR_MGMT_1 of mpu6050 sensor
    //after powerup the sensor sets this bit to 1(SLEEP bit = 1), putting the device into low-power mode
    pub fn init(&mut self) -> Result<(), T::Error> {
        self.write_byte(reg::MPU6050_REG_PWR_MGMT_1, 0x00)
    }

    fn write_byte(&mut self, reg_addr: u8, reg_value: u8) -> Result<(), T::Error> {
        let buf: [u8; 2] = [reg_addr, reg_value];
        self.i2c_interface.write(self.device_address, &buf)
    }

    fn read_byte(&mut self, reg_addr: u8) -> Result<u8, T::Error> {
        let mut read_buf = [0u8];
        self.i2c_interface.
            write_read(self.device_address, &[reg_addr], &mut read_buf)?;
        Ok(read_buf[0])
    }

    fn read_n_byte(&mut self, reg_addr: u8, read_buf: &mut [u8]) -> Result<(), T::Error> {
        self.i2c_interface
            .write_read(self.device_address, &[reg_addr], read_buf)
    }

    pub fn read_accel_data_raw(&mut self) -> Result<[i16; 3], T::Error> {
        let mut buf = [0; 6];


        self.read_n_byte(reg::MPU6060_REG_ACCEL_READ, &mut buf)?;

        let x = i16::from_be_bytes([buf[0], buf[1]]);
        let y = i16::from_be_bytes([buf[2], buf[3]]);
        let z = i16::from_be_bytes([buf[4], buf[5]]);

        Ok([x, y, z])
   }

pub fn set_low_pass_filter(&mut self, dlpf_cfg: u8) -> Result<(), T::Error> {
    assert!(dlpf_cfg <= 6, "Invalid DLPF_CFG value");

    const CONFIG_REGISTER: u8 = 0x1A;

    // Write dlpf_cfg to CONFIG register
    self.write_byte(CONFIG_REGISTER, dlpf_cfg)
 }



}