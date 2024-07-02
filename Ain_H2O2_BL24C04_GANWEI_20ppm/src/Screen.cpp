/*
 * Screen.cpp
 *
 *  Created on: 2020年2月2日
 *      Author: Administrator
 */

#include "Anjian.h"
#include "Screen.h"
#include "weizhi.h"
#include "Timer.h"
#include <math.h>

Screen::Screen(Ainstrument *ainstr1, Ainstrument *ainstr2, Ainstrument *ainstr3,
		HistoryData *m_history1, HistoryData *m_history2, HistoryData *m_history3) {
	// TODO 自动生成的构造函数存根
	this->ainstr = ainstr1;
	this->ainstr1 = ainstr1;
	this->ainstr2 = ainstr2;
	this->ainstr3 = ainstr3;
	this->m_history = m_history1;
	this->m_history1 = m_history1;
	this->m_history2 = m_history2;
	this->m_history3 = m_history3;
	m_show = 0xff;
	m_once = true;
	m_bShuaxin = true;
	m_bShuaxinQuxian = true;
	m_guoTimes = 0;
	m_curItem = -1;
	m4_20 = 0;
	bSec2 = false;
	m_msJishu = 0;
	m_bFasongJiankong = true;
	m_bShuaxinQuxianYici = false;
	m_curCaozuoxiang = 0;

}

void Screen::start() {
	m_anjian.KeyInit();
	m_dgus.ShowPicture(0, m_show, m_curItem);
	m_show = 0xff;
}

void Screen::gengxinQuxian() {
	u8 x = m_history->getPingmuShijian();
	if (x == 2)
		x = 4;
	else if (x == 4)
		x = 24;
	if (m_show == 10 && m_history->isBAutoShuxin()
			&& ((m_history->getPR() + x) % HistoryData::DATALEN
					== m_history->getPW())) {
		m_history->setPR(m_history->getPW());
		m_bShuaxinQuxianYici = true;
	}
}

