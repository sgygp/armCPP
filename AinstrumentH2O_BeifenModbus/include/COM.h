/*
 * COM.h
 *
 *  Created on: 2021年3月9日
 *      Author: Administrator
 */

#ifndef COM_H_
#define COM_H_
#include "stm32f10x_conf.h"
#define COM_SHOUKONGXIAN 4 //接收断开时间10mS
#define COM3_PORT 	GPIOB
#define COM3_PIN 	GPIO_Pin_1

typedef struct {
	u8 m_SHOU[256];
	u8 m_FA[256];
	u8 m_shouPos;
	short m_faPos;
	u32 m_curShou_ms5;
	u8 m_yaofaShu;
	u8 m_bFasongWan;

} COMDATA;

class COM {
public:
	COM(u8 no);
	virtual ~COM();
	void init(u32 baud);
public:
	static COMDATA m_data1;
//	static COMDATA m_data2;
//	static COMDATA m_data3;
//	static COMDATA m_data4;
//	static COMDATA m_data5;
	u8 m_NO = 0;
	COMDATA *m_data;
	void FasongX(u8 geshu);
private:
	void init1(u32 baud);
	void init2(u32 baud);
	void init3(u32 baud);
	void init4(u32 baud);
	void init5(u32 baud);
};

#endif /* COM_H_ */
