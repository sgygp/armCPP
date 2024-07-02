/*
 * RTCwakeup.cpp
 *
 *  Created on: 2023��9��8��
 *      Author: Administrator
 */
//#include "diag/Trace.h"

#include "RTCwakeup.h"
#include "delay.h"
#include "BlinkLed.h"
#define WAKEMIN 30//����ʱ�䣬����

RTC_wakeup::RTC_wakeup() {
	// TODO �Զ����ɵĹ��캯�����

}

void RTC_wakeup::setWakeup() {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); //ʹ��PWR��BKPʱ��
	PWR_BackupAccessCmd(ENABLE); //ʹ�ܺ󱸼Ĵ�������
	//BKP_DeInit(); //��λ�󱸼Ĵ���
	RCC_LSICmd(ENABLE);						//ʹ���ڲ�����ʱ��
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI); //ѡ��LSI��ΪRTC��ʱ��Դ
	RCC_RTCCLKCmd(ENABLE); //ʹ��RTCʱ��
	RTC_WaitForSynchro(); //�ȴ�RTCͬ��
	RTC_WaitForLastTask(); //�ȴ����һ���������
	RTC_SetPrescaler(39999); //����RTCԤ��Ƶֵ  RTC period = RTCCLK/RTC_PR = (40.000 KHz)/(39999+1) = 1/s
	RTC_WaitForLastTask(); //�ȴ����һ���������

	RTC_SetCounter(0);//
	RTC_WaitForLastTask(); //�ȴ����һ���������
	RTC_SetAlarm(WAKEMIN*60);//���������ж�10s
//	RTC_SetAlarm(10);//���������ж�10s
	RTC_WaitForLastTask(); //�ȴ����һ���������
	PWR_BackupAccessCmd(DISABLE);



	/*��ʼǰ����������Ų����Է��󴥷�*/
	RTC_ClearFlag(RTC_FLAG_ALR);
	RTC_ClearITPendingBit(RTC_IT_ALR);//���RTC����A�ı�־
	EXTI_ClearITPendingBit(EXTI_Line17);//���LINE17�ϵ��жϱ�־λ


	/*
		 RTC��ʱ��Դ��3����
		 > LSE��32.768kHz��
		 > LSI��40kHz��
		 > HSE/128��
		 > ��������㣺Tsec=LSI/[(RTC_AsynchPrediv+1)*(RTC_SynchPrediv +1)]
	 */

	/*EXTI��17�����ӵ�RTC�����¼���,�������ж�Ҳ��������RTC�����ж�*/
//	EXTI_InitTypeDef  EXTI_InitStructure;
//	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//���õ��������ش���
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);


	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RTC_ITConfig(RTC_IT_ALR,ENABLE);//����RTC�����жϲ�ʹ��

}

void RTC_wakeup::setOFF() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR| RCC_APB1Periph_BKP,ENABLE);
	/* Enable Access To The RTC & Backup Registers */
	PWR_BackupAccessCmd(ENABLE);
	RTC_ITConfig(RTC_IT_ALR,DISABLE);//����RTC�����жϲ�ʹ��
	RCC_RTCCLKCmd(DISABLE); //ʹ��RTCʱ��
	delay_ms(10);
	PWR_BackupAccessCmd(DISABLE);
}

extern "C" void RTC_IRQHandler(void) {
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
		RTC_ClearITPendingBit(RTC_IT_ALR);
	EXTI_ClearITPendingBit(EXTI_Line17);
}
