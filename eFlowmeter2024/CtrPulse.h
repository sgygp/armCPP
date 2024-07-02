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

#define  SELECT_PORT		GPIOB
#define  SELECT_L			GPIO_Pin_11
#define  SELECT_H			GPIO_Pin_10

class CtrPulse {
public:
	CtrPulse();
	virtual ~CtrPulse();
	void init(u16 psc,u16 arr);
	static u32 shangciTime;
	static u32 benciTime;
	static u32 jiangeTimeH;
	static u32 jiangeTimeL;
	static int m_jishi;
	float m_pinglv=0;
	void run();
	static  void on0(){
		GPIO_ResetBits(SELECT_PORT,SELECT_L);
		GPIO_ResetBits(SELECT_PORT,SELECT_H);
	};
	static  void on1(){
		GPIO_SetBits(SELECT_PORT,SELECT_L);
		GPIO_ResetBits(SELECT_PORT,SELECT_H);
	};
	static  void off(){
		GPIO_SetBits(SELECT_PORT,SELECT_H);
		GPIO_SetBits(SELECT_PORT,SELECT_L);
	};
};

#endif /* CTRPULSE_H_ */
