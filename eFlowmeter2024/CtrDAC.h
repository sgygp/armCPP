/*
 * CtrDAC.h
 *
 *  Created on: 2021Äê10ÔÂ9ÈÕ
 *      Author: Administrator
 */
#ifndef CTRDAC_H_
#define CTRDAC_H_
#include "stm32f10x_conf.h"

class CtrDAC {
public:
	CtrDAC();
	virtual ~CtrDAC();
	void init();
	void out(float dianya);
};

#endif /* CTRDAC_H_ */
