/*
 * W25QXX_Init.cpp
 *
 *  Created on: 2019��11��14��
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
	if (i != 0) //��ʼ�������ǲ���������������
			{
		WZ5QXX_disk_read(temp, sector - (sector % 8), 8); //��Ҫ������FLASH�е�����ȫ��������4K��8������
		memcpy(temp + (sector % 8) * 512L, buff, i * 512); //�����Ҫд�����ݺϲ�����������������
		u32 start_sector = (sector / 8) * 4096;
		SPI_FLASH_SectorErase(start_sector); //����4KFLASH
		for (int x = 0; x < 16; x++) //��һ��д���������һ��page�����д16�Ρ�
				{
			memcpy(spi1_tx + 4, temp + 256 * x, 256);
			SPI_FLASH_PageWriteData(start_sector + x * 256, 256);
		}
		if (i == count) { //һ��д�������
			free(temp);
			return 0; //RES_OK
		}
	}
	int shengyu = count - i;

	for (int j = 0; j < shengyu / 8; j++) //�м���һ����ֱ��д�����ö�
			{
		memcpy(temp, buff + i * 512 + j * 4096, 4096);
		u32 start_sector = ((i + sector) / 8 + j) * 4096;
		SPI_FLASH_SectorErase(start_sector); //����4KFLASH
		for (int x = 0; x < 16; x++) //��һ��д���������һ��page�����д16�Ρ�
				{
			memcpy(spi1_tx + 4, temp + 256 * x, 256);
			SPI_FLASH_PageWriteData(start_sector + x * 256, 256);
		}
	}
	if (shengyu % 8 == 0) {
		free(temp);
		return 0; //RES_OK
	}
	///��д��������
	WZ5QXX_disk_read(temp, ((sector + count) / 8) * 4096, 8); //��Ҫ������FLASH�е�����ȫ��������4K��8������
	memcpy(temp, buff + (count - (sector + count) % 8) * 512,
			((sector + count) % 8) * 512); //�����Ҫд�����ݺϲ�����������������
	u32 start_sector = ((sector + count) / 8) * 4096;
	SPI_FLASH_SectorErase(start_sector); //����4KFLASH
	for (int x = 0; x < 16; x++) //��һ��д���������һ��page�����д16�Ρ�
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
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //SPIģʽΪ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      //����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;              //SPI���ͽ��ն�Ϊ8λ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                 //����SPI����ʱ SCLΪ��
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                   //���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                      //Ƭѡ�ź�����Ϊ���ģʽ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //SCL�ķ�Ƶֵ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;              //����Ϊ��λ�����ȴ���
	SPI_InitStructure.SPI_CRCPolynomial = 7;                  //crcУ��   Flash��֧��
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);

	memset(spi1_tx, 0, sizeof(spi1_tx));
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_StructInit(&DMA_InitStructure);
	DMA_DeInit(DMA1_Channel2);
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_TDR; //����DMAԴ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) spi1_tx; //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;             //������ΪDMA��Ŀ�Ķ�
	DMA_InitStructure.DMA_BufferSize = 1;           //�����С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //�����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //�ڴ��ַ����1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݿ��8bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�ڴ����ݿ��8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular ����ͣ�ش��ͣ�
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;           //(DMA�������ȼ�Ϊ��)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        //���ڴ浽�ڴ�
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);				//ʹ��DMAͨ��3 TX
//	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);       //ʹ��DMAͨ��3 TX�ж�

	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_RDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) spi1_tx;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);                   //ʹ��DMAͨ��2 RX
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);       //ʹ��DMAͨ��2 RX�ж�

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
void SPIDMA_Write(u16 size)                  //ÿ������ܳ���256�ֽ�
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
	for (int i = 0; i < 10000; i++) {                  //�ȴ��10����
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
	for (int i = 0; i < 100000; i++) {                  //�ȴ��100����
		delay_us(1);
		spi1_tx[0] = W25X_readRegstatus;
		SPIDMA_Write(2);
		if ((spi1_tx[1] & 1) != 1)
			break;

	}

}
/*
 * ��������
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
 ******  һҳ��д����
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
 һҳ�Ķ�����
 */
void SPI_FLASH_PageReadData(u32 SectorAddr, int x) {
	spi1_tx[0] = W25X_ReadData;
	spi1_tx[1] = (SectorAddr & 0Xff0000) >> 16;
	spi1_tx[2] = (SectorAddr & 0Xff00) >> 8;
	spi1_tx[3] = SectorAddr & 0Xff;
	SPIDMA_Write(x + 4);
}

void DMA1_Channel2_IRQHandler(void)  //DMA�����ж�
{
	DMA_ClearFlag(DMA1_FLAG_TC2);
	m_bOK = 1;

}
/*
 extern "C" void DMA1_Channel3_IRQHandler(void)  //DMA�����ж�
 {
 DMA_ClearFlag(DMA1_FLAG_TC3);
 m_bOK2= true;
 }
 */

/*
 �ĵã�
 1 	 ����DMAʱ�������޸ļ���ֵDMA_BufferSize����ENABLE
 DMA1_Channel3->CNDTR = size;
 DMA1_Channel2->CNDTR = size;
 2	SPI��DMA���������ɷ�������ģ�����ͬʱ��DMA�����DMA�����жϿ����ˣ�
 �϶��ȵ��������жϣ�Ȼ������ŵ��������жϡ�
 ���Է��;Ͳ����ж��ˣ�ֻ���տ��жϼ��ɡ����������ڽ����ж��м��ɡ�
 3	���ͽ��չ���һ������������Ϊ�������˲Ż���գ�����������ͬ
 4 һ��Ҫ��ENABLE ����DMA,��ENABLE ����DMA

 */
/*
	res = f_mkfs("", 0,work , sizeof work);
 *
 * 0 63 64 80 112
 * 1�ȴ���DBR(�߼�0����)63.(EB XX 90 MSDOS5.0... 55AA)
 * 2�����ļ����������Ŀ¼����(�ͷ�����ʼ�Լ����F8FFFF/F8FFFFFF,��������)
 * 3��ͷ����MBR(��������¼����������Ҫ��д��������������������Ϣ����ʼ������������������55AA
 *
 * д�ļ�
 * 1��д80����Ŀ¼��Ϣ��
 * 2����FAT�������ļ�������λ�ã��˴�д112������
 * 3��дFAT
 * 4����80��Ŀ¼���ļ���С�Լ���ʵ�غ�
 */

/*
 * 	MKFS_PARM abc;
	abc.align=8;//���ݶ��뵽����
	abc.au_size=4096;//�ش�С
	abc.fmt=FM_FAT;
	abc.n_fat=2;
	abc.n_root=512;//��Ŀ¼�ļ�����
	res = f_mkfs("", &abc,work , sizeof work);
 * 0 63 64 68 72 104��Ϊ��2��FAT��
 *
 *
 */
