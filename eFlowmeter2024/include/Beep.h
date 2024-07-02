/*
 * Beep.h
 *
 *  Created on: 2020Äê1ÔÂ30ÈÕ
 *      Author: Administrator
 */

#ifndef Beep_
#define Beep_

#include "stm32f10x.h"

// ----------------------------------------------------------------------------
#define PORTS_BEEP               GPIOC
#define PINS_BEEP               GPIO_Pin_0

class Beep {
public:
	Beep() = default;

	void
	init();

	static inline void
	__attribute__((always_inline)) turnOn() {
//		GPIO_SetBits(PORTS_BEEP, PINS_BEEP);
	}

	static inline void
	__attribute__((always_inline)) turnOff() {
		GPIO_ResetBits(PORTS_BEEP, PINS_BEEP);
	}
};

// ----------------------------------------------------------------------------

#endif /* SSR_H_ */
