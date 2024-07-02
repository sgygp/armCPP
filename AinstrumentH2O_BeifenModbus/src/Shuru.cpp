/*
 * Shuru.cpp
 *
 *  Created on: 2020年2月5日
 *      Author: Administrator
 */

#include "Shuru.h"
#include <math.h>
#include "Anjian.h"

Shuru::Shuru() {
	// TODO Auto-generated constructor stub
	m_data = 0; //更改的数据
	m_fanhuiHuamian = 0xff; //传过来的按钮值
	m_dot = 2;
	m_bFuShuru = false; //是否允许负输入
	m_guangbiao = 1; //光标所在位置0-5

}
void Shuru::toData() {
	int xx = 0;
	for (int i = 1; i < 6; i++) {
		if (m_shuzi[i] < 10) {
			xx += m_shuzi[i];
			xx *= 10;
		}
	}
	xx /= 10;
	if (m_shuzi[0] == 12)
		xx = -xx;
	m_data = xx;
}
void Shuru::toShuzi() {
	u8 a1, a2, a3, a4;
	u16 ww = m_data;
	if (m_data < 0) {
		m_shuzi[0] = 12;
		ww = 0 - m_data;
	} else if (m_guangbiao == 0 && m_bFuShuru)
		m_shuzi[0] = 11;
	else
		m_shuzi[0] = 0xff;
	a4 = ww % 10, ww /= 10;
	a3 = ww % 10, ww /= 10;
	a2 = ww % 10, ww /= 10;
	a1 = ww % 10;
	if (m_dot == 0) {
		m_shuzi[1] = a1;
		m_shuzi[2] = a2;
		m_shuzi[3] = a3;
		m_shuzi[4] = a4;
		m_shuzi[5] = 0xff;
	} else if (m_dot == 1) {
		m_shuzi[1] = a1;
		m_shuzi[2] = a2;
		m_shuzi[3] = a3;
		m_shuzi[4] = 10;
		m_shuzi[5] = a4;
	} else if (m_dot == 2) {
		m_shuzi[1] = a1;
		m_shuzi[2] = a2;
		m_shuzi[3] = 10;
		m_shuzi[4] = a3;
		m_shuzi[5] = a4;
	} else if (m_dot == 3) {
		m_shuzi[1] = a1;
		m_shuzi[2] = 10;
		m_shuzi[3] = a2;
		m_shuzi[4] = a3;
		m_shuzi[5] = a4;
	}

}
void Shuru::getKey(u8 key, bool &m_once) {
	switch (key) {
	case UP:
		if (m_shuzi[m_guangbiao] < 10)
			m_shuzi[m_guangbiao] = (m_shuzi[m_guangbiao] + 1) % 10;
		else if (m_shuzi[m_guangbiao] == 11)
			m_shuzi[m_guangbiao] = 12;
		else if (m_shuzi[m_guangbiao] == 12)
			m_shuzi[m_guangbiao] = 11;
		m_once = true;
		break;
	case DOWN:
		if (m_shuzi[m_guangbiao] < 10)
			m_shuzi[m_guangbiao] = (m_shuzi[m_guangbiao] + 9) % 10;
		else if (m_shuzi[m_guangbiao] == 11)
			m_shuzi[m_guangbiao] = 12;
		else if (m_shuzi[m_guangbiao] == 12)
			m_shuzi[m_guangbiao] = 11;
		m_once = true;
		break;
	case RIGHT:
		m_guangbiao = (m_guangbiao + 1) % 6;
		if (m_guangbiao == (5 - m_dot))
			m_guangbiao = (m_guangbiao + 1) % 6;
		if (m_guangbiao == 5 && m_dot == 0)
			m_guangbiao = 0;
		else if (m_guangbiao == 0 && !m_bFuShuru)
			m_guangbiao = 1;
		toShuzi();
		m_once = true;
		break;
	case LEFT:
		m_guangbiao = (m_guangbiao + 5) % 6;
		if (m_guangbiao == 0 && !m_bFuShuru)
			m_guangbiao = 5;
		if (m_guangbiao == (5 - m_dot))
			m_guangbiao = (m_guangbiao + 5) % 6;

		toShuzi();
		m_once = true;
		break;
	default:
		break;

	};
	toData();
}
