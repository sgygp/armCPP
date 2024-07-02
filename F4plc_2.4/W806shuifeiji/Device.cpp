/*
 * Device.cpp
 *
 *  Created on: 2020年10月13日
 *      Author: Administrator
 */
#include <Device.h>
#include "HuanjingShuju.h"
u16 Device::KeyWord= 0;
//u16 Device::m_wendu;
//u16 Device::m_shidu;
//u16 Device::m_guangdu;
bool Device::m_dataOK;
u16 Device::m_command;
u16 Device::m_commandKey[];
u8 Device::m_commandR;
u8 Device::m_commandW;
u8 Device::zhuFengjiOut=0;

bool Device::isKeyOn(u16 no) {
	bool b = (KeyWord == no);
	if (b)
		KeyWord = 0;
	return (b);
}
void Device::run() {
	if(!Cunchu::m_Canshu[ID].qiyong)
		return;
	u8 runType ;
	if (Cunchu::YuanKong == 1)
		runType = 0;
	else {
		runType = (Cunchu::m_Canshu[ID].m_ctrType&0x00ff) + 1;//低字节为运行模式
	}
	if (runType > 5) //不可识别的运行模式时，改为手动
		runType = 1;

//	if (Device::m_command < 3 || Device::m_command > 99) //0-32(3-33)
//		Device::m_command = 0;
	if (isDianji)
		runDianji(runType);
	else
		runOnOff(runType);
	Cunchu::m_Canshu[ID].m_tmpZhuangtai=this->m_zhuangtai;
	if(m_bZhengzaiFenduan)
	Cunchu::m_Canshu[ID].m_tmpZhengzaiyunxingSec = m_curDuan;
	else
	Cunchu::m_Canshu[ID].m_tmpZhengzaiyunxingSec = this->m_dongzuoTime;


}
void Device::ChuliCommandDianji() {
	int no = Device::m_command;
	if ((ID + 1) != no / 10) //ID为0时，本机对应的寄存器范围10+；1->20+
		return;
	switch (no % 10) {
	case 1:
		zhengzhuan();
		break;
	case 2:
		fanzhuan();
		break;
	case 0:
		stop();
		break;
	}
	Device::m_command = 0;
}
void Device::ChuliCommandOnOff() {
	int no = Device::m_command;
	if ((ID + 1) != no / 10) //ID为0时，本机对应的寄存器范围10+；1->20+
		return;
	switch (no % 10) {
	case 0:
		Off();
		break;
	case 1:
		On();
		break;
	}
	Device::m_command = 0;
}
void Device::runDianji(u8 runType) {
	int shu=0;

	bool isIn1 = false;
	bool isIn2 = false;
	bool isIn3 = false;
	u16 bencitime = CalendarTimer::RTC_TimeStructure.RTC_Hours * 100
	          + CalendarTimer::RTC_TimeStructure.RTC_Minutes;
					
	if(ID>=24&&ID<36)
	{
		if(Cunchu::m_sysCanshu.m_yuxue>0)//启用了雨雪开关
		{
			if(PortSet::m_bYuxue>0)
				runType=99;//特殊处理
		}
	}
	m_bZhengzaiFenduan=false;
	switch (runType) {
	case 0: //远控中
		ChuliCommandDianji();
		break;

	case 1: //手动中
		ChuliCommandDianji();
		if (Cunchu::m_Canshu[ID].inNo1!= 0)
			isIn1 = isKeyOn(Cunchu::m_Canshu[ID].inNo1);
		if (Cunchu::m_Canshu[ID].inNo2!= 0)
			isIn2 = isKeyOn(Cunchu::m_Canshu[ID].inNo2);
		if (Cunchu::m_Canshu[ID].inNo3!= 0)
			isIn3 = isKeyOn(Cunchu::m_Canshu[ID].inNo3);
		if (isIn3)
			stop();
		if (isIn1)
			zhengzhuan();
		else if (isIn2)
			fanzhuan();
		break;
	case 2: //时控中
		if (oldTime < Cunchu::m_Canshu[ID].m_zhengzhuanShijian && bencitime >= Cunchu::m_Canshu[ID].m_zhengzhuanShijian)
			zhengzhuan();
		else if (oldTime < Cunchu::m_Canshu[ID].m_fanzhuanShijian && bencitime >= Cunchu::m_Canshu[ID].m_fanzhuanShijian)
			fanzhuan();
		break;
	case 3: //温控中
	case 4: //光控中
		if(Cunchu::m_Canshu[ID].m_gaoShuju<=Cunchu::m_Canshu[ID].m_diShuju) {//处理高低数据设置错误的情况

			Cunchu::m_Canshu[ID].m_gaoShuju = 0;
			Cunchu::m_Canshu[ID].m_diShuju = 0;
			Cunchu::m_Canshu[ID].m_ctrType = 0; //回手动
			Cunchu::write();
			break;
		}
		if(Cunchu::m_Canshu[ID].m_gaoShuju==Cunchu::m_Canshu[ID].m_diShuju)//高低数据相等不处理
			break;
		m_tmpShujuNO=Cunchu::m_Canshu[ID].m_huancaiNO;
		if(m_tmpShujuNO>4)
			m_tmpShujuNO=Cunchu::m_sysCanshu.m_huancaiNO;
		if(m_tmpShujuNO<1)
			m_tmpShujuNO=1;
		m_tmpShujuNO--;
		if (m_tmpShujuNO < 4 && HuanjingShuju::m_OK[m_tmpShujuNO]) 
		{
			short wendu = HuanjingShuju::m_data1_4[m_tmpShujuNO][1];
			short guangdu = HuanjingShuju::m_data1_4[m_tmpShujuNO][2]/10;
			if(runType==3)//温控
//				shu=310;
				shu=wendu;
			else		//光控
				shu=guangdu;

			bool fuluoji=(Cunchu::m_Canshu[ID].m_ctrType&0xff00);

			if(Cunchu::m_Canshu[ID].m_fenduan)
			{
				fenDuanChuli(shu, fuluoji);
				m_bZhengzaiFenduan=true;
			}
			else
			{
				if(!fuluoji)
				{ //负逻辑
					if(shu > Cunchu::m_Canshu[ID].m_gaoShuju && m_zhuangtai != ZHENGZHUANWANCHENG)
					zhengzhuan();
					else if(shu < Cunchu::m_Canshu[ID].m_diShuju && m_zhuangtai != FANZHUANWANCHENG)
					fanzhuan();
				}
				else
				{
					if(shu > Cunchu::m_Canshu[ID].m_gaoShuju && m_zhuangtai != FANZHUANWANCHENG)
					fanzhuan();
					else if(shu < Cunchu::m_Canshu[ID].m_diShuju &&
							m_zhuangtai != ZHENGZHUANWANCHENG)
					zhengzhuan();
				}
			}
		}

		break;
	case 99: //启用了雨雪开关，并且开关已打开
		if(m_zhuangtai==YIZANTING||m_zhuangtai==ZHENGZHUANZHONG||m_zhuangtai==ZHENGZHUANWANCHENG)
			fanzhuan();
		break;
	}
	oldTime = bencitime;

	bool x, y;

	if (m_jitingTime > 0) { //反转直接到正转之间的延时
		m_jitingTime--;
		if (m_jitingTime == 0)
			zhengzhuan();
		x = 0;
		y = 0;
	} else if (m_jitingTime < 0) { //正转直接到反转之间的延时
		m_jitingTime++;
		if (m_jitingTime == 0)
			fanzhuan();
		x = 0;
		y = 0;
	} else {

		if (m_dongzuoTime > 0) {
			m_dongzuoTime--;
			x = 1;
			y = 0;
			if (!m_dongzuoTime)
			{
			    if(m_bZhengzaiFenduan)
				m_zhuangtai = FENDUANZHONG; //分段中
			    else
				{
				    m_zhuangtai = ZHENGZHUANWANCHENG;           //正转完成
				    m_curDuan = Cunchu::m_Canshu[ID].m_fenduan; //正转完成后分段数最大
				}
			}

			else
				m_zhuangtai = ZHENGZHUANZHONG; //正转中
		} else if (m_dongzuoTime < 0) {
			m_dongzuoTime++;
			x = 0;
			y = 1;
			if (!m_dongzuoTime)
			{
			    if(m_bZhengzaiFenduan)
				m_zhuangtai = FENDUANZHONG; //分段中
			    else
				{
				    m_zhuangtai = FANZHUANWANCHENG; //反转完成
				    m_curDuan = 0;                  //正转完成后分段数为0
				}
			}

		        else
				m_zhuangtai = FANZHUANZHONG; //反转中
		} else {
			x = 0;
			y = 0;

		}
	}

	setOutBit(Cunchu::m_Canshu[ID].outNo1, x);
	setOutBit(Cunchu::m_Canshu[ID].outNo2, y);

}
void Device::runOnOff(u8 runType) {
	int shu=0;
	u8 item=0;

	bool isIn1 = false;
	u16 bencitime = CalendarTimer::RTC_TimeStructure.RTC_Hours * 100
	          + CalendarTimer::RTC_TimeStructure.RTC_Minutes;
					
					
	if(ID>=60&&ID<64&&Cunchu::m_sysCanshu.m_gensuiFengji>0)	//水泵跟随风机
	{
		item=ID-60;
		if(item<Cunchu::m_sysCanshu.m_gensuiFengji)
			runType=5;
	}
	switch (runType) {
	case 0: //远控中
		ChuliCommandOnOff();
		break;

	case 1: //手动中
		ChuliCommandOnOff();
		if (Cunchu::m_Canshu[ID].inNo1!= 0)
			isIn1 = isKeyOn(Cunchu::m_Canshu[ID].inNo1);
		if (isIn1)
			OnOff();
		break;
	case 2: //时控中
		if (oldTime < Cunchu::m_Canshu[ID].m_zhengzhuanShijian && bencitime >= Cunchu::m_Canshu[ID].m_zhengzhuanShijian)
			On();
		else if (oldTime < Cunchu::m_Canshu[ID].m_fanzhuanShijian && bencitime >= Cunchu::m_Canshu[ID].m_fanzhuanShijian)
			Off();
		break;
	case 3: //温控中
	case 4: //光控中
		if(Cunchu::m_Canshu[ID].m_gaoShuju<=Cunchu::m_Canshu[ID].m_diShuju) {

			Cunchu::m_Canshu[ID].m_gaoShuju = 0;
			Cunchu::m_Canshu[ID].m_diShuju = 0;
			Cunchu::m_Canshu[ID].m_ctrType = 0; //回手动
			Cunchu::write();

			break;
		}
		m_tmpShujuNO=Cunchu::m_Canshu[ID].m_huancaiNO;
		if(m_tmpShujuNO>4)
			m_tmpShujuNO=Cunchu::m_sysCanshu.m_huancaiNO;

		if (m_tmpShujuNO < 4 && HuanjingShuju::m_OK[m_tmpShujuNO]) {
			short wendu = HuanjingShuju::m_data1_4[m_tmpShujuNO][1];
			short guangdu = HuanjingShuju::m_data1_4[m_tmpShujuNO][2]/10;
			if(runType==3)//温控
				shu=wendu;
			else		//光控
				shu=guangdu;

			bool fuluoji=(Cunchu::m_Canshu[ID].m_ctrType&0xff00);
			if(!fuluoji) {//负逻辑
				if(shu>Cunchu::m_Canshu[ID].m_gaoShuju&&m_zhuangtai!=_ON_)
					On();
				else if(shu<Cunchu::m_Canshu[ID].m_diShuju&&m_zhuangtai!=_OFF_)
					Off();
			} else {
				if(shu>Cunchu::m_Canshu[ID].m_gaoShuju&&m_zhuangtai!=_OFF_)
					Off();
				else if(shu<Cunchu::m_Canshu[ID].m_diShuju&&m_zhuangtai!=_ON_)
					On();

			}
		}
		break;
        case 5: //从动，跟随其他设备启动
			if(zhuFengjiOut)
				On();
			else
				Off();
			break;
        }
        oldTime = bencitime;
        bool x;
	if (m_dongzuoTime) {
		x = 1;
		m_zhuangtai = _ON_;
	} else {
		x = 0;
		m_zhuangtai = _OFF_;
	}
	setOutBit(Cunchu::m_Canshu[ID].outNo1, x);
	if(ID==46)
		zhuFengjiOut=x;

}
void Device::setOutBit(u16 outNo, bool b) {
	u8 x1 =outNo% 100;
	if(!x1)
		return;
	u8 x2 = outNo / 100;

	if(x2>15)//最大16个站（含本站）
		return;
	x1--;
	if (b)
		PortSet::m_OUT[x2] |= (1 << x1);
	else
		PortSet::m_OUT[x2] &= (~(1 << x1));
	PortSet::outOnOff(x1,PortSet::m_OUT[x2]&(1 << x1));

}
void Device::OnFenduan(bool jin)//本次是前进还是后退
{
    int go = 0;
    if(jin) //需要前进
	{
		if(m_curDuan<0)
			m_curDuan=0;
	    if(m_curDuan < Cunchu::m_Canshu[ID].m_fenduan) //未全开
		{
			if(Cunchu::m_Canshu[ID].m_fenduan==0)
				Cunchu::m_Canshu[ID].m_fenduan=1;
		    go = Cunchu::m_Canshu[ID].m_yunxingSecs /Cunchu::m_Canshu[ID].m_fenduan;
		    if(m_curDuan == Cunchu::m_Canshu[ID].m_fenduan - 1) //最后一段+3秒
			go += 3;
		    m_dongzuoTime = go;
		    m_curDuan++;
		}
	}
    else
	{
		if(m_curDuan<0)
			m_curDuan=Cunchu::m_Canshu[ID].m_fenduan;
	    if(m_curDuan != 0) //未全关
		{
			if(Cunchu::m_Canshu[ID].m_fenduan==0)
				Cunchu::m_Canshu[ID].m_fenduan=1;
		    go = Cunchu::m_Canshu[ID].m_yunxingSecs /Cunchu::m_Canshu[ID].m_fenduan;
		    if(m_curDuan == 1) //最后一段+3秒
			go += 3;
		    m_dongzuoTime = 0 - go;
		    m_curDuan--;
		}
	}
}

void Device::fenDuanChuli(u16 shuju,bool fuluoji)
{
	u16 jiange=Cunchu::m_sysCanshu.m_fenduanJiange;
	if(jiange<3)
		jiange=3;
	if(CalendarTimer::RTC_TimeStructure.RTC_Minutes%jiange==0)
	{
		if(!benciFenzhongyichuli)
		{
			benciFenzhongyichuli=true;
			if(!fuluoji)
				{ //正逻辑
					if(shuju > Cunchu::m_Canshu[ID].m_gaoShuju)
						OnFenduan(true);//前进一段
					else if(shuju < Cunchu::m_Canshu[ID].m_diShuju)
						OnFenduan(false);//后退一段
				}
				else
				{
					if(shuju > Cunchu::m_Canshu[ID].m_gaoShuju)
						OnFenduan(false);//后退一段
					else if(shuju < Cunchu::m_Canshu[ID].m_diShuju)
						OnFenduan(true);//前进一段
				}
		
		}
	}else
		benciFenzhongyichuli=false;
}
