/*
 * Yiqi.cpp
 *
 *  Created on: 2021��10��10��
 *      Author: Administrator
 */

#include "math.h"
#include "Yiqi.h"
#include <CtrPulse.h>
#include "II_C.h"
#include "math.h"
#include <string.h>
#include "Beep.h"
#include "Timer.h"

#define _PI 3.14159265

LeijiCanshu Yiqi::m_leijiCanshu;
SysCanshu Yiqi::m_syscanshu;

float Liusu2Liuliang(float liusu/*m/s*/,u8 koujing)//����L/s
{
	float r=KOUJING[koujing]/1000.0/2;
	float jiemianji=_PI*r*r;//��λƽ����
	return liusu*jiemianji*1000.0;

}
float getZhi(float liuliang/*L/s*/,u8 danwei,float midu)
{
	float ff=liuliang*DANWEI_XISHU_MIDU[danwei];
	if(danwei==6||danwei==7)
		ff*=midu;
	return ff;
}
void Yiqi::canshu2Hardware() {
	u8 pinlv=m_yiqicanshu.m_pinlv;
	if (pinlv == 0)
		pinlv = 1;
	if (pinlv > 50)
		pinlv = 50;
	CtrPulse::init(1125, CTRLOAD / pinlv); //1125ʱ��CTRLOAD��64000������Ϊ1��
	CtrPulse::m_fangxiang=m_yiqicanshu.m_fangxiang;
	tiaoDianya(m_yiqicanshu.m_licidianya);

}
void Yiqi::MorenCanshu(void) {
	memset(&m_syscanshu,0,sizeof(m_syscanshu));
	memset(&m_yiqicanshu,0,sizeof(m_yiqicanshu));
	m_syscanshu.m_bLogo = 1;
	m_syscanshu.m_yuyan = 0;
	m_syscanshu.m_addr = 1;
	m_syscanshu.m_botelv=3;
	m_syscanshu.m_mima[0] = MIMA1;
	m_syscanshu.m_mima[1] = MIMA2;
	m_syscanshu.m_leijiqinglingMima = MIMA3;
	m_syscanshu.m_MARK = 0x55aaaa55;
	m_syscanshu. m_NO=1234;

	m_leijiCanshu. m_leijiZheng=0;
	m_leijiCanshu. m_leijiFu=0;


	m_yiqicanshu. m_koujing=5;//DN25
	m_yiqicanshu.m_danwei=2;//L/s
	m_yiqicanshu.m_jisuanDanwei=4;//0.001,0.01,0.1,1,0.001,0.01,0.1,1 /L /m3 2=0.1L
	m_yiqicanshu.m_fangxiang=0;
	m_yiqicanshu.m_bXiaoQiechu=0;//С�ź��Ƿ��г�
	m_yiqicanshu.m_fuyunxu=1;
	m_yiqicanshu.m_maichongshuchufangshi=0;//Ĭ������
	m_yiqicanshu.m_maichongdangliang=2;//0.001,0.01,0.1,1m3
	//	m_yiqicanshu.m_kongguanBaojingYunxu;
	//	m_yiqicanshu.m_licibaojingYunxu;
	m_yiqicanshu.m_licidianya=0;
	m_yiqicanshu.m_pinlv=15;
	//u8 m_8beiyong[6];

	m_yiqicanshu.m_lingdianAD=0;
	m_yiqicanshu.m_pinlvfanwei=200;
	m_yiqicanshu.m_kongguangbaojingYuzhi=0;
	m_yiqicanshu.m_4=1800;
	m_yiqicanshu.m_20=9000;
	//	m_yiqicanshu.m_16beiyong[4];
	m_yiqicanshu. m_outH=500; //
	m_yiqicanshu. m_baojingL=0;
	m_yiqicanshu. m_baojingH=0;
	m_yiqicanshu. m_kk2=0.000;
	m_yiqicanshu. m_kk=1.000;
	//m_yiqicanshu. m_xiuzheng[4];
	//m_yiqicanshu. m_buchang[2];
	m_yiqicanshu. m_quxian=1;
	m_yiqicanshu. m_xiaoxinhaoqiechuDian=0.5;//С�ź��г���ٷֱ�
	m_yiqicanshu. m_chuanganqixishu=1;
	m_yiqicanshu. m_midu=1.000;
	m_yiqicanshu. m_zhengleijiTiaozheng=0;
	m_yiqicanshu. m_fuleijiTiaozheng=0;

	//u32 m_32beiyong[2];
	//float m_floatBeiyong[3];

	WriteCanshu();
	ReadCanshu();

	canshu2Hardware();
}
void Yiqi::init(u32 id) {
	m_ID = id;
	II_C::init();
	ReadCanshu();
	II_C::Read24Cxx((u8*) &m_leijiCanshu, 0, sizeof(m_leijiCanshu));
	if (m_syscanshu.m_MARK != 0x55aaaa55) //δ��ʼ��
		MorenCanshu();
	m_shangdianLeiji=m_leijiCanshu.m_leijiZheng;

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LICI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LICI_PORT, &GPIO_InitStructure);

}

