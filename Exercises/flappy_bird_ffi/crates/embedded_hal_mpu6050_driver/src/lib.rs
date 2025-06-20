
//when features !=  use_std then enable the attribute #![no_std]

#![cfg_attr(not(feature = "use_std"), no_std)]

// this makes your driver usable outside the crate
pub mod mpu6050;

// this keeps this module private to teh crate. no need to expose constants unless needed)
mod reg;

//this is the placeholder for unit tests. 
#[cfg(test)]
mod tests {
    use super::*;
}
