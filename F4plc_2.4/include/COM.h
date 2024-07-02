/*
 * COM.h
 *
 *  Created on: 2021年3月9日
 *      Author: Administrator
 */

#ifndef COM_H_
#define COM_H_
#include "stm32f4xx_conf.h"
#define COM_SHOUKONGXIAN 2 //接收断开时间20mS

typedef struct {
	u8 m_SHOU[256];
	u8 m_FA[256];
	u8 m_shouPos;
	short m_faPos;
	u32 m_curShou_ms10;
	u8 m_yaofaShu;
	bool m_bFasongWan;

} COMDATA;

class COM {
public:
	COM(u8 no);
	virtual ~COM();
	void init(u32 baud);
public:
	static COMDATA m_data0;
	static COMDATA m_data1;
	static COMDATA m_data2;
	u8 m_no = 0;
	COMDATA *m_data;
	void FasongX(u8 geshu);
	void FasongX(u8 *data,u8 geshu);
private:
	void init0(u32 baud);
	void init1(u32 baud);
	void init2(u32 baud);
public:
	static inline void enableTX1() {
		GPIO_SetBits(GPIOA, GPIO_Pin_11);
	}
	static inline void enableRX1() {
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	}
	static inline void enableTX2() {
		GPIO_SetBits(GPIOA, GPIO_Pin_12);
	}
	static inline void enableRX2() {
		GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	}
};

#endif /* COM_H_ */
