/*
 * DwinDGUS.cpp
 *
 *  Created on: 2020年2月2日
 *      Author: Administrator
 */

#include <math.h>
#include <string.h>
#include "DwinDGUS.h"
#include "delay.h"
#include "Timer.h"
#include "weizhi.h"
#include "usart2.h"

void DwinDGUS::DwinDGUS::init() {
	// TODO 自动生成的构造函数存根
	m_shouPos = 0;
	m_curShou_ms = 0;
	m_bOK = true;

}

DwinDGUS::DwinDGUS() {
	// TODO 自动生成的构造函数存根
	m_bufMain = (char*) m_bufQuxian;
	USART2_Init(115200);
}

DwinDGUS::~DwinDGUS() {
	// TODO 自动生成的析构函数存根
}

void DwinDGUS::FasongX() {
	u8 geshu = m_FA[2] + 3;
	if (geshu > 0) {
		USART2_TX(geshu);
		for (int i = 0; i < 100; i++) {
			delay_ms(1);
			if (m_bOK)
				break;
		}
		delay_ms(10);
	}
}
void DwinDGUS::FillRect(u16 dizhi, u16 x1, u16 y1, u16 x2, u16 y2, u16 color) {
	m_FA[0] = 0x5a;
	m_FA[1] = 0xa5;
	m_FA[2] = 0x11;
	m_FA[3] = 0x82;
	m_FA[4] = dizhi >> 8;
	m_FA[5] = dizhi; //变量地址
	m_FA[6] = 0x00;
	m_FA[7] = 0x04;
	m_FA[8] = 0x00;
	m_FA[9] = 0x01;

	m_FA[10] = x1 >> 8;
	m_FA[11] = x1;
	m_FA[12] = y1 >> 8;
	m_FA[13] = y1;
	m_FA[14] = x2 >> 8;
	m_FA[15] = x2;
	m_FA[16] = y2 >> 8;
	m_FA[17] = y2;
	m_FA[18] = color >> 8; //颜色0xffff为反色
	m_FA[19] = color;

	FasongX();

}
void DwinDGUS::ShowRect(float bili, u16 color, u16 dizhi, u16 x1, u16 y1,
		u16 x2, u16 y2) {
	if (bili < 0)
		bili = 0;
	else if (bili > 1)
		bili = 1;
	x2 = x1 + bili * (x2 - x1);
	FillRect(dizhi, x1, y1, x2, y2, color);

}
void DwinDGUS::ShowTubiao(u16 x1, u16 y1, u16 x2, u16 y2, u8 picture) {
	m_FA[0] = 0x5a;
	m_FA[1] = 0xa5;
	m_FA[2] = 0x15;
	m_FA[3] = 0x82;
	m_FA[4] = 0x00;
	m_FA[5] = 0x70; //变量地址
	m_FA[6] = 0x00;
	m_FA[7] = 0x06; //剪切粘贴图形
	m_FA[8] = 0x00;
	m_FA[9] = 0x01; //指令数
	m_FA[10] = 0x00;
	m_FA[11] = picture;

	m_FA[3 + 9] = x1 >> 8;
	m_FA[4 + 9] = x1;
	m_FA[5 + 9] = y1 >> 8;
	m_FA[6 + 9] = y1;
	m_FA[7 + 9] = x2 >> 8;
	m_FA[8 + 9] = x2;
	m_FA[9 + 9] = y2 >> 8;
	m_FA[10 + 9] = y2;
	m_FA[20] = x1 >> 8;
	m_FA[21] = x1;
	m_FA[22] = y1 >> 8;
	m_FA[23] = y1;

	FasongX();

}
void DwinDGUS::SetDGUS_BianliangDot(u16 dizhi, u8 dot) {
	m_FA[0] = 0x5a;
	m_FA[1] = 0xa5;

	m_FA[2] = 0x07;
	m_FA[3] = 0x82;
	dizhi += 5;
	m_FA[4] = dizhi >> 8;
	m_FA[5] = dizhi;
	m_FA[6] = 2;
	m_FA[7] = 4 - dot;
	m_FA[8] = dot;
	m_FA[9] = 0;
	FasongX();
}
void DwinDGUS::SetMiaoshu_Ziku(u16 dizhi, u8 a, u8 b, u8 dot) {
	m_FA[0] = 0x5a;
	m_FA[1] = 0xa5;

	m_FA[2] = 0x09;
	m_FA[3] = 0x82;
	m_FA[4] = dizhi >> 8;
	m_FA[5] = dizhi;
	m_FA[6] = a;
	m_FA[7] = b;
	m_FA[8] = 0; //左对齐
	m_FA[9] = 4 - dot; //整数位数
	m_FA[10] = dot; //小数位数
	m_FA[11] = 5; //数据类型 U16
	FasongX();
}

