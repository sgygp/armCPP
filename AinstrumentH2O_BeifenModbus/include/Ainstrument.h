/*
 * Ainstrument.h
 *
 *  Created on: 2019年11月25日
 *      Author: Administrator
 */

#ifndef AINSTRUMENT_H_
#define AINSTRUMENT_H_

#include "stm32f10x_conf.h"
#include "HeartPID.h"
#include "Quxian0_1.h"
#include "II_C.h"

#define LVBO 40
class Ainstrument {
public:
	bool m_bXY;//消音

private:
	float ffBuf[40];

	u8 m_name;
	u8 m_danwei;
	u8 m_Dot; //
	u8 m_yunxuJiaoling;

	u16 m_4;
	u16 m_20;
	u16 m_mima[2];
	u32 m_NO;

	float m_ll; 		//标定完后显示的零点
	float m_hh; 		//标定完后显示的量程
	float m_outL;
	float m_outH; //
	float m_lingdian;
	float m_kk2;
	float m_baojingL;
	float m_baojingH;
	float m_xiuzheng[4];
	float m_buchang[2];
	float m_henwendu;
	float m_quxian;
	float m_zhongjianhangliang;
	float m_pinlv;

	float m_ling;
	float m_kk;
	float m_kuadu;
	float m_wenduBiao; //
	float m_biaoLingHanliang; //


	float m_Hanliang;
	float m_Hanliang2;
	u8 lvbo;
	u8 m_pLVBO;
	float m_zhiLVBO[LVBO];
	float m_lvbohouBiliZhi;

	u8 m_bJiaozhun; //0正常测量，1校准零点，2校准量程
	u8 m_bMA; //0取消校准电流状态，1校准4mA，2校准20mA
	u8 m_baojingOut; //0无报警，1低限报警，2高限报警
	u16 m_outMA; //4-20mA源码输出o

	u16 m4_20;
	float m_zhixian;

	float m_AD1;
	float m_AD2;
	float m_curHengwenwendu;
	float m_tanceqiWendu;
	float m_outHeart;
	HeartPID m_HeartPid;
	Quxian0_1 quxian0_1;
	 u32 m_dataAddr;
	 II_C m_iic;

	u16 m_dianjizhuansu=0;
	int cpuUsed=0;

	float m_adLvbo[2][LVBO]={{0},{0}};
	float m_ad[2]={0};


public:
	// Default constructor
	Ainstrument(u32 addr) {

		m_bXY=false;

		lvbo = 1;
		m_pLVBO = 0;
		m_Hanliang = 0;
		m_Hanliang2 = 0;
		m_lvbohouBiliZhi = 0;
		for (u8 i = 0; i < LVBO; i++)
			m_zhiLVBO[i] = 0;
		m_bJiaozhun = 0;
		m_bMA = 0;
		m4_20 = 5000;
		m_zhixian = 0;

		m_AD1 = 10000;
		m_AD2 = 20000;
		m_curHengwenwendu = 25;
		m_tanceqiWendu = 25;
		m_outHeart = 0;
		m_baojingOut = 0;
		m_outMA = 5000;

		m_name = 3; //O2
		m_danwei = 0; //%
		m_Dot = 2; //
		m_yunxuJiaoling = 1;

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
		m_henwendu = 40;
		m_quxian = 1;
		m_zhongjianhangliang = 10;

		m_ling = 0;
		m_kk = 25;
		m_kuadu = 20;
		m_wenduBiao = 20; //
		m_biaoLingHanliang=0;//标定时的零点含量
		m_NO = 1000;
		m_dataAddr=addr;
	};
	void online();
	void setJiaozhun(u8 jiaozhun);
	float getHanliang();
	float getHanliang2();
	double getCanshuZhi(u8 no);
	void setCanshuZhi(u8 no, double zhi);
	void MorenCanshu();
	void init();
	void load();
	void save();
	void WriteCanshu();
	void ReadCanshu();
private:
	float Xiuzheng(float x1);
	void DataChuli(float a,float b, float wendu);
	void JIAOZHUNchuli(float wendu);
	u16 MAchuli();
	u8 BJchuli(void);

};

#endif /* AINSTRUMENT_H_ */
