/*
 * Diaodian.cpp
 *
 *  Created on: 2021��10��30��
 *      Author: Administrator
 */

#include "Diaodian.h"
#include "Yiqi.h"
#include "II_C.h"

Diaodian::Diaodian() {
	// TODO �Զ����ɵĹ��캯�����

}

Diaodian::~Diaodian() {
	// TODO �Զ����ɵ������������
}
void Diaodian::init() {
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
//	ʱ������(Ҫ��AFIOʱ��)

//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);

//	�ⲿ�ж�ʹ�õ�GPIO������ʹ��PA�ڵ�0����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//�ⲿ�ж�ʹ�õ�GPIO����Ҳ����PB�ڵ�9�������ӵ�EXTILine9
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);

//	�����ⲿ�ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
//����NVIC
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