void DwinDGUS::ShowPicture(u16 page, u8 &show, int &curItem) {
	m_FA[0] = 0x5a;
	m_FA[1] = 0xa5;

	m_FA[2] = 0x04;
	m_FA[3] = 0x80;
	m_FA[4] = 0x03;
	m_FA[5] = page >> 8;
	m_FA[6] = page;
	FasongX();
	show = page;
	curItem = -1;
	ShowTubiao(0, 0, 0, 0, 0);
}
u32 DwinDGUS::ReadDGUS() {
	u32 dd = 0;
	if (!m_shouPos)
		return 0;
	dd = (Timer::ms5_jishu + Timer::MAXSHU - m_curShou_ms);
	dd = dd % Timer::MAXSHU;
	if (dd < SHOUKONGXIAN)
		return 0;

	dd=0;
	if (m_shouPos == 9 && m_SHOU[0] == 0x5A
			&& m_SHOU[1] == 0xA5) {
		//////////////返回值前2个字节代表地址，后两个字节代表数据值
		dd = m_SHOU[4];
		dd <<= 8;
		dd |= m_SHOU[5];
		dd <<= 8;
		dd |= m_SHOU[7];
		dd <<= 8;
		dd |= m_SHOU[8];
	} else if (m_shouPos == 11 && m_SHOU[0] == 0x5A
			&& m_SHOU[1] == 0xA5) {
		//////////////返回值前2个字节代表地址，后两个字节代表数据值
		dd = m_SHOU[5]; //取变量地址的低8位
		dd <<= 8;
		dd |= m_SHOU[8];
		dd <<= 8;
		dd |= m_SHOU[9];
		dd <<= 8;
		dd |= m_SHOU[10];
	} else
		m_SHOU[0] = 0;
	m_shouPos = 0;
	return dd;

}

void DwinDGUS::Show_KeyMove(u8 page, int item) {
	if (item < 0) {
		ShowTubiao(0, 0, 0, 0, 0);
		return;
	}

	if (page == 2)
		ShowTubiao(XS2[item], YS2[item], XE2[item], YE2[item], 22);
	else if (page == 3 || page == 15)
		ShowTubiao(XS3[item], YS3[item], XE3[item], YE3[item], 23);
	else if (page == 4)
		ShowTubiao(XS4[item], YS4[item], XE4[item], YE4[item], 24);
	else if (page == 6)
		ShowTubiao(XS6[item], YS6[item], XE6[item], YE6[item], 26);
	else if (page == 7)
		ShowTubiao(XS7[item], YS7[item], XE7[item], YE7[item], 29);
	else if (page == 10)
		ShowTubiao(XS10[item], YS10[item], XE10[item], YE10[item], 11);
	else if ((page > 30) && (page < 37))
		ShowTubiao(XS31[item], YS31[item], XE31[item], YE31[item], 30);
	else if (page == 40 || page == 41)
		ShowTubiao(XS40[item], YS40[item], XE40[item], YE40[item], 42);
}

