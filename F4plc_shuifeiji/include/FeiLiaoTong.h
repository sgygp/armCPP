/*
 * FeiLiaoTong.h
 *
 *  Created on: 2022.8.24
 *      Author: Administrator
 */

#ifndef FEILIAOTONG_H_
#define FEILIAOTONG_H_
#include "stm32f4xx_conf.h"
#include "Cunchu.h"
#include "math.h"


class FeiLiaoTong {
public:
	FeiLiaoTong();
	static u16 m_baohuZhi;
	static float getGaodu(float ronglianL);
	void setNO(u8 no);
	void DoJinshui(bool isON);
	void DoJiaoban(bool isON);
	void DoOut(bool isON);
	void setJinshuiLiang(float ff);
	float getCurRongliang_L();
	void run();


	float getCurH() const {
		return m_curH;
	}

	void setCurH(float curH) {
		m_curH = curH-Cunchu::m_sysCanshu.m_tongdiH/100.0;
	}
	bool isPeiliaoWancheng()
	{
		return m_bPeiliaoWancheng;
	}

	bool isBenciGuangaiWancheng()
	{
		return m_bBenciguanquWancheng;
	}
	bool isJinshuiWancheng()
	{
		return m_bJinshuiWancheng;
	}
	float getShengyuJinshui()
	{
		float ff=Cunchu::m_sysCanshu.m_D;
		ff/=100.0;
		return (m_jinshuiMubiaoH-m_curH)*M_PI/4.0*ff*ff*1000.0;
	};
	float getShengyuJiaoban()
	{
		return m_jiaobanJishi;
	}
	float getShengyuOut()
	{
		float ff=Cunchu::m_sysCanshu.m_D;
		ff/=100.0;
		return (m_curH-m_outYeweiH)*M_PI/4.0*ff*ff*1000.0;
	}
	
	void start(float mubiaoH,float jiaobanshijianFenzhong,float shifeiliang);
	void pause(bool isZanting);
	void startGuangai(u8 cishu);
	void end();
	static void setGuangaiZongcishu(u8 x);
	static u8 m_guanGaiZongcishu;
//	static u8 m_guanGaibuzhou;//0预灌溉，1灌溉，2冲洗
	float m_qishiGuangaiGaodu=0;
	void JisuanShifeiCanshu();
	void runBaohu(void);
private:
	u8  m_no;//第几号桶0-7
	float m_curH=0;//当前液位高度,单位米
	float m_jinshuiMubiaoH=0;//目标高度，负数时无动作
	float m_outYeweiH=0;//本次输出时目标液位高度
	int m_jiaobanJishi=0;
	float m_shifeiliang=0;
	float m_meiciShuchuGaodu=0;
	bool m_bJinshuiWancheng=true;
	bool m_bPeiliaoWancheng=true;
	bool m_bShifei=false;
	bool m_bBenciguanquWancheng=false;
	void runPeiliao();
	void runGuangai();
	int m_jiaobanjishi=0;
	
};

#endif /* FEILIAOTONG_H_ */