Screen::~Screen() {
	// TODO 自动生成的析构函数存根
}
void Screen::show() {
	u16 ww = 0;
	m_anjian.PutKey(m_dgus.ReadDGUS());

	u32 jian = m_anjian.GetKey();
	if (jian != 0)
		m_guoTimes = 0;
	m_keymove.move(m_show, m_curItem, jian, m_once);
	//只处理上下左右移动
	//不处理40状态下按键输入

	switch (m_show) {
	case 1:
		/*
		if (m_once) {
			m_once = false;
			m_dgus.ShowMainYiciX(ainstr1, 0x5, 0x400);
			m_dgus.ShowMainYiciX(ainstr2, 0xa5, 0x460);
			m_dgus.ShowMainYiciX(ainstr3, 0xB5, 0x470);
		}
		if (m_bShuaxin) {
			m_bShuaxin = false;
			m_dgus.ShowMainX(ainstr1, bSec2 & (ainstr1->getCanshuZhi(107) > 0),
					0X00, 0X440);
			m_dgus.ShowMainX(ainstr2, bSec2 & (ainstr2->getCanshuZhi(107) > 0),
					0XA0, 0X4A0);
			m_dgus.ShowMainX(ainstr3, bSec2 & (ainstr3->getCanshuZhi(107) > 0),
					0XB0, 0X4B0);
			m_dgus.ShowMainRect(ainstr1, 0x60, 265, 28, 470, 28+193-167);
			m_dgus.ShowMainRect(ainstr2, 0x68, 265, 107, 470, 107+193-167);
			m_dgus.ShowMainRect(ainstr3, 0x78, 265, 186, 470, 186+193-167);
		}

		if (jian == 0x00ff0101 || jian == UP) {
			ainstr = ainstr1;
			m_history = m_history1;
			m_dgus.ShowPicture(2, m_show, m_curItem);
			m_once = true;
		} else if (jian == 0x00ff0102 || jian == DOWN) {
			ainstr = ainstr2;
			m_history = m_history2;
			m_dgus.ShowPicture(2, m_show, m_curItem);
			m_once = true;
		} else if (jian == 0x00ff0103 || jian ==RIGHT ) {
			ainstr = ainstr3;
			m_history = m_history3;
			m_dgus.ShowPicture(2, m_show, m_curItem);
			m_once = true;
		} else if (jian != 0) {
			ainstr1->m_bXY = true;
			ainstr2->m_bXY = true;
		}
		break;
		 */
		if (m_once) {
			m_once = false;
			m_dgus.ShowMainYiciX(ainstr1, 0x5, 0x400);
			//			m_dgus.ShowMainYiciX(ainstr2, 0xa5, 0x460);
		}
		if (m_bShuaxin) {
			m_bShuaxin = false;
			m_dgus.ShowMainX(ainstr1, bSec2 & (ainstr1->getCanshuZhi(107) > 0),
					0X00, 0X440);
			//			m_dgus.ShowMainX(ainstr2, bSec2 & (ainstr2->getCanshuZhi(107) > 0),
			//					0XA0, 0X4A0);
			//			m_dgus.ShowMainRect(ainstr1, 0x60, 19, 167, 223, 194);
			//			m_dgus.ShowMainRect(ainstr2, 0x68, 256, 167, 460, 194);
			m_dgus.ShowMainRect(ainstr, 0x60, 57, 167, 422, 194);

		}

		if (jian == 0x00ff0101 || jian == ENTER) {
			ainstr = ainstr1;
			m_history = m_history1;
			m_dgus.ShowPicture(2, m_show, m_curItem);
			//m_curItem=3;//默认选中 系统信息
			m_once = true;
		} /*else if (jian == 0x00ff0102 || jian == DOWN) {
			ainstr = ainstr2;
			m_history = m_history2;
			m_dgus.ShowPicture(2, m_show, m_curItem);
			m_once = true;
		}*/ else if (jian != 0) {
			ainstr1->m_bXY = true;
			//			ainstr2->m_bXY = true;
		}
		break;

	case 2: ////////////////////////////////////////////////////////////////////////////////////////////按钮画面
		if (m_once) {
			m_dgus.Show_KeyMove(m_show, m_curItem);
			m_dgus.SetDGUS_Bianliang(0x000B, (u16) (ainstr->getCanshuZhi(0)));
			m_once = false;
		}

		if (jian == (0x00F00000 | (u16) ainstr->getCanshuZhi(7))) { //判断校准密码
			if (ainstr->getCanshuZhi(4)) //允许校零
				m_dgus.ShowPicture(3, m_show, m_curItem);
			else
				m_dgus.ShowPicture(15, m_show, m_curItem);
			m_once = true;
		} else if (jian == (0x00F10000 | (u16) ainstr->getCanshuZhi(8))) { //判断参数密码
			m_dgus.ShowPicture(4, m_show, m_curItem);
			m_once = true;
		} else if (jian == (0x00F10000 | NEIBUMIMA)) { ////内部参数 密码7788 init中初始化
			m_dgus.ShowPicture(6, m_show, m_curItem);
			m_once = true;
		} else if (jian == (0x00F00000 | 5555L)) { //密码恢复初始值
			ainstr->setCanshuZhi(7, 1000);
			ainstr->setCanshuZhi(8, 2000);
			ainstr->WriteCanshu();
			m_show = 0xff;
		} else if (jian == 0x00FF0025) { //点击了曲线图按钮
			m_history->setPingmuShijian(1); //1-1小时 ；2-4小时 ；4-24小时
			m_history->setXiangqianYe(0);
			m_history->setBAutoShuxin(true); //自动刷新曲线
			m_history->setPR(m_history->getPW());
			m_dgus.ShowPicture(10, m_show, m_curItem);
			m_bShuaxinQuxian = true;
			m_once = true;
		} else if (jian == 0x00FF0026) { //点击了返回按钮
			m_show = 0xff;
		} else if (jian == 0x00FF0027) { //点击了系统信息按钮
			m_dgus.ShowPicture(14, m_show, m_curItem);
			m_once = true;
		} else if (jian == 0x00FF00F0 || jian == 0x00FF00F1) { //进入按键输入密码画面
			m_shuru.m_bFuShuru = false;
			m_shuru.m_data = 0;
			m_shuru.m_fanhuiHuamian = 2;
			m_shuru.m_guangbiao = 1;
			m_shuru.m_dot = 0;
			m_shuru.toShuzi();
			m_dgus.ShowPicture(40, m_show, m_curItem);
			m_once = true;

		} else if (jian == (0x00FF0023)) { //点击了校准时钟按钮
			m_dgus.ShowPicture(7, m_show, m_curItem);
			m_bFasongJiankong = true;
			m_msJishu = Timer::ms5_jishu; //这个变量的目的是延时再发送键控命令
			m_once = true;
		} else if (jian == ENTER) { //按键回车后发送键码（以便下次接收按键）
			m_anjian.SendKey(0X00FF0000 | KEY2[m_curItem], m_curItem);
			m_curCaozuoxiang = KEY2[m_curItem];
		}
		break;
	case 3: //////////////////////////////////////////////////////////////////////////////////////////////////////////校准画面
	case 15: //////////////////////////////////////////////////////////////////////////////////////////////////////////校准画面
		if (m_once) {
			m_once = false;
			m_dgus.Show_KeyMove(m_show, m_curItem);
			m_dgus.ShowJiaozhunYici(ainstr);
		}
		if (m_bShuaxin) {
			m_bShuaxin = false;
			m_dgus.ShowJiaozhunHanliang(ainstr);
		}

		if (jian == 0x00ff000A || jian == 0x00ff000B) //按下了修改按钮
		{
			m_curCaozuoxiang = jian;
			m_dgus.SetJiankong((u8) ainstr->getCanshuZhi(2) + 0xf0);
		} else if ((jian & 0x00ff0000) == 0x00f00000)	//修改了含量
		{
			u16 zhi = jian;
			float ff = zhi / pow(10, ainstr->getCanshuZhi(2));
			ainstr->setCanshuZhi(m_curCaozuoxiang, ff);
			ainstr->WriteCanshu();
			m_dgus.SetDGUS_Bianliang(0x100 + m_curCaozuoxiang, zhi);
		} else if (jian == (0x00FE0001)) { //校准零点
			ainstr->setJiaozhun(1);
		} else if (jian == (0x00ff0001)) { //校准量程
			ainstr->setJiaozhun(2);
		} else if (jian == 0x00ff00AA || jian == 0xAA) {
			m_once = true;
			m_dgus.ShowPicture(2, m_show, m_curItem);
		} else if (jian == 0x33 || jian == 0x34) { //校准零点或量程
			m_once = true;
			m_popMenu.m_iWhat = jian - 0x32;
			m_popMenu.m_fanHui = 3;
			m_dgus.ShowPicture(31 + jian - 0x33, m_show, m_curItem);
		} else if (jian == 10 || jian == 11) { //去修改零点气含量
			m_shuru.m_bFuShuru = false;
			m_shuru.m_dot = ainstr->getCanshuZhi(2);
			float ff = ainstr->getCanshuZhi(10 + jian - 10);
			ff *= pow(10, m_shuru.m_dot);
			m_shuru.m_data = ff;
			m_shuru.m_fanhuiHuamian = m_show;
			m_shuru.m_guangbiao = 1;
			m_shuru.toShuzi();
			m_dgus.ShowPicture(41, m_show, m_curItem);
			m_once = true;
		} else if (jian == ENTER) {
			m_anjian.SendKey(KEY3[m_curItem], m_curItem);
			m_curCaozuoxiang = KEY3[m_curItem];
		}
		break;
	case 4: ///////////////////////////////////////////////////////////////////////////////////////////////////////基本参数画面
		if (m_once) {
			m_once = false;
			m_dgus.Show_KeyMove(m_show, m_curItem);
			m_dgus.ShowJiben(ainstr);
		}

		if ((jian >= 0x00ff000C && jian <= 0x00ff0010)) //按下了修改按钮
		{
			m_curCaozuoxiang = jian;
			m_dgus.SetJiankong((u8) ainstr->getCanshuZhi(2) + 0xf0);
		} else if ((jian >> 16) == 0x0105 || (jian >> 16) == 0x0106	) {
			u16 ll = jian;
			jian >>= 16;
			m_dgus.SetDGUS_Bianliang(jian, ll);	//送入屏幕
			jian-=0x100;
			ainstr->setCanshuZhi(jian, ll);
			ainstr->WriteCanshu();
		} else if ((jian >> 16) == 0x00f0)	//修改了
		{
			short zhi = jian;
			float ff = zhi / pow(10, ainstr->getCanshuZhi(2));
			ainstr->setCanshuZhi(m_curCaozuoxiang, ff);
			ainstr->WriteCanshu();
			m_dgus.SetDGUS_Bianliang(0x100 + m_curCaozuoxiang, zhi);	//送入屏幕
		} else if ((jian >> 16) == 0x0107 || (jian >> 16) == 0x0108
				|| (jian >> 16) == 0x0111) { //密码+线性值
			u16 ll = jian;
			jian >>= 16;
			if (jian == 0X0111)
				ainstr->setCanshuZhi(jian - 0X100, short(ll) / 1000.0f);
			else
				ainstr->setCanshuZhi(jian - 0X100, ll); //7,8地址为密码1,2
			ainstr->WriteCanshu();
			m_dgus.SetDGUS_Bianliang(jian, ll);	//送入屏幕
		} else if (jian == 0xfe0001) { //恢复出厂值
			m_once = true;
			ainstr->load();
			NVIC_SystemReset();

		} else if (jian == 0x00ff0001) {
			m_once = true;
			m4_20 = ainstr->getCanshuZhi(5);
			ainstr->setCanshuZhi(38, 1); //调整4毫安
			m_dgus.ShowPicture(12, m_show, m_curItem);
		} else if (jian == 0x00ff0002) {
			m_once = true;
			m4_20 = ainstr->getCanshuZhi(6);
			ainstr->setCanshuZhi(38, 2); //调整20毫安
			m_dgus.ShowPicture(12, m_show, m_curItem);
		} else if (jian == 0x00ff00AA || jian == (100 + 0XAA)) { //返回
			m_once = true;
			m_dgus.ShowPicture(2, m_show, m_curItem);
		} else if (jian == (100 + 0XA0)) { //恢复出厂
			m_once = true;
			m_popMenu.m_iWhat = 3 + jian - (100 + 0XA0);
			m_popMenu.m_fanHui = 4;
			m_dgus.ShowPicture(33, m_show, m_curItem);
		} else if (jian > 100 && jian < 140) //修改参数
		{
			ww = jian - 100;
			m_shuru.m_bFuShuru = false;
			if (ww < 9)
				m_shuru.m_dot = 0; //是否调整电流在41输入过程中判断，此处不做判断
			else if (ww == 17) {
				m_shuru.m_dot = 3;
				m_shuru.m_bFuShuru = true;
			} else {
				m_shuru.m_dot = ainstr->getCanshuZhi(2);
				if (ww == 14) //静态补偿，允许负输入
					m_shuru.m_bFuShuru = true;
			}
			float ff = ainstr->getCanshuZhi(ww);
			ff *= pow(10, m_shuru.m_dot);
			m_shuru.m_data = ff;
			m_shuru.m_fanhuiHuamian = 4;
			m_shuru.m_guangbiao = 1;
			m_shuru.toShuzi();
			m_dgus.ShowPicture(41, m_show, m_curItem);
			m_once = true;
		} else if (jian == ENTER) {
			m_anjian.SendKey(100 + KEY4[m_curItem], m_curItem);
			m_curCaozuoxiang = KEY4[m_curItem];

		}
		break;
	case 6: //////////////////////////////////////////////////////////////////////////////////////////////内部参数画面
		if (m_once) {
			m_once = false;
			m_dgus.Show_KeyMove(m_show, m_curItem);
			m_dgus.ShowNeibuCanshu(ainstr);
		}
		if ((jian >= 0x00ff0012 && jian <= 0x00ff0015)) //按下了修改按钮
		{
			m_curCaozuoxiang = jian;
			m_dgus.SetJiankong((u8) ainstr->getCanshuZhi(2) + 0xf0);
		} else if ((jian >> 16) == 0x00f0)	//修改了
		{
			short zhi = jian;
			float ff = zhi / pow(10, ainstr->getCanshuZhi(2));
			ainstr->setCanshuZhi(m_curCaozuoxiang, ff);
			ainstr->WriteCanshu();
			m_dgus.SetDGUS_Bianliang(0x100 + m_curCaozuoxiang, zhi);	//送入屏幕
		} else if (((jian >> 16) >= 0x0116 && (jian >> 16) <= 0x0119)
				|| (jian >> 16) == 0x0103) { //补偿，恒温，曲线，光源频率
			u16 ll = jian;
			jian >>= 16;
			if (jian == 0X0103&&ll == 255) {
				u32 uu=ainstr->getCanshuZhi(9);
				uu%=10000;
				u32 u1=ainstr->getCanshuZhi(23) * 100+0.5;
				uu=u1*10000+uu;
				ainstr->setCanshuZhi(9,uu ); //补偿2的值设为本机编号
				ainstr->setCanshuZhi(23, 0); //补偿2恢复为零
			}else if (jian == 0X0103&&ll == 254) {
				u32 uu=ainstr->getCanshuZhi(9);
				uu/=10000;
				u32 u1=ainstr->getCanshuZhi(23) * 100+0.5;
				uu=uu*10000+u1;
				ainstr->setCanshuZhi(9,uu ); //补偿2的值设为本机编号
				ainstr->setCanshuZhi(23, 0); //补偿2恢复为零
			}else {
				ainstr->setCanshuZhi(jian - 0X100, short(ll) / 100.0f);
				m_dgus.SetDGUS_Bianliang(jian, ll);	//送入屏幕
			}
			m_once = true;
			ainstr->WriteCanshu();
		} else if (jian >= 0x00FE0000 && jian <= 0x00FE0005) {
			m_once = true;

			u16 xx = jian;
			int yy = ainstr->getCanshuZhi(xx);
			if (xx == 0)
				yy = (yy + 1) % 12;
			else if (xx == 1)
				yy = (yy + 1) % 3;
			else if (xx == 2) {
				yy = (yy + 1) % 4;

			} else if (xx == 4)
				yy = (yy + 1) % 2;
			ainstr->setCanshuZhi(xx, yy);
			ainstr->WriteCanshu();

		} else if (jian == (0x00FB0001)) //线性，计算曲线常数
		{
			m_once = true;
			ainstr->setJiaozhun(3);
			ainstr->online(); //为了计算出曲线常数以便显示
		} else if (jian == (0x00FC0001)) //初始化
		{
			m_once = true;
			ainstr->MorenCanshu();
			//			NVIC_SystemReset();
		} else if (jian == 0x00FD0001) //保存出厂
			ainstr->save();
		else if (jian == 0x00ff00AA || jian == (200 + 0XAA)) { //返回
			m_once = true;
			m_dgus.ShowPicture(2, m_show, m_curItem);
		} else if (jian == (200 + 25)) { //线性，计算曲线常数
			m_once = true;
			m_popMenu.m_iWhat = 6;
			m_popMenu.m_fanHui = 6;
			m_dgus.ShowPicture(36, m_show, m_curItem);
		} else if (jian == (200 + 0XA0)) { //初始化
			m_once = true;
			m_popMenu.m_iWhat = 4;
			m_popMenu.m_fanHui = 6;
			m_dgus.ShowPicture(34, m_show, m_curItem);
		} else if (jian == (200 + 0XA1)) { //保存出厂
			m_once = true;
			m_popMenu.m_iWhat = 5;
			m_popMenu.m_fanHui = 6;
			m_dgus.ShowPicture(35, m_show, m_curItem);
		} else if (jian == (200 + 0)) { //NAME
			m_once = true;
			ainstr->setCanshuZhi(0, ((u8) ainstr->getCanshuZhi(0) + 1) % 12);
			ainstr->WriteCanshu();
		} else if (jian == (200 + 1)) { //DANWEI
			m_once = true;
			ainstr->setCanshuZhi(1, ((u8) ainstr->getCanshuZhi(1) + 1) % 3);
			ainstr->WriteCanshu();
		} else if (jian == (200 + 2)) { //XIAOSHU
			m_once = true;
			ainstr->setCanshuZhi(2, ((u8) ainstr->getCanshuZhi(2) + 1) % 4);
			ainstr->WriteCanshu();
		} else if (jian == (200 + 4)) { //YUNXUBIAOLING
			m_once = true;
			ainstr->setCanshuZhi(4, ((u8) ainstr->getCanshuZhi(4) + 1) % 2);
			ainstr->WriteCanshu();
		} else if (jian > 200 && jian < 240) { //修改参数
			m_once = true;
			ww = jian - 200;
			m_shuru.m_bFuShuru = false;
			if (ww == 3)
				m_shuru.m_dot = 2;
			else if (ww == 24)
				m_shuru.m_dot = 2;
			else if (ww == 22 || ww == 23) //补偿1,2
			{
				m_shuru.m_dot = 2;
				m_shuru.m_bFuShuru = true;
			} else //4个修正
			{
				m_shuru.m_dot = ainstr->getCanshuZhi(2);
				m_shuru.m_bFuShuru = true;
			}
			float ff = ainstr->getCanshuZhi(ww);
			ff *= pow(10, m_shuru.m_dot);
			m_shuru.m_data = ff;
			m_shuru.m_fanhuiHuamian = 6;
			m_shuru.m_guangbiao = 1;
			m_shuru.toShuzi();
			m_dgus.ShowPicture(41, m_show, m_curItem);
		} else if (jian == ENTER) {
			m_anjian.SendKey(200 + KEY6[m_curItem], m_curItem);
			m_curCaozuoxiang = KEY6[m_curItem];
		}
		break;
	case 7: ////////////////////////////////////////////////////////////////////////////////////////////按钮画面
		if (m_bFasongJiankong) {
			if (((Timer::ms5_jishu + Timer::MAXSHU - m_msJishu) % Timer::MAXSHU)
					> (300/5)) {
				m_dgus.SetJiankong(0x23);
				m_bFasongJiankong = false;
			}
		}
		if (m_once) {
			m_dgus.Show_KeyMove(m_show, m_curItem);
			m_once = false;
		}

		if (jian == ENTER) {
			m_dgus.SetJiankong(m_curItem);
			if (m_curItem == 12 || m_curItem == 14) {
				m_dgus.ShowPicture(2, m_show, m_curItem);
				m_once = true;
			}
		}
		break;
	case 10: //////////////////////////////////////////////////////////////////////////////////////////////曲线画面
		if (m_once) {
			m_once = false;
			m_dgus.ShowQuxianAnniu(m_history, ainstr);
			m_dgus.Show_KeyMove(m_show, m_curItem);
			//			m_dgus.SetDGUS_Bianliang(0x000B,(u16)(ainstr->getCanshuZhi(0)));//画面2中已设定过当前组份
			m_dgus.ShowL_H(ainstr);
		}
		if (m_bShuaxin) { //此处只刷新含量
			m_bShuaxin = false;
			m_dgus.ShowQuxianHanliang(ainstr);
		}
		if (m_bShuaxinQuxian) {
			m_bShuaxinQuxian = false;
			m_dgus.ShowQuxian(m_history, ainstr);
			m_bShuaxinQuxianYici = false;
		}
		if (m_bShuaxinQuxianYici) {
			m_bShuaxinQuxianYici = false;
			m_dgus.ShowQuxianYici(m_history, ainstr);
		}

		if (jian == 0x00ff0021 || jian == 0x00ff0022 || jian == 0x00ff0024) //更改曲线满屏时间//1-1小时 ；2-4小时 ；4-24小时
		{
			m_once = true;
			u16 xx = jian - 0x00ff0020;
			m_history->setPingmuShijian(xx);
			m_history->setXiangqianYe(0);
			//			m_history->setPR(m_history->getPW()); //要查看的曲线当前点等于写入的当前点
			m_history->setBAutoShuxin(false); //不自动刷新曲线
			m_bShuaxinQuxian = true;
			m_once = true;

		} else if (jian == 0x00ff0031 || jian == 0x00ff0032) //向前向后移动四分之一屏幕
		{
			int times = m_history->getPingmuShijian();
			if (times == 1)
				times = 4;
			else if (times == 4)
				times = 24;
			jian -= 0x00ff0031;
			if (jian == 1)
				m_history->setPR(m_history->getPR() + 120 * times);
			else
				m_history->setPR(m_history->getPR() - 120 * times);
			m_history->setBAutoShuxin(false); //不自动刷新曲线
			m_bShuaxinQuxian = true;
			m_once = true;

		} else if (jian == 0x00ff0011) //<<
		{
			u8 ye = m_history->getXiangqianYe();
			if (m_history->getPingmuShijian() == 4)
				m_history->setXiangqianYe(0);
			else if (m_history->getPingmuShijian() == 2) {
				ye = (ye + 1) % 6;
				m_history->setXiangqianYe(ye);
			} else if (m_history->getPingmuShijian() == 1) {
				ye = (ye + 1) % 24;
				m_history->setXiangqianYe(ye);
			}
			m_history->setBAutoShuxin(false); //不自动刷新曲线
			m_bShuaxinQuxian = true;
			m_once = true;
		} else if (jian == 0x00ff0012) //>>
		{
			u8 ye = m_history->getXiangqianYe();
			if (m_history->getPingmuShijian() == 4)
				m_history->setXiangqianYe(0);
			else if (m_history->getPingmuShijian() == 2) {
				ye = (ye + 5) % 6;
				m_history->setXiangqianYe(ye);
			} else if (m_history->getPingmuShijian() == 1) {
				ye = (ye + 23) % 24;
				m_history->setXiangqianYe(ye);
			}
			m_history->setBAutoShuxin(false); //不自动刷新曲线
			m_bShuaxinQuxian = true;
			m_once = true;

		} else if (jian == 0x00ff0013) //->|
		{
			m_history->setXiangqianYe(0);
			m_history->setBAutoShuxin(true); //自动刷新曲线
			m_history->setPR(m_history->getPW()); //要查看的曲线当前点等于写入的当前点
			m_bShuaxinQuxian = true;
			m_once = true;

		} else if (jian == 0x00ff0002) //返回主画面
			m_show = 0xff;
		else if (jian == 0x00ff0001) //返回2号画面
			m_dgus.ShowPicture(2, m_show, m_curItem);
		else if (jian == ENTER) {
			m_anjian.SendKey(0x00ff0000 + KEY10[m_curItem], m_curItem);
			m_bShuaxinQuxian = true;
		}
		break;
	case 12: ////////////////////////////////////////////////////////////////////////////////////////////////////电流调整画面
		if (m_once) {
			m_dgus.Show4_20(m4_20);
			ainstr->setCanshuZhi(39, m4_20);
			m_once = 0;
		}

		if (jian == 0x00ff00AA) //取消返回
		{
			m_once = true;
			ainstr->setCanshuZhi(38, 0); //取消调整电流
			m_dgus.ShowPicture(4, m_show, m_curItem);
		} else if (jian >= 0x001b0000 && jian < 0x001f0000) //调整
		{
			ww = jian >> 16;
			ww = u16(0x1e) - ww;
			int w2 = 1;
			for (int i = 0; i < ww; i++)
				w2 *= 10;
			int i = jian;
			ww = m4_20 / 10 / w2;
			ww *= 10;
			ww += i;
			ww *= w2;
			m4_20 = ww + m4_20 % w2;
			m_once = true;
		} else if (jian == 0x00ff0001) //确定
		{
			m_once = true;
			u8 i = ainstr->getCanshuZhi(38);
			if (i == 1) //调整的是4毫安
			{
				ainstr->setCanshuZhi(5, m4_20);
				ainstr->WriteCanshu();
			} else if (i == 2) //调整的是20毫安
			{
				ainstr->setCanshuZhi(6, m4_20);
				ainstr->WriteCanshu();
			}
			ainstr->setCanshuZhi(38, 0); //取消调整电流
			m_dgus.ShowPicture(4, m_show, m_curItem);
		}
		break;
	case 14: ////////////////////////////////////////////////////////////////////////////////////////////系统信息
		if (m_once) {
			m_dgus.ShowNO(ainstr1);
			m_once = 0;
		}
		if (m_bShuaxin) {
			m_bShuaxin = false;
			m_dgus.ShowInfo(ainstr1, 0X90);
			//			m_dgus.ShowInfo(ainstr2, 0X98);
			//			m_dgus.ShowInfo(ainstr3, 0X86);
		}

		if (jian == 0x00ff00AA || jian == ENTER) {
			m_show = 0xff;
		}
		break;

	case 31: ////////////////////////////////////////////////////////////////////////////////////////////弹出菜单
	case 32: ////////////////////////////////////////////////////////////////////////////////////////////弹出菜单
	case 33: ////////////////////////////////////////////////////////////////////////////////////////////弹出菜单
	case 34: ////////////////////////////////////////////////////////////////////////////////////////////弹出菜单
	case 35: ////////////////////////////////////////////////////////////////////////////////////////////弹出菜单
	case 36: ////////////////////////////////////////////////////////////////////////////////////////////弹出菜单
		if (m_once) {
			m_dgus.Show_KeyMove(m_show, m_curItem);
			m_once = 0;
		}
		if (jian == 0xf0) //按了取消按钮
		{
			m_once = true;
			m_dgus.ShowPicture(m_popMenu.m_fanHui, m_show, m_curItem);
		} else if (jian == 0xf1) //按了确定按钮
			PopMenuFanhui();
		else if (jian == ENTER)
			m_anjian.SendKey(KEY31[m_curItem], m_curItem);
		break;
	case 40: //
	case 41: //
		if (m_once) {
			m_dgus.Show_KeyMove(m_show, m_shuru.m_guangbiao);
			m_dgus.ShowInput(m_shuru);
			m_once = false;
			if (m_curCaozuoxiang == 5) {
				ainstr->setCanshuZhi(38, 1); //调整4毫安
				ainstr->setCanshuZhi(39, m_shuru.m_data); //38不为0的情况下，输出39的参数，为0正常输出4-20mA
			} else if (m_curCaozuoxiang == 6) {
				ainstr->setCanshuZhi(38, 2); //调整20毫安
				ainstr->setCanshuZhi(39, m_shuru.m_data); //38不为0的情况下，输出39的参数，为0正常输出4-20mA
			}
		}
		m_shuru.getKey(jian, m_once); //处理按键，先生成数组
		if (jian == ENTER) {
			InputFanhui();
			ainstr->setCanshuZhi(38, 0); //取消调整电流
		}
		break;

	default:
		m_once = true;
		m_dgus.ShowPicture(1, m_show, m_curItem);

		break;
	}

}
void Screen::PopMenuFanhui() {
	m_once = true;
	switch (m_popMenu.m_iWhat) {
	case 1:
		ainstr->setJiaozhun(1);
		break;
	case 2:
		ainstr->setJiaozhun(2);
		break;
	case 3:
		ainstr->load();
		NVIC_SystemReset();
		break;
	case 4:
		ainstr->MorenCanshu();
		NVIC_SystemReset();
		break;
	case 5:
		ainstr->save();
		break;
	case 6:
		ainstr->setJiaozhun(3);
		ainstr->online(); //为了计算出曲线常数以便显示
		break;
	}
	m_once = true;
	m_dgus.ShowPicture(m_popMenu.m_fanHui, m_show, m_curItem);
}
void Screen::InputFanhui() {

	m_once = true;
	m_dgus.ShowPicture(m_shuru.m_fanhuiHuamian, m_show, m_curItem);
	u32 uu = m_curCaozuoxiang;
	if (m_curCaozuoxiang > 9 && m_curCaozuoxiang < 22 && m_curCaozuoxiang != 17)
		uu = 0xf0;
	else {
		if (m_shuru.m_fanhuiHuamian == 4 || m_shuru.m_fanhuiHuamian == 6)
			uu += 0x100;
	}
	uu <<= 16;
	m_anjian.SendKey(uu | (u16) m_shuru.m_data, 1);
}
void Screen::run() { //每秒运行一次
	// TODO 自动生成的析构函数存根
	bSec2 = !bSec2;

	if (m_guoTimes >= GUOTIMES) {
		m_guoTimes = GUOTIMES;
		if (m_show != 1 && m_show != 10 && m_show != 14) {
			m_show = 0xff;
			m_dgus.SetJiankong(0x0E);
		}
	} else {
		m_guoTimes++;
	}
	if (m_show == 1 || m_show == 3 || m_show == 10 || m_show == 15
			|| m_show == 14)
		m_bShuaxin = true;

}

