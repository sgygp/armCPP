/*
 * LCD7565.cpp
 *
 *  Created on: 2021年10月7日
 *      Author: Administrator
 */

#include <LCD7565.h>
#include "dianzhen.h"
#include "delay.h"

LCD7565::LCD7565() {
	// TODO 自动生成的构造函数存根
}

LCD7565::~LCD7565() {
	// TODO 自动生成的析构函数存根
}
void LCD7565::ST7565_Write(uint8_t dat, uint8_t rs) //rs == 1 data rs==0  cmd
{
	uint8_t i;
	IO_ST7565_SCK_0;
	IO_ST7565_CS_0;
	if (!rs) {
		IO_ST7565_RS_0; //Command
	} else {
		IO_ST7565_RS_1;
	}
	ST7565_IO_Delay(ST7565_DELAY_IO);
	for (i = 0; i < 8; i++) {
		if (dat & 0x80) {
			IO_ST7565_SDA_1;
		} else {
			IO_ST7565_SDA_0;
		}
		dat <<= 1;
		IO_ST7565_SCK_1;
		ST7565_IO_Delay(ST7565_DELAY_IO);
		IO_ST7565_SCK_0;
		ST7565_IO_Delay(ST7565_DELAY_IO);
	}
	ST7565_IO_Delay(ST7565_DELAY_IO);
	IO_ST7565_CS_1;
	IO_ST7565_RS_1;
	ST7565_IO_Delay(ST7565_DELAY_IO);

}
void LCD7565::writec(u8 com) {
	ST7565_Write(com, 0);
}

void LCD7565::writed(u8 dat) {
	ST7565_Write(dat, 1);
}

void LCD7565::Clear(uint8_t FillData) {
	uint8_t i, j;
	for (i = 0XB0; i < 0XB8; i++) {
		ST7565_Write(i, 0); //Set Page Address
		ST7565_Write(0x10, 0); //Set Column Address(MSByte) = 0
		ST7565_Write(0x00, 0); //Colum(LSByte) from S1 -> S128 auto add
		for (j = 0; j < 128; j++) {
			ST7565_Write(FillData, 1);
		}
	}
}

void LCD7565::Fill_bian() {
}

void LCD7565::init() {

	GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO
	GPIO_InitStructure.GPIO_Pin = LCD_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCD_A0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  LCD_CLK|LCD_SI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	IO_ST7565_CS_1;
	//	IO_ST7565_RST_0;
	//	Delay::delay_ms(10); //hard reset for 10ms
	//	IO_ST7565_RST_1;
	Delay::delay_ms(10);
	ST7565_Write(0xE2, 0); //Soft Reset
	ST7565_Write(0xA2, 0); //Duty bias set  0xA2 is light 0xA3 is dark
	//ST7565_Write(0xA7,0); //0xA7 reverse 0xA6 normal default normal
	ST7565_Write(0xA0, 0); //ADC select S0->S131(?????S1-S128)
	ST7565_Write(0xC8, 0); //com1 --> com64
	ST7565_Write(0x23, 0); //V0 Voltage Regulator Internal Resistor Ratio Set 0x20~0x27
//	delay_ms(2);
	ST7565_Write(0x81, 0); //V0 voltage set first cmd
	ST7565_Write(0x3c, 0); //following V0 voltage set 0x00~0x3F
//	delay_ms(2);
	ST7565_Write(0xF8, 0); //Booster Ratio Select Mode Set first cmd
	ST7565_Write(0x00 & 0x03, 0); //following Booset Ratio Register Set 0x00~0x03
//	delay_ms(2);
	ST7565_Write(0x2F, 0); //power control all on
//	delay_ms(2);
	ST7565_Write(0x40 + 0x00, 0); //0x40 + Display Start Line(0)
	Clear(0x00); //full clear
	ST7565_Write(0xAF, 0); //Display ON


	/*	  display(0xff,0xff);
	 display(0x00,0x00);
	 display(0x55,0xaa);
	 display(0xaa,0x55);
	 displaychar((u8*)chara1);
	 displaychar((u8*)chara2);//vop_test();

	 */

}
/*

 void LCD7565::display(u8 dat1, u8 dat2) {
 u8 row, col;

 for (row = 0xb0; row < 0xb8; row++)    //0XB0    0XB8
 {
 writec(row);    //set page address
 writec(0x10);    //set column address
 writec(0x00);
 for (col = 0; col < 128; col++) {
 writed(dat1);
 writed(dat2);
 }
 }

 Delay::delay_ms(1000);

 }

 void LCD7565::displaychar(u8 *p) {
 u8 row, col;

 for (row = 0xb0; row < 0xb8; row++) {
 writec(row);    //set page address
 writec(0x10);    //set column address
 writec(0x00);
 for (col = 0; col < 128; col++)
 writed(*p++);
 }

 Delay::delay_ms(1000);

 }
 */
