/*
 * Pwm_Timer3.cpp
 *
 *  Created on: 2019��4��24��
 *      Author: Administrator
 *      PB0,PB1��ӦADC12_IN8,ADC12_IN9
 */
#include "Adc_Dma.h"
#include <string.h>

u16 AdcDMA::m_adZhi[2];
u8 AdcDMA::m_adOK;
constexpr u16 AdcDMA::DATALEN;
volatile u16 AdcDMA::m_ad[DATALEN][2];


AdcDMA::AdcDMA() {
	memset((void *) m_ad, 0, sizeof(m_ad));
}
void AdcDMA::ADC_DMA_InitD(void) {
	DMA_InitTypeDef DMA_InitStructure={0};
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��ʱ��
	DMA_DeInit(DMA1_Channel1);    //��ͨ��һ�Ĵ�����ΪĬ��ֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) &(ADC1->DR); //�ò������Զ���DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32) &m_ad; //�ò������Զ���DMA�ڴ����ַ(ת���������ĵ�ַ)
	DMA_InitStructure.DMA_BufferSize = DATALEN * 2; //����ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ������Ҳ����m_adZhi�Ĵ�С
	//DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&m_ad;//�ò������Զ���DMA�ڴ����ַ(ת���������ĵ�ַ)
	//  DMA_InitStructure.DMA_BufferSize = 8192;//����ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ������Ҳ����m_adZhi�Ĵ�С
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //�ò����涨����������Ϊ���ݴ����Ŀ�ĵػ�����Դ���˴�����Ϊ��Դ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�趨�����ַ�Ĵ����������,�˴���Ϊ���� Disable
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�����趨�ڴ��ַ�Ĵ����������,�˴���Ϊ������Enable
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ��ӵ�и����ȼ� �ֱ�4���ȼ� �͡��С��ߡ��ǳ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //ʹ��DMAͨ�����ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); //����DMAͨ��һ

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //NVIC_Group:NVIC���� 0~4 �ܹ�5��,���2λ��ռ
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
void AdcDMA::ADC_DMA_InitA(void) {
	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	ADC_DeInit(ADC1);
	/*3��IO�ڵ����ã�PA0��PA1��PA2��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*IO��ADCʹ��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);



	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ɨ��ģʽ
	//ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 2;
//	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5); //ͨ��һת��������浽m_adZhi[0~10][0]
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5); //ͨ����ת��������浽m_adZhi[0~10][1]
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_239Cycles5);//ͨ����ת��������浽m_adZhi[0~10][2]

	ADC_DMACmd(ADC1, ENABLE); //����ADC��DMA֧��
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1))
		;
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1))
		;

}
void AdcDMA::start(void) {
	ADC_DMA_InitD();
	ADC_DMA_InitA();
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

u8 cishu = 0;
long he1 = 0, he2 = 0;
extern "C" void DMA1_Channel1_IRQHandler(void) {

	//if (DMA_GetITStatus(DMA1_IT_TC1)) {
	DMA_ClearITPendingBit(DMA1_IT_GL1); //���ȫ���жϱ�־

//	}
	for (int i = 0; i < AdcDMA::DATALEN; i++) {
		he1 += AdcDMA::m_ad[i][0];
		he2 += AdcDMA::m_ad[i][1];
	}
	cishu++;
	if (cishu >= 4) {
		u32 uu = he1 / AdcDMA::DATALEN/4;
		 AdcDMA::m_adZhi[0] = uu;
		uu = he2 / AdcDMA::DATALEN/4;
		 AdcDMA::m_adZhi[1] = uu;

		cishu = 0;
		he1 = 0;
		he2 = 0;
		 AdcDMA::m_adOK = 1;
	}

}
