/*
 * TongxunChuli.h
 *
 *  Created on: 2023Äê5ÔÂ30ÈÕ
 *      Author: Administrator
 */

#ifndef TONGXUNCHULI_H_
#define TONGXUNCHULI_H_
#include "BlinkLed.h"
#include "COM.h"
#include "W25X16.h"

class TongxunChuli {
public:
	TongxunChuli();
	COM *m_com;
	W25X16 *m_25;
	void init(COM *com,W25X16 *w25);
	bool run(float batt);
	u8 readCOM();

};

#endif /* TONGXUNCHULI_H_ */
