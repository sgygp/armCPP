/*
 * PortSet.h
 *
 *  Created on: 2020��10��13��AntData_PortIn
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
	static u32 m_OUT[2];//���1����վ��0��վ
	static u32 m_PLUS[16];
	static u16 m_faZhuangtai[6];//0/1��32����������2����ˮ��3�����裻4�����ʣ�5:��0��ˮ�ã�1�ʱã�2�綯����
	
public:
	static void setBitOut(u16 outNo,bool b);
};

#endif /* ANTDATA_PORTSET_H_ */
