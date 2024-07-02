/*
 * H7Power.h
 *
 *  Created on: 2024Äê4ÔÂ9ÈÕ
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
