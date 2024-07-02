/*
 * DianyaPWM.h
 *
 *  Created on: 2021Äê9ÔÂ12ÈÕ
 *      Author: Administrator
 */

#ifndef DianyaPWM_H_
#define DianyaPWM_H_
#include "stm32f10x_conf.h"

class DianyaPWM {
public:
	DianyaPWM();
	virtual ~DianyaPWM();
	static void init(u16 psc,u16 arr);
	static void out(u16 zhi);
};

#endif /* LIGHT_H_ */