void Yiqi::save(void) {
	ReadCanshu();
	//	II_C::Write24C04((u8*) &m_leijiCanshu, 200, sizeof(m_leijiCanshu));
	II_C::Write24C04((u8*) &m_syscanshu, 200+sizeof(m_leijiCanshu), sizeof(m_syscanshu));
	II_C::Write24C04((u8*) &m_yiqicanshu, 200+sizeof(m_leijiCanshu)+sizeof(m_syscanshu),sizeof(m_yiqicanshu));
}
void Yiqi::load(void) {
	II_C::Read24Cxx((u8*) &m_yiqicanshu, 200+sizeof(m_leijiCanshu)+sizeof(m_syscanshu),sizeof(m_yiqicanshu));
	II_C::Read24Cxx((u8*) &m_syscanshu, 200+sizeof(m_leijiCanshu), sizeof(m_syscanshu));
	//	II_C::Read24Cxx((u8*) &m_leijiCanshu, 200, sizeof(m_leijiCanshu));
	WriteCanshu();
	ReadCanshu();

}
void Yiqi::WriteCanshu(void) {
	II_C::Write24C04((u8*) &m_syscanshu, sizeof(m_leijiCanshu), sizeof(m_syscanshu));
	II_C::Write24C04((u8*) &m_yiqicanshu, sizeof(m_leijiCanshu)+sizeof(m_syscanshu),sizeof(m_yiqicanshu));

}
void Yiqi::ReadCanshu(void) { //ǰ10���ֽڱ���
	II_C::Read24Cxx((u8*) &m_syscanshu, sizeof(m_leijiCanshu), sizeof(m_syscanshu));
	II_C::Read24Cxx((u8*) &m_yiqicanshu, sizeof(m_leijiCanshu)+sizeof(m_syscanshu),sizeof(m_yiqicanshu));

	canshu2Hardware();

}
float Yiqi::Xiuzheng(float x1) {
	float f11;
	float m_kuadu=1;
	//	float f22 = (m_hh - m_ll) * 0.2;
	float f22 = m_kuadu * 0.2;
	if ((x1 < 0) || (x1 > (m_kuadu)))
		return 0;
	if (x1 < f22)
		f11 = m_yiqicanshu.m_xiuzheng[0] * x1 / f22;
	else if (x1 < f22 * 2)
		f11 = m_yiqicanshu.m_xiuzheng[0]
									  + (m_yiqicanshu.m_xiuzheng[1] - m_yiqicanshu.m_xiuzheng[0]) * (x1 - f22) / f22;
	else if (x1 < f22 * 3)
		f11 = m_yiqicanshu.m_xiuzheng[1]
									  + (m_yiqicanshu.m_xiuzheng[2] - m_yiqicanshu.m_xiuzheng[1]) * (x1 - f22 * 2) / f22;
	else if (x1 < f22 * 4)
		f11 = m_yiqicanshu.m_xiuzheng[2]
									  + (m_yiqicanshu.m_xiuzheng[3] - m_yiqicanshu.m_xiuzheng[2]) * (x1 - f22 * 3) / f22;
	else
		f11 = m_yiqicanshu.m_xiuzheng[3] - m_yiqicanshu.m_xiuzheng[3] * (x1 - f22 * 4) / f22;
	return f11;
}

