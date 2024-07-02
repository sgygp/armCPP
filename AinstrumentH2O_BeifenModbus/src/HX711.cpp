#include"HX711.h"
#include "delay.h"
void Hx711::init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(HX711_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = HX711_DO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(HX711_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = HX711_CLK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HX711_PORT, &GPIO_InitStructure);
}
void Hx711::WriteHX711_CLK(bool b)
{
	if (b)
		GPIO_SetBits(HX711_PORT, HX711_CLK); //模块没准备好时，单片机在CLK线上输出高电平
	else
		GPIO_ResetBits(HX711_PORT, HX711_CLK); //模块没准备好时，单片机在CLK线上输出低电平

}
u8 Hx711::ReadHx711_DATA()
{
	return GPIO_ReadInputDataBit(HX711_PORT, HX711_DO);
}

long Hx711::ReadHX711(u8 mode)//mode=0,1,2分别对应增益是32 64 128
{
	u32 dd = 0;
	WriteHX711_CLK(false); //模块没准备好时，单片机在CLK线上输出低电平
	for (u32 i = 0; i < 10000; i++)
	{
		if (ReadHx711_DATA() == RESET)
			break;
		delay_us(1);
	}
	delay_us(1);

	for (u32 i = 0; i < 24; i++)
	{
		WriteHX711_CLK(true); //单片机在CLK线上输出高电平
		delay_us(1);
		dd = dd << 1;
		WriteHX711_CLK(false); //单片机在CLK线上输出低电平
		if (ReadHx711_DATA() == SET)
		{
			dd++;
		}
		delay_us(1);
	}
	int x=0;
	if(mode==0)//2个脉冲增益是32
		x=2;
	else if(mode==1)//3个脉冲增益是64
		x=3;
	else///////1个脉冲增益是128
		x=1;

	/////////////////以下增加3个脉冲是采样通道A增益64；2个是采样通道B增益32；1个是采样通道A增益128
	for (int i=0;i<x;i++)
	{
		WriteHX711_CLK(true);
		delay_us(1);
		WriteHX711_CLK(false); //单片机在CLK线上输出低电平
		delay_us(1);
	}
//	WriteHX711_CLK(true);
//	delay_us(1);
//	WriteHX711_CLK(false); //单片机在CLK线上输出低电平
//	delay_us(1);
//	WriteHX711_CLK(true);
//	delay_us(1);
//	WriteHX711_CLK(false); //单片机在CLK线上输出低电平
//	delay_us(1);
///////////////////////////////////////////////////////////////////////////////////////////
	if(dd>=0x800000)
		dd|=0xff000000;
	long lll=long(dd);
	return lll;
}
