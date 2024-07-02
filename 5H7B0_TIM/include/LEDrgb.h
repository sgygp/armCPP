/*
 * LEDrgb.h
 *
 *  Created on: 2024��3��28��
 *      Author: Administrator
 */

#ifndef LEDRGB_H_
#define LEDRGB_H_
#include "stm32h7xx.h"

#define LED_GPIO_Port	GPIOC

#define RED_Pin 				GPIO_PIN_0
#define GREEN_Pin			GPIO_PIN_1
#define BLUE_Pin				GPIO_PIN_2

//#define LED_R(n)			(n?HAL_GPIO_WritePin(RED_GPIO_Port,RED_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(RED_GPIO_Port,RED_Pin,GPIO_PIN_RESET))
//#define LED_R_TogglePin		HAL_GPIO_TogglePin(RED_GPIO_Port,RED_Pin)	//LED_R��ƽ��ת
//
//#define LED_G(n)			(n?HAL_GPIO_WritePin(GREEN_GPIO_Port,GREEN_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(GREEN_GPIO_Port,GREEN_Pin,GPIO_PIN_RESET))
//#define LED_G_TogglePin     HAL_GPIO_TogglePin(GREEN_GPIO_Port,GREEN_Pin)	//LED_G��ƽ��ת
//
//#define LED_B(n)			(n?HAL_GPIO_WritePin(BLUE_GPIO_Port,BLUE_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(BLUE_GPIO_Port,BLUE_Pin,GPIO_PIN_RESET))
//#define LED_B_TogglePin     HAL_GPIO_TogglePin(BLUE_GPIO_Port,BLUE_Pin)	//LED_B��ƽ��ת

class LEDrgb {
public:
	void init();
	void run();

	void LED_R(u8 b);//0����ͣ�1����ߣ�2��ƽ��ת
	void LED_G(u8 b);
	void LED_B(u8 b);
};

#endif /* LEDRGB_H_ */
