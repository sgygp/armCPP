/*
 * OutFreq.h
 *
 *  Created on: 2024��3��21��
 *      Author: Administrator
 */

#ifndef OUTFREQ_H_
#define OUTFREQ_H_
#include "stm32f10x_conf.h"
//#define  OUT_PORT	GPIOA
//#define  OUT_PIN		GPIO_Pin_2
#define  OUT_PORT		GPIOA
#define  OUT_PIN		GPIO_Pin_8

class OutFreq {
public:
	OutFreq();
	static void initPinlv();
	static void initMaichong();
	void out(float pinlv);//Ƶ�����
	void out(bool bDidianping);//�������
};


#endif /* OUTFREQ_H_ */
