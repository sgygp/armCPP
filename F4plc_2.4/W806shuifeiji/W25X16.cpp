/*
 * W25X16.cpp
 *
 *  Created on: 2021年3月4日
 *      Author: Administrator
 */
//MOSI MISO接一起
#include "W25X16.h"
u8 W25X16::spi1_tx[256+4];

void W25X16::delay_us(u16 us) {
	u16 i;

	do {
		i = 13;
		while (i--)
			asm("nop");
		//__nop();
	} while (--us);
}

void W25X16::init() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_W25X_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = RCC_W25X_CS | RCC_W25X_CLK | RCC_W25X_DIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(RCC_W25X_PORT, &GPIO_InitStructure);

	GPIO_SetBits(RCC_W25X_PORT, RCC_W25X_CS);
	GPIO_SetBits(RCC_W25X_PORT, RCC_W25X_CLK);
	GPIO_SetBits(RCC_W25X_PORT, RCC_W25X_DIO);

}
void W25X16::setIn() {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = RCC_W25X_DIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(RCC_W25X_PORT, &GPIO_InitStructure);

}
void W25X16::setOut() {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = RCC_W25X_DIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(RCC_W25X_PORT, &GPIO_InitStructure);

}
void W25X16::WaitForWriteEnd() {
	for (int i = 0; i < 100000; i++) {                  //等待最长100毫秒
		delay_us(1);
		spi1_tx[0] = W25X_readRegstatus;
		SPI_Write(1, 1);
		if ((spi1_tx[1] & 1) != 1)
			break;
	}
}

void W25X16::SPI_Write(u16 dataGeshu, u16 clkGeshu) {

	GPIO_ResetBits(RCC_W25X_PORT, RCC_W25X_CS);
	delay_us(1);
	for (int var = 0; var < (dataGeshu + clkGeshu); ++var) {
		u8 shuju = spi1_tx[var];
		if (var < dataGeshu)
			setOut();
		else
			setIn();
		for (int i = 0; i < 8; i++) {
			GPIO_ResetBits(RCC_W25X_PORT, RCC_W25X_CLK);
			if (var < dataGeshu) {
				if (shuju & 0x80)
					GPIO_SetBits(RCC_W25X_PORT, RCC_W25X_DIO);
				else
					GPIO_ResetBits(RCC_W25X_PORT, RCC_W25X_DIO);
				shuju <<= 1;

			} else {
				shuju <<= 1;
				if (GPIO_ReadInputDataBit(RCC_W25X_PORT, RCC_W25X_DIO))
					shuju |= 0x01;

			}
			GPIO_SetBits(RCC_W25X_PORT, RCC_W25X_CLK);
			delay_us(2);

		}
		if (var >= dataGeshu)
			spi1_tx[var]=shuju;
	}
	GPIO_SetBits(RCC_W25X_PORT, RCC_W25X_CS);

}
u8 W25X16::SPI_FLASH_readdeviceID() {
	spi1_tx[0] = W25X_DeviceID;
	SPI_Write(1, 4);
	return spi1_tx[4];
}
u32 W25X16::SPI_FLASH_madeID() {
	spi1_tx[0] = W25X_madeID;
	SPI_Write(1, 3);
	u32 temp = (spi1_tx[1] << 16) | (spi1_tx[2] << 8) | spi1_tx[3];
	return temp;
}
void W25X16::SPI_FLASH_SectorErase(u32 SectorAddr) {//4K 扇区擦除
	spi1_tx[0] = W25X_WriteEnable;
	SPI_Write(1, 0);
	WaitForWriteEnd();
	spi1_tx[0] = W25X_sectorID;
	spi1_tx[1] = SectorAddr >> 16;
	spi1_tx[2] = SectorAddr >> 8;
	spi1_tx[3] = SectorAddr;
	SPI_Write(4, 0);
	WaitForWriteEnd();
}
void W25X16::SPI_FLASH_PageWriteData(u32 SectorAddr, int x) {//256字节页写
	spi1_tx[0] = W25X_WriteEnable;
	SPI_Write(1, 0);
	WaitForWriteEnd();
	spi1_tx[0] = W25X_PageProgram;
	spi1_tx[1] = SectorAddr >> 16;
	spi1_tx[2] = SectorAddr >> 8;
	spi1_tx[3] = SectorAddr;
	SPI_Write(4+ x,0);
	WaitForWriteEnd();
}
void W25X16::SPI_FLASH_PageReadData(u32 SectorAddr, int x) {
	spi1_tx[0] = W25X_ReadData;
	spi1_tx[1] = SectorAddr >> 16;
	spi1_tx[2] = SectorAddr >> 8;
	spi1_tx[3] = SectorAddr;
	SPI_Write(4, x);
}
