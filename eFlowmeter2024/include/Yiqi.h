/*
 * Yiqi.h
 *
 *  Created on: 2021��10��10��
 *      Author: Administrator
 */

#ifndef YIQI_H_
#define YIQI_H_
#include "Quxian0_1.h"
#include <RealQuxian.h>

#define LVBO 15
#define MIMA1 1000
#define MIMA2 2000
#define MIMA3 8888
#define PORTS_DIANYA  GPIOC
#define PINS_A               GPIO_Pin_8
#define PINS_B               GPIO_Pin_9
#define LICI_PORT			GPIOB
#define LICI_PIN				GPIO_Pin_0

//#define CUNCHUCANSHUSTART sizeof(SysCanshu)
static const u16 KOUJING[36] = {
		3,6,10,15,20,25/*2.0382165605095541401273885350318��Ϊ1L*/,32,40,50,65,80,//11��
		100,150,200,250,300,350,400,450,500,600,700,800,900,//13��
		1000,1200,1400,1600,1800,2000,2200,2400,2500,2600,2800,3000//12��

};
static const char KOUJINGDN[36][6] = {
				{' ',' ',' ','D','N','3'},{' ',' ',' ','D','N','6'},{' ',' ','D','N','1','0'},{' ',' ','D','N','1','5'},{' ',' ','D','N','2','0'},{' ',' ','D','N','2','5'}/*2.0382165605095541401273885350318��Ϊ1L*/,{' ',' ','D','N','3','2'},{' ',' ','D','N','4','0'},{' ',' ','D','N','5','0'},{' ',' ','D','N','6','5'},{' ',' ','D','N','8','0'},//11��
				{' ','D','N','1','0','0'},{' ','D','N','1','5','0'},{' ','D','N','2','0','0'},{' ','D','N','2','5','0'},{' ','D','N','3','0','0'},{' ','D','N','3','5','0'},{' ','D','N','4','0','0'},{' ','D','N','4','5','0'},{' ','D','N','5','0','0'},{' ','D','N','6','0','0'},{' ','D','N','7','0','0'},{' ','D','N','8','0','0'},{' ','D','N','9','0','0'},//13��
				{'D','N','1','0','0','0'},{'D','N','1','2','0','0'},{'D','N','1','4','0','0'},{'D','N','1','6','0','0'},{'D','N','1','8','0','0'},{'D','N','2','0','0','0'},{'D','N','2','2','0','0'},{'D','N','2','4','0','0'},{'D','N','2','5','0','0'},{'D','N','2','6','0','0'},{'D','N','2','8','0','0'},{'D','N','3','0','0','0'}//12��

};
static const u8 DANWEISHUZU[8][4] = {
		{' ','L','/','h'},
		{' ','L','/','m'},
		{' ','L','/','s'},
		{' ','#','/','h'},
		{' ','#','/','m'},
		{' ','#','/','s'},


		{' ','T','/','h'},
		{'K','g','/','h'},
};
static const u8 JISUANDANWEI[8][6] = {
		{'0','.','0','0','1','L'},
		{' ','0','.','0','1','L'},
		{' ',' ','0','.','1','L'},
		{' ',' ',' ',' ','1','L'},
		{'0','.','0','0','1','#'},
		{' ','0','.','0','1','#'},
		{' ',' ','0','.','1','#'},
		{' ',' ',' ',' ','1','#'},
};
static const float DANWEI_XISHU_MIDU[8] = {
		3600,
		60,
		1,
		3.6,
		0.060,
		0.001,
		3.6,
		3600,
};


typedef struct   //4+4+4+4+4=20
{
	double m_leijiZheng;
	double m_leijiFu;

}LeijiCanshu;


typedef struct   //4+4+4+4+4=20
{
	u8 m_bLogo;
	u8 m_yuyan; //����
	u8 m_addr;
	u8 m_botelv;
	u16 m_mima[2];
	u32 m_leijiqinglingMima;
	u32 m_MARK; //55AA���
	u32 m_NO;

} SysCanshu;

