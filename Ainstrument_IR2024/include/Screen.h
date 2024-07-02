/*
 * Screen.h
 *
 *  Created on: 2020年2月2日
 *      Author: Administrator
 */
#ifndef SCREEN_H_
#define SCREEN_H_

#include "Ainstrument.h"
#include "HistoryData.h"
#include "DwinDGUS.h"
#include "Anjian.h"
#include "KeyMove.h"
#include "Shuru.h"

#define GUOTIMES 30//30秒无按键自动返回主画面
#define NEIBUMIMA 6873

struct PopMenu{
	u8 m_iWhat;
	u8 m_fanHui;
};

class Screen {
public:
	Screen(Ainstrument *ainstr1,Ainstrument *ainstr2, HistoryData *m_history1,HistoryData *m_history2);
	virtual ~Screen();
	void run(); //每秒运行一次
	void show();
	void start();
	void gengxinQuxian();

private:
	Ainstrument *ainstr;
	Ainstrument *ainstr1;
	Ainstrument *ainstr2;
	HistoryData *m_history;
	HistoryData *m_history1;
	HistoryData *m_history2;
	DwinDGUS m_dgus;
	Anjian m_anjian;
	KeyMove m_keymove;
	Shuru m_shuru;
	PopMenu m_popMenu;
	u8 m_show;
	bool m_once;
	bool m_bShuaxin;
	bool m_bShuaxinQuxian;
	bool m_bShuaxinQuxianYici;
	int m_guoTimes;
	int m_curItem;
	u16 m4_20;
	bool bSec2; //
	u32 m_msJishu;
	bool m_bFasongJiankong;

	u8 m_curCaozuoxiang;

	void InputFanhui();
	void PopMenuFanhui();

};

#endif /* SCREEN_H_ */
// ----------------------------------------------------------------------------

