/*
 * gc9a01a.c
 *
 *  Created on: Feb 27, 2025
 *      Author: Admin
 */


// gc9a01a.c
#include "display.h"
#include <stdio.h>  // For demonstration purposes
#include "gc9a01a.h"

static void gc9a01a_hw_reset(void);
static void gc9a01a_configure(void);
static void gc9a01a_fill_rect(uint16_t x, uint16_t w, uint16_t y, uint16_t h, uint16_t color);
static void gc9a01a_set_address_window(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1);
static void gc9a01a_write_cmd(uint8_t cmd);
static void gc9a01a_write_data(uint8_t* buff, uint32_t buff_size);
static void gc9a01a_invert_colors(bool invert);
static void gc9a01a_lcd_fill_image(const uint16_t* data);
static void gc9a01a_write_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
static void gc9a01a_set_orientation(uint8_t orientation);

static void gc9a01a_init(void) {
    // Hardware-specific initialization code for the GC9A01A
    GC9A01A_BL_ON;
    GC9A01A_RD_HIGH;
    gc9a01a_hw_reset();
    gc9a01a_configure();
    gc9a01a_set_orientation(LANDSCAPE);
}

/**
  * @brief  Write a string on the GC9A01A LCD
  * @param  x: Start column address
  * @param  y: Start row address
  * @param  str: Pointer to the string to be displayed
  * @param  font: Font definition structure
  * @param  color: Text color (RGB565 format)
  * @param  bgcolor: Background color (RGB565 format)
  * @retval None
  */

  void gc9a01a_write_string(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
    //GC9A01A_CS_LOW;
  
    while(*str) {
      if(x + font.width >= GC9A01A_WIDTH) {
        x = 0;
        y += font.height;
        if(y + font.height >= GC9A01A_HEIGHT) {
          break;
        }
  
        if(*str == ' ') {
          // skip spaces in the beginning of the new line
          str++;
          continue;
        }
      }
  
      gc9a01a_write_char(x, y, *str, font, color, bgcolor);
      x += font.width;
      str++;
    }
  
    //GC9A01A_CS_HIGH;
  }


/**
  * @brief  Write a character on the GC9A01A LCD
  * @param  x: Start column address
  * @param  y: Start row address
  * @param  ch: Character to be displayed
  * @param  font: Font definition structure
  * @param  color: Color of the character (RGB565 format)
  * @param  bgcolor: Background color of the character (RGB565 format)
  * @retval None
  */
static void gc9a01a_write_char(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
    // Send character to display hardware
	   uint32_t i, b, j;

	   gc9a01a_set_address_window(x, x+font.width-1, y, y+font.height-1);

	   for(i = 0; i < font.height; i++) {
	     b = font.data[(ch - 32) * font.height + i];
	     for(j = 0; j < font.width; j++) {
	       if((b << j) & 0x8000)  {
	         uint8_t data[] = { color >> 8, color & 0xFF };
	         gc9a01a_write_data(data, sizeof(data));
	       } else {
	         uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
	         gc9a01a_write_data(data, sizeof(data));
	       }
	     }
	   }
}


/**
   * @brief  Draw an image on the GC9A01A LCD
   * @param  x: Start column address
   * @param  w: Width of the image
   * @param  y: Start row address
   * @param  h: Height of the image
   * @param  data: Pointer to the image data (RGB565 format)
   * @retval None
   */
static void gc9a01a_draw_image(
		uint16_t x, uint16_t w, uint16_t y, uint16_t h, const uint16_t* img_data
		)
{
    // Draw an image on the display
	if ((x >= GC9A01A_WIDTH) || (y >= GC9A01A_HEIGHT)) return;
	   if ((x + w - 1) >= GC9A01A_WIDTH) w = GC9A01A_HEIGHT - x;
	   if ((y + h - 1) >= GC9A01A_WIDTH) h = GC9A01A_HEIGHT - y;
	   //GC9A01A_CS_LOW;
	   gc9a01a_set_address_window(x, (x+w-1), y, (y+h-1));

	   for (uint32_t i = 0; i < w * h; i++) {
	     uint8_t color_high = (img_data[i] >> 8) & 0xFF;
	     uint8_t color_low = img_data[i] & 0xFF;
	     GC9A01A_WRITE_8BIT(color_high);
	     GC9A01A_WRITE_8BIT(color_low);
	   }
	   //GC9A01A_CS_HIGH;
}

