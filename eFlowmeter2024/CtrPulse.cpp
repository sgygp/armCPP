/*
 * CtrPulse.cpp
 *
 *  Created on: 2021��10��8��
 *      Author: Administrator
 */

#include <CtrPulse.h>
u32 CtrPulse::shangciTime;
u32 CtrPulse::benciTime;
u32 CtrPulse::jiangeTimeH;
u32 CtrPulse::jiangeTimeL;
int CtrPulse::m_jishi;

CtrPulse::CtrPulse() {
	// TODO �Զ����ɵĹ��캯�����

}

CtrPulse::~CtrPulse() {
	// TODO �Զ����ɵ������������
}

void CtrPulse::init(u16 psc, u16 arr) {
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SELECT_L | SELECT_H;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SELECT_PORT, &GPIO_InitStructure);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = arr - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ICInitTypeDef TIM3_TIM_ICInitStructure;
	TIM3_TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM3_TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //������
	TIM3_TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM3_TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //TIM �����ڲ���������ÿ̽�⵽һ������ִ��һ��
	TIM3_TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM3, &TIM3_TIM_ICInitStructure);
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	shangciTime = 0;
	m_jishi = 0;

	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = arr / 2;	//����ռ�ձ�ʱ��
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);    //����Tָ���Ĳ�����ʼ������TIM8 OC2
	TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);

	TIM_ITConfig(TIM3,
	TIM_IT_Update | TIM_IT_Trigger | TIM_IT_Break | TIM_IT_COM, DISABLE);

}
void CtrPulse::run() {
	if (m_jishi < 2)
		m_pinglv = 0;
	else
		m_pinglv = 64000.0 * 3.0 / (jiangeTimeH + jiangeTimeL);
	m_jishi = 0;

}
extern "C" void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET) {
		u32 xxx = TIM3->CNT;
		CtrPulse::off();
		CtrPulse::benciTime = TIM_GetCapture3(TIM3);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)) { //�Ѿ�������������
			TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling);
			if (CtrPulse::benciTime > CtrPulse::shangciTime)
				CtrPulse::jiangeTimeL = CtrPulse::benciTime
						- CtrPulse::shangciTime;
			else
				CtrPulse::jiangeTimeL = CtrPulse::benciTime + CTRLOAD
						- CtrPulse::shangciTime;
			xxx += (CtrPulse::jiangeTimeH / 2);
			xxx %= CTRLOAD;
			//TIM_SetCompare4(TIM3, xxx);
			TIM3->CCR4 = xxx;
		} else {
			TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising);
			if (CtrPulse::benciTime > CtrPulse::shangciTime)
				CtrPulse::jiangeTimeH = CtrPulse::benciTime
						- CtrPulse::shangciTime;
			else
				CtrPulse::jiangeTimeH = CtrPulse::benciTime + CTRLOAD
						- CtrPulse::shangciTime;
			xxx += (CtrPulse::jiangeTimeL / 2);
			xxx = xxx % CTRLOAD;
			//TIM_SetCompare4(TIM3, xxx);
			TIM3->CCR4 = xxx;
		}

		CtrPulse::shangciTime = CtrPulse::benciTime;
		CtrPulse::m_jishi++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
	}
	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET) {
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))  //�Ѿ�������������
				{
			CtrPulse::on0();
		} else {
			CtrPulse::on1();
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
	}
}

