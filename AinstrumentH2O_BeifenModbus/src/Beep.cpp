/*
 * SSR.cpp
 *
 *  Created on: 2020Äê1ÔÂ30ÈÕ
 *      Author: Administrator
 */
#include "Beep.h"

void Beep::init() {
	// Enable GPIO Peripheral clock
	RCC_APB2PeriphClockCmd(RCC_CLK, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	// Configure pin in output push/pull mode
	GPIO_InitStructure.GPIO_Pin = PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORTS, &GPIO_InitStructure);
	// Start with led turned off
	turnOff();
}