void DwinDGUS::SetDGUS_Bianliang(u16 dizhi, u16 ww) {
	m_FA[0] = 0x5a;
	m_FA[1] = 0xa5;

	m_FA[2] = 0x05;
	m_FA[3] = 0x82;
	m_FA[4] = dizhi >> 8;
	m_FA[5] = dizhi;
	m_FA[6] = ww >> 8;
	m_FA[7] = ww;
	FasongX();
}
void DwinDGUS::SetDGUS_Dizhi(u16 dizhi, char buf[], u8 geshu) {
	u8 i;
	m_FA[0] = 0x5a;
	m_FA[1] = 0xa5;

	m_FA[2] = geshu + 3;
	m_FA[3] = 0x82;
	m_FA[4] = dizhi >> 8;
	m_FA[5] = dizhi;
	for (i = 0; i < geshu; i++)
		m_FA[6 + i] = buf[i];
	FasongX();
}
void DwinDGUS::SetJiankong(u8 xx) {
	m_FA[0] = 0x5a;
	m_FA[1] = 0xa5;

	m_FA[2] = 0x03;
	m_FA[3] = 0x80;
	m_FA[4] = 0x4F;
	m_FA[5] = xx;
	FasongX();
}

void DwinDGUS::SendQuxian(u16 *m_data, u8 quxianNo, u8 geshu) {
	m_FA[0] = 0x5a;
	m_FA[1] = 0xa5;

	m_FA[2] = 2 + geshu * 2;
	m_FA[3] = 0x84;
	m_FA[4] = quxianNo;
	for (int i = 0; i < geshu; i++) {
		m_FA[5 + i * 2] = m_data[i] >> 8;
		m_FA[6 + i * 2] = m_data[i];

	}
	FasongX();
}


void DwinDGUS::ShowQuxianYici(HistoryData *his, Ainstrument *ainstr) {
	float beilv = pow(10, ainstr->getCanshuZhi(2));
	u16 xx = 0;
	float ll = ainstr->getCanshuZhi(12) * beilv;
	float kuadu = ainstr->getCanshuZhi(13) * beilv - ll;

	xx = his->m_data[(his->getPR() + HistoryData::DATALEN - 1)
			% HistoryData::DATALEN];
	float ff = xx - ll;
	ff /= kuadu; //数值比例；
	ff *= 179;
	if (ff > 179)
		xx = 179;
	else
		xx = ff;

	m_bufQuxian[0] = xx;
	SendQuxian(m_bufQuxian, 0x01, 1);

}

void DwinDGUS::ShowQuxian(HistoryData *his, Ainstrument *ainstr) {
///曲线值固定0-180之间

	int ye = his->getXiangqianYe();
	int times = his->getPingmuShijian();
	if (times == 2)
		times = 4;
	else if (times == 4)
		times = 24;

	float beilv = pow(10, ainstr->getCanshuZhi(2));
	u16 xx = 0;
	float ll = ainstr->getCanshuZhi(12) * beilv;
	float kuadu = ainstr->getCanshuZhi(13) * beilv - ll;

	SetDGUS_Bianliang(0X0509, 0Xffff); //改变曲线通道07，用于消隐曲线（防止发送4次的过程中曲线乱变）
	int p = his->getPR();
	int k = (HistoryData::DATALEN * 2 + p - (ye + 1) * 480 * times)
			% HistoryData::DATALEN;
	for (int j = 0; j < 4; j++) {
		int b = k + j * 120 * times;
		for (int i = 0; i < 120; i++) {
			int a = (HistoryData::DATALEN + b + i * times)
					% HistoryData::DATALEN;
			xx = his->m_data[a];

			float ff = xx - ll;
			ff /= kuadu; //数值比例；
			ff *= 179;
			if (ff > 179)
				xx = 179;
			else
				xx = ff;

			m_bufQuxian[i] = xx;

		}
		SendQuxian(m_bufQuxian, 0x01, 120);
	}
	SetDGUS_Bianliang(0X0509, 0X0001); //改变曲线通道为00，正常显示
}

