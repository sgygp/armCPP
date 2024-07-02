/*
 * Pwm_Timer3.h
 *
 *  Created on: 2019年4月24日
 *      Author: Administrator
 */

#ifndef ADC_DMA_H_
#define ADC_DMA_H_

#include "stm32f10x_conf.h"


class AdcDMA {
public:
	static u16 m_adZhi[2];
	static u8 m_adOK;
	static constexpr u16 DATALEN=4096;//AD采样频率为12M/(239.5+12.5)=47.619K
	AdcDMA();
	static volatile u16 m_ad[DATALEN][2]; //用来存放ADC转换结果，也是DMA的目标地址,3通道，每通道采集10次后面取平均数
public:
	void start();
private:
	void ADC_DMA_InitD(void);
	void ADC_DMA_InitA(void);

};

#endif /* ADC_DMA_H_ */
