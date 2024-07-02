/*
 * Pwm_Timer.h
 *
 *  Created on: 2019Äê4ÔÂ24ÈÕ
 *      Author: Administrator
 */
#ifndef PWM_TIMER_H_
#define PWM_TIMER_H_

#include "stm32f10x_conf.h"

class Timer2PWM{
public:
	Timer2PWM()=default;

public:
	void init(u16 psc,u16 arr);
	void outPWM3(u16 x);
};

#endif /* PWM_TIMER_H_ */
