/*
 * Set.cpp
 *
 *  Created on: 2021��9��1��
 *      Author: Administrator
 */

#include "Set.h"
#include "delay.h"
Set::Set() {
	// TODO �Զ����ɵĹ��캯�����

}

Set::~Set() {
	// TODO �Զ����ɵ������������
}

void Set::init() {

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =PINS_SET;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  		//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //
	GPIO_Init(PORTS_SET, &GPIO_InitStructure);

}
void Set::setOn() {
	GPIO_ResetBits(PORTS_SET,PINS_SET);
	delay_ms(100);
}
