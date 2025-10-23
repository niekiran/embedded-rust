/*
 * lcd_ex.h
 *
 *  Created on: Feb 27, 2025
 *      Author: Admin
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include <stdint.h>
#include<stdio.h>
#include <stdbool.h>
#include "font.h"

typedef enum {
	PORTRAIT = 0,
	LANDSCAPE
}disply_orientation;

// Define a type for the driver interface
typedef struct {
    void (*init)(void);
    /**
         * @brief  Write a string on the display at the specified position, font, and colors.
         * @param  x       X-coordinate (start column) in pixels
         * @param  y       Y-coordinate (start row) in pixels
         * @param  str     Pointer to the null-terminated string to be displayed
         * @param  font    Font definition structure
         * @param  color   Text color in RGB565 format
         * @param  bgcolor Background color in RGB565 format
         * @retval None
         */
    void (*write_string)(uint16_t x,
                             uint16_t y,
                             const char *str,
                             FontDef font,
                             uint16_t color,
                             uint16_t bgcolor);
    void (*write_char)(
    		uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor
			);
    void (*draw_image)(uint16_t x, uint16_t w, uint16_t y, uint16_t h, const uint16_t* img_data);
    void (*fill_screen)(uint16_t color);
    void (*draw_pixel)(uint16_t x, uint16_t y, uint16_t color);
    void (*set_orientation)(disply_orientation orientation);
    void (*fill_rectangle)(uint16_t x, uint16_t w, uint16_t y, uint16_t h, uint16_t color);
} display_driver_t;

// Expose common functions that the application will call
void display_init(void);
void display_write_string(uint16_t x,
        uint16_t y,
        const char *str,
        FontDef font,
        uint16_t color,
        uint16_t bgcolor);
void display_write_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
void display_draw_image(uint16_t x, uint16_t w, uint16_t y, uint16_t h, const uint16_t* img_data);
void display_fill_screen(uint16_t color);
void display_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void display_set_orientation(disply_orientation orientation);
void display_fill_rectangle(uint16_t x, uint16_t w, uint16_t y, uint16_t h, uint16_t color);

// Function to register the hardware-specific driver
void display_register_driver(const display_driver_t *driver);




#endif /* INC_LCD_H_ */
