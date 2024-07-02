#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "stm32f10x.h"

// ----- LED definitions ------------------------------------------------------

// Adjust these definitions for your own board.

// Olimex STM32-H103 definitions (the GREEN led, C12, active low)
// (SEGGER J-Link device name: STM32F103RB).

// Port numbers: 0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, ...
#define RCC_LED            RCC_APB2Periph_GPIOB
#define PORTS_LED             GPIOB
#define PINS_LED               GPIO_Pin_0
// ----------------------------------------------------------------------------

class BlinkLed {
public:
	BlinkLed() = default;

	void
	init();

	inline void
	__attribute__((always_inline)) turnOn() {
	    GPIO_ResetBits(PORTS_LED,PINS_LED );
	}

	inline void
	__attribute__((always_inline)) turnOff() {
	    GPIO_SetBits(PORTS_LED,PINS_LED );
	}
};

// ----------------------------------------------------------------------------

#endif // BLINKLED_H_
