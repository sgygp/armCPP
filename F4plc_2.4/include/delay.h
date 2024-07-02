#ifndef CPP_DELAY_H
#define CPP_DELAY_H
#include "stm32f4xx_conf.h"


class Delay {
public:
	static void delay_us(u16 us);
	static void delay_ms(u16 ms);
};

#endif

