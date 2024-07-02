/*
 * Ainstrument.cpp
 *
 *  Created on: 2019年11月25日
 *      Author: Administrator
 */
#include "Ainstrument.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
void Ainstrument::save(void) {
	u8 *buf = (u8*)malloc(160);
	m_iic.Read24Cxx(buf, m_dataAddr, 160);
	m_iic.Write24C04(buf, m_dataAddr + 256, 160);
	free(buf);
}
void Ainstrument::load(void) {
	u8 *buf = (u8*)malloc(160);
	m_iic.Read24Cxx(buf, m_dataAddr + 256, 160);
	m_iic.Write24C04(buf, m_dataAddr, 160);
	free(buf);
	ReadCanshu();

}
void Ainstrument::WriteCanshu(void) {
	u32 *uu = (u32*) ffBuf;
	memset(ffBuf, m_dataAddr, 160);
	uu[0] = m_name;
	uu[1] = m_danwei;
	uu[2] = m_Dot; //
	uu[4] = m_yunxuJiaoling;

	uu[5] = m_4;
	uu[6] = m_20;
	uu[7] = m_mima[0];
	uu[8] = m_mima[1];
	uu[9] = m_NO;


	ffBuf[3]=m_pinlv;

	ffBuf[10] = m_ll; 		//标定完后显示的零点
	ffBuf[11] = m_hh; 		//标定完后显示的量程
	ffBuf[12] = m_outL;
	ffBuf[13] = m_outH; //
	ffBuf[14] = m_lingdian;
	ffBuf[15] = m_baojingH;
	ffBuf[16] = m_baojingL;
	ffBuf[17] = m_kk2;
	ffBuf[18] = m_xiuzheng[0];
	ffBuf[19] = m_xiuzheng[1];
	ffBuf[20] = m_xiuzheng[2];
	ffBuf[21] = m_xiuzheng[3];
	ffBuf[22] = m_buchang[0];
	ffBuf[23] = m_buchang[1];
	ffBuf[24] = m_henwendu;
	ffBuf[25] = m_quxian;
	ffBuf[26] = m_zhongjianhangliang;


	ffBuf[30] = m_ling;
	ffBuf[31] = m_kk;
	ffBuf[32] = m_kuadu;
	ffBuf[33] = m_wenduBiao; //
	ffBuf[34] = m_biaoLingHanliang; //

	ffBuf[39]=0x55aa;
	m_iic.Write24C04((u8*) ffBuf, m_dataAddr, 160);
}
void Ainstrument::ReadCanshu(void) {
	u32 *uu = (u32*) ffBuf;
	m_iic.Read24Cxx((u8*) ffBuf, m_dataAddr, 160);
	m_name = uu[0];
	m_danwei = uu[1];
	m_Dot = uu[2]; //
	m_yunxuJiaoling = uu[4];

	m_4 = uu[5];
	m_20 = uu[6];
	m_mima[0] = uu[7];
	m_mima[1] = uu[8];
	m_NO = uu[9];

	m_pinlv=ffBuf[3];
	m_ll = ffBuf[10]; 		//标定完后显示的零点
	m_hh = ffBuf[11]; 		//标定完后显示的量程
	m_outL = ffBuf[12];
	m_outH = ffBuf[13]; //
	m_lingdian = ffBuf[14];
	m_baojingH = ffBuf[15];
	m_baojingL = ffBuf[16];
	m_kk2 = ffBuf[17];
	m_xiuzheng[0] = ffBuf[18];
	m_xiuzheng[1] = ffBuf[19];
	m_xiuzheng[2] = ffBuf[20];
	m_xiuzheng[3] = ffBuf[21];
	m_buchang[0] = ffBuf[22];
	m_buchang[1] = ffBuf[23];
	m_henwendu = ffBuf[24];
	m_quxian = ffBuf[25];
	m_zhongjianhangliang = ffBuf[26];

	m_ling = ffBuf[30];
	m_kk = ffBuf[31];
	m_kuadu = ffBuf[32];
	m_wenduBiao = ffBuf[33]; //
	m_biaoLingHanliang = ffBuf[34]; //


}
void Ainstrument::init(void) {
	m_iic.init();
	ReadCanshu();
	if(ffBuf[39]!=0x55aa)
		MorenCanshu();
}
void Ainstrument::MorenCanshu(void) {
	m_name = 0; //O2
	m_danwei = 0; //%
	m_Dot = 2; //

	m_yunxuJiaoling = 1;
	m_NO = 1000;

	m_4 = 1800;
	m_20 = 9000;
	m_mima[0] = 1000;
	m_mima[1] = 2000;
//	m_NO = 1001;

	m_ll = 0; 		//标定完后显示的零点
	m_hh = 20; 		//标定完后显示的量程
	m_outL = 0;
	m_outH = 20; //
	m_lingdian = 0;
	m_baojingH = 0;
	m_baojingL = 0;
	m_kk2 = 0;
	m_xiuzheng[0] = 0;
	m_xiuzheng[1] = 0;
	m_xiuzheng[2] = 0;
	m_xiuzheng[3] = 0;
	m_buchang[0] = 0;
	m_buchang[1] = 0;
	m_henwendu = 60;
	m_quxian = 1;
	m_zhongjianhangliang = 10;
	m_pinlv=2.0;

	m_ling = 0;
	m_kk = 1;
	m_kuadu = 20;
	m_wenduBiao = 20; //
	m_biaoLingHanliang = 0; //
	WriteCanshu();
}
void Ainstrument::setCanshuZhi(u8 no, double zhi) {
	switch (no) {
	case 0:
		m_name = zhi;
		break;
	case 1:
		m_danwei = zhi;
		break;
	case 2:
		m_Dot = zhi;
		break;

	case 3:
		m_pinlv = zhi;
		break;
	case 4:
		m_yunxuJiaoling = zhi;
		break;
	case 5: //
		m_4 = zhi;
		break;
	case 6:
		m_20 = zhi;
		break;
	case 7:
		m_mima[0] = zhi;
		break;
	case 8:
		m_mima[1] = zhi;
		break;
	case 9:
		m_NO = zhi;
		break;

	case 10:
		m_ll = zhi;
		break;
	case 11: 		//标定完后显示的零点
		m_hh = zhi;
		break;
	case 12: 		//标定完后显示的量程
		m_outL = zhi;
		break;
	case 13:
		m_outH = zhi;
		break;
	case 14: //
		m_lingdian = zhi;
		break;
	case 15:
		m_baojingH = zhi;
		break;
	case 16:
		m_baojingL = zhi;
		break;
	case 17:
		m_kk2 = zhi;
		break;
	case 18:
		m_xiuzheng[0] = zhi;
		break;
	case 19:
		m_xiuzheng[1] = zhi;
		break;
	case 20:
		m_xiuzheng[2] = zhi;
		break;
	case 21:
		m_xiuzheng[3] = zhi;
		break;
	case 22:
		m_buchang[0] = zhi;
		break;
	case 23:
		m_buchang[1] = zhi;
		break;
	case 24:
		m_henwendu = zhi;
		break;
	case 25:
		m_quxian = zhi;
		break;
	case 26:
		m_zhongjianhangliang = zhi;
		break;

	case 30:
		m_ling = zhi;
		break;
	case 31:
		m_kk = zhi;
		break;
	case 32:
		m_kuadu = zhi;
		break;
	case 33:
		m_wenduBiao = zhi;
		break;
	case 34:
		m_biaoLingHanliang = zhi;
		break;

	case 38:
		m_bMA = zhi;
		break;
	case 39:
		m4_20 = zhi;
		break;

	case 101:
		m_AD1 = zhi;
		break;
	case 102:
		m_AD2 = zhi;
		break;
	case 103:
		m_curHengwenwendu = zhi;
		break;
	case 104:
		m_tanceqiWendu = zhi;
		break;
	case 105:
		m_outHeart = zhi;
		break;
	case 106:
		m_outMA = zhi;
		break;
	case 107:
		m_baojingOut = zhi;
		break;
	case 108:
		m_dianjizhuansu = zhi;
		break;
	case 109:
		cpuUsed = zhi;
		break;
	}

}
double Ainstrument::getCanshuZhi(u8 no) {
	double ff = 0;
	switch (no) {
	case 0:
		ff = m_name;
		break;
	case 1:
		ff = m_danwei;
		break;
	case 2:
		ff = m_Dot;
		break;
	case 3:
		ff=m_pinlv;
		break;
	case 4:
		ff = m_yunxuJiaoling;
		break;

	case 5: //
		ff = m_4;
		break;
	case 6:
		ff = m_20;
		break;
	case 7:
		ff = m_mima[0];
		break;
	case 8:
		ff = m_mima[1];
		break;
	case 9:
		ff = m_NO;
		break;

	case 10:
		ff = m_ll;
		break;
	case 11: 		//标定完后显示的零点
		ff = m_hh;
		break;
	case 12: 		//标定完后显示的量程
		ff = m_outL;
		break;
	case 13:
		ff = m_outH;
		break;
	case 14: //
		ff = m_lingdian;
		break;
	case 15:
		ff = m_baojingH;
		break;
	case 16:
		ff = m_baojingL;
		break;
	case 17:
		ff = m_kk2;
		break;
	case 18:
		ff = m_xiuzheng[0];
		break;
	case 19:
		ff = m_xiuzheng[1];
		break;
	case 20:
		ff = m_xiuzheng[2];
		break;
	case 21:
		ff = m_xiuzheng[3];
		break;
	case 22:
		ff = m_buchang[0];
		break;
	case 23:
		ff = m_buchang[1];
		break;
	case 24:
		ff = m_henwendu;
		break;
	case 25:
		ff = m_quxian;
		break;
	case 26:
		ff = m_zhongjianhangliang;
		break;

	case 30:
		ff = m_ling;
		break;
	case 31:
		ff = m_kk;
		break;
	case 32:
		ff = m_kuadu;
		break;
	case 33:
		ff = m_wenduBiao;
		break;
	case 34:
		ff = m_biaoLingHanliang;
		break;

	case 38:
		ff = m_bMA;
		break;
	case 39:
		ff = m4_20;
		break;

	case 101:
		ff = m_ad[0];//m_AD1;
		break;
	case 102:
		ff = m_ad[1];//m_AD2;
		break;
	case 103:
		ff = m_curHengwenwendu;
		break;
	case 104:
		ff = m_tanceqiWendu;
		break;
	case 105:
		ff = m_outHeart;
		break;
	case 106:
		ff = m_outMA;
		break;
	case 107:
		ff = m_baojingOut;
		break;
	case 108:
		ff = m_dianjizhuansu;
		break;
	case 109:
		ff = cpuUsed;
		break;
	}
	return ff;
}
float Ainstrument::getHanliang(void) {
	return m_Hanliang;
}
float Ainstrument::getHanliang2(void) {
	return m_Hanliang2;
}
float Ainstrument::Xiuzheng(float x1) {
	float f11;
//	float f22 = (m_hh - m_ll) * 0.2;
	float f22 = m_kuadu * 0.2;
	if ((x1 < 0) || (x1 > (m_kuadu)))
		return 0;
	if (x1 < f22)
		f11 = m_xiuzheng[0] * x1 / f22;
	else if (x1 < f22 * 2)
		f11 = m_xiuzheng[0]
				+ (m_xiuzheng[1] - m_xiuzheng[0]) * (x1 - f22) / f22;
	else if (x1 < f22 * 3)
		f11 = m_xiuzheng[1]
				+ (m_xiuzheng[2] - m_xiuzheng[1]) * (x1 - f22 * 2) / f22;
	else if (x1 < f22 * 4)
		f11 = m_xiuzheng[2]
				+ (m_xiuzheng[3] - m_xiuzheng[2]) * (x1 - f22 * 3) / f22;
	else
		f11 = m_xiuzheng[3] - m_xiuzheng[3] * (x1 - f22 * 4) / f22;
	return f11;
}

