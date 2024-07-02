/*
 * CalendarTimer.cpp
 *
 *  Created on: 2021��3��3��
 *      Author: Administrator
 */

#include "CalendarTimer.h"
RTC_TimeTypeDef CalendarTimer::RTC_TimeStructure;
RTC_DateTypeDef CalendarTimer::RTC_DataStructure;

void CalendarTimer::init() {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);/*ʹ�ܺ󱸼Ĵ�������*/

	if (RTC_ReadBackupRegister(RTC_BKP_DR19) != 0xA5A5) {
		/*���ñ��ݼĴ�������ʾ�Ѿ����ù�RTC*/
		SetCalendarTime(24,4,25,10,0,0);//24-4-25 10:0:0
		RTC_WriteBackupRegister(RTC_BKP_DR19, 0xA5A5);
	}
}
void CalendarTimer::SetCalendarTime(){
	RTC_InitTypeDef RTC_InitStructure;
	RCC_LSEConfig(RCC_LSE_ON);/*ʹ��LSE����*/
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)/*�ȴ�����*/
	{
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);/*ѡ��RTCʱ��Դ*/
	RCC_RTCCLKCmd(ENABLE);/*ʹ��RTCʱ��*/
	RTC_WaitForSynchro();/*�ȴ�RTC APB�Ĵ���ͬ��*/
	/*����RTC���ݼĴ����ͷ�Ƶ��*/
	RTC_InitStructure.RTC_AsynchPrediv = 0x7f;
	RTC_InitStructure.RTC_SynchPrediv = 0xff;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);
	RTC_SetDate(RTC_Format_BIN, &RTC_DataStructure);
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
}
void CalendarTimer::SetCalendarTime(u8 year,u8 mon,u8 day,u8 hour,u8 min,u8 sec){
	RTC_InitTypeDef RTC_InitStructure;
	RCC_LSEConfig(RCC_LSE_ON);/*ʹ��LSE����*/
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)/*�ȴ�����*/
	{
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);/*ѡ��RTCʱ��Դ*/
	RCC_RTCCLKCmd(ENABLE);/*ʹ��RTCʱ��*/
	RTC_WaitForSynchro();/*�ȴ�RTC APB�Ĵ���ͬ��*/
	/*����RTC���ݼĴ����ͷ�Ƶ��*/
	RTC_InitStructure.RTC_AsynchPrediv = 0x7f;
	RTC_InitStructure.RTC_SynchPrediv = 0xff;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);
	/*���������պ�����*/
	RTC_DataStructure.RTC_Year = year;
	RTC_DataStructure.RTC_Month = mon;
	RTC_DataStructure.RTC_Date = day;
	RTC_DataStructure.RTC_WeekDay = 5;
	RTC_SetDate(RTC_Format_BIN, &RTC_DataStructure);
	/*����ʱ���� ����ʾ��ʽ*/
	RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = hour;
	RTC_TimeStructure.RTC_Minutes = min;
	RTC_TimeStructure.RTC_Seconds = sec;
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);

}

void CalendarTimer::GetCalendarTime(){
	RTC_GetDate(RTC_Format_BIN, &RTC_DataStructure);
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
}