void DwinDGUS::ShowQuxianAnniu(HistoryData *his, Ainstrument *ainstr) {
	u16 xx = his->getPingmuShijian();
	m_bufMain[0] = xx >> 8;
	m_bufMain[1] = xx;
	if (xx == 1)
		xx = 24;
	else if (xx == 2)
		xx = 6;
	else
		xx = 1;
	m_bufMain[4] = 0;
	m_bufMain[5] = xx;
	xx = his->getXiangqianYe() + 1;
	m_bufMain[2] = xx >> 8;
	m_bufMain[3] = xx;
	SetDGUS_Dizhi(0X50, m_bufMain, 6);
	SetDGUS_BianliangDot(0x2C0, ainstr->getCanshuZhi(2));

}
void DwinDGUS::ShowNeibuCanshu(Ainstrument *ainstr) {
	u8 dot = ainstr->getCanshuZhi(2);
	float beilv = pow(10, dot);
	for (int i = 0; i < 7; i++) {

		float ff;
		if (i > 3)
			ff = ainstr->getCanshuZhi(18 + i) * 100; //固定2位小数
		else
			ff = ainstr->getCanshuZhi(18 + i) * beilv;
		short xx = ff;
		m_bufMain[0 + i * 2] = xx >> 8;
		m_bufMain[1 + i * 2] = xx;
	}
	u32 uu = ainstr->getCanshuZhi(25) * 1000; //曲线常数
	m_bufMain[17] = uu;
	uu >>= 8;
	m_bufMain[16] = uu;
	uu >>= 8;
	m_bufMain[15] = uu;
	uu >>= 8;
	m_bufMain[14] = uu;

	SetDGUS_Dizhi(0x0112, m_bufMain, 18);
	for (int i = 0; i < 5; i++) {

		float ff = ainstr->getCanshuZhi(i);
		if(i==3)
			ff*=100.0;
		u16 xx = ff;
		m_bufMain[0 + i * 2] = xx >> 8;
		m_bufMain[1 + i * 2] = xx;

	}
	SetDGUS_Dizhi(0X100, m_bufMain, 10);
	for (int i = 0; i < 4; i++)
		SetDGUS_BianliangDot(0x280 + 0x10 * i, dot);
}
void DwinDGUS::Show4_20(u16 w) {
	u8 i;
	for (i = 0; i < 4; i++) {
		m_bufMain[6 - i * 2] = 0;
		m_bufMain[7 - i * 2] = w % 10;
		w /= 10;
	}
	SetDGUS_Dizhi(0X1B, m_bufMain, 8);
}

void DwinDGUS::ShowL_H(Ainstrument *ainstr) {
	u8 dot = ainstr->getCanshuZhi(2);
	float beilv = pow(10, dot);
	float ff = ainstr->getCanshuZhi(12) * beilv; //m_ll
	int xx = ff;
	m_bufMain[0] = xx >> 8;
	m_bufMain[1] = xx;
	ff = ainstr->getCanshuZhi(13) * beilv; //m_hh
	xx = ff;
	m_bufMain[2] = xx >> 8;
	m_bufMain[3] = xx;
	SetDGUS_Dizhi(0x120, m_bufMain, 4);
	SetDGUS_BianliangDot(0x130, dot);
	SetDGUS_BianliangDot(0x140, dot);
}

