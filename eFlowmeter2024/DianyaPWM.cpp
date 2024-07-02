/*
 * DianyaPWM.cpp
 *
 *  Created on: 2021��9��12��
 *      Author: Administrator
 */

#include <DianyaPWM.h>

DianyaPWM::DianyaPWM() {
	// TODO �Զ����ɵĹ��캯�����

}

DianyaPWM::~DianyaPWM() {
	// TODO �Զ����ɵ������������
}

void DianyaPWM::init(u16 psc, u16 arr) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	 //ʹ�ܶ�ʱ��1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��GPIO����ʱ��

	//���Ź�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //���������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	//��ʼ��TIM8
	TIM_TimeBaseStructure.TIM_Period = arr - 1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_OCStructInit(&TIM_OCInitStructure);
	//��ʼ��TIM8 Channel2��Channel3 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = arr / 2;	//����ռ�ձ�ʱ��
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);    //����Tָ���Ĳ�����ʼ������TIM8 OC2

	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM8��CCR2�ϵ�Ԥװ�ؼĴ���

	TIM_ARRPreloadConfig(TIM1, ENABLE);                        // ʹ��TIM8���ؼĴ���ARR
	TIM_Cmd(TIM1, ENABLE);                   // ʹ�ܶ�ʱ��8
	TIM_CtrlPWMOutputs(TIM1, ENABLE);        // ע�⣺ ���ö�ʱ��8��PWMģʽ�����������仰����
}
void DianyaPWM::out(u16 zhi) {
	TIM_SetCompare3(TIM1, zhi);        //*10000
}
