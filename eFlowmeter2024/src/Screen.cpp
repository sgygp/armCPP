/*
 * Screen.cpp
 *
 *  Created on: 2020年2月2日
 *      Author: Administrator
 */

#include "Screen.h"
#include "Timer.h"
#include "DIANZHEN.H"
#include "delay.h"
#include "string.h"
#include "BJ.h"
#include "II_C.h"
#include <math.h>
#include "Anjian.h"
#include "OutFreq.h"


Screen::Screen(Yiqi *ainstr1, RealQuxian *quxian) {
	// TODO 自动生成的构造函数存根
	this->ainstr = ainstr1;
	m_show = 0xff;
	m_once = true;
	m_bShuaxin = true;
	m_bShuaxinQuxian = true;
	m_guoTimes = 0;
	m4_20 = 0;
	bSec2 = false;
	m_quxian = quxian;
	m_jiaozhun = 0;
}

void Screen::init() {

	m_lcd.init();
	m_show = 0xff;
}

void Screen::LOGO() {
	m_lcd.Clear(0);
	m_lcd.WriteZi(0, 3, 16, 2, 8, GONGSI, false);

	Delay::delay_ms(3000);

}

Screen::~Screen() {
	// TODO 自动生成的析构函数存根
}
void Screen::show() {

	u32 jian = 0;
	DOT = 3-ainstr->m_yiqicanshu.m_jisuanDanwei%4;
	u8 i = 0;
	static u8 item=0;

	switch (m_show) {
	case 1:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			IInt();
			m_bShuaxinQuxian = true;
		}
		if (m_bShuaxin) {
			m_bShuaxin = false;
			ShowMain(item);
		}
		if (m_bShuaxinQuxian) {
			ShowQuxian();
			m_bShuaxinQuxian = false;
		}
		jian = Anjian::GetKey();
		if (jian == ENTER) {
			if (m_jiaozhun > 0) {
				ainstr->setJiaozhun(m_jiaozhun);
				m_jiaozhun = 0;
			} else {
				m_show = 2;
				m_once = true;
				m_curline = 0;
			}
		} else if (jian != 0) {
			ainstr->m_bXY = true;
			m_jiaozhun = 0;
			if (jian == UP){
				m_bShuaxin=true;
				item=(item+4)%5;
			}
			else if(jian == DOWN){
				m_bShuaxin=true;
				item=(item+1)%5;
			}

		}
		break;
	case 2:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			m_lcd.WriteHZx(32, 0, 2, JIBEN, m_curline == 1); //基本参数
			m_lcd.WriteHZx(32 + 16 * 2, 0, 2, CANSHU, m_curline == 1); //基本参数

			m_lcd.WriteHZx(32, 2, 2, ERJI, m_curline == 2); //二级参数
			m_lcd.WriteHZx(32 + 16 * 2, 2, 2, CANSHU, m_curline == 2); //二级参数

			m_lcd.WriteHZx(32, 4, 4, LEIJI, m_curline == 3); //累计清零

			m_lcd.WriteHZx(32, 6, 4, XITONG, m_curline == 4); //系统参数

		}
		jian = Anjian::GetKey();
		if (jian & UP) //向上
		{
			m_curline = (m_curline + 4) % 5;
			m_once = true;
		} else if (jian & DOWN) //向下
		{
			m_curline = (m_curline + 1) % 5;
			m_once = true;
		} else if (jian & ENTER) //MENU按钮
		{
			if (!m_curline) {
				m_show = 0xff;
			} else {
				m_once = true;
				if (m_curline == 4) {
					m_show = 190;
				} else {
					m_show = 3;
					m_gai = 3;
					m_shuju = 0;
					m_fu = false;
					m_lcd.Fill(1, 16, 2, 112, 0XF0);
					m_lcd.Fill(2, 16, 6, 112, 0XFF);
					m_lcd.Fill(6, 16, 7, 112, 0X0F);
					m_lcd.WriteHZx(48, 2, 2, MIMA, 1); //
				}
			}
		}
		break;
	case 3:
		jian = ShowItem(&m_shuju, 0, 0);
		if (jian & ENTER) {
			u16 ww = m_shuju;
			m_once = true;
			if (m_curline == 1 && ww == Yiqi::m_syscanshu.m_mima[0]) //m_mima[0]
			{
				m_curline = 0;
				m_show = 10;
			} else if (m_curline == 2 && ww == Yiqi::m_syscanshu.m_mima[1]) //m_mima[1]
			{
				m_curline = 0;
				m_show = 100;
			} else if (m_curline == 2 && ww == XUZONG_NO) //m_mima[1]
			{
				m_curline = 0;
				m_show = 200;
			} else if (m_curline == 3 && ww == Yiqi::m_syscanshu.m_leijiqinglingMima) //m_mima[1]
			{
				inQueren();
			} else if (m_curline == 2 && ww == 3000) {
				m_curline = 0;
				m_show = 200;
			}
			//			else  if(m_curline==3&&ww==m_password)
			//			{
			//				m_curline=0;
			//				m_show=170;
			//				ff=m_qixian/86400;
			//			}
			else if (m_curline == 1 && ww == 5555) {
				ainstr->m_syscanshu.m_mima[0]= MIMA1;
				ainstr->m_syscanshu.m_mima[1]= MIMA2;
				ainstr->WriteCanshu();
				m_show = 0xff;
			} else
				m_show = 2;

		}
		break;
	case 6:
		if (m_once) {
			m_lcd.WriteHZx(56 - 16, 4, 1, FOUSHI, m_shifou);
			m_lcd.WriteHZx(56 - 8 + 32, 4, 1, FOUSHI + 32, !m_shifou);
		}

		jian = Anjian::GetKey();
		if (jian & ENTER) {
			if (m_shifou) {
				ainstr->m_leijiCanshu.m_leijiZheng = 0;
				ainstr->m_leijiCanshu.m_leijiFu = 0;
				ainstr->WriteLeiji();
			}
			m_curline = 3;
			m_show = 2;
			m_once = true;

		} else if ((jian & RIGHT) || (jian & LEFT)) {
			m_shifou = !m_shifou;
			m_once = true;
		}
		break;
	case 10:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			i = 0;
			if (Yiqi::m_syscanshu.m_yuyan)
				m_lcd.WriteHZx(i,			2 * 0, 2, YUYAN, 					m_curline == 1); //语言
			else
				m_lcd.WriteString(i, 			2 * 0,	 (u8*) "Language", 		m_curline == 1); //语言

			m_lcd.WriteHZx(i,				2 * 1, 2, KOUJINGDIANZHEN,	m_curline == 2); //口径

			m_lcd.WriteHZx(i,				2 * 2, 4, FANGXIANG,			m_curline == 3); //方向选择

			m_lcd.WriteHZx(i,				2 * 3, 1, ZHENGFAN+32*1,	m_curline == 4); //反
			m_lcd.WriteHZx(i+16*1,		2 * 3, 1, FANGXIANG+32*1,	m_curline == 4); //向
			m_lcd.WriteHZx(i+16*2,		2 * 3, 2, YUNXU,					m_curline == 4); //允许



			if (Yiqi::m_syscanshu.m_yuyan)
				m_lcd.WriteString(128 - 56, 0, (u8*) "English", false);
			else
				m_lcd.WriteHZx(96, 2 * 0, 2, ZHONGWEN, false);
			m_lcd.WriteString(128-8*6, 		2 * 1,(u8*)KOUJINGDN[ainstr->m_yiqicanshu.m_koujing], 		false); //DN口径

			i=ainstr->m_yiqicanshu.m_fangxiang;
			m_lcd.WriteHZx(128-16*2, 		2 * 2, 2, ZHENGFANXIANG+i*32*2, 		false); //正向/反向

			i=ainstr->m_yiqicanshu.m_fuyunxu;
			m_lcd.WriteHZx(128-16*2, 		2 * 3, 2, JINYUNXU+i*32*2, 		false); //禁止/允许

		}
		jian = Anjian::GetKey();
		if(jian==RETURN)
		{
			m_curline = 1;
			m_show = 2;
			m_once = true;
		}else
			if (jian & UP) //向上
			{
				if (m_curline) {
					m_curline = (m_curline + 4) % 5;
					m_once = true;
				}
			} else if (jian & DOWN) //向下
			{
				if (m_curline == 4) {
					m_curline = 1;
					m_show = 20;
				} else
					m_curline = (m_curline + 1) % 5;
				m_once = true;
			} else if (jian & ENTER) //MENU按钮
			{
				switch (m_curline) {
				case 0:
					m_curline = 1;
					m_show = 2;
					m_once = true;
					break;
				case 1:
					m_once = true;
					ainstr->m_syscanshu.m_yuyan=!ainstr->m_syscanshu.m_yuyan;
					ainstr->WriteCanshu();
					break;
				case 2:
					m_once = true;
					ainstr->m_yiqicanshu.m_koujing=(ainstr->m_yiqicanshu.m_koujing+1)%36;
					ainstr->WriteCanshu();
					break;
				case 3:
					m_once = true;
					ainstr->m_yiqicanshu.m_fangxiang=!ainstr->m_yiqicanshu.m_fangxiang;
					ainstr->canshu2Hardware();
					ainstr->WriteCanshu();
					break;
				case 4:
					m_once = true;
					ainstr->m_yiqicanshu.m_fuyunxu=!ainstr->m_yiqicanshu.m_fuyunxu;
					ainstr->WriteCanshu();
					break;
				default:
					break;
				}
			}
		break;
	case 20:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			i = 0;
			m_lcd.WriteHZx(i,				2 * 0, 2, LINGDIAN, 		m_curline == 1); //零点
			m_lcd.WriteHZx(i+16*2, 	2 * 0, 2, TIAOZHENG, 	m_curline == 1); //调整

			m_lcd.WriteHZx(i,				2 * 1, 2, LIULIANG, 		m_curline == 2); //流量
			m_lcd.WriteHZx(i+16*2, 	2 * 1, 2, DANWEI, 			m_curline == 2); //单位

			m_lcd.WriteHZx(i,				2 * 2, 2, JISUAN_, 		m_curline == 3); //积算
			m_lcd.WriteHZx(i+16*2, 	2 * 2, 2, DANWEI, 			m_curline == 3); //单位

			m_lcd.WriteHZx(i,				2 * 3, 2, LIANGCHENG, 	m_curline == 4); //量程
			m_lcd.WriteHZx(i+16*2,		2 * 3, 2, SHEZHI, 			m_curline == 4); //设置

			m_lcd.WriteShuju(88, 				2 * 0, ainstr->m_yiqicanshu.m_lingdianAD, 0);
			i=ainstr->m_yiqicanshu.m_danwei;
			m_lcd.WriteString(128-8*4, 		2 * 1,(u8*)DANWEISHUZU[i], 		false); //L/s
			i=ainstr->m_yiqicanshu.m_jisuanDanwei;
			m_lcd.WriteString(128-8*6, 		2 * 2,(u8*)JISUANDANWEI[i], 		false); //L/s
			m_lcd.WriteShuju(88, 				2 * 3, ainstr->m_yiqicanshu.m_outH, 0);

		}
		jian = Anjian::GetKey();
		if(jian==RETURN)
		{
			m_curline = 1;
			m_show = 2;
			m_once = true;
		}else
			if (jian & UP) //向上
			{
				m_once = true;
				m_curline = (m_curline + 4) % 5;
				if(!m_curline)
				{
					m_curline=4;
					m_show=10;
				}
			}else if (jian & DOWN) //向下
			{
				if (m_curline == 4) {
					m_curline = 1;
					m_show = 30;
				} else
					m_curline = (m_curline + 1) % 5;
				m_once = true;
			} else if (jian & ENTER) //MENU按钮
			{
				switch (m_curline) {
				case 0:
					m_curline = 1;
					m_show = 2;
					m_once = true;
					break;
				case 1:
					inQueren();
					break;
				case 2:
					m_once = true;
					ainstr->m_yiqicanshu.m_danwei=(ainstr->m_yiqicanshu.m_danwei+1)%8;
					ainstr->WriteCanshu();
					break;
				case 3:
					m_once = true;
					ainstr->m_yiqicanshu.m_jisuanDanwei=(ainstr->m_yiqicanshu.m_jisuanDanwei+1)%8;
					ainstr->WriteCanshu();
					break;
				case 4:
					m_shuju = ainstr->m_yiqicanshu.m_outH;
					Input();
					break;
				default:
					break;
				}
			}
		break;
	case 21:
		if (m_once) {
			m_lcd.WriteHZx(56 - 16, 4, 1, FOUSHI, m_shifou);
			m_lcd.WriteHZx(56 - 8 + 32, 4, 1, FOUSHI + 32, !m_shifou);
		}

		jian = Anjian::GetKey();
		if (jian & ENTER) {
			if (m_shifou) {
				ainstr->m_yiqicanshu.m_lingdianAD=fabs(ainstr->m_AD[2]-ainstr->m_AD[3]);
				ainstr->WriteCanshu();
			}
			m_curline = 1;
			m_show = 20;
			m_once = true;

		} else if ((jian & RIGHT) || (jian & LEFT)) {
			m_shifou = !m_shifou;
			m_once = true;
		}
		break;
	case 24:
		jian = ShowItem(&m_shuju, DOT, 0);
		if (jian & ENTER) {
			ainstr->m_yiqicanshu.m_outH=m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;
	case 30:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			i = 0;
			m_lcd.WriteHZx(i,				2 * 0, 3, QIEXIAOXINHAO+32,	m_curline == 1); //小信号
			m_lcd.WriteHZx(i+16*3, 	2 * 0, 2, XIANSHI, 						m_curline == 1); //显示

			m_lcd.WriteHZx(i,				2 * 1,4, QIEXIAOXINHAO,			m_curline == 2); //切小信号
			m_lcd.WriteString(i+16*4, 	2 * 1,			(u8*) "%", 			m_curline == 2); //

			m_lcd.WriteHZx(i, 				2 * 2, 2, GAOXIAN, 					m_curline == 3);//高限
			m_lcd.WriteHZx(i+16*2, 	2 * 2, 2, BAOJING, 					m_curline == 3);//报警

			m_lcd.WriteHZx(i, 				2 * 3, 2, DIXIAN, 						m_curline == 4);//低限
			m_lcd.WriteHZx(i+16*2, 	2 * 3, 2, BAOJING, 					m_curline == 4);//报警


			i=ainstr->m_yiqicanshu.m_bXiaoQiechu;
			m_lcd.WriteHZx(128-16*2, 			2 * 0, 2, JINYUNXU+i*32*2, 		false); //禁止/允许
			m_lcd.WriteShuju(88, 				2 * 1, ainstr->m_yiqicanshu.m_xiaoxinhaoqiechuDian, 2);

			if(ainstr->m_yiqicanshu.m_baojingH<0.001)
				m_lcd.WriteHZx(128-16*1, 			2 * 2, 1, GUANBI, 		false); //禁止/允许
			else
				m_lcd.WriteShuju(88, 				2 * 2, ainstr->m_yiqicanshu.m_baojingH, DOT);
			if(ainstr->m_yiqicanshu.m_baojingL<0.001)
				m_lcd.WriteHZx(128-16*1, 			2 * 3, 1, GUANBI, 		false); //禁止/允许
			else
				m_lcd.WriteShuju(88, 				2 * 3, ainstr->m_yiqicanshu.m_baojingL, DOT);
		}
		jian = Anjian::GetKey();
		if(jian==RETURN)
		{
			m_curline = 1;
			m_show = 2;
			m_once = true;
		}else
			if (jian & UP) //向上
			{
				m_once = true;
				m_curline = (m_curline + 4) % 5;
				if(!m_curline)
				{
					m_curline=4;
					m_show=20;
				}
			}else if (jian & DOWN) //向下
			{
				if (m_curline == 4) {
					m_curline = 1;
					m_show = 40;
				} else
					m_curline = (m_curline + 1) % 5;
				m_once = true;
			} else if (jian & ENTER) //MENU按钮
			{
				switch (m_curline) {
				case 0:
					m_curline = 1;
					m_show = 2;
					m_once = true;
					break;
				case 1:
					m_once = true;
					ainstr->m_yiqicanshu.m_bXiaoQiechu=!ainstr->m_yiqicanshu.m_bXiaoQiechu;
					ainstr->WriteCanshu();
					break;
				case 2:
					m_shuju = ainstr->m_yiqicanshu.m_xiaoxinhaoqiechuDian;
					Input();
					break;
				case 3:
					m_shuju = ainstr->m_yiqicanshu.m_baojingH;
					Input();
					break;
				case 4:
					m_shuju = ainstr->m_yiqicanshu.m_baojingL;
					Input();
					break;
				default:
					break;
				}

			}
		break;
	case 32:
		jian = ShowItem(&m_shuju, 2, 0);
		if (jian & ENTER) {
			ainstr->m_yiqicanshu.m_xiaoxinhaoqiechuDian=m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;
	case 33:
		jian = ShowItem(&m_shuju, DOT, 0);
		if (jian & ENTER) {
			ainstr->m_yiqicanshu.m_baojingH=m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;
	case 34:
		jian = ShowItem(&m_shuju, DOT, 0);
		if (jian & ENTER) {
			ainstr->m_yiqicanshu.m_baojingL=m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;
	case 40:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			i = 0;
			m_lcd.WriteHZx(i,				2 * 0, 2, TIAOZHENG, 	m_curline == 1); //调整
			m_lcd.WriteHZx(i+16*2,		2 * 0, 1, DIANYA, 			m_curline == 1); //电
			m_lcd.WriteHZx(i+16*3, 	2 * 0, 1, LIULIANG, 		m_curline == 1); //流
			m_lcd.WriteHZx(i+16*4, 	2 * 0, 1, LINGDIAN, 		m_curline == 1); //零

			m_lcd.WriteHZx(i,				2 * 1, 2, TIAOZHENG, 	m_curline == 2); //调整
			m_lcd.WriteHZx(i+16*2,		2 * 1, 1, DIANYA, 			m_curline == 2); //电
			m_lcd.WriteHZx(i+16*3, 	2 * 1, 1, LIULIANG, 		m_curline == 2); //流
			m_lcd.WriteHZx(i+16*4, 	2 * 1, 1, MAN, 				m_curline == 2); //满

			m_lcd.WriteHZx(i, 				2 * 2, 2, TONGXUN, 		m_curline == 3);//通讯
			m_lcd.WriteHZx(i+16*2, 	2 * 2, 2, DIZHI, 				m_curline == 3);//地址

			m_lcd.WriteHZx(i, 				2 * 3, 2, TONGXUN, 		m_curline == 4);//通讯
			m_lcd.WriteHZx(i+16*2, 	2 * 3, 3, BOTELV, 				m_curline == 4);//波特率

			m_lcd.WriteShuju(88, 				2 * 0, ainstr->m_yiqicanshu.m_4, 0);
			m_lcd.WriteShuju(88, 				2 * 1, ainstr->m_yiqicanshu.m_20, 0);
			m_lcd.WriteShuju(88, 2 * 2, Yiqi::m_syscanshu.m_addr, 0);
			switch (Yiqi::m_syscanshu.m_botelv) {
			case 0:
				m_lcd.WriteString(128 - 32, 2 * 3, (u8*) "4800", false);
				break;
			case 1:
				m_lcd.WriteString(128 - 32, 2 * 3, (u8*) "9600", false);
				break;
			case 2:
				m_lcd.WriteString(128 - 40, 2 * 3, (u8*) "19200", false);
				break;
			case 3:
				m_lcd.WriteString(128 - 40, 2 * 3, (u8*) "57600", false);
				break;
			case 4:
				m_lcd.WriteString(128 - 48, 2 * 3, (u8*) "115200", false);
				break;
			}
		}
		jian = Anjian::GetKey();
		if(jian==RETURN)
		{
			m_curline = 1;
			m_show = 2;
			m_once = true;
		}else
			if (jian & UP) //向上
			{
				m_once = true;
				m_curline = (m_curline + 4) % 5;
				if(!m_curline)
				{
					m_curline=4;
					m_show=30;
				}
			}else if (jian & DOWN) //向下
			{
				if (m_curline ) {
					m_curline = (m_curline + 1) % 5;
					m_once = true;
				}
			} else if (jian & ENTER) //MENU按钮
			{
				switch (m_curline) {
				case 0:
					m_curline = 1;
					m_show = 2;
					m_once = true;
					break;
				case 1:
					m_shuju = ainstr->m_yiqicanshu.m_4;
					ainstr->m_bMA=1;//进入校准4mA
					ainstr->m4_20= m_shuju;
					Input();
					break;
				case 2:
					m_shuju = ainstr->m_yiqicanshu.m_20;
					ainstr->m_bMA=2;//进入校准4mA
					ainstr->m4_20= m_shuju;
					Input();
					break;
				case 3:
					m_shuju = ainstr->m_syscanshu.m_addr;
					Input();
					break;
				case 4:
					m_once = true;
					ainstr->m_syscanshu.m_botelv=(ainstr->m_syscanshu.m_botelv+1)%5;
					ainstr->WriteCanshu();
					break;
				default:
					break;
				}


			}
		break;
	case 41:
		jian = ShowItem(&m_shuju, 0, 0);
		if (jian ) {
			ainstr->m4_20=m_shuju;
			if( jian==ENTER)
			{
				ainstr->m_bMA= 0;//取消校准4-20mA
				ainstr->m_yiqicanshu.m_4=m_shuju;
				ainstr->WriteCanshu();
				m_show = fanhuiHuamian;
				m_curline = fanhuiXiang;
			}
		}
		break;
	case 42:
		jian = ShowItem(&m_shuju, 0, 0);
		if (jian ) {
			ainstr->m4_20=m_shuju;
			if( jian==ENTER)
			{
				ainstr->m_bMA= 0;//取消校准4-20mA
				ainstr->m_yiqicanshu.m_20=m_shuju;
				ainstr->WriteCanshu();
				m_show = fanhuiHuamian;
				m_curline = fanhuiXiang;
			}
		}
		break;
	case 43:
		jian = ShowItem(&m_shuju, 0, 0);
		if (jian & ENTER) {
			ainstr->m_syscanshu.m_addr=m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;
	case 100:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			i = 0;
			m_lcd.WriteHZx(i,				2 * 0, 2, LICI, 				m_curline == 1); //励磁
			m_lcd.WriteHZx(i+16*2, 	2 * 0, 2, BAOJING, 		m_curline == 1); //报警

			m_lcd.WriteHZx(i,				2 * 1, 2, KONGGUAN, 	m_curline == 2); //空管
			m_lcd.WriteHZx(i+16*2, 	2 * 1, 2, BAOJING, 		m_curline == 2); //报警

			m_lcd.WriteHZx(i,				2 * 2, 2, LICI, 				m_curline == 3); //励磁
			m_lcd.WriteHZx(i+16*2, 	2 * 2, 2, MOSHI, 			m_curline == 3); //模式

			m_lcd.WriteHZx(i,				2 * 3, 2, MAICHONG, 		m_curline == 4); //脉冲
			m_lcd.WriteHZx(i+16*2, 	2 * 3, 2, MOSHI, 			m_curline == 4); //模式

			i=1;
			m_lcd.WriteHZx(128-16*2, 			2 * 0, 2, JINYUNXU+i*32*2, 		false); //禁止/允许
			i=0;
			m_lcd.WriteHZx(128-16*2, 			2 * 1, 2, JINYUNXU+i*32*2, 		false); //禁止/允许
			i=ainstr->m_yiqicanshu.m_licidianya;
			u8 a[1];
			a[0]='0'+i;
			m_lcd.WriteString(128-16*1, 	2 * 2,			a, 	1,		false); //
			i=ainstr->m_yiqicanshu.m_maichongshuchufangshi;
			m_lcd.WriteHZx(128-16*2, 			2 * 3, 2, MAICHONG+i*32*2, 		false); //禁止/允许
		}
		jian = Anjian::GetKey();
		if(jian==RETURN)
		{
			m_curline = 1;
			m_show = 2;
			m_once = true;
		}else
			if (jian & UP) //向上
			{
				if (m_curline) {
					m_curline = (m_curline + 4) % 5;
					m_once = true;
				}
			} else if (jian & DOWN) //向下
			{
				if (m_curline == 4) {
					m_curline = 1;
					m_show = 110;
				} else
					m_curline = (m_curline + 1) % 5;
				m_once = true;
			} else if (jian & ENTER) //MENU按钮
			{
				switch (m_curline) {
				case 0:
					m_curline = 1;
					m_show = 2;
					m_once = true;
					break;
				case 3:
					m_once = true;
					ainstr->m_yiqicanshu.m_licidianya=(ainstr->m_yiqicanshu.m_licidianya+1)%4;
					ainstr->m_yiqicanshu.m_pinlv=5;
					ainstr->canshu2Hardware();
					ainstr->WriteCanshu();
					break;
				case 4:
					m_once = true;
					ainstr->m_yiqicanshu.m_maichongshuchufangshi=!ainstr->m_yiqicanshu.m_maichongshuchufangshi;
					if(ainstr->m_yiqicanshu.m_maichongshuchufangshi)
						OutFreq::initPinlv();
					else
						OutFreq::initMaichong();

					ainstr->WriteCanshu();
					break;
				default:
					break;
				}
			}
		break;
	case 110:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			i = 0;
			m_lcd.WriteHZx(i,				2 * 0, 2, MAICHONG, m_curline == 1); //脉冲
			m_lcd.WriteHZx(i+16*2, 	2 * 0, 2, DANGLIANG,m_curline == 1); //当量

			m_lcd.WriteHZx(i,				2 * 1, 2, MAICHONG+32*2, 		m_curline == 2); //频率
			m_lcd.WriteHZx(i+16*2, 	2 * 1, 2, FANWEI, 		m_curline == 2); //范围

			m_lcd.WriteHZx(i,				2 * 2, 4, LIUTIMIDU, 	m_curline == 3); //流体密度

			m_lcd.WriteHZx(i,				2 * 3, 1, LEIJI+32*2, m_curline == 4); //清
			m_lcd.WriteHZx(i+16*1, 	2 * 3, 2, LEIJI, 			m_curline == 4); //累计
			m_lcd.WriteHZx(i+16*3, 	2 * 3, 2, MIMA, 			m_curline == 4); //密码

			i=4+ainstr->m_yiqicanshu.m_maichongdangliang;
			m_lcd.WriteString(128-8*6, 		2 * 0,(u8*)JISUANDANWEI[i], 		false); //
			m_lcd.WriteShuju(88, 				2 * 1, ainstr->m_yiqicanshu.m_pinlvfanwei, 0);
			m_lcd.WriteShuju(88, 				2 * 2, ainstr->m_yiqicanshu.m_midu, 3);
			m_lcd.WriteShuju(88, 				2 * 3, ainstr->m_syscanshu.m_leijiqinglingMima, 0);

		}
		jian = Anjian::GetKey();
		if(jian==RETURN)
		{
			m_curline = 1;
			m_show = 2;
			m_once = true;
		}else
			if (jian & UP) //向上
			{
				m_once = true;
				m_curline = (m_curline + 4) % 5;
				if(!m_curline)
				{
					m_curline=4;
					m_show=100;
				}
			}else if (jian & DOWN) //向下
			{
				if (m_curline == 4) {
					m_curline = 1;
					m_show = 120;
				} else
					m_curline = (m_curline + 1) % 5;
				m_once = true;
			} else if (jian & ENTER) //MENU按钮
			{
				switch (m_curline) {
				case 0:
					m_curline = 1;
					m_show = 2;
					m_once = true;
					break;
				case 1:
					m_once = true;
					ainstr->m_yiqicanshu.m_maichongdangliang=(ainstr->m_yiqicanshu.m_maichongdangliang+1)%4;
					ainstr->WriteCanshu();
					break;
				case 2:
					m_shuju = ainstr->m_yiqicanshu.m_pinlvfanwei;
					Input();
					break;
				case 3:
					m_shuju = ainstr->m_yiqicanshu.m_midu;
					Input();
					break;
				case 4:
					m_shuju = ainstr->m_syscanshu.m_leijiqinglingMima;
					Input();
					break;
				default:
					break;
				}

			}
		break;
	case 112:
		jian = ShowItem(&m_shuju, 0, 0);
		if (jian & ENTER) {
			ainstr->m_yiqicanshu.m_pinlvfanwei=m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;
	case 113:
		jian = ShowItem(&m_shuju, 3, 0);
		if (jian & ENTER) {
			ainstr->m_yiqicanshu.m_midu=m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;
	case 114:
		jian = ShowItem(&m_shuju, 0, 0);
		if (jian & ENTER) {
			ainstr->m_syscanshu.m_leijiqinglingMima=m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;

	case 120:
	{
		static u8 item1=0;
		static long leiji1=0,leiji2=0;
		static bool m_once2=true;
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			i = 0	;//20;
			m_lcd.WriteHZx(i,				2 * 0, 1, ZHENGFAN,		m_curline == 1); //正
			m_lcd.WriteHZx(i+16*1, 	2 * 0, 2, LEIJI,				m_curline == 1); //累计
			m_lcd.WriteHZx(i+16*3, 	2 * 0, 2, SHEZHI,			m_curline == 1); //设置

			m_lcd.WriteHZx(i,				2 * 2, 1, FU, 					m_curline == 3); //负
			m_lcd.WriteHZx(i+16*1, 	2 * 2, 2, LEIJI,				m_curline == 3); //累计
			m_lcd.WriteHZx(i+16*3, 	2 * 2, 2, SHEZHI,			m_curline == 3); //设置


			i=ainstr->m_yiqicanshu.m_jisuanDanwei;
			m_lcd.WriteString(88+8, 	2 * 1,	(u8*)DANWEISHUZU[i],2, 			false); //
			m_lcd.WriteString(88+8, 	2 * 3,	(u8*)DANWEISHUZU[i],2, 			false); //
			leiji1=ainstr->m_yiqicanshu.m_zhengleijiTiaozheng;
			leiji2=ainstr->m_yiqicanshu.m_fuleijiTiaozheng;
			m_once2=true;
		}
		if (m_once2) {
			m_once2=false;
			ShowGaiLeiji(0,item1,&leiji1,m_curline == 2);
			ShowGaiLeiji(1,item1,&leiji2,m_curline == 4);

		}
		jian = Anjian::GetKey();
		if(jian==RETURN)
		{
			m_curline = 1;
			m_show = 2;
			m_once = true;
		}else
			if (jian & UP) //向上
			{
				if(m_curline==1)
				{
					m_curline=4;
					m_show=110;
					m_once = true;
				}else if(m_curline==3)
				{
					m_curline =1;
					m_once = true;
				}
				else if(m_curline==2)
				{
					GaiLeiji(item1,&leiji1,true);
					m_once2=true;
				}
				else if(m_curline==4)
				{
					GaiLeiji(item1,&leiji2,true);
					m_once2=true;
				}

			}else if (jian & DOWN) //向下
			{
				if (m_curline == 3) {
					m_curline = 1;
					m_show = 130;
					m_once = true;
				} else if (m_curline == 1)
				{
					m_curline = 3;
					m_once = true;
				}
				else if(m_curline==2)
				{
					GaiLeiji(item1,&leiji1,false);
					m_once2=true;
				}
				else if(m_curline==4)
				{
					GaiLeiji(item1,&leiji2,false);
					m_once2=true;
				}
			}else if ((jian & RIGHT)&&((m_curline==2)||(m_curline==4))) //向右
			{
				item1=(item1+1)%9;
				m_once2=true;
			} else if (jian & ENTER) //MENU按钮
			{
				switch (m_curline) {
				case 0:
					m_curline = 1;
					m_show = 2;
					m_once = true;
					break;
				case 1:
					m_curline=2;
					item1=0;
					m_once = true;
					break;
				case 2:
					ainstr->m_yiqicanshu.m_zhengleijiTiaozheng=leiji1;
					ainstr->WriteCanshu();
					m_curline=1;
					m_once = true;
					break;
				case 3:
					m_curline=4;
					item1=0;
					m_once = true;
					break;
				case 4:
					ainstr->m_yiqicanshu.m_fuleijiTiaozheng=leiji2;
					ainstr->WriteCanshu();
					m_curline=3;
					m_once = true;
					break;
				}
			}
		break;
	}

	case 130:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			i = 0;

			m_lcd.WriteHZx(i,				2 * 0, 3, CHUANGANQI,	m_curline == 1); //传感器
			m_lcd.WriteHZx(i+16*3, 	2 * 0, 2, XISHU, 			m_curline == 1); //系数

			m_lcd.WriteHZx(i,				2 * 1, 2, JISUAN,			m_curline == 2); //计算
			m_lcd.WriteHZx(i+16*2, 	2 * 1, 2, XISHU, 			m_curline == 2); //系数

			m_lcd.WriteHZx(i,				2 * 2, 2, XIANXING,		m_curline == 3); //线性
			m_lcd.WriteHZx(i+16*2, 	2 * 2, 2, TIAOZHENG, 	m_curline == 3); //调整

			m_lcd.WriteHZx(i,				2 * 3, 5, HUIFUCHUCHANG, 	m_curline ==4); //恢复出厂值

			m_lcd.WriteShuju(88, 				2 * 0, ainstr->m_yiqicanshu.m_chuanganqixishu, 3);
			m_lcd.WriteShuju(88, 				2 * 1, ainstr->m_yiqicanshu.m_kk, 3);
			m_lcd.WriteShuju(88, 				2 * 2, ainstr->m_yiqicanshu.m_kk2, 3);
		}
		jian = Anjian::GetKey();
		if(jian==RETURN)
		{
			m_curline = 1;
			m_show = 2;
			m_once = true;
		}else
			if (jian & UP) //向上
			{
				m_once = true;
				m_curline = (m_curline + 4) % 5;
				if(!m_curline)
				{
					m_curline=3;
					m_show=120;
				}
			}else if (jian & DOWN) //向下
			{
				m_once = true;
				m_curline = (m_curline + 1) % 5;
				if(!m_curline)
				{
					m_curline=1;
					m_show=140;
				}

			} else if (jian & ENTER) //MENU按钮
			{
				switch (m_curline) {
				case 0:
					m_curline = 1;
					m_show = 2;
					m_once = true;
					break;
				case 1:
					m_shuju = ainstr->m_yiqicanshu.m_chuanganqixishu;
					Input();
					break;
				case 2:
					m_shuju = ainstr->m_yiqicanshu.m_kk;
					Input();
					break;
				case 3:
					m_shuju = ainstr->m_yiqicanshu.m_kk2;
					Input();
					break;
				case 4:
					inQueren();
					break;
				default:
					break;
				}
			}
		break;
	case 131:
		jian = ShowItem(&m_shuju, 3, 0);
		if (jian & ENTER) {
			ainstr->m_yiqicanshu.m_chuanganqixishu=m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;
	case 132:
		jian = ShowItem(&m_shuju, 3, 0);
		if (jian & ENTER) {
			ainstr->m_yiqicanshu.m_kk=m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;
	case 133:
		jian = ShowItem(&m_shuju, 3, 0);
		if (jian & ENTER) {
			ainstr->m_yiqicanshu.m_kk2=m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;
	case 134:
		if (m_once) {
			m_lcd.WriteHZx(56 - 16, 4, 1, FOUSHI, m_shifou);
			m_lcd.WriteHZx(56 - 8 + 32, 4, 1, FOUSHI + 32, !m_shifou);
		}

		jian = Anjian::GetKey();
		if (jian & ENTER) {
			if (m_shifou) {
				ainstr->load();
			}
			m_curline = 4;
			m_show = 130;
			m_once = true;

		} else if ((jian & RIGHT) || (jian & LEFT)) {
			m_shifou = !m_shifou;
			m_once = true;
		}
		break;

	case 140:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			i = 0;

			m_lcd.WriteHZx(i, 			2 * 0, 2, MIMA, m_curline == 1); //
			m_lcd.WriteHZx(i + 32, 	2 * 0, 1, YIER, m_curline == 1); //

			m_lcd.WriteHZx(i, 			2 * 1, 2, MIMA, m_curline == 2); //
			m_lcd.WriteHZx(i + 32, 	2 * 1, 1, YIER + 32, m_curline == 2); //

			m_lcd.WriteShuju(88 , 2 * 0, Yiqi::m_syscanshu.m_mima[0], 0);
			m_lcd.WriteShuju(88 , 2 * 1, Yiqi::m_syscanshu.m_mima[1], 0);

		}
		jian = Anjian::GetKey();
		if(jian==RETURN)
		{
			m_curline = 1;
			m_show = 2;
			m_once = true;
		}else
			if (jian & UP) //向上
			{
				m_once = true;
				m_curline = (m_curline + 2) % 3;
				if(!m_curline)
				{
					m_curline=4;
					m_show=130;
				}
			}else if (jian & DOWN) //向下
			{
				if (m_curline) {
					m_curline = (m_curline + 1) % 3;
					m_once = true;
				}
			} else if (jian & ENTER) //MENU按钮
			{
				m_once = true;
				switch (m_curline) {
				case 0:
					m_curline = 1;
					m_show = 2;
					m_once = true;
					break;
				case 1:
				case 2:
					m_shuju = Yiqi::m_syscanshu.m_mima[m_curline - 1];
					Input();
					break;
				case 3:
				case 4:
					inQueren();
					break;
				default:
					break;
				}
			}
		break;
	case 141:
	case 142:
		jian = ShowItem(&m_shuju, 0, 0);
		if (jian & ENTER) {
			Yiqi::m_syscanshu.m_mima[m_show - 141] = m_shuju;
			ainstr->WriteCanshu();
			m_show = fanhuiHuamian;
			m_curline = fanhuiXiang;
		}
		break;






	case 200:
	{
		static u8 item1=0;
		static long leiji1=0;
		static bool m_once2=true;
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			i = 0;
			m_lcd.WriteHZx(i, 2 * 0, 2, XIANSHI, 						m_curline == 1);//显示
			m_lcd.WriteString(i + 32, 2 * 0, (u8*) "LOGO", 		m_curline == 1);//LOGO

			m_lcd.WriteHZx(i,				2 * 1, 3, CHUSHIHUA, 						m_curline == 2); //初始化


			m_lcd.WriteHZx(i,				2 * 2, 2, SHEBEI, 								m_curline == 3);//设备
			m_lcd.WriteHZx(i+16*2,		2 * 2, 2, BIANHAO, 							m_curline == 3);//编号

			i=Yiqi::m_syscanshu.m_bLogo;
			m_lcd.WriteHZx(128-16*2, 			2 * 0, 2, JINYUNXU+i*32*2, 		false); //禁止/允许
			m_once2=true;
			leiji1=ainstr->m_syscanshu.m_NO;
		}
		if (m_once2) {
			m_once2=false;
			ShowGaiLeiji(1,item1,&leiji1,m_curline == 4);
		}
		jian = Anjian::GetKey();
		if(jian==RETURN)
		{
			m_curline = 2;
			m_show = 2;
			m_once = true;
		}else
			if (jian & UP) //向上
			{
				if(m_curline==4)
				{
					GaiLeiji(item1,&leiji1,true);
					m_once2=true;
				}else
				{
					m_curline =(m_curline+3)%4;
					m_once = true;
				}

			}else if (jian & DOWN) //向下
			{
				if(m_curline==4)
				{
					GaiLeiji(item1,&leiji1,false);
					m_once2=true;
				}else
				{
					m_curline =(m_curline+1)%4;
					if(!m_curline)
					{
						m_show=210;
						m_curline=1;
					}
					m_once = true;
				}
			}else if ((jian & RIGHT)&&m_curline==4) //向右
			{
				item1=(item1+1)%9;
				m_once2=true;
			} else if (jian & ENTER) //MENU按钮
			{
				switch (m_curline) {
				case 0:
					m_curline = 1;
					m_show = 2;
					m_once = true;
					break;
				case 1:
					m_once = true;
					Yiqi::m_syscanshu.m_bLogo=!Yiqi::m_syscanshu.m_bLogo;
					ainstr->WriteCanshu();
					break;
				case 2:
					inQueren();
					break;
				case 3:
					m_curline=4;
					item1=0;
					m_once = true;
					break;
				case 4:
					ainstr->m_syscanshu.m_NO=leiji1;
					ainstr->WriteCanshu();
					m_curline=3;
					m_once = true;
					break;
				}
			}
		break;
	}
	case 202:
		if (m_once) {
			m_lcd.WriteHZx(56 - 16, 4, 1, FOUSHI, m_shifou);
			m_lcd.WriteHZx(56 - 8 + 32, 4, 1, FOUSHI + 32, !m_shifou);
		}

		jian = Anjian::GetKey();
		if (jian & ENTER) {
			if (m_shifou) {
				ainstr->MorenCanshu();
			}
			m_curline = 2;
			m_show = 200;
			m_once = true;

		} else if ((jian & RIGHT) || (jian & LEFT)) {
			m_shifou = !m_shifou;
			m_once = true;
		}
		break;
	case 210:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			i = 0;
			m_lcd.WriteHZx(i,				2 * 0, 2, CUNWEI, 								m_curline == 1); //存为
			m_lcd.WriteHZx(i+16*2, 	2 * 0, 3, HUIFUCHUCHANG+32*2, 		m_curline == 1); //出厂值
		}
		jian = Anjian::GetKey();
		if(jian==RETURN)
		{
			m_curline = 2;
			m_show = 2;
			m_once = true;
		}else
			if (jian & UP) //向上
			{
				m_once = true;
				m_curline = (m_curline + 1) % 2;
				if(!m_curline)
				{
					m_curline=3;
					m_show=200;
				}
			}else if (jian & DOWN) //向下
			{
				if (m_curline ) {
					m_curline = (m_curline + 1) % 2;
					m_once = true;
				}
			} else if (jian & ENTER) //MENU按钮
			{
				switch (m_curline) {
				case 0:
					m_curline = 1;
					m_show = 2;
					m_once = true;
					break;
				case 1:
					inQueren();
					break;
				default:
					break;
				}
			}
		break;
	case 211:
		if (m_once) {
			m_lcd.WriteHZx(56 - 16, 4, 1, FOUSHI, m_shifou);
			m_lcd.WriteHZx(56 - 8 + 32, 4, 1, FOUSHI + 32, !m_shifou);
		}

		jian = Anjian::GetKey();
		if (jian & ENTER) {
			if (m_shifou) {
				ainstr->save();
			}
			m_curline = 1;
			m_show = 200;
			m_once = true;

		} else if ((jian & RIGHT) || (jian & LEFT)) {
			m_shifou = !m_shifou;
			m_once = true;
		}
		break;



	case 190:
		if (m_once) {
			m_once = false;
			m_lcd.Clear(0);
			ShowInfo();
		}
		if (m_bShuaxin) {
			m_bShuaxin = false;
			ShowInfoData();
		}
		jian = Anjian::GetKey();
		if (jian == ENTER)
			m_show = 0xff;
		break;

	default:
		m_once = true;
		m_show = 1;

		break;
	}

	if (jian != 0)
		m_guoTimes = 0;

}
void Screen::run(u32 ms) { //每秒运行一次
	// TODO 自动生成的析构函数存根
	show();
	if (ms % 1000 == 0) {
		bSec2 = !bSec2;
		ms += 5;
		if (m_guoTimes >= GUOTIMES) {
			m_guoTimes = GUOTIMES;
			if (m_show > 1 && m_show != 190) {
				m_show = 0xff;
			}
			if (ms % 1200 == 0)	//20分钟
				m_show = 0xff;
		} else {
			m_guoTimes++;
		}
		m_bShuaxin = true;
	}
}
void Screen::ShowWord(u8 lie, u16 ww, u8 shan, u8 dot) {
	u8 x = 0;
	u8 y;
	for (; x < 4; x++) {
		if (x < dot)
			y = 0;
		else
			y = 1;
		m_lcd.WriteShuzi(56 + (lie + 3 - x - y) * 8, 4, 1,
				ASCII + 16 * ((ww % 10) + '0' - ' '), (shan != x));
		ww = ww / 10;
	}
	if (dot)
		m_lcd.WriteShuzi(56 + (lie + 3 - dot) * 8, 4, 1,
				ASCII + 16 * ('.' - ' '), 1);	//.

}
void Screen::inQueren() {
	m_show += m_curline;
	m_once = true;
	m_lcd.Fill(1, 16, 2, 112, 0XF0);
	m_lcd.Fill(2, 16, 6, 112, 0XFF);
	m_lcd.Fill(6, 16, 7, 112, 0X0F);
	m_lcd.WriteHZx(48, 2, 2, QUEREN, 1); //
	m_shifou = false;
}
void Screen::Input() {
	m_lcd.Fill(1, 16, 2, 112, 0XF0);
	m_lcd.Fill(2, 16, 6, 112, 0XFF);
	m_lcd.Fill(6, 16, 7, 112, 0X0F);
	m_lcd.WriteHZx(48, 2, 2, SHURU, 1); //
	if (m_shuju < 0) {
		m_shuju = 0 - m_shuju;
		m_fu = true;
	} else
		m_fu = false;

	m_gai = 3;
	m_once = true;
	fanhuiXiang = m_curline;
	fanhuiHuamian = m_show;
	m_show += m_curline;

}
void Screen::ShowGaiLeiji(u8 isFUleiji,u8 item,long *leiji,bool isGAI) {
	if(*leiji>999999999)
		*leiji=999999999;
	long ll=*leiji;
	u8 a[1];
	for(int i=0;i<9;i++)
	{
		a[0]=ll%10+'0';ll/=10;
		m_lcd.WriteString(128-8-8*i,2+4*isFUleiji,a,1,(item==(8-i)&&isGAI));
	}
}

