/*
 * HistoryData.cpp
 *
 *  Created on: 2020年1月31日
 *      Author: Administrator
 */

#include "HistoryData.h"
#include <string.h>
#include <math.h>



HistoryData::HistoryData(Ainstrument *ainstr) {
	// TODO Auto-generated constructor stub
	float beilv = pow(10, ainstr->getCanshuZhi(2));
	float ll = ainstr->getCanshuZhi(12) * beilv;
	float kuadu = ainstr->getCanshuZhi(13) * beilv-ll;
	memset(m_data, 0, DATALEN*2);
	for (int i = 0; i < DATALEN; i++)//48*24
	{
		float f=i;
		f=ll+kuadu/2+kuadu*sin(f/480*3.14159265*2)/2;
		m_data[i] = f;
		//m_data[i] = 90+sin(f/480*6.28)*90;
	}
	m_pW = 0;
	m_pR = 0;
	m_pingmuShijian = 0x0001; //满屏1H；2-满屏8小时；4-满屏24小时
	m_xiangqianYe = 0;
	m_bAutoShuxin=true;

}
HistoryData::HistoryData() {
	memset(m_data, 0, DATALEN*2);
	m_pW = 0;
	m_pR = 0;
	m_pingmuShijian = 0x0001; //满屏1H；2-满屏8小时；4-满屏24小时
	m_xiangqianYe = 0;
	m_bAutoShuxin=true;

}

HistoryData::~HistoryData() {
	// TODO Auto-generated destructor stub
}
void HistoryData::WriteData(u16 xx) {
	// TODO Auto-generated destructor stub
	m_data[m_pW] = xx;
	m_pW = (m_pW + 1) % DATALEN;
}
u16 HistoryData::getPingmuShijian() {
	return m_pingmuShijian;
}
void HistoryData::setPingmuShijian(u16 xx) {
	m_pingmuShijian = xx;
}

