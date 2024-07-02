/*
 * Modbus.h
 *
 *  Created on: 2021年3月9日
 *      Author: Administrator
 */

#ifndef MODBUS_H_
#define MODBUS_H_
#include "COM.h"
#define GUZHANGCISHU 5
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
class Modbus
{
public:
	Modbus(COM *com);
public:
	static u8 m_bufDiannaoShou[20]; //存放两个串口之间的数据交换，从MODBUS时，是电脑下发或要读取LORA模块，要传给模块；主MODBUS时,是LORA模块发过来的数据，要传给电脑
	static u8 m_bufLoraShou[20]; //存放两个串口之间的数据交换，从MODBUS时，是电脑下发或要读取LORA模块，要传给模块；主MODBUS时,是LORA模块发过来的数据，要传给电脑
	COM *m_com;
	u16 m_jicunqiShou[100];
	u8 m_shebeiAddr=0;//设备地址
	u8 m_jicunqiShouLen = 0; //收到寄存器个数
	u16 m_jicunqiShouAddr = 0; //收到的寄存器首地址

	u16 m_jicunqiFa[100];
	bool m_JieshouOK = false;	//接收03指令成功
	bool m_SetOK = false;		//设置06指令成功
	bool m_05OK=false;

public:
//	void run(u32 ms,u8 addr,u32 zhouqi,u16 jicunqiStart,u8 geshu);
	bool runTerminal(u8 addr);
	void runMain();
	bool DoCommand();
	void putSetCommand(u8 addr, u16 jicunqi, u16 zhi);
	void putSetCommand05(u8 addr, u16 jicunqi, u16 zhi);
	void putReadCommand(u8 addr, u16 start, u16 geshu);
	void putReturnDataCommand();
	void putReturnSetCommand();
	void putQitaCommand(u8*buf,u8 geshu);
	int DoQitaCommand(u8*buf,u8 geshu,u16 yanshiMS);
	int readCOM();
	void CommandNet();
private:
	bool ChuliDiannaoTouchuan(u8 len);
	bool ChuliLoraTouchuan(u8 len);

	void putCommand(u8 geshu);
	u16 getWord(u8 *b);

	u8 m_bufLoraFA[20];
	u8 m_commandBuf[10][256];//单次最大发送64个寄存器数据
	u8 m_pR = 0;
	u8 m_pW = 0;
	u8 m_commandCishu = 0;
	bool m_bMainError = true;		//通讯失败
	u8 tempFA[256];
public:
	u8 *tempSHOU;
};

#endif /* MODBUS_H_ */
