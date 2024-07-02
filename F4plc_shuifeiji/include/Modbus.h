/*
 * Modbus.h
 *
 *  Created on: 2021��3��9��
 *      Author: Administrator
 */

#ifndef MODBUS_H_
#define MODBUS_H_
#include "COM.h"
#define GUZHANGCISHU 5
/*
 ��վʹ��˵����
	 1	m_modbusMain.putReadCommand(1, 20, 10); //д��ַ��1���豸���ӼĴ���0��ʼ��������ȡ10���Ĵ���ֵ
		 ���ɹ���m_modbusMain.m_JieshouOK=1;
		 �����ֱ����� ��
		 u16 m_jicunqiShou[64];
		u8 m_shebeiAddr=0;//�豸��ַ
		 u8 m_jicunqiShouLen=0;//�յ��Ĵ�������
		 u16 m_jicunqiShouAddr=0;//�յ��ļĴ����׵�ַ

	 2	m_modbusMain.putSetCommand(1, 25, 48); //д��ַ��1���豸��д�Ĵ�����ַ25Ϊ����ֵ48
		 д�ɹ���m_modbusMain.m_SetOK=1;

 ��վʹ��˵����
	 1�յ���ȡָ���m_modbusTerminal.m_JieshouOK=1;
	 	 �������������ִ���ϴ����ݣ���ַ�Ǳ�����ַ m_addr

		u8 m_jicunqiShouLen = 0; //�յ��Ĵ�������
		u16 m_jicunqiShouAddr = 0; //�յ��ļĴ����׵�ַ

	2 �յ�����ָ���m_modbusTerminal.m_SetOK=1;
	 	 �������������ִ���������ݣ���ַ�Ǳ�����ַ m_addr
	 	 m_jicunqiShou[0]//�Ĵ���ֵ
		u16 m_jicunqiShouAddr = 0; //�յ��ļĴ�����ַ

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
��վ��վÿ5msִ�� run(ms);
 */
class Modbus
{
public:
	Modbus(COM *com);
public:
	static u8 m_bufDiannaoShou[20]; //�����������֮������ݽ�������MODBUSʱ���ǵ����·���Ҫ��ȡLORAģ�飬Ҫ����ģ�飻��MODBUSʱ,��LORAģ�鷢���������ݣ�Ҫ��������
	static u8 m_bufLoraShou[20]; //�����������֮������ݽ�������MODBUSʱ���ǵ����·���Ҫ��ȡLORAģ�飬Ҫ����ģ�飻��MODBUSʱ,��LORAģ�鷢���������ݣ�Ҫ��������
	COM *m_com;
	u16 m_jicunqiShou[100];
	u8 m_shebeiAddr=0;//�豸��ַ
	u8 m_jicunqiShouLen = 0; //�յ��Ĵ�������
	u16 m_jicunqiShouAddr = 0; //�յ��ļĴ����׵�ַ

	u16 m_jicunqiFa[100];
	bool m_JieshouOK = false;	//����03ָ��ɹ�
	bool m_SetOK = false;		//����06ָ��ɹ�
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
	u8 m_commandBuf[10][256];//���������64���Ĵ�������
	u8 m_pR = 0;
	u8 m_pW = 0;
	u8 m_commandCishu = 0;
	bool m_bMainError = true;		//ͨѶʧ��
	u8 tempFA[256];
public:
	u8 *tempSHOU;
};

#endif /* MODBUS_H_ */
