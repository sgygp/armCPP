/*
 * HuanjingShuju.cpp
 *
 *  Created on: 2021年3月14日
 *      Author: Administrator
 */

#include "HuanjingShuju.h"
#include "Cunchu.h"
#include "string.h"
u16 HuanjingShuju::m_shuju[32+40]={0};//32个实时湿度+29个实时数据+11个备用
u32 HuanjingShuju::m_command=0;//高字节代表设备地址；第二字节代表寄存器地址，第三四字节代表ON/OFF
u32 HuanjingShuju::m_command2=0;//高字节代表设备地址；第二字节代表寄存器地址，第三四字节代表值
u16 HuanjingShuju::m_AD[12]={0};
//地址1 肥泵；2 水泵；3 LORA网关；4 流量计
bool HuanjingShuju::m_OK=true;
HuanjingShuju::HuanjingShuju(Modbus *mm)
{
	// TODO 自动生成的构造函数存根
	m_modbus = mm;
}
void HuanjingShuju::run(u32 ms)  //2毫秒运行一次
{
	ms*=2;
	if(ms%100==0)//每0.1秒
	{
		if(chongfa)
		{
			m_modbus->DoCommand();//重发的时候是05指令，命令不前进
			chongfa--;
			if(!m_modbus)//重发了多次依旧没收到数据，命令就前进吧
			{
				HuanjingShuju::m_OK=false;
				m_modbus->CommandNet();//命令前进
			}
		}else	if(m_modbus->DoCommand())//判断是否05命令
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
				m_modbus->putReadCommand(1,101,1);//读肥泵变频器压力设定
				break;
			case 1:
				m_modbus->putReadCommand(2,101,1);//读清水泵变频器压力设定
				break;
			case 2:
				m_modbus->putReadCommand(3,40,32);//读LORA湿度值
				break;
			case 3:
				m_modbus->putReadCommand(1,200,2);//读肥泵变频器实际压力，实际频率
				break;
			case 4:
				m_modbus->putReadCommand(2,200,2);//读清水泵变频器实际压力，实际频率
				break;
			case 5:
				m_modbus->putReadCommand(4,0,10);//读电磁流量计瞬时流量，累计流量
				break;
		}
		
		m_curItem=(m_curItem+1)%6;

	}
	if (m_modbus->m_05OK)
	{
		m_modbus->CommandNet();//收到05设置指令，命令前进
		chongfa=0;
	}
	if (m_modbus->m_JieshouOK) {
		m_modbus->m_JieshouOK = false; //用完复位
		switch(m_modbus->m_shebeiAddr)
		{
			case 1://肥泵
				if(m_modbus->m_jicunqiShouLen==2)	
				{
					HuanjingShuju::m_shuju[32+163-140]=m_modbus->m_jicunqiShou[0];
					HuanjingShuju::m_shuju[32+165-140]=m_modbus->m_jicunqiShou[1];
				
				}else if(m_modbus->m_jicunqiShouLen==1)	
				{
					Cunchu::m_sysCanshu.m_feibengYaliSet=m_modbus->m_jicunqiShou[0];
				}
				break;
			case 2://水泵
				if(m_modbus->m_jicunqiShouLen==2)	
				{
					HuanjingShuju::m_shuju[32+162-140]=m_modbus->m_jicunqiShou[0];
					HuanjingShuju::m_shuju[32+164-140]=m_modbus->m_jicunqiShou[1];
				
				}else if(m_modbus->m_jicunqiShouLen==1)	
				{
					Cunchu::m_sysCanshu.m_shuibengYaliSet=m_modbus->m_jicunqiShou[0];
				}
				break;
			case 3://LORA网关
				if(m_modbus->m_jicunqiShouLen==32)
					memcpy(HuanjingShuju::m_shuju,m_modbus->m_jicunqiShou,64);
				break;
			case 4://电磁流量计
				if(m_modbus->m_jicunqiShouLen==10)	
				{
					HuanjingShuju::m_shuju[32+160-140]=m_modbus->m_jicunqiShou[0];//瞬时流量
					HuanjingShuju::m_shuju[32+167-140]=m_modbus->m_jicunqiShou[7];//累计流量
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
