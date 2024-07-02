/*
 * Bujindianji.cpp
 *
 *  Created on: 2019年5月9日
 *      Author: Administrator
 */
#include "Bujindianji.h"
u32 Bujindianji::m_curStep;
u8 Bujindianji::pABCD,Bujindianji::pinA, Bujindianji::pinB, Bujindianji::pinC, Bujindianji::pinD;

void Bujindianji::init() {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = A1 | A2| B1 | B2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(_DIANJIPORT_, &GPIO_InitStructure);
	m_curStep = 0;
	NVIC_InitTypeDef NVIC_InitStructure;
	//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	SetTime(72, 800);
	//开启中断
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	//清除中断标志位
	//开启外设
	TIM_Cmd(TIM4, ENABLE);

}


void Bujindianji::Run() {

	switch (m_curStep % 8) {
	case 0:
		Set0(A2);
		Set0(B1);
		Set0(B2);
		Set1(A1);

		break;
	case 1:
		Set0(A2);
		Set0(B2);
		Set1(A1);
		Set1(B1);
		break;
	case 2:
		Set0(A1);
		Set0(A2);
		Set0(B2);
		Set1(B1);
		break;
	case 3:
		Set0(A1);
		Set0(B2);
		Set1(A2);
		Set1(B1);
		break;
	case 4:
		Set0(A1);
		Set0(B1);
		Set0(B2);
		Set1(A2);
		break;
	case 5:
		Set0(A1);
		Set0(B1);
		Set1(A2);
		Set1(B2);
		break;
	case 6:
		Set0(A1);
		Set0(A2);
		Set0(B1);
		Set1(B2);
		break;
	case 7:
		Set0(A2);
		Set0(B1);
		Set1(A1);
		Set1(B2);
		break;
	default:
		break;
	}
	m_curStep = (m_curStep + 1) % 8;
}
void Bujindianji::SetTime(u16 clk, u16 load)
{
//	TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //TIM_ClearFlag(TIM4,TIM_FLAG_Update);//两者作用相同

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//自动装载的周期值0-0xffff，72M/36000=2000
	TIM_TimeBaseStructure.TIM_Period = load - 1;
	//时钟除数预分频值0-oxffff，下面是36000分频
	TIM_TimeBaseStructure.TIM_Prescaler = clk - 1;
	//时钟分割，暂时为0，高级应用才用
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//计数模式，向上下，中央对齐123
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);


}

extern "C" void TIM4_IRQHandler()
{
	//判断TIM4更新中断是否发生
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		//必须清除标志位
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		Bujindianji::Run();
	}

}

