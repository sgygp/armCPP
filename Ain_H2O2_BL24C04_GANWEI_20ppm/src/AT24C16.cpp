/*
 * AT24CL04.cpp
 *
 *  Created on: 2020年1月7日
 *      Author: Administrator
 */

#include <AT24C16.h>
u8 AT24C16::m_bInit;

/***************************************************************************/
void AT24C16::delay__us(u16 us) {
	u16 i;

	do {
		i = 30;
		while (i--)
			asm("nop");
		//__nop();
	} while (--us);
}

void AT24C16::init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO
	//使能串口的RCC时钟
	RCC_APB2PeriphClockCmd(AT24_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = AT24_SCL;	//SCL
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AT24_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = AT24_SDA;	//SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AT24_PORT, &GPIO_InitStructure);
	SCL_set1();
	SDA_set1();
	delay__us(1);
	m_bInit=true;

}

/***************************************************************************/
void AT24C16::Start() {

	SDA_set1();
	delay__us(1);
	SCL_set1();
	delay__us(1);
	SDA_set0();
	delay__us(1);
}

/***************************************************************************/
void AT24C16::Stop() {
	SDA_set0();
	delay__us(1);
	SCL_set1();
	delay__us(1);
	SDA_set1();
	delay__us(1);
}

/***************************************************************************/
void AT24C16::Ack() {
	SCL_set0();
	delay__us(1);
	SDA_set0();
	delay__us(1);
	SCL_set1();
	delay__us(1);
	SCL_set0();
	delay__us(1);
}

/***************************************************************************/
void AT24C16::NoAck() {
	SCL_set0();
	delay__us(1);
	SDA_set1();
	delay__us(1);
	SCL_set1();
	delay__us(1);
	SCL_set0();
	delay__us(1);
}

/***************************************************************************/
//u8 AT24C16::TestAck() {
//	u8 ErrorBit;
//	SDA_set1();
//	delay__us(1);
//	SCL_set1();
//	delay__us(1);
//	for (int i = 0; i < 80; i++) {
//		ErrorBit = SDA_Read();
//		if (!ErrorBit)
//			break;
//		delay__us(1);
//	}
//	SCL_set0();
//	delay__us(1);
//	return (ErrorBit);
//}
/***************************************************************************/
u8 AT24C16::Write8Bit(u8 input) {
	u8 temp;
	SCL_set0();
	delay__us(1);
	for (temp = 8; temp != 0; temp--) {
		if (input & 0x80)
			SDA_set1();
		else
			SDA_set0();
		delay__us(1);
		SCL_set1();
		delay__us(1);
		SCL_set0();
		delay__us(1);
		input = input << 1;
	}
	SDA_set1();
	delay__us(1);
	SCL_set1();
	delay__us(20);
	temp = SDA_Read();
	SCL_set0();
	delay__us(1);
	return temp;
}
/***************************************************************************/
u8 AT24C16::Read8Bit() {
	u8 temp, rbyte = 0;
	for (temp = 8; temp != 0; temp--) {
		SDA_set1();
		delay__us(1);
		SCL_set1();
		delay__us(1);
		rbyte = rbyte << 1;
		rbyte = rbyte | SDA_Read();
		SCL_set0();
		delay__us(1);
	}
	return (rbyte);
}
/***************************************************************************/
u8 AT24C16::Busy() {
	Start();
	return (SDA_Read());
}


void AT24C16::Write_16byte(u8 *_pWriteBuf, u16 RomAddress, u16 ee_Size) {
	if (ee_Size > 16 || ee_Size == 0)
		return;
	u8 dd;
	dd = (RomAddress & 0x7ff) >> 8;
	dd <<= 1;
	Start();
	Write8Bit(WriteDeviceAddress | dd);
	Write8Bit(RomAddress & 0xff);
	for (int i = 0; i < ee_Size; i++) {
		Write8Bit(_pWriteBuf[i]);
	}
	Stop();
	Busy();
//	for (int i = 0; i < 1000; i++) {
//		if (!Busy())
//			break;
//		delay__us(100);
//
//	}

}

void AT24C16::Write24c16(u8 *_pWriteBuf, u16 RomAddress, u16 ee_Size) {
	int i = 0;
	for (; i < ee_Size / 16; i++)
		Write_16byte(_pWriteBuf + i * 16, RomAddress + i * 16, 16);
	Write_16byte(_pWriteBuf + i * 16, RomAddress + i * 16, ee_Size % 16);
}

void AT24C16::Read24c16(u8 *_pReadBuf, u16 RomAddress, u16 ee_Size) {
	u8 dd;
	dd = (RomAddress & 0x7ff) >> 8;
	dd <<= 1;
	Start();
	Write8Bit(WriteDeviceAddress | dd);
	Write8Bit(RomAddress & 0xff);
	Start();
	Write8Bit(ReadDviceAddress | dd);
	for (int i = 0; i < ee_Size; i++) {
		_pReadBuf[i] = Read8Bit();
		if (i != ee_Size - 1)
			Ack();
		else
			NoAck();
	}

	Stop();
}
