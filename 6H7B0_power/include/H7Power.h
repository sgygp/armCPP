/*
 * H7Power.h
 *
 *  Created on: 2024��4��9��
 *      Author: Administrator
 */

#ifndef H7POWER_H_
#define H7POWER_H_
#include "main.h"

class H7Power {
public:
	static RTC_HandleTypeDef hrtc;
	void init(int sleepSec);
	u32 getWakeUpTimer();
};

#endif /* H7POWER_H_ */
