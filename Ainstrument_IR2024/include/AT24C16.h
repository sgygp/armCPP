/*
 * AT24CL04.h
 *
 *  Created on: 2020年1月7日
 *      Author: Administrator
 */
#ifndef AT24CL16_H_
#define AT24CL16_H_
#include "stm32f10x_conf.h"
/*

#define EEPROM_PAGE_SIZE		  16			   24Cxx的页面大小
#define EEPROM_SIZE				  255			   24Cxx总容量
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767

#define   EE_TYPE    AT24C02      * 定义EEPROM型号 *
*/

#define WriteDeviceAddress 0xa0
#define ReadDviceAddress 0xa1


#define AT24_PORT 		GPIOC
#define AT24_SCL 			GPIO_Pin_4
#define AT24_SDA		 	GPIO_Pin_5


class AT24C16 {
public:

	void init(void);
	void Write24c16( u8 *_pWriteBuf, u16 RomAddress,u16 ee_Size);
	void Read24c16( u8 *_pReadBuf, u16 RomAddress,u16 ee_Size);
private:
	void delay__us(u16 us);
	void Write_16byte(u8 *_pWriteBuf, u16 RomAddress,u16 ee_Size);
	void SCL_set1() {
		GPIO_SetBits(AT24_PORT, AT24_SCL);
	}
	void SCL_set0() {
		GPIO_ResetBits(AT24_PORT, AT24_SCL);
	}

	void SDA_set1(void) {
		GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO
		GPIO_InitStructure.GPIO_Pin = AT24_SDA;    //SDA
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(AT24_PORT, &GPIO_InitStructure);
		GPIO_SetBits(AT24_PORT, AT24_SDA);
	}
	void SDA_set0(void) {
		GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO
		GPIO_InitStructure.GPIO_Pin = AT24_SDA;    //SDA
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(AT24_PORT, &GPIO_InitStructure);
		GPIO_ResetBits(AT24_PORT, AT24_SDA);
	}
	uint8_t SDA_Read(void) {
		GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO
		GPIO_InitStructure.GPIO_Pin = AT24_SDA;    //SDA
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(AT24_PORT, &GPIO_InitStructure);
		return GPIO_ReadInputDataBit(AT24_PORT, AT24_SDA);
	}
	void Start();
	void Stop();
	void Ack();
	void NoAck();
	u8 Write8Bit(u8 input);
	u8 Read8Bit();
	u8 Busy();
};

#endif /* AT24CL04_H_ */
