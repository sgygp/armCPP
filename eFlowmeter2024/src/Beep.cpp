/*
 * SSR.cpp
 *
 *  Created on: 2020��1��30��
 *      Author: Administrator
 */
#include "Beep.h"

void Beep::init() {

	GPIO_InitTypeDef GPIO_InitStructure; //��Ϊ�����ע�⣺2MHZ
	GPIO_InitStructure.GPIO_Pin = PINS_BEEP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORTS_BEEP, &GPIO_InitStructure);

	turnOff();
}

