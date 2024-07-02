/*
 * AntLock.cpp
 *
 *  Created on: 2020年12月15日
 *      Author: Administrator
 */

#include "AntLock.h"
#include "Cunchu.h"
#include "CalendarTimer.h"
u32	Ant_Lock::m_secs = 0;
void Ant_Lock::run() {

	m_secs++;

	u32 cur = CalendarTimer::RTC_DataStructure.RTC_Year * 10000
			+ CalendarTimer::RTC_DataStructure.RTC_Month * 100 + CalendarTimer::RTC_DataStructure.RTC_Date;
	u32 uu = Cunchu::m_sysCanshu.m_riqiLock;
	u32 a = ((uu)&0x7f);
	a *= 10000;
	u32 set = a;
	a = (uu) >> 12;
	a *= 100;
	set += a;
	set += (((uu) >> 7) & 0x1f);

	//	if (getWordRiqi(RTC_DataStructure)>=*uu) {
	if (cur >= set) {
		if (m_secs >= 600) {
			while (1)
				IWDG_ReloadCounter();
		}
	}
}
/*
 void Ant_Lock::run() {
 u16 *uu = (u16*) &Cunchu::m_canshu1[42]; //42-43为运行天数，44-55共12字节，6个寄存器。56以后是设备参数了
 if (uu[0] == 999)
 return;
 if (uu[0] > 365)
 uu[0] = 365;
 m_secs++;

 if (uu[0] == 0) {
 if (m_secs >= 600) {
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, DISABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, DISABLE);
 while (1)
 IWDG_ReloadCounter();
 }
 } else {
 if (m_secs >= 3600 * 24) { //一天一存
 m_secs = 0;
 uu[0]--;
 Cunchu::WriteCunchu();
 }
 }
 }

 */
