/*
 * LCD7565.h
 *
 *  Created on: 2021Äê10ÔÂ7ÈÕ
 *      Author: Administrator
 */

#ifndef LCD7565_H_
#define LCD7565_H_

#include "stm32f10x.h"

#define LCD_CS 	GPIO_Pin_5
//#define LCD_RST GPIO_Pin_13
#define LCD_A0	GPIO_Pin_2		//A0=¡°H¡±: data. A0=¡°L¡±: Instruction command
#define LCD_CLK GPIO_Pin_12
#define LCD_SI 	GPIO_Pin_11

#define IO_ST7565_CS_0 	GPIO_ResetBits(GPIOB,LCD_CS)
#define IO_ST7565_CS_1 	GPIO_SetBits(GPIOB,LCD_CS)
//#define IO_ST7565_RST_0 	GPIO_ResetBits(GPIOB,LCD_RST)
//#define IO_ST7565_RST_1 	GPIO_SetBits(GPIOB,LCD_RST)
#define IO_ST7565_RS_0 	GPIO_ResetBits(GPIOD,LCD_A0)
#define IO_ST7565_RS_1 	GPIO_SetBits(GPIOD,LCD_A0)
#define IO_ST7565_SCK_0 	GPIO_ResetBits(GPIOC,LCD_CLK)
#define IO_ST7565_SCK_1 	GPIO_SetBits(GPIOC,LCD_CLK)
#define IO_ST7565_SDA_0	GPIO_ResetBits(GPIOC,LCD_SI)
#define IO_ST7565_SDA_1	GPIO_SetBits(GPIOC,LCD_SI)

#define ST7565_DELAY_IO	  	10


class LCD7565 {
public:
	LCD7565();
	virtual ~LCD7565();

	void init();
	void Clear(uint8_t FillData);
	void WriteZi(u16 x/*0-127*/, u8 y/*0-7*/, u16 kuan/*0-127*/, u8 gao/*0-7*/,u8 geshu, const u8 * buf, bool isFan);
	void WriteShujuD(u16 x,u8 y,float shuju,u8 dot,bool shan);
	void WriteShuju(u16 x,u8 y,float shuju,u8 dot);
	void WriteShuju0(u16 x,u8 y,float shuju);
	void WriteString(u16 x,u8 y,u8 *buf,bool isFan);
	void WriteString(u16 x,u8 y,u8 *buf,u8 geshu,bool isFan);
	void WriteLeijiliuliang(double xx,u8 dot,bool isFan);

	void WriteHZx(u8 x, u8 y, u8 geshu,const u8 * buf, bool fan);
	void WriteShuzi(u8 x, u8 y, u8 geshu,const u8 * buf, bool fan);
	void Fill(u8 x1, u8 y1, u8 x2, u8 y2, u8 val);
	void Fill_bian();
private:
	void ST7565_Write(uint8_t dat, uint8_t rs); //rs == 1 data rs==0  cmd
	void inline ST7565_IO_Delay(u8 t) {
		while (t--)	;
	};
	void writec(u8 com);
	void writed(u8 dat);
	void display(u8 dat1, u8 dat2);
	void displaychar(u8 *p);
	void shuju2buf(float shuju ,u8 dot ,u8 *buf,u8 weishu);
};

#endif /* LCD7565_H_ */