/**
  * @brief  Fill the entire background of the GC9A01A LCD with a specified color
  * @param  color: Color to fill the background (RGB565 format)
  * @retval None
  */
static void gc9a01a_fill_screen(uint16_t color) {
    // Fill the display with a color
	gc9a01a_fill_rect(0, GC9A01A_WIDTH, 0, GC9A01A_HEIGHT, color);
}

/**
  * @brief  Draw a single pixel on the GC9A01A LCD
  * @param  x: X-coordinate of the pixel
  * @param  y: Y-coordinate of the pixel
  * @param  color: Color of the pixel in RGB565 format
  * @retval None
  */
static void gc9a01a_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    // Draw a single pixel on the display
	  if((x >= GC9A01A_WIDTH) || (y >= GC9A01A_HEIGHT))
	     return;

	   //GC9A01A_CS_LOW;

	   gc9a01a_set_address_window(x, y, x+1, y+1);
	   uint8_t data[] = { color >> 8, color & 0xFF };
	   gc9a01a_write_data(data, sizeof(data));

	   //GC9A01A_CS_HIGH;
}

/**
  * @brief  Set the orientation of the GC9A01A LCD display
  * @param  orientation: Orientation mode (LANDSCAPE or PORTRAIT)
  * @retval None
  */
static void gc9a01a_set_orientation(uint8_t orientation) {
  uint8_t params[4];
  //GC9A01A_CS_LOW;
  if (orientation == LANDSCAPE) {
    // Set column address for landscape orientation
    gc9a01a_write_cmd(GC9A01A_CASET);
    params[0] = 0x00;
    params[1] = 0x00;
    params[2] = 0x00;
    params[3] = 0xf0;
    gc9a01a_write_data(params, 4);

    // Set row address for landscape orientation
    gc9a01a_write_cmd(GC9A01A_RASET);
    gc9a01a_write_data(params, 4);

    // Set orientation parameters for landscape mode
    params[0] = MADCTL_MV | MADCTL_BGR;
  } else if (orientation == PORTRAIT) {
    // Set column address for portrait orientation
    gc9a01a_write_cmd(GC9A01A_CASET);
    params[0] = 0x00;
    params[1] = 0x00;
    params[2] = 0x00;
    params[3] = 0xf0;
    gc9a01a_write_data(params, 4);

    // Set row address for portrait orientation
    gc9a01a_write_cmd(GC9A01A_RASET);
    gc9a01a_write_data(params, 4);

    // Set orientation parameters for portrait mode
    params[0] =  MADCTL_MX | MADCTL_BGR;
  }

  // Set MADCTL register with the configured parameters
  gc9a01a_write_cmd(GC9A01A_MADCTL);
  gc9a01a_write_data(params, 1);
  //GC9A01A_CS_HIGH;
}




static void gc9a01a_hw_reset() {
  GC9A01A_RST_HIGH  ;
  HAL_Delay(10);
  GC9A01A_RST_LOW;
  HAL_Delay(10);
  GC9A01A_RST_HIGH ;
  HAL_Delay(10);
}

/**
  * @brief  Initialize the GC9A01A LCD
  * @param  None
  * @retval None
  */
