/*
 * FeiLiaoTong.cpp
 *
 *  Created on: 2022��8��24��
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
	// TODO �Զ����ɵĹ��캯�����
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
	ff=M_PI/4.0*1000*(ff*ff)*(m_curH);//��λ��
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
		if(m_jinshuiMubiaoH>m_curH)//��Ҫ��ˮ����ˮ��ɺ�m_mubiaoH����
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
//////////////��ˮ��ɺ��ж��Ƿ���Ҫ����
	if(this->m_jiaobanjishi)
	{
		DoJinshui(false);
		DoJiaoban(true);
		DoOut(false);

		if(m_jiaobanJishi<=0)//δ��ʼ��ʱ
			m_jiaobanJishi=m_jiaobanjishi*60;//��ʼ��ʱ
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
	/////�����һ������ m_bPeiliaoWancheng ��λ����һ��������ʼ��ʱ��m_bBentongYunxing��λ����������Ŀ��Һλ��������ʼ��
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
{//1��������һ��
	setCurH((HuanjingShuju::m_AD[this->m_no]-MA_4)*Cunchu::m_sysCanshu.m_yalichuanganqiLiangcheng/100.0/MA_CHA);
	runPeiliao();
	runGuangai();
}
void FeiLiaoTong::runBaohu(void){
	return;
	if(getCurH()<=0)
	{
		DoOut(false);
		FeiLiaoTong::m_baohuZhi|=(1<<0);//Һλ����0������
	}else
		FeiLiaoTong::m_baohuZhi&=~(1<<0);//Һλ������0��ȡ������

	int h=Cunchu::m_sysCanshu.m_muyeRongliangBaohu/100.0*1000;
	if(h&&getCurRongliang_L()>h)
	{
		FeiLiaoTong::m_baohuZhi|=(1<<1);//Һλ�ﵽ���߶ȣ�����
		DoJinshui(false);
	}else
	{
		FeiLiaoTong::m_baohuZhi&=~(1<<1);//Һλδ�ﵽ���߶ȣ�ȡ������
	}
}
