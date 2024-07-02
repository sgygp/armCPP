/*
 * Cunchu.cpp
 *
 *  Created on: 2021年3月13日
 *      Author: Administrator
 */

#include "Cunchu.h"
#include "string.h"
sys_Canshu Cunchu::m_sysCanshu;
DingshiFangan Cunchu::m_dingshiCanshu[8];
CelvCanshu  Cunchu::m_celvCanshu[16];
Cunchu::Cunchu()
{
	// TODO 自动生成的构造函数存根

}
void Cunchu::WriteCunchu(u32 shoudizhi)
{
//	W25X16::SPI_FLASH_madeID();
	W25X16::SPI_FLASH_SectorErase(shoudizhi);
	memset(W25X16::spi1_tx + 4,0, 256);
	int len=sizeof(m_sysCanshu);
	memcpy(W25X16::spi1_tx + 4,(u8*) &m_sysCanshu, len);//<220
	memcpy(W25X16::spi1_tx + 4+220,(u8*) &m_dingshiCanshu, 4*8);//32
	W25X16::SPI_FLASH_PageWriteData(shoudizhi, 256);
	
	len=sizeof(CelvCanshu);
	for(int i=0;i<8;i++)//2048字节
	{
		memcpy(W25X16::spi1_tx + 4+128*(i),(u8*) &m_celvCanshu[2*i], len);
		memcpy(W25X16::spi1_tx + 4+128*(i+1),(u8*) &m_celvCanshu[2*i+1], len);
		W25X16::SPI_FLASH_PageWriteData(shoudizhi+256+256*i, 256);
	
	}
}
void Cunchu::ReadCunchu(u32 shoudizhi)
{
	int len=sizeof(m_sysCanshu);
	W25X16::SPI_FLASH_PageReadData(shoudizhi, 256);
	memcpy((u8*) &m_sysCanshu, W25X16::spi1_tx + 4,len);//<220
	memcpy((u8*) &m_dingshiCanshu,W25X16::spi1_tx + 4+220, 4*8);//32

	len=sizeof(CelvCanshu);
	for(int i=0;i<8;i++)//2048字节
	{
		W25X16::SPI_FLASH_PageReadData(shoudizhi+256+256*i, 256);
		memcpy((u8*) &m_celvCanshu[2*i], W25X16::spi1_tx + 4+128*(i),len);
		memcpy((u8*) &m_celvCanshu[2*i+1], W25X16::spi1_tx + 4+128*(i+1),len);

	}

	if(m_sysCanshu.m_FLAG!=0x55aa)
		MorenCanshu();

}
void Cunchu::MorenCanshu()
{
	memset((u8*)&m_dingshiCanshu,0,32);
	memset((u8*)&m_celvCanshu,0,sizeof(CelvCanshu)*16);
	for (int i = 0; i < 16; i++)
	{
		m_celvCanshu[i]. m_xTong=i;//选择了哪些肥料桶
	}
	
	memset((u8*)&m_sysCanshu,0,140);
	for (int i = 0; i < 8; i++)
	{
		m_sysCanshu.m_maichongLiuliangNO[i]=i+1;//选择了哪些肥料桶
	}
/*	
	u16 m_timeNet=0;//1为网络对时
	u16 m_baojingNO=0;
	u16 m_maichongLiuliangNO[8]={0};
	u16 m_tongJinshuiNO[8]={0};
	u16 m_tongJiaobanNO[8]={0};
	u16 m_tongOutFaNO[8]={0};
	u16 m_qingshuiBengNO=0;
	u16 m_feiBengNO=0;
	u16 m_dianDongfaON=0;
	u16 m_dianDongfaOFF=0;
	u16 m_guanquFaNO[32]={0};
*/
	
	m_sysCanshu.m_shuibengYaliSet=0.4*100;//MPa 2位小数
	m_sysCanshu.m_shuibengYaliDixian=0.1*100;
	m_sysCanshu.m_feibengYaliSet=0.3*100;
	m_sysCanshu.m_muyeRongliangBaohu=0.4*100;
	m_sysCanshu.m_yalichuanganqiLiangcheng=2.0*100;//米 2位小数
	m_sysCanshu.m_maichongLiuliangjiMeishengGeshu=450;
	
	m_sysCanshu.m_EC_H=5.0*10;//1位小数
	m_sysCanshu.m_EC_Huifu=2.0*10;
	m_sysCanshu.m_PH_Huifu=6.5*10;
	m_sysCanshu.m_PH_L=6.0*10;
	m_sysCanshu.m_D=100;//桶直径cm
	m_sysCanshu.m_tongdiH=10;//桶锥底高度
	
	m_sysCanshu.m_shuibengXielou=0;
	m_sysCanshu.m_maichongliuliangXielou=0;
	m_sysCanshu.m_bShuibengBaoya=0;
	m_sysCanshu.m_shuiyuanDiancifIO=0;
	m_sysCanshu.m_sysBeiyong2=0;
	m_sysCanshu.m_sysBeiyong3=0;
	
	m_sysCanshu.m_shiduGuanQu=0;
	m_sysCanshu.m_shiduStart=400;//40.0%
	m_sysCanshu.m_shiduEnd=800;//80.0%
	
	m_sysCanshu.m_curCelvNO=0;
	m_sysCanshu.m_addr=1;
	m_sysCanshu.m_FLAG=0X55AA;
	

	WriteCunchu(0);
}

