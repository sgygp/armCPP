/*
 * ADC.h
 *
 *  Created on: 2021��9��29��
 *      Author: Administrator
 */

#ifndef ADC_H_
#define ADC_H_
#include "stm32f10x_conf.h"
#define TONGDAOx 4

#define ADC_DATALEN 1024//72000000/8/252;//AD����Ƶ��Ϊ12M/(239.5+12.5)=35.714K ;(12:47.619)K

class ADC {
public:
	ADC();
	virtual ~ADC();

	volatile u16 m_ad[ADC_DATALEN][TONGDAOx];
	u16 m_adZhi[TONGDAOx];
	u32 m_he[TONGDAOx]={0};
	u16 m_cishu=0;
	u8 m_bOK = 0;
	void init();
	void start();
	void ADC_DMA_InitD(void);
	void ADC_DMA_InitA(void);
	void run(u32 ms);
};

#endif /* ADC_H_ */
