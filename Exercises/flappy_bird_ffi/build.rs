use std::fs;
use std::{env, path::PathBuf};
// we have to take the help of CC crate to compile C code

fn main() {
    let out_dir = env::var("OUT_DIR").expect("OUT_DIR not set by Cargo");
    println!("cargo:rustc-link-search=native={}", out_dir);

    let mut builder: cc::Build = cc::Build::new();
    //1. set the cross compiler
    builder.compiler("arm-none-eabi-gcc");

    //2.a Add all .c files from the HAL driver folder
    let hal_src_path = "c_src/lcd_tsc_mpu6050_drivers/Drivers/STM32F3xx_HAL_Driver/Src";
    for entry in fs::read_dir(hal_src_path).expect("can not read hal driver src folder") {
        let path = entry.unwrap().path();
        if path.extension().and_then(|s| s.to_str()) == Some("c") {
            println!("Compiling {:?}", path);
            println!("cargo::rerun-if-changed={}", path.display());
            builder.file(&path);
        }
    }

    //2.b Add all .c files from Core/Src
    let core_src_path = "c_src/lcd_tsc_mpu6050_drivers/Core/Src";
    for entry in fs::read_dir(core_src_path).expect("can not read core src folder") {
        let path = entry.unwrap().path();
        if path.extension().and_then(|s| s.to_str()) == Some("c") {
            println!("Compiling {:?}", path);
            println!("cargo::rerun-if-changed={}", path.display());
            builder.file(&path);
        }
    }

    //3. Add all C include files (.h files)

    builder.include("c_src/lcd_tsc_mpu6050_drivers/Core/Inc");
    builder.include("c_src/lcd_tsc_mpu6050_drivers/Drivers/STM32F3xx_HAL_Driver/Inc");
    builder.include("c_src/lcd_tsc_mpu6050_drivers/Drivers/CMSIS/Include");
    builder.include("c_src/lcd_tsc_mpu6050_drivers/Drivers/CMSIS/Device/ST/STM32F3xx/Include");

    //4. Add Define macros , -D (optional)

    builder.define("DEBUG", None);
    builder.define("USE_HAL_DRIVER", None);
    builder.define("STM32F303xC", None);

    //5. Add .s or .asm files (optional)
    builder.file("c_src/lcd_tsc_mpu6050_drivers/Core/Startup/startup_stm32f303cctx.s");
    println!(
        "cargo::rerun-if-changed=c_src/lcd_tsc_mpu6050_drivers/Core/Startup/startup_stm32f303cctx.s"
    );

    //6. Add compiler flags
    builder
        .flag("-mcpu=cortex-m4")
        .flag("-mthumb")
        .flag("-mfpu=fpv4-sp-d16")
        .flag("-mfloat-abi=hard")
        .flag("-std=gnu11")
        .flag("-g3")
        .flag("-O0")
        .flag("-ffunction-sections")
        .flag("-fdata-sections")
        .flag("-Wall")
        .flag("-fstack-usage");
        //.flag("-fcyclomatic-complexity");
    // .flag("-flto");

    //7 . Add linker flags
    // println!("cargo:rustc-link-arg=-nostartfiles");
    // println!("cargo:rustc-link-arg=-nostdlib");
    println!("cargo:rustc-link-arg=--specs=nano.specs"); //link against Newlib-nano
    println!("cargo:rustc-link-arg=--specs=nosys.specs");
    println!("cargo:rustc-link-arg=-Wl,--gc-sections");

    //8. generate object files for C files
    // builder.compile("stm32_c_drivers");
    let object_files = builder.compile_intermediates();

    //9. this tells the cargo to pass each object file directly to the linker
    for obj_file in &object_files {
        println!("cargo:rustc-link-arg={}", obj_file.display());
    }

    // //9. informs Cargo to pass -lstm32_c_drivers to the linker, which makes
    // // the linker look for libstm32_c_drivers.a and link it.
    // println!("cargo:rustc-link-lib=static=stm32_c_drivers");
}
