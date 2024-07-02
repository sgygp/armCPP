#include "BJ.h"

// ----------------------------------------------------------------------------

void BJ::init() {
	// Enable GPIO Peripheral clock

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	PWR_BackupAccessCmd(ENABLE);/* 允许修改RTC和后备寄存器*/
	RCC_LSEConfig( RCC_LSE_OFF); /* 关闭外部低速时钟,可以用作普通IO*/
	BKP_TamperPinCmd(DISABLE); /* 关闭入侵检测功能,可以用作普通IO*/

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	PWR_BackupAccessCmd(DISABLE);/* 禁止修改RTC和后备寄存器*/



	// Configure pin in output push/pull mode
	GPIO_InitStructure.GPIO_Pin = PINS_BJ1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORTS_BJ1, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = PINS_BJ2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORTS_BJ2, &GPIO_InitStructure);

}

// ----------------------------------------------------------------------------
