/*
 * Adc32.cpp
 *
 *  Created on: 2023年9月25日
 *      Author: Administrator
 */

//#include "diag/Trace.h"
#include "Adc32.h"
u16 Adc32::data_table[AD_LEN]={0};
bool Adc32::bOK=false;
Adc32::Adc32() {
	// TODO 自动生成的构造函数存根

}

void Adc32::init(int xHz,bool isB_wendu) {

	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE); //使能ADC1,GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置GPIO口速度为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	  //模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  //初始化GPIOA3


	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 		//时钟使能

	//定时器TIM2初始化
	TIM_TimeBaseStructure.TIM_Prescaler =SystemCoreClock/36000000-1; 			//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_Period = (36000000/xHz)-1; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);			//根据指定的参数初始化TIMx的时间基数单位.


	/////////////当外部触发信号被选为ADC规则或注入转换时，只有它的上升沿可以启动转换。
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = (TIM_TimeBaseStructure.TIM_Period+1)/2;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;		//输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM2, & TIM_OCInitStructure);		//初始化外设TIM2_CH2

	TIM_Cmd(TIM2, ENABLE); 			//使能TIMx





	RCC_ADCCLKConfig(RCC_PCLK2_Div16); // ADC最高时钟为PCLK2的8分频 216/16=13.5M
	ADC_DeInit(ADC1);				  //复位ADC1

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					// ADC工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2; //转换由软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;				//需要转换的通道个数
	ADC_Init(ADC1, &ADC_InitStructure);									//初始化ADC1

	if(isB_wendu)
	{
		ADC_RegularChannelConfig(ADC1, ADC_Channel_16,  1, ADC_SampleTime_239Cycles5); //配置温度传感器通道
		ADC_TempSensorVrefintCmd(ENABLE);		//使能温度传感器和VREFINT通道
	}
	else
		ADC_RegularChannelConfig(ADC1, ADC_Channel_4,  1, ADC_SampleTime_239Cycles5); //配置ADC1的通道


	ADC_Cmd(ADC1, ENABLE); //使能ADC1
	ADC_ResetCalibration(ADC1); //复位校准寄存器
	while (ADC_GetResetCalibrationStatus(ADC1))
		;						//等待复位校准寄存器完成
	ADC_StartCalibration(ADC1); //开始校准
	while (ADC_GetCalibrationStatus(ADC1))
		; //等待校准完成

	ADC_SoftwareStartConvCmd(ADC1,ENABLE);


	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);						//使能DMA1时钟
	DMA_DeInit(DMA1_Channel1);												//复位DMA1通道1
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;			// DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Adc32::data_table;			// DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						// DMA方向：外设到内存
	DMA_InitStructure.DMA_BufferSize = (sizeof(Adc32::data_table))/2;					// DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//内存数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//工作在循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;						// DMA通道x的优先级为高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							// 非内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);							//根据DMA_InitStruct中指定的参数初始化DMA的通道x的寄存器
	DMA_Cmd(DMA1_Channel1, ENABLE);											//使能DMA1通道1
	ADC_DMACmd(ADC1, ENABLE);												//使能ADC1的DMA支持

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;  // DMA1通道1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //子优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	DMA_ClearITPendingBit(DMA1_IT_TC1); //清除DMA1通道1传输结束标志
	DMA_ClearFlag(DMA1_FLAG_TC1);		//清除DMA1通道1传输完成标志
	DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);		  //使能DMA1通道1中断

}

extern "C" void DMA1_Channel1_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC1) != RESET) //检查DMA1通道1传输结束标志
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1); //清除DMA1通道1传输结束标志
		DMA_ClearFlag(DMA1_FLAG_TC1);		//清除DMA1通道1传输完成标志
		Adc32::bOK=true;

		//		PRINTF_LOG("Code Value = %d , 电压值 = %2.4f\n", DAM_ADC_Value[0],
		//				   (float)VREF * DAM_ADC_Value[0] / 4095 / 1000); //输出ADC值
	}
}

