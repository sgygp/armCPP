/*
 * DACF4.cpp
 *
 *  Created on: 2021Äê3ÔÂ7ÈÕ
 *      Author: Administrator
 */

#include "DACF4.h"

void DAC_F4::init() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	DAC_InitTypeDef DAC_InitStructure;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_Cmd(DAC_Channel_2, ENABLE);
	DAC_SetChannel1Data(DAC_Align_12b_R, 0x0000);
	DAC_SetChannel2Data(DAC_Align_12b_R, 0x0000);
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
	DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);

}
void DAC_F4::out(u8 no, u16 zhi) {

	if (no == 0) {
		DAC_SetChannel1Data(DAC_Align_12b_R, zhi);
		DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
	} else {

		DAC_SetChannel2Data(DAC_Align_12b_R, zhi);
		DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
	}

}

