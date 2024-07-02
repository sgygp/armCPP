/*
 * Adc32.cpp
 *
 *  Created on: 2023��9��25��
 *      Author: Administrator
 */

//#include "diag/Trace.h"
#include "Adc32.h"
u16 Adc32::data_table[AD_LEN]={0};
bool Adc32::bOK=false;
Adc32::Adc32() {
	// TODO �Զ����ɵĹ��캯�����

}

void Adc32::init(int xHz,bool isB_wendu) {

	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1,GPIOAʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����GPIO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	  //ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  //��ʼ��GPIOA3


	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 		//ʱ��ʹ��

	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Prescaler =SystemCoreClock/36000000-1; 			//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_Period = (36000000/xHz)-1; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);			//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ.


	/////////////���ⲿ�����źű�ѡΪADC�����ע��ת��ʱ��ֻ�����������ؿ�������ת����
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = (TIM_TimeBaseStructure.TIM_Period+1)/2;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		//�������:TIM����Ƚϼ��Ե�
	TIM_OC2Init(TIM2, & TIM_OCInitStructure);		//��ʼ������TIM2_CH2

	TIM_Cmd(TIM2, ENABLE); 			//ʹ��TIMx





	RCC_ADCCLKConfig(RCC_PCLK2_Div16); // ADC���ʱ��ΪPCLK2��8��Ƶ 216/16=13.5M
	ADC_DeInit(ADC1);				  //��λADC1

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					// ADC�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//��ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2; //ת�����������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//�Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;				//��Ҫת����ͨ������
	ADC_Init(ADC1, &ADC_InitStructure);									//��ʼ��ADC1

	if(isB_wendu)
	{
		ADC_RegularChannelConfig(ADC1, ADC_Channel_16,  1, ADC_SampleTime_239Cycles5); //�����¶ȴ�����ͨ��
		ADC_TempSensorVrefintCmd(ENABLE);		//ʹ���¶ȴ�������VREFINTͨ��
	}
	else
		ADC_RegularChannelConfig(ADC1, ADC_Channel_4,  1, ADC_SampleTime_239Cycles5); //����ADC1��ͨ��


	ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1
	ADC_ResetCalibration(ADC1); //��λУ׼�Ĵ���
	while (ADC_GetResetCalibrationStatus(ADC1))
		;						//�ȴ���λУ׼�Ĵ������
	ADC_StartCalibration(ADC1); //��ʼУ׼
	while (ADC_GetCalibrationStatus(ADC1))
		; //�ȴ�У׼���

	ADC_SoftwareStartConvCmd(ADC1,ENABLE);


	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);						//ʹ��DMA1ʱ��
	DMA_DeInit(DMA1_Channel1);												//��λDMA1ͨ��1
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;			// DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Adc32::data_table;			// DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						// DMA�������赽�ڴ�
	DMA_InitStructure.DMA_BufferSize = (sizeof(Adc32::data_table))/2;					// DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�������ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//�ڴ����ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;						// DMAͨ��x�����ȼ�Ϊ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							// ���ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);							//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��x�ļĴ���
	DMA_Cmd(DMA1_Channel1, ENABLE);											//ʹ��DMA1ͨ��1
	ADC_DMACmd(ADC1, ENABLE);												//ʹ��ADC1��DMA֧��

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;  // DMA1ͨ��1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	DMA_ClearITPendingBit(DMA1_IT_TC1); //���DMA1ͨ��1���������־
	DMA_ClearFlag(DMA1_FLAG_TC1);		//���DMA1ͨ��1������ɱ�־
	DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);		  //ʹ��DMA1ͨ��1�ж�

}

extern "C" void DMA1_Channel1_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC1) != RESET) //���DMA1ͨ��1���������־
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1); //���DMA1ͨ��1���������־
		DMA_ClearFlag(DMA1_FLAG_TC1);		//���DMA1ͨ��1������ɱ�־
		Adc32::bOK=true;

		//		PRINTF_LOG("Code Value = %d , ��ѹֵ = %2.4f\n", DAM_ADC_Value[0],
		//				   (float)VREF * DAM_ADC_Value[0] / 4095 / 1000); //���ADCֵ
	}
}