static void gc9a01a_configure() {
  uint8_t params[15];
  GC9A01A_CS_LOW;

  gc9a01a_write_cmd(GC9A01A_INREGEN1);      ///< Inter register enable 1
  gc9a01a_write_cmd(GC9A01A_INREGEN2);      ///< Inter register enable 2

  params[0] = 0x14;
  gc9a01a_write_cmd(0xEB);
  gc9a01a_write_data(params, 1);

  params[0] = 0x60;
  gc9a01a_write_cmd(0x84);
  gc9a01a_write_data(params, 1);

  params[0] = 0xFF;
  gc9a01a_write_cmd(0x85);
  gc9a01a_write_data(params, 1);

  params[0] = 0xFF;
  gc9a01a_write_cmd(0x86);
  gc9a01a_write_data(params, 1);

  params[0] = 0xFF;
  gc9a01a_write_cmd(0x87);
  gc9a01a_write_data(params, 1);

  params[0] = 0xFF;
  gc9a01a_write_cmd(0x8E);
  gc9a01a_write_data(params, 1);

  params[0] = 0xFF;
  gc9a01a_write_cmd(0x8F);
  gc9a01a_write_data(params, 1);

  params[0] = 0x0A;
  gc9a01a_write_cmd(0x88);
  gc9a01a_write_data(params, 1);

  params[0] = 0x21;
  gc9a01a_write_cmd(0x89);
  gc9a01a_write_data(params, 1);

  params[0] = 0x00;
  gc9a01a_write_cmd(0x8A);
  gc9a01a_write_data(params, 1);

  params[0] = 0x80;
  gc9a01a_write_cmd(0x8B);
  gc9a01a_write_data(params, 1);

  params[0] = 0x01;
  gc9a01a_write_cmd(0x8C);
  gc9a01a_write_data(params, 1);

  params[0] = 0x03;
  gc9a01a_write_cmd(0x8D);
  gc9a01a_write_data(params, 1);

  params[0] = 0x08;
  params[1] = 0x09;
  params[2] = 0x14;
  params[3] = 0x08;
  gc9a01a_write_cmd(0xB5);
  gc9a01a_write_data(params, 4);

  params[0] = 0x00;
  params[1] = 0x00;
  gc9a01a_write_cmd(GC9A01A_DISP_CTRL);     ///<Display Function Control
  gc9a01a_write_data(params, 2);

  params[0] = 0x48;
  gc9a01a_write_cmd(GC9A01A_MADCTL);        ///< Memory Access Control

  gc9a01a_write_data(params, 1);

  params[0] = 0x05;
  gc9a01a_write_cmd(GC9A01A_COLMOD);        ///< Pixel Format Set
  gc9a01a_write_data(params, 1);

  params[0] = 0x08;
  params[1] = 0x08;
  params[2] = 0x08;
  params[3] = 0x08;
  gc9a01a_write_cmd(0x90);
  gc9a01a_write_data(params, 4);

  params[0] = 0x06;
  gc9a01a_write_cmd(0xBD);
  gc9a01a_write_data(params, 1);

  params[0] = 0x01;
  gc9a01a_write_cmd(0xBA);
  gc9a01a_write_data(params, 1);

  params[0] = 0x00;
  gc9a01a_write_cmd(0xBC);
  gc9a01a_write_data(params, 1);

  params[0] = 0x60;
  params[1] = 0x01;
  params[2] = 0x04;
  gc9a01a_write_cmd(0xFF);
  gc9a01a_write_data(params, 3);

  params[0] = 0x14;
  gc9a01a_write_cmd(GC9A01A1_POWER2);       ///< Power Control 2
  gc9a01a_write_data(params, 1);

  params[0] = 0x14;
  gc9a01a_write_cmd(GC9A01A1_POWER3);       ///< Power Control 3
  gc9a01a_write_data(params, 1);

  params[0] = 0x25;
  gc9a01a_write_cmd(GC9A01A1_POWER4);       ///< Power Control 4
  gc9a01a_write_data(params, 1);

  params[0] = 0x11;
  gc9a01a_write_cmd(0xBE);
  gc9a01a_write_data(params, 1);

  params[0] = 0x10;
  params[1] = 0x0e;
  gc9a01a_write_cmd(0xE1);
  gc9a01a_write_data(params, 2);

  params[0] = 0x21;
  params[1] = 0x0c;
  params[2] = 0x02;
  gc9a01a_write_cmd(0xDF);
  gc9a01a_write_data(params, 3);

  params[0] = 0x45;
  params[1] = 0x09;
  params[2] = 0x08;
  params[3] = 0x08;
  params[4] = 0x26;
  params[5] = 0x2A;
  gc9a01a_write_cmd(GC9A01A_GAMMA1);        ///< Set gamma 1
  gc9a01a_write_data(params, 6);

  params[0] = 0x43;
  params[1] = 0x70;
  params[2] = 0x72;
  params[3] = 0x36;
  params[4] = 0x37;
  params[5] = 0x6F;
  gc9a01a_write_cmd(GC9A01A_GAMMA2);        ///< Set gamma 2
  gc9a01a_write_data(params, 6);

  params[0] = 0x45;
  params[1] = 0x09;
  params[2] = 0x08;
  params[3] = 0x08;
  params[4] = 0x26;
  params[5] = 0x2A;
  gc9a01a_write_cmd(GC9A01A_GAMMA3);        ///< Set gamma 3
  gc9a01a_write_data(params, 6);

  params[0] = 0x43;
  params[1] = 0x70;
  params[2] = 0x72;
  params[3] = 0x36;
  params[4] = 0x37;
  params[5] = 0x6F;
  gc9a01a_write_cmd(GC9A01A_GAMMA4);        ///< Set gamma 4
  gc9a01a_write_data(params, 6);

  params[0] = 0x1B;
  params[1] = 0x0B;
  gc9a01a_write_cmd(0xED);
  gc9a01a_write_data(params, 2);

  params[0] = 0x77;
  gc9a01a_write_cmd(0xAE);
  gc9a01a_write_data(params, 1);

  params[0] = 0x63;
  gc9a01a_write_cmd(0xCD);
  gc9a01a_write_data(params, 1);

  params[0] = 0x07;
  params[1] = 0x07;
  params[2] = 0x04;
  params[3] = 0x0E;
  params[4] = 0x0F;
  params[5] = 0x09;
  params[6] = 0x07;
  params[7] = 0x08;
  params[8] = 0x03;
  gc9a01a_write_cmd(0x70);
  gc9a01a_write_data(params, 9);

  params[0] = 0x34;
  gc9a01a_write_cmd(GC9A01A_FRAMERATE);     ///< Frame rate control
  gc9a01a_write_data(params, 1);

  params[0] = 0x18;
  params[1] = 0x0D;
  params[2] = 0x71;
  params[3] = 0xED;
  params[4] = 0x70;
  params[5] = 0x70;
  params[6] = 0x18;
  params[7] = 0x0F;
  params[8] = 0x71;
  params[9] = 0xEF;
  params[10] = 0x70;
  params[11] = 0x70;
  gc9a01a_write_cmd(0x62);
  gc9a01a_write_data(params, 12);

  params[0] = 0x18;
  params[1] = 0x11;
  params[2] = 0x71;
  params[3] = 0xF1;
  params[4] = 0x70;
  params[5] = 0x70;
  params[6] = 0x18;
  params[7] = 0x13;
  params[8] = 0x71;
  params[9] = 0xF3;
  params[10] = 0x70;
  params[11] = 0x70;
  gc9a01a_write_cmd(0x63);
  gc9a01a_write_data(params, 12);

  params[0] = 0x28;
  params[1] = 0x29;
  params[2] = 0xF1;
  params[3] = 0x01;
  params[4] = 0xF1;
  params[5] = 0x00;
  params[6] = 0x07;
  gc9a01a_write_cmd(0x64);
  gc9a01a_write_data(params, 7);

  params[0] = 0x3C;
  params[1] = 0x00;
  params[2] = 0xCD;
  params[3] = 0x67;
  params[4] = 0x45;
  params[5] = 0x45;
  params[6] = 0x10;
  params[7] = 0x00;
  params[8] = 0x00;
  params[9] = 0x00;
  gc9a01a_write_cmd(0x66);
  gc9a01a_write_data(params, 10);

  params[0] = 0x00;
  params[1] = 0x3C;
  params[2] = 0x00;
  params[3] = 0x00;
  params[4] = 0x00;
  params[5] = 0x01;
  params[6] = 0x54;
  params[7] = 0x10;
  params[8] = 0x32;
  params[9] = 0x98;
  gc9a01a_write_cmd(0x67);
  gc9a01a_write_data(params, 10);

  params[0] = 0x10;
  params[1] = 0x85;
  params[2] = 0x80;
  params[3] = 0x00;
  params[4] = 0x00;
  params[5] = 0x4E;
  params[6] = 0x00;
  gc9a01a_write_cmd(0x74);
  gc9a01a_write_data(params, 7);

  params[0] = 0x3E;
  params[1] = 0x07;
  gc9a01a_write_cmd(0x98);
  gc9a01a_write_data(params, 2);

  params[0] = 0x3E;
  params[1] = 0x07;
  gc9a01a_write_cmd(0x99);
  gc9a01a_write_data(params, 2);

  params[0] = 0x00;
  gc9a01a_write_cmd(GC9A01A_TEON);          ///< Tearing Effect Line ON
  gc9a01a_write_data(params, 1);

  gc9a01a_write_cmd(GC9A01A_INVON);         ///< Display Inversion ON
  HAL_Delay(120);
  gc9a01a_write_cmd(GC9A01A_SLPOUT);        ///< Sleep Out
  HAL_Delay(120);
  gc9a01a_write_cmd(GC9A01A_DISPON);        ///< Display ON
  HAL_Delay(20);


  //GC9A01A_CS_HIGH;
}


