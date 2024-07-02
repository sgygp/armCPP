/*
 * Adc32.h
 *
 *  Created on: 2023��9��25��
 *      Author: Administrator
 */

#ifndef ADC32_H_
#define ADC32_H_
#include "air32f10x_conf.h"
#define AD_LEN 128


class Adc32 {
public:
	Adc32();
	void init(int xHz,u8 iWhat);
public:
	static u16 data_table[AD_LEN];
	static bool bOK;
};

#endif /* ADC32_H_ */
