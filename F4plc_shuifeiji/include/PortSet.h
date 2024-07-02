/*
 * PortSet.h
 *
 *  Created on: 2020年10月13日AntData_PortIn
 *      Author: Administrator
 */

#ifndef _PORTSET_H_
#define _PORTSET_H_
#include "stm32f4xx_conf.h"
#define KEYSLEN 20
#define INSIZE 14
#define OUTSIZE 12

class PortSet {
public:
	PortSet()=default;
	static u32 m_OUT[2];//最大1个从站，0本站
	static u32 m_PLUS[16];
	static u16 m_faZhuangtai[6];//0/1：32个罐区阀；2：进水；3：搅拌；4：出肥；5:（0清水泵；1肥泵；2电动阀）
	
public:
	static void setBitOut(u16 outNo,bool b);
};

#endif /* ANTDATA_PORTSET_H_ */
