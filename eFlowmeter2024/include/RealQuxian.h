/*
 * RealQuxian.h
 *
 *  Created on: 2021年2月14日
 *      Author: Administrator
 */

#ifndef REALQUXIAN_H_
#define REALQUXIAN_H_

#include "stm32f10x.h"
#include "string.h"
#include "math.h"

class RealQuxian {
public:
	static constexpr u8 DATALEN = 128;
	float m_data[DATALEN]; //22..5秒存储一次 160*22.5=3600s
	u8 m_pData = 0;
	RealQuxian() {
		memset(m_data, 0, DATALEN * 4);

//		for (int i = 0; i < DATALEN; i++) {
//			m_data[i] = sin(i * 3.1416 * 2.0 / DATALEN) * 10.0 + 10;
//		}

	}
	void setData(float x);
};

#endif /* REALQUXIAN_H_ */
