/*
 * CtrDAC.cpp
 *
 *  Created on: 2021��10��9��
 *      Author: Administrator
 */

#include "CtrDAC.h"

CtrDAC::CtrDAC() {
	// TODO �Զ����ɵĹ��캯�����

}

CtrDAC::~CtrDAC() {
	// TODO �Զ����ɵ������������
}

void CtrDAC::init() {
/*
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4); //PA.4 �����


	DAC_InitTypeDef DAC_InitType;
	DAC_InitType.DAC_Trigger = DAC_Trigger_None; //��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None; //��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; //���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Enable; //DAC1�������ر� BOFF1=1
	DAC_Init(DAC_Channel_1, &DAC_InitType);  //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DAC1}
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
*/
}
void CtrDAC::out(float dianya) {
/*	float temp = dianya;
	if (temp > 2.5)
		temp = 2.5;
	else if (temp < 0)
		temp = 0;
	temp = temp * 4095 / 2.5;
	DAC_SetChannel1Data(DAC_Align_12b_R, temp);  //12λ�Ҷ������ݸ�ʽ����DACֵ
	*/
}