/**
  * @brief  Fills a rectangle on the GC9A01A LCD display with a specified color.
  * @param  x: X-coordinate of the top-left corner of the rectangle.
  * @param  w: Width of the rectangle.
  * @param  y: Y-coordinate of the top-left corner of the rectangle.
  * @param  h: Height of the rectangle.
  * @param  color: Color to fill the rectangle with.
  * @retval None
  */
static void gc9a01a_fill_rect(uint16_t x, uint16_t w, uint16_t y, uint16_t h, uint16_t color) {
  if((x >= BSP_LCD_ACTIVE_WIDTH) || (y >= BSP_LCD_ACTIVE_HEIGHT)) return;
  if((x + w - 1) >= BSP_LCD_ACTIVE_WIDTH) w = BSP_LCD_ACTIVE_WIDTH - x;
  if((y + h - 1) >= BSP_LCD_ACTIVE_HEIGHT) h = BSP_LCD_ACTIVE_HEIGHT - y;
  //GC9A01A_CS_LOW;
  gc9a01a_set_address_window(x,(x+w-1), y, (y+h-1));
  uint8_t data[] = { color >> 8, color & 0xFF };

  for(y = h; y > 0; y--) {
    for(x = w; x > 0; x--) {
      GC9A01A_WRITE_8BIT(data[0]);
      GC9A01A_WRITE_8BIT(data[1]);
    }
  }
  //GC9A01A_CS_HIGH;
}

