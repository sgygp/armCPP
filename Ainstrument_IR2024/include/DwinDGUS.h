/*
 * DwinDGUS.h
 *
 *  Created on: 2020年2月2日
 *      Author: Administrator
 */
#ifndef DWINDGUS_H_
#define DWINDGUS_H_

#include "stm32f10x_conf.h"
#include"Ainstrument.h"
#include"HistoryData.h"
#include "Shuru.h"

#define SHOUKONGXIAN 4 //接收断开时间20mS

class DwinDGUS {
public:
	static u8 m_USART1_FA[256];
	static u8 m_USART1_SHOU[256];
	static u8 m_USART1_shouPos;
	static u32 m_USART1_curShou_ms;
	static bool m_bOK;

	u16 m_bufQuxian[128];
	char *m_bufMain;

public:
	DwinDGUS();
	virtual ~DwinDGUS();
	void init();
	u32 ReadDGUS();
	void ShowPicture(u16 page, u8 &show,int &curItem);
	void SetDGUS_Dizhi(u16 dizhi, char buf[], u8 geshu);
	void Show_KeyMove(u8 page, int item);
	void SetDGUS_Bianliang(u16 dizhi, u16 ww);
	void SetJiankong(u8 xx);
	void SetDGUS_BianliangDot(u16 dizhi, u8 dot);
	void SetMiaoshu_Ziku(u16 dizhi, u8 a,u8 b,u8 dot);
	void SendQuxian(u16 *m_data, u8 quxianNo,u8 geshu);


	void ShowNeibuCanshu(Ainstrument *ainstr);
	void Show4_20(u16 w);
	void ShowL_H(Ainstrument *ainstr);
	void ShowJiben(Ainstrument *ainstr);
	void ShowJiaozhunYici(Ainstrument *ainstr);
	void ShowJiaozhunHanliang(Ainstrument *ainstr);
	void ShowMainYiciX(Ainstrument *ainstr,u16 dizhi1,u16 dizhi2);
	void ShowMainX(Ainstrument *ainstr, bool yincang,u16 dizhi1,u16 dizhi2);
	void ShowMainRect(Ainstrument *ainstr,u16 dizhi,u16 x1,u16 y1,u16 x2,u16 y2);
	void ShowNO(Ainstrument *ainstr);
	void ShowInfo(Ainstrument *ainstr,u16 dizhi);
	void ShowQuxianAnniu(HistoryData *his,Ainstrument *ainstr);
	void ShowQuxian(HistoryData *his,Ainstrument *ainstr);
	void ShowQuxianYici(HistoryData *his,Ainstrument *ainstr);
	void ShowQuxianHanliang(Ainstrument *ainstr);
	void ShowInput(Shuru &shuru);


private:
	void FasongX();
	void FillRect(u16 dizhi,u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
	void ShowTubiao(u16 x1, u16 y1, u16 x2, u16 y2, u8 picture);
	void ShowRect(float bili, u16 coloru,u16 dizhi,u16 x1,u16 y1,u16 x2,u16 y2);
	u16 Color(u8 r,u8 g,u8 b);
}
;

#endif /* DWINDGUS_H_ */