void Yiqi::DataChuli() {
	float zhi = 0;
	bool bFu=false;
	if(m_AD[2]>0&&m_AD[3]==0)
		zhi=m_AD[2];
	else if(m_AD[3]>0&&m_AD[2]==0)
	{
		bFu=true;
		zhi=-m_AD[3];
	}
	else
		m_Liusu=0;

	m_zhiLVBO[m_pLVBO] =zhi;
	m_pLVBO = (m_pLVBO + 1) % LVBO;

	float benciYuanMaCha = fabs(zhi - m_lvbohou) ;

	if (benciYuanMaCha > 50) //�仯����1%��������ʾ
		lvbo = 1;
	else if (lvbo < LVBO)
		lvbo++;
	//	lvbo=LVBO;
	float ss = 0;
	for (int i = 0; i < lvbo; i++)
		ss += m_zhiLVBO[(m_pLVBO + LVBO - 1 - i) % LVBO];

	ss = ss / lvbo;
	m_lvbohou = ss;
	if(ss<0)
		ss+=m_yiqicanshu.m_lingdianAD;
	else
		ss-=m_yiqicanshu.m_lingdianAD;
	ss = ss * m_yiqicanshu.m_kk/1000.0;
	m_zhixian = ss;
	float m_quxian=1;
	if (m_yiqicanshu.m_kk2 > 0) {
		ss = (pow(10, m_zhixian * log10(m_quxian + 1)) - 1) / m_quxian;	//0-1֮�� ��������
		ss = m_zhixian - (m_zhixian - ss) * m_yiqicanshu.m_kk2;//m_kk2=-1���������=0��ֱ�ߣ�=1���������
	} else if (m_yiqicanshu.m_kk2 < 0) {
		ss = log10(m_quxian * m_zhixian + 1) / log10(m_quxian + 1);	//0-  -1֮�� ��������
		ss = m_zhixian + (m_zhixian - ss) *m_yiqicanshu. m_kk2;//m_kk2=-1���������=0��ֱ�ߣ�=1���������
	}

	//	//	ss = ss * (m_hh - m_ll) / 1.00;
	//	ss = ss * m_yiqicanshu.m_outH / 1.00;

	ss = ss + Xiuzheng(ss);
	m_Liusu = ss;
	//	if (ss > (m_hh - m_ll))
	//		ss = (ss + zhixian * m_quxian) / 2.0;
	float liuliang=GetXianshiZhi(Liusu2Liuliang(m_Liusu,m_yiqicanshu.m_koujing));
	m_Liusu = ss;

	if(m_yiqicanshu.m_bXiaoQiechu)
	{
		if (fabs(liuliang) <= m_yiqicanshu.m_xiaoxinhaoqiechuDian*m_yiqicanshu.m_outH /100.0)
			m_Liusu = 0;
	}
	if(liuliang<!m_yiqicanshu.m_fuyunxu)
		liuliang=0;

	m_Liuliang_xLmeiS=Liusu2Liuliang(m_Liusu,m_yiqicanshu.m_koujing);


}
u16 Yiqi::MAchuli() {
	float ff = 0;
	if (m_bMA > 0)
		ff = m4_20;
	else {
		if (m_Liuliang_xianshi <= 0.0f)
			ff = 0;
		else if (m_Liuliang_xianshi >= m_yiqicanshu.m_outH)
			ff = 1;
		else
			ff = (m_Liuliang_xianshi - 0.0f) / (m_yiqicanshu.m_outH -0.0f);
		ff = m_yiqicanshu.m_4 + (m_yiqicanshu.m_20 - m_yiqicanshu.m_4) * ff;
	}
	return u16(ff);
}
void Yiqi::setJiaozhun(u8 jiaozhun) { //0ȡ��У׼״̬��1У׼��㣬2У׼У׼����,3У׼�м�ֵ
	m_bJiaozhun = jiaozhun;
}
void Yiqi::JIAOZHUNchuli(float wendu) {
	/*
	if (m_bJiaozhun == 1) { //У׼���
		m_ling = m_lvbohouBiliZhi;
		m_biaoLingHanliang = m_ll;
		m_wenduBiao = wendu;
		WriteCanshu();
	} else if (m_bJiaozhun == 2) //У׼����
			{
		m_kk = 1.00 / (m_lvbohouBiliZhi - m_ling);
		m_biaoLingHanliang = m_ll;
		m_wenduBiao = wendu;
		m_kuadu = m_hh - m_ll;
		WriteCanshu();
	} else if (m_bJiaozhun == 3) //У׼����
			{
		m_zhongjianhangliang = 0.50;
		quxian0_1.getQuxianChangshu(m_zhixian, m_zhongjianhangliang, m_quxian);
		if (m_zhixian > 0.5) //��������
			m_kk2 = 1;
		else if (m_zhixian < 0.5) //��������
			m_kk2 = -1;

		WriteCanshu();
	}
	m_bJiaozhun = 0;
	 */
}

