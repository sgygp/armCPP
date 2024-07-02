/*
 * W25QXX_Init.cpp
 *
 *  Created on: 2019年11月14日
 *      Author: Administrator
 */
#include "w25qxx.h"
#include <string.h>
#include "delay.h"
#include <stdlib.h>

u8 spi1_tx[256 + 4];
u8 m_bOK = 0;

u8 WZ5QXX_disk_read(u8 *buff, u32 sector, u16 count) {
	if ((sector + count) * 512 > 0x200000)
		return 1; //RES_ERROR;
	for (int i = 0; i < count; i++) {
		SPI_FLASH_PageReadData((sector + i) * 512, 256);
		memcpy(buff + i * 512, spi1_tx + 4, 256);
		SPI_FLASH_PageReadData((sector + i) * 512 + 256, 256);
		memcpy(buff + i * 512 + 256, spi1_tx + 4, 256);
	}
	return 0; //RES_OK;
}
u8 WZ5QXX_disk_write(const u8 *buff, u32 sector, u16 count) {
	if (count == 0)
		return 1; //RES_ERROR;
	if ((sector + count) * 512 > 0x200000)
		return 1; //RES_ERROR;
	u8 *temp = (u8*) malloc(4096);
	int i = 0;
	for (; i < count; i++) {
		if (((i + sector) % 8) == 0)
			break;
	}
	if (i != 0) //起始扇区不是擦除扇区的整数倍
			{
		WZ5QXX_disk_read(temp, sector - (sector % 8), 8); //把要擦除的FLASH中的内容全读进来，4K，8个扇区
		memcpy(temp + (sector % 8) * 512L, buff, i * 512); //把这次要写的数据合并到读上来的数据中
		u32 start_sector = (sector / 8) * 4096;
		SPI_FLASH_SectorErase(start_sector); //擦除4KFLASH
		for (int x = 0; x < 16; x++) //用一次写半个扇区（一个page）命令，写16次。
				{
			memcpy(spi1_tx + 4, temp + 256 * x, 256);
			SPI_FLASH_PageWriteData(start_sector + x * 256, 256);
		}
		if (i == count) { //一次写就完成了
			free(temp);
			return 0; //RES_OK
		}
	}
	int shengyu = count - i;

	for (int j = 0; j < shengyu / 8; j++) //中间这一部分直接写，不用读
			{
		memcpy(temp, buff + i * 512 + j * 4096, 4096);
		u32 start_sector = ((i + sector) / 8 + j) * 4096;
		SPI_FLASH_SectorErase(start_sector); //擦除4KFLASH
		for (int x = 0; x < 16; x++) //用一次写半个扇区（一个page）命令，写16次。
				{
			memcpy(spi1_tx + 4, temp + 256 * x, 256);
			SPI_FLASH_PageWriteData(start_sector + x * 256, 256);
		}
	}
	if (shengyu % 8 == 0) {
		free(temp);
		return 0; //RES_OK
	}
	///读写第三部分
	WZ5QXX_disk_read(temp, ((sector + count) / 8) * 4096, 8); //把要擦除的FLASH中的内容全读进来，4K，8个扇区
	memcpy(temp, buff + (count - (sector + count) % 8) * 512,
			((sector + count) % 8) * 512); //把这次要写的数据合并到读上来的数据中
	u32 start_sector = ((sector + count) / 8) * 4096;
	SPI_FLASH_SectorErase(start_sector); //擦除4KFLASH
	for (int x = 0; x < 16; x++) //用一次写半个扇区（一个page）命令，写16次。
			{
		memcpy(spi1_tx + 4, temp + 256 * x, 256);
		SPI_FLASH_PageWriteData(start_sector + x * 256, 256);
	}
	free(temp);
	return 0; //RES_OK;
}

void SPI_DMA_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //CS
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA, GPIO_Pin_2);
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //SPI模式为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      //设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;              //SPI发送接收都为8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                 //设置SPI空闲时 SCL为高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                   //数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                      //片选信号设置为软件模式
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //SCL的分频值
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;              //设置为高位数据先传送
	SPI_InitStructure.SPI_CRCPolynomial = 7;                  //crc校验   Flash不支持
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);

	memset(spi1_tx, 0, sizeof(spi1_tx));
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_StructInit(&DMA_InitStructure);
	DMA_DeInit(DMA1_Channel2);
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_TDR; //设置DMA源
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) spi1_tx; //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;             //外设作为DMA的目的端
	DMA_InitStructure.DMA_BufferSize = 1;           //传输大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //外设地址不增加
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //内存地址自增1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据宽度8bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //内存数据宽度8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;           //(DMA传送优先级为高)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        //非内存到内存
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);				//使能DMA通道3 TX
//	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);       //使能DMA通道3 TX中断

	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_RDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) spi1_tx;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);                   //使能DMA通道2 RX
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);       //使能DMA通道2 RX中断

	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
