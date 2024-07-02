/*
 * SPIC.h
 *
 *  Created on: 2023Äê4ÔÂ18ÈÕ
 *      Author: Administrator
 */

#ifndef SPI2C_H_
#define SPI2C_H_
#include "air32f10x_conf.h"

#define SPI2_PORT               	GPIOB

#define GPIO_SPI2NSS            GPIO_Pin_12
#define SPI2_CLK_PIN            	GPIO_Pin_13
#define SPI2_MISO_PIN           GPIO_Pin_14
#define SPI2_MOSI_PIN           GPIO_Pin_15

//#define SPI2_TDR			(u32)(SPI2_BASE+0x0C)
//#define SPI2_RDR			(u32)(SPI2_BASE+0x0C)

#define CS_H2					GPIO_SetBits(SPI2_PORT,GPIO_SPI2NSS)
#define CS_L2					GPIO_ResetBits(SPI2_PORT,GPIO_SPI2NSS)


class SPI2_C {
public:
	SPI2_C()=default;
	static u8 tx[256+4];
	static u8 m_bOK ;

	static void init(void);
//	static void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
	static u8 SPI_ReadWrite_Byte(u16 size, bool isFillFF) ;

};

#endif /* SPIC_H_ */
