/*
 * HuanjingShuju.h
 *
 *  Created on: 2021年3月14日
 *      Author: Administrator
 */

#ifndef HUANJINGSHUJU_H_
#define HUANJINGSHUJU_H_
#include "Modbus.h"

class HuanjingShuju {
public:
	HuanjingShuju(Modbus * mm);
public :
	static u16 m_data1_4[4][10];
	static u16 m_data5[20];
	static u8 m_Error[5];	//主MODBUS模式时，最多5个设备，各路大于等于GUZHANGCISHU次后认为通讯故障
							//各设备含义 =0无故障，=GUZHANGCISHU通讯故障，=-1(0xff)不采样

	static u8 m_OK[5];//数据是否准备好
	Modbus *m_modbus;
	void run(u32 ms);
private:
	u8 m_curItem=0;
};

#endif /* HUANJINGSHUJU_H_ */
