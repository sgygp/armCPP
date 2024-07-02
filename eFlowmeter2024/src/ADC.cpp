/*
 * ADC.cpp
 *
 *  Created on: 2021��9��29��
 *      Author: Administrator
 */

#include "ADC.h"

ADC::ADC() {
	// TODO �Զ����ɵĹ��캯�����
}
ADC::~ADC() {
	// TODO �Զ����ɵ������������
}

void ADC::init(void) {
	//		RCC_APB2PeriphClockCmd(GPIO_CE_RCC, ENABLE); //ʹ��ʱ��
	GPIO_InitTypeDef GPIO_InitStructure;    //����һ���ṹ�������������ʼ��GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	//	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	//	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void ADC::ADC_DMA_InitD(void) {
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��ʱ��
	DMA_DeInit(DMA1_Channel1);    //��ͨ��һ�Ĵ�����ΪĬ��ֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) &(ADC1->DR); //�ò������Զ���DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32) &m_ad; //�ò������Զ���DMA�ڴ����ַ(ת���������ĵ�ַ)
	DMA_InitStructure.DMA_BufferSize = sizeof(m_ad)/2; //����ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ������Ҳ����m_adHe�Ĵ�С
	//DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&m_ad;//�ò������Զ���DMA�ڴ����ַ(ת���������ĵ�ַ)
	//	DMA_InitStructure.DMA_BufferSize = ADC_DATALEN * 2; //����ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ������Ҳ����m_adHe�Ĵ�С
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //�ò����涨����������Ϊ���ݴ����Ŀ�ĵػ�����Դ���˴�����Ϊ��Դ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�趨�����ַ�Ĵ����������,�˴���Ϊ���� Disable
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�����趨�ڴ��ַ�Ĵ����������,�˴���Ϊ������Enable
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //�����ڲ�ѭ��ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ��ӵ�и����ȼ� �ֱ�4���ȼ� �͡��С��ߡ��ǳ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //ʧ��DMAͨ�����ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

	DMA_ClearFlag(DMA1_FLAG_TC1); //���ͨ��1�Ĵ�����ɱ�־λ
	//	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA1_Channel1, ENABLE); //����DMAͨ��һ

}
void ADC::ADC_DMA_InitA(void) {
	ADC_InitTypeDef ADC_InitStructure;

	/*IO��ADCʹ��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ɨ��ģʽ
	//ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = TONGDAOx;
	//ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 	1,	ADC_SampleTime_239Cycles5); //ͨ��0ת��������浽m_adHe[0~10][1]
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5,		2,	ADC_SampleTime_239Cycles5); //ͨ��0ת��������浽m_adHe[0~10][1]
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6,		3,	ADC_SampleTime_239Cycles5); //ͨ��0ת��������浽m_adHe[0~10][1]
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7,		4,	ADC_SampleTime_239Cycles5); //ͨ��0ת��������浽m_adHe[0~10][1]
	//	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 	5,	ADC_SampleTime_239Cycles5); //ͨ��0ת��������浽m_adHe[0~10][1]
	//	ADC_RegularChannelConfig(ADC1, ADC_Channel_8,		6,	ADC_SampleTime_239Cycles5); //ͨ��0ת��������浽m_adHe[0~10][1]

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1))
		;
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1))
		;
	ADC_DMACmd(ADC1, ENABLE); //����ADC��DMA֧��
}
void ADC::start(void) {
	init();
	ADC_DMA_InitD();
	ADC_DMA_InitA();
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}
void ADC::run(u32 ms) {


	if (DMA_GetFlagStatus(DMA1_FLAG_TC1) != RESET) {
		DMA_ClearFlag(DMA1_FLAG_TC1); //���ͨ��1�Ĵ�����ɱ�־λ
		for(int i=0;i<TONGDAOx;i++)
		{
			for(int j=0;j<1024;j++)
				m_he[i]+=m_ad[j][i];
		}
		m_cishu++;
	}
	if (ms % 1000 == 0) {
		for(int i=0;i<TONGDAOx;i++)
		{
			m_he[i]>>=10;
			m_adZhi[i] = m_he[i] / m_cishu;
			m_he[i] = 0;
		}
		m_cishu = 0;
	}


}
