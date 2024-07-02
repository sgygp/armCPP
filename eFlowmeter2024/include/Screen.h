/*
 * Screen.h
 *
 *  Created on: 2020年2月2日
 *      Author: Administrator
 */
#ifndef SCREEN_H_
#define SCREEN_H_
#include "Yiqi.h"
#include "Shuru.h"
#include "LCD7565.h"
#include "RealQuxian.h"

#define GUOTIMES 30//30秒无按键自动返回主画面
#define XUZONG_NO 8666

class Screen {
public:
	Screen(Yiqi *ainstr1, RealQuxian *quxian);
	virtual ~Screen();
	void run(u32 ms); //每秒运行一次
	void init();
	void LOGO();
	void setShuaxinQuxian();

private:
	void show();
	u32 ms = 0;
	Yiqi *ainstr;
	LCD7565 m_lcd;
	RealQuxian *m_quxian;

	Shuru m_shuru;
	u8 m_show;
	bool m_once;
	bool m_bShuaxin;
	bool m_bShuaxinQuxian;
	int m_guoTimes;
	u16 m4_20;
	bool bSec2; //
private:
	u8 DOT=0 ;
	u8 fanhuiHuamian=0;
	u8 fanhuiXiang=0;
	u8 m_curline = -1;
	double m_shuju = 0;
	u8 m_gai = 0;
	bool m_fu = false;
	u8 m_shifou = false;
	u8 m_jiaozhun;
	void ShowWord(u8 lie, u16 ww, u8 shan, u8 dot);
	void Gaishu(double *ff, u8 shan, u8 dot, bool ADD_SUB);
	u8 ShowItem(double *shuju, u8 xiaoshu,bool yunxufu);
	void IInt();
	void ShowMain(u8 item);
	void ShowInfo();
	void ShowInfoData();
	void ShowQuxian();
//	void ShowZufen(u8 x,u8 y,u8 item);
	void Input();
	void ShowGaiLeiji(u8 isFUleiji,u8 item,long *leiji,bool isGAI);
	void inQueren();
	void GaiLeiji(u8 item,long *leiji,bool isUP);

};

#endif /* SCREEN_H_ */
// ----------------------------------------------------------------------------

