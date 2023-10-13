/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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

//show main page
void main_show(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_KEY_Pin GPIO_PIN_0
#define USER_KEY_GPIO_Port GPIOA
#define SIG1_Pin GPIO_PIN_0
#define SIG1_GPIO_Port GPIOB
#define SIG1_EXTI_IRQn EXTI0_IRQn
#define SIG2_Pin GPIO_PIN_1
#define SIG2_GPIO_Port GPIOB
#define SIG2_EXTI_IRQn EXTI1_IRQn
#define ALARM_Pin GPIO_PIN_2
#define ALARM_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_12
#define LED_GREEN_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_13
#define LED_RED_GPIO_Port GPIOB
#define NUM_KEY_I4_Pin GPIO_PIN_14
#define NUM_KEY_I4_GPIO_Port GPIOB
#define NUM_KEY_I3_Pin GPIO_PIN_15
#define NUM_KEY_I3_GPIO_Port GPIOB
#define NUM_KEY_I2_Pin GPIO_PIN_8
#define NUM_KEY_I2_GPIO_Port GPIOA
#define NUM_KEY_I1_Pin GPIO_PIN_9
#define NUM_KEY_I1_GPIO_Port GPIOA
#define NUM_KEY_O3_Pin GPIO_PIN_10
#define NUM_KEY_O3_GPIO_Port GPIOA
#define NUM_KEY_O2_Pin GPIO_PIN_11
#define NUM_KEY_O2_GPIO_Port GPIOA
#define NUM_KEY_O1_Pin GPIO_PIN_12
#define NUM_KEY_O1_GPIO_Port GPIOA
#define FLOW_NUM_KEY_Pin GPIO_PIN_15
#define FLOW_NUM_KEY_GPIO_Port GPIOA
#define FLOW_NUM_KEY_EXTI_IRQn EXTI15_10_IRQn
#define ENTER_KEY_Pin GPIO_PIN_3
#define ENTER_KEY_GPIO_Port GPIOB
#define TIME_LIMIT_KEY_Pin GPIO_PIN_4
#define TIME_LIMIT_KEY_GPIO_Port GPIOB
#define TIME_LIMIT_KEY_EXTI_IRQn EXTI4_IRQn
#define FLOW_LIMIT_KEY_Pin GPIO_PIN_5
#define FLOW_LIMIT_KEY_GPIO_Port GPIOB
#define FLOW_LIMIT_KEY_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
