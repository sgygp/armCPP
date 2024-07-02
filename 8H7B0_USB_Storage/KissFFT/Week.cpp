/*
 * Week.cpp
 *
 *  Created on: 2024年4月25日
 *      Author: Administrator
 */

#include "Week.h"

int Week::getWeek(int year, int month, int day) {//输出1-7 分别代表 周一到周日
	if(month == 1 || month == 2) {
		month += 12;
		--year;
	}
	int week = -1;
	week = (day + 2 * month + 3 * (month + 1) / 5 + year + year / 4 -year / 100 +year / 400) % 7 + 1;
	return week;
	// 输出-1为错误
}
