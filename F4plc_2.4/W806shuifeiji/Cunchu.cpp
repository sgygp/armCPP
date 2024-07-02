/*
 * Cunchu.cpp
 *
 *  Created on: 2021年3月13日
 *      Author: Administrator
 */

#include "Cunchu.h"
#include "string.h"
u8 Cunchu::cpuUsed=0;
u16 Cunchu::benjiZhuangtai=0;
u8 Cunchu::YuanKong=0;
device_Canshu Cunchu::m_Canshu[__DEVICES__];//100个设备+1个系统参数（最后一个设备参数转为系统参数，都是24字节结构）
sys_Canshu Cunchu::m_sysCanshu;
Cunchu::Cunchu()
{
	// TODO 自动生成的构造函数存根

}
void Cunchu::WriteCunchu(u32 shoudizhi)
{
//	W25X16::SPI_FLASH_madeID();
	W25X16::SPI_FLASH_SectorErase(shoudizhi);
	int i;
	for(i=0; i<(__DEVICES__/8); i++)
		{
			memcpy(W25X16::spi1_tx + 4,(u8*) &m_Canshu[8*i], 256);
			W25X16::SPI_FLASH_PageWriteData(shoudizhi+256*i, 256);
		}
	u8 len=32*(__DEVICES__%8);
	memcpy(W25X16::spi1_tx + 4,(u8*) &m_Canshu[8*i], len);
	memcpy(W25X16::spi1_tx + 4+len,(u8*) &m_sysCanshu, 32);
	W25X16::SPI_FLASH_PageWriteData(shoudizhi+256*i, len+32);
}
void Cunchu::ReadCunchu(u32 shoudizhi)
{
	int i;
	for(i=0; i<(__DEVICES__/8); i++)
		{
			W25X16::SPI_FLASH_PageReadData(shoudizhi+256*i, 256);
			memcpy((u8*) &m_Canshu[8*i], W25X16::spi1_tx + 4, 256);
		}

	u8 len=32*(__DEVICES__%8);
	W25X16::SPI_FLASH_PageReadData(shoudizhi+256*i, len+32);
	memcpy((u8*) &m_Canshu[8*i],W25X16::spi1_tx + 4, len);
	memcpy((u8*) &m_sysCanshu,W25X16::spi1_tx + 4+len, 32);



	if(m_sysCanshu.m_FLAG!=0x55aa)
		MorenCanshu();

}
void Cunchu::MorenCanshu()
{
	for (int i = 0; i < __DEVICES__; i++)
		{
			m_Canshu[i].qiyong=0;//默认不启用
			m_Canshu[i].inNo1=0;
			m_Canshu[i].inNo2=0;
			m_Canshu[i].inNo3=0;
			m_Canshu[i].outNo1=0;
			m_Canshu[i].outNo2=0;
			m_Canshu[i].m_ctrType=0;//0手动，1，时控，2温控，3光控
			m_Canshu[i].m_huancaiNO=-1;//0-1使用单独设置环采，-1跟随系统

			m_Canshu[i].m_zhengzhuanShijian=600;//6:00
			m_Canshu[i].m_fanzhuanShijian=1800;//18:00
			m_Canshu[i].m_gaoShuju=300;//30度
			m_Canshu[i].m_diShuju=200;//20度
			m_Canshu[i].m_yunxingSecs=100;//运行时长
			m_Canshu[i].m_fenduan=0;//分段数
			m_Canshu[i].m_tmpZhuangtai=0;
			m_Canshu[i].m_tmpZhengzaiyunxingSec=0;

		}

	m_sysCanshu.m_addr=1;
	m_sysCanshu.m_qiyongZhanhao=1;
	m_sysCanshu.m_huancaiAddr0=1;//环境数据站1地址
	m_sysCanshu.m_huancaiAddr1=2;//环境数据站2地址
	m_sysCanshu.m_huancaiAddr2=3;//环境数据站3地址
	m_sysCanshu.m_huancaiAddr3=4;//环境数据站4地址
	m_sysCanshu.m_huancaiAddr4=5;//环境数据站5地址
	m_sysCanshu.m_huancaiNO=0;//使用站1数据
	m_sysCanshu.m_caijiTime=5;
	m_sysCanshu.m_yuxue=0;//不启用
	m_sysCanshu.m_gensuiFengji=0;//不跟随
	m_sysCanshu.m_riqiLock=0x10e3; //1月1日2099年
	m_sysCanshu.m_fenduanJiange=0;
	m_sysCanshu.m_beiyongU16_2=0;
	m_sysCanshu.m_beiyongU16_3=0;
	m_sysCanshu.m_FLAG=0X55AA;

	WriteCunchu(0);
//	NVIC_SystemReset();
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