void Ainstrument::DataChuli(float a,float b, float wendu) {
	float zhi=a*1.0;///b;
//	zhi = 0.35;
	m_zhiLVBO[m_pLVBO] = zhi;
	m_adLvbo[0][m_pLVBO]=a;
	m_adLvbo[1][m_pLVBO]=b;

	m_pLVBO = (m_pLVBO + 1) % LVBO;

	float benciYuanMaCha = fabs((zhi - m_lvbohouBiliZhi) * m_kk);

	if (benciYuanMaCha > (m_kuadu * 0.1))//变化超过10%，快速显示
		lvbo = 2;
	else if (lvbo < LVBO)
		lvbo++;

	float ss = 0,s0=0,s1=0;

	for (int i = 0; i < lvbo; i++)
	{
		ss += m_zhiLVBO[(m_pLVBO + LVBO - 1 - i) % LVBO];
		s0 += m_adLvbo[0][(m_pLVBO + LVBO - 1 - i) % LVBO];
		s1 += m_adLvbo[1][(m_pLVBO + LVBO - 1 - i) % LVBO];
	}
	m_ad[0]=s0/lvbo;
	m_ad[1]=s1/lvbo;

	ss = ss / lvbo;
	m_lvbohouBiliZhi = ss;
//	m_lvbohouBiliZhi = s0/s1;
	ss = ss * (1 + (wendu - m_wenduBiao) * m_buchang[0]/100.0);
	ss = (ss - m_ling) * m_kk;
	m_zhixian = ss;


	if (m_kk2 > 0) {
		ss = (pow(10, m_zhixian * log10(m_quxian + 1)) - 1) / m_quxian;	//0-1之间 下弯曲线
		ss = m_zhixian - (m_zhixian - ss) * m_kk2;//m_kk2=-1上弯曲最大；=0是直线；=1下弯曲最大
	} else if (m_kk2 < 0) {
		ss = log10(m_quxian * m_zhixian + 1) / log10(m_quxian + 1);	//0-1之间 上弯曲线
		ss = m_zhixian + (m_zhixian - ss) * m_kk2;//m_kk2=-1上弯曲最大；=0是直线；=1下弯曲最大
	}

//	ss = ss * (m_hh - m_ll) / 1.00;
	ss = ss * m_kuadu / 1.00;

	ss = ss + Xiuzheng(ss);
//	if (ss > (m_hh - m_ll))
//		ss = (ss + zhixian * m_quxian) / 2.0;

	ss += m_biaoLingHanliang;
	ss += m_lingdian;
	m_Hanliang2 = ss;
	if (ss <= 0)
		m_Hanliang = 0;
	else
		m_Hanliang = ss;

}
u16 Ainstrument::MAchuli() {
	float ff = 0;
	if (m_bMA > 0)
		ff = m4_20;
	else {
		if (m_Hanliang <= m_outL)
			ff = 0;
		else if (m_Hanliang >= m_outH)
			ff = 1;
		else
			ff = (m_Hanliang - m_outL) / (m_outH - m_outL);
		ff = m_4 + (m_20 - m_4) * ff;
	}
	return u16(ff);
}
void Ainstrument::setJiaozhun(u8 jiaozhun) { //0取消校准状态，1校准零点，2校准校准量程,3校准中间值
	m_bJiaozhun = jiaozhun;
}
void Ainstrument::JIAOZHUNchuli(float wendu) {
//	if (m_bJiaozhun == 1) { //校准零点
//		m_ling = m_lvbohouBiliZhi;
//		m_wenduBiao = wendu;
//		WriteCanshu();
//	} else if (m_bJiaozhun == 2) //校准量程
//			{
//		m_biaozhi = (m_hh - m_ll) / m_quxian;
//		m_kk = m_biaozhi / (m_lvbohouBiliZhi);
//		m_wenduBiao = wendu;
//		WriteCanshu();

	if (m_bJiaozhun == 1) { //校准零点
		m_ling = m_lvbohouBiliZhi;
		m_biaoLingHanliang = m_ll;
		m_wenduBiao = wendu;
		WriteCanshu();
	} else if (m_bJiaozhun == 2) //校准量程
			{
		m_kk = 1.00 / (m_lvbohouBiliZhi - m_ling);
		m_biaoLingHanliang = m_ll;
		m_wenduBiao = wendu;
		m_kuadu = m_hh - m_ll;
		WriteCanshu();
	} else if (m_bJiaozhun == 3) //校准线性
			{
		m_zhongjianhangliang = 0.50;
		quxian0_1.getQuxianChangshu(m_zhixian, m_zhongjianhangliang, m_quxian);
		if (m_zhixian > 0.5) //下弯曲线
			m_kk2 = 1;
		else if (m_zhixian < 0.5) //上弯曲线
			m_kk2 = -1;

		WriteCanshu();
	}
	m_bJiaozhun = 0;
}

