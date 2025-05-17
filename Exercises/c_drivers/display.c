/*
 * lcd_ex.c
 *
 *  Created on: Feb 27, 2025
 *      Author: Admin
 */


#include "display.h"


// Pointer to the registered driver; should be set at initialization time
static const display_driver_t *lcd_driver = NULL;


void display_register_driver(const display_driver_t *driver) {
	lcd_driver = driver;
}

void display_init(void) {
    if (lcd_driver && lcd_driver->init) {
        lcd_driver->init();
    }
}
void display_write_string(uint16_t x,
        uint16_t y,
        const char *str,
        FontDef font,
        uint16_t color,
        uint16_t bgcolor) {

    if (lcd_driver && lcd_driver->write_string) {
        lcd_driver->write_string(x, y, str, font, color, bgcolor);
    }
}


void display_write_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
    if (lcd_driver && lcd_driver->write_char) {
        lcd_driver->write_char(x, y, ch, font, color, bgcolor);
    }
}

void display_draw_image(uint16_t x, uint16_t w, uint16_t y, uint16_t h, const uint16_t* img_data) {
    if (lcd_driver && lcd_driver->draw_image) {
        lcd_driver->draw_image(x, w, y, h, img_data);
    }
}

void display_fill_screen(uint16_t color) {
    if (lcd_driver && lcd_driver->fill_screen) {
        lcd_driver->fill_screen(color);
    }
}

void display_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (lcd_driver && lcd_driver->draw_pixel) {
        lcd_driver->draw_pixel(x, y, color);
    }
}

void display_set_orientation(disply_orientation orientation) {
	 if (lcd_driver && lcd_driver->set_orientation) {
	        lcd_driver->set_orientation(orientation);
	    }
}

void display_fill_rectangle(uint16_t x, uint16_t w, uint16_t y, uint16_t h, uint16_t color) {
    if (lcd_driver && lcd_driver->fill_rectangle) {
        lcd_driver->fill_rectangle(x, w, y, h, color);
    }
}
