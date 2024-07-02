/*
 * Th.cpp
 *
 *  Created on: 2021年8月13日
 *      Author: Administrator
 */
#include <Th.h>
#include "diag/Trace.h"
#include "Timer.h"
#include "string.h"
#include "CRC16.h"

Th::Th(Modbus *m_mod) {
	// TODO 自动生成的构造函数存根
	m_modbus=m_mod;
}

Th::~Th() {
	// TODO 自动生成的析构函数存根
}

void Th::run(u32 ms) {
	m_errJishi++;
	if (m_errJishi >= ERROR_Th) {
		m_bERROR = true;
		m_errJishi = ERROR_Th;
	} else
		m_bERROR = false;
	if(!m_modbus->run(ms,0x80,1000,0x0819,11))
		return;
	m_errJishi = 0; //清错误计时
//	u16* buf=(u16*)m_modbus->m_jicunqiShou;
	if(m_modbus->m_jicunqiShouLen==11)
	{
		memcpy(m_data,m_modbus->m_jicunqiShou,22);
	}
}
/*void Th::sendRead()
{
	u8 *buf=m_com1->m_data->m_FA;
	buf[0]=0x01;
	buf[1]=0x04;
	buf[2]=0x50;
	buf[3]=0x01;
	buf[4]=0x00;
	buf[5]=0x0A;
	buf[6]=0x30;
	buf[7]=0xCD;
	m_com1->FasongX(8);

}
float Th::getFloat(u8 *m_buf,u8 x)
{
	float ff;
	u8 *a=(u8*)&ff;
	a[0]=m_buf[x+1];
	a[1]=m_buf[x+0];
	a[2]=m_buf[x+3];
	a[3]=m_buf[x+2];
	return ff;
}
*/
