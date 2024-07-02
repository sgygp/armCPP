/*
 * Exti8.cpp
 *
 *  Created on: 2019年4月20日
 *      Author: Administrator
 */
#include <Exti.h>
#include "Timer.h"
extern  float m_zhuansu;

void EXTI_Configration() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef a;
	a.GPIO_Pin = GPIO_Pin_4;
	a.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
	a.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &a);

	EXTI_DeInit();
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	EXTI_ClearITPendingBit(EXTI_Line4);

	EXTI_InitTypeDef b;
	b.EXTI_Line = EXTI_Line4;
	b.EXTI_Mode = EXTI_Mode_Interrupt;
	b.EXTI_Trigger = EXTI_Trigger_Falling;
	b.EXTI_LineCmd = ENABLE;
	EXTI_Init(&b);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);

}

void  getJishu(long *shuju)
{
	shuju[0]=Timer::ms5_jishu;
	shuju[1]=SystemCoreClock / Timer::FREQUENCY_HZ- SysTick->VAL;
}

bool bJisuanzhong=false;
long kaishizhi[2];
long jieshuzhi[2];
extern "C" void EXTI4_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line4) == SET) {
		EXTI_ClearITPendingBit(EXTI_Line4);
		EXTI_ClearFlag(EXTI_Line4);
		if(m_zhuansu<1)
		{
			if(!bJisuanzhong)
			{
				getJishu(kaishizhi);
				bJisuanzhong=true;
			}else if(bJisuanzhong)
			{
				getJishu(jieshuzhi);
				bJisuanzhong=false;
				long ll=0;
				ll=jieshuzhi[0]-kaishizhi[0];
				float ff=ll+((jieshuzhi[1]-kaishizhi[1])*1.0/(SystemCoreClock / Timer::FREQUENCY_HZ));
				ff*=5.0;//ff为脉冲宽度ms
				m_zhuansu=1000.0/ff;//频率
			}
		}

	}
}
