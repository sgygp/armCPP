/*
 * AdcDma10.cpp
 *
 *  Created on: 2021��3��6��
 *      Author: Administrator
 */

#include "AdcDma10.h"
#include "diag/Trace.h"

u16 AdcDma10::m_adZhi[10];
u16 AdcDma10::zhizhi[ADCDATALEN][10];
void AdcDma10::init() {
	/* ����DMA��ʼ���ṹ�� */

	DMA_InitTypeDef DMA_InitStructure;

	/* ����DMAʱ�� */

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	/* ����DMA�ṹ�� */

	DMA_InitStructure.DMA_Channel = DMA_Channel_0; /* ADC1 ��DMA2_Stream0 ��ͨ��0�� */
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t) &ADC1->DR); /* �����ַ */
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) zhizhi; /* �ڴ��ַ */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; /* �����赽�ڴ洫�� */
	DMA_InitStructure.DMA_BufferSize = 10 * ADCDATALEN; /* ����������Ϊ*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* �����ַ���� */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; /* �ڴ��ַ���� */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; /* ���䵥λΪ16λ */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; /* ���䵥λΪ16λ */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; /* ѭ������ģʽ DMA_Mode_Circular*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; /* �����ȼ� */
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; /* ��ʹ��FIFO */
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; /* ��ʹ��FIFO */
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; /* �洢��ͻ�����δ��� */
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; /* ����ͻ�����δ��� */

	/* ��ʼ��DMA2_Stream0 */
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	/* ����DMA */
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* ����ADC���ƽṹ�� */

	ADC_InitTypeDef ADC_InitStructure;

	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	/* ����ADCʱ�� */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* ����GPIOCʱ�� */
	RCC_AHB1PeriphClockCmd(
	RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

	/* ��ʼ��GPIOF7/8/9/10 */
	GPIO_InitTypeDef GPIO_InitStructure;

	// Configure pin in output push/pull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
			| GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ��ʼ������ADC�ṹ�� */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; /* ADC����ģʽ */
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1; /* ʹ��DMA ģʽ1 ����ADC_DMAAccessMode_1����12λ���ȣ�һ����һ����ȡ��
	 ADC_DMAAccessMode_2��������adcͬʱ���������ADCת��Ч�ʣ�
	 ADC_DMAAccessMode_3��2�����ͬ�����Ǿ���ȡ6λ�������ٶȸ��졣*/
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; /* ��ƵֵΪ4   84/4=21M */
	ADC_CommonInitStructure.ADC_TwoSamplingDelay =
	ADC_TwoSamplingDelay_20Cycles; /* ת���ٶ�Ϊ20������ */
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ��ʼ������ADC1�ṹ�� */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; /* 12λ���� */
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; /* ָ����ͨ��ɨ��ʹ�� */
//	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; /* ��������ɨ�� */
//	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; /* ��ʹ���ⲿ�ٷ� */
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; /* �ⲿ����Դ ��ʱ��1��PWM���ͨ��1 */
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; /* ��������ɨ�� */
	ADC_InitStructure.ADC_ExternalTrigConvEdge =
	ADC_ExternalTrigConvEdge_Rising; //�����ش���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; //��ʱ���¼�2����ADC
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; /* ת�������Ҷ��� */
	ADC_InitStructure.ADC_NbrOfConversion = 10; /* ת����ͨ���� */
	/* ��ʼ��ADC1 */
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 5, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 6, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 7, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 8, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 9, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 10,
	ADC_SampleTime_480Cycles);

	/* ʹ��ADC1 DMA������ */

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	//����DMA�ж�
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE); //��������ж�

	NVIC_InitTypeDef NVIC_InitStructure;
	//DMA�ж�����
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;  //DMA2_Stream0�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);    //����ָ���Ĳ�����ʼ��NVIC�Ĵ���

//	ADC_SoftwareStartConv(ADC1);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//ʧ�ܶ�ʱ��
	TIM_Cmd(TIM2, DISABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); //��ʼ����ʱ��

	//��ʱ������,T=(84 * 625) / 84M = 625us = 20ms/32,50Hz�ź�,ÿ���ڲ���32����,����Ƶ��1600
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; //ʱ��Ƶ��1M
	TIM_TimeBaseStructure.TIM_Period =2000-1; //500�Σ�ת��һ�֣�10��ͨ������һ�Ρ�
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷ�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //��ʼ����ʱ��2

	//ʹ�ܶ�ʱ���ж�
	TIM_ARRPreloadConfig(TIM2, ENABLE); //����TIM2��ʱ����
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); //ѡ��TIM2��UPDATA�¼�����Ϊ����Դ
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //����TIM2�ж�����

	//ʹ��TIM2
	TIM_Cmd(TIM2, ENABLE);

//	//��ʱ���ж�����
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //��ʱ��TIM2�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //�����ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);    //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
//
//}
//extern "C" void TIM2_IRQHandler(void) {//2000ʱ��2msһ���ж�
//
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update))  //�жϷ���update�¼��ж�
//			{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //���update�¼��жϱ�־
//	}


}

extern "C" void DMA2_Stream0_IRQHandler(void) {
	u32 uu[10];
	DMA_ClearITPendingBit( DMA2_Stream0, DMA_IT_TCIF0);	//���������ɱ�־
	for (int i = 0; i < 10; i++) {
		uu[i] = 0;
		for (int j = 0; j < ADCDATALEN; j++) {
			uu[i] += AdcDma10::zhizhi[j][i];
		}
		AdcDma10::m_adZhi[i] = uu[i] / ADCDATALEN;
	}
//	trace_puts("---");
}
//��ʱ������ADC��ע�ⲻ������ת����DMA����

