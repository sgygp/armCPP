/*
 * BenjiTerminal.cpp
 *
 *  Created on: 2021年3月15日
 *      Author: Administrator
 */
#include "string.h"
#include "BenjiTerminal.h"
#include "Cunchu.h"
#include "HuanjingShuju.h"
#include "AntLock.h"
#include "AIR720.h"
#include "command.h"
#include "Week.h"


u8 BenjiTerminal::m_wushujuShijian=0;
u8 BenjiTerminal::fuweihouSec=0;
u16 BenjiTerminal::temp[1600];
BenjiTerminal::BenjiTerminal(Modbus* mm, Device* dev, bool is4G_)
{
    // TODO 自动生成的构造函数存根
    m_modbus = mm;
    m_devices = dev;
    is4G = is4G_;
}

void BenjiTerminal::Reset()
{
    //执行硬件复位
	//HC595::set(12,0);//断电
	//HC595::out();
	fuweihouSec=3;
}

void BenjiTerminal::run()
{
    ms++;
	if(is4G)
	{
		if(ms % 100 == 0)
		{
			m_wushujuShijian++;
			if(fuweihouSec)
			{
				fuweihouSec--;
				if(!fuweihouSec)
				{
					//HC595::set(12,1);//上电
					//HC595::out();
				}
			}
		
			if((m_wushujuShijian / 60) > 30 )
			{
				Reset(); // 4g模块复位
				m_wushujuShijian = 0;
				Air720::m_bShuju=false;
			}
			if((ms+17000) % 18000 == 0)//每隔3分钟180000l
			{
				if(!Air720::IMEI)
					m_modbus->putQitaCommand((u8*)AT_IDTU_IMEI,0);
			}
			if((ms+17000) % 18000 == 1000)//每隔3分钟180000l
			{
				if(!Air720::m_iccid[0])
					m_modbus->putQitaCommand((u8*)AT_IDTU_ICCID,0);
			}
			if((ms+17000) % 18000 == 2000)
				m_modbus->putQitaCommand((u8*)AT_IDTU_LOC,0);
			
		}
	}
	int len=m_modbus->runTerminal(Cunchu::m_sysCanshu.m_addr);
	if(len)
	{
		if(is4G)
		{
			if(Air720::isBaohanString(m_modbus->tempSHOU,"rrpc,getimei"))
				Air720::getIMEI(m_modbus->tempSHOU+13);
			if(Air720::isBaohanString(m_modbus->tempSHOU,"rrpc,geticcid"))
				Air720::getICCID(m_modbus->tempSHOU+14);
			if(Air720::isBaohanString(m_modbus->tempSHOU,"rrpc,location"))
				Air720::getLoction(m_modbus->tempSHOU+14);

		}
	
	}

    if(m_modbus->m_JieshouOK)
	{
		if(is4G)
		{
			m_wushujuShijian = 0;
			Air720::m_bShuju=true;
		}
	    m_modbus->m_JieshouOK = false;
	    u16 jicunqiAddr = m_modbus->m_jicunqiShouAddr;
	    u16 len = m_modbus->m_jicunqiShouLen;
	    if(jicunqiAddr + len <= 60) //环境数据
		{
		    //			u16 *temp=m_modbus->m_jicunqiFa;
		    if(HuanjingShuju::m_Error[0] < GUZHANGCISHU)
			memcpy((u8*)temp, (u8*)HuanjingShuju::m_data1_4[0], 10 * 2);
		    else
			memset((u8*)temp, 0, 10 * 2);
		    if(HuanjingShuju::m_Error[1] < GUZHANGCISHU)
			memcpy((u8*)&temp[10], (u8*)HuanjingShuju::m_data1_4[1], 10 * 2);
		    else
			memset((u8*)&temp[10], 0, 10 * 2);
		    if(HuanjingShuju::m_Error[2] < GUZHANGCISHU)
			memcpy((u8*)&temp[20], (u8*)HuanjingShuju::m_data1_4[2], 10 * 2);
		    else
			memset((u8*)&temp[20], 0, 10 * 2);
		    if(HuanjingShuju::m_Error[3] < GUZHANGCISHU)
			memcpy((u8*)&temp[30], (u8*)HuanjingShuju::m_data1_4[3], 10 * 2);
		    else
			memset((u8*)&temp[30], 0, 10 * 2);
		    if(HuanjingShuju::m_Error[4] < GUZHANGCISHU)
			memcpy((u8*)&temp[40], (u8*)HuanjingShuju::m_data5, 20 * 2);
		    else
			memset((u8*)&temp[40], 0, 20 * 2);

		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr], len * 2);
		    m_modbus->putReturnDataCommand();
		}
	    else if(jicunqiAddr >= 5000 && jicunqiAddr + len <= 5005) //读取安全数据
		{
		    u16* buf = (u16*)(&Cunchu::m_sysCanshu.m_riqiLock);
		    for(int i = 0; i < 5; i++)
			temp[i] = buf[i];
		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr - 5000], len * 2);
		    m_modbus->putReturnDataCommand();
		}
	    else if(jicunqiAddr >= 1000 && jicunqiAddr + len <= (1000 + 16 * 100) //读取各个设备数据
	            &&
	            len <= 100) // 1000-2600寄存器
		{
		    u8 id = (jicunqiAddr - 1000) / 16;
		    u8 x;
		    for(int i = 0; i < 5; i++) //一次最多传输4个设备的数据（4*16个寄存器），多放一个
			{
			    x = id + i;
			    if(x > 99)
				break;
			    memcpy(&temp[x * 16], &Cunchu::m_Canshu[x], 32);
			}
		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr - 1000], len * 2);
		    m_modbus->putReturnDataCommand();
		}
	    else if(jicunqiAddr == 950)
		{
		    m_modbus->m_jicunqiFa[0] = m_gaoji;
		    m_modbus->putReturnDataCommand();
		}
	    else if(jicunqiAddr >= 900 && jicunqiAddr + len <= 916) //读取系统参数
		{
		    memcpy(&temp[0], &Cunchu::m_sysCanshu, 32);
		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr - 900], len * 2);
		    m_modbus->putReturnDataCommand();
		}
	    else if(jicunqiAddr >= 148 && jicunqiAddr + len <= (150 + 100 + 32) && len <= 100)
		{
		    temp[0] = Cunchu::m_sysCanshu.m_addr; //本机地址
		    temp[1] = Cunchu::YuanKong;           //远控
		    short abc = 0;
		    for(int i = 0; i < 100; i++)
			{
			    abc = Cunchu::m_Canshu[i].m_tmpZhengzaiyunxingSec;
			    if(abc < 0)
				abc = 0 - abc;
			    abc <<= 4;
			    temp[2 + i] = abc | Cunchu::m_Canshu[i].m_tmpZhuangtai;
			}
		    for(int i = 0; i < 8; i++)
			{
			    temp[2 + 100 + i * 2] = PortSet::m_OUT[i] >> 16;
			    temp[2 + 100 + i * 2 + 1] = PortSet::m_OUT[i];
			}

		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr - 148], len * 2);
		    m_modbus->putReturnDataCommand();
		}
	    else if(jicunqiAddr >= 300 && jicunqiAddr + len <= 327) //读取系统参数
		{
		    temp[0] = (Cunchu::benjiZhuangtai);
		    temp[1] = (Cunchu::cpuUsed);
			temp[2] = Cunchu::m_sysCanshu.m_addr; //本机地址
			temp[3] =Cunchu::m_sysCanshu.m_caijiTime;
			temp[4] =Cunchu::m_sysCanshu.m_huancaiAddr0;
//			temp[5] =Lora::m_bDingdian;
//			temp[6] =Lora::m_dizhi;
//			temp[7] =Lora::m_xinDao;
			temp[5] =0;
			temp[6] =0;
			temp[7] =0;
			temp[8]=Air720::XINHAO;
			temp[10]=Air720::WEIDU>>16;
			temp[9]=Air720::WEIDU;
			temp[12]=Air720::JINGDU>>16;
			temp[11]=Air720::JINGDU;
			u32 uu1=Air720::IMEI/100000000l;
			u32 uu2=Air720::IMEI%100000000l;
			temp[14] =uu1>>16;
			temp[13] =uu1;
			temp[16] =uu2>>16;
			temp[15] =uu2;
			
			memcpy(&temp[17],Air720::m_iccid,20);
			memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr - 300], len * 2);
		    m_modbus->putReturnDataCommand();
		}
	    else if(jicunqiAddr >= 98 && jicunqiAddr + len <= (100 + 7))
		{
		    temp[0] = (Cunchu::benjiZhuangtai);
		    temp[1] = (Cunchu::cpuUsed);

		    temp[2] = CalendarTimer::RTC_DataStructure.RTC_Year;
		    temp[1 +2] = CalendarTimer::RTC_DataStructure.RTC_Month;
		    temp[2 +2] = CalendarTimer::RTC_DataStructure.RTC_Date;
		    temp[3 +2] =CalendarTimer::RTC_TimeStructure.RTC_Hours;
		    temp[4 +2] = CalendarTimer::RTC_TimeStructure.RTC_Minutes;
		    temp[5 +2] = CalendarTimer::RTC_TimeStructure.RTC_Seconds;
		    temp[6 +2] = Week::getWeek(temp[2]+2000,temp[3],temp[4]);
		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr - 98], len * 2);
		    m_modbus->putReturnDataCommand();
		}
	    else if(jicunqiAddr >=3000 && jicunqiAddr + len <= 3600&& len <= 100)
		{
			for(int i = 0; i < 100; i++)
			{
			    memcpy(&temp[i*6],&Cunchu::m_Canshu[i],6*2);
			}
		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr - 3000], len * 2);
		    m_modbus->putReturnDataCommand();

		}
	}

    if(m_modbus->m_SetOK)
	{
	    m_wushujuShijian = 0;
	    m_modbus->m_SetOK = false;
	    m_modbus->putReturnSetCommand();
	    u16 zhi = m_modbus->m_jicunqiShou[0];
	    u16 jicunqiAddr = m_modbus->m_jicunqiShouAddr;
	    bool isXie = false;

	    if(jicunqiAddr == 149) //远控寄存器，本地远控都可以操作
		{
		    if(Cunchu::YuanKong == zhi) //
				return;
		    //切换到远控后，开关型设备复位
		    for(u16 i = 0; i < __DEVICES__; i++)
			{
			   // if(!m_devices[i].getIsDianji())
				m_devices[i].stop();
			}
		    Cunchu::YuanKong = zhi;
		    return;
		}

	    if(Cunchu::YuanKong != is4G) //远控下只能4G操作，反之只能本地操作
		return;

	    switch(jicunqiAddr)
		{
		case 100: //修改时间
			CalendarTimer::RTC_DataStructure.RTC_Year = zhi % 100;// + 100;
			CalendarTimer::SetCalendarTime();
			return;
		case 101: //修改时间
			if(zhi<1)
				zhi=1;
			CalendarTimer::RTC_DataStructure.RTC_Month = zhi;
			CalendarTimer::SetCalendarTime();
			return;
		case 102: //修改时间
			CalendarTimer::RTC_DataStructure.RTC_Date = zhi;
			CalendarTimer::SetCalendarTime();
			return;
		case 103: //修改时间
			CalendarTimer::RTC_TimeStructure.RTC_Hours= zhi;
			CalendarTimer::SetCalendarTime();
			return;
		case 104: //修改时间
			CalendarTimer::RTC_TimeStructure.RTC_Minutes = zhi;
			CalendarTimer::SetCalendarTime();
			return;
		case 105: //修改时间
			CalendarTimer::RTC_TimeStructure.RTC_Seconds = zhi;
			CalendarTimer::SetCalendarTime();
			return;
//		case 290: //
//			if(zhi)
//				Lora::toSet();
//			else
//				Lora::toWork();
//			else if(zhi==2)
//				Lora::toWakeUp();
			//m_modbus->putReturnSetCommand();
			m_modbus->DoCommand();//立即执行，如果这里不执行，下面这条语句会阻止DoCommand
			Modbus::m_bufDiannaoShou[0]=0;
//			Lora::m_bSet=zhi;
			return;

		case 148: //本机地址
		case 302: //本机地址
		    Cunchu::m_sysCanshu.m_addr = zhi;
		    isXie = true;
		    break;
		case 303: //采集周期
		    Cunchu::m_sysCanshu.m_caijiTime = zhi;
		    isXie = true;
		    break;
		case 912: //
		    Cunchu::m_sysCanshu.m_fenduanJiange = zhi;
		    isXie = true;
		    break;

		case 327: //重启
		    NVIC_SystemReset();
		    return;
		case 328: //恢复出厂值
		    if(zhi == 1)
			Cunchu::load();
		    return;
//		case 502: //初始化
//		    if(zhi == 1)
//			Cunchu::MorenCanshu();
//		    return;
		case 329: //保存出厂值
		    if(zhi == 1)
			Cunchu::save();
		    return;
		default:
		    break;
		}
	    if(jicunqiAddr >= 3000 && jicunqiAddr < 3600)
		{
			u16 id=(jicunqiAddr-3000)/6;
		    u16* uu = (u16*)&Cunchu::m_Canshu[id];
		    uu[(jicunqiAddr-3000)%6] = zhi;
		    isXie = true;
		}
	    else if(jicunqiAddr >= 1000 && jicunqiAddr < (1000 + 16 * 100)) //修改设备参数
		{
		    int x = jicunqiAddr - 1000;
		    int id = x / 16;
		    int item = x % 16;
/*		    if(item == 8)
			{
			    int y = Cunchu::m_Canshu[id].m_zhengzhuanShijian;
			    if(zhi < 100) //如果小于100，只修改分钟
				y = (y / 100) * 100 + zhi;
			    else if(zhi >= 2400) //通过设为24XX可以将时间一次设为0点几分
				y = zhi % 100;
			    Cunchu::m_Canshu[id].m_zhengzhuanShijian = y;
			}
		    else if(item == 9)
			{
			    int y = Cunchu::m_Canshu[id].m_fanzhuanShijian;
			    if(zhi < 100) //如果小于100，只修改分钟
				y = (y / 100) * 100 + zhi;
			    else if(zhi >= 2400) //通过设为24XX可以将时间一次设为0点几分
				y = zhi % 100;
			    Cunchu::m_Canshu[id].m_fanzhuanShijian = y;
			}
		    else if(item == 14)
			{
			    int y = Cunchu::m_Canshu[id].m_zhengzhuanShijian;
			    Cunchu::m_Canshu[id].m_zhengzhuanShijian = (y / 100) * 100 + zhi % 100;
			}
		    else if(item == 15)
			{
			    int y = Cunchu::m_Canshu[id].m_fanzhuanShijian;
			    Cunchu::m_Canshu[id].m_fanzhuanShijian = (y / 100) * 100 + zhi % 100;
			}
		    else */
		    if(item == 6)
			{
//			    if(zhi == 11) //写11对高位置位
//					Cunchu::m_Canshu[id].m_ctrType |= 0x8000;
//			    else if(zhi == 10) //写10清高位
//					Cunchu::m_Canshu[id].m_ctrType &= ~0x8000;
//			    else
				if(zhi<4)
					Cunchu::m_Canshu[id].m_ctrType = (Cunchu::m_Canshu[id].m_ctrType & 0xff00) | zhi;
				
				m_devices[id].stop();
			}
		    else
			{
			    u16* y = (u16*)&Cunchu::m_Canshu[id];
			    y += item;
			    *y = zhi;
			}
		    isXie = true;
		}
	    else if(jicunqiAddr >= 900 && jicunqiAddr <= 910)
		{
		    u16* uu = (u16*)&Cunchu::m_sysCanshu;
		    uu[jicunqiAddr - 900] = zhi;
		    isXie = true;
		}
	    else if(jicunqiAddr >= 0x1080) // 00年1月1日
		{
		    if(jicunqiAddr == 5000 && zhi == 0) // 5000寄存器写0，立即锁定
			{
			    Cunchu::m_sysCanshu.m_riqiLock = 0;
			    isXie = true;
			}
		    u16 uu = Ant_Lock::getWordRiqi(&CalendarTimer::RTC_DataStructure);
		    if(jicunqiAddr == uu) //是解锁寄存器
			{
			    Cunchu::m_sysCanshu.m_riqiLock = zhi;
			    isXie = true;
			}
		}
	    else if(jicunqiAddr >= 150 && jicunqiAddr < 250)
		{
		    int id = jicunqiAddr - 150;
		    u16 keyword = (id + 1) * 10 + zhi; // zhi=0，停止，1正转，2反转
		    Device::putCommand(keyword);
		}
	    if(isXie)
		{
		    Cunchu::WriteCunchu(0);
		    return;
		}
	}
}
