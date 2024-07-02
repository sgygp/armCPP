/*
 * H7Power.cpp
 *
 *  Created on: 2024年4月9日
 *      Author: Administrator
 */

#include "H7Power.h"
#include "string.h"
RTC_HandleTypeDef H7Power::hrtc;
uint8_t backup_buf[128]={0};

void H7Power::init(int sleepSec) {
	//首先使能备份域
	HAL_PWR_EnableBkUpAccess();
	//以下测试备份寄存器
	/*
		HAL_PWREx_EnableBkUpReg(); //使能备份寄存器
		HAL_RTCEx_BKUPWrite(&hrtc,30,0x1234);//总共32个寄存器

		static volatile u32 a=HAL_RTCEx_BKUPRead(&hrtc,30);
		a++;
		 */
	//以下测试备份RAM
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

/*可以不用配置日期
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
	//必须先清除闹钟中断标志，否则一进入待机立即唤醒
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	//可以不用开中断
//	HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0, 1);
//	HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);

	GPIO_InitTypeDef gpio_init_struct;

	gpio_init_struct.Pin = GPIO_PIN_0; /* WKUP引脚 */
	gpio_init_struct.Mode = GPIO_MODE_IT_RISING; /* 中断,上升沿 */
	gpio_init_struct.Pull = GPIO_PULLDOWN; /* 下拉 */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /* 高速*/
	HAL_GPIO_Init(GPIOA, &gpio_init_struct); /* WKUP引脚初始化 */
	//可以不用开中断
//	HAL_NVIC_SetPriority(EXTI0_IRQn,2,2); /* 抢占优先级2，子优先级2 */
//	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_PWR_EnableWakeUpPin(GPIO_PIN_0);








}

u32 H7Power::getWakeUpTimer() {
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	//这里必须选清除wakeup中断标志位，否则可能触发不了中断
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
