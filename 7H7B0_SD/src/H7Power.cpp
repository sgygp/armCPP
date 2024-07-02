/*
 * H7Power.cpp
 *
 *  Created on: 2024��4��9��
 *      Author: Administrator
 */

#include "H7Power.h"
#include "string.h"
RTC_HandleTypeDef H7Power::hrtc;
uint8_t backup_buf[128]={0};

void H7Power::init(int sleepSec) {
	//����ʹ�ܱ�����
	HAL_PWR_EnableBkUpAccess();
	//���²��Ա��ݼĴ���
	/*
		HAL_PWREx_EnableBkUpReg(); //ʹ�ܱ��ݼĴ���
		HAL_RTCEx_BKUPWrite(&hrtc,30,0x1234);//�ܹ�32���Ĵ���

		static volatile u32 a=HAL_RTCEx_BKUPRead(&hrtc,30);
		a++;
		 */
	//���²��Ա���RAM
/*	__HAL_RCC_BKPRAM_CLK_SLEEP_DISABLE();
	__HAL_RCC_BKPRAM_CLK_ENABLE();
	u32 addr=SRD_BKPSRAM_BASE;

	for(int i=0;i<128;i++)
	{
		*(u8 *)(addr+i)=i*2;
	}

	for(int i=0;i<128;i++)
	{
		backup_buf[i]=*(u8 *)(addr+i);
	}
*/


	memset(&hrtc,0,sizeof(hrtc));
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}

/*���Բ�����������
	RTC_DateTypeDef date;
	memset(&date,0,sizeof(date));
	RTC_TimeTypeDef time;
	memset(&time,0,sizeof(time));
	date.Date=9;
	date.Month=4;
	date.Year=24;
	date.WeekDay=RTC_WEEKDAY_SUNDAY;
	time.Hours=23;
	time.Minutes=59;
	time.Seconds=59;
	HAL_RTC_SetTime(&hrtc,&time,RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc,&date,RTC_FORMAT_BIN);
*/

	if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 20000, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)//wakeup time  = 20000 *(16/32000) = 10s
	{
		Error_Handler();
	}
	//��������������жϱ�־������һ���������������
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	//���Բ��ÿ��ж�
//	HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0, 1);
//	HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);

	GPIO_InitTypeDef gpio_init_struct;

	gpio_init_struct.Pin = GPIO_PIN_0; /* WKUP���� */
	gpio_init_struct.Mode = GPIO_MODE_IT_RISING; /* �ж�,������ */
	gpio_init_struct.Pull = GPIO_PULLDOWN; /* ���� */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /* ����*/
	HAL_GPIO_Init(GPIOA, &gpio_init_struct); /* WKUP���ų�ʼ�� */
	//���Բ��ÿ��ж�
//	HAL_NVIC_SetPriority(EXTI0_IRQn,2,2); /* ��ռ���ȼ�2�������ȼ�2 */
//	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_PWR_EnableWakeUpPin(GPIO_PIN_0);








}

u32 H7Power::getWakeUpTimer() {
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	//�������ѡ���wakeup�жϱ�־λ��������ܴ��������ж�
	HAL_RTC_GetTime(&hrtc,&time,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);

	return HAL_RTCEx_GetWakeUpTimer(&hrtc)+time.Seconds+date.Month;
}

extern "C" void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}


extern "C" void  HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_RTC_CLK_ENABLE();

	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct ;
	memset(&PeriphClkInitStruct,0,sizeof(PeriphClkInitStruct));

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Peripheral clock enable */
	__HAL_RCC_RTC_ENABLE();
	//__HAL_RCC_SYSCFG_CLK_ENABLE();

}
extern "C" void  HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_RTC_CLK_DISABLE();

}
