/*
 * Screen.h
 *
 *  Created on: 2023��11��17��
 *      Author: Administrator
 */

#ifndef SCREEN_H_
#define SCREEN_H_
#include"air32f10x_conf.h"
#include "W25X16.h"


class Screen {
public:
	Screen(W25X16 *m_);
	void init();
	void show();
	void Bianhuan(u16 *shu,u8 *xiaoshu);
	char m_page=0;
	bool m_once=true;
	int m_zongcishu=0;
	W25X16 *m_25;
	u16 m_shuju[30]={0};
};

#endif /* SCREEN_H_ */
