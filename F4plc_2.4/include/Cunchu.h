/*
 * Cunchu.h
 *
 *  Created on: 2021年3月13日
 *      Author: Administrator
 */

#ifndef CUNCHU_H_
#define CUNCHU_H_
#include "W25X16.h"
#define __DEVICES__ 100
//#include "AntData_Device.h"
//#include <vector>
/*
 W25 存储结构说明
 参数占用第一个4K扇区，擦除的最小单位
总共16页，256字节一个页。
 第0-10页存放96个设备+1个公共参数
  每页存10个设备
   *
页擦除是擦除4K字节，每页256*16页
 */

typedef struct   //占用32个字节
{
	u16 qiyong;//是否启用
	u16 inNo1;//百位代表站号，十个位代表哪个输入点（从1开始）
	u16 inNo2;//百位代表站号，十个位代表哪个输入点（从1开始）
	u16 inNo3;//百位代表站号，十个位代表哪个输入点（从1开始）
	u16 outNo1;//百位代表站号，十个位代表哪个输出点（从1开始）
	u16 outNo2;//百位代表站号，十个位代表哪个输出点（从1开始）
	u16 m_ctrType;///高字节代表是否为负逻辑（只影响温控跟光控模式），低字节0，手动，1时控，2温控，3光控
	u16 m_huancaiNO;//0-4 使用哪个站数据 -1跟随系统参数
	u16 m_zhengzhuanShijian;//写此寄存器时，写的是小时*100+分钟
	u16 m_fanzhuanShijian;//写此寄存器时，写的是小时*100+分钟
	u16 m_gaoShuju;
	u16 m_diShuju;
	u16 m_yunxingSecs;
	u16 m_fenduan;
	u16 m_tmpZhuangtai;//写无效
	u16 m_tmpZhengzaiyunxingSec;//写无效
} device_Canshu;

typedef struct   //占用32个字节
{
	u16 m_addr;//作为从站时，本机地址为1
	u16 m_qiyongZhanhao;//开启第一路 环境数据采集 bit0-4分别代表是否开启1-5站,默认只开站1
	u16 m_huancaiAddr0;//环境数据站1地址
	u16 m_huancaiAddr1;
	u16 m_huancaiAddr2;
	u16 m_huancaiAddr3;
	u16 m_huancaiAddr4;
	u16 m_huancaiNO;//0-4 默认使用站1数据
	u16 m_caijiTime; //5秒采集1次
	u16 m_yuxue;//雨雪开关输入序号，0不启用
	u16 m_gensuiFengji;//水泵跟随风机数，0都不跟随。
	u16 m_riqiLock;
	u16 m_fenduanJiange;
	u16 m_beiyongU16_2;
	u16 m_beiyongU16_3;
	u16 m_FLAG;//0X55AA
} sys_Canshu;

class Cunchu
{
public:
	Cunchu();
public:
	static u8 cpuUsed;
	static u16 benjiZhuangtai;

	static u8 YuanKong;
	static device_Canshu m_Canshu[__DEVICES__];//100个设备，是32字节结构
	static sys_Canshu  m_sysCanshu;
public :
	static void WriteCunchu(u32 shoudizhi);
	static void ReadCunchu(u32 shoudizhi);
	static void MorenCanshu();
	static void write();
	static void read();
	static void save();
	static void load();
};

#endif /* CUNCHU_H_ */
