/*
 * COM.h
 *
 *  Created on: 2021年3月9日
 *      Author: Administrator
 */

#ifndef COM_H_
#define COM_H_
#include "stm32f10x_conf.h"
#define COM_SHOUKONGXIAN 4 //接收断开时间20mS
#define MAX_LENGTH 256//6

typedef struct {
	u8 m_SHOU[256];
	u8 m_FA[256];
	u8 m_shouPos;
	short m_faPos;
	u32 m_curShou_ms5;
	u8 m_yaofaShu;
	bool m_bFasongzhong;

} COMDATA;

class COM{
public:
	COM();
	virtual ~COM();
	void init();
public:
	static COMDATA m_data[1];
	void FasongX(u8 no,u8 geshu);
private:
	void init1(u32 baud);
	void init2(u32 baud);
	void init3(u32 baud);
	void init4(u32 baud);
	void init5(u32 baud);
	void init6(u32 baud);
};

#endif /* COM_H_ */
