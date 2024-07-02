/*
 * Screen.cpp
 *
 *  Created on: 2020��2��2��
 *      Author: Administrator
 */

#include "Anjian.h"
#include "Screen.h"
#include "weizhi.h"
#include "Timer.h"
#include <math.h>

Screen::Screen(Ainstrument *ainstr1, Ainstrument *ainstr2, Ainstrument *ainstr3,
		HistoryData *m_history1, HistoryData *m_history2, HistoryData *m_history3) {
	// TODO �Զ����ɵĹ��캯�����
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
	// TODO �Զ����ɵ������������
}
void Screen::show() {
	u16 ww = 0;
	m_anjian.PutKey(m_dgus.ReadDGUS());

	u32 jian = m_anjian.GetKey();
	if (jian != 0)
		m_guoTimes = 0;
	m_keymove.move(m_show, m_curItem, jian, m_once);
	//ֻ�������������ƶ�
	//������40״̬�°�������

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
			//m_curItem=3;//Ĭ��ѡ�� ϵͳ��Ϣ
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

	case 2: ////////////////////////////////////////////////////////////////////////////////////////////��ť����
		if (m_once) {
			m_dgus.Show_KeyMove(m_show, m_curItem);
			m_dgus.SetDGUS_Bianliang(0x000B, (u16) (ainstr->getCanshuZhi(0)));
			m_once = false;
		}

		if (jian == (0x00F00000 | (u16) ainstr->getCanshuZhi(7))) { //�ж�У׼����
			if (ainstr->getCanshuZhi(4)) //����У��
				m_dgus.ShowPicture(3, m_show, m_curItem);
			else
				m_dgus.ShowPicture(15, m_show, m_curItem);
			m_once = true;
		} else if (jian == (0x00F10000 | (u16) ainstr->getCanshuZhi(8))) { //�жϲ�������
			m_dgus.ShowPicture(4, m_show, m_curItem);
			m_once = true;
		} else if (jian == (0x00F10000 | NEIBUMIMA)) { ////�ڲ����� ����7788 init�г�ʼ��
			m_dgus.ShowPicture(6, m_show, m_curItem);
			m_once = true;
		} else if (jian == (0x00F00000 | 5555L)) { //����ָ���ʼֵ
			ainstr->setCanshuZhi(7, 1000);
			ainstr->setCanshuZhi(8, 2000);
			ainstr->WriteCanshu();
			m_show = 0xff;
		} else if (jian == 0x00FF0025) { //���������ͼ��ť
			m_history->setPingmuShijian(1); //1-1Сʱ ��2-4Сʱ ��4-24Сʱ
			m_history->setXiangqianYe(0);
			m_history->setBAutoShuxin(true); //�Զ�ˢ������
			m_history->setPR(m_history->getPW());
			m_dgus.ShowPicture(10, m_show, m_curItem);
			m_bShuaxinQuxian = true;
			m_once = true;
		} else if (jian == 0x00FF0026) { //����˷��ذ�ť
			m_show = 0xff;
		} else if (jian == 0x00FF0027) { //�����ϵͳ��Ϣ��ť
			m_dgus.ShowPicture(14, m_show, m_curItem);
			m_once = true;
		} else if (jian == 0x00FF00F0 || jian == 0x00FF00F1) { //���밴���������뻭��
			m_shuru.m_bFuShuru = false;
			m_shuru.m_data = 0;
			m_shuru.m_fanhuiHuamian = 2;
			m_shuru.m_guangbiao = 1;
			m_shuru.m_dot = 0;
			m_shuru.toShuzi();
			m_dgus.ShowPicture(40, m_show, m_curItem);
			m_once = true;

		} else if (jian == (0x00FF0023)) { //�����У׼ʱ�Ӱ�ť
			m_dgus.ShowPicture(7, m_show, m_curItem);
			m_bFasongJiankong = true;
			m_msJishu = Timer::ms5_jishu; //���������Ŀ������ʱ�ٷ��ͼ�������
			m_once = true;
		} else if (jian == ENTER) { //�����س����ͼ��루�Ա��´ν��հ�����
			m_anjian.SendKey(0X00FF0000 | KEY2[m_curItem], m_curItem);
			m_curCaozuoxiang = KEY2[m_curItem];
		}
		break;
	case 3: //////////////////////////////////////////////////////////////////////////////////////////////////////////У׼����
	case 15: //////////////////////////////////////////////////////////////////////////////////////////////////////////У׼����
		if (m_once) {
			m_once = false;
			m_dgus.Show_KeyMove(m_show, m_curItem);
			m_dgus.ShowJiaozhunYici(ainstr);
		}
		if (m_bShuaxin) {
			m_bShuaxin = false;
			m_dgus.ShowJiaozhunHanliang(ainstr);
		}

		if (jian == 0x00ff000A || jian == 0x00ff000B) //�������޸İ�ť
		{
			m_curCaozuoxiang = jian;
			m_dgus.SetJiankong((u8) ainstr->getCanshuZhi(2) + 0xf0);
		} else if ((jian & 0x00ff0000) == 0x00f00000)	//�޸��˺���
		{
			u16 zhi = jian;
			float ff = zhi / pow(10, ainstr->getCanshuZhi(2));
			ainstr->setCanshuZhi(m_curCaozuoxiang, ff);
			ainstr->WriteCanshu();
			m_dgus.SetDGUS_Bianliang(0x100 + m_curCaozuoxiang, zhi);
		} else if (jian == (0x00FE0001)) { //У׼���
			ainstr->setJiaozhun(1);
		} else if (jian == (0x00ff0001)) { //У׼����
			ainstr->setJiaozhun(2);
		} else if (jian == 0x00ff00AA || jian == 0xAA) {
			m_once = true;
			m_dgus.ShowPicture(2, m_show, m_curItem);
		} else if (jian == 0x33 || jian == 0x34) { //У׼��������
			m_once = true;
			m_popMenu.m_iWhat = jian - 0x32;
			m_popMenu.m_fanHui = 3;
			m_dgus.ShowPicture(31 + jian - 0x33, m_show, m_curItem);
		} else if (jian == 10 || jian == 11) { //ȥ�޸����������
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
	case 4: ///////////////////////////////////////////////////////////////////////////////////////////////////////������������
		if (m_once) {
			m_once = false;
			m_dgus.Show_KeyMove(m_show, m_curItem);
			m_dgus.ShowJiben(ainstr);
		}

		if ((jian >= 0x00ff000C && jian <= 0x00ff0010)) //�������޸İ�ť
		{
			m_curCaozuoxiang = jian;
			m_dgus.SetJiankong((u8) ainstr->getCanshuZhi(2) + 0xf0);
		} else if ((jian >> 16) == 0x0105 || (jian >> 16) == 0x0106	) {
			u16 ll = jian;
			jian >>= 16;
			m_dgus.SetDGUS_Bianliang(jian, ll);	//������Ļ
			jian-=0x100;
			ainstr->setCanshuZhi(jian, ll);
			ainstr->WriteCanshu();
		} else if ((jian >> 16) == 0x00f0)	//�޸���
		{
			short zhi = jian;
			float ff = zhi / pow(10, ainstr->getCanshuZhi(2));
			ainstr->setCanshuZhi(m_curCaozuoxiang, ff);
			ainstr->WriteCanshu();
			m_dgus.SetDGUS_Bianliang(0x100 + m_curCaozuoxiang, zhi);	//������Ļ
		} else if ((jian >> 16) == 0x0107 || (jian >> 16) == 0x0108
				|| (jian >> 16) == 0x0111) { //����+����ֵ
			u16 ll = jian;
			jian >>= 16;
			if (jian == 0X0111)
				ainstr->setCanshuZhi(jian - 0X100, short(ll) / 1000.0f);
			else
				ainstr->setCanshuZhi(jian - 0X100, ll); //7,8��ַΪ����1,2
			ainstr->WriteCanshu();
			m_dgus.SetDGUS_Bianliang(jian, ll);	//������Ļ
		} else if (jian == 0xfe0001) { //�ָ�����ֵ
			m_once = true;
			ainstr->load();
			NVIC_SystemReset();

		} else if (jian == 0x00ff0001) {
			m_once = true;
			m4_20 = ainstr->getCanshuZhi(5);
			ainstr->setCanshuZhi(38, 1); //����4����
			m_dgus.ShowPicture(12, m_show, m_curItem);
		} else if (jian == 0x00ff0002) {
			m_once = true;
			m4_20 = ainstr->getCanshuZhi(6);
			ainstr->setCanshuZhi(38, 2); //����20����
			m_dgus.ShowPicture(12, m_show, m_curItem);
		} else if (jian == 0x00ff00AA || jian == (100 + 0XAA)) { //����
			m_once = true;
			m_dgus.ShowPicture(2, m_show, m_curItem);
		} else if (jian == (100 + 0XA0)) { //�ָ�����
			m_once = true;
			m_popMenu.m_iWhat = 3 + jian - (100 + 0XA0);
			m_popMenu.m_fanHui = 4;
			m_dgus.ShowPicture(33, m_show, m_curItem);
		} else if (jian > 100 && jian < 140) //�޸Ĳ���
		{
			ww = jian - 100;
			m_shuru.m_bFuShuru = false;
			if (ww < 9)
				m_shuru.m_dot = 0; //�Ƿ����������41����������жϣ��˴������ж�
			else if (ww == 17) {
				m_shuru.m_dot = 3;
				m_shuru.m_bFuShuru = true;
			} else {
				m_shuru.m_dot = ainstr->getCanshuZhi(2);
				if (ww == 14) //��̬��������������
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
	case 6: //////////////////////////////////////////////////////////////////////////////////////////////�ڲ���������
		if (m_once) {
			m_once = false;
			m_dgus.Show_KeyMove(m_show, m_curItem);
			m_dgus.ShowNeibuCanshu(ainstr);
		}
		if ((jian >= 0x00ff0012 && jian <= 0x00ff0015)) //�������޸İ�ť
		{
			m_curCaozuoxiang = jian;
			m_dgus.SetJiankong((u8) ainstr->getCanshuZhi(2) + 0xf0);
		} else if ((jian >> 16) == 0x00f0)	//�޸���
		{
			short zhi = jian;
			float ff = zhi / pow(10, ainstr->getCanshuZhi(2));
			ainstr->setCanshuZhi(m_curCaozuoxiang, ff);
			ainstr->WriteCanshu();
			m_dgus.SetDGUS_Bianliang(0x100 + m_curCaozuoxiang, zhi);	//������Ļ
		} else if (((jian >> 16) >= 0x0116 && (jian >> 16) <= 0x0119)
				|| (jian >> 16) == 0x0103) { //���������£����ߣ���ԴƵ��
			u16 ll = jian;
			jian >>= 16;
			if (jian == 0X0103&&ll == 255) {
				u32 uu=ainstr->getCanshuZhi(9);
				uu%=10000;
				u32 u1=ainstr->getCanshuZhi(23) * 100+0.5;
				uu=u1*10000+uu;
				ainstr->setCanshuZhi(9,uu ); //����2��ֵ��Ϊ�������
				ainstr->setCanshuZhi(23, 0); //����2�ָ�Ϊ��
			}else if (jian == 0X0103&&ll == 254) {
				u32 uu=ainstr->getCanshuZhi(9);
				uu/=10000;
				u32 u1=ainstr->getCanshuZhi(23) * 100+0.5;
				uu=uu*10000+u1;
				ainstr->setCanshuZhi(9,uu ); //����2��ֵ��Ϊ�������
				ainstr->setCanshuZhi(23, 0); //����2�ָ�Ϊ��
			}else {
				ainstr->setCanshuZhi(jian - 0X100, short(ll) / 100.0f);
				m_dgus.SetDGUS_Bianliang(jian, ll);	//������Ļ
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

		} else if (jian == (0x00FB0001)) //���ԣ��������߳���
		{
			m_once = true;
			ainstr->setJiaozhun(3);
			ainstr->online(); //Ϊ�˼�������߳����Ա���ʾ
		} else if (jian == (0x00FC0001)) //��ʼ��
		{
			m_once = true;
			ainstr->MorenCanshu();
			//			NVIC_SystemReset();
		} else if (jian == 0x00FD0001) //�������
			ainstr->save();
		else if (jian == 0x00ff00AA || jian == (200 + 0XAA)) { //����
			m_once = true;
			m_dgus.ShowPicture(2, m_show, m_curItem);
		} else if (jian == (200 + 25)) { //���ԣ��������߳���
			m_once = true;
			m_popMenu.m_iWhat = 6;
			m_popMenu.m_fanHui = 6;
			m_dgus.ShowPicture(36, m_show, m_curItem);
		} else if (jian == (200 + 0XA0)) { //��ʼ��
			m_once = true;
			m_popMenu.m_iWhat = 4;
			m_popMenu.m_fanHui = 6;
			m_dgus.ShowPicture(34, m_show, m_curItem);
		} else if (jian == (200 + 0XA1)) { //�������
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
		} else if (jian > 200 && jian < 240) { //�޸Ĳ���
			m_once = true;
			ww = jian - 200;
			m_shuru.m_bFuShuru = false;
			if (ww == 3)
				m_shuru.m_dot = 2;
			else if (ww == 24)
				m_shuru.m_dot = 2;
			else if (ww == 22 || ww == 23) //����1,2
			{
				m_shuru.m_dot = 2;
				m_shuru.m_bFuShuru = true;
			} else //4������
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
	case 7: ////////////////////////////////////////////////////////////////////////////////////////////��ť����
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
	case 10: //////////////////////////////////////////////////////////////////////////////////////////////���߻���
		if (m_once) {
			m_once = false;
			m_dgus.ShowQuxianAnniu(m_history, ainstr);
			m_dgus.Show_KeyMove(m_show, m_curItem);
			//			m_dgus.SetDGUS_Bianliang(0x000B,(u16)(ainstr->getCanshuZhi(0)));//����2�����趨����ǰ���
			m_dgus.ShowL_H(ainstr);
		}
		if (m_bShuaxin) { //�˴�ֻˢ�º���
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

		if (jian == 0x00ff0021 || jian == 0x00ff0022 || jian == 0x00ff0024) //������������ʱ��//1-1Сʱ ��2-4Сʱ ��4-24Сʱ
		{
			m_once = true;
			u16 xx = jian - 0x00ff0020;
			m_history->setPingmuShijian(xx);
			m_history->setXiangqianYe(0);
			//			m_history->setPR(m_history->getPW()); //Ҫ�鿴�����ߵ�ǰ�����д��ĵ�ǰ��
			m_history->setBAutoShuxin(false); //���Զ�ˢ������
			m_bShuaxinQuxian = true;
			m_once = true;

		} else if (jian == 0x00ff0031 || jian == 0x00ff0032) //��ǰ����ƶ��ķ�֮һ��Ļ
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
			m_history->setBAutoShuxin(false); //���Զ�ˢ������
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
			m_history->setBAutoShuxin(false); //���Զ�ˢ������
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
			m_history->setBAutoShuxin(false); //���Զ�ˢ������
			m_bShuaxinQuxian = true;
			m_once = true;

		} else if (jian == 0x00ff0013) //->|
		{
			m_history->setXiangqianYe(0);
			m_history->setBAutoShuxin(true); //�Զ�ˢ������
			m_history->setPR(m_history->getPW()); //Ҫ�鿴�����ߵ�ǰ�����д��ĵ�ǰ��
			m_bShuaxinQuxian = true;
			m_once = true;

		} else if (jian == 0x00ff0002) //����������
			m_show = 0xff;
		else if (jian == 0x00ff0001) //����2�Ż���
			m_dgus.ShowPicture(2, m_show, m_curItem);
		else if (jian == ENTER) {
			m_anjian.SendKey(0x00ff0000 + KEY10[m_curItem], m_curItem);
			m_bShuaxinQuxian = true;
		}
		break;
	case 12: ////////////////////////////////////////////////////////////////////////////////////////////////////������������
		if (m_once) {
			m_dgus.Show4_20(m4_20);
			ainstr->setCanshuZhi(39, m4_20);
			m_once = 0;
		}

		if (jian == 0x00ff00AA) //ȡ������
		{
			m_once = true;
			ainstr->setCanshuZhi(38, 0); //ȡ����������
			m_dgus.ShowPicture(4, m_show, m_curItem);
		} else if (jian >= 0x001b0000 && jian < 0x001f0000) //����
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
		} else if (jian == 0x00ff0001) //ȷ��
		{
			m_once = true;
			u8 i = ainstr->getCanshuZhi(38);
			if (i == 1) //��������4����
			{
				ainstr->setCanshuZhi(5, m4_20);
				ainstr->WriteCanshu();
			} else if (i == 2) //��������20����
			{
				ainstr->setCanshuZhi(6, m4_20);
				ainstr->WriteCanshu();
			}
			ainstr->setCanshuZhi(38, 0); //ȡ����������
			m_dgus.ShowPicture(4, m_show, m_curItem);
		}
		break;
	case 14: ////////////////////////////////////////////////////////////////////////////////////////////ϵͳ��Ϣ
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

	case 31: ////////////////////////////////////////////////////////////////////////////////////////////�����˵�
	case 32: ////////////////////////////////////////////////////////////////////////////////////////////�����˵�
	case 33: ////////////////////////////////////////////////////////////////////////////////////////////�����˵�
	case 34: ////////////////////////////////////////////////////////////////////////////////////////////�����˵�
	case 35: ////////////////////////////////////////////////////////////////////////////////////////////�����˵�
	case 36: ////////////////////////////////////////////////////////////////////////////////////////////�����˵�
		if (m_once) {
			m_dgus.Show_KeyMove(m_show, m_curItem);
			m_once = 0;
		}
		if (jian == 0xf0) //����ȡ����ť
		{
			m_once = true;
			m_dgus.ShowPicture(m_popMenu.m_fanHui, m_show, m_curItem);
		} else if (jian == 0xf1) //����ȷ����ť
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
				ainstr->setCanshuZhi(38, 1); //����4����
				ainstr->setCanshuZhi(39, m_shuru.m_data); //38��Ϊ0������£����39�Ĳ�����Ϊ0�������4-20mA
			} else if (m_curCaozuoxiang == 6) {
				ainstr->setCanshuZhi(38, 2); //����20����
				ainstr->setCanshuZhi(39, m_shuru.m_data); //38��Ϊ0������£����39�Ĳ�����Ϊ0�������4-20mA
			}
		}
		m_shuru.getKey(jian, m_once); //������������������
		if (jian == ENTER) {
			InputFanhui();
			ainstr->setCanshuZhi(38, 0); //ȡ����������
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
		ainstr->online(); //Ϊ�˼�������߳����Ա���ʾ
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
void Screen::run() { //ÿ������һ��
	// TODO �Զ����ɵ������������
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
 if (m_shuru.m_fanhuiHuamian == 0x21) { //������У׼����
 if (m_shuru.m_data == 5555L) { //����ָ���ʼֵ
 ainstr->setCanshuZhi(7, 1000);
 ainstr->setCanshuZhi(8, 2000);
 ainstr->WriteCanshu();
 m_dgus.ShowPicture(2, m_show, m_curItem);
 } else if (m_shuru.m_data == (u16) ainstr->getCanshuZhi(7)) {
 if (ainstr->getCanshuZhi(4)) //����У��
 m_dgus.ShowPicture(3, m_show, m_curItem);
 else
 m_dgus.ShowPicture(15, m_show, m_curItem);
 } else
 m_dgus.ShowPicture(2, m_show, m_curItem);
 m_once = true;

 } else if (m_shuru.m_fanhuiHuamian == 0x22) { //�����˲�������
 if (m_shuru.m_data == (u16) ainstr->getCanshuZhi(8)) {
 m_dgus.ShowPicture(4, m_show, m_curItem);
 } else if (m_shuru.m_data == NEIBUMIMA) {
 m_dgus.ShowPicture(6, m_show, m_curItem);
 } else
 m_dgus.ShowPicture(2, m_show, m_curItem);
 m_once = true;
 } else if (m_shuru.m_fanhuiHuamian == 0x31 || m_shuru.m_fanhuiHuamian == 0x32) { //��������㺬���������̺�������ֵ
 float ff = m_shuru.m_data;
 ff /= pow(10, m_shuru.m_dot);
 ainstr->setCanshuZhi(10 + m_shuru.m_fanhuiHuamian - 0x31, ff);
 ainstr->WriteCanshu();
 m_dgus.ShowPicture(3, m_show, m_curItem);
 m_once = true;
 } else if (m_shuru.m_fanhuiHuamian > 100 && m_shuru.m_fanhuiHuamian < 140) //�޸Ļ�������
 {
 u16 ww = m_shuru.m_fanhuiHuamian - 100;
 if (ww < 9) {
 m_shuru.m_dot = 0;
 ainstr->setCanshuZhi(38, 0); //ȡ���������� ww���Ѿ��������޸�4��20����
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
 } else if (m_shuru.m_fanhuiHuamian > 200 && m_shuru.m_fanhuiHuamian < 240) { //�޸��ڲ�����
 u16 ww = m_shuru.m_fanhuiHuamian - 200;
 if (ww == 3) { //�Ŵ���
 m_shuru.m_dot = 0;
 if (m_shuru.m_data == 255) {
 ainstr->setCanshuZhi(9, ainstr->getCanshuZhi(23) * 100); //����2��ֵ��Ϊ�������
 ainstr->setCanshuZhi(23, 0); //����2�ָ�Ϊ��
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