//	NVIC_Init(&NVIC_InitStructure);

}
void SPIDMA_Write(u16 size)                  //每次最大不能超过256字节
{
	if (size > (sizeof(spi1_tx) + 4))
		return;
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	m_bOK = 0;
	DMA1_Channel2->CNDTR = size;
	DMA1_Channel3->CNDTR = size;
	DMA_Cmd(DMA1_Channel2, ENABLE);
	DMA_Cmd(DMA1_Channel3, ENABLE);
	for (int i = 0; i < 10000; i++) {                  //等待最长10毫秒
		if (m_bOK) {
			break;
		delay_us(1);
		}
	}
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}
u8 SPI_FLASH_readdeviceID() {
	spi1_tx[0] = W25X_DeviceID;
	SPIDMA_Write(5);
	return spi1_tx[4];
}
u32 SPI_FLASH_madeID() {

	spi1_tx[0] = W25X_madeID;
	SPIDMA_Write(4);
	u32 temp = (spi1_tx[1] << 16) | (spi1_tx[2] << 8) | spi1_tx[3];
	return temp;
}

void SPI_FLASH_WaitForWriteEnd() {
	for (int i = 0; i < 100000; i++) {                  //等待最长100毫秒
		delay_us(1);
		spi1_tx[0] = W25X_readRegstatus;
		SPIDMA_Write(2);
		if ((spi1_tx[1] & 1) != 1)
			break;

	}

}
/*
 * 扇区擦除
 */
void SPI_FLASH_SectorErase(u32 SectorAddr) {
	spi1_tx[0] = W25X_WriteEnable;
	SPIDMA_Write(1);
	SPI_FLASH_WaitForWriteEnd();
	spi1_tx[0] = W25X_sectorID;
	spi1_tx[1] = (SectorAddr & 0Xff0000) >> 16;
	spi1_tx[2] = (SectorAddr & 0Xff00) >> 8;
	spi1_tx[3] = SectorAddr & 0Xff;
	SPIDMA_Write(4);
	SPI_FLASH_WaitForWriteEnd();
}

/*
 ******  一页的写操作
 */
void SPI_FLASH_PageWriteData(u32 SectorAddr, int x) {
	spi1_tx[0] = W25X_WriteEnable;
	SPIDMA_Write(1);
	SPI_FLASH_WaitForWriteEnd();
	spi1_tx[0] = W25X_PageProgram;
	spi1_tx[1] = (SectorAddr & 0Xff0000) >> 16;
	spi1_tx[2] = (SectorAddr & 0Xff00) >> 8;
	spi1_tx[3] = SectorAddr & 0Xff;
	SPIDMA_Write(x + 4);
	SPI_FLASH_WaitForWriteEnd();

}

/*
 一页的读操作
 */
void SPI_FLASH_PageReadData(u32 SectorAddr, int x) {
	spi1_tx[0] = W25X_ReadData;
	spi1_tx[1] = (SectorAddr & 0Xff0000) >> 16;
	spi1_tx[2] = (SectorAddr & 0Xff00) >> 8;
	spi1_tx[3] = SectorAddr & 0Xff;
	SPIDMA_Write(x + 4);
}

void DMA1_Channel2_IRQHandler(void)  //DMA接收中断
{
	DMA_ClearFlag(DMA1_FLAG_TC2);
	m_bOK = 1;

}
/*
 extern "C" void DMA1_Channel3_IRQHandler(void)  //DMA发送中断
 {
 DMA_ClearFlag(DMA1_FLAG_TC3);
 m_bOK2= true;
 }
 */

/*
 心得：
 1 	 启动DMA时必须先修改计数值DMA_BufferSize，再ENABLE
 DMA1_Channel3->CNDTR = size;
 DMA1_Channel2->CNDTR = size;
 2	SPI的DMA接收来是由发送引起的，所以同时打开DMA后，如果DMA发送中断开启了，
 肯定先到来发送中断，然后紧接着到来接收中断。
 所以发送就不用中断了，只接收开中断即可。处理结果就在接收中断中即可。
 3	发送接收共用一个缓冲器，因为发送完了才会接收，并且数量相同
 4 一定要先ENABLE 接收DMA,再ENABLE 发送DMA

 */
/*
	res = f_mkfs("", 0,work , sizeof work);
 *
 * 0 63 64 80 112
 * 1先创建DBR(逻辑0扇区)63.(EB XX 90 MSDOS5.0... 55AA)
 * 2创建文件分配表，跟主目录区。(就分区表开始自己填充F8FFFF/F8FFFFFF,其余清零)
 * 3回头创建MBR(主引导记录）。里面主要填写保留扇区数，分区表信息（开始扇区，总扇区数），55AA
 *
 * 写文件
 * 1先写80出根目录信息。
 * 2根据FAT表查出，文件首扇区位置，此处写112扇区。
 * 3回写FAT
 * 4更新80真目录处文件大小以及其实簇号
 */

/*
 * 	MKFS_PARM abc;
	abc.align=8;//数据对齐到扇区
	abc.au_size=4096;//簇大小
	abc.fmt=FM_FAT;
	abc.n_fat=2;
	abc.n_root=512;//根目录文件个数
	res = f_mkfs("", &abc,work , sizeof work);
 * 0 63 64 68 72 104因为是2个FAT表
 *
 *
 */