/**
 * Set an area for drawing on the display with start row, end row and start col, end col.
 * User don't need to call it usually, call it only before some functions who don't call it by default.
 * @param x0 start column address.
 * @param x1 end column address.
 * @param y0 start row address.
 * @param y1 end row address.
 */
static void gc9a01a_set_address_window(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1) {
  // Send commands to set column address (x2 to x2)
  GC9A01A_DC_CMD;
  GC9A01A_WRITE_8BIT(GC9A01A_CASET); // Column address set command
  GC9A01A_DC_DATA;
  GC9A01A_WRITE_8BIT((uint8_t)(x0 >> 8));
  GC9A01A_WRITE_8BIT((uint8_t)x0);
  GC9A01A_WRITE_8BIT((uint8_t)(x1 >> 8));
  GC9A01A_WRITE_8BIT((uint8_t)x1);

  // Send commands to set row address (y2 to y2)
  GC9A01A_DC_CMD;
  GC9A01A_WRITE_8BIT(GC9A01A_RASET); // Row address set command
  GC9A01A_DC_DATA;
  GC9A01A_WRITE_8BIT((uint8_t)(y0 >> 8));
  GC9A01A_WRITE_8BIT((uint8_t)y0);
  GC9A01A_WRITE_8BIT((uint8_t)(y1 >> 8));
  GC9A01A_WRITE_8BIT((uint8_t)y1);

  // Send command to write to RAM
  GC9A01A_DC_CMD;
  GC9A01A_WRITE_8BIT(GC9A01A_RAMWR); // Write to RAM command
  GC9A01A_DC_DATA;
}


