/*
 * Pwm_Timer2.cpp
 *
 *  Created on: 2019��4��24��
 *      Author: Administrator
 */
#include <TimerPWM.h>

void TimerPWM::init(u16 psc, u16 arr) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8, ENABLE);	//ʹ�ܶ�ʱ��1ʱ��

	//���ø�����Ϊ�����������,���TIM2 CH1��PWM���岨��	GPIOA.0
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  //��ʼ��GPIO

	//��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = arr - 1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_OCStructInit(&TIM_OCInitStructure);
	//��ʼ��TIM Channel 1-4 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC1
//	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC2
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC3
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC4

//	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM1_CH1�ϵ�Ԥװ�ؼĴ���
//	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM1_CH2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM1_CH3�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM1_CH4�ϵ�Ԥװ�ؼĴ���

	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM1
	TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_CtrlPWMOutputs(TIM8, ENABLE);        //MOE �����ʹ��,�߼���ʱ�����뿪�����

}
void TimerPWM::outPWM(u8 no,u16 x) {
	if (x > 10000)
		x = 10000;
	switch(no)
	{


	case 0:
		TIM_SetCompare4(TIM8, x);
		break;
	case 1:
		TIM_SetCompare3(TIM8, x);
		break;
/*	case 2:
		TIM_SetCompare2(TIM8, x);
		break;
	case 3:
		TIM_SetCompare1(TIM8, x);
		break;
*/	}


}