void Yiqi::BJchuli(void) {
	if (m_yiqicanshu.m_baojingL > 0.001) {
		if (m_Liuliang_xianshi < m_yiqicanshu.m_baojingL) {
			m_baojingOut |= 0x01;
		} else if (m_Liuliang_xianshi > (m_yiqicanshu.m_baojingL + (m_yiqicanshu.m_outH - 0.0f) / 100.0)) {
			m_baojingOut &= (~0x01);
		}
	} else {
		m_baojingOut &= (~0x01);
	}
	if (m_yiqicanshu.m_baojingH > 0.001) {
		if (m_Liuliang_xianshi > m_yiqicanshu.m_baojingH) {
			m_baojingOut |= 0x02;
		} else if (m_Liuliang_xianshi < (m_yiqicanshu.m_baojingH - (m_yiqicanshu.m_outH - 0.0f) / 100.0)) {
			m_baojingOut &= (~0x02);
		}
	} else {
		m_baojingOut &= (~0x02);
	}

}
void Yiqi::PinlvChuli() {
	m_outFreq=m_Liuliang_xianshi/m_yiqicanshu.m_outH*m_yiqicanshu.m_pinlvfanwei;

	double bencizhi=m_leijiCanshu.m_leijiZheng-m_shangdianLeiji;
	bencizhi*=1000.0;//��λΪ0.001L
	for(int i=0;i<m_yiqicanshu.m_maichongdangliang;i++)
		bencizhi/=10.0;
	int a=bencizhi;
	m_weifasongMaichongshu=a-m_yiFasongMaichongshu;
}
bool Yiqi::FasongMaichong(void) {
	static bool bShuchuDi=false;
	if(bShuchuDi)
	{
		m_weifasongMaichongshu--;
		m_yiFasongMaichongshu++;
		bShuchuDi=false;
		return true;
	}else
	{
		if(m_weifasongMaichongshu>0)
		{
			bShuchuDi=true;
			return false;
		}else
			return true;
	}

}

void Yiqi::online(u16* a) { //ÿ��һ�ε����߻
	memcpy(m_AD,a,sizeof(m_AD));
	if(m_baojingOut&0x08)
		m_Liuliang_xLmeiS=0;
	else
		DataChuli();
	m_Liuliang_xianshi=GetXianshiZhi(m_Liuliang_xLmeiS);
	PinlvChuli();
	JIAOZHUNchuli(0);
	m_outMA = MAchuli();
	BJchuli();
	if(GPIO_ReadInputDataBit(LICI_PORT,LICI_PIN))
		m_baojingOut&=(~0x08);
	else
		m_baojingOut|=0x08;

	LeijiChuli();

}

void Yiqi::WriteLeiji() {
	//	m_leijiCanshu.m_leijiFu=-112234578;
	II_C::Write24C04((u8*) &m_leijiCanshu.m_leijiZheng, 0,16);
}

double Yiqi::getLeijiXianshizhi(u8 bFu) {
	double l=m_leijiCanshu.m_leijiZheng;
	if(bFu)
		l=m_leijiCanshu.m_leijiFu;
	if(m_yiqicanshu.m_jisuanDanwei>3)
		l/=1000.0;
	return l;
}


float Yiqi::GetXianshiZhi(float xl) {
	return getZhi(xl,m_yiqicanshu.m_danwei,m_yiqicanshu.m_midu);
}

void Yiqi::LeijiChuli(void) {//�ۻ���ԭʼ��λ��L

	u32 msShu=(Timer::ms5_jishu+Timer::MAXSHU-m_oldMS5)%Timer::MAXSHU;
	double sec=msShu/200.0;
	if(m_Liuliang_xLmeiS<0)
		m_leijiCanshu.m_leijiFu+=(m_Liuliang_xLmeiS*sec);
	else
		m_leijiCanshu.m_leijiZheng+=(m_Liuliang_xLmeiS*sec);

	m_oldMS5=Timer::ms5_jishu;
}
/*
 ��������
 y=(pow(10,x*log10(k+1))-1)/k
 ���Ϲ�ʽx 0-1��Ӧy 0-1��kԽ��Խ����

 ��������
 y=log(kx+1)/log(x+1)
 ���Ϲ�ʽx 0-1��Ӧy 0-1��kԽ��Խ����

 */
