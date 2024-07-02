/*
 * Device.cpp
 *
 *  Created on: 2020年10月13日
 *      Author: Administrator
 */
#include "CalendarTimer.h"
#include "Device.h"
#include "HuanjingShuju.h"


u16 Device::sysShebeiZhuangtai=0;
u16 Device::yunXingShengyu=0;
u16 Device::dangQianGuanQu=0;
u8 Device::YuanKong=0;
u8 Device::m_mode=0;//0手动，1湿度，2定时
u8 Device::m_buzhou=0;//0空闲状态；1配料状态（此步骤计量进水量跟搅拌时间）；2预灌溉；3灌溉（此步骤打开肥桶阀并计量（有的话），打开罐区阀并计量）；4灌溉冲洗阶段，5水泵关闭延时
u8 Device::cpuUsed=0;
u16 Device::baohuZhuangtai=0;

Device::Device()
{
	this->m_curCelv=Cunchu::m_celvCanshu[Cunchu::m_sysCanshu.m_curCelvNO];
	celv=&m_curCelv;
	m_dataOK=true;
	m_commandW=0;
	m_commandR=0;
	for(int i=0;i<8;i++)
	{
		this->m_feiliaotong[i].setNO(i);

	}
}

void Device::DoGuanqufa(u8 no,bool isON){
	if(no>31)
		return;
	u32 x;
	if(no>=16)
		x=1;
	else
		x=0;

	if(isON==(PortSet::m_faZhuangtai[x]&(1<<(no%16))))
		return;
	PortSet::setBitOut(Cunchu::m_sysCanshu.m_guanquFaNO[no],isON);
	if(isON)
		PortSet::m_faZhuangtai[x]|=(1<<(no%16));
	else
		PortSet::m_faZhuangtai[x]&=(~(1<<(no%16)));
	x=no;
	x<<=16;
	HuanjingShuju::m_command2=0x03000000|x|isON;
}
void Device::DoQingshuibeng(bool isON){//9号站
	if(isON==(PortSet::m_faZhuangtai[5]&(1<<0)))
		return;
	PortSet::setBitOut(Cunchu::m_sysCanshu.m_qingshuiBengNO,isON);
	if(isON)
		PortSet::m_faZhuangtai[5]|=(1<<0);
	else
		PortSet::m_faZhuangtai[5]&=(~(1<<0));
	HuanjingShuju::m_command=0x09000000|isON;

}
void Device::DoFeibeng(bool isON){
	if(isON==(PortSet::m_faZhuangtai[5]&(1<<1)))
		return;
	PortSet::setBitOut(Cunchu::m_sysCanshu.m_feiBengNO,isON);
	if(isON)
		PortSet::m_faZhuangtai[5]|=(1<<1);
	else
		PortSet::m_faZhuangtai[5]&=(~(1<<1));
	HuanjingShuju::m_command=0x0A000000|isON;
}
void Device::DoDiandongFa(bool isON){
	if(m_diandongfaFanzhuanYanshi||m_diandongfaJishi)
		DiandongFaPAUSE(isON);
	else
	{
		if(isON)
			DiandongFaON();
		else
			DiandongFaOFF();
}
}
void Device::chuliDiandongfa() {
	if(m_diandongfaFanzhuanYanshi)
	{
		m_diandongfaFanzhuanYanshi--;
		if(m_diandongfaFanzhuanYanshi<=0)
		{
			if(m_diandongfaYanshihouShuchu)
				DiandongFaON();
			else
				DiandongFaOFF();
		}
	}else if(m_diandongfaJishi)
	{
		m_diandongfaJishi--;
		if(m_diandongfaJishi<=0)
			DiandongFaSTOP();
	}

	if(m_diancifaJishi)
	{
		m_diancifaJishi--;
		if(m_diancifaJishi<=0)
			PortSet::setBitOut(Cunchu::m_sysCanshu.m_shuiyuanDiancifIO,0);//电磁阀关闭

	}

}
void Device::doCelv(int no) {//no=-1;执行当前策略
	if(m_buzhou)//正在执行策略
		return;
	if(!YuanKong&&m_mode==1)//湿度控制模式
		return;
	if(no>15)//策略选择错误
		return;
	stop();

	if(no<0)
		celv=&m_curCelv;
	else
		celv=&Cunchu::m_celvCanshu[no];
	m_guanQushu=0;
	for(int i=0;i<32;i++)
	{
		if(celv->m_xGuanqu&(1<<i))
			m_guanQushu++;
	}
	FeiLiaoTong::setGuangaiZongcishu(m_guanQushu);
	u8 a=celv->m_xTong;
	for(int i=0;i<8;i++)
	{
		if(!((a>>i)&0x01))
			continue;
		float jinshuigaodu=FeiLiaoTong::getGaodu( celv->m_jinshuiLiang[i]);
		m_feiliaotong[i].start(m_feiliaotong[i].getCurH()+jinshuigaodu,celv->m_jiaobanTime,celv->m_shifeiLiang[i]);
	}
	m_buzhou=1;
	m_bShifei=bool(celv->m_xTong);
}


