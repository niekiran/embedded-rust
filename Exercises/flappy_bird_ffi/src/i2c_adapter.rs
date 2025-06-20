// adapter layer: converts embedded-hal 0.2.7 I2C to embedded-hal 1.0.0 I2C

//use embedded_hal::blocking::i2c::{Write, WriteRead};

use stm32f3xx_hal::hal::blocking::i2c::{Write, WriteRead};

use embedded_hal_1::i2c::{
    Error as Eh1ErrorTrait, ErrorKind, ErrorType as Eh1ErrorType,
    I2c as Eh1I2c, NoAcknowledgeSource, Operation,
};

/// Wrapper for stm32f3xx-hal I2C error to satisfy embedded-hal 1.0.0's Error trait
#[derive(Debug)]
pub struct MappedI2cError(pub stm32f3xx_hal::i2c::Error);

impl Eh1ErrorTrait for MappedI2cError {
    fn kind(&self) -> ErrorKind {
        match self.0 {
            stm32f3xx_hal::i2c::Error::Arbitration => ErrorKind::ArbitrationLoss,
            stm32f3xx_hal::i2c::Error::Bus => ErrorKind::Bus,
            stm32f3xx_hal::i2c::Error::Nack => ErrorKind::NoAcknowledge(NoAcknowledgeSource::Unknown),
            stm32f3xx_hal::i2c::Error::Busy => ErrorKind::Other,
            _ => ErrorKind::Other,
        }
    }
}

/// adapter that allows using stm32f3xx-hal's I2C (eh 0.2.7) with drivers requiringg embedded-hal 1.0.0
pub struct I2cAdapter<I2C> {
    pub inner: I2C,
}

impl<I2C> I2cAdapter<I2C> {
    pub fn new(inner: I2C) -> Self {
        Self { inner }
    }
}


type I2cWriteError<I2C> = <I2C as Write>::Error;
type I2cWriteReadError<I2C> = <I2C as WriteRead>::Error;


impl<I2C> Eh1ErrorType for I2cAdapter<I2C>
where
    I2C: Write + WriteRead,
    I2cWriteError<I2C>: Into<stm32f3xx_hal::i2c::Error>,
    I2cWriteReadError<I2C>: Into<stm32f3xx_hal::i2c::Error>,
{
    type Error = MappedI2cError;
}

// implement embedded-hal 1.0.0 I2c trait
impl<I2C> Eh1I2c for I2cAdapter<I2C>
where
    I2C: Write + WriteRead,
    I2cWriteError<I2C>: Into<stm32f3xx_hal::i2c::Error>,
    I2cWriteReadError<I2C>: Into<stm32f3xx_hal::i2c::Error>,
{
    fn read(&mut self, _addr: u8, _buffer: &mut [u8]) -> Result<(), Self::Error> {
        // Optional: not used by MPU6050
        unimplemented!("MPU6050 driver does not call read() directly.")
    }

    fn write(&mut self, addr: u8, bytes: &[u8]) -> Result<(), Self::Error> {
        self.inner
            .write(addr, bytes)
            .map_err(|e| MappedI2cError(e.into()))
    }

    fn write_read(
        &mut self,
        addr: u8,
        bytes: &[u8],
        buffer: &mut [u8],
    ) -> Result<(), Self::Error> {
        self.inner
            .write_read(addr, bytes, buffer)
            .map_err(|e| MappedI2cError(e.into()))
    }

    fn transaction(
        &mut self,
        addr: u8,
        operations: &mut [Operation<'_>],
    ) -> Result<(), Self::Error> {
        for op in operations {
            match op {
                Operation::Write(data) => {
                    self.inner
                        .write(addr, data)
                        .map_err(|e| MappedI2cError(e.into()))?
                }
                Operation::Read(buf) => {
                    self.inner
                        .write_read(addr, &[], buf)
                        .map_err(|e| MappedI2cError(e.into()))?
                }
            }
        }
        Ok(())
    }
}