typedef struct   //16+4*2+4*4+18*4=112�ֽ�
{
	u8  m_koujing;
	u8 m_danwei;
	u8 m_jisuanDanwei;//0.001,0.01,0.1,1,0.001,0.01,0.1,1 /L /m3
	u8 m_fangxiang;
	u8 m_bXiaoQiechu;//С�ź��Ƿ���ʾ
	u8 m_fuyunxu;
	u8 m_maichongshuchufangshi;//0���壬1Ƶ��
	u8 m_maichongdangliang;//0.001,0.01,0.1,1
//	u8 m_kongguanBaojingYunxu;
//	u8 m_licibaojingYunxu;
	u8 m_licidianya;
	u8 m_pinlv;
	u8 m_8beiyong[4];

	u16 m_lingdianAD;
	u16 m_pinlvfanwei;
	u16 m_kongguangbaojingYuzhi;
	u16 m_4;
	u16 m_20;
	float m_outH; //
	float m_baojingL;
	float m_baojingH;
	float m_kk2;
	float m_kk;
	float m_xiuzheng[4];
	float m_buchang[2];
	float m_quxian;
	float m_xiaoxinhaoqiechuDian;//С�ź��г���ٷֱ�
	float m_chuanganqixishu;
	float m_beiyong;
	float m_midu;
	long m_zhengleijiTiaozheng;
	long m_fuleijiTiaozheng;

} YiqiCanshu;


class Yiqi {
public:
	u8 m_bXY=0;
	static SysCanshu m_syscanshu;
	static LeijiCanshu m_leijiCanshu;
	YiqiCanshu m_yiqicanshu;
	u16 m_AD[6]={0};

	u8 m_Dot=0; //
	float m_Liusu=0;
	float m_Liuliang_xLmeiS=0;
	float m_Liuliang_xianshi=0;
	u8 m_bJiaozhun=0; //0����������1У׼��㣬2У׼����
	u8 m_bMA=0; //0ȡ��У׼����״̬��1У׼4mA��2У׼20mA
	u16 m4_20=1000;		//4-20mAԴ��У׼ʱ�����
	u8 m_baojingOut=0; //0�ޱ�����1���ޱ�����2���ޱ�����4�չܱ�����8���ű���
	u16 m_outMA=0; 	//4-20mAԴ���У׼ʱ�����
	float m_outFreq=0.1;
private:
	u8 lvbo=0;
	u8 m_pLVBO=0;
	float m_zhiLVBO[LVBO]={0};
	float m_lvbohou=0;
	u32 m_ID=0;
	float m_zhixian=1;
	Quxian0_1 quxian0_1;
	u32 m_oldMS5 = 0;
	double m_shangdianLeiji=0;
	int m_yiFasongMaichongshu=0;
	int m_weifasongMaichongshu=0;



public:
	static void WriteLeiji();
	void canshu2Hardware();
	void online(u16 *a);
	void setJiaozhun(u8 jiaozhun);
//	double getCanshuZhi(u8 no);
//	void setCanshuZhi(u8 no, double zhi);
	void MorenCanshu();
	void init(u32 id);
	void load();
	void save();
	void WriteCanshu(void);
	void ReadCanshu(void);
	double getLeijiXianshizhi(u8 bFu);
	bool FasongMaichong(void);

private:
	float Xiuzheng(float x1);
	void DataChuli();
	void PinlvChuli();
	void JIAOZHUNchuli(float wendu);
	u16 MAchuli();
	void BJchuli(void);
	void LeijiChuli(void);
	float GetXianshiZhi(float xl);

	inline void	__attribute__((always_inline)) onA() {
		GPIO_InitTypeDef GPIO_InitStructure; //��Ϊ�����ע�⣺2MHZ
		GPIO_InitStructure.GPIO_Pin =  PINS_A;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_ResetBits(PORTS_DIANYA, GPIO_Pin_14);
	}
	inline void	__attribute__((always_inline)) onB() {
		GPIO_InitTypeDef GPIO_InitStructure; //��Ϊ�����ע�⣺2MHZ
		GPIO_InitStructure.GPIO_Pin =  PINS_B;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_ResetBits(PORTS_DIANYA, GPIO_Pin_14);
	}
	 inline void	__attribute__((always_inline))  offA() {
		GPIO_InitTypeDef GPIO_InitStructure; //��Ϊ�����ע�⣺2MHZ
		GPIO_InitStructure.GPIO_Pin =  PINS_A;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(PORTS_DIANYA, &GPIO_InitStructure);
	}
	 inline void	__attribute__((always_inline)) offB() {
		GPIO_InitTypeDef GPIO_InitStructure; //��Ϊ�����ע�⣺2MHZ
		GPIO_InitStructure.GPIO_Pin =  PINS_B;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(PORTS_DIANYA, &GPIO_InitStructure);
	}
	 inline void	__attribute__((always_inline)) tiaoDianya(u8 i) {
		switch(i)
		{
		case 1:
			onA();
			offB();
			break;
		case 2:
			offA();
			onB();
			break;
		case 3:
			onA();
			onB();
			break;
		default:
			offA();
			offB();
			break;
		}
	}
};

#endif /* YIQI_H_ */
