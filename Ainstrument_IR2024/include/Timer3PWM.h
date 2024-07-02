/*
 * Pwm_Timer.h
 *
 *  Created on: 2019Äê4ÔÂ24ÈÕ
 *      Author: Administrator
 */
#ifndef PWM_TIMER3_H_
#define PWM_TIMER3_H_

#include "stm32f10x_conf.h"

class Timer3PWM{
public:
	Timer3PWM()=default;

public:
	void init(float pinlv);
	void setPinlv(float  pinlv);
};

#endif /* PWM_TIMER_H_ */