void DwinDGUS::ShowJiben(Ainstrument *ainstr) {
	u8 dot = ainstr->getCanshuZhi(2);
	float beilv = pow(10, dot);
	float ff = ainstr->getCanshuZhi(12) * beilv; //m_ll
	int xx = ff;
	m_bufMain[0] = xx >> 8;
	m_bufMain[1] = xx;
	ff = ainstr->getCanshuZhi(13) * beilv; //m_hh
	xx = ff;
	m_bufMain[2] = xx >> 8;
	m_bufMain[3] = xx;
	ff = ainstr->getCanshuZhi(14) * beilv; //m_lingdian
	xx = ff;
	m_bufMain[4] = xx >> 8;
	m_bufMain[5] = xx;
	ff = ainstr->getCanshuZhi(15) * beilv; //m_baojinH
	xx = ff;
	m_bufMain[6] = xx >> 8;
	m_bufMain[7] = xx;
	ff = ainstr->getCanshuZhi(16) * beilv; //m_baojingL
	xx = ff;
	m_bufMain[8] = xx >> 8;
	m_bufMain[9] = xx;
	xx = ainstr->getCanshuZhi(17) * 1000.0; //m_kk2
	m_bufMain[10] = xx >> 8;
	m_bufMain[11] = xx;
	SetDGUS_Dizhi(0x100 + 12, m_bufMain, 12);
	for (int i = 0; i < 5; i++)
		SetDGUS_BianliangDot(0x230 + 0x10 * i, dot);

	xx = ainstr->getCanshuZhi(5); //m_4
	m_bufMain[10] = xx >> 8;
	m_bufMain[11] = xx;
	xx = ainstr->getCanshuZhi(6); //m_20
	m_bufMain[12] = xx >> 8;
	m_bufMain[13] = xx;
	xx = ainstr->getCanshuZhi(7); //m_20
	m_bufMain[14] = xx >> 8;
	m_bufMain[15] = xx;
	xx = ainstr->getCanshuZhi(8); //m_20
	m_bufMain[16] = xx >> 8;
	m_bufMain[17] = xx;

	SetDGUS_Dizhi(0x100 + 5, m_bufMain + 10, 8);

}
void DwinDGUS::ShowJiaozhunYici(Ainstrument *ainstr) {

	u8 dot = ainstr->getCanshuZhi(2);
	float beilv = pow(10, dot);
	float ff = ainstr->getCanshuZhi(10) * beilv; //m_ll
	int xx = ff;
	m_bufMain[0] = xx >> 8;
	m_bufMain[1] = xx;
	ff = ainstr->getCanshuZhi(11) * beilv; //m_hh
	xx = ff;
	m_bufMain[2] = xx >> 8;
	m_bufMain[3] = xx;

	SetDGUS_Dizhi(0x100 + 10, m_bufMain, 4);
	SetDGUS_BianliangDot(0X200 + 0x10 * 0, dot);
	SetDGUS_BianliangDot(0X200 + 0x10 * 1, dot);
	SetDGUS_BianliangDot(0X200 + 0x10 * 2, dot);

}

void DwinDGUS::ShowJiaozhunHanliang(Ainstrument *ainstr) {
	u8 dot = ainstr->getCanshuZhi(2);
	float beilv = pow(10, dot);
	float ff = ainstr->getHanliang2() * beilv;
	int xx = ff;
	m_bufMain[0] = xx >> 8;
	m_bufMain[1] = xx;

	SetDGUS_Dizhi(0x14, m_bufMain, 2);
}

void DwinDGUS::ShowQuxianHanliang(Ainstrument *ainstr) {
	u8 dot = ainstr->getCanshuZhi(2);
	float beilv = pow(10, dot);
	float ff = ainstr->getHanliang2() * beilv;
	int xx = ff;
	m_bufMain[0] = xx >> 8;
	m_bufMain[1] = xx;

	SetDGUS_Dizhi(0x53, m_bufMain, 2);
}

