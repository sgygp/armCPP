/*
 * OutFreq.cpp
 *
 *  Created on: 2024��3��21��
 *      Author: Administrator
 */


//////////////////////////////////////////////////////////////////////////////////////////////
//���������Ƶ������ķ�ΧΪ100-5000Hz
//ʹ�ùܽ�PA2, TIM2-CH3

#include "OutFreq.h"

OutFreq::OutFreq() {
	// TODO �Զ����ɵĹ��캯�����

}

void OutFreq::initMaichong() {//��ʼΪ10��֮һ��0.1Hz
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);	 //ʹ�ܶ�ʱ��2ʱ��

	//���Ź�������
	GPIO_InitStructure.GPIO_Pin = OUT_PIN; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(OUT_PORT, &GPIO_InitStructure);
	GPIO_SetBits(OUT_PORT,OUT_PIN);

}

void OutFreq::initPinlv() {//��ʼΪ10��֮һ��0.1Hz

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	 //ʹ�ܶ�ʱ��2ʱ��

	//���Ź�������
	GPIO_InitStructure.GPIO_Pin = OUT_PIN; //TIM2_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  //���������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(OUT_PORT, &GPIO_InitStructure);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	//��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = 7200*5-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 10000*2-1;   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_OCStructInit(&TIM_OCInitStructure);
	//��ʼ��TIM2 Channel3 PWMģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Prescaler/2;	//����ռ�ձ�ʱ��
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);    //����Tָ���Ĳ�����ʼ������TIM2 OC3

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
}

void OutFreq::out(float pinlv) {
	if(pinlv<0.1)
		pinlv=0.1;
	int Prescaler=72000000/pinlv/65536;
	Prescaler++;
	int arr=72000000/(Prescaler*pinlv);
	TIM_PrescalerConfig(TIM2,Prescaler,TIM_PSCReloadMode_Immediate);
	TIM_SetAutoreload(TIM2,arr);
	TIM_SetCompare3(TIM2,arr/2);
}

void OutFreq::out(bool bDidianping) {
	if(bDidianping)
		GPIO_SetBits(OUT_PORT,OUT_PIN);
	else
		GPIO_ResetBits(OUT_PORT,OUT_PIN);
}