void Cunchu::write()
{
	Cunchu::WriteCunchu(0);
}

void Cunchu::read()
{
	Cunchu::ReadCunchu(0);
}

void Cunchu::save()
{
	Cunchu::WriteCunchu(4096);
}

void Cunchu::load()
{
	Cunchu::ReadCunchu(4096);
	Cunchu::WriteCunchu(0);
//	NVIC_SystemReset();
}
u8 Cunchu::getZhanShu()
{
	u16 zhanshu=0;
    if(Cunchu::m_sysCanshu. m_baojingNO> zhanshu)
		zhanshu = Cunchu::m_sysCanshu.m_baojingNO;
	 for(int i = 0; i < 8; i++)//输入通道固定在主站，故不做判断
	{
	    if(Cunchu::m_sysCanshu.m_tongJinshuiNO[i] > zhanshu)
			zhanshu = Cunchu::m_sysCanshu.m_tongJinshuiNO[i] ;
	    if(Cunchu::m_sysCanshu.m_tongJiaobanNO[i] > zhanshu)
			zhanshu = Cunchu::m_sysCanshu.m_tongJiaobanNO[i] ;
	    if(Cunchu::m_sysCanshu.m_tongOutFaNO[i] > zhanshu)
			zhanshu = Cunchu::m_sysCanshu.m_tongOutFaNO[i] ;
	}
     for(int i = 0; i < 32; i++)//输入通道固定在主站，故不做判断
	{
	    if(Cunchu::m_sysCanshu.m_guanquFaNO[i] > zhanshu)
			zhanshu = Cunchu::m_sysCanshu.m_guanquFaNO[i] ;
	}
    if(Cunchu::m_sysCanshu. m_qingshuiBengNO> zhanshu)
		zhanshu = Cunchu::m_sysCanshu.m_qingshuiBengNO;
    if(Cunchu::m_sysCanshu. m_feiBengNO> zhanshu)
		zhanshu = Cunchu::m_sysCanshu.m_feiBengNO;
    if(Cunchu::m_sysCanshu. m_dianDongfaON> zhanshu)
		zhanshu = Cunchu::m_sysCanshu.m_dianDongfaON;
    if(Cunchu::m_sysCanshu. m_dianDongfaOFF> zhanshu)
		zhanshu = Cunchu::m_sysCanshu.m_dianDongfaOFF;
	
   zhanshu /= 100;

	return (u8)zhanshu;
}
