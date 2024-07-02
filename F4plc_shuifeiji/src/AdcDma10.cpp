/*
 * AdcDma10.cpp
 *
 *  Created on: 2021年3月6日
 *      Author: Administrator
 */

#include "AdcDma10.h"
#include "diag/Trace.h"

u16 AdcDma10::m_adZhi[10];
u16 AdcDma10::zhizhi[ADCDATALEN][10];
void AdcDma10::init() {
	/* 定义DMA初始化结构体 */

	DMA_InitTypeDef DMA_InitStructure;

	/* 开启DMA时钟 */

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	/* 配置DMA结构体 */

	DMA_InitStructure.DMA_Channel = DMA_Channel_0; /* ADC1 在DMA2_Stream0 的通道0上 */
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t) &ADC1->DR); /* 外设地址 */
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) zhizhi; /* 内存地址 */
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; /* 从外设到内存传输 */
	DMA_InitStructure.DMA_BufferSize = 10 * ADCDATALEN; /* 传输数据量为*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/* 外设地址不变 */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; /* 内存地址自增 */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; /* 传输单位为16位 */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; /* 传输单位为16位 */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; /* 循环传输模式 DMA_Mode_Circular*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; /* 高优先级 */
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; /* 不使用FIFO */
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; /* 不使用FIFO */
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; /* 存储器突发单次传输 */
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; /* 外设突发单次传输 */

	/* 初始化DMA2_Stream0 */
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	/* 启动DMA */
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* 定义ADC控制结构体 */

	ADC_InitTypeDef ADC_InitStructure;

	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	/* 开启ADC时钟 */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* 开启GPIOC时钟 */
	RCC_AHB1PeriphClockCmd(
	RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

	/* 初始化GPIOF7/8/9/10 */
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

	/* 初始化所有ADC结构体 */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent; /* ADC独立模式 */
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1; /* 使用DMA 模式1 这里ADC_DMAAccessMode_1用于12位精度，一个接一个获取；
	 ADC_DMAAccessMode_2用于两个adc同时工作以提高ADC转换效率，
	 ADC_DMAAccessMode_3与2大抵相同，但是精度取6位，这样速度更快。*/
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; /* 分频值为4   84/4=21M */
	ADC_CommonInitStructure.ADC_TwoSamplingDelay =
	ADC_TwoSamplingDelay_20Cycles; /* 转换速度为20个周期 */
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* 初始化所有ADC1结构体 */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; /* 12位精度 */
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; /* 指定的通道扫描使能 */
//	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; /* 启用连续扫描 */
//	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; /* 不使用外部促发 */
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; /* 外部触发源 定时器1的PWM输出通道1 */
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; /* 启用连续扫描 */
	ADC_InitStructure.ADC_ExternalTrigConvEdge =
	ADC_ExternalTrigConvEdge_Rising; //上升沿触发
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; //定时器事件2触发ADC
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; /* 转换数据右对齐 */
	ADC_InitStructure.ADC_NbrOfConversion = 10; /* 转换的通道数 */
	/* 初始化ADC1 */
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

	/* 使能ADC1 DMA的请求 */

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	//设置DMA中断
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE); //传输完成中断

	NVIC_InitTypeDef NVIC_InitStructure;
	//DMA中断设置
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;  //DMA2_Stream0中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化NVIC寄存器

//	ADC_SoftwareStartConv(ADC1);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//失能定时器
	TIM_Cmd(TIM2, DISABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); //初始化定时器

	//定时器设置,T=(84 * 625) / 84M = 625us = 20ms/32,50Hz信号,每周期采样32个点,采样频率1600
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; //时钟频率1M
	TIM_TimeBaseStructure.TIM_Period =2000-1; //500次，转换一轮（10个通道）算一次。
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分频因子
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //初始化定时器2

	//使能定时器中断
	TIM_ARRPreloadConfig(TIM2, ENABLE); //允许TIM2定时重载
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update); //选择TIM2的UPDATA事件更新为触发源
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //配置TIM2中断类型

	//使能TIM2
	TIM_Cmd(TIM2, ENABLE);

//	//定时器中断设置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //定时器TIM2中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        //子优先级1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化NVIC寄存器
//
//}
//extern "C" void TIM2_IRQHandler(void) {//2000时是2ms一次中断
//
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update))  //判断发生update事件中断
//			{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除update事件中断标志
//	}


}

extern "C" void DMA2_Stream0_IRQHandler(void) {
	u32 uu[10];
	DMA_ClearITPendingBit( DMA2_Stream0, DMA_IT_TCIF0);	//清除发送完成标志
	for (int i = 0; i < 10; i++) {
		uu[i] = 0;
		for (int j = 0; j < ADCDATALEN; j++) {
			uu[i] += AdcDma10::zhizhi[j][i];
		}
		AdcDma10::m_adZhi[i] = uu[i] / ADCDATALEN;
	}
//	trace_puts("---");
}
//定时器启动ADC，注意不能连续转换。DMA搬运

