/*
 * Modbus.h
 *
 *  Created on: 2021年3月9日
 *      Author: Administrator
 */

#ifndef MODBUS_H_
#define MODBUS_H_
#include "COM.h"
#define GUZHANGCISHU 3
/*
 主站使用说明：
	 1	m_modbusMain.putReadCommand(1, 20, 10); //写地址是1的设备，从寄存器0开始，连续读取10个寄存器值
		 读成功后，m_modbusMain.m_JieshouOK=1;
		 参数分别存放在 ：
		 u16 m_jicunqiShou[64];
		u8 m_shebeiAddr=0;//设备地址
		 u8 m_jicunqiShouLen=0;//收到寄存器个数
		 u16 m_jicunqiShouAddr=0;//收到的寄存器首地址

	 2	m_modbusMain.putSetCommand(1, 25, 48); //写地址是1的设备，写寄存器地址25为数据值48
		 写成功后，m_modbusMain.m_SetOK=1;

 从站使用说明：
	 1收到读取指令后，m_modbusTerminal.m_JieshouOK=1;
	 	 根据下面参数，执行上传数据，地址是本机地址 m_addr

		u8 m_jicunqiShouLen = 0; //收到寄存器个数
		u16 m_jicunqiShouAddr = 0; //收到的寄存器首地址

	2 收到设置指令后，m_modbusTerminal.m_SetOK=1;
	 	 根据下面参数，执行设置数据，地址是本机地址 m_addr
	 	 m_jicunqiShou[0]//寄存器值
		u16 m_jicunqiShouAddr = 0; //收到的寄存器地址

	if(m_modbusTerminal.m_JieshouOK)
	{
		m_modbusTerminal.m_JieshouOK=false;
		m_modbusTerminal.m_jicunqiFa[0]=1;
		m_modbusTerminal.m_jicunqiFa[1]=2;
		m_modbusTerminal.m_jicunqiFa[2]=3;
		m_modbusTerminal.putReturnDataCommand();
	}
	if(m_modbusTerminal.m_SetOK)
	{
		m_modbusTerminal.m_SetOK=false;
		m_modbusTerminal.putReturnSetCommand();
	}
主站从站每5ms执行 run(ms);

 */
class Modbus {
public:
	Modbus(COM *com);
public:
	COM *m_com;
	u16 m_jicunqiShou[64]={0};
	u8 m_shebeiAddr=0;//设备地址
	u8 m_jicunqiShouLen = 0; //收到寄存器个数
	u16 m_jicunqiShouAddr = 0; //收到的寄存器首地址

	u16 m_jicunqiFa[64]={0};
	bool m_JieshouOK = false;	//接收03指令成功
	bool m_SetOK = false;		//设置06指令成功


public:
	bool run(u32 ms,u8 addr,u32 zhouqi,u16 jicunqiStart,u8 geshu);
	void runTerminal(u8 addr);
	bool runMain();
	void DoCommand();
	void putSetCommand03(u8 addr, u16 jicunqi, u16 zhi);
	void putSetCommand06(u8 addr, u16 jicunqi,u16* buff,u16 jicunqishu);
	void putReadCommand(u8 addr, u16 start, u16 geshu);
	void putReturnDataCommand();
	void putReturnSetCommand();
private:

	void putCommand(u8 geshu);
	u8 readCOM();
	u16 getWord(u8 *b);

	char m_commandBuf[10][140];//单次最大发送64个寄存器数据
	u8 m_pR = 0;
	u8 m_pW = 0;
	u8 m_commandCishu = 0;
	bool m_bMainError = true;		//通讯失败
	u8 temp[140];//单次最大发送64个寄存器数据
};

#endif /* MODBUS_H_ */
