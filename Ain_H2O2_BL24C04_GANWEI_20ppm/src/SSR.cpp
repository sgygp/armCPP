/*
 * SSR.cpp
 *
 *  Created on: 2020Äê1ÔÂ30ÈÕ
 *      Author: Administrator
 */
#include "SSR.H"

void SSR_gutaijidianqi::init() {

	GPIO_InitTypeDef GPIO_InitStructure;
	// Configure pin in output push/pull mode
	GPIO_InitStructure.GPIO_Pin = SSRPIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SSRPORT, &GPIO_InitStructure);


	// Start with led turned off
	turnOff();
}

