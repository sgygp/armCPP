/*
 * RTCwakeup.cpp
 *
 *  Created on: 2023年9月8日
 *      Author: Administrator
 */
//#include "diag/Trace.h"

#include "RTCwakeup.h"
#include "delay.h"
#include "BlinkLed.h"
#define WAKEMIN 30//唤醒时间，分钟

RTC_wakeup::RTC_wakeup() {
	// TODO 自动生成的构造函数存根

}

void RTC_wakeup::setWakeup() {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //使能PWR和BKP时钟
	PWR_BackupAccessCmd(ENABLE); //使能后备寄存器访问
	//BKP_DeInit(); //复位后备寄存器
	RCC_LSICmd(ENABLE);						//使能内部低速时钟
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI); //选择LSI作为RTC的时钟源
	RCC_RTCCLKCmd(ENABLE); //使能RTC时钟
	RTC_WaitForSynchro(); //等待RTC同步
	RTC_WaitForLastTask(); //等待最后一个操作完成
	RTC_SetPrescaler(39999); //设置RTC预分频值  RTC period = RTCCLK/RTC_PR = (40.000 KHz)/(39999+1) = 1/s
	RTC_WaitForLastTask(); //等待最后一个操作完成

	RTC_SetCounter(0);//
	RTC_WaitForLastTask(); //等待最后一个操作完成
	RTC_SetAlarm(WAKEMIN*60);//开启闹钟中断10s
//	RTC_SetAlarm(10);//开启闹钟中断10s
	RTC_WaitForLastTask(); //等待最后一个操作完成
	PWR_BackupAccessCmd(DISABLE);



	/*开始前，先清除干扰残留以防误触发*/
	RTC_ClearFlag(RTC_FLAG_ALR);
	RTC_ClearITPendingBit(RTC_IT_ALR);//清除RTC闹钟A的标志
	EXTI_ClearITPendingBit(EXTI_Line17);//清除LINE17上的中断标志位


	/*
		 RTC的时钟源有3个：
		 > LSE（32.768kHz）
		 > LSI（40kHz）
		 > HSE/128。
		 > 秒输出计算：Tsec=LSI/[(RTC_AsynchPrediv+1)*(RTC_SynchPrediv +1)]
	 */

	/*EXTI线17是连接到RTC闹钟事件的,配置其中断也就是配置RTC闹钟中断*/
//	EXTI_InitTypeDef  EXTI_InitStructure;
//	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//采用的是上升沿触发
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);


	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RTC_ITConfig(RTC_IT_ALR,ENABLE);//配置RTC闹钟中断并使能

}

void RTC_wakeup::setOFF() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR| RCC_APB1Periph_BKP,ENABLE);
	/* Enable Access To The RTC & Backup Registers */
	PWR_BackupAccessCmd(ENABLE);
	RTC_ITConfig(RTC_IT_ALR,DISABLE);//配置RTC闹钟中断并使能
	RCC_RTCCLKCmd(DISABLE); //使能RTC时钟
	delay_ms(10);
	PWR_BackupAccessCmd(DISABLE);
}

extern "C" void RTC_IRQHandler(void) {
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
		RTC_ClearITPendingBit(RTC_IT_ALR);
	EXTI_ClearITPendingBit(EXTI_Line17);
}
