/*
 * WatchDog.h
 *
 *  Created on: 2020Äê2ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_
#include <stm32f4xx_wwdg.h>

class WatchDog {
public:
	void init(u8 prv,u16 load);
	void run();
};

#endif /* WATCHDOG_H_ */
