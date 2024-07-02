/*
 * PortSet.cpp
 *
 *  Created on: 2020
 *      Author: Administrator
 */
#include "PortSet.h"
#include "PortSetDo.h"

u32 PortSet::m_OUT[2]= {0};
u32 PortSet::m_PLUS[16]= {0};

u16 PortSet::m_faZhuangtai[6]={0};// {0x1234,0x5678,0x34,0x56,0x78,0x12};

void PortSet::setBitOut(u16 outNo,bool b){
	u8 x1 =outNo% 100;
	if(!x1)
		return;
	u8 x2 = outNo / 100;

	if(x2>1)//最大2个站（含本站）
		return;
	x1--;
	if (b)
		PortSet::m_OUT[x2] |= (1 << x1);
	else
		PortSet::m_OUT[x2] &= (~(1 << x1));
	if(!x2)//如果是本机
		PortSetDo::outOnOff(x1,b);
}
