/*
 * CtrDAC.h
 *
 *  Created on: 2021��10��9��
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
