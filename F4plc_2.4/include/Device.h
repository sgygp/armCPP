/*
 * Device.h
 *
 *  Created on: 2020��10��13��
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

#define JIZHUANYANSHI 2//�����ת��ʱ

class Device
{
public:
	u8 ID = 0;
	//���´洢
	/*	u8 inNo1;
		u8 inNo2;
		u8 inNo3;
		u8 outNo1;
		u8 outNo2;
		u8 m_ctrType; //0���ֶ���1ʱ�أ�2�¿أ�3���

		u16 m_zhengzhuanShijian; //��£ʱ��
		u16 m_fanzhuanShijian; //ƽ��ʱ��
		u16 m_zhengzhuanZhi; //��£�¶�/���
		u16 m_fanzhuanZhi; //ƽ���¶�/���
		u16 m_zhengzhuanSecs; //��£ʱ�����������
		u16 m_fanzhuanSecs; //ƽ��ʱ�����������
		bool m_fanCtrl;//������ƣ������Ǵ��ں���ת���ߴ򿪣����С�ں�����ת��򿪣�
		*/
	u8 m_tmpShujuNO;//ѡ���ĸ�����վ����ЧֵΪ0-3��վ1-վ4��

	u8 m_zhuangtai=YIZANTING;
	short m_dongzuoTime = 0; //����ʱ�䣬������ʱ����ִ��zhengzhuan��С��0ʱ����ִ��fanzhuan
	int m_jitingTime=0; //������0���������á�����ʱ���������תֱ���л�Ϊ��ת������ʱ��֮��
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
		m_zhuangtai = YIZANTING; //��ʼ״̬������ͣ
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
		m_zhuangtai = YIZANTING; //״̬��ͣ
		m_dongzuoTime = 0;
	}
	bool getIsDianji()
	{
		return isDianji;
	}
private:
	bool isDianji = true;
	bool benciFenzhongyichuli=false;
	u16 oldTime = 0; //�ϴ�ʱ��
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
	void OnFenduan(bool jin);//������ǰ�����Ǻ���
	bool isKeyOn(u16 no);
	void runDianji(u8 runType);
	void runOnOff(u8 runType);
	void ChuliCommandDianji();
	void ChuliCommandOnOff();
	void setOutBit(u16 outNo, bool b);
	void fenDuanChuli(u16 shuju,bool fuluoji);
};

#endif /* ANTDATA_DEVICE_H_ */