int Device::getCurGuangaiNO() {
	int dangQianguanquNO=0;
	for(int i=0;i<32;i++)
	{
		if(celv->m_xGuanqu&(1<<i))
			dangQianguanquNO++;
		if(dangQianguanquNO==m_curGuangaiCishu)
		{
			dangQianguanquNO=i;
			break;
		}
	}
	return dangQianguanquNO;
}

void Device::chuliCommand() {
	u16 command=getCommand();
	if(command==0)
		return;
	if(command<10)//解决command为0的情况
		command--;
	u8 h=command>>8;
	u8 l=command&0xff;
	if(h>=60)
	{
		if(l==1)
		{
//			bZhunbeiShidumoshi=true;
			shiduGuangaiX=-1;
		}
		Device::m_mode=l;
		stop();	}
	else if(h==0)
	{
		if(l==0)
			stop();
		else if(l==2)
			pause();
		else
			doCelv(-1);
	}
	else
	{
		if(m_mode!=0)//不是手动模式
			return;
		if(m_buzhou!=0)//不是空闲模式
			return;
		if(h>=1&&h<=32)
			DoGuanqufa(h-1,l);
		else if(h>=1+32&&h<=8+32)
			m_feiliaotong[h-1-32].DoJinshui(l);
		else if(h>=1+40&&h<=8+40)
			m_feiliaotong[h-1-40].DoJiaoban(l);
		else if(h>=1+48&&h<=8+48)
			m_feiliaotong[h-1-48].DoOut(l);
		else if(h==57)
			DoQingshuibeng(l);
		else if(h==58)
			DoFeibeng(l);
		else if(h==59)
			DoDiandongFa(l);

	}
}
int Device::getShiduKeyiGuangai()
{

	int a=-1;
	for(int i=0;i<32;i++)
	{
		if(Cunchu::m_sysCanshu.m_shiduGuanQu&(1<<i))
		{
			if(HuanjingShuju::m_shuju[i]<Cunchu::m_sysCanshu.m_shiduStart)
			{
				a=i;
				break;
			}

		}
	}
	return a;
}

void Device::runShidu() {

	if(m_bZanting)
		return;
	if(shiduGuangaiX<0)
	{
		shiduGuangaiX=getShiduKeyiGuangai();
	}else
	{
		if(HuanjingShuju::m_shuju[shiduGuangaiX]>=Cunchu::m_sysCanshu.m_shiduEnd)//灌溉达到结束条件
		{
			int g=getShiduKeyiGuangai();
			if(g>=0)//还有需要灌溉的
			{
				DoGuanqufa(shiduGuangaiX,false);//
				DoGuanqufa(g,true);//
			}
			shiduGuangaiX=g;

		}
	}
	if(bShiduGuangai)//已开始灌溉
	{
		if(shiduGuangaiX<0)//全关闭
		{
			bShiduGuangai=false;
//			bZhunbeiShidumoshi=true;
			stop();
		}else
			DoGuanqufa(shiduGuangaiX,true);//

	}else{ 
		if(shiduGuangaiX>=0)
		{//未开始,但是需要灌溉
			DoGuanqufa(shiduGuangaiX,true);//
			DoDiandongFa(true);//
			DoQingshuibeng(true);
			bShiduGuangai=true;

		}
	}
}

