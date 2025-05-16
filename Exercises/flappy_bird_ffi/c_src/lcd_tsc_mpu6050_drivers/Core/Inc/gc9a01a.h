#ifndef _LCD_H_
#define _LCD_H_
/*
 * GC9A01A.h
 *
 *  Created on: Jun 27, 2024
 *      Author: Shreyas Acharya, BHARATI SOFTWARE
 */

#include <font.h>
#include "main.h"

#define GC9A01A_WIDTH   240
#define GC9A01A_HEIGHT  240

#define PORTRAIT        0
#define LANDSCAPE       1

#if(BSP_LCD_ORIENTATION == PORTRAIT)
    #define  BSP_LCD_ACTIVE_WIDTH           GC9A01A_WIDTH
    #define  BSP_LCD_ACTIVE_HEIGHT          GC9A01A_HEIGHT
#elif(BSP_LCD_ORIENTATION == LANDSCAPE)
    #define  BSP_LCD_ACTIVE_WIDTH           GC9A01A_HEIGHT
    #define  BSP_LCD_ACTIVE_HEIGHT          GC9A01A_WIDTH
#endif

/********************** GC9A01A commands **********************/
#define GC9A01A_NOP             0x00  ///< No operation
#define GC9A01A_SWRESET         0x01  ///< Software Reset (maybe, not documented)
#define GC9A01A_RDDID           0x04  ///< Read display identification information
#define GC9A01A_RDDST           0x09  ///< Read Display Status

#define GC9A01A_SLPIN           0x10  ///< Enter Sleep Mode
#define GC9A01A_SLPOUT          0x11  ///< Sleep Out
#define GC9A01A_PTLON           0x12  ///< Partial Mode ON
#define GC9A01A_NORON           0x13  ///< Normal Display Mode ON

#define GC9A01A_INVOFF          0x20  ///< Display Inversion OFF
#define GC9A01A_INVON           0x21  ///< Display Inversion ON
#define GC9A01A_DISPOFF         0x28  ///< Display OFF
#define GC9A01A_DISPON          0x29  ///< Display ON

#define GC9A01A_CASET           0x2A  ///< Column Address Set
#define GC9A01A_RASET           0x2B  ///< Row Address Set
#define GC9A01A_RAMWR           0x2C  ///< Memory Write

#define GC9A01A_MADCTL          0x36  ///< Memory Access Control
#define GC9A01A_COLMOD          0x3A  ///< Pixel Format Set

#define GC9A01A1_POWER1         0xC1  ///< Power Control 1
#define GC9A01A1_POWER2         0xC3  ///< Power Control 2
#define GC9A01A1_POWER3         0xC4  ///< Power Control 3
#define GC9A01A1_POWER4         0xC9  ///< Power Control 4
#define GC9A01A1_POWER7         0xA7  ///< Power Control 7

#define GC9A01A_FRAMERATE       0xE8  ///< Frame rate control

#define GC9A01A_INREGEN1        0xFE  ///< Inter register enable 1
#define GC9A01A_INREGEN2        0xEF  ///< Inter register enable 2

#define GC9A01A_GAMMA1          0xF0  ///< Set gamma 1
#define GC9A01A_GAMMA2          0xF1  ///< Set gamma 2
#define GC9A01A_GAMMA3          0xF2  ///< Set gamma 3
#define GC9A01A_GAMMA4          0xF3  ///< Set gamma 4

#define GC9A01A_TEOFF           0x34  ///< Tearing Effect Line OFF
#define GC9A01A_TEON            0x35  ///< Tearing Effect Line ON

#define GC9A01A_DISP_CTRL       0xB6  ///<Display Function Control

#define MADCTL_MY               0x80  ///< Bottom to top
#define MADCTL_MX               0x40  ///< Right to left
#define MADCTL_MV               0x20  ///< Reverse Mode
#define MADCTL_ML               0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB              0x00  ///< Red-Green-Blue pixel order
#define MADCTL_BGR              0x08  ///< Blue-Green-Red pixel order
#define MADCTL_MH               0x04  ///< LCD refresh right to left


/********************** Color definitions **********************/
#define VIOLET                  0x801F  ///< RGB: 148, 0, 211
#define INDIGO                  0x480F  ///< RGB: 75, 0, 130
#define BLUE                    0x001F  ///< RGB: 0, 0, 255
#define GREEN                   0x07E0  ///< RGB: 0, 255, 0
#define YELLOW                  0xFFE0  ///< RGB: 255, 255, 0
#define ORANGE                  0xFC00  ///< RGB: 255, 165, 0
#define RED                     0xF800  ///< RGB: 255, 0, 0
#define WHITE                   0xFFFF  ///< RGB: 255, 255, 255
#define BLACK                   0x0000  ///< RGB: 0, 0, 0
#define NAVY                    0x000F  ///< RGB: 0, 0, 128
#define DARKGREEN               0x03E0  ///< RGB: 0, 128, 0
#define DARKCYAN                0x03EF  ///< RGB: 0, 128, 128
#define MAROON                  0x7800  ///< RGB: 128, 0, 0
#define PURPLE                  0x780F  ///< RGB: 128, 0, 128
#define OLIVE                   0x7BE0  ///< RGB: 128, 128, 0
#define LIGHTGREY               0xC618  ///< RGB: 192, 192, 192
#define DARKGREY                0x7BEF  ///< RGB: 128, 128, 128
#define CYAN                    0x07FF  ///< RGB: 0, 255, 255
#define MAGENTA                 0xF81F  ///< RGB: 255, 0, 255
#define GREENYELLOW             0xAFE5  ///< RGB: 173, 255, 47
#define PINK                    0xF81F  ///< RGB: 255, 0, 255
#define GC9A01A_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))


