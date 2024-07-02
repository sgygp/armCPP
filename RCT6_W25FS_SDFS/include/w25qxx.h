/*
 * W25QXX.h
 *
 *  Created on: 2019��11��14��
 *      Author: Administrator
 */
#ifndef W25QXX_H_
#define W25QXX_H_

#include "stm32f10x_conf.h"

#define 	  Dummy_Byte 				0xfc     //����Ϊ������
#define     W25X_DeviceID			0xab
#define     W25X_madeID  			0x9f
#define     W25X_WriteEnable 		0x06
#define     W25X_sectorID     		0x20
#define     W25X_PageProgram 	0x02
#define     W25X_ReadData  			0x03
#define     W25X_readRegstatus 	0x05

#define 	SPI1_TDR (u32)(SPI1_BASE+0x0C)
#define 	SPI1_RDR (u32)(SPI1_BASE+0x0C)

#define 	DISK_SECTOR_COUNT 0X200000/0x200
#define 	DISK_BLOCK_SIZE 8 //ÿ���ذ����������� 0 �Զ�

#ifdef __cplusplus
extern "C" {
#endif

u8 WZ5QXX_disk_read(u8 *buff,u32  sector, u16 count);
u8 WZ5QXX_disk_write(const u8 *buff,u32  sector, u16 count);

void SPI_DMA_Config(void);
u8 SPI_FLASH_readdeviceID();
u32 SPI_FLASH_madeID();
void SPI_FLASH_PageReadData(u32 SectorAddr, int x);
void SPI_FLASH_PageWriteData(u32 SectorAddr, int x);
void SPI_FLASH_SectorErase(u32 SectorAddr);

#ifdef __cplusplus
}
#endif

#endif /* W25QXX_H_ */

/*
 W25Q16 2M�ֽ�
 ÿҳ256���ֽ� ��
 ÿ��������16ҳ����������С�Ĳ�����λ��
 �ܹ�����512������
 */
