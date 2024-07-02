/*
 * AdcDma10.h
 *
 *  Created on: 2021Äê3ÔÂ6ÈÕ
 *      Author: Administrator
 */

#ifndef ADCDMA10_H_
#define ADCDMA10_H_
#include "stm32f4xx_conf.h"
#define ADCDATALEN 500
//21M/500=42K
class AdcDma10 {

public:
 static	u16 m_adZhi[10];
static	u16 zhizhi[ADCDATALEN][10];
	void init();
};

#endif /* ADCDMA10_H_ */
