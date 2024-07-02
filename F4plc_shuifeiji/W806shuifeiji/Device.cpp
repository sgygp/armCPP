/*
 * Device.cpp
 *
 *  Created on: 2020��10��13��
 *      Author: Administrator
 */
#include "CalendarTimer.h"
#include "Device.h"
#include "HuanjingShuju.h"


u16 Device::sysShebeiZhuangtai=0;
u16 Device::yunXingShengyu=0;
u16 Device::dangQianGuanQu=0;
u8 Device::YuanKong=0;
u8 Device::m_mode=0;//0�ֶ���1ʪ�ȣ�2��ʱ
u8 Device::m_buzhou=0;//0����״̬��1����״̬���˲��������ˮ��������ʱ�䣩��2Ԥ��ȣ�3��ȣ��˲���򿪷�Ͱ�����������еĻ������򿪹���������������4��ȳ�ϴ�׶Σ�5ˮ�ùر���ʱ
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
void Device::DoQingshuibeng(bool isON){//9��վ
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
			PortSet::setBitOut(Cunchu::m_sysCanshu.m_shuiyuanDiancifIO,0);//��ŷ��ر�

	}

}
void Device::doCelv(int no) {//no=-1;ִ�е�ǰ����
	if(m_buzhou)//����ִ�в���
		return;
	if(!YuanKong&&m_mode==1)//ʪ�ȿ���ģʽ
		return;
	if(no>15)//����ѡ�����
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
	if(command<10)//���commandΪ0�����
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
		if(m_mode!=0)//�����ֶ�ģʽ
			return;
		if(m_buzhou!=0)//���ǿ���ģʽ
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
		if(HuanjingShuju::m_shuju[shiduGuangaiX]>=Cunchu::m_sysCanshu.m_shiduEnd)//��ȴﵽ��������
		{
			int g=getShiduKeyiGuangai();
			if(g>=0)//������Ҫ��ȵ�
			{
				DoGuanqufa(shiduGuangaiX,false);//
				DoGuanqufa(g,true);//
			}
			shiduGuangaiX=g;

		}
	}
	if(bShiduGuangai)//�ѿ�ʼ���
	{
		if(shiduGuangaiX<0)//ȫ�ر�
		{
			bShiduGuangai=false;
//			bZhunbeiShidumoshi=true;
			stop();
		}else
			DoGuanqufa(shiduGuangaiX,true);//

	}else{ 
		if(shiduGuangaiX>=0)
		{//δ��ʼ,������Ҫ���
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
	case 1://������
		{
			x=0;
			for(int i=0;i<8;i++)//ͳ���Ƿ�ȫ����ˮ���
			{
				if(!m_feiliaotong[i].isJinshuiWancheng())
				{
					x=1;
					break;
				}
			}
			if(!x)//��ˮȫ�����
			{
				shengyu=0;
				for(int i=0;i<8;i++)
				{
					x=m_feiliaotong[i].getShengyuJiaoban();
					if(x>shengyu)
						shengyu=x;
				}
			}else//��ˮ��
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
			for(int i=0;i<8;i++)//ͳ���Ƿ��������
			{
				if(!m_feiliaotong[i].isPeiliaoWancheng())
				{
					x=1;
					break;
				}
			}
			if(!x)//�������
			{
				m_once=false;
				if(celv->m_xGuanqu==0)//�޹���ѡ��
				{
					DoQingshuibeng(false);//ˮ�ÿ����Ѵ򿪣��ر�һ��
					m_buzhou=0;
				}else
				{
					DoQingshuibeng(true);//�������������䣬ֻ�����ʱ���ǲ���ˮ��
					m_buzhou=2;
					DoDiandongFa(true);
					m_curGuangaiCishu=1;
				}

			}
		}
		break;
	case 2://Ԥ���
		{
			DoGuanqufa(getCurGuangaiNO(),true);
			if(!m_once)//Ԥ���δ��ʼ
			{
				m_once=true;
				if(celv->m_startGuangai==0)//Ԥ�������Ϊ1
					celv->m_startGuangai=1;

				if(celv->m_mode==0)//ʱ��ģʽ
					m_secs=celv->m_startGuangai*60;
				else//������ģʽ
					m_mubiaoZhi=m_leijiLiuliang+celv->m_startGuangai;

			}else
			{//Ԥ��ȼ���׶�
				bool b=false;
				if(celv->m_mode==0)//ʱ��ģʽ
				{
					shengyu=m_secs;
					if(!m_secs)
						b=true;
				}else//������ģʽ
				{
					shengyu=m_mubiaoZhi-m_leijiLiuliang;
					if(m_leijiLiuliang>=m_mubiaoZhi)
						b=true;
				}
				if(b)//Ԥ������
				{
					m_once=false;
					m_buzhou=3;
				}

			}
		}
		break;
	case 3://����м�׶�
		{
			DoGuanqufa(getCurGuangaiNO(),true);
			if(!m_once)//����м�׶�δ��ʼ
			{
				if(m_bShifei)//�����Ҫʩ��
				{
					DoFeibeng(true);
					if(celv->m_mode==0)//ʱ��ģʽ
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
						//��Ҫʩ�ʵķʷ�����
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

					if(celv->m_mode==0)//ʱ��ģʽ
						m_secs=ll*60;
					else//������ģʽ
						m_mubiaoZhi=m_leijiLiuliang+ll;

				}
				m_once=true;
			}else//����м����׶�
			{
				if(m_bShifei&&celv->m_mode!=0)//ʱ��ģʽ)//��Ҫʩ�ʵĻ�����ȫ��ʩ�����Ϊ��������
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
					if(x==0)//���
					{
						m_once=false;
						m_buzhou=4;

					}
				}else//�����ʩ�ʣ��Լ���Ϊ׼
				{
					bool b=false;

					if(celv->m_mode==0)//ʱ��ģʽ
					{
						shengyu=m_secs;
						if(!m_secs)
						{
							b=true;
							if(celv->m_endGuangai<1)
								celv->m_endGuangai=1;
							m_secs=celv->m_endGuangai*60;
						}
					}else//������ģʽ
					{
						shengyu=m_mubiaoZhi-m_leijiLiuliang;
						if(m_leijiLiuliang>=m_mubiaoZhi)
						{
							b=true;
							if(celv->m_endGuangai<1)//Ԥ�������Ϊ1
								celv->m_endGuangai=1;
							m_mubiaoZhi=m_leijiLiuliang+celv->m_endGuangai;

						}
					}
					if(b)//����м����
					{
						m_once=false;
						m_buzhou=4;
					}

				}

			}
		}
		break;
	case 4://��ϴ�׶�
		{
			DoGuanqufa(getCurGuangaiNO(),true);
			if(!m_once)//��ϴδ��ʼ
			{
				m_once=true;
				for(int i=0;i<32;i++)//��Ͱ��ȫ��
					m_feiliaotong[i].DoOut(false);
				if(celv->m_endGuangai==0)//Ԥ�������Ϊ1
					celv->m_endGuangai=1;
				if(celv->m_mode==0)//ʱ��ģʽ
					m_secs=celv->m_endGuangai*60;
				else//������ģʽ
					m_mubiaoZhi=m_leijiLiuliang+celv->m_endGuangai;
			}else
			{
				bool b=false;
				if(celv->m_mode==0)//ʱ��ģʽ
				{
					shengyu=m_secs;
					if(!m_secs)
						b=true;
				}else//������ģʽ
				{
					shengyu=m_mubiaoZhi-m_leijiLiuliang;
					if(m_leijiLiuliang>=m_mubiaoZhi)
						b=true;
				}
				if(b)//��ϴ���
				{
					m_once=false;
					if(m_curGuangaiCishu==FeiLiaoTong::m_guanGaiZongcishu)//��������һ���������
					{
						DoFeibeng(false);//�طʱ�
						DoQingshuibeng(false);
						m_secs=SHUIBENGQIANHOUJIANGE;//��ʱ10��
						m_buzhou=5;
					}else
					{
						DoGuanqufa(getCurGuangaiNO(),false);//�ر�����
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
				for(int i=0;i<32;i++)//������ȫ��
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
	if (runType > 3) //����ʶ�������ģʽʱ����Ϊ�ֶ�
		runType = 1;

	if(runType==2)//ʪ�ȿ���
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
		if(runType==3)//��ʱ����
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
		if(PortSet::m_faZhuangtai[0]==0&&PortSet::m_faZhuangtai[1]==0&&(HuanjingShuju::m_shuju[160+32-140]>Cunchu::m_sysCanshu.m_shuibengXielou))//������ȫȫ�أ���ˮ�ô���й¶ֵ
		{
			b1=true;
			baohuZhuangtai|=(1<<6);
		}
		else
			baohuZhuangtai&=~(1<<6);
	}else
		baohuZhuangtai&=~(1<<6);
	PortSet::setBitOut(Cunchu::m_sysCanshu.m_baojingNO,b1);

	//�ʱÿ�ת������8������������Ϊ0����ˮ��ҲΪ0������£��طʱ�
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
	//EC����
	if(Cunchu::m_sysCanshu.m_EC_H&&HuanjingShuju::m_shuju[156+32-140]>Cunchu::m_sysCanshu.m_EC_H)
		baohuZhuangtai|=(1<<2);
	else
		baohuZhuangtai&=~(1<<2);

	if(Cunchu::m_sysCanshu.m_EC_Huifu&&HuanjingShuju::m_shuju[156+32-140]<Cunchu::m_sysCanshu.m_EC_Huifu)
		baohuZhuangtai|=(1<<3);
	else
		baohuZhuangtai&=~(1<<3);
	//PH����
	if(Cunchu::m_sysCanshu.m_PH_L&&HuanjingShuju::m_shuju[158+32-140]<Cunchu::m_sysCanshu.m_PH_L)
		baohuZhuangtai|=(1<<4);
	else
		baohuZhuangtai&=~(1<<4);

	if(Cunchu::m_sysCanshu.m_PH_Huifu&&HuanjingShuju::m_shuju[158+32-140]>Cunchu::m_sysCanshu.m_PH_Huifu)
		baohuZhuangtai|=(1<<5);
	else
		baohuZhuangtai&=~(1<<5);

}