/**
  * @brief  Sends a command to the GC9A01A LCD display.
  * @param  cmd: The command byte to be sent to the display.
  * @retval None
  */
static void gc9a01a_write_cmd(uint8_t cmd) {
  GC9A01A_DC_CMD;
  GC9A01A_WRITE_8BIT(cmd);
}


/**
  * @brief  Sends a buffer of data to the GC9A01A LCD display.
  * @param  buff: Pointer to the buffer containing the data to be sent.
  * @param  buff_size: Size of the buffer.
  * @retval None
  */
static void gc9a01a_write_data(uint8_t* buff, uint32_t buff_size) {
  GC9A01A_DC_DATA;

  for(uint32_t i = 0; i < buff_size; i++) {
    GC9A01A_WRITE_8BIT(buff[i]);
  }
}

/**
  * @brief  Inverts the colors on the GC9A01A LCD display.
  * @param  invert: Boolean flag to determine whether to invert colors.
  *                 If true, colors will be inverted. If false, colors will be set to normal.
  * @retval None
  */
static void gc9a01a_invert_colors(bool invert) {
  //GC9A01A_CS_LOW;
  gc9a01a_write_cmd(invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
  //GC9A01A_CS_HIGH;
}


/**
  * @brief  Draw an full scale image on the GC9A01A LCD(240 x 240)
  * @param  data: Pointer to the image data (RGB565 format)
  * @retval None
  */
static void gc9a01a_lcd_fill_image(const uint16_t* data){
  gc9a01a_draw_image(0, GC9A01A_WIDTH, 0, GC9A01A_HEIGHT, data);
}

// /**
//   * @brief  Fills a rectangle on the GC9A01A LCD display with a specified color.
//   * @param  x: X-coordinate of the top-left corner of the rectangle.
//   * @param  w: Width of the rectangle.
//   * @param  y: Y-coordinate of the top-left corner of the rectangle.
//   * @param  h: Height of the rectangle.
//   * @param  color: Color to fill the rectangle with.
//   * @retval None
//   */
//  void gc9a01a_fill_rect(uint16_t x, uint16_t w, uint16_t y, uint16_t h, uint16_t color) {
//   if((x >= BSP_LCD_ACTIVE_WIDTH) || (y >= BSP_LCD_ACTIVE_HEIGHT)) return;
//   if((x + w - 1) >= BSP_LCD_ACTIVE_WIDTH) w = BSP_LCD_ACTIVE_WIDTH - x;
//   if((y + h - 1) >= BSP_LCD_ACTIVE_HEIGHT) h = BSP_LCD_ACTIVE_HEIGHT - y;
//   //GC9A01A_CS_LOW;
//   gc9a01a_set_address_window(x,(x+w-1), y, (y+h-1));
//   uint8_t data[] = { color >> 8, color & 0xFF };

//   for(y = h; y > 0; y--) {
//     for(x = w; x > 0; x--) {
//       GC9A01A_WRITE_8BIT(data[0]);
//       GC9A01A_WRITE_8BIT(data[1]);
//     }
//   }
//   //GC9A01A_CS_HIGH;
// }

// Create an instance of the driver structure with our implementations
const display_driver_t gc9a01a_driver = {
  .init = gc9a01a_init,
  .write_string = gc9a01a_write_string,
  .write_char = gc9a01a_write_char,
  .draw_image = gc9a01a_draw_image,
  .fill_screen = gc9a01a_fill_screen,
  .draw_pixel = gc9a01a_draw_pixel,
.set_orientation = gc9a01a_set_orientation,
.fill_rectangle = gc9a01a_fill_rect,
};