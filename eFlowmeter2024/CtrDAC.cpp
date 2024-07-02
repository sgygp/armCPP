/*
 * CtrDAC.cpp
 *
 *  Created on: 2021年10月9日
 *      Author: Administrator
 */

#include "CtrDAC.h"

CtrDAC::CtrDAC() {
	// TODO 自动生成的构造函数存根

}

CtrDAC::~CtrDAC() {
	// TODO 自动生成的析构函数存根
}

void CtrDAC::init() {
/*
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4); //PA.4 输出高


	DAC_InitTypeDef DAC_InitType;
	DAC_InitType.DAC_Trigger = DAC_Trigger_None; //不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None; //不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; //屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Enable; //DAC1输出缓存关闭 BOFF1=1
	DAC_Init(DAC_Channel_1, &DAC_InitType);  //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1}
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
*/
}
void CtrDAC::out(float dianya) {
/*	float temp = dianya;
	if (temp > 2.5)
		temp = 2.5;
	else if (temp < 0)
		temp = 0;
	temp = temp * 4095 / 2.5;
	DAC_SetChannel1Data(DAC_Align_12b_R, temp);  //12位右对齐数据格式设置DAC值
	*/
}
