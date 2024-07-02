/*
 * Cunchu.h
 *
 *  Created on: 2021年3月13日
 *      Author: Administrator
 */

#ifndef CUNCHU_H_
#define CUNCHU_H_
#include "W25X16.h"
/*
 W25 存储结构说明
 参数占用第一个4K扇区，擦除的最小单位
总共16页，256字节一个页。
 第0-10页存放96个设备+1个公共参数
  每页存10个设备
   *
页擦除是擦除4K字节，每页256*16页
 */
typedef struct   //4字节*8=32
{
	u8 m_bQiyong=0;//是否启用
	u8 m_hour=0;
	u8 m_fen=0;
	u8 m_celvNO=0;//选用的策略号
} DingshiFangan;

typedef struct   //188
{
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
	//70+6+6+6+6+6
	
	u16 m_shuibengYaliSet;
	u16 m_shuibengYaliDixian;
	u16 m_feibengYaliSet;
	u16 m_muyeRongliangBaohu;
	u16 m_yalichuanganqiLiangcheng=2.0;
	u16 m_maichongLiuliangjiMeishengGeshu=450;
	
	u16 m_EC_H=0;
	u16 m_EC_Huifu=0;
	u16 m_PH_Huifu=0;
	u16 m_PH_L=0;
	u16 m_D=1;//桶直径，cm
	u16 m_tongdiH=0.2;//桶锥底高度

	u16 m_shuibengXielou;
	u16 m_maichongliuliangXielou;
	u16 m_bShuibengBaoya=0;
	u16 m_shuiyuanDiancifIO=0;
	u16 m_sysBeiyong2=0;
	u16 m_sysBeiyong3=0;
	
	
	
	u32 m_shiduGuanQu;
	u16 m_shiduStart;
	u16 m_shiduEnd;
	
	u16 m_curCelvNO=0;
	u16 m_addr=1;
	u16 m_riqiLock=0;
	u16 m_FLAG;//0X55AA;
	
} sys_Canshu;

typedef struct  //128字节*16=2048
{
	u8 m_mode=0;//0:按时间；1:按流量
	u8 m_xTong=0;//选择了哪些肥料桶
	u8 m_beiyong1=0;
	u8 m_beiyong2=0;
	u32 m_xGuanqu;//选择了哪些罐区
	
	u16 m_jinshuiLiang[8];//
	short m_shifeiLiang[8];//按流量时，每个桶需要用多少流量，要平均到所选择的罐区中，-1为当前全部
	u16 m_jiaobanTime=0;
	u16 m_zongGuangai;
	u16 m_startGuangai;
	u16 m_endGuangai;
} CelvCanshu;


class Cunchu
{
public:
	Cunchu();
public:
	static sys_Canshu m_sysCanshu;
	static DingshiFangan m_dingshiCanshu[8];
	static CelvCanshu  m_celvCanshu[16];
public :
	static void WriteCunchu(u32 shoudizhi);
	static void ReadCunchu(u32 shoudizhi);
	static void MorenCanshu();
	static void write();
	static void read();
	static void save();
	static void load();
	static u8 getZhanShu();
};

#endif /* CUNCHU_H_ */
