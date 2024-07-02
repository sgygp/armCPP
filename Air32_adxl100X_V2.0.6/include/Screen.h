/*
 * Screen.h
 *
 *  Created on: 2023Äê11ÔÂ17ÈÕ
 *      Author: Administrator
 */

#ifndef SCREEN_H_
#define SCREEN_H_
#include"air32f10x_conf.h"
#include "W25X16.h"

const u8 VER[]={"Ver 2.06"};
class Screen {
public:
	Screen(W25X16 *m_);
	void init();
	void show();
	void showBatt(float ff);
	void Bianhuan(long *shu,u8 *xiaoshu);
	int m_page=0;
	bool m_once=true;
	int m_curWeizhi=0;
	W25X16 *m_25;
	u16 *m_shuju;
};

#endif /* SCREEN_H_ */
