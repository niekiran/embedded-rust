#ifndef _TSC_H
#define _TSC_H

/*
 * CST816S.h
 *
 *  Created on: Jun 29, 2024
 *      Author: Shreyas Acharya, BHARATI SOFTWARE
 */


#include "main.h"

#define TP_RESET_PIN_LOW     HAL_GPIO_WritePin(TP_RESET_GPIO_Port, TP_RESET_Pin, GPIO_PIN_RESET)
#define TP_RESET_PIN_HIGH    HAL_GPIO_WritePin(TP_RESET_GPIO_Port, TP_RESET_Pin, GPIO_PIN_SET)

// Gesture definitions
enum GESTURE {
  NONE = 0x00,
  SWIPE_UP = 0x03,
  SWIPE_DOWN = 0x04,
  SWIPE_LEFT = 0x02,
  SWIPE_RIGHT = 0x01,
  SINGLE_CLICK = 0x05,
  DOUBLE_CLICK = 0x0B,
  LONG_PRESS = 0x0C
};

struct touch_data {
  uint8_t gestureID;       // Gesture ID
  uint8_t points;          // Number of touch points
  uint8_t event;           // Event (0 = Down, 1 = Up, 2 = Contact)
  int x;                   // X coordinate
  int y;                   // Y coordinate
  uint8_t version;         // Version information
  uint8_t versionInfo[3];  // Additional version information
};

// Function prototypes
void CST816S_read_touch(I2C_HandleTypeDef *hi2c);
void CST816S_init(I2C_HandleTypeDef *hi2c);
const char* CST816S_gesture();
struct touch_data* CST816S_get_data(void);
uint8_t CST816S_data_available(I2C_HandleTypeDef *hi2c);

#endif 
