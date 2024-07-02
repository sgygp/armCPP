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
		GPIO_SetBits(HX711_PORT, HX711_CLK); //ģ��û׼����ʱ����Ƭ����CLK��������ߵ�ƽ
	else
		GPIO_ResetBits(HX711_PORT, HX711_CLK); //ģ��û׼����ʱ����Ƭ����CLK��������͵�ƽ

}
u8 Hx711::ReadHx711_DATA()
{
	return GPIO_ReadInputDataBit(HX711_PORT, HX711_DO);
}

long Hx711::ReadHX711(u8 mode)//mode=0,1,2�ֱ��Ӧ������32 64 128
{
	u32 dd = 0;
	WriteHX711_CLK(false); //ģ��û׼����ʱ����Ƭ����CLK��������͵�ƽ
	for (u32 i = 0; i < 10000; i++)
	{
		if (ReadHx711_DATA() == RESET)
			break;
		delay_us(1);
	}
	delay_us(1);

	for (u32 i = 0; i < 24; i++)
	{
		WriteHX711_CLK(true); //��Ƭ����CLK��������ߵ�ƽ
		delay_us(1);
		dd = dd << 1;
		WriteHX711_CLK(false); //��Ƭ����CLK��������͵�ƽ
		if (ReadHx711_DATA() == SET)
		{
			dd++;
		}
		delay_us(1);
	}
	int x=0;
	if(mode==0)//2������������32
		x=2;
	else if(mode==1)//3������������64
		x=3;
	else///////1������������128
		x=1;

	/////////////////��������3�������ǲ���ͨ��A����64��2���ǲ���ͨ��B����32��1���ǲ���ͨ��A����128
	for (int i=0;i<x;i++)
	{
		WriteHX711_CLK(true);
		delay_us(1);
		WriteHX711_CLK(false); //��Ƭ����CLK��������͵�ƽ
		delay_us(1);
	}
//	WriteHX711_CLK(true);
//	delay_us(1);
//	WriteHX711_CLK(false); //��Ƭ����CLK��������͵�ƽ
//	delay_us(1);
//	WriteHX711_CLK(true);
//	delay_us(1);
//	WriteHX711_CLK(false); //��Ƭ����CLK��������͵�ƽ
//	delay_us(1);
///////////////////////////////////////////////////////////////////////////////////////////
	if(dd>=0x800000)
		dd|=0xff000000;
	long lll=long(dd);
	return lll;
}
