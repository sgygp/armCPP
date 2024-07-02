/*
 * CalendarTimer.h
 *
 *  Created on: 2021Äê3ÔÂ3ÈÕ
 *      Author: Administrator
 */

#ifndef CALENDARTIMER_H_
#define CALENDARTIMER_H_

#include "stm32f4xx_conf.h"

class CalendarTimer {
public:
	static RTC_TimeTypeDef RTC_TimeStructure;
	static RTC_DateTypeDef RTC_DataStructure;
public:
	static void init();
	static void GetCalendarTime();
	static void SetCalendarTime(u8 year,u8 mon,u8 day,u8 hour,u8 min,u8 sec);
	static void SetCalendarTime();
};

#endif /* CALENDARTIMER_H_ */
