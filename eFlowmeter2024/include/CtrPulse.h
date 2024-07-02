/*
 * CtrPulse.h
 *
 *  Created on: 2021Äê10ÔÂ8ÈÕ
 *      Author: Administrator
 */

#ifndef CTRPULSE_H_
#define CTRPULSE_H_
#include "stm32f10x_conf.h"
#define CTRLOAD 64000

#define  SELECT_PORT		GPIOA
#define  SELECT_L			GPIO_Pin_0
#define  SELECT_H			GPIO_Pin_1

class CtrPulse {
public:
	static u8 m_fangxiang;
	static u16 m_arr;
	static void init(u16 psc, u16 arr);
	static void on0();
	static void on1();
	static void off() ;
};

#endif /* CTRPULSE_H_ */
