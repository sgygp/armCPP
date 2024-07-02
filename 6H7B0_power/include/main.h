/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
//#include "diag/Trace.h"
#include "stm32h7xx_hal.h"

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#define LED_GPIO_Port	GPIOC

#define RED_Pin 				GPIO_PIN_0
#define GREEN_Pin			GPIO_PIN_1
#define BLUE_Pin				GPIO_PIN_2

#define LCD_PWR_Pin GPIO_PIN_3
#define LCD_RST_Pin GPIO_PIN_4
#define LCD_PWR_GPIO_Port GPIOD
#define LCD_RST_GPIO_Port GPIOD


#define LCD_DC_Pin GPIO_PIN_4
#define LCD_DC_GPIO_Port GPIOB

void SystemClock_Config(void);
void MPU_Config(void);
void PeriphCommonClock_Config(void);
void Error_Handler(void);


#endif /* __MAIN_H */
