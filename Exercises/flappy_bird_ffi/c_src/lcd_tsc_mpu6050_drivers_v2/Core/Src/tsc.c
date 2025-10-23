/*
 * CST816S.c
 *
 *  Created on: Jun 29, 2024
 *      Author: Shreyas Acharya, BHARATI SOFTWARE
 */
#include "tsc.h"

#define CST816S_ADDRESS     0x15

static uint8_t is_touch_detected = 0;
static struct touch_data data;

#define TP_IN_Pin TP_INT_Pin

/**
  * @brief  Reads data from the CST816S touch sensor over I2C.
  * @param  addr: I2C address of the CST816S device.
  * @param  reg_addr: Address of the register to read from.
  * @param  reg_data: Pointer to a buffer where the read data will be stored.
  * @param  length: Number of bytes to read.
  * @retval None
  */
static void CST816S_i2c_read(I2C_HandleTypeDef *hi2c, uint16_t addr, uint8_t reg_addr, uint8_t *reg_data, size_t length) {
  HAL_I2C_Mem_Read(hi2c, addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, length, HAL_MAX_DELAY);
}

/**
  * @brief  Writes data to the CST816S touch sensor over I2C.
  * @param  addr: I2C address of the CST816S device.
  * @param  reg_addr: Address of the register to write to.
  * @param  reg_data: Pointer to the data to be written.
  * @param  length: Number of bytes to write.
  * @retval None
  */
static void CST816S_i2c_write(I2C_HandleTypeDef *hi2c, uint8_t addr, uint8_t reg_addr, const uint8_t *reg_data, size_t length) {
  HAL_I2C_Mem_Write(hi2c, addr << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)reg_data, length, HAL_MAX_DELAY);
}

/**
  * @brief  Reads touch data from the CST816S touch sensor.
  * @retval None
  */
void CST816S_read_touch(I2C_HandleTypeDef *hi2c) {
  uint8_t data_raw[8];
  uint8_t reg_addr = 0x01;

  CST816S_i2c_read(hi2c, CST816S_ADDRESS, reg_addr, (uint8_t *)data_raw, 6);

  data.gestureID = data_raw[0];
  data.points = data_raw[1];
  data.event = data_raw[2] >> 6;
  data.x = ((data_raw[2] & 0xF) << 8) + data_raw[3];
  data.y = ((data_raw[4] & 0xF) << 8) + data_raw[5];
}

/**
  * @brief  Returns a pointer to the structure containing the latest touch data.
  * @retval Pointer to the touch_data structure containing the latest touch information.
  */
struct touch_data* CST816S_get_data(void) {
  return &data;
}

/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if(GPIO_Pin == TP_IN_Pin) {
    is_touch_detected = 1;
  }
}

void CST816S_hw_reset(void) {
  HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(50);
  HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(50);
}

/**
  * @brief  Performs a hardware reset of the CST816S touch sensor.
  *         This function assumes GPIO pins and delays are configured appropriately.
  * @retval None
  */
void CST816S_init(I2C_HandleTypeDef *hi2c) {
  CST816S_hw_reset();

  CST816S_i2c_read(hi2c, CST816S_ADDRESS, 0x15, &data.version, 1);
  HAL_Delay(5);
  CST816S_i2c_read(hi2c, CST816S_ADDRESS, 0xA7, data.versionInfo, 3);

  if(data.versionInfo[0] == 0xB4){
    printf("CST86S detected.\n\t");
  } else {
    printf("CST86S not detected.\n\t");
  }
  HAL_Delay(50);
}

/**
  * @brief  Checks if touch data is available from the CST816S touch sensor.
  * @retval 1 if touch data is available, 0 otherwise.
  */
uint8_t CST816S_data_available(I2C_HandleTypeDef *hi2c) {
  uint8_t data_flag = 0;
  HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
  if (is_touch_detected) {
    CST816S_read_touch(hi2c);
    is_touch_detected = 0;
    data_flag = 1;
  } else {
    data_flag = 0;
  }
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  return data_flag;
}


/**
  * @brief  Maps gesture IDs from the CST816S touch sensor to corresponding string representations.
  * @retval Pointer to a constant string representing the gesture.
  */
const char* CST816S_gesture() {
  switch (data.gestureID) {
  case NONE:
    return "NONE        ";
  case SWIPE_DOWN:
    return "SWIPE DOWN  ";
  case SWIPE_UP:
    return "SWIPE UP    ";
  case SWIPE_LEFT:
    return "SWIPE LEFT  ";
  case SWIPE_RIGHT:
    return "SWIPE RIGHT ";
  case SINGLE_CLICK:
    return "SINGLE CLICK";
  case DOUBLE_CLICK:
    return "DOUBLE CLICK";
  case LONG_PRESS:
    return "LONG PRESS  ";
  default:
    return "UNKNOWN     ";
  }
}
