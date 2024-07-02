/*
 * Device.h
 *
 *  Created on: 2020年10月13日
 *      Author: Administrator
 */

#ifndef ANTDATA_DEVICE_H_
#define ANTDATA_DEVICE_H_
#include "stm32f4xx_conf.h"
#include "PortSet.h"
#include "Cunchu.h"
#include "CalendarTimer.h"
#include "delay.h"

#define COMMANDLEN 10

#define YIZANTING				0
#define ZHENGZHUANZHONG			1
#define FANZHUANZHONG 			2
#define ZHENGZHUANWANCHENG		3
#define FANZHUANWANCHENG		4
#define FENDUANZHONG			5

#define _OFF_ 			0
#define _ON_			1

#define JIZHUANYANSHI 2//电机急转延时

class Device
{
public:
	u8 ID = 0;
	//以下存储
	/*	u8 inNo1;
		u8 inNo2;
		u8 inNo3;
		u8 outNo1;
		u8 outNo2;
		u8 m_ctrType; //0，手动，1时控，2温控，3光控

		u16 m_zhengzhuanShijian; //合拢时间
		u16 m_fanzhuanShijian; //平铺时间
		u16 m_zhengzhuanZhi; //合拢温度/光度
		u16 m_fanzhuanZhi; //平铺温度/光度
		u16 m_zhengzhuanSecs; //合拢时电机运行秒数
		u16 m_fanzhuanSecs; //平铺时电机运行秒数
		bool m_fanCtrl;//反向控制（正常是大于后正转或者打开，这个小于后是正转或打开）
		*/
	u8 m_tmpShujuNO;//选用哪个数据站，有效值为0-3（站1-站4）

	u8 m_zhuangtai=YIZANTING;
	short m_dongzuoTime = 0; //动作时间，大于零时是在执行zhengzhuan，小于0时是在执行fanzhuan
	int m_jitingTime=0; //正常是0，不起作用。负数时，代表从正转直接切换为反转，正数时反之。
	int m_curDuan=-1;
	bool m_bZhengzaiFenduan=false;
	

	static u16 KeyWord;
	static u16 m_command;
	static u16 m_commandKey[COMMANDLEN];
	static u8 m_commandR;
	static u8 m_commandW;
//	static u16 m_wendu;
//	static u16 m_shidu;
//	static u16 m_guangdu;
	static bool m_dataOK;
	
	static u8 zhuFengjiOut;

	static u16 getCommand()
	{
		if (m_commandR == m_commandW)
			{
				return 0;
			}
		u16 x = m_commandKey[m_commandR];
		m_commandR = (m_commandR + 1) % COMMANDLEN;
		return x;
	}
	static void putCommand(u16 command)
	{
		if ((m_commandW + 1) % COMMANDLEN == m_commandR)
			return;
		m_commandKey[m_commandW] = command;
		m_commandW = (m_commandW + 1) % COMMANDLEN;

	}
public:

	Device() = default;
	Device(u8 id, bool isDJ)
	{
		ID = id;
		isDianji = isDJ;
		oldTime = CalendarTimer::RTC_TimeStructure.RTC_Hours * 100
		          + CalendarTimer::RTC_TimeStructure.RTC_Minutes;
		m_zhuangtai = YIZANTING; //开始状态出于暂停
		m_jitingTime=0;
		m_curDuan=-1;

	}
	/*static void testOut() {
		for (u16 i = 0; i <= OUTSIZE; i++) {
			AntData_PortSet::outOnOff(i - 1, false);
			AntData_PortSet::outOnOff(i, true);
			Delay::delay_Xus(50000);
		}
	}*/
//	void readCanshu();
	void run();
	inline void stop()
	{
		m_zhuangtai = YIZANTING; //状态暂停
		m_dongzuoTime = 0;
	}
	bool getIsDianji()
	{
		return isDianji;
	}
private:
	bool isDianji = true;
	bool benciFenzhongyichuli=false;
	u16 oldTime = 0; //上次时间
	inline void jiTingToZheng()
	{
		stop();
		m_jitingTime = JIZHUANYANSHI;
	}
	inline void jiTingToFan()
	{
		stop();
		m_jitingTime = -JIZHUANYANSHI;
	}
	inline void zhengzhuan()
	{
		if (m_zhuangtai == FANZHUANZHONG)
			jiTingToZheng();
		else if(m_zhuangtai!=ZHENGZHUANZHONG)
//			m_dongzuoTime = Cunchu::m_Canshu[ID].m_zhengzhuanSecs;
			m_dongzuoTime = Cunchu::m_Canshu[ID].m_yunxingSecs;
	}
	inline void fanzhuan()
	{
		if (m_zhuangtai == ZHENGZHUANZHONG)
			jiTingToFan();
		else if(m_zhuangtai!=FANZHUANZHONG)
//			m_dongzuoTime = 0-Cunchu::m_Canshu[ID].m_fanzhuanSecs;
			m_dongzuoTime = 0-Cunchu::m_Canshu[ID].m_yunxingSecs;
	}
	inline void On()
	{
		m_dongzuoTime = 1;
	}
	inline void Off()
	{
		m_dongzuoTime = 0;
	}
	inline void OnOff()
	{
		m_dongzuoTime = !m_dongzuoTime;
	}
	void OnFenduan(bool jin);//本次是前进还是后退
	bool isKeyOn(u16 no);
	void runDianji(u8 runType);
	void runOnOff(u8 runType);
	void ChuliCommandDianji();
	void ChuliCommandOnOff();
	void setOutBit(u16 outNo, bool b);
	void fenDuanChuli(u16 shuju,bool fuluoji);
};

#endif /* ANTDATA_DEVICE_H_ */
