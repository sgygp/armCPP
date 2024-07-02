/*
 * Modbus.cpp
 *
 *  Created on: 2021��3��9��
 *      Author: Administrator
 */

#include "Modbus.h"
#include "CRC16.h"
#include "string.h"
//#include "Lora.h"
#include "delay.h"
#include "Timer.h"
u8 Modbus::m_bufDiannaoShou[20]= {0}; //�����������֮������ݽ�������MODBUSʱ���ǵ����·���Ҫ��ȡLORAģ�飬Ҫ����ģ�飻��MODBUSʱ,��LORAģ�鷢���������ݣ�Ҫ��������
u8 Modbus::m_bufLoraShou[20]= {0}; //�����������֮������ݽ�������MODBUSʱ���ǵ����·���Ҫ��ȡLORAģ�飬Ҫ����ģ�飻��MODBUSʱ,��LORAģ�鷢���������ݣ�Ҫ��������

Modbus::Modbus(COM *com)
{
	// TODO �Զ����ɵĹ��캯�����
	m_com = com;
	tempSHOU=m_com->m_data->m_SHOU;
}

//void Modbus::run(u32 ms,u8 addr,u32 zhouqi,u16 jicunqiStart,u8 geshu) { //5ms����һ��
//	if (ms % 100 == 0) //ÿ100����ִ��һ��ָ��
//		DoCommand();
//	if((ms/1000%zhouqi)==0)
//		this->putReadCommand(addr,jicunqiStart,geshu);
//
//	runMain();
//
//}
int Modbus::DoQitaCommand(u8*buf,u8 geshu,u16 yanshiMS)
{
	putQitaCommand(buf,geshu);
	DoCommand();
	int len=0;
	for(int i=0;i<yanshiMS;i++)
	{
		len=readCOM();
		if(len)
			return len;
		Delay::delay_us(1000);
	}
	return 0;
}

void Modbus::putQitaCommand(u8*buf,u8 geshu)
{
	if(geshu==0)
	{
		for(int i=0;i<100;i++)
		{
			if(!buf[i]){
				geshu=i;
				break;
			}
		}
	}
	m_commandBuf[m_pW][0] = geshu;
	memcpy(m_commandBuf[m_pW] + 1, buf, geshu);
	m_pW = (m_pW + 1) % 10;
}

