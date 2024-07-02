/*
 * HuanjingShuju.cpp
 *
 *  Created on: 2021��3��14��
 *      Author: Administrator
 */

#include "HuanjingShuju.h"
#include "Cunchu.h"
#include "string.h"
u16 HuanjingShuju::m_shuju[32+40]={0};//32��ʵʱʪ��+29��ʵʱ����+11������
u32 HuanjingShuju::m_command=0;//���ֽڴ����豸��ַ���ڶ��ֽڴ���Ĵ�����ַ���������ֽڴ���ON/OFF
u32 HuanjingShuju::m_command2=0;//���ֽڴ����豸��ַ���ڶ��ֽڴ���Ĵ�����ַ���������ֽڴ���ֵ
u16 HuanjingShuju::m_AD[12]={0};
//��ַ1 �ʱã�2 ˮ�ã�3 LORA���أ�4 ������
bool HuanjingShuju::m_OK=true;
HuanjingShuju::HuanjingShuju(Modbus *mm)
{
	// TODO �Զ����ɵĹ��캯�����
	m_modbus = mm;
}
void HuanjingShuju::run(u32 ms)  //2��������һ��
{
	ms*=2;
	if(ms%100==0)//ÿ0.1��
	{
		if(chongfa)
		{
			m_modbus->DoCommand();//�ط���ʱ����05ָ����ǰ��
			chongfa--;
			if(!m_modbus)//�ط��˶������û�յ����ݣ������ǰ����
			{
				HuanjingShuju::m_OK=false;
				m_modbus->CommandNet();//����ǰ��
			}
		}else	if(m_modbus->DoCommand())//�ж��Ƿ�05����
			chongfa=CHONGFACISHU;

	}

	if(m_command!=0)
	{
		u32 xx=HuanjingShuju::m_command;
		setShebeiONOFF(xx>>24,(xx>>16)&0xff,xx&0xffff);
		HuanjingShuju::m_command=0;
	}
	if(m_command2!=0)
	{
		u32 xx=HuanjingShuju::m_command2;
		setShebeiData(xx>>24,(xx>>16)&0xff,xx&0xffff);
		HuanjingShuju::m_command2=0;
		chongfa=CHONGFACISHU;
	}
	m_modbus->runMain();
	
	if(ms%400==0)
	{
		
		switch(m_curItem)
		{
			case 0:
				m_modbus->putReadCommand(1,101,1);//���ʱñ�Ƶ��ѹ���趨
				break;
			case 1:
				m_modbus->putReadCommand(2,101,1);//����ˮ�ñ�Ƶ��ѹ���趨
				break;
			case 2:
				m_modbus->putReadCommand(3,40,32);//��LORAʪ��ֵ
				break;
			case 3:
				m_modbus->putReadCommand(1,200,2);//���ʱñ�Ƶ��ʵ��ѹ����ʵ��Ƶ��
				break;
			case 4:
				m_modbus->putReadCommand(2,200,2);//����ˮ�ñ�Ƶ��ʵ��ѹ����ʵ��Ƶ��
				break;
			case 5:
				m_modbus->putReadCommand(4,0,10);//�����������˲ʱ�������ۼ�����
				break;
		}
		
		m_curItem=(m_curItem+1)%6;

	}
	if (m_modbus->m_05OK)
	{
		m_modbus->CommandNet();//�յ�05����ָ�����ǰ��
		chongfa=0;
	}
	if (m_modbus->m_JieshouOK) {
		m_modbus->m_JieshouOK = false; //���긴λ
		switch(m_modbus->m_shebeiAddr)
		{
			case 1://�ʱ�
				if(m_modbus->m_jicunqiShouLen==2)	
				{
					HuanjingShuju::m_shuju[32+163-140]=m_modbus->m_jicunqiShou[0];
					HuanjingShuju::m_shuju[32+165-140]=m_modbus->m_jicunqiShou[1];
				
				}else if(m_modbus->m_jicunqiShouLen==1)	
				{
					Cunchu::m_sysCanshu.m_feibengYaliSet=m_modbus->m_jicunqiShou[0];
				}
				break;
			case 2://ˮ��
				if(m_modbus->m_jicunqiShouLen==2)	
				{
					HuanjingShuju::m_shuju[32+162-140]=m_modbus->m_jicunqiShou[0];
					HuanjingShuju::m_shuju[32+164-140]=m_modbus->m_jicunqiShou[1];
				
				}else if(m_modbus->m_jicunqiShouLen==1)	
				{
					Cunchu::m_sysCanshu.m_shuibengYaliSet=m_modbus->m_jicunqiShou[0];
				}
				break;
			case 3://LORA����
				if(m_modbus->m_jicunqiShouLen==32)
					memcpy(HuanjingShuju::m_shuju,m_modbus->m_jicunqiShou,64);
				break;
			case 4://���������
				if(m_modbus->m_jicunqiShouLen==10)	
				{
					HuanjingShuju::m_shuju[32+160-140]=m_modbus->m_jicunqiShou[0];//˲ʱ����
					HuanjingShuju::m_shuju[32+167-140]=m_modbus->m_jicunqiShou[7];//�ۼ�����
					HuanjingShuju::m_shuju[32+168-140]=m_modbus->m_jicunqiShou[6];
				
				}
				break;
			default:
				return;
		}
	
	}
}
void HuanjingShuju::setShebeiONOFF(u8 shebeiAddr,u8 jicunqiAddr,bool b)
{
	u16 x=0;
	if(b)
		x=0xff00;
	m_modbus->putSetCommand05(shebeiAddr,jicunqiAddr,x);
	HuanjingShuju::m_OK=true;

}
void HuanjingShuju::setShebeiData(u8 shebeiAddr,u8 jicunqiAddr,u16 x)
{
	m_modbus->putSetCommand(shebeiAddr,jicunqiAddr,x);
}
