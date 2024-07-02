/*
 * Pwm_Timer2.cpp
 *
 *  Created on: 2019��4��24��
 *      Author: Administrator
 */
#include <Timer2PWM.h>


void Timer2PWM::init( u16 psc,u16 arr) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	 //ʹ�ܶ�ʱ��2ʱ��

	//���Ź�������
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2; //TIM2_CH1 TIM2_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  //���������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	//��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_OCStructInit(&TIM_OCInitStructure);
	//��ʼ��TIM2 Channel2��Channel3 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 5000;	//����ռ�ձ�ʱ��
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC3

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
}

void Timer2PWM::outPWM3(u16 x)
{
	if(x>10000)
		x=10000;
	TIM_SetCompare3(TIM2, x);

}


