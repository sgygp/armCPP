/*
 * AT24CL04.h
 *
 *  Created on: 2020��1��7��
 *      Author: Administrator
 */
#ifndef AIR720_H_
#define AIR720_H_
#include "stm32f4xx_conf.h"

class Air720 //ռ��32���ֽ�
{
public:
    static u16 XINHAO;
    static long JINGDU;  
    static long WEIDU;
    static uint64_t IMEI; //
	static u8 m_iccid[20];
 
	static bool m_OK;//��ȡ����ʱ������ź��������Ƿ����
	static bool m_bShuju;//30������������ͨѶ
	
	static void getIMEI(u8 *buf);
	static void getICCID(u8 *buf);
	static void getLoction(u8 *buf);
	static bool isBaohanString(u8 *buf,const char *cc);
	static void setTime(u8 *buf);
};

#endif /* AT24CL04_H_ */
