/*
 * BenjiTerminal.h
 *
 *  Created on: 2021
 *      Author: Administrator
 */

#ifndef BENJITERMINAL_H_
#define BENJITERMINAL_H_
#include "Modbus.h"
#include "Device.h"

class BenjiTerminal
{
public:
	BenjiTerminal(Modbus *mm, Device *dev,bool is4G_);
public:
	Modbus *m_modbus;
	void run();
	static u8 m_wushujuShijian;
	static u8 fuweihouSec;
private:
	u32 ms2=0;
	static u16 temp[1600];//16*100
	bool is4G;
	
	Device *m_device;
	u16 m_gaoji=0;
	static void Reset();
	
};

#endif /* BENJITERMINAL_H_ */
