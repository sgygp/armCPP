/*
 * FeiLiaoTong.cpp
 *
 *  Created on: 2022年8月24日
 *      Author: Administrator
 */
#include "math.h"
#include "FeiLiaoTong.h"
#include "PortSet.h"
#include "Cunchu.h"
#include "HuanjingShuju.h"
u8 FeiLiaoTong::m_guanGaiZongcishu=0;
u16 FeiLiaoTong::m_baohuZhi=0;
FeiLiaoTong::FeiLiaoTong() {
	// TODO 自动生成的构造函数存根
	m_no=0;
}
void FeiLiaoTong::setNO(u8 no) {
	this->m_no=no;
}

void FeiLiaoTong::DoJinshui(bool isON) {
	PortSet::setBitOut(Cunchu::m_sysCanshu.m_tongJinshuiNO[m_no],isON);
	if(isON)
		PortSet::m_faZhuangtai[2]|=(1<<m_no);
	else
		PortSet::m_faZhuangtai[2]&=(~(1<<m_no));
}

void FeiLiaoTong::DoJiaoban(bool isON) {
	PortSet::setBitOut(Cunchu::m_sysCanshu.m_tongJiaobanNO[m_no],isON);
	if(isON)
		PortSet::m_faZhuangtai[3]|=(1<<m_no);
	else
		PortSet::m_faZhuangtai[3]&=(~(1<<m_no));

}
void FeiLiaoTong::DoOut(bool isON) {
	PortSet::setBitOut(Cunchu::m_sysCanshu.m_tongOutFaNO[m_no],isON);
	if(isON)
		PortSet::m_faZhuangtai[4]|=(1<<m_no);
	else
		PortSet::m_faZhuangtai[4]&=(~(1<<m_no));

}
float FeiLiaoTong::getCurRongliang_L() {
	float ff=Cunchu::m_sysCanshu.m_D;
	ff/=100.0;
	ff=M_PI/4.0*1000*(ff*ff)*(m_curH);//单位升
	return ff;
}
float FeiLiaoTong::getGaodu(float ronglianL) {
	float ff=Cunchu::m_sysCanshu.m_D;
	ff/=100.0;
	ff=ronglianL/1000.0/(M_PI/4.0*ff*ff);
	return ff;
}
void FeiLiaoTong::runGuangai()
{
	if(!m_bPeiliaoWancheng)
		return;
	if(!m_bShifei)
		return;
	if(m_curH<=m_outYeweiH)
	{
		m_bBenciguanquWancheng=true;
		m_bShifei=false;
		DoOut(false);
	}else
	{
		DoOut(true);

	}
}
void FeiLiaoTong::setGuangaiZongcishu(u8 x)
{
	m_guanGaiZongcishu=x;
}
void FeiLiaoTong::runPeiliao()
{
	if(m_bPeiliaoWancheng)
		return;

	if(!m_bJinshuiWancheng)
	{
		if(m_jinshuiMubiaoH>m_curH)//需要进水，进水完成后m_mubiaoH归零
		{
			DoJinshui(true);
			DoJiaoban(false);
			DoOut(false);
		}else 
		{
			DoJinshui(false);
			DoJiaoban(false);
			DoOut(false);
			m_jinshuiMubiaoH=0;
			m_bJinshuiWancheng=true;
		
		}
		return;
	}
//////////////进水完成后判断是否需要搅拌
	if(this->m_jiaobanjishi)
	{
		DoJinshui(false);
		DoJiaoban(true);
		DoOut(false);

		if(m_jiaobanJishi<=0)//未开始计时
			m_jiaobanJishi=m_jiaobanjishi*60;//开始计时
		else
		{
			m_jiaobanJishi--;
			if(m_jiaobanJishi<=0)
			{
				m_jiaobanjishi=0;
				m_bShifei=false;
				m_bBenciguanquWancheng=false;
				DoJiaoban(false);
			}
	
		}
	}else
		m_bPeiliaoWancheng=true;
	/////灌溉完一个罐区 m_bPeiliaoWancheng 复位，下一个罐区开始的时候m_bBentongYunxing置位并重新设置目标液位，继续开始。
}
void FeiLiaoTong::start(float mubiaoH,float jiaobanshijianFenzhong,float shifeiliang){
	
	m_bJinshuiWancheng=false;
	m_bPeiliaoWancheng=false;
	m_bShifei=false;
	
	m_jinshuiMubiaoH=mubiaoH;
	m_jiaobanjishi = jiaobanshijianFenzhong;
	m_jiaobanJishi=0;
	m_shifeiliang=shifeiliang;
}
	
void FeiLiaoTong::startGuangai(u8 cishu){
	if(m_shifeiliang==0)
	{
		m_meiciShuchuGaodu=0;
		m_bShifei=false;
		m_bBenciguanquWancheng=true;
	
	}else{

		m_outYeweiH=m_qishiGuangaiGaodu-cishu*m_meiciShuchuGaodu;
		m_bShifei=true;
		m_bBenciguanquWancheng=false;
	
	}
}
void FeiLiaoTong::pause(bool isZanting){
	if(isZanting)
	{
	
		DoJinshui(false);
		DoJiaoban(false);
		DoOut(false);
	}
}
void FeiLiaoTong::JisuanShifeiCanshu(){
	m_qishiGuangaiGaodu=getCurH();
	if(m_shifeiliang<0)
		m_meiciShuchuGaodu=m_qishiGuangaiGaodu/FeiLiaoTong::m_guanGaiZongcishu;
	else if(m_shifeiliang>0)
	{
		if(m_shifeiliang>getCurRongliang_L())
			m_shifeiliang=getCurRongliang_L();
		m_meiciShuchuGaodu=getGaodu(m_shifeiliang)/FeiLiaoTong::m_guanGaiZongcishu;

	}else
		m_meiciShuchuGaodu=0;
}
void FeiLiaoTong::end(){
	m_bPeiliaoWancheng=true;
	m_bJinshuiWancheng=true;
	m_bBenciguanquWancheng=true;
	m_bShifei=false;
	DoJinshui(false);
	DoJiaoban(false);
	DoOut(false);
	
	
}
void FeiLiaoTong::run() 
{//1秒钟运行一次
	setCurH((HuanjingShuju::m_AD[this->m_no]-MA_4)*Cunchu::m_sysCanshu.m_yalichuanganqiLiangcheng/100.0/MA_CHA);
	runPeiliao();
	runGuangai();
}
void FeiLiaoTong::runBaohu(void){
	return;
	if(getCurH()<=0)
	{
		DoOut(false);
		FeiLiaoTong::m_baohuZhi|=(1<<0);//液位低于0，保护
	}else
		FeiLiaoTong::m_baohuZhi&=~(1<<0);//液位不低于0，取消保护

	int h=Cunchu::m_sysCanshu.m_muyeRongliangBaohu/100.0*1000;
	if(h&&getCurRongliang_L()>h)
	{
		FeiLiaoTong::m_baohuZhi|=(1<<1);//液位达到最大高度，保护
		DoJinshui(false);
	}else
	{
		FeiLiaoTong::m_baohuZhi&=~(1<<1);//液位未达到最大高度，取消保护
	}
}
