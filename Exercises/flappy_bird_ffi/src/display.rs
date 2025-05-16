use crate::config::*;
use core::convert::TryInto;
use core::ffi;

extern "C" {
    fn display_register_driver(driver: *const DisplayDriver);
    fn display_init();
    fn display_draw_image(x: u16, w: u16, y: u16, h: u16, img_data: *const u16);
    fn display_fill_screen(color: u16);
    fn display_fill_rectangle(x: u16, w: u16, y: u16, h: u16, color: u16);
    fn display_write_string(
        x: u16,
        y: u16,
        str_ptr: *const ffi::c_char,
        font: FontDef,
        color: u16,
        bgcolor: u16,
    );

    static Font_16x26: FontDef;

}

#[repr(C)]
pub struct DisplayDriver {
    __private: [u8; 0],
}

#[derive(Copy, Clone)]
#[repr(C)]
pub struct FontDef {
    width: ffi::c_uchar,
    height: ffi::c_uchar,
    data: *const u16,
}

pub fn register_driver(driver: &DisplayDriver) {
    unsafe {
        display_register_driver(driver as *const DisplayDriver);
    }
}

pub fn init() {
    unsafe {
        display_init();
    }
}

pub fn draw_image(x: Coord, w: u32, y: Coord, h: u32, image_data: &[u16]) {
    let x: u16 = x.try_into().expect("X co-ordinate is out of range");
    let y: u16 = y.try_into().expect("y co-ordinate is out of range");
    let w: u16 = w.try_into().expect("width out of range");
    let h: u16 = h.try_into().expect("height out of range");
    unsafe {
        display_draw_image(x, w, y, h, image_data.as_ptr());
    }
}

pub fn set_background_color(bg_color: u16) {
    unsafe {
        display_fill_screen(bg_color);
    }
}

pub fn draw_rect_angle(x: Coord, w: u32, y: Coord, h: u32, color: u16) {
    let x: u16 = x.try_into().expect("X co-ordinate is out of range");
    let y: u16 = y.try_into().expect("y co-ordinate is out of range");
    let w: u16 = w.try_into().expect("width out of range");
    let h: u16 = h.try_into().expect("height out of range");
    unsafe {
        display_fill_rectangle(x, w, y, h, color);
    }
}

pub fn write_string(x: Coord, y: Coord, c_str: &ffi::CStr, color: u16, bgcolor: u16) {
    let x: u16 = x.try_into().expect("X co-ordinate is out of range");
    let y: u16 = y.try_into().expect("y co-ordinate is out of range");
    unsafe {
        //call  display_write_string
        display_write_string(x, y, c_str.as_ptr(), Font_16x26, color, bgcolor);
    }
}
