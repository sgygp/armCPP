/*
 * W25X16.h
 *
 *  Created on: 2021年3月4日
 *      Author: Administrator
 */

#ifndef W25X16_H_
#define W25X16_H_
#include "air32f10x_conf.h"
#define nFFT 2560

#define 	  Dummy_Byte 				0xff     //可以为任意数
#define     W25X_DeviceID			0xab
#define     W25X_madeID  			0x9f
#define     W25X_WriteEnable 		0x06
#define     W25X_sectorID     		0x20
#define     W25X_64KblockID     	0xd8
#define     W25X_PageProgram 	0x02
#define     W25X_ReadData  		0x03
#define     W25X_readRegstatus 0x05
#define     W25X_chipErase  		0x60


#define STARTADDR 0X200000//起始地址2M
#define YUANMA_1G_1001 125
//#define YUANMA_1G_1002 166
//#define YUANMA_1G_1005 400

#define YUANMA_1G YUANMA_1G_1001

#define FFT_START 10
#define FFT_END 1000

class W25X16 {
public:
	u8 *spi1_tx;
	static u16 jiasudu_sudu[50];

public:
	W25X16();
	virtual ~W25X16();
public:
	u8 SPI_FLASH_readdeviceID();
	u32 SPI_FLASH_madeID();
	void SPI_FLASH_SectorErase(u32 SectorAddr);
	void SPI_FLASH_ChipErase();
	void SPI_FLASH_BlockErase(u32 SectorAddr) ;
	void SPI_FLASH_PageWriteData(u32 SectorAddr, int x);
	void SPI_FLASH_PageReadData(u32 SectorAddr, int x);
	void WriteBenciSudu();
	void readShuju(int cishu);
	void jisuanFFT();

private:
	float jiasudu;
	float sudu;
	float weiyi;
	void SPI_Write(u16 dataGeshu, u16 clkGeshu);
	void WaitForWriteEnd();
	int JisuanJiasuduSudu(int weizhi);
	void getJieguo(u16 *jieguo);
	void getJieguo2(u16 *jieguo);
	void getJieguoFromFFT(u16 *suduweiyi);
};

#endif /* W25X16_H_ */