u8 Device::runCelvBuzhou() {
	int x=0;
	u8 shengyu=0;
	m_leijiLiuliang=HuanjingShuju::m_shuju[32+167-140];
	m_leijiLiuliang<<=16;
	m_leijiLiuliang|=HuanjingShuju::m_shuju[32+168-140];
	switch(m_buzhou)
	{
	case 0:
		break;
	case 1://配料中
		{
			x=0;
			for(int i=0;i<8;i++)//统计是否全部进水完成
			{
				if(!m_feiliaotong[i].isJinshuiWancheng())
				{
					x=1;
					break;
				}
			}
			if(!x)//进水全部完成
			{
				shengyu=0;
				for(int i=0;i<8;i++)
				{
					x=m_feiliaotong[i].getShengyuJiaoban();
					if(x>shengyu)
						shengyu=x;
				}
			}else//进水中
			{
				DoQingshuibeng(true);

				shengyu=0;
				for(int i=0;i<8;i++)
				{
					x=m_feiliaotong[i].getShengyuJinshui();
					if(x>shengyu)
						shengyu=x;
				}

			}

			x=0;
			for(int i=0;i<8;i++)//统计是否配料完成
			{
				if(!m_feiliaotong[i].isPeiliaoWancheng())
				{
					x=1;
					break;
				}
			}
			if(!x)//配料完成
			{
				m_once=false;
				if(celv->m_xGuanqu==0)//无罐区选择
				{
					DoQingshuibeng(false);//水泵可能已打开，关闭一下
					m_buzhou=0;
				}else
				{
					DoQingshuibeng(true);//如果不加这条语句，只搅拌的时候是不进水的
					m_buzhou=2;
					DoDiandongFa(true);
					m_curGuangaiCishu=1;
				}

			}
		}
		break;
	case 2://预灌溉
		{
			DoGuanqufa(getCurGuangaiNO(),true);
			if(!m_once)//预灌溉未开始
			{
				m_once=true;
				if(celv->m_startGuangai==0)//预灌溉最少为1
					celv->m_startGuangai=1;

				if(celv->m_mode==0)//时间模式
					m_secs=celv->m_startGuangai*60;
				else//立方米模式
					m_mubiaoZhi=m_leijiLiuliang+celv->m_startGuangai;

			}else
			{//预灌溉计算阶段
				bool b=false;
				if(celv->m_mode==0)//时间模式
				{
					shengyu=m_secs;
					if(!m_secs)
						b=true;
				}else//立方米模式
				{
					shengyu=m_mubiaoZhi-m_leijiLiuliang;
					if(m_leijiLiuliang>=m_mubiaoZhi)
						b=true;
				}
				if(b)//预灌溉完成
				{
					m_once=false;
					m_buzhou=3;
				}

			}
		}
		break;
	case 3://灌溉中间阶段
		{
			DoGuanqufa(getCurGuangaiNO(),true);
			if(!m_once)//灌溉中间阶段未开始
			{
				if(m_bShifei)//如果需要施肥
				{
					DoFeibeng(true);
					if(celv->m_mode==0)//时间模式
					{
						if(celv->m_startGuangai==0)
							celv->m_startGuangai=1;
						if(celv->m_endGuangai==0)
							celv->m_endGuangai=1;
						long ll=celv->m_zongGuangai;
						ll-=celv->m_startGuangai;
						ll-=celv->m_endGuangai;
						if(ll<=0)
						{
							celv->m_zongGuangai=3;
							ll=1;
						}
						m_secs=ll*60;
						//需要施肥的肥阀都打开
						for(int i=0;i<8;i++)
						{
							m_feiliaotong[i].DoOut(celv->m_xTong&(1<<i));
						}
					}else
					{
						for(int i=0;i<8;i++)
						{
							m_feiliaotong[i].JisuanShifeiCanshu();
							if(celv->m_xTong&(1<<i))
								m_feiliaotong[i].startGuangai(m_curGuangaiCishu);
						}
					}
				}else{
					if(celv->m_startGuangai==0)
						celv->m_startGuangai=1;
					if(celv->m_endGuangai==0)
						celv->m_endGuangai=1;
					long ll=celv->m_zongGuangai;
					ll-=celv->m_startGuangai;
					ll-=celv->m_endGuangai;
					if(ll<=0)
					{
						ll=1;
						celv->m_zongGuangai=3;
					}

					if(celv->m_mode==0)//时间模式
						m_secs=ll*60;
					else//立方米模式
						m_mubiaoZhi=m_leijiLiuliang+ll;

				}
				m_once=true;
			}else//灌溉中间计算阶段
			{
				if(m_bShifei&&celv->m_mode!=0)//时间模式)//需要施肥的话，以全部施肥完毕为结束条件
				{
					int x=0;
					shengyu=0;
					for(int i=0;i<8;i++)
					{
						if(celv->m_xTong&(1<<i))
						{
							if(!m_feiliaotong[i].isBenciGuangaiWancheng())
								x=1;

							int abc=m_feiliaotong[i].getShengyuOut();
							if(abc>shengyu)
								shengyu=abc;
						}

					}
					if(x==0)//完成
					{
						m_once=false;
						m_buzhou=4;

					}
				}else//如果不施肥，以计算为准
				{
					bool b=false;

					if(celv->m_mode==0)//时间模式
					{
						shengyu=m_secs;
						if(!m_secs)
						{
							b=true;
							if(celv->m_endGuangai<1)
								celv->m_endGuangai=1;
							m_secs=celv->m_endGuangai*60;
						}
					}else//立方米模式
					{
						shengyu=m_mubiaoZhi-m_leijiLiuliang;
						if(m_leijiLiuliang>=m_mubiaoZhi)
						{
							b=true;
							if(celv->m_endGuangai<1)//预灌溉最少为1
								celv->m_endGuangai=1;
							m_mubiaoZhi=m_leijiLiuliang+celv->m_endGuangai;

						}
					}
					if(b)//灌溉中间完成
					{
						m_once=false;
						m_buzhou=4;
					}

				}

			}
		}
		break;
	case 4://冲洗阶段
		{
			DoGuanqufa(getCurGuangaiNO(),true);
			if(!m_once)//冲洗未开始
			{
				m_once=true;
				for(int i=0;i<32;i++)//肥桶阀全关
					m_feiliaotong[i].DoOut(false);
				if(celv->m_endGuangai==0)//预灌溉最少为1
					celv->m_endGuangai=1;
				if(celv->m_mode==0)//时间模式
					m_secs=celv->m_endGuangai*60;
				else//立方米模式
					m_mubiaoZhi=m_leijiLiuliang+celv->m_endGuangai;
			}else
			{
				bool b=false;
				if(celv->m_mode==0)//时间模式
				{
					shengyu=m_secs;
					if(!m_secs)
						b=true;
				}else//立方米模式
				{
					shengyu=m_mubiaoZhi-m_leijiLiuliang;
					if(m_leijiLiuliang>=m_mubiaoZhi)
						b=true;
				}
				if(b)//冲洗完成
				{
					m_once=false;
					if(m_curGuangaiCishu==FeiLiaoTong::m_guanGaiZongcishu)//如果是最后一个罐区完成
					{
						DoFeibeng(false);//关肥泵
						DoQingshuibeng(false);
						m_secs=SHUIBENGQIANHOUJIANGE;//延时10秒
						m_buzhou=5;
					}else
					{
						DoGuanqufa(getCurGuangaiNO(),false);//关本罐区
						m_curGuangaiCishu++;
						m_buzhou=2;
					}
				}

			}
		}
		break;
	case 5://
		{
			shengyu=m_secs;
			if(!m_secs)
			{
				for(int i=0;i<32;i++)//罐区阀全关
					DoGuanqufa(i,false);
//				DoDiandongFa(false);
				m_curGuangaiCishu=1;
				m_buzhou=0;
			}
		}
		break;
	default://
		m_buzhou=0;
		break;

	}
	if(m_buzhou)
	{
		runCelvBaohu();
		FeiLiaoTong::m_baohuZhi=0;
		for(int i=0;i<8;i++)
			m_feiliaotong[i].runBaohu();
		Device::baohuZhuangtai|=FeiLiaoTong::m_baohuZhi;
	}

	return shengyu;
}

