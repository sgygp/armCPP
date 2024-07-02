/*
 * RealQuxian.cpp
 *
 *  Created on: 2021Äê2ÔÂ14ÈÕ
 *      Author: Administrator
 */

#include "RealQuxian.h"
void RealQuxian::setData(float x) {
	m_data[m_pData] = x;
	m_pData = (m_pData + 1) % DATALEN;
}

