/*
 * Screen.cpp
 *
 *  Created on: 2023年11月17日
 *      Author: Administrator
 */

#include "Screen.h"
#include "oled.h"
#include "oledfont.h"
#include "Anjian.h"
#include "RtcBkp.h"

Screen::Screen(W25X16 *m_) {
	// TODO 自动生成的构造函数存根
	m_25=m_;

}

void Screen::init() {
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
	OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	//OLED_ShowPicture(0,0,64,32,(u8 *)JNTG,1);
	OLED_ShowWave();
	OLED_Refresh();
	m_zongcishu=RtcBkp::getBkpData(BKP_DR2);
	if(m_zongcishu>0)
		m_zongcishu--;
	m_page=m_zongcishu;
	m_25->readShuju(m_shuju,m_page);
}

void Screen::show() {
	static u8 _p=0;
	//	bool key=Anjian::isKeyLED();
	//	static bool isDown=false;
	//	if(key)
	//	{
	//		isDown=true;
	//		ms=0;
	//		key=false;
	//		//m
	//	}else if(ms>20&&isDown)
	//	{
	//		isDown=false;
	//		key=true;
	//	}

	u32 key=Anjian::GetKey();

	if((key>>16)>3&&((key&0x00001)==ENTER))//长按
	{
		if(m_page)
			m_page--;
		else
			m_page=m_zongcishu;

		if((m_zongcishu-m_page)>9)
			m_page=m_zongcishu;
		m_25->readShuju(m_shuju,m_page);
		m_once=true;
		_p=0;

	}
	else if(key==ENTER)
	{
		_p=(_p+1)%6;
		m_once=true;

	}
	u8 fuhao[]={'A','S','D','N'};
	u8 xiaoshu[]={1,1,0,0};
	if(m_once)
	{
		OLED_Clear();
		u8 t=0;
		if(_p==0)
		{
			OLED_ShowChar(8,0*8,fuhao[(t+0)],8,1);OLED_ShowShuju(	20+8,	0*8,	m_shuju[(0)],		xiaoshu[(t+0)],8,1);
			OLED_ShowChar(8,1*8,fuhao[(t+1)],8,1);OLED_ShowShuju(	20+8,	1*8,	m_shuju[(1)],		xiaoshu[(t+1)],8,1);
			OLED_ShowChar(8,2*8,fuhao[(t+2)],8,1);OLED_ShowShuju(	20+8,	2*8,	m_shuju[(2)],		xiaoshu[(t+2)],8,1);
																		OLED_ShowShuju(	20+8,	3*8,	m_shuju[(9)],		1,8,1);
		}else if(_p==1)
		{
			OLED_ShowChar(8,0*8,'N',8,1);OLED_ShowShuju(	20+8,	0*8,	m_shuju[3],		xiaoshu[(t+3)],8,1);
			OLED_ShowChar(8,1*8,'T',8,1);OLED_ShowShuju(	20+8,	1*8,	m_shuju[4],		1,8,1);
			if(m_shuju[6]>9999)
				m_shuju[6]=9999;
			OLED_ShowShuju(	0,	2*8,	m_shuju[6],		0,8,1);OLED_ShowChar(0,2*8,'B',8,1);
			if(m_shuju[7]>99)
				m_shuju[7]=99;
			OLED_ShowShuju(	32,	2*8,	m_shuju[7],	0,8,1);OLED_ShowChar(40,2*8,'L',8,1);
			OLED_ShowChar(8,3*8,'F',8,1);OLED_ShowShuju(	20+8,	3*8,	m_shuju[5],		0,8,1);

		}else
		{
			t=_p-2;

			u16 shu;
			u8 temp=0;
			shu=m_shuju[(t*4+10)];
			Bianhuan(&shu,&temp);
			OLED_ShowChar(8,0*8,'V',8,1);OLED_ShowChar(8+6,0*8,'0'+t*2+0,8,1);OLED_ShowShuju(	20+8,	0*8,	shu,		temp,8,1);
			shu=m_shuju[(t*4+11)];
			Bianhuan(&shu,&temp);
			OLED_ShowChar(8,1*8,'U',8,1);OLED_ShowChar(8+6,1*8,'0'+t*2+0,8,1);OLED_ShowShuju(	20+8,	1*8,	shu,		temp,8,1);
			shu=m_shuju[(t*4+12)];
			Bianhuan(&shu,&temp);
			OLED_ShowChar(8,2*8,'V',8,1);OLED_ShowChar(8+6,2*8,'0'+t*2+1,8,1);OLED_ShowShuju(	20+8,	2*8,shu,		temp,8,1);
			shu=m_shuju[(t*4+13)];
			Bianhuan(&shu,&temp);
			OLED_ShowChar(8,3*8,'U',8,1);OLED_ShowChar(8+6,3*8,'0'+t*2+1,8,1);OLED_ShowShuju(	20+8,	3*8,	shu,		temp,8,1);
			t=_p*4+1;
		}


		OLED_ShowChar(0,3*8,'0'+m_zongcishu-m_page,8,1);
		OLED_Refresh();
		m_once=false;
	}
}
const u16 DIANYARONGLIANG[]={3220,3680,3740,3770,3790,3820,3870,3920,3980,4060,4200};
float jisuanDianliang(u16 dianya)
{
	float xx=0;
	if(dianya<3220)
		xx=0;
	else if(dianya>4200)
		xx=99.9;
	else
	{
		int i=0;
		for(;i<10;i++)
		{
			if(dianya>=DIANYARONGLIANG[i]&&dianya<DIANYARONGLIANG[i+1])
				break;
		}
		float  ff=dianya-DIANYARONGLIANG[i];
		ff=ff/(DIANYARONGLIANG[i+1]-DIANYARONGLIANG[i]);
		xx=i*10.0+ff*10.0;
		if(xx>99.9)
			xx=99.9;
	}
	return xx;
}

void Screen::showBatt(float ff) {
	OLED_Clear();
//	OLED_ShowShuju(	2,	2*8,	ff,	0,16,1);OLED_ShowString(44,2*8,(u8*)"mV",16,1);
	OLED_ShowShuju(	8,	2*8,	ff,	0,8,1);OLED_ShowString(40,2*8,(u8*)"mV",8,1);
	OLED_ShowString(8,3*8,(u8 *)VER,8,1);
	ff=jisuanDianliang(ff);
	int a=0;
	if(ff>=90)
		a=5;
	else if(ff<10)
		a=0;
	else
	{
		a=(ff-10)/20;
		a++;
	}
	OLED_ShowShuju(	34-8,	4,	ff,	0,8,1);
	OLED_ShowPicture(0,0,32,16,(u8*) (BATT[a]),1);
	OLED_ShowString(58,4,(u8*)"%",8,1);
	OLED_Refresh();

}

void Screen::Bianhuan(u16* shu, u8* xiaoshu) {

	if(*shu>9999)
	{
		*xiaoshu=0;
		*shu=(*shu)/10;
	}else
		*xiaoshu=1;

}
