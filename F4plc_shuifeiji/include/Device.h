/*
 * Device.h
 *
 *  Created on: 2020��10��13��
 *      Author: Administrator
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_
#include "stm32f4xx_conf.h"
#include "PortSet.h"
#include "Cunchu.h"
#include "delay.h"
#include "FeiLiaoTong.h"
#define SHUIBENGQIANHOUJIANGE 10
#define DIANDONGFADONGZUOSHIJIAN 60
#define DIANDONGFAFANZHUANSHIJIAN 3
#define DIANCIFAYANCHIGUANSHIJIAN 10

#define COMMANDLEN 10

class Device
{
public:
	static u16 sysShebeiZhuangtai;
	static u16 baohuZhuangtai;
	static u16 yunXingShengyu;
	static u16 dangQianGuanQu;
	static u8 YuanKong;
	static u8 m_mode;//0�ֶ���1ʪ�ȣ�2��ʱ
	static u8 m_buzhou;//0����״̬��1����״̬���˲��������ˮ��������ʱ�䣩��2Ԥ��ȣ�3��ȣ��˲���򿪷�Ͱ�����������еĻ������򿪹���������������4��ȳ�ϴ�׶Ρ�
	static u8 cpuUsed;
	
	
	
	u32 m_leijiLiuliang=0;
	
	u16 KeyWord=0;
	u16 m_command=0;
	u16 m_commandKey[COMMANDLEN];
	u8 m_commandR=0;
	u8 m_commandW=0;

	

	bool m_dataOK=false;
	FeiLiaoTong m_feiliaotong[8];
	CelvCanshu m_curCelv;

	u8 m_guanQushu=0;//��������
	u8 m_curGuangaiCishu=1;
	u8 m_curBaifenbi=0;
	bool m_bShifei=false;
	bool m_bZanting=false;

	

	bool isBusy()
	{
		return m_buzhou;
	}
	void DoGuanqufa(u8 no,bool isON);
	void DoQingshuibeng(bool isON);
	void DoFeibeng(bool isON);
	void DoDiandongFa(bool isON);
	void runXielouJianceBaohu();


	u16 getCommand()
	{
		if (m_commandR == m_commandW)
			{
				return 0;
			}
		u16 x = m_commandKey[m_commandR];
		m_commandR = (m_commandR + 1) % COMMANDLEN;
		return x;
	}
	void putCommand(u16 command)//д����ʱ���ֽ�=0�������ָ��,���ֽ�ֵ0ֹͣ��2��ͣ��1������ǰ���ԣ�
												//���ֽ�1-59����59����/���е��ĸ������ֽ�ֵΪ0/1
												//���ֽ�60���ƹ���ģʽ�����ֽ�ֵΪ0/1/2��
	{
		if ((m_commandW + 1) % COMMANDLEN == m_commandR)
			return;
		if(command<10)//���command Ϊ0��ʱ��
			command++;
		m_commandKey[m_commandW] = command;
		m_commandW = (m_commandW + 1) % COMMANDLEN;

	}
public:
	CelvCanshu *celv;
	u32 m_secs=0;
	u32 m_mubiaoZhi=0;
	u16 oldTime=0;

	Device();
	void run();
	void doCelv(int no);
	inline void stop()
	{
		if(m_mode==1&&shiduGuangaiX>=0)//ʪ��ģʽ
			return;
		m_buzhou=0;
		for(int i=0;i<8;i++)
			m_feiliaotong[i].end();
		if(PortSet::m_faZhuangtai[5]&(1<<0))
		{
			DoQingshuibeng(false);
			m_secs=SHUIBENGQIANHOUJIANGE;//��ʱ10��
			m_buzhou=5;
		}else
		{
			DoQingshuibeng(false);
//			if(bZhunbeiShidumoshi)
//			{
//				m_buzhou=6;
//				bZhunbeiShidumoshi=false;
//			}
//			else
				m_buzhou=0;

		}
			
		 m_curGuangaiCishu=1;
		 m_bZanting=false;
		 shiduGuangaiX=-1;
	}
	inline void pause()
	{
		if(m_buzhou==0&&shiduGuangaiX>=0)
			return;
		m_bZanting=!m_bZanting;
		for(int i=0;i<8;i++)
			m_feiliaotong[i].pause(m_bZanting);
		if(m_bZanting)
		{
			buzhouSave=m_buzhou;
			for(int i=0;i<32;i++)
				DoGuanqufa(i,false);
			DoFeibeng(false);	
		}else
		{
			m_buzhou=buzhouSave;
		}
	}
	int shiduGuangaiX=-1;
private:
	void runCelvBaohu();
	void chuliCommand();
	void chuliDiandongfa();
	void setOutBit(u16 outNo, bool b);
	bool m_once=false;
	void doShoudongtingzhi();
	u8 buzhouSave=0;
	int getCurGuangaiNO();
	bool bShiduGuangai=false;
//	bool bZhunbeiShidumoshi=false;
	int getShiduKeyiGuangai();
	void runShidu();
	u8 runCelvBuzhou();
	int m_diandongfaJishi=0;
	int m_diandongfaFanzhuanYanshi=0;
	int m_diancifaJishi=0;
	bool m_diandongfaYanshihouShuchu=false;//true:ON;false:OFF
	void inline DiandongFaON()
	{
		PortSet::setBitOut(Cunchu::m_sysCanshu.m_dianDongfaON,1);
		PortSet::setBitOut(Cunchu::m_sysCanshu.m_dianDongfaOFF,0);
		PortSet::m_faZhuangtai[5]|=(1<<2);
		m_diandongfaJishi=DIANDONGFADONGZUOSHIJIAN;
		m_diancifaJishi=0;
		PortSet::setBitOut(Cunchu::m_sysCanshu.m_shuiyuanDiancifIO,1);//��ŷ�������
	}
	void inline DiandongFaOFF()
	{
		PortSet::setBitOut(Cunchu::m_sysCanshu.m_dianDongfaON,0);
		PortSet::setBitOut(Cunchu::m_sysCanshu.m_dianDongfaOFF,1);
		PortSet::m_faZhuangtai[5]&=(~(1<<2));
		m_diandongfaJishi=DIANDONGFADONGZUOSHIJIAN;
		m_diancifaJishi=DIANCIFAYANCHIGUANSHIJIAN;//��ŷ���ʱ

	}
	void inline DiandongFaPAUSE(bool next)
	{
		PortSet::setBitOut(Cunchu::m_sysCanshu.m_dianDongfaON,0);
		PortSet::setBitOut(Cunchu::m_sysCanshu.m_dianDongfaOFF,0);
		m_diandongfaJishi=0;
		m_diandongfaFanzhuanYanshi=DIANDONGFAFANZHUANSHIJIAN;
		m_diandongfaYanshihouShuchu=next;
	}

	void inline DiandongFaSTOP()
	{
		PortSet::setBitOut(Cunchu::m_sysCanshu.m_dianDongfaON,0);
		PortSet::setBitOut(Cunchu::m_sysCanshu.m_dianDongfaOFF,0);
		m_diandongfaJishi=0;
		m_diandongfaFanzhuanYanshi=0;
	}
};

#endif /* ANTDATA_DEVICE_H_ */