#define BL_A_GPIO_Port 		LCD_BL_A_GPIO_Port
#define DCX_GPIO_Port  		LCD_DCX_GPIO_Port
//#define LCD_CSX_GPIO_Port 	LCD_CSX_GPIO_Port
#define LCD_RST_GPIO_Port 	LCD_RESET_GPIO_Port
#define WRD_GPIO_Port 		LCD_WRX_GPIO_Port

#define DCX_Pin 			LCD_DCX_Pin
#define BL_A_Pin 			LCD_BL_A_Pin
//#define LCD_CSX_Pin 		LCD_CSX_Pin
#define LCD_RST_Pin 		LCD_RESET_Pin
#define WRD_Pin				LCD_WRX_Pin

#define GC9A01A_BL_ON           (BL_A_GPIO_Port->BSRR = BL_A_Pin)

#define GC9A01A_DC_CMD          (DCX_GPIO_Port->BSRR = (DCX_Pin << 16)) // Reset pin
#define GC9A01A_DC_DATA         (DCX_GPIO_Port->BSRR = DCX_Pin) // Set pin

#define GC9A01A_CS_LOW          (LCD_CSX_GPIO_Port->BSRR = (LCD_CSX_Pin << 16)) // Reset pin
#define GC9A01A_CS_HIGH         (LCD_CSX_GPIO_Port->BSRR = LCD_CSX_Pin) // Set pin

#define GC9A01A_RST_LOW         (LCD_RST_GPIO_Port->BSRR = (LCD_RST_Pin << 16)) // Reset pin
#define GC9A01A_RST_HIGH        (LCD_RST_GPIO_Port->BSRR = LCD_RST_Pin) // Set pin

#define GC9A01A_RD_LOW          (LCD_RDX_GPIO_Port->BSRR = (LCD_RDX_Pin << 16)) // Reset pin
#define GC9A01A_RD_HIGH         (LCD_RDX_GPIO_Port->BSRR = LCD_RDX_Pin) // Set pin
#define GC9A01A_RD_STROBE       { GC9A01A_RD_LOW; GC9A01A_RD_HIGH; }

#define GC9A01A_WR_LOW          (WRD_GPIO_Port->BSRR = (WRD_Pin << 16)) // Reset pin
#define GC9A01A_WR_HIGH         (WRD_GPIO_Port->BSRR = WRD_Pin) // Set pin
#define GC9A01A_WR_STROBE       { GC9A01A_WR_LOW; GC9A01A_WR_HIGH; }

//#define GC9A01A_TE_LOW          (LCD_TE_GPIO_Port->BSRR = (LCD_TE_Pin << 16)) // Reset pin

#define GC9A01A_WRITE_8BIT(d) { \
  GPIOA->BSRR = (0b0000011000000000 << 16); /* Clear PA9 and PA10 */ \
  GPIOB->BSRR = (0b1111110000000000 << 16); /* Clear PB10 to PB15 */ \
  GPIOA->BSRR = (((d) & (1<<0)) << 10)  /* Set PA10 if bit 0 of d is 1 */ \
              | (((d) & (1<<1)) << 8);  /* Set PA9 if bit 1 of d is 1 */ \
  GPIOB->BSRR = (((d) & (1<<2)) << 13)  /* Set PB15 if bit 2 of d is 1 */ \
              | (((d) & (1<<3)) << 11)  /* Set PB14 if bit 3 of d is 1 */ \
              | (((d) & (1<<4)) << 9)   /* Set PB13 if bit 4 of d is 1 */ \
              | (((d) & (1<<5)) << 7)   /* Set PB12 if bit 5 of d is 1 */ \
              | (((d) & (1<<6)) << 5)   /* Set PB11 if bit 6 of d is 1 */ \
              | (((d) & (1<<7)) << 3);  /* Set PB10 if bit 7 of d is 1 */ \
  GC9A01A_WR_STROBE; \
}

#define GC9A01A_WRITE_8BIT_1(d) { \
  /* Clear PA9 and PA10 */ \
  GPIOA->BSRR = (0b0000011000000000 << 16); \
  /* Clear PB8 to PB13 */ \
  GPIOB->BSRR = (0b1111110000000000 << 16); \
  \
  /* Set PA10 and PA9 based on d's bit 0 and bit 1 */ \
  GPIOA->BSRR = ((d & (1 << 0)) ? (1 << 10) : 0)  /* PA10 = d[0] */ \
              | ((d & (1 << 1)) ? (1 << 9) : 0);  /* PA9 = d[1] */ \
  \
  /* Set PB8 to PB13 based on d's bit 2 to bit 7 */ \
  GPIOB->BSRR = ((d & (1 << 2)) ? (1 << 8) : 0)   /* PB8 = d[2] */ \
              | ((d & (1 << 3)) ? (1 << 9) : 0)   /* PB9 = d[3] */ \
              | ((d & (1 << 4)) ? (1 << 10) : 0)  /* PB10 = d[4] */ \
              | ((d & (1 << 5)) ? (1 << 11) : 0)  /* PB11 = d[5] */ \
              | ((d & (1 << 6)) ? (1 << 12) : 0)  /* PB12 = d[6] */ \
              | ((d & (1 << 7)) ? (1 << 13) : 0); /* PB13 = d[7] */ \
  \
  /* Strobe function (assuming it latches data) */ \
  GC9A01A_WR_STROBE; \
}



#endif 
