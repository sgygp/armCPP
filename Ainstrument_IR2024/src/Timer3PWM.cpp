/*
 * Pwm_Timer2.cpp
 *
 *  Created on: 2019��4��24��
 *      Author: Administrator
 */
#include <Timer3PWM.h>


void Timer3PWM::init( float  pinlv) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	 //ʹ�ܶ�ʱ��3ʱ��

	//���Ź�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM3_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //���������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	setPinlv(pinlv);
	TIM_OCStructInit(&TIM_OCInitStructure);
	//��ʼ��TIM3 Channel3��Channel4 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 5000;	//����ռ�ձ�ʱ��

	TIM_OC2Init(TIM3, &TIM_OCInitStructure);    //����Tָ���Ĳ�����ʼ������TIM3 OC3
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM2
}
void Timer3PWM:: setPinlv(float  pinlv)
{
	//x=7200;��Ӧ1HZ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	if(pinlv<1)
		pinlv=1;
	//��ʼ��TIM3
	u16 x=(7200/pinlv);
	TIM_TimeBaseStructure.TIM_Period = 10000-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =x -1;   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

}
