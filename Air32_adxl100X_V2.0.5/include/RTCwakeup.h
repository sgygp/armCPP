/*
 * RTCwakeup.h
 *
 *  Created on: 2023Äê9ÔÂ8ÈÕ
 *      Author: Administrator
 */

#ifndef RTCWAKEUP_H_
#define RTCWAKEUP_H_
#include "air32f10x_conf.h"

class RTC_wakeup {
public:
	RTC_wakeup();
	void setWakeup();
	void setOFF();
};

#endif /* RTCWAKEUP_H_ */
