/*
 * HuanjingShuju.cpp
 *
 *  Created on: 2021年3月14日
 *      Author: Administrator
 */

#include "HuanjingShuju.h"
#include "Cunchu.h"
#include "string.h"
u16 HuanjingShuju::m_data1_4[4][10];
u16 HuanjingShuju::m_data5[20];
u8 HuanjingShuju::m_Error[5];
u8 HuanjingShuju::m_OK[];

HuanjingShuju::HuanjingShuju(Modbus *mm)
{
	// TODO 自动生成的构造函数存根
	m_modbus = mm;
	m_Error[0]=GUZHANGCISHU;
	m_Error[1]=GUZHANGCISHU;
	m_Error[2]=GUZHANGCISHU;
	m_Error[3]=GUZHANGCISHU;
	m_Error[4]=GUZHANGCISHU;

}
void HuanjingShuju::run(u32 ms)  //10毫秒运行一次
{
	ms*=10;
	m_modbus->runMain();

	u8 a = Cunchu::m_sysCanshu.m_qiyongZhanhao;
	u8 shebeishu=0;
	int x=0;
	for( x=0; x<5; x++)
		{
			if(a&(1<<x))
				shebeishu++;
		}
	if(shebeishu==0)
		return;
	u16 zhouqi = Cunchu::m_sysCanshu.m_caijiTime;
	if(zhouqi==0||zhouqi>600)
		zhouqi=1;
	int shijianjiange=100*zhouqi/shebeishu;//应该是直接乘以1000，但是ms这个变量是以10ms为单位
	shijianjiange*=10;

	u8* error=NULL;
	if ((ms%shijianjiange) == 0) //在采集时间里平均分配时间,用以放置发送指令
		{
			u8 t=0;
			for( x=0; x<5; x++)
				{
					if(a&(1<<x))//
						t++;
					else
						m_Error[x]=-1;//不采样

					if((t-1)==m_curItem)
						break;

				}
			t=1<<x;
			switch(t)
				{
				case 0x01:
					m_modbus->putReadCommand(Cunchu::m_sysCanshu.m_huancaiAddr0, 0, 10);
					error=m_Error;
					break;
				case 0x02:
					m_modbus->putReadCommand(Cunchu::m_sysCanshu.m_huancaiAddr1, 0, 10);
					error=m_Error+1;
					break;
				case 0x04:
					m_modbus->putReadCommand(Cunchu::m_sysCanshu.m_huancaiAddr2, 0, 10);
					error=m_Error+2;
					break;
				case 0x08:
					m_modbus->putReadCommand(Cunchu::m_sysCanshu.m_huancaiAddr3, 0, 10);
					error=m_Error+3;
					break;
				case 0x10:
					m_modbus->putReadCommand(Cunchu::m_sysCanshu.m_huancaiAddr4, 0, 20);
					error=m_Error+4;
					break;
				}
			if(error!=NULL)
				{
					(*error)++;
					if(*error>=GUZHANGCISHU)
						*error=GUZHANGCISHU;
				}


			m_curItem++;
			m_curItem%=shebeishu;

		}

	error=NULL;
	if (m_modbus->m_JieshouOK)
		{
			m_modbus->m_JieshouOK = false; //用完复位
			if (m_modbus->m_shebeiAddr== Cunchu::m_sysCanshu.m_huancaiAddr0)
				{
					memcpy(m_data1_4[0], m_modbus->m_jicunqiShou, 10 * 2);
					error=m_Error;
				}
			else if (m_modbus->m_shebeiAddr == Cunchu::m_sysCanshu.m_huancaiAddr1)
				{
					memcpy(m_data1_4[1], m_modbus->m_jicunqiShou, 10 * 2);
					error=m_Error+1;
				}
			else if (m_modbus->m_shebeiAddr == Cunchu::m_sysCanshu.m_huancaiAddr2)
				{
					memcpy(m_data1_4[2], m_modbus->m_jicunqiShou, 10 * 2);
					error=m_Error+2;
				}
			else if (m_modbus->m_shebeiAddr == Cunchu::m_sysCanshu.m_huancaiAddr3)
				{
					memcpy(m_data1_4[3], m_modbus->m_jicunqiShou, 10 * 2);
					error=m_Error+3;
				}
			else if (m_modbus->m_shebeiAddr == Cunchu::m_sysCanshu.m_huancaiAddr4)
				{
					memcpy(m_data5, m_modbus->m_jicunqiShou, 20 * 2);
					error=m_Error+4;
				}

		}
	if(error!=NULL)
		*error=0;//无错误

	for(int i=0; i<5; i++)
		{
			if(m_Error[i]<GUZHANGCISHU)
				m_OK[i]=1;
			else
				m_OK[i]=0;
		}
}
