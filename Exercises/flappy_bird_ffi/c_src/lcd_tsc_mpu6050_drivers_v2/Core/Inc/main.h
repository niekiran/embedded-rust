/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_CSX_Pin GPIO_PIN_4
#define LCD_CSX_GPIO_Port GPIOA
#define LCD_SCL_Pin GPIO_PIN_5
#define LCD_SCL_GPIO_Port GPIOA
#define LCD_MISO_Pin GPIO_PIN_6
#define LCD_MISO_GPIO_Port GPIOA
#define LCD_MOSI_Pin GPIO_PIN_7
#define LCD_MOSI_GPIO_Port GPIOA
#define LCD_DCX_Pin GPIO_PIN_1
#define LCD_DCX_GPIO_Port GPIOB
#define TP_RST_Pin GPIO_PIN_4
#define TP_RST_GPIO_Port GPIOB
#define TP_INT_Pin GPIO_PIN_5
#define TP_INT_GPIO_Port GPIOB
#define MPU6050_TSC_SCL_Pin GPIO_PIN_6
#define MPU6050_TSC_SCL_GPIO_Port GPIOB
#define MPU6050_TSC_SDA_Pin GPIO_PIN_7
#define MPU6050_TSC_SDA_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_8
#define LCD_RST_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