void Modbus::putCommand(u8 geshu)
{
	if (m_commandCishu >= GUZHANGCISHU)
		{
			m_bMainError = true;
			m_commandCishu = 0;
		}
	m_commandBuf[m_pW][0] = geshu;
	memcpy(m_commandBuf[m_pW] + 1, tempFA, geshu);
	m_pW = (m_pW + 1) % 10;
	m_commandCishu++;
}
void Modbus::CommandNet()//ָ��ǰ��
{
	m_pR = (m_pR + 1) % 10;
}
bool Modbus::DoCommand()//�����Ƿ���05���05����Զ�ǰ��
{
	if (m_pR == m_pW)
		return false;
	u16 x = m_commandBuf[m_pR][3];
	x <<= 8;
	x += m_commandBuf[m_pR][4];
	m_jicunqiShouAddr = x;
	u8 len = m_commandBuf[m_pR][0];
	m_com->FasongX(m_commandBuf[m_pR] + 1,len);
	if (m_commandBuf[m_pR][0] == 3)
		m_JieshouOK = false; //�ȸ�λ
	else if (m_commandBuf[m_pR][0] == 6)
		m_SetOK = false; //�ȸ�λ
	else if(m_commandBuf[m_pR][0] == 5)
	{
		m_05OK=false;
		return true;
	}
	m_pR = (m_pR + 1) % 10;
	return false;

}
void Modbus::putSetCommand(u8 addr, u16 jicunqi, u16 zhi)
{
	u8 *buf = tempFA;
	buf[0] = addr;
	buf[1] = 0x06;
	buf[2] = jicunqi >> 8;
	buf[3] = jicunqi;
	buf[4] = zhi >> 8;
	buf[5] = zhi;
	CRC16::JisuanCRC(buf, 6, true);
	putCommand(8);
}
void Modbus::putSetCommand05(u8 addr, u16 jicunqi, u16 zhi)
{
	u8 *buf = tempFA;
	buf[0] = addr;
	buf[1] = 0x05;
	buf[2] = jicunqi >> 8;
	buf[3] = jicunqi;
	buf[4] = zhi >> 8;
	buf[5] = zhi;
	CRC16::JisuanCRC(buf, 6, true);
	putCommand(8);
}
void Modbus::putReadCommand(u8 addr, u16 start, u16 geshu)
{
	u8 *buf = tempFA;
	int x=0;
/*
	if(Lora::m_bDingdian&&m_NO==4)
		{
			buf[0]=0x00;
			buf[1]=addr;
			buf[2]=Lora::m_xinDao;
			x=3;
		}
*/

	buf[x+0] = addr;
	buf[x+1] = 0x03;
	buf[x+2] = start >> 8;
	buf[x+3] = start;
	buf[x+4] = geshu >> 8;
	buf[x+5] = geshu;
	CRC16::JisuanCRC(buf, x+6, true);
	putCommand(x+8);
}
u16 Modbus::getWord(u8 *b)
{
	u16 u = b[0];
	u <<= 8;
	u |= b[1];
	return u;
}
int Modbus::readCOM()
{
	u32 d1 = 0;
	COMDATA *ddata=m_com->m_data;
	if (!ddata->m_shouPos)
		return false;
	d1 = (Timer::ms2_jishu  - ddata->m_curShou_ms2);
	if (d1 < COM_SHOUKONGXIAN)
		return false;
	u8 len = ddata->m_shouPos;
	ddata->m_shouPos = 0;
	return len;
}
bool Modbus::ChuliLoraTouchuan(u8 len)
{
/*
		if(Lora::m_bSet)//��ģʽֻ��һ���������ж�ID
		{
			if(m_bufDiannaoShou[0])//���յ��ĵ������ݷ���LORAģ��
				{
					u8 len=m_bufDiannaoShou[0];
					memcpy(m_bufLoraFA,m_bufDiannaoShou+1,len);
					m_com->FasongX(m_bufLoraFA,len);
					m_bufDiannaoShou[0]=0;
					return true;
				}
			if(len)
			{
			if(len>16)//��������鳤����10
				len=16;
			m_bufLoraShou[0]=len;
			memcpy(&m_bufLoraShou[1],tempSHOU,len);
			return true;
			
			}
		}
*/


	return false;
}
void Modbus::runMain()
{

//	if(!Lora::m_bSet)
//		DoCommand();
	int len = readCOM();
/*
	if(ChuliLoraTouchuan(len))
		return;
*/
	if (len < (1 * 2 + 5)) //��������������1���Ĵ�����ֵ��7�ֽ�
		return;
	u8 *buf=tempSHOU;
	m_shebeiAddr = buf[0];
	u16 crc = CRC16::JisuanCRC(buf, len - 2, false);
	u16 bufCRC = buf[len - 1];
	bufCRC <<= 8;
	bufCRC |= buf[len - 2];
	if (crc != bufCRC)
		return;
	m_bMainError = false; //�յ�CRCͨ�������ݣ������������״̬
	if (buf[1] == 0x03)   //��ȡ
		{
			m_jicunqiShouLen = (len - 5) / 2;
			for (int i = 0; i < m_jicunqiShouLen; i++)
				{
					m_jicunqiShou[i] = getWord(buf + 3 + 2 * i);
				}
			m_JieshouOK = true;
		}
	else if (buf[1] == 0x06)   //���óɹ�
			m_SetOK = true;
	else if (buf[1] == 0x05)   //���óɹ�
		m_05OK=true;
}
void Modbus::putReturnDataCommand()
{
	//����ǰ���Ǹ��������Ѿ��ŵ�m_jicunqiFa�У�
	//�Ĵ�����ַ��m_jicunqiShouAddr,
	//�Ĵ���������m_jicunqiShouLen

	u8 *buf = tempFA;
	buf[0] = tempSHOU[0];
	buf[1] = 0x03;
	buf[2] = m_jicunqiShouLen*2;
	for (int i = 0; i < m_jicunqiShouLen; i++)
		{
			buf[3+ i * 2] = m_jicunqiFa[i] >> 8;
			buf[3 + i * 2 + 1] = m_jicunqiFa[i];
		}
	CRC16::JisuanCRC(buf, 3 + m_jicunqiShouLen * 2, true);
	putCommand(3 + m_jicunqiShouLen * 2 + 2);

}
void Modbus::putReturnSetCommand()
{
//����ǰ������Ѿ����ú� �Ĵ�����ַ��m_jicunqiShouAddr
//�Ĵ���ֵ��m_jicunqiShou[0]
//	u8 *buf = temp;
//	buf[0] = *m_addr;
//	buf[1] = 0x06;
//	buf[2] = m_jicunqiShouAddr >> 8;
//	buf[3] = m_jicunqiShouAddr;
//	buf[4] = m_jicunqiShou[0] >> 8;
//	buf[5] = m_jicunqiShou[0];
//	CRC16::JisuanCRC(buf, 6, true);
	memcpy(tempFA,tempSHOU,8);
	putCommand(8);

}
bool Modbus::ChuliDiannaoTouchuan(u8 len)
{
/*	u8 *buf=tempSHOU;


	if(Lora::m_bSet&&m_NO==3)
	{
		if(len)//��LORA����״̬��,�������Թ��������ݣ��ѵ��Է����������ݣ�����LORA���飬�Թ�����LORAģ��
		{
			if(len>16)//��������鳤����10
				len=16;
			if(len==8
			&&buf[2]==0x01
			&&buf[3]==0x22
			&&buf[4]==0x00
			)//�Ĵ�����ַ290,����LORA������ʽ
			{
				return false;
			}else{
				m_bufDiannaoShou[0]=len;
				memcpy(&m_bufDiannaoShou[1],buf,len);
				return true;
			}
		}
		if(m_bufLoraShou[0])//���յ��ĵ������ݷ���LORAģ��
		{
			u8 len=m_bufLoraShou[0];
			memcpy(m_bufLoraFA,m_bufLoraShou+1,len);
			m_com->FasongX(m_bufLoraFA,len);
			m_bufLoraShou[0]=0;
			return true;
		}

	}
*/
/*	if(!Lora::m_bSet&&m_NO==3&&len==3&&
	        ((buf[0]==0xc1&&buf[1]==0xc1&&buf[2]==0xc1)
	         ||(buf[0]==0xc3&&buf[1]==0xc3&&buf[2]==0xc3)))
	{
		Lora::m_bSet=true;
		Lora::toSet();
		return true;
	}*/
	return false;
}
bool Modbus::runTerminal(u8 addr)
{
/*
	if(!Lora::m_bSet)
*/
	DoCommand();

	u8 len = readCOM();
/*
	if(ChuliDiannaoTouchuan(len))
		return false;
*/
	if(!len)
		return false;
	u8 *buf=tempSHOU;
	
	if (len >8)
		return len;

	if (buf[0] != addr&&buf[0]!=0xff)  //��վ�·������ݶ���8�ֽ�
		{
			return false;
		}
	u16 crc = CRC16::JisuanCRC(buf, 6, false);
	u16 bufCRC = buf[7];
	bufCRC <<= 8;
	bufCRC |= buf[6];
	if (crc != bufCRC)
		return false;
	m_jicunqiShouAddr = getWord(buf + 2);
	if (buf[1] == 0x03)   //�յ���ȡָ��
		{
			m_JieshouOK = true;
			m_jicunqiShouLen = getWord(buf + 4);
		}
	else if (buf[1] == 6)
		{
			m_jicunqiShou[0] = getWord(buf + 4);
			m_SetOK = true;
		}
	return false;

}
