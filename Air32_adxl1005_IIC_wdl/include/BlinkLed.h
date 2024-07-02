#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "air32f10x_conf.h"
#define PORTS_LED             GPIOB
#define PINS_LEDR               GPIO_Pin_9
#define PINS_LEDG               GPIO_Pin_8
// ----------------------------------------------------------------------------

class BlinkLed {
public:
	BlinkLed() = default;


	static void init();

	static inline void turnR_On() {
		GPIO_ResetBits(PORTS_LED, PINS_LEDR);
		GPIO_SetBits(PORTS_LED, PINS_LEDG);
		isON1=true;
		isON3=true;
	}

	static inline void turnR_Off() {
		GPIO_SetBits(PORTS_LED, PINS_LEDR);
		GPIO_SetBits(PORTS_LED, PINS_LEDG);
		isON1=false;
	}
	static inline void turnR_() {
		if(isON1)
			turnR_Off();
		else
			turnR_On();
	}


	static inline void turnG_On() {
		GPIO_SetBits(PORTS_LED, PINS_LEDR);
		GPIO_ResetBits(PORTS_LED, PINS_LEDG);
		isON2=true;
		isON3=false;
	}

	static inline void turnG_Off() {
		GPIO_SetBits(PORTS_LED, PINS_LEDG);
		GPIO_SetBits(PORTS_LED, PINS_LEDR);
		isON2=false;
	}
	static inline void turnG_() {
		if(isON2)
			turnG_Off();
		else
			turnG_On();
	}
	static inline void turnALL_() {
		if(isON3)
			turnG_On();
		else
			turnR_On();
	}


private:
	static bool isON1;
	static bool isON2;
	static bool isON3;
};

// ----------------------------------------------------------------------------

#endif // BLINKLED_H_
