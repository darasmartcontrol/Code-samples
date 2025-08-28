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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_tim4_ch4;
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
void fadeNeopixels();
void rainbow(uint8_t wait);
void IR_Reset(void);
extern volatile uint8_t command;
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

void CheckState(void) ;
void ledstate_define(void) ;
void manageLed(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define output_Pin GPIO_PIN_7
#define output_GPIO_Port GPIOC
#define Neopixel_regulator_Pin GPIO_PIN_8
#define Neopixel_regulator_GPIO_Port GPIOC
#define CS_Pin_Pin GPIO_PIN_15
#define CS_Pin_GPIO_Port GPIOA
#define tim3_ch4_IR_Pin GPIO_PIN_7
#define tim3_ch4_IR_GPIO_Port GPIOB
#define tim4_ch4_neopixels_Pin GPIO_PIN_9
#define tim4_ch4_neopixels_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