u8 Ainstrument::BJchuli(void) {
	u8 aa = 0;
	if (m_baojingL > 0.001) {
		{
			if (m_Hanliang < m_baojingL)
				aa |= 0x01;
			else if (m_Hanliang > (m_baojingL + (m_outH - m_outL) / 100.0))
				aa &= (~0x01);
		}
	} else
		aa &= (~0x01);
	if (m_baojingH > 0.001) {
		if (m_Hanliang > m_baojingH)
			aa |= 0x02;
		else if (m_Hanliang < (m_baojingH - (m_outH - m_outL) / 100.0))
			aa &= (~0x02);
	} else
		aa &= (~0x02);
	return aa;
}

void Ainstrument::online() { //每秒一次的在线活动
	DataChuli(m_AD1 , m_AD2, m_tanceqiWendu);
	JIAOZHUNchuli(m_tanceqiWendu);
	m_outMA = MAchuli();
	m_outHeart = m_HeartPid.pid(m_curHengwenwendu, m_henwendu);
	m_baojingOut = BJchuli();
	if (m_baojingOut == 0) //无报警
		m_bXY = false;

}

/*
 下弯曲线
 y=(pow(10,x*log10(k+1))-1)/k
 以上公式x 0-1对应y 0-1，k越大越下弯

 上弯曲线
 y=log(kx+1)/log(x+1)
 以上公式x 0-1对应y 0-1，k越大越上弯

 */
