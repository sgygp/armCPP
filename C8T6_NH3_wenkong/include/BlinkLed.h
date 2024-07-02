#ifndef BLINKLED_H_
#define BLINKLED_H_

#include "stm32f10x_conf.h"
#define PORTS_LED             GPIOB
#define PINS_LEDR               GPIO_Pin_9
#define PINS_LEDG               GPIO_Pin_8
// ----------------------------------------------------------------------------

class BlinkLed {
public:
	BlinkLed() = default;


	 void init();
	 void run(u32 ms,float piancha);

	 inline void turnR_On() {
		GPIO_ResetBits(PORTS_LED, PINS_LEDR);
		GPIO_SetBits(PORTS_LED, PINS_LEDG);
		isON1=true;
		isON3=true;
	}

	 inline void turnR_Off() {
		GPIO_SetBits(PORTS_LED, PINS_LEDR);
		GPIO_SetBits(PORTS_LED, PINS_LEDG);
		isON1=false;
	}
	 inline void turnR_() {
		if(isON1)
			turnR_Off();
		else
			turnR_On();
	}


	 inline void turnG_On() {
		GPIO_SetBits(PORTS_LED, PINS_LEDR);
		GPIO_ResetBits(PORTS_LED, PINS_LEDG);
		isON2=true;
		isON3=false;
	}

	 inline void turnG_Off() {
		GPIO_SetBits(PORTS_LED, PINS_LEDG);
		GPIO_SetBits(PORTS_LED, PINS_LEDR);
		isON2=false;
	}
	 inline void turnG_() {
		if(isON2)
			turnG_Off();
		else
			turnG_On();
	}
	 inline void turnALL_() {
		if(isON3)
			turnG_On();
		else
			turnR_On();
	}


private:
	 bool isON1=false;
	 bool isON2=false;
	 bool isON3=false;
};

// ----------------------------------------------------------------------------

#endif // BLINKLED_H_
