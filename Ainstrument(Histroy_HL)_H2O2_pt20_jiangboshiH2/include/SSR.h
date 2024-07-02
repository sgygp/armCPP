/*
 * SSR.h
 *
 *  Created on: 2020Äê1ÔÂ30ÈÕ
 *      Author: Administrator
 */

#ifndef SSR_H_
#define SSR_H_

#include "stm32f10x.h"
#define SSRPORT GPIOB
#define SSRPIN GPIO_Pin_7

class SSR_gutaijidianqi {
public:
	SSR_gutaijidianqi() = default;

	void
	init();

	inline void
	__attribute__((always_inline)) turnOff() {
		GPIO_ResetBits(SSRPORT,SSRPIN);
	}

	inline void
	__attribute__((always_inline)) turnOn() {
		GPIO_SetBits(SSRPORT,SSRPIN);
	}
};

// ----------------------------------------------------------------------------

#endif /* SSR_H_ */
