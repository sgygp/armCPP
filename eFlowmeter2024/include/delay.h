#include "stm32f10x_conf.h"
#ifndef Delay_
#define Delay_

class Delay {
public:
	Delay()=default;
	static void delay_us(u16 us);
	static void delay_ms(u16 ms);
	static void delay2_ms(u16 ms);
};
#endif /* SSR_H_ */
