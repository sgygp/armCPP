/*
 * HuanjingShuju.h
 *
 *  Created on: 2021年3月14日
 *      Author: Administrator
 */
#ifndef HUANJINGSHUJU_H_
#define HUANJINGSHUJU_H_
#include "Modbus.h"

#define MA_4 753
#define MA_20 3770
#define MA_CHA (MA_20-MA_4)
#define CHONGFACISHU 15

class HuanjingShuju {
public:
	HuanjingShuju(Modbus * mm);
public :
	static u16 m_shuju[32+40];//32个实时湿度+29个实时数据+11个备用
	static u32 m_command;//高字节代表寄存器地址；第二字节代表设备地址，第三四字节代表ON/OFF
	static u32 m_command2;//高字节代表寄存器地址；第二字节代表设备地址，第三四字节代表值
	static u16 m_AD[12];
	static bool m_OK;
	Modbus *m_modbus;
	void run(u32 ms);
	void setShebeiONOFF(u8 shebeiAddr,u8 jicunqiAddr,bool b);
	void setShebeiData(u8 shebeiAddr,u8 jicunqiAddr,u16 x);
private:
	u8 m_curItem=0;
	int chongfa=0;
	
};

#endif /* HUANJINGSHUJU_H_ */
