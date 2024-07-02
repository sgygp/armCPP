/*
 * ADC.cpp
 *
 *  Created on: 2021年9月29日
 *      Author: Administrator
 */

#include "ADC.h"

ADC::ADC() {
	// TODO 自动生成的构造函数存根
}
ADC::~ADC() {
	// TODO 自动生成的析构函数存根
}

void ADC::init(void) {
	//		RCC_APB2PeriphClockCmd(GPIO_CE_RCC, ENABLE); //使能时钟
	GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO
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
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能时钟
	DMA_DeInit(DMA1_Channel1);    //将通道一寄存器设为默认值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) &(ADC1->DR); //该参数用以定义DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32) &m_ad; //该参数用以定义DMA内存基地址(转换结果保存的地址)
	DMA_InitStructure.DMA_BufferSize = sizeof(m_ad)/2; //定义指定DMA通道的DMA缓存的大小,单位为数据单位。这里也就是m_adHe的大小
	//DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&m_ad;//该参数用以定义DMA内存基地址(转换结果保存的地址)
	//	DMA_InitStructure.DMA_BufferSize = ADC_DATALEN * 2; //定义指定DMA通道的DMA缓存的大小,单位为数据单位。这里也就是m_adHe的大小
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //该参数规定了外设是作为数据传输的目的地还是来源，此处是作为来源
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //设定外设地址寄存器递增与否,此处设为不变 Disable
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //用来设定内存地址寄存器递增与否,此处设为递增，Enable
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //工作在不循环模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道拥有高优先级 分别4个等级 低、中、高、非常高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //失能DMA通道的内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //根据DMA_InitStruct中指定的参数初始化DMA的通道

	DMA_ClearFlag(DMA1_FLAG_TC1); //清除通道1的传输完成标志位
	//	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA1_Channel1, ENABLE); //启动DMA通道一

}
void ADC::ADC_DMA_InitA(void) {
	ADC_InitTypeDef ADC_InitStructure;

	/*IO和ADC使能时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //扫描模式
	//ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = TONGDAOx;
	//ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 	1,	ADC_SampleTime_239Cycles5); //通道0转换结果保存到m_adHe[0~10][1]
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5,		2,	ADC_SampleTime_239Cycles5); //通道0转换结果保存到m_adHe[0~10][1]
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6,		3,	ADC_SampleTime_239Cycles5); //通道0转换结果保存到m_adHe[0~10][1]
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7,		4,	ADC_SampleTime_239Cycles5); //通道0转换结果保存到m_adHe[0~10][1]
	//	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 	5,	ADC_SampleTime_239Cycles5); //通道0转换结果保存到m_adHe[0~10][1]
	//	ADC_RegularChannelConfig(ADC1, ADC_Channel_8,		6,	ADC_SampleTime_239Cycles5); //通道0转换结果保存到m_adHe[0~10][1]

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1))
		;
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1))
		;
	ADC_DMACmd(ADC1, ENABLE); //开启ADC的DMA支持
}
void ADC::start(void) {
	init();
	ADC_DMA_InitD();
	ADC_DMA_InitA();
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}
void ADC::run(u32 ms) {


	if (DMA_GetFlagStatus(DMA1_FLAG_TC1) != RESET) {
		DMA_ClearFlag(DMA1_FLAG_TC1); //清除通道1的传输完成标志位
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
