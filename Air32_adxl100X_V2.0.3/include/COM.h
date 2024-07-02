/*
 * COM.h
 *
 *  Created on: 2021年3月9日
 *      Author: Administrator
 */

#ifndef COM_H_
#define COM_H_
#include "air32f10x_conf.h"
#define COM_SHOUKONGXIAN 4 //接收断开时间10mS

typedef struct {
	u8 m_SHOU[32];
	u8 m_FA[800];
	u8 m_shouPos;
	u8 m_bFasongWan;
	short m_faPos;
	u16 m_yaofaShu;
	u32 m_curShou_ms5;


} COMDATA;

class COM {
public:
	COM(u8 no);
	virtual ~COM();
	void init(u32 baud);
public:
	//static COMDATA m_data1;
	static COMDATA m_data2;
	//static COMDATA m_data3;
	u8 m_NO = 0;
	COMDATA *m_data;
	void FasongX(u16 geshu);
	private:
	void init1(u32 baud);
	void init2(u32 baud);
	void init3(u32 baud);
};

#endif /* COM_H_ */