void DwinDGUS::ShowMainYiciX(Ainstrument *ainstr, u16 dizhi1, u16 dizhi2) {
	u8 dot = ainstr->getCanshuZhi(2);
	float beilv = pow(10, dot);
	m_bufMain[0] = 0;
	m_bufMain[1] = ainstr->getCanshuZhi(0);
	m_bufMain[2] = 0;
	m_bufMain[3] = ainstr->getCanshuZhi(1);
	float fL = ainstr->getCanshuZhi(12) * beilv;
	float fH = ainstr->getCanshuZhi(13) * beilv;
	int xx = fL;
	m_bufMain[4] = xx >> 8;
	m_bufMain[5] = xx;
	xx = fH;
	m_bufMain[6] = xx >> 8;
	m_bufMain[7] = xx;

	SetDGUS_Dizhi(dizhi1, m_bufMain, 8);
	SetDGUS_BianliangDot(dizhi2 + 0x20 * 0, dot);
	SetDGUS_BianliangDot(dizhi2 + 0x20 * 1, dot);
	SetDGUS_BianliangDot(dizhi2 + 0x20 * 2, dot);

}
u16 DwinDGUS::Color(u8 r, u8 g, u8 b) {
	u32 ww = 0;
	ww |= r;
	ww >>= 3;
	ww <<= 8;
	ww |= g;
	ww >>= 2;
	ww <<= 8;
	ww |= b;
	ww >>= 3;
	return (u16) ww;
}
void DwinDGUS::ShowMainRect(Ainstrument *ainstr, u16 dizhi, u16 x1, u16 y1,
		u16 x2, u16 y2) {
	float f1 = (ainstr->getHanliang() - ainstr->getCanshuZhi(12));
	float f2 = (ainstr->getCanshuZhi(13) - ainstr->getCanshuZhi(12));
	u16 color = Color(96, 96, 96);
	u8 bj = ainstr->getCanshuZhi(107);

	if (bj == 1) {
		color = Color(0, 0, 255); //Blue
	} else if (bj == 2) {
		color = Color(255, 0, 0); //RED
	}
	ShowRect(f1 / f2, color, dizhi, x1, y1, x2, y2);
}
void DwinDGUS::ShowMainX(Ainstrument *ainstr, bool yincang, u16 dizhi1,
		u16 dizhi2) {
	float ff = ainstr->getHanliang();
	u8 dot = ainstr->getCanshuZhi(2);
	if (dot > 3)
		dot = 0;
	for (int i = 0; i < dot; i++)
		ff *= 10.0;
//
//	 {	m_bufMain[8] = 0;
//		m_bufMain[9] = 11;
//	}
	if (dot == 0) //不想显示在前4位，想显示到后4位上
		dot = 4;
	int j = 0;
	for (int i = 0; i < 5; i++) {
		j = 4 - i; //从后面往前填数
		if (i == dot) {
			m_bufMain[j * 2] = 0;
			if (dot == 4)
				m_bufMain[j * 2 + 1] = 11;
			else
				m_bufMain[j * 2 + 1] = 10;

		} else {
			u16 x = ff;
			x %= 10;
			m_bufMain[j * 2] = 0;
			m_bufMain[j * 2 + 1] = x;
			ff /= 10;
		}
	}
	//以下为隐藏最前面的0
	if (dot == 4) { //无小数
		if (m_bufMain[1 * 2 + 1] == 0) {
			m_bufMain[1 * 2 + 1] = 11;
			if (m_bufMain[2 * 2 + 1] == 0) {
				m_bufMain[2 * 2 + 1] = 11;
				if (m_bufMain[3 * 2 + 1] == 0)
					m_bufMain[3 * 2 + 1] = 11;
			}

			if (m_bufMain[1 * 2 + 1] == 11) {
//				m_bufMain[1 * 2 + 1] = 11;
				///整体左移保证居中
				m_bufMain[1 * 2 + 1] = m_bufMain[2 * 2 + 1];
				m_bufMain[2 * 2 + 1] = m_bufMain[3 * 2 + 1];
				m_bufMain[3 * 2 + 1] = m_bufMain[4 * 2 + 1];
				m_bufMain[4 * 2 + 1] = 11;
			}
			if (m_bufMain[1 * 2 + 1] == 11 && m_bufMain[2 * 2 + 1] == 11) {
//				m_bufMain[1 * 2 + 1] = 11;
				///整体左移保证居中
				m_bufMain[1 * 2 + 1] = m_bufMain[2 * 2 + 1];
				m_bufMain[2 * 2 + 1] = m_bufMain[3 * 2 + 1];
				m_bufMain[3 * 2 + 1] = m_bufMain[4 * 2 + 1];
				m_bufMain[4 * 2 + 1] = 11;
			}
		}
	} else if (dot == 1) {
		if (m_bufMain[0 * 2 + 1] == 0) {
			m_bufMain[0 * 2 + 1] = 11;
			if (m_bufMain[1 * 2 + 1] == 0) {
//				m_bufMain[1 * 2 + 1] = 11;
				///整体左移保证居中
				m_bufMain[1 * 2 + 1] = m_bufMain[2 * 2 + 1];
				m_bufMain[2 * 2 + 1] = m_bufMain[3 * 2 + 1];
				m_bufMain[3 * 2 + 1] = m_bufMain[4 * 2 + 1];
				m_bufMain[4 * 2 + 1] = 11;

			}
		}
	} else if (dot == 2) {
		if (m_bufMain[0 * 2 + 1] == 0) {
			m_bufMain[0 * 2 + 1] = 11;
		}
	}

	if (yincang)
		memset(m_bufMain, 0xff, 10);
	SetDGUS_Dizhi(dizhi1, m_bufMain, 10);

/////////////////////////////////以下发送高低限图标跟数值
	u8 bj = ainstr->getCanshuZhi(107);
	u8 gaodixian = 0xff;
	if (bj == 1) {
		gaodixian = 0;
		ff = ainstr->getCanshuZhi(16);
	} else if (bj == 2) {
		gaodixian = 1;
		ff = ainstr->getCanshuZhi(15);
	}
	ff *= pow(10, dot);
	u16 ww = ff;
	m_bufMain[0] = 0;
	m_bufMain[1] = gaodixian;
	m_bufMain[2] = ww >> 8;
	m_bufMain[3] = ww;
	SetDGUS_Dizhi(dizhi1 + 9, m_bufMain, 4); //显示高限或者低限

//////////////////////////////////以下判断是用正常字体显示还是隐藏字符
	if (gaodixian != 0xff)
		SetMiaoshu_Ziku(dizhi2 + 4, 0, 10, dot);
	else
		SetMiaoshu_Ziku(dizhi2 + 4, 0xff, 0, dot);

}