void Screen::Gaishu(double *ff, u8 shan, u8 dot, bool ADD_SUB) {
	u8 b;
	u16 w1, w2;
	if (shan == 4) {
		m_fu = !m_fu;
		return;
	}
	w1 = 1;
	for (b = 0; b < dot; b++)
		*ff = *ff * 10.0;
	w2 = *ff;
	w2 = w2 % 10000;
	*ff = w2;

	for (b = 0; b < shan; b++)
		w1 *= 10;
	w2 = *ff + 0.5;
	b = (w2 / w1) % 10;
	if (ADD_SUB) {
		if (b == 9)
			w2 = w2 - w1 * 9;
		else
			w2 += w1;
	} else {
		if (b == 0)
			w2 = w2 + w1 * 9;
		else
			w2 -= w1;
	}
	*ff = w2;
	for (b = 0; b < dot; b++)
		*ff /= 10.0;
}
u8 Screen::ShowItem(double *shuju, u8 xiaoshu, bool yunxufu) {
	u8 i;
	u16 ww = 1;
	if (m_once) {
		m_lcd.Fill(4, 40, 6, 80, 0xff);
		for (i = 0; i < xiaoshu; i++)
			ww *= 10;
		if ((*shuju) > 9999.0 || (*shuju) < -9999.0)
			*shuju = 0;
		ww = (*shuju) * ww + 0.5;
		if (yunxufu) {
			if (m_fu)
				m_lcd.WriteShuzi(56 - 16, 4, 1, ASCII + 16 * ('-' - ' '),
						m_gai != 4);
			else {
				if (m_gai == 4)
					m_lcd.WriteShuzi(56 - 16, 4, 1, ASCII + 16 * ('+' - ' '),
							0);
			}
		}
		ShowWord(0, ww, m_gai, xiaoshu);
		m_once = false;
	}

	i =  Anjian::GetKey();
	if (i & UP)
		Gaishu(shuju, m_gai, xiaoshu, 1);
	else if (i & DOWN)
		Gaishu(shuju, m_gai, xiaoshu, 0);
	else if (i & LEFT)
		m_gai = (m_gai + 1) % (4 + yunxufu);
	else if (i & RIGHT)
		m_gai = (m_gai + (3 + yunxufu)) % (4 + yunxufu);
	else if (i & ENTER) {
		if (m_fu)
			m_shuju = 0 - m_shuju;
	}
	if (i)
		m_once = true;
	return i;
}

