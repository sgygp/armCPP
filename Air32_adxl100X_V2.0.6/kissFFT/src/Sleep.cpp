/*
 * Sleep.cpp
 *
 *  Created on: 2020年12月31日
 *      Author: Administrator
 */

#include "Sleep.h"
#include "air32f10x_conf.h"
#include "delay.h"

void Sleep::sleep() {

	//while(1);

	for(int i=0;i<1000;i++)//10秒
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET)
			break;
	}
//	RCC_AHBPeriphClockCmd(~0xFFFFFAA8, DISABLE); //关闭所有时钟(除电源时钟)&~RCC_APB1Periph_BKP
//	RCC_APB2PeriphClockCmd(~0xFFC00002, DISABLE); //关闭所有时钟(除电源时钟)&~RCC_APB1Periph_BKP
//	RCC_APB1PeriphClockCmd(~0x81013600 , DISABLE); //关闭所有时钟(除电源时钟)&~RCC_APB1Periph_BKP
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_WakeUpPinCmd(ENABLE);
    PWR_ClearFlag(PWR_FLAG_WU);
    PWR_ClearFlag(PWR_FLAG_SB);
    PWR_EnterSTANDBYMode();


/*	uint8_t i;

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
							   RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG,
						   ENABLE); //使能GPIO时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;		  //所有引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	  //模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz

	for (i = 0; i < (GPIOG_BASE - GPIOA_BASE) / 0x400; i++)
	{
		GPIO_Init((GPIO_TypeDef *)((APB2PERIPH_BASE + (i + 3) * 0x0400)), &GPIO_InitStructure); //初始化GPIO
	}

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; //所有引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);		//初始化GPIO

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_WakeUpPinCmd(ENABLE);
    PWR_ClearFlag(PWR_FLAG_WU);
    PWR_ClearFlag(PWR_FLAG_SB);
    PWR_EnterSTANDBYMode();
*/
}