void LCD7565::WriteZi(u16 x/*0-127*/, u8 y/*0-7*/, u16 kuan/*0-127*/,
		u8 gao/*0-7*/, u8 geshu, const u8 * buf, bool isFan) {

	int zikaishi;
	int aaa = 0;
	for (int k = 0; k < geshu; k++) {
		zikaishi = x + kuan * k;
		for (int j = 0; j < gao; j++) {
			writec(0xb0 + y + j);    //set page address
			writec(0x10 + (zikaishi >> 4));    //set column address
			writec(zikaishi & 0x0f);
			for (int i = 0; i < kuan; i++) {
				if (isFan)
					writed(~buf[aaa]);
				else
					writed(buf[aaa]);
				aaa++;
			}
		}
	}
}
void LCD7565::Fill(u8 x1, u8 y1, u8 x2, u8 y2, u8 val) {
	for (int i = x1; i < x2; i++) {
		writec(0xb0 + i);    //set page address
		writec(0x10 + (y1 >> 4));    //set column address
		writec(y1 & 0xf);
		for (int j = y1; j < y2; j++)
			writed(val);
	}
}
void LCD7565::WriteShuzi(u8 x, u8 y, u8 geshu, const u8 * buf, bool fan) {
	WriteZi(x, y, 8, 2, geshu, buf, fan);
}
void LCD7565::WriteHZx(u8 x, u8 y, u8 geshu, const u8 * buf, bool fan) {
	WriteZi(x, y, 16, 2, geshu, buf, fan);
}
void LCD7565::shuju2buf(float shuju, u8 dot, u8 *buf,u8 weishu) {
	int i, a;
	int w;
	for (i = 0; i < dot; i++)
		shuju *= 10;
	w = shuju + 0.5;
	for (i = 0; i <weishu; i++) {
		a = w % 10;
		w /= 10;
		buf[ (weishu-1) - i] = a + '0';
	}
	for (i = 0; i < (weishu-1) - dot; i++)
		buf[i] = buf[i + 1];
	buf[i] = '.';
	for (w = 0; w < i - 1; w++) {
		if (buf[w] != '0')
			break;
		else
			buf[w] = ' ';    //写空格
	}

	if (!dot)
		buf[4] = ' ';

}
void LCD7565::WriteShujuD(u16 x, u8 y, float shuju, u8 dot,bool shan) {
	u8 i = 0;
	u8 text[6];
	if(shuju>99.99)
		dot=1;


	if(shan)
	{
		Fill(0,0,4,128,0);
		return;
	}

	if (shuju < 0) {
		shuju = 0-shuju;
		i = 80;
	}
	WriteZi(0, y, 20, 4, 1, LCDZHENGFU2032 + i, false);
	shuju2buf(shuju, dot, text,6);
	u8 a = 0;
	for (i = 0; i < 6; i++) {
		if (text[i] == '.') {
			WriteZi(x + i * 20, y, 8, 4, 1, LCDDOT0832, false);
			a = 12;
		} else if (text[i] == ' ')
			WriteZi(x + i * 20, y, 20, 4, 1, LCDZHENGFU2032 + 80 * 2, false);
		else
			WriteZi(x + i * 20 - a, y, 20, 4, 1,	LCDSHUZI2032 + 80 * (text[i] - '0'), false);
	}
}
void LCD7565::WriteShuju(u16 x, u8 y, float shuju, u8 dot) {

	if(dot==0)
		x+=8;
	u8 i = ' ';
	u8 text[5];
	if (shuju < 0) {
		shuju = 0 - shuju;
		i = '-';
	}
	WriteZi(x - 8, y, 8, 2, 1, ASCII + 16 * (i - ' '), false);
	shuju2buf(shuju, dot, text,5);
	for (i = 0; i < 5; i++)
		WriteZi(x + i * 8, y, 8, 2, 1, ASCII + 16 * (text[i] - ' '), false);
}
void LCD7565::WriteShuju0(u16 x, u8 y, float shuju) {

	u8 i = ' ';
	if (shuju < 0) {
		shuju = 0 - shuju;
		i = '-';
	}
	WriteZi(x - 8, y, 8, 2, 1, ASCII + 16 * (i - ' '), false);
	u16 w=shuju;
	for (i = 0; i < 4; i++)
	{

		WriteZi(x + (3-i) * 8, y, 8, 2, 1, ASCII + 16 * (w%10 + '0'-' '), false);
		w/=10;
	}
}
void LCD7565::WriteString(u16 x, u8 y, u8 *buf, bool isFan) {
	for (u8 i = 0; i < 16; i++)    //一次最大写16个字符
	{
		if (!buf[i])
			return;
		WriteZi(x + i * 8, y, 8, 2, 1, ASCII + (buf[i] - ' ') * 16, isFan);
	}
}
void LCD7565::WriteString(u16 x, u8 y, u8 *buf, u8 geshu,bool isFan) {
	for (u8 i = 0; i < geshu; i++)    //一次最大写16个字符
	{
		WriteZi(x + i * 8, y, 8, 2, 1, ASCII + (buf[i] - ' ') * 16, isFan);
	}
}
void LCD7565::WriteLeijiliuliang(double xx,u8 dot,bool isFan) {
	bool fu=false;
	if(xx<0)
	{
		xx=0-xx;
		fu=true;
	}
	for(int i=0;i<dot;i++)
		xx*=10;
	long long uu=xx;
	u8 buf[12];
	for(u8 i=0;i<12;i++)
	{
		if(i==dot&&dot)
		{
			buf[11-i]='.';
			continue;
		}

		buf[11-i]=(uu%10)+'0';
		uu/=10;
	}
	for(int i=0;i<11;i++)
	{
		if(buf[i]=='0'&&buf[i+1]!='.')
			buf[i]=' ';
		else
		{
			if(fu)
			{
				if((i-1)>=0)
					buf[i-1]='-';
			}
			break;
		}

	}
	for(int i=0;i<12;i++)
		WriteZi(16+8+8*i, 6, 8, 2, 1, ASCII + (buf[i] - ' ') * 16, isFan);

}

