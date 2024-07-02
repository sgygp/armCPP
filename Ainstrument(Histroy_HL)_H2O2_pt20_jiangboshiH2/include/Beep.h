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
#define PORTS               GPIOC
#define PINS               GPIO_Pin_4
#define RCC_CLK             RCC_APB2Periph_GPIOC

class Beep {
public:
	Beep() = default;

	void
	init();

	static inline void
	__attribute__((always_inline)) turnOn() {
		GPIO_SetBits(PORTS, PINS);
	}

	static inline void
	__attribute__((always_inline)) turnOff() {
		GPIO_ResetBits(PORTS, PINS);
	}
};

// ----------------------------------------------------------------------------

#endif /* SSR_H_ */
