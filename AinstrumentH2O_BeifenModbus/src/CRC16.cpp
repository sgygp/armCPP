/*
 * CRC16.cpp
 *
 *  Created on: 2020Äê9ÔÂ28ÈÕ
 *      Author: Administrator
 */
#include "CRC16.h"
unsigned short CRC16::m_crc;
void CRC16::_crc(unsigned char  x)//
{
	unsigned char  i;
	CRC16::m_crc^=x;
	for(i=0;i<8;i++)
	{
		if(CRC16::m_crc&0x01)
		{
			CRC16::m_crc>>=1;
			CRC16::m_crc^=0xa001;
		}else
			CRC16::m_crc>>=1;

	}
}
unsigned short CRC16::JisuanCRC(unsigned char  *buf,unsigned char  geshu,bool write)
{
	unsigned char  i;
	CRC16::m_crc=0xffff;
	for(i=0;i<geshu;i++)
	{
		_crc(buf[i]);
	}
	if(write)
	{
		buf[i]=CRC16::m_crc;
		buf[i+1]=CRC16::m_crc>>8;
	}

	return CRC16::m_crc;
}



