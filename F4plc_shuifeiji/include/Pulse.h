#ifndef CPP_PULSE_H
#define CPP_PULSE_H
#include "stm32f4xx_conf.h"

static const GPIO_TypeDef * GPIOXX[14]={
		GPIOA,
		GPIOA,
		GPIOB,
		GPIOA,
		GPIOB,
		GPIOB,
		GPIOB,
		GPIOB,
		GPIOE,
		GPIOE,
		GPIOE,
		GPIOE,
		GPIOE,
		GPIOE
};

static const u16 PINXX[]={
		GPIO_Pin_0,
		GPIO_Pin_1,
		GPIO_Pin_12,
		GPIO_Pin_15,
		GPIO_Pin_3,
		GPIO_Pin_13,
		GPIO_Pin_11,
		GPIO_Pin_10,
		GPIO_Pin_15,
		GPIO_Pin_14,
		GPIO_Pin_13,
		GPIO_Pin_12,
		GPIO_Pin_11,
		GPIO_Pin_10
};



class Pulse {
public:
	void init();
	void run(u32 ms5);
	u8 oldState[14]={0};
	u16 m_pulse[14]={0};
};

#endif
