/*
 * SSR.cpp
 *
 *  Created on: 2020年1月30日
 *      Author: Administrator
 */
#include "Beep.h"
#include "delay.h"

void Beep::init() {

	GPIO_InitTypeDef GPIO_InitStructure;

	// Configure pin in output push/pull mode
	GPIO_InitStructure.GPIO_Pin = PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORTS, &GPIO_InitStructure);
	// Start with led turned off
	Beep::turnOn();
	delay_ms(250); //开机响
	Beep::turnOff();
}

