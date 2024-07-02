/*
 * AT24CL04.h
 *
 *  Created on: 2020年1月7日
 *      Author: Administrator
 */
#ifndef II_C_H_
#define II_C_H_
#include "stm32f10x_conf.h"
#include "delay.h"


#define WriteDeviceAddress 0xa0
#define ReadDviceAddress 0xa1

#define II_C_RCC 			RCC_APB2Periph_GPIOB
#define II_C_PORT 	GPIOB
#define II_C_SCL 	GPIO_Pin_12
#define II_C_SDA		GPIO_Pin_13

class II_C {
public:

	static	void init(void);
	static	void Write24C04(u8 *buf,u16 addr ,u16 geshu);
	static	void Read24Cxx(u8 * buf,u16 addr,u16 geshu);
	static	void Write24C02(u8 *buf,u8 addr,u8 geshu);
	//private:
	static	void SCL_set1() {
		GPIO_SetBits(II_C_PORT, II_C_SCL);
		//delay_us(1);
	}
	static	void SCL_set0() {
		GPIO_ResetBits(II_C_PORT, II_C_SCL);
		//delay_us(1);
	}
	
	static	void SDA_OUT(void) {
		GPIO_InitTypeDef GPIO_InitStruct ;
		GPIO_InitStruct.GPIO_Pin = II_C_SDA;    //SDA
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(II_C_PORT, &GPIO_InitStruct);

	}
	static	void SDA_set1(void) {
		GPIO_SetBits(II_C_PORT, II_C_SDA);
		//delay_us(1);
	}
	static	void SDA_set0(void) {
		GPIO_ResetBits(II_C_PORT, II_C_SDA);
		//delay_us(1);
	}

	static	u8 READ_SDA(void) {
		//delay_us(1);
		return GPIO_ReadInputDataBit(II_C_PORT, II_C_SDA);

	}
	static	void SDA_IN(void) {
		GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO
		GPIO_InitStructure.GPIO_Pin = II_C_SDA;    //SDA
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(II_C_PORT, &GPIO_InitStructure);
	}

	static	void Start();
	static	void Stop();
	static	u8 Wait_Ack();
	static	void Ack();
	static	void NAck();
	static	void Send_Byte(u8 txd);
	static	u8 Read_Byte(u8 ack);
	static	void WriteXbyte(u16 addr,u8 *buf,u8 geshu,u8 xianzhi);
};
#endif /* _IIC_H_ */

