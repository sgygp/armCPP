/*
 * RealQuxian.cpp
 *
 *  Created on: 2021��2��14��
 *      Author: Administrator
 */

#include "RealQuxian.h"
void RealQuxian::setData(float x) {
	m_data[m_pData] = x;
	m_pData = (m_pData + 1) % DATALEN;
}

