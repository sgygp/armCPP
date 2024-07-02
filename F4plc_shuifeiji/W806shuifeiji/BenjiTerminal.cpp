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
#include "CalendarTimer.h"


u8 BenjiTerminal::m_wushujuShijian=0;
u8 BenjiTerminal::fuweihouSec=0;
u16 BenjiTerminal::temp[1600];
BenjiTerminal::BenjiTerminal(Modbus* mm, Device* dev, bool is4G_)
{
    // TODO 自动生成的构造函数存根
    m_modbus = mm;
    m_device = dev;
    is4G = is4G_;
}

void BenjiTerminal::Reset()
{
	//HC595::out();
	fuweihouSec=3;
}

void BenjiTerminal::run()//2毫秒执行一次
{
    ms2++;
    if(ms2>=3600*500*24)//每天数据清零
    {
    	ms2=0;
    }
	if(is4G)
	{
		if(ms2 % 500 == 0)
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
		
			if((m_wushujuShijian / 60) > 30 )//30分钟没收到数据
			{
				Reset(); // 4g模块复位
				m_wushujuShijian = 0;
				Air720::m_bShuju=false;
			}
			int sec=ms2/500;
			if((sec+170) % 180 == 0)//每隔3分钟180000l
			{
				if(!Air720::IMEI)
					m_modbus->putQitaCommand((u8*)AT_IDTU_IMEI,0);
			}
			if((sec+170) % 180 == 10)//每隔3分钟180000l
			{
				if(!Air720::m_iccid[0])
				{
					Air720::m_OK=false;//发送读取CCID指令前先取消OK，收到CCID会置位OK
					m_modbus->putQitaCommand((u8*)AT_IDTU_ICCID,0);
				}
			}
			if((sec+170) % 180 == 20)
				m_modbus->putQitaCommand((u8*)AT_IDTU_LOC,0);
			if(sec%3600==0)//每小时对时一次
			{
				if(Cunchu::m_sysCanshu.m_timeNet)
					m_modbus->putQitaCommand((u8*)AT_IDTU_TIME,0);
			}
			
		}
	}
	int len=m_modbus->runTerminal(Cunchu::m_sysCanshu.m_addr);
	if(len)
	{
		if(is4G)
		{
			if(Air720::isBaohanString(m_modbus->tempSHOU,"rrpc,location"))
				Air720::getLoction(m_modbus->tempSHOU+14);
			else if(Air720::isBaohanString(m_modbus->tempSHOU,"rrpc,nettime"))
				Air720::setTime(m_modbus->tempSHOU+13);
			else if(Air720::isBaohanString(m_modbus->tempSHOU,"rrpc,geticcid"))
				Air720::getICCID(m_modbus->tempSHOU+14);
			else if(Air720::isBaohanString(m_modbus->tempSHOU,"rrpc,getimei"))
				Air720::getIMEI(m_modbus->tempSHOU+13);

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
	    if(jicunqiAddr + len <=76) 
		{
			temp[0]=Device::m_buzhou;
			if(Device::m_buzhou==6)
				temp[0]=Device::m_buzhou+=(bool)(m_device->shiduGuangaiX>=0);
			if(m_device->m_bZanting)
				temp[0]|=0x0100;

			memcpy(&temp[1],PortSet::m_faZhuangtai,6*2);
			temp[6]|=Device::m_mode<<8;
			temp[7]=Device::dangQianGuanQu;
			temp[8]=Device::yunXingShengyu;
			
			u8 aa=0;
			for(int i=0;i<8;i++)
			{
				if(Cunchu::m_dingshiCanshu[i].m_bQiyong)
					aa|=(1<<i);
				temp[10+i]=Cunchu::m_dingshiCanshu[i].m_celvNO;
				temp[10+8+i]=Cunchu::m_dingshiCanshu[i].m_hour;
				temp[10+16+i]=Cunchu::m_dingshiCanshu[i].m_fen;
			}
			temp[9]=aa;
			
			temp[40]=Cunchu::m_sysCanshu.m_shiduGuanQu&0xffff;
			temp[41]=Cunchu::m_sysCanshu.m_shiduGuanQu>>16;
			temp[42]=Cunchu::m_sysCanshu.m_shiduStart;
			temp[43]=Cunchu::m_sysCanshu.m_shiduEnd;
			memcpy(&temp[44],HuanjingShuju::m_shuju,64);
			
		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr], len * 2);
		    m_modbus->putReturnDataCommand();
		}else if(jicunqiAddr>=100&& jicunqiAddr + len <=135)
		{
			temp[0]=Device::YuanKong;
			temp[1]=m_device->m_curCelv.m_mode;
			temp[2]=m_device->m_curCelv.m_xTong;
			temp[3]=m_device->m_curCelv.m_xGuanqu&0xffff;
			temp[4]=m_device->m_curCelv.m_xGuanqu>>16;
			temp[5]=m_device->m_curCelv.m_jiaobanTime;
			temp[6]=m_device->m_curCelv.m_zongGuangai;
			temp[7]=m_device->m_curCelv.m_startGuangai;
			temp[8]=m_device->m_curCelv.m_endGuangai;
			for(int i=0;i<8;i++)
			{
				temp[9+i]=m_device->m_curCelv.m_jinshuiLiang[i];
				temp[9+8+i]=m_device->m_curCelv.m_shifeiLiang[i];
				temp[9+16+i]=m_device->m_feiliaotong[i].getCurRongliang_L();
			}
			temp[33]=Cunchu::m_sysCanshu.m_curCelvNO;
			temp[34]=0;
		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr-100], len * 2);
		    m_modbus->putReturnDataCommand();
			
		}else if(jicunqiAddr>=140&& jicunqiAddr + len <=179)
		{
//			u32 uu=12345678;
//			HuanjingShuju::m_shuju[32+27]=uu>>16;
//			HuanjingShuju::m_shuju[32+28]=uu&0xffff;
			memcpy(&temp,&HuanjingShuju::m_shuju[32],(29)*2);

		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr-140], len * 2);
		    m_modbus->putReturnDataCommand();
		}else if(jicunqiAddr>=199&& jicunqiAddr + len <=286)
		{
			memcpy(temp,&Cunchu::m_sysCanshu,(282-199+1)*2);
		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr-199+1], len * 2);
		    m_modbus->putReturnDataCommand();
		}
	    else if(jicunqiAddr >= 299 && jicunqiAddr + len <= 327) //读取系统参数
		{
		    temp[0] = (Device::YuanKong);
		    temp[1+0] = (Device::sysShebeiZhuangtai);
		    temp[1+1] = (Device::cpuUsed);
			temp[1+2] = Cunchu::m_sysCanshu.m_addr; //本机地址
			temp[1+3] =0;
			temp[1+4] =0;
//			temp[1+5] =Lora::m_bDingdian;
//			temp[1+6] =Lora::m_dizhi;
//			temp[1+7] =Lora::m_xinDao;
			temp[1+8]=Air720::XINHAO;
			temp[1+10]=Air720::WEIDU>>16;
			temp[1+9]=Air720::WEIDU;
			temp[1+12]=Air720::JINGDU>>16;
			temp[1+11]=Air720::JINGDU;
			u32 uu1=Air720::IMEI/100000000l;
			u32 uu2=Air720::IMEI%100000000l;
			temp[1+14] =uu1>>16;
			temp[1+13] =uu1;
			temp[1+16] =uu2>>16;
			temp[1+15] =uu2;
			
			memcpy(&temp[1+17],Air720::m_iccid,20);
			memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr - 299], len * 2);
		    m_modbus->putReturnDataCommand();
		}
		else if(jicunqiAddr>=398&& jicunqiAddr + len <=409)
		{
			temp[0]=Device::sysShebeiZhuangtai;
			temp[1]=Device::cpuUsed;
			temp[0+2]=Cunchu::m_sysCanshu.m_timeNet;
			temp[1+2]=(CalendarTimer::RTC_DataStructure.RTC_Year);//-100;
			temp[2+2]=(CalendarTimer::RTC_DataStructure.RTC_Month);
			temp[3+2]=(CalendarTimer::RTC_DataStructure.RTC_Date);
			temp[4+2]=(CalendarTimer::RTC_TimeStructure.RTC_Hours);
			temp[5+2]=(CalendarTimer::RTC_TimeStructure.RTC_Minutes);
			temp[6+2]=(CalendarTimer::RTC_TimeStructure.RTC_Seconds);
			temp[7+2]=0;    //秒，正常范围从 0 到 59，但允许至 61
			temp[8+2]=Cunchu::m_sysCanshu.m_riqiLock;    //秒，正常范围从 0 到 59，但允许至 61
		    memcpy((u8*)m_modbus->m_jicunqiFa, (u8*)&temp[jicunqiAddr - 398], len * 2);
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
		u32 xx=0;

	    if(jicunqiAddr == 299||jicunqiAddr==100) //远控寄存器，本地远控都可以操作
		{
		    if(Device::YuanKong == zhi) //
				return;
		    Device::YuanKong = zhi;
		    return;
		}

	    if(Device::YuanKong != is4G) //远控下只能4G操作，反之只能本地操作
		return;



	    switch(jicunqiAddr)
		{
			case 0://本地工作模式
				m_device->putCommand(zhi);//放置执行指令
				return;
			case 9:
				for(int i=0;i<8;i++)
				{
					if(zhi&(1<<i))
						Cunchu::m_dingshiCanshu[i].m_bQiyong=1;
					else
						Cunchu::m_dingshiCanshu[i].m_bQiyong=0;
				}
				isXie=true;
				break;
			case 40://湿度选择
				xx=Cunchu::m_sysCanshu.m_shiduGuanQu;
				Cunchu::m_sysCanshu.m_shiduGuanQu=(xx&0xffff0000)|zhi;
			   isXie=true;
				break;
			case 41://湿度选择
				xx=Cunchu::m_sysCanshu.m_shiduGuanQu;
				Cunchu::m_sysCanshu.m_shiduGuanQu=(xx&0x0000ffff)|(zhi<<16);
			   isXie=true;
				break;
			case 42://启动湿度值
				Cunchu::m_sysCanshu.m_shiduStart=zhi;
			   isXie=true;
				break;
			case 43://启动湿度值
				Cunchu::m_sysCanshu.m_shiduEnd=zhi;
			   isXie=true;
				break;
				
			case 101://灌溉模式
				m_device->m_curCelv.m_mode=zhi;
				break;
			case 102://通选择
				m_device->m_curCelv.m_xTong=zhi;
				break;
			case 103://罐区选择
				xx=m_device->m_curCelv.m_xGuanqu;
				m_device->m_curCelv.m_xGuanqu=(xx&0xffff0000)|zhi;
				break;
			case 104://罐区选择
				xx=m_device->m_curCelv.m_xGuanqu;
				m_device->m_curCelv.m_xGuanqu=(xx&0x0000ffff)|(zhi<<16);
				break;
			case 105://搅拌时间
				m_device->m_curCelv.m_jiaobanTime=zhi;
				break;
			case 106://灌溉总
				m_device->m_curCelv.m_zongGuangai=zhi;
				break;
			case 107://灌溉头
				m_device->m_curCelv.m_startGuangai=zhi;
				break;
			case 108://灌溉尾
				m_device->m_curCelv.m_endGuangai=zhi;
				break;
			case 109:
			case 110:
			case 111:
			case 112:
			case 113:
			case 114:
			case 115:
			case 116:
				m_device->m_curCelv.m_jinshuiLiang[jicunqiAddr-109]=zhi;
				break;
			case 117:
			case 118:
			case 119:
			case 120:
			case 121:
			case 122:
			case 123:
			case 124:
				m_device->m_curCelv.m_shifeiLiang[jicunqiAddr-117]=zhi;
				break;
		
			case 133: //装载策略
			   m_device->m_curCelv=Cunchu::m_celvCanshu[zhi];
			   Cunchu::m_sysCanshu.m_curCelvNO=zhi;
			   isXie=true;
				break;
			case 134: //清除策略
				memset(&m_device->m_curCelv,0,sizeof(CelvCanshu));
				break;
			case 135: //保存策略
			   Cunchu::m_celvCanshu[zhi%16]= m_device->m_curCelv;
			   isXie=true;
				break;
			case 290:
			{
				u8 a=zhi/100;
				u8 b=zhi%100;
				Cunchu::m_sysCanshu.m_D=a;
				Cunchu::m_sysCanshu.m_tongdiH=b;
				float h=0;
				if(Cunchu::m_sysCanshu.m_D==79)
					h=0.5;
				else if(Cunchu::m_sysCanshu.m_D==100)
					h=1.0;
				else	if(Cunchu::m_sysCanshu.m_D==130)
					h=2.0;
				else
					h=0.3;
				Cunchu::m_sysCanshu.m_muyeRongliangBaohu=h*100;
				isXie=true;
			}
				break;
			case 99:
			case 299:
				Device::YuanKong=zhi;
				break;
			case 302: //本机地址
				Cunchu::m_sysCanshu.m_addr = zhi;
				isXie = true;
				break;
			case 327: //重启
				if(zhi == 1)
					NVIC_SystemReset();
				return;
			case 328: //恢复出厂值
				if(zhi == 1)
					Cunchu::load();
				return;
			case 329: //保存出厂值
				if(zhi == 1)
					Cunchu::save();
				return;
			case 330: //初始化
				if(zhi == 1)
					Cunchu::MorenCanshu();
				return;
			case 400: //修改时间
			   Cunchu::m_sysCanshu.m_timeNet=zhi;
				isXie=true;
				break;
			case 401: //修改时间
				CalendarTimer::RTC_DataStructure.RTC_Year = zhi % 100;// + 100;
				CalendarTimer::SetCalendarTime();
				return;
			case 402: //修改时间
				if(zhi<1)
					zhi=1;
				CalendarTimer::RTC_DataStructure.RTC_Month = zhi;
				CalendarTimer::SetCalendarTime();
				return;
			case 403: //修改时间
				CalendarTimer::RTC_DataStructure.RTC_Date = zhi;
				CalendarTimer::SetCalendarTime();
				return;
			case 404: //修改时间
				CalendarTimer::RTC_TimeStructure.RTC_Hours= zhi;
				CalendarTimer::SetCalendarTime();
				return;
			case 405: //修改时间
				CalendarTimer::RTC_TimeStructure.RTC_Minutes = zhi;
				CalendarTimer::SetCalendarTime();
				return;
			case 406: //修改时间
				CalendarTimer::RTC_TimeStructure.RTC_Seconds = zhi;
				CalendarTimer::SetCalendarTime();
				return;
			case 408:
				//写任意数据，立即锁定
				{
					Cunchu::m_sysCanshu.m_riqiLock = 0;
					isXie = true;
				}
			break;
		}
		if(jicunqiAddr >=10&&jicunqiAddr<=17)
		{
			Cunchu::m_dingshiCanshu[jicunqiAddr-10].m_celvNO=zhi;
			isXie=true;
		}
		else if(jicunqiAddr >=18&&jicunqiAddr<=25)
		{
			Cunchu::m_dingshiCanshu[jicunqiAddr-18].m_hour=zhi;
			isXie=true;
		}
		else if(jicunqiAddr >=26&&jicunqiAddr<=33)
		{
			Cunchu::m_dingshiCanshu[jicunqiAddr-26].m_fen=zhi;
			isXie=true;
		}
		else if(jicunqiAddr ==268)//设置水泵压力
		{
			HuanjingShuju::m_command2=0x02650000|zhi;
		}
		else if(jicunqiAddr==270)//设置肥泵压力
		{
			HuanjingShuju::m_command2=0x01650000|zhi;
		}
		else if(jicunqiAddr >=199&&jicunqiAddr<=285)
		{
			u16 *xxxx=(u16*)&Cunchu::m_sysCanshu;
			if(jicunqiAddr >=200&&jicunqiAddr<=207&&zhi>14)//脉冲输入只能是主站1-14个输入口
				zhi=0;
			xxxx[jicunqiAddr-199+1]=zhi;
			isXie=true;
		}else
		if(jicunqiAddr >= 0x1080) // 00年1月1日
		{
		    u16 uu = Ant_Lock::getWordRiqi(&CalendarTimer::RTC_DataStructure);
		    if(jicunqiAddr == uu) //是解锁寄存器
			{
			    Cunchu::m_sysCanshu.m_riqiLock = zhi;
			    isXie = true;
			}
		}
			
	    if(isXie)
		{
		    Cunchu::WriteCunchu(0);
		    return;
		}
	}

}
