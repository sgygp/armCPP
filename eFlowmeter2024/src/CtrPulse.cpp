/*
 * CtrPulse.cpp
 *
 *  Created on: 2021年10月8日
 *      Author: Administrator
 */

#include "diag/Trace.h"
#include <CtrPulse.h>
u8 CtrPulse::m_fangxiang;
u16 CtrPulse::m_arr=0;
void CtrPulse::on0(){
	if (m_fangxiang) {
		GPIO_ResetBits(SELECT_PORT, SELECT_L);
		GPIO_ResetBits(SELECT_PORT, SELECT_H);
	} else {
		GPIO_SetBits(SELECT_PORT, SELECT_L);
		GPIO_ResetBits(SELECT_PORT, SELECT_H);

	}
}
void CtrPulse::on1(){
	if (m_fangxiang) {
		GPIO_SetBits(SELECT_PORT, SELECT_L);
		GPIO_ResetBits(SELECT_PORT, SELECT_H);
	} else {
		GPIO_ResetBits(SELECT_PORT, SELECT_L);
		GPIO_ResetBits(SELECT_PORT, SELECT_H);

	}
}
void CtrPulse::off(){
	GPIO_SetBits(SELECT_PORT, SELECT_H);
	GPIO_SetBits(SELECT_PORT, SELECT_L);
}

void CtrPulse::init(u16 psc, u16 arr) {
	m_arr=arr;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SELECT_L | SELECT_H;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SELECT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3 ,ENABLE );//TIM3完全映射 PC7->TIM3_CH2

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = arr - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = arr / 2;	//设置占空比时间
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing; //选择定时器模式:
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = arr / 4;	//设置定时时间
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);    //根据T指定的参数初始化外设TIM
	TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);



	TIM_Cmd(TIM3, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
extern "C" void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET) {    //比较后拉低
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		TIM3->CCR4=CtrPulse::m_arr*3/4;
		CtrPulse::off();
	}
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		TIM3->CCR4=CtrPulse::m_arr/4;
		CtrPulse::off();
	}
	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET) {    //比较后拉低
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
//		trace_printf("%u\n",TIM3->CNT);
		if(TIM3->CNT>(CtrPulse::m_arr/2))
			CtrPulse::on0();
		else
			CtrPulse::on1();

	}

}

