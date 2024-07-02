/*
 * SHT30.cpp
 *
 *  Created on: 2020年1月7日
 *      Author: Administrator
 */

#include "II_C.h"
/*

const u8  CRC_Table[]= {
	0, 49, 98, 83, 196, 245, 166, 151, 185, 136, 219, 234, 125, 76, 31, 46, 67, 114, 33, 16,
	135, 182, 229, 212, 250, 203, 152, 169, 62, 15, 92, 109, 134, 183, 228, 213, 66, 115, 32, 17,
	63, 14, 93, 108, 251, 202, 153, 168, 197, 244, 167, 150, 1, 48, 99, 82, 124, 77, 30, 47,
	184, 137, 218, 235, 61, 12, 95, 110, 249, 200, 155, 170, 132, 181, 230, 215, 64, 113, 34, 19,
	126, 79, 28, 45, 186, 139, 216, 233, 199, 246, 165, 148, 3, 50, 97, 80, 187, 138, 217, 232,
	127, 78, 29, 44, 2, 51, 96, 81, 198, 247, 164, 149, 248, 201, 154, 171, 60, 13, 94, 111,
	65, 112, 35, 18, 133, 180, 231, 214, 122, 75, 24, 41, 190, 143, 220, 237, 195, 242, 161, 144,
	7, 54, 101, 84, 57, 8, 91, 106, 253, 204, 159, 174, 128, 177, 226, 211, 68, 117, 38, 23,
	252, 205, 158, 175, 56, 9, 90, 107, 69, 116, 39, 22, 129, 176, 227, 210, 191, 142, 221, 236,
	123, 74, 25, 40, 6, 55, 100, 85, 194, 243, 160, 145, 71, 118, 37, 20, 131, 178, 225, 208,
	254, 207, 156, 173, 58, 11, 88, 105, 4, 53, 102, 87, 192, 241, 162, 147, 189, 140, 223, 238,
	121, 72, 27, 42, 193, 240, 163, 146, 5, 52, 103, 86, 120, 73, 26, 43, 188, 141, 222, 239,
	130, 179, 224, 209, 70, 119, 36, 21, 59, 10, 89, 104, 255, 206, 157, 172 };

u8 CRC8(u8 *buf, u8 len)
{
	u8 crc=0;
	for(int i=0;i<len;i++)
        crc=CRC_Table[buf[i] ^ crc];
	return crc;
}

*/
/*
 * 
	AT24C02 , 2K SERIAL EEPROM: Internally organized with 32 pages of 8 bytes each, the 2K
			requires an 8-bit data word address for random word addressing.
	AT24C04 , 4K SERIAL EEPROM: Internally organized with 32 pages of 16 bytes each, the 4K
			requires a 9-bit data word address for random word addressing.
	AT24C08 , 8K SERIAL EEPROM: Internally organized with 64 pages of 16 bytes each, the 8K
			requires a 10-bit data word address for random word addressing.
	AT24C16 , 16K SERIAL EEPROM: Internally organized with 128 pages of 16 bytes each, the 16K
			requires an 11-bit data word address for random word addressing.
 * 
 * */



//初始化IIC
void II_C::init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin =  II_C_SCL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(II_C_PORT, &GPIO_InitStruct);
	
	SCL_set1();
	SDA_OUT();     //sda线输出
	SDA_set1();

}

//产生IIC起始信号

void II_C::Start(void)
{
	SDA_OUT();     //sda线输出
	SDA_set1();
	SCL_set1();
	SDA_set0();//START:when CLK is high,DATA change form high to low
	SCL_set0();//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void II_C::Stop(void)
{
	SDA_OUT();//sda线输出
	SCL_set0();
	SDA_set0();//STOP:when CLK is high DATA change form low to high
	SCL_set1();
	SDA_set1();//发送I2C总线结束信号
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 II_C::Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入
	SDA_set1();
	SCL_set1();
	while(READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			Stop();
			return 0;
		}
	}
	SCL_set0();//时钟输出0
	return 1;
}
//产生ACK应答
void II_C::Ack(void)
{
	SCL_set0();
	SDA_OUT();
	SDA_set0();
	SCL_set1();
	SCL_set0();
}
//不产生ACK应答
void II_C::NAck(void)
{
	SCL_set0();
	SDA_OUT();
	SDA_set1();
	SCL_set1();
	SCL_set0();
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void II_C::Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	SCL_set0();//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{
		if((txd&0x80)>>7)
			SDA_set1();
		else
			SDA_set0();
		txd<<=1;
		SCL_set1();
		SCL_set0();
	}
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 II_C::Read_Byte(u8 ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		SCL_set0();
			SCL_set1();
		receive<<=1;
		if(READ_SDA())receive++;
		}
	if (!ack)
		NAck();//发送nACK
	else
		Ack(); //发送ACK
	return receive;
}
void II_C::WriteXbyte(u16 addr,u8 *buf,u8 geshu,u8 xianzhi)
{
	if (geshu > xianzhi||geshu==0)
		return;
	u16 dd;
	if(addr>=256)
		dd=0x02;
	else
		dd=0x00;
//	dd = (addr & 0x7ff) >> 8;
//	dd <<= 1;
	Start();
	Send_Byte(WriteDeviceAddress | dd);
	Wait_Ack();
	Send_Byte(addr & 0xff);
	Wait_Ack();
	for(int i=0;i<geshu;i++)
	{
		Send_Byte(buf[i]);
		Wait_Ack();
	}
	Stop();
	Delay::delay_ms(5);

}
void II_C::Write24C02(u8 *buf,u8 addr,u8 geshu)
{
	int i = 0;
	u8 x=8-addr%8;
	u8 y=geshu>x?x:geshu;
	WriteXbyte( addr ,buf,y,8);
	addr+=y,buf+=y;geshu-=y;
	for (; i < geshu / 8; i++)
		WriteXbyte( addr + i * 8,buf + i * 8, 8,8);
	WriteXbyte( addr + i * 8,buf + i * 8, geshu % 8,8);

}
void II_C::Write24C04(u8 *buf,u16 addr,u16 geshu)
{
	int i = 0;
	u8 x=16-addr%16;
	u8 y=geshu>x?x:geshu;
	WriteXbyte( addr ,buf,y,16);//处理不是页首开始
	addr+=y,buf+=y;geshu-=y;
	for (; i < geshu / 16; i++)
		WriteXbyte( addr + i * 16,buf + i * 16, 16,16);
	WriteXbyte( addr + i * 16,buf + i * 16, geshu % 16,16);

}
void II_C::Read24Cxx(u8 * buf,u16 addr,u16 geshu)
{
	u16 dd;
	if(addr>=256)
		dd=0x02;
	else
		dd=0x00;
	Start();
	Send_Byte(WriteDeviceAddress | dd);
	Wait_Ack();
	Send_Byte(addr & 0xff);
	Wait_Ack();
	Start();
	Send_Byte(ReadDviceAddress | dd);
	if(Wait_Ack())
	{
		int i=0;
		for(;i<geshu-1;i++)
		{
			buf[i]=Read_Byte(1);
		}
		buf[i]=Read_Byte(0);
	}
	Stop();


}
