/*
 * Set.cpp
 *
 *  Created on: 2021年9月1日
 *      Author: Administrator
 */

#include "Set.h"
#include "delay.h"
Set::Set() {
	// TODO 自动生成的构造函数存根

}

Set::~Set() {
	// TODO 自动生成的析构函数存根
}

void Set::init() {

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =PINS_SET;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  		//输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //
	GPIO_Init(PORTS_SET, &GPIO_InitStructure);

}
void Set::setOn() {
	GPIO_ResetBits(PORTS_SET,PINS_SET);
	delay_ms(100);
}
