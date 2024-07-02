/*
 * Set.h
 *
 *  Created on: 2024Äê3ÔÂ12ÈÕ
 *      Author: Administrator
 */

#ifndef SET_H_
#define SET_H_
#include "stm32f10x_conf.h"

#define SETPORT GPIOB
#define SETPIN1 	GPIO_Pin_6
#define SETPIN2 	GPIO_Pin_7


class Set {
public:
	Set();
	void init();
	u8 getSet();
};

#endif /* SET_H_ */
