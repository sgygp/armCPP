/*
 * Pwm_Timer3.h
 *
 *  Created on: 2019��4��24��
 *      Author: Administrator
 */

#ifndef ADC_DMA_H_
#define ADC_DMA_H_

#include "stm32f10x_conf.h"


class AdcDMA {
public:
	static u16 m_adZhi[2];
	static u8 m_adOK;
	static constexpr u16 DATALEN=4096;//AD����Ƶ��Ϊ12M/(239.5+12.5)=47.619K
	AdcDMA();
	static volatile u16 m_ad[DATALEN][2]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ,3ͨ����ÿͨ���ɼ�10�κ���ȡƽ����
public:
	void start();
private:
	void ADC_DMA_InitD(void);
	void ADC_DMA_InitA(void);

};

#endif /* ADC_DMA_H_ */