void Device::run() {
	u8 shengyu;
	chuliCommand();
	chuliDiandongfa();
	runXielouJianceBaohu();
	if(m_bZanting)
		return;

	if(m_secs)
		m_secs--;

	for(int i=0;i<8;i++)
		m_feiliaotong[i].run();
	shengyu=runCelvBuzhou();

	u16 bencitime = CalendarTimer::RTC_TimeStructure.RTC_Hours * 100
			+ CalendarTimer::RTC_TimeStructure.RTC_Minutes;

	u8 runType ;
	if (YuanKong == 1)
		runType = 0;
	else {
		runType = m_mode+ 1;
	}
	if (runType > 3) //不可识别的运行模式时，改为手动
		runType = 1;

	if(runType==2)//湿度控制
	{
		runShidu();
		if(shiduGuangaiX<0)
		{
			if(m_buzhou==5)
				yunXingShengyu=m_secs;
			else
				yunXingShengyu=0;

			dangQianGuanQu=0;

		}
		else
		{
			u16 sheng=Cunchu::m_sysCanshu.m_shiduEnd-HuanjingShuju::m_shuju[shiduGuangaiX];
			yunXingShengyu=sheng/10;
			dangQianGuanQu=(shiduGuangaiX+1);

		}
	}else
	{
		if(runType==3)//定时控制
		{
			for(int i=0;i<8;i++)
			{
				if(!Cunchu::m_dingshiCanshu[i].m_bQiyong)
					continue;
				u16 dingshiTime=Cunchu::m_dingshiCanshu[i].m_hour;
				dingshiTime*=100;
				dingshiTime+=Cunchu::m_dingshiCanshu[i].m_fen;
				if(oldTime <dingshiTime  && bencitime >=dingshiTime)
					doCelv(Cunchu::m_dingshiCanshu[i].m_celvNO);
			}
		}
		yunXingShengyu=shengyu;
		dangQianGuanQu=getCurGuangaiNO()+1;
	}
	oldTime=bencitime;

}
void Device::runXielouJianceBaohu(){
	//return;
	bool b1=false;
	if(Cunchu::m_sysCanshu.m_shuibengXielou)
	{
		if(PortSet::m_faZhuangtai[0]==0&&PortSet::m_faZhuangtai[1]==0&&(HuanjingShuju::m_shuju[160+32-140]>Cunchu::m_sysCanshu.m_shuibengXielou))//罐区阀全全关，清水泵大于泄露值
		{
			b1=true;
			baohuZhuangtai|=(1<<6);
		}
		else
			baohuZhuangtai&=~(1<<6);
	}else
		baohuZhuangtai&=~(1<<6);
	PortSet::setBitOut(Cunchu::m_sysCanshu.m_baojingNO,b1);

	//肥泵空转保护，8个霍尔流量计为0并且水泵也为0的情况下，关肥泵
	bool b=false;
	if(HuanjingShuju::m_shuju[32+160-140]==0&&Cunchu::m_sysCanshu.m_maichongliuliangXielou)//&&(PortSet::m_faZhuangtai[5]&(1<<1)))
	{
		for(int i=0;i<8;i++)
		{
			if(	HuanjingShuju::m_shuju[32+i]>Cunchu::m_sysCanshu.m_maichongliuliangXielou)
			{
				b=true;
				break;
			}
		}
	}
	if(b){
		DoFeibeng(false);
		baohuZhuangtai|=(1<<7);
	}else
		baohuZhuangtai&=~(1<<7);
}
void Device::runCelvBaohu()
{
	return;
	//EC保护
	if(Cunchu::m_sysCanshu.m_EC_H&&HuanjingShuju::m_shuju[156+32-140]>Cunchu::m_sysCanshu.m_EC_H)
		baohuZhuangtai|=(1<<2);
	else
		baohuZhuangtai&=~(1<<2);

	if(Cunchu::m_sysCanshu.m_EC_Huifu&&HuanjingShuju::m_shuju[156+32-140]<Cunchu::m_sysCanshu.m_EC_Huifu)
		baohuZhuangtai|=(1<<3);
	else
		baohuZhuangtai&=~(1<<3);
	//PH保护
	if(Cunchu::m_sysCanshu.m_PH_L&&HuanjingShuju::m_shuju[158+32-140]<Cunchu::m_sysCanshu.m_PH_L)
		baohuZhuangtai|=(1<<4);
	else
		baohuZhuangtai&=~(1<<4);

	if(Cunchu::m_sysCanshu.m_PH_Huifu&&HuanjingShuju::m_shuju[158+32-140]>Cunchu::m_sysCanshu.m_PH_Huifu)
		baohuZhuangtai|=(1<<5);
	else
		baohuZhuangtai&=~(1<<5);

}
