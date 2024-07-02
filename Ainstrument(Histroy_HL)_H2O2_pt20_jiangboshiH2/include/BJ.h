#ifndef BJ_H_
#define BJ_H_

#include "stm32f10x.h"

// ----- LED definitions ------------------------------------------------------

// Adjust these definitions for your own board.

// Olimex STM32-H103 definitions (the GREEN led, C12, active low)
// (SEGGER J-Link device name: STM32F103RB).

// Port numbers: 0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, ...
#define RCC_BJ1            RCC_APB2Periph_GPIOC
#define PORTS_BJ1             GPIOC
#define PINS_BJ1               GPIO_Pin_6

#define RCC_BJ2            RCC_APB2Periph_GPIOC
#define PORTS_BJ2             GPIOC
#define PINS_BJ2               GPIO_Pin_7
// ----------------------------------------------------------------------------

class BJ {
public:
	BJ() = default;

	void
	init();

	inline void
	__attribute__((always_inline)) turnOn1() {
	    GPIO_SetBits(PORTS_BJ1,PINS_BJ1 );
	}

	inline void
	__attribute__((always_inline)) turnOff1() {
	    GPIO_ResetBits(PORTS_BJ1,PINS_BJ1 );
	}
	inline void
	__attribute__((always_inline)) turnOn2() {
	    GPIO_SetBits(PORTS_BJ2,PINS_BJ2 );
	}

	inline void
	__attribute__((always_inline)) turnOff2() {
	    GPIO_ResetBits(PORTS_BJ2,PINS_BJ2 );
	}
};

// ----------------------------------------------------------------------------

#endif // BJ_H_
