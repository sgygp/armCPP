/*
 * LEDrgb.cpp
 *
 *  Created on: 2024年3月28日
 *      Author: Administrator
 */

#include "main.h"
#include "LEDrgb.h"
#include "string.h"


void LEDrgb::init() {
	  /*Configure GPIO pin Output Level */
	  GPIO_InitTypeDef GPIO_InitStruct;
	  memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));
	  HAL_GPIO_WritePin(LED_GPIO_Port, RED_Pin|GREEN_Pin|BLUE_Pin, GPIO_PIN_SET);

	  /*Configure GPIO pins : PCPin PCPin PCPin */
	  GPIO_InitStruct.Pin = RED_Pin|GREEN_Pin|BLUE_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
}

void LEDrgb::run() {
	static u8 color=0;
	switch(color%7)
	{
	case 0://红
		LED_R(0);
		LED_G(1);
		LED_B(1);
		break;
	case 1://黄
		LED_R(0);
		LED_G(0);
		LED_B(1);
		break;
	case 2://绿
		LED_R(1);
		LED_G(0);
		LED_B(1);
		break;
	case 3:	//白
		LED_R(0);
		LED_G(0);
		LED_B(0);
		break;
	case 4://青
		LED_R(1);
		LED_G(0);
		LED_B(0);
		break;
	case 5://蓝
		LED_R(1);
		LED_G(1);
		LED_B(0);
		break;
	case 6://紫
		LED_R(0);
		LED_G(1);
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
