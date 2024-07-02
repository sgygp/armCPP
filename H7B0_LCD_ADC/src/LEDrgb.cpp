/*
 * LEDrgb.cpp
 *
 *  Created on: 2024年3月28日
 *      Author: Administrator
 */

#include "main.h"
#include "LEDrgb.h"


void LEDrgb::init() {
}

void LEDrgb::run() {
	static u8 color=0;
	switch(color%7)
	{
	case 0:
		LED_R(0);
		LED_G(1);
		LED_B(1);
		break;
	case 1:
		LED_R(1);
		LED_G(0);
		LED_B(1);
		break;
	case 2:
		LED_R(1);
		LED_G(1);
		LED_B(0);
		break;
	case 3:
		LED_R(0);
		LED_G(0);
		LED_B(1);
		break;
	case 4:
		LED_R(0);
		LED_G(1);
		LED_B(0);
		break;
	case 5:
		LED_R(1);
		LED_G(0);
		LED_B(0);
		break;
	case 6:
		LED_R(0);
		LED_G(0);
		LED_B(0);
		break;
	default:
		break;
	}
	color++;
}

void LEDrgb::LED_R(u8 b) {
	if(b==0)
		HAL_GPIO_WritePin(LED_GPIO_Port,RED_Pin,GPIO_PIN_RESET);
	else if(b==1)
		HAL_GPIO_WritePin(LED_GPIO_Port,RED_Pin,GPIO_PIN_SET);
	else if(b==2)
		HAL_GPIO_TogglePin(LED_GPIO_Port,RED_Pin);//LED_R电平翻转
}

void LEDrgb::LED_G(u8 b) {
	if(b==0)
		HAL_GPIO_WritePin(LED_GPIO_Port,GREEN_Pin,GPIO_PIN_RESET);
	else if(b==1)
		HAL_GPIO_WritePin(LED_GPIO_Port,GREEN_Pin,GPIO_PIN_SET);
	else if(b==2)
		HAL_GPIO_TogglePin(LED_GPIO_Port,GREEN_Pin);//LED_R电平翻转
}

void LEDrgb::LED_B(u8 b) {
	if(b==0)
		HAL_GPIO_WritePin(LED_GPIO_Port,BLUE_Pin,GPIO_PIN_RESET);
	else if(b==1)
		HAL_GPIO_WritePin(LED_GPIO_Port,BLUE_Pin,GPIO_PIN_SET);
	else if(b==2)
		HAL_GPIO_TogglePin(LED_GPIO_Port,BLUE_Pin);//LED_R电平翻转
}