/*
 void Screen::InputFanhui() {
 if (m_shuru.m_fanhuiHuamian == 0x21) { //输入了校准密码
 if (m_shuru.m_data == 5555L) { //密码恢复初始值
 ainstr->setCanshuZhi(7, 1000);
 ainstr->setCanshuZhi(8, 2000);
 ainstr->WriteCanshu();
 m_dgus.ShowPicture(2, m_show, m_curItem);
 } else if (m_shuru.m_data == (u16) ainstr->getCanshuZhi(7)) {
 if (ainstr->getCanshuZhi(4)) //允许校零
 m_dgus.ShowPicture(3, m_show, m_curItem);
 else
 m_dgus.ShowPicture(15, m_show, m_curItem);
 } else
 m_dgus.ShowPicture(2, m_show, m_curItem);
 m_once = true;

 } else if (m_shuru.m_fanhuiHuamian == 0x22) { //输入了参数密码
 if (m_shuru.m_data == (u16) ainstr->getCanshuZhi(8)) {
 m_dgus.ShowPicture(4, m_show, m_curItem);
 } else if (m_shuru.m_data == NEIBUMIMA) {
 m_dgus.ShowPicture(6, m_show, m_curItem);
 } else
 m_dgus.ShowPicture(2, m_show, m_curItem);
 m_once = true;
 } else if (m_shuru.m_fanhuiHuamian == 0x31 || m_shuru.m_fanhuiHuamian == 0x32) { //输入了零点含量或者量程含量标气值
 float ff = m_shuru.m_data;
 ff /= pow(10, m_shuru.m_dot);
 ainstr->setCanshuZhi(10 + m_shuru.m_fanhuiHuamian - 0x31, ff);
 ainstr->WriteCanshu();
 m_dgus.ShowPicture(3, m_show, m_curItem);
 m_once = true;
 } else if (m_shuru.m_fanhuiHuamian > 100 && m_shuru.m_fanhuiHuamian < 140) //修改基本参数
 {
 u16 ww = m_shuru.m_fanhuiHuamian - 100;
 if (ww < 9) {
 m_shuru.m_dot = 0;
 ainstr->setCanshuZhi(38, 0); //取消调整电流 ww中已经包含了修改4还20毫安
 } else if (ww == 17)
 m_shuru.m_dot = 3;
 else
 m_shuru.m_dot = ainstr->getCanshuZhi(2);
 float ff = m_shuru.m_data;
 ff /= pow(10, m_shuru.m_dot);
 ainstr->setCanshuZhi(ww, ff);
 ainstr->WriteCanshu();
 m_dgus.ShowPicture(4, m_show, m_curItem);
 m_once = true;
 } else if (m_shuru.m_fanhuiHuamian > 200 && m_shuru.m_fanhuiHuamian < 240) { //修改内部参数
 u16 ww = m_shuru.m_fanhuiHuamian - 200;
 if (ww == 3) { //放大倍数
 m_shuru.m_dot = 0;
 if (m_shuru.m_data == 255) {
 ainstr->setCanshuZhi(9, ainstr->getCanshuZhi(23) * 100); //补偿2的值设为本机编号
 ainstr->setCanshuZhi(23, 0); //补偿2恢复为零
 ainstr->WriteCanshu();
 m_dgus.ShowPicture(6, m_show, m_curItem);
 m_once = true;
 return;
 } else {
 ainstr->setCanshuZhi(3, ww);
 if (!(ainstr->getDAC0832()).m_isNULL)
 ainstr->gengXinFangdaX();
 ainstr->WriteCanshu();
 return;
 }

 } else if (ww == 25 || ww == 22 || ww == 23)
 m_shuru.m_dot = 2;
 else
 m_shuru.m_dot = ainstr->getCanshuZhi(2);

 float ff = m_shuru.m_data;
 ff /= pow(10, m_shuru.m_dot);
 ainstr->setCanshuZhi(ww, ff);
 ainstr->WriteCanshu();
 m_dgus.ShowPicture(6, m_show, m_curItem);
 m_once = true;
 }
 }
 */
