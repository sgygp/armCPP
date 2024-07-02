/*
 * W25X16.h
 *
 *  Created on: 2021年3月4日
 *      Author: Administrator
 */

#ifndef W25X16_H_
#define W25X16_H_
#include "stm32f4xx_conf.h"

#define RCC_W25X_RCC          	RCC_AHB1Periph_GPIOD
#define RCC_W25X_PORT       	GPIOD
#define RCC_W25X_CS 				GPIO_Pin_13
#define RCC_W25X_CLK            GPIO_Pin_12
#define RCC_W25X_DIO            GPIO_Pin_11

#define 	  Dummy_Byte 				0xfc     //可以为任意数
#define     W25X_DeviceID			0xab
#define     W25X_madeID  			0x9f
#define     W25X_WriteEnable 	0x06
#define     W25X_sectorID     		0x20
#define     W25X_PageProgram 	0x02
#define     W25X_ReadData  		0x03
#define     W25X_readRegstatus 0x05

class W25X16 {
public:
	static u8 spi1_tx[256 + 4];
	static void init();
	static u8 SPI_FLASH_readdeviceID();
	static u32 SPI_FLASH_madeID();
	static void SPI_FLASH_SectorErase(u32 SectorAddr);
	static void SPI_FLASH_PageWriteData(u32 SectorAddr, int x);
	static void SPI_FLASH_PageReadData(u32 SectorAddr, int x);
	static void delay_us(u16 us);
	static void SPI_Write(u16 dataGeshu, u16 clkGeshu);
	static void setIn();
	static void setOut();
	static void WaitForWriteEnd();
};

#endif /* W25X16_H_ */
