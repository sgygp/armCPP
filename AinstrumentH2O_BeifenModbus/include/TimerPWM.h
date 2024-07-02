/*
 * Pwm_Timer.h
 *
 *  Created on: 2019��4��24��
 *      Author: Administrator
 */
#ifndef PWM_TIMER_H_
#define PWM_TIMER_H_

#include "stm32f10x_conf.h"

class TimerPWM{
public:
	TimerPWM()=default;

public:
	void init(u16 psc, u16 arr);
	void outPWM(u8 no,u16 x);
};

#endif /* PWM_TIMER_H_ */
