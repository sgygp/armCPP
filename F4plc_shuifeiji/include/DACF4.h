/*
 * DACF4.h
 *
 *  Created on: 2021Äê3ÔÂ7ÈÕ
 *      Author: Administrator
 */

#ifndef DACF4_H_
#define DACF4_H_
#include "stm32f4xx_conf.h"
class DAC_F4 {
public:
	void init();
	void out(u8 no,u16 zhi);
};

#endif /* DACF4_H_ */
