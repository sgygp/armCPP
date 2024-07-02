/*
 * Pwm_Timer3.cpp
 *
 *  Created on: 2019年4月24日
 *      Author: Administrator
 *      PB0,PB1对应ADC12_IN8,ADC12_IN9
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
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能时钟
	DMA_DeInit(DMA1_Channel1);    //将通道一寄存器设为默认值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) &(ADC1->DR); //该参数用以定义DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32) &m_ad; //该参数用以定义DMA内存基地址(转换结果保存的地址)
	DMA_InitStructure.DMA_BufferSize = DATALEN * 2; //定义指定DMA通道的DMA缓存的大小,单位为数据单位。这里也就是m_adZhi的大小
	//DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&m_ad;//该参数用以定义DMA内存基地址(转换结果保存的地址)
	//  DMA_InitStructure.DMA_BufferSize = 8192;//定义指定DMA通道的DMA缓存的大小,单位为数据单位。这里也就是m_adZhi的大小
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //该参数规定了外设是作为数据传输的目的地还是来源，此处是作为来源
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //设定外设地址寄存器递增与否,此处设为不变 Disable
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //用来设定内存地址寄存器递增与否,此处设为递增，Enable
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道拥有高优先级 分别4个等级 低、中、高、非常高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //使能DMA通道的内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //根据DMA_InitStruct中指定的参数初始化DMA的通道

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); //启动DMA通道一

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //NVIC_Group:NVIC分组 0~4 总共5组,最高2位抢占
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
	/*3个IO口的配置（PA0、PA1、PA2）*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*IO和ADC使能时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);



	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //扫描模式
	//ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 2;
//	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5); //通道一转换结果保存到m_adZhi[0~10][0]
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5); //通道二转换结果保存到m_adZhi[0~10][1]
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_239Cycles5);//通道三转换结果保存到m_adZhi[0~10][2]

	ADC_DMACmd(ADC1, ENABLE); //开启ADC的DMA支持
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
	DMA_ClearITPendingBit(DMA1_IT_GL1); //清除全部中断标志

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
