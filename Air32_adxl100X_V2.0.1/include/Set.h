/*
 * Set.h
 *
 *  Created on: 2021Äê9ÔÂ1ÈÕ
 *      Author: Administrator
 */

#ifndef SET_H_
#define SET_H_
#include "air32f10x_conf.h"

#define PORTS_SET             GPIOB
#define PINS_SET               GPIO_Pin_11

class Set {
public:
	Set();
	virtual ~Set();
	void init();
	void setOn();
};

#endif /* SET_H_ */