void DwinDGUS::ShowNO(Ainstrument *ainstr) {
	u32 xx = ainstr->getCanshuZhi(9);
	m_bufMain[3] = xx ;xx>>=8;
	m_bufMain[2] = xx ;xx>>=8;
	m_bufMain[1] = xx ;xx>>=8;
	m_bufMain[0] = xx ;
	SetDGUS_Dizhi(0X9F, m_bufMain, 4);

}
void DwinDGUS::ShowInfo(Ainstrument *ainstr, u16 dizhi) {
	float ff = ainstr->getCanshuZhi(101);
	short xx = ff;
	float fx=ff;
	m_bufMain[0] = xx >> 8;
	m_bufMain[1] = xx;
	ff = ainstr->getCanshuZhi(102);
	fx=fx/ff;
	xx = ff;
	m_bufMain[2] = xx >> 8;
	m_bufMain[3] = xx;

	ff = ainstr->getCanshuZhi(104);
	xx = ff * 100.0;
	m_bufMain[4] = xx >> 8;
	m_bufMain[5] = xx;
	ff = ainstr->getCanshuZhi(103);
	xx = ff * 100.0;
	m_bufMain[6] = xx >> 8;
	m_bufMain[7] = xx;
	ff = ainstr->getCanshuZhi(105);
//	xx = ff * 10.0;
	xx = ff;
	m_bufMain[8] = xx >> 8;
	m_bufMain[9] = xx;
	float f1 = ainstr->getCanshuZhi(5);
	float f2 = ainstr->getCanshuZhi(6);
	ff = ainstr->getCanshuZhi(106);

	ff = 4 + 16.0 * (ff - f1) / (f2 - f1);
	xx = ff * 100.0;
	m_bufMain[10] = xx >> 8;
	m_bufMain[11] = xx;

	///////////////以下为转速显示
		xx = ainstr->getCanshuZhi(108);
		m_bufMain[12] = xx >> 8;
		m_bufMain[13] = xx;
		///////////////以下为转速显示
		xx = ainstr->getCanshuZhi(109);
		m_bufMain[14] = xx >> 8;
		m_bufMain[15] = xx;

		xx = fx*10000.0;
		m_bufMain[16] = xx >> 8;
		m_bufMain[17] = xx;


	SetDGUS_Dizhi(dizhi, m_bufMain, 18);

}
void DwinDGUS::ShowInput(Shuru &shuru) {
	for (int i = 0; i < 6; i++) {
		m_bufMain[i * 2] = shuru.m_shuzi[i] >> 8;
		m_bufMain[i * 2 + 1] = shuru.m_shuzi[i];

	}
	SetDGUS_Dizhi(0X80, m_bufMain, 12);

}
