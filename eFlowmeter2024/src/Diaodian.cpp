/*
 * Diaodian.cpp
 *
 *  Created on: 2021年10月30日
 *      Author: Administrator
 */

#include "Diaodian.h"
#include "Yiqi.h"
#include "II_C.h"

Diaodian::Diaodian() {
	// TODO 自动生成的构造函数存根

}

Diaodian::~Diaodian() {
	// TODO 自动生成的析构函数存根
}
void Diaodian::init() {
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
//	时钟配置(要打开AFIO时钟)

//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);

//	外部中断使用的GPIO的配置使用PA口的0引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//外部中断使用的GPIO引脚也就是PB口的9引脚连接到EXTILine9
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);

//	配置外部中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//配置NVIC
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

extern "C" void EXTI2_IRQHandler(void) {
//	if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
		Yiqi::WriteLeiji() ;
		EXTI_ClearITPendingBit(EXTI_Line2);
//	}

}
