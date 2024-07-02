/*
 * Quxian0_1.cpp
 *
 *  Created on: 2020年2月14日
 *      Author: Administrator
 */

#include <math.h>
#include "Quxian0_1.h"

void Quxian0_1::jisuanShangpian(float m_50, float zhi, float &ffH, float &ffL) {
	float ss = 0;
	float cha = 0;
	float oldcha = 0;
	float temp = 0;

	while (1) {
		ss=log10(ffH*m_50+1)/log10(ffH+1);
		cha =  ss-zhi;
		if (fabs(cha) < 0.00001)
			break;
		if (cha < 0 && oldcha > 0) {
			ffL = ffH;
			ffH = temp;
			oldcha = 0;
			jisuanShangpian(m_50, zhi, ffH, ffL);

			break;
		}
		temp = ffH;
		ffH = ffL + (ffH - ffL) / 2;
		oldcha = cha;
	}

	return;

}
void Quxian0_1::jisuanXiapian(float m_50, float zhi, float &ffH, float &ffL) {
	float ss = 0;
	float cha = 0;
	float oldcha = 0;
	float temp = 0;

	while (1) {
		ss = (pow(10, m_50 * log10(ffH + 1)) - 1) / (ffH);
		cha =  zhi-ss;
		if (fabs(cha) < 0.00001)
			break;
		if (cha < 0 && oldcha > 0) {
			ffL = ffH;
			ffH = temp;
			oldcha = 0;
			jisuanXiapian(m_50, zhi, ffH, ffL);

			break;
		}
		temp = ffH;
		ffH = ffL + (ffH - ffL) / 2;
		oldcha = cha;
	}

	return;

}
void Quxian0_1::getQuxianChangshu(float m_50, float zhi, float &quxian) {
	//(e^(x/20*ln((100/20)+1)/(100/20))-1)*20
	float m_quxian = 1000000.0;
	float ll = 0;
	if (m_50 >zhi && m_50 < 0.99) { //下偏曲线必须大于0.5
		jisuanXiapian(m_50, zhi, m_quxian, ll);
		quxian = m_quxian;
	} else if (m_50 > 0.01 && m_50 < zhi) { //上偏曲线必须小于0.5
		jisuanShangpian(m_50, zhi, m_quxian, ll);
		quxian = m_quxian;

	} else {
		quxian = 1;
	}
	//*zhi=(pow(10.0, m_50 / m_quxian * log10((100.0 / m_quxian) + 1) / (100 / m_quxian)) - 1) * m_quxian;
	return;
}

