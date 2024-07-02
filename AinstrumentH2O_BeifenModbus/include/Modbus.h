/*
 * Modbus.h
 *
 *  Created on: 2021��3��9��
 *      Author: Administrator
 */

#ifndef MODBUS_H_
#define MODBUS_H_
#include "COM.h"
#define GUZHANGCISHU 3
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
class Modbus {
public:
	Modbus(COM *com);
public:
	COM *m_com;
	u16 m_jicunqiShou[64]={0};
	u8 m_shebeiAddr=0;//�豸��ַ
	u8 m_jicunqiShouLen = 0; //�յ��Ĵ�������
	u16 m_jicunqiShouAddr = 0; //�յ��ļĴ����׵�ַ

	u16 m_jicunqiFa[64]={0};
	bool m_JieshouOK = false;	//����03ָ��ɹ�
	bool m_SetOK = false;		//����06ָ��ɹ�


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

	char m_commandBuf[10][140];//���������64���Ĵ�������
	u8 m_pR = 0;
	u8 m_pW = 0;
	u8 m_commandCishu = 0;
	bool m_bMainError = true;		//ͨѶʧ��
	u8 temp[140];//���������64���Ĵ�������
};

#endif /* MODBUS_H_ */