void Screen::IInt() {
	/*
	 u8 DOT = ainstr->getCanshuZhi(2);
	 float m_outH = ainstr->getCanshuZhi(13);
	 float m_outL = ainstr->getCanshuZhi(12);
	 u8 name = ainstr->getCanshuZhi(0);
	 u8 danwei = ainstr->getCanshuZhi(1);
	 */

}
void Screen::ShowMain(u8 item) {
	float ff = ainstr->m_Liuliang_xianshi;
//	m_lcd.WriteShujuD(20, 0, ff, DOT,bSec2&&m_jiaozhun);
	m_lcd.WriteShujuD(20, 0, ff, 2,bSec2&&m_jiaozhun);
	m_lcd.WriteString(128-8*4, 4, (u8*) DANWEISHUZU[ainstr->m_yiqicanshu.m_danwei],	 false);

	if(ainstr->m_baojingOut)
		m_lcd.WriteHZx(0, 4,  1, BAOJINGFUHAO, false);
	else
		m_lcd.WriteString(0, 4, (u8*) "  ", false);
	if(ainstr->m_baojingOut==8)
		m_lcd.WriteString(16, 4, (u8*) "Ex ", false);
	else if(ainstr->m_baojingOut==4)
		m_lcd.WriteString(16, 4, (u8*) "Sys", false);
	else if(ainstr->m_baojingOut==2)
		m_lcd.WriteString(16, 4, (u8*) "PH ", false);
	else if(ainstr->m_baojingOut==1)
		m_lcd.WriteString(16, 4, (u8*) "PL ", false);
	else
		m_lcd.WriteString(16, 4, (u8*) "   ", false);

	m_lcd.WriteString(0, 6, (u8*) "                ", false);
	if(item<3)
	{
		m_lcd.WriteHZx(0, 6, 1, XIGEMA+32*item, false);
		m_lcd.WriteString(128-8*2, 6, (u8*) DANWEISHUZU[ainstr->m_yiqicanshu.m_jisuanDanwei/4*3],2,	 false);
		switch (item) {
		case 0:
			m_lcd.WriteLeijiliuliang(ainstr->getLeijiXianshizhi(0), DOT, false);
			break;
		case 1:
			m_lcd.WriteLeijiliuliang(ainstr->getLeijiXianshizhi(1), DOT, false);
			break;
		case 2:
		{
			double dd=ainstr->m_leijiCanshu.m_leijiZheng+ainstr->m_leijiCanshu.m_leijiFu;
//			if(dd<0)
//			{
//				dd=-dd;
//				m_lcd.WriteString(16+8 , 6, (u8*) "-", false);
//			}
			m_lcd.WriteLeijiliuliang(dd, DOT, false);

		}
		break;
		default:
			break;
		}
	}else if(item==3)
	{
		m_lcd.WriteString(0, 6, (u8*) "FLS", false);
		m_lcd.WriteString(13*8, 6, (u8*) "m/s", false);
		m_lcd.WriteShuju(56,6,ainstr->m_Liusu,2);

	}

}
void Screen::ShowInfoData() {
	m_lcd.WriteShuju(88, 0, ainstr->m_AD[0] , 0);
	m_lcd.WriteShuju(38, 0, ainstr->m_AD[1], 0);

	m_lcd.WriteShuju(88, 2, ainstr->m_AD[2] , 0);
	m_lcd.WriteShuju(38, 2, ainstr->m_AD[3], 0);


	m_lcd.WriteShuju(38, 4, ainstr->m_yiqicanshu.m_pinlv, 1);
	long leiji1=ainstr->m_syscanshu.m_NO;
	ShowGaiLeiji(1,0,&leiji1,0);

}
void Screen::ShowInfo() {
	m_lcd.WriteHZx(0, 0, 2, YUANMA, false);
	m_lcd.WriteHZx(0, 4, 2, MAICHONG+32*2, false);
	m_lcd.WriteHZx(0, 6, 2, BIANHAO, false);

}
void Screen::ShowQuxian() {
	/*
	 int shuju[RealQuxian::DATALEN];
	 const u8 GAO = 50;	//曲线高50
	 u8 i, j;
	 u8 k[GAO];
	 memset(k, 0, GAO);
	 long x, m, n;
	 n = ainstr->getCanshuZhi(12) * 1000.0;	//乘以1000是为了将float转为u32计算，速度快
	 m = ainstr->getCanshuZhi(13) * 1000.0;
	 if (m == 0)
	 m = 1000;
	 for (i = 0; i < RealQuxian::DATALEN; i++) {
	 x = m_quxian->m_data[(m_quxian->m_pData + i + RealQuxian::DATALEN)
	 % RealQuxian::DATALEN] * 1000.0;
	 shuju[i] = (GAO - 1) * (x - n) / (m - n);
	 if (shuju[i] < 0)
	 shuju[i] = 0;
	 else if (shuju[i] >= (GAO - 1))
	 shuju[i] = GAO - 1;
	 }

	 for (i = 0; i < RealQuxian::DATALEN; i++) {
	 k[GAO - 1 - shuju[i]] |= (0x1 << (7 - i % 8));
	 if ((i + 1) % 8 == 0) {
	 for (j = 0; j < GAO; j++) {
	 m_lcd.WriteXY(i / 8, j + (128 - GAO - 1), k[j], true);
	 k[j] = 0;
	 }
	 }

	 }
	 */

}
void Screen::setShuaxinQuxian() {

	//	m_quxian[0].setData(ainstr[0].getHanliang());
	//	m_quxian[1].setData(ainstr[1].getHanliang());
	//	m_quxian[2].setData(ainstr[2].getHanliang());
	//	m_quxian[3].setData(ainstr[3].getHanliang());
	m_bShuaxinQuxian = true;
}

void Screen::GaiLeiji(u8 item, long *leiji, bool isUP) {
	long ll=1;
	for(int i=0;i<8-item;i++)
		ll*=10;
	long a=*leiji/ll;a%=10;
	if(isUP)
	{
		if(a==9)
			*leiji-=(ll*9);
		else
			*leiji+=ll;
	}else
	{
		if(a==0)
			*leiji+=(ll*9);
		else
			*leiji-=ll;
	}
}
/*void Screen::ShowZufen(u8 x, u8 y, u8 item) {

	 u8 name = ainstr[item].getCanshuZhi(0);
	 switch (name) {
	 case 0:
	 m_lcd.WriteShuzi(x + 1, y, 2, QITI_O2, false);
	 break;
	 case 1:
	 m_lcd.WriteShuzi(x + 1, y, 2, QITI_H2, false);
	 break;
	 case 2:
	 m_lcd.WriteShuzi(x, y, 2, QITI_CO, false);
	 break;
	 case 3:
	 m_lcd.WriteShuzi(x, y, 3, QITI_CO2, false);
	 break;
	 }


}
 */
