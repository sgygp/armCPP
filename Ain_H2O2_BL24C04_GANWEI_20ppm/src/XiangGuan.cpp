/*
 * XiangGuan.cpp
 *
 *  Created on: 2021年8月13日
 *      Author: Administrator
 */
#include <XiangGuan.h>
#include "diag/Trace.h"
#include "Timer.h"
#include "string.h"
//#include <stdio.h>
#include <stdlib.h>
#include "CRC16.h"

XiangGuan::XiangGuan(COM *_com) {
	// TODO 自动生成的构造函数存根
	m_com1=_com;
}

XiangGuan::~XiangGuan() {
	// TODO 自动生成的析构函数存根
}

void XiangGuan::run(void) {
	m_errJishi++;
	if (m_errJishi >= ERROR_XiangGuan) {
		m_bERROR = true;
		m_errJishi = ERROR_XiangGuan;
	} else
		m_bERROR = false;
	u32 d1 = 0;
	if (!m_com1->m_data->m_shouPos)
		return;
	d1 = (Timer::ms5_jishu + Timer::MAXSHU - m_com1->m_data->m_curShou_ms5);
	d1 = d1 % Timer::MAXSHU;
	if (d1 < COM_SHOUKONGXIAN)
		return;
	u8 shoupos = m_com1->m_data->m_shouPos;
	m_com1->m_data->m_shouPos = 0;
	///由于本协议不发送数据，所以借用发送数组作为临时使用
	memcpy(m_com1->m_data->m_FA,m_com1->m_data->m_SHOU,shoupos);
	char *p=(char *)m_com1->m_data->m_FA;
	p[shoupos]=0;
	float ff=0;
	ff=getZhi(p,"A");
	if(ff>-999)
		m_A=ff;

	ff=getZhi(p,"R");
	if(ff>-999)
		m_R=ff;

	ff=getZhi(p,"CC");
	if(ff>-999)
		m_CC=ff;

	ff=getZhi(p,"T");
	if(ff>-999)
		m_T=ff;

	m_com1->m_data->m_FA[0]=0;
	/*
	if (shoupos !=25)
		return;

	memcpy(m_buf, m_com1->m_data->m_SHOU, 25);

	u16 crc = CRC16::JisuanCRC(m_buf, shoupos-2, false);
	u16 bufCRC = m_buf[shoupos - 1];
	bufCRC <<= 8;
	bufCRC |= m_buf[shoupos - 2];
	if (crc != bufCRC)
		return;

	m_errJishi = 0; //清错误计时
	m_CC = getFloat(3);
	m_R = getFloat(7);
	m_T = getFloat(11);
	 */
}
void XiangGuan::sendRead()
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
float XiangGuan::getFloat(u8 x)
{
	float ff;
	u8 *a=(u8*)&ff;
	for(int i=0;i<4;i++)
	{
		*a=m_buf[3+x-i];
		a++;
	}
	return ff;
}

float XiangGuan::getZhi(char* p,const char *s) {
	float ff=0;
	char *pc=strstr(p,s);
	if(pc==NULL)
		return -99999;
	pc=pc+strlen(s)+1;
	int x=strcspn(pc," ");
	if(!x)
		return -99999;
	strncpy((char *)m_buf,pc,x);
	ff=atof((const char *)m_buf);
	return ff;
}
