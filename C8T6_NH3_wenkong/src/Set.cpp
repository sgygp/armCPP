/*
 * Set.cpp
 *
 *  Created on: 2024年3月12日
 *      Author: Administrator
 */

#include "Set.h"

Set::Set() {
	// TODO 自动生成的构造函数存根

}

void Set::init() {
	GPIO_InitTypeDef GPIO_InitStructure;
	// Configure pin in output push/pull mode
	GPIO_InitStructure.GPIO_Pin = SETPIN2|SETPIN2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(SETPORT, &GPIO_InitStructure);

}

u8 Set::getSet() {
	u8 a= (GPIO_ReadInputDataBit(SETPORT,SETPIN1)<<1)|GPIO_ReadInputDataBit(SETPORT,SETPIN2);
	switch (a) {
	case 0:
		a=50;
		break;
	case 1:
		a=42;
		break;
	case 2:
		a=45;
		break;
	case 3:
		a=40;
		break;
		default:
			break;
	}
	return a;
}
