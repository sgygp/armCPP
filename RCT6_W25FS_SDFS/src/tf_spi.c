/*
 * tf_spi.c
 *
 *  Created on: 2020年2月27日
 *      Author: Administrator
 */

#include "tf_spi.h"
#include <string.h>
#include "delay.h"

u8 spi2_tx[512];
u8 m_bOK2 = 0;
u8 CardType;

void spi2_dma_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); //使能SPI2时钟

	//GPIOB13,14,15初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; //PB3~5复用功能输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化
	CS_H;

	//这里只针对SPI口初始化
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE); //复位SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE); //停止复位SPI2

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; //定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure); //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPI2, ENABLE); //使能SPI外设

	memset(spi2_tx, 0, sizeof(spi2_tx));
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_StructInit(&DMA_InitStructure);
	DMA_DeInit(DMA1_Channel4);
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI2_TDR; //设置DMA源
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) spi2_tx; //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;             //外设作为DMA的目的端
	DMA_InitStructure.DMA_BufferSize = 1;           //传输大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //外设地址不增加
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //内存地址自增1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据宽度8bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //内存数据宽度8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;           //(DMA传送优先级为高)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        //非内存到内存
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);				//使能DMA通道5 TX
	//	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);       //使能DMA通道5 TX中断

	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI2_RDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) spi2_tx;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);                   //使能DMA通道4 RX
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);       //使能DMA通道4 RX中断

	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	//	NVIC_Init(&NVIC_InitStructure);

}
//设置SPI速率 初始速率不能高于400k
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler) {
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	//判断有效性
	SPI2->CR1 &= 0XFFC7;	//位3-5清零，用来设置波特率
	SPI2->CR1 |= SPI_BaudRatePrescaler;	//设置SPI2速度
	SPI_Cmd(SPI2, ENABLE); //使能SPI1
}

void DMA1_Channel4_IRQHandler(void)  //DMA接收中断
{
	DMA_ClearFlag(DMA1_FLAG_TC4);
	m_bOK2 = 1;

}
u8 SPI_ReadWrite_Byte(u16 size, u8 isFillFF)         //每次最大不能超过512字节,返回最后一个收到的数据
{
	if (size > (sizeof(spi2_tx) + 4) || size == 0)
		return 0;
	if (isFillFF)
		memset(spi2_tx, 0xff, size);
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	m_bOK2 = 0;
	DMA1_Channel4->CNDTR = size;
	DMA1_Channel5->CNDTR = size;
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	for (int i = 0; i < 10000; i++) {                  //等待最长10毫秒
		if (m_bOK2) {
			break;
			delay_us(1);
		}
	}
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	return spi2_tx[size - 1];
}
//SD卡的一些操作函数：
//取消片选 释放SPI总线
void SD_DisSelect(void) {
	CS_H;
	u8 a = spi2_tx[0];
	SPI_ReadWrite_Byte(1, 1);
	spi2_tx[0] = a;
}
//等待卡准备好
//准备好了返回值0；其他错误代码
u8 SD_WaitReady(void) {
	u32 t = 0;
	do {
		if (SPI_ReadWrite_Byte(1, 1) == 0XFF)
			return 0; //OK
		delay_us(1);
		t++;
	} while (t < 10000);
	return 1;
}
//选择SD卡并等待卡准备好
//返回值 0成功，1失败
u8 SD_Select(void) {
	CS_L;
	if (SD_WaitReady() == 0)
		return 0;
	SD_DisSelect();
	return 1;
}

//向SD卡发送一个命令
//u8 cmd  命令
//u32 arg 命令参数
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc) {
	u8 r1;
	u8 Retry = 0;
	SD_DisSelect(); //释放已经包含了发送一个字节
	if (SD_Select())
		return 0;
	spi2_tx[0] = (cmd | 0x40);
	spi2_tx[1] = (arg >> 24);
	spi2_tx[2] = (arg >> 16);
	spi2_tx[3] = (arg >> 8);
	spi2_tx[4] = (arg);
	spi2_tx[5] = (crc);
//	if (cmd == CMD12) {
//		spi2_tx[6] = (0xff);   	//Skip a stuff byte when stop reading
//		SPI_ReadWrite_Byte(7);
//	} else
	SPI_ReadWrite_Byte(6, 0);
	Retry = 10;
	do {
		r1 = SPI_ReadWrite_Byte(1, 1); //发送一个0xff
	} while ((r1 & 0X80) && Retry--);
	return r1;
}

/**
 * @brief  Checks for error conditions for CMD0.
 * @param  None
 * @retval SD_Error: SD Card Error code.
 */
static SD_Error Cmd0(void) {
	u32 timeout;
	timeout = 20;
	while ((SD_SendCmd(SDIO_GO_IDLE_STATE, 0, 0x95) != 0X01) && timeout--)
		;
	return (timeout == 0) ? SD_CMD_RSP_TIMEOUT : SD_OK;
}
/**
 * @brief  Checks for error conditions for R7.
 *   response.
 * @param  None
 * @retval SD_Error: SD Card Error code.
 */
static SD_Error Cmd8(void) {
	u32 timeout;
	timeout = 20;
	while ((SD_SendCmd(SDIO_HS_SEND_EXT_CSD, 0x1AA, 0x87) != 0X01) && timeout--)
		;
	return (timeout == 0) ? SD_CMD_RSP_TIMEOUT : SD_OK;
}

//初始化SD
u8 SD_Init_Config(void) {
	u8 r1 = 1;      //
	u16 retry;  //

	SPI2_SetSpeed(SPI_BaudRatePrescaler_128);
	SPI_ReadWrite_Byte(40, 1);	////发送至少74个脉冲
	if (Cmd0() != SD_OK)
		return -1;
	CardType = 0;	//默认无卡
	if (Cmd8() == SD_OK) {	//SD V2.0
		SPI_ReadWrite_Byte(4, 1);
		if (spi2_tx[2] == 0X01 && spi2_tx[3] == 0XAA)	//卡是否支持2.7~3.6V
				{
			retry = 20;
			do {
				SD_SendCmd(CMD55, 0, 0X01);	//发送CMD55
				r1 = SD_SendCmd(CMD41, 0x40000000, 0X01);	//发送CMD41
			} while (r1 && retry--);
			if (retry && SD_SendCmd(CMD58, 0, 0X01) == 0) {	//鉴别SD2.0开始
				SPI_ReadWrite_Byte(4, 1);//Get trailing return value of R7 resp
				if (spi2_tx[0] & 0x40)
					CardType = SD_TYPE_V2HC;    //检测CCS
				else
					CardType = SD_TYPE_V2;
			}
		}
	}
	if (!CardType)    //无卡
	//SD V1.x/ MMC	V3
	{
		SD_SendCmd(CMD55, 0, 0X01);		//发送CMD55
		r1 = SD_SendCmd(CMD41, 0, 0X01);	//发送CMD41
		if (r1 <= 1) {
			CardType = SD_TYPE_V1;
			retry = 0XFFFE;
			do {
				SD_SendCmd(CMD55, 0, 0X01);	//发送CMD55
				r1 = SD_SendCmd(CMD41, 0, 0X01);	//发送CMD41
			} while (r1 && retry--);
		} else {
			CardType = SD_TYPE_MMC;	//MMC V3
			retry = 0XFFFE;
			do {
				r1 = SD_SendCmd(CMD1, 0, 0X01);
			} while (r1 && retry--);
		}
		if (retry == 0 || SD_SendCmd(CMD16, 512, 0X01) != 0)
			CardType = SD_TYPE_ERR;
	}

	SD_DisSelect();
	SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
	if (CardType)
		return 0;
	else if (r1)
		return r1;
	return 0xaa;
}

//获得响应
//Response 要得到的回应?
//其他响应错误
u8 SD_GetResponse(u8 Response) {
	u16 Count = 0xFFFF;   		//等待次数
	while ((SPI_ReadWrite_Byte(1, 1) != Response) && Count)
		Count--;
	if (Count == 0)
		return MSD_RESPONSE_FAILURE;
	else
		return MSD_RESPONSE_NO_ERROR;
}
//从SD卡读取一个数据包的内容
//buf数据缓存区
//len读取的数据长度
u8 SD_RecvData(u16 len) {
	if (SD_GetResponse(0xFE))
		return 1;
	SPI_ReadWrite_Byte(len, 1);
	u8 a0 = spi2_tx[0];
	u8 a1 = spi2_tx[1];
	SPI_ReadWrite_Byte(2, 1);
	spi2_tx[0] = a0;
	spi2_tx[1] = a1;

	return 0;
}
//获取SD卡的CID信息包括 包括制造商信息
//u8 *cid_data	存放CID的内存至少16个字节
//返回 0:NO_ERR
//		 1:错误
u8 SD_GetCID() {
	u8 r1;
	r1 = SD_SendCmd(CMD10, 0, 0x01);
	if (r1 == 0x00) {
		r1 = SD_RecvData(16);	//接收16个字节的数据
	}
	SD_DisSelect();
	if (r1)
		return 1;
	else
		return 0;
}
//获取SD卡的CSD信息包括 包括容量和速度信息
//u8 *cid_data	存放CsD的内存至少16个字节
//返回 0:NO_ERR
//		 1:错误
u8 SD_GetCSD() {
	u8 r1;
	r1 = SD_SendCmd(CMD9, 0, 0x01);
	if (r1 == 0) {
		r1 = SD_RecvData(16);
	}
	SD_DisSelect();
	if (r1)
		return 1;
	else
		return 0;
}

//获取SD卡总扇区数
//返回值:0: 取出容量出错
//      其他:SD容量(扇区数/512字节)
u32 SD_GetSectorCount() {
	u32 Capacity;
	u8 n;
	u16 csize;
	//取CSD信息
	if (SD_GetCSD() != 0)
		return 0;

	if ((spi2_tx[0] & 0xC0) == 0x40)	 //V2.00卡
			{
		csize = spi2_tx[9] + ((u16) spi2_tx[8] << 8) + 1;
		Capacity = (u32) csize << 10;	 //得到扇区数
	} else	 //V1.XX卡
	{
		n = (spi2_tx[5] & 15) + ((spi2_tx[10] & 128) >> 7)
				+ ((spi2_tx[9] & 3) << 1) + 2;
		csize = (spi2_tx[8] >> 6) + ((u16) spi2_tx[7] << 2)
				+ ((u16) (spi2_tx[6] & 3) << 10) + 1;
		Capacity = (u32) csize << (n - 9);	 //得到扇区数
	}
	return Capacity;
}

//向SD卡写入一个数据包的内容512字节
//buf数据缓存
//命令
//返回值0成功 其他失败
u8 SD_SendBlock(const u8 *buf, u8 cmd) {
	u16 t;
	if (SD_WaitReady())
		return 1;
	spi2_tx[0] = cmd;
	SPI_ReadWrite_Byte(1, 0);
	if (cmd != 0XFD) {
		memcpy(spi2_tx, buf, 512);
		SPI_ReadWrite_Byte(512, 0);
		t = SPI_ReadWrite_Byte(3, 1);
		if ((t & 0x1F) != 0x05)
			return 2;
	}
	return 0;
}

//u8*buf数据缓存
//sector起始扇区
//扇区数
u8 SD_WriteDisk(const u8 *buf, u32 sector, u8 cnt) {
	u8 r1;
	if (CardType != SD_TYPE_V2HC)
		sector *= 512;
	if (cnt == 1) {
		r1 = SD_SendCmd(CMD24, sector, 0X01);
		if (r1 == 0) {
			r1 = SD_SendBlock(buf, 0xFE);
		}
	} else {
		if (CardType != SD_TYPE_MMC) {
			SD_SendCmd(CMD55, 0, 0X01);
			SD_SendCmd(CMD23, cnt, 0X01);
		}
		r1 = SD_SendCmd(CMD25, sector, 0X01);
		if (r1 == 0) {
			do {
				r1 = SD_SendBlock(buf, 0xFC);
				buf += 512;
			} while (--cnt && r1 == 0);
			r1 = SD_SendBlock(0, 0xFD);
		}
	}
	SD_DisSelect();
	//	return r1;
	return 0; //RES_OK;
}
u8 SD_ReadDisk(u8 *buf, u32 sector, u8 cnt) {
	u8 r1;
	if (CardType != SD_TYPE_V2HC)
		sector <<= 9;
	if (cnt == 1) {
		r1 = SD_SendCmd(CMD17, sector, 0X01);
		if (r1 == 0) {
			r1 = SD_RecvData(512);
			memcpy(buf, spi2_tx, 512);
		}
	} else {
		r1 = SD_SendCmd(CMD18, sector, 0X01);
		do {
			r1 = SD_RecvData(512);
			memcpy(buf, spi2_tx, 512);
			buf += 512;
		} while (--cnt && r1 == 0);
		SD_SendCmd(CMD12, 0, 0X01);
	}
	SD_DisSelect();
//	return r1;
	return 0; //RES_OK;

}
void Long2Long(u8 *buf, u8 geshu) {
	u8 x;
	for (u8 a = 0; a < geshu; a++) {
		x = buf[a * 4 + 0];
		buf[a * 4 + 0] = buf[a * 4 + 3];
		buf[a * 4 + 3] = x;
		x = buf[a * 4 + 1];
		buf[a * 4 + 1] = buf[a * 4 + 2];
		buf[a * 4 + 2] = x;
	}

}
/**/
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo) {
	SD_Error errorstatus = SD_OK;
	uint8_t tmp = 0;

	cardinfo->CardType = (uint8_t) CardType;
	//cardinfo->RCA = (uint16_t)RCA;
	if (SD_GetCSD() != 0)
		return 0;
	u32 *CSD_Tab = (u32*) spi2_tx;
	Long2Long(spi2_tx, 4);
	/* Byte 0 */
	tmp = (uint8_t) ((CSD_Tab[0] & 0xFF000000) >> 24);
	cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
	cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
	cardinfo->SD_csd.Reserved1 = tmp & 0x03;

	/* Byte 1 */
	tmp = (uint8_t) ((CSD_Tab[0] & 0x00FF0000) >> 16);
	cardinfo->SD_csd.TAAC = tmp;

	/* Byte 2 */
	tmp = (uint8_t) ((CSD_Tab[0] & 0x0000FF00) >> 8);
	cardinfo->SD_csd.NSAC = tmp;

	/* Byte 3 */
	tmp = (uint8_t) (CSD_Tab[0] & 0x000000FF);
	cardinfo->SD_csd.MaxBusClkFrec = tmp;

	/* Byte 4 */
	tmp = (uint8_t) ((CSD_Tab[1] & 0xFF000000) >> 24);
	cardinfo->SD_csd.CardComdClasses = tmp << 4;

	/* Byte 5 */
	tmp = (uint8_t) ((CSD_Tab[1] & 0x00FF0000) >> 16);
	cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
	cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

	/* Byte 6 */
	tmp = (uint8_t) ((CSD_Tab[1] & 0x0000FF00) >> 8);
	cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
	cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
	cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
	cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
	cardinfo->SD_csd.Reserved2 = 0; /* Reserved */

	if ((CardType == SD_TYPE_V1) || (CardType == SD_TYPE_V2)) {
		cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

		/* Byte 7 */
		tmp = (uint8_t) (CSD_Tab[1] & 0x000000FF);
		cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

		/* Byte 8 */
		tmp = (uint8_t) ((CSD_Tab[2] & 0xFF000000) >> 24);
		cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

		cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
		cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

		/* Byte 9 */
		tmp = (uint8_t) ((CSD_Tab[2] & 0x00FF0000) >> 16);
		cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
		cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
		cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
		/* Byte 10 */
		tmp = (uint8_t) ((CSD_Tab[2] & 0x0000FF00) >> 8);
		cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;

		cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1);
		cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
		cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
		cardinfo->CardCapacity *= cardinfo->CardBlockSize;
	} else if (CardType == SD_TYPE_V2HC) {
		/* Byte 7 */
		tmp = (uint8_t) (CSD_Tab[1] & 0x000000FF);
		cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

		/* Byte 8 */
		tmp = (uint8_t) ((CSD_Tab[2] & 0xFF000000) >> 24);

		cardinfo->SD_csd.DeviceSize |= (tmp << 8);

		/* Byte 9 */
		tmp = (uint8_t) ((CSD_Tab[2] & 0x00FF0000) >> 16);

		cardinfo->SD_csd.DeviceSize |= (tmp);

		/* Byte 10 */
		tmp = (uint8_t) ((CSD_Tab[2] & 0x0000FF00) >> 8);

//		cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
		cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
		cardinfo->CardCapacity <<=19;
		cardinfo->CardBlockSize = 512;
	}

	cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
	cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

	/* Byte 11 */
	tmp = (uint8_t) (CSD_Tab[2] & 0x000000FF);
	cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
	cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

	/* Byte 12 */
	tmp = (uint8_t) ((CSD_Tab[3] & 0xFF000000) >> 24);
	cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
	cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
	cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
	cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

	/* Byte 13 */
	tmp = (uint8_t) ((CSD_Tab[3] & 0x00FF0000) >> 16);
	cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
	cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
	cardinfo->SD_csd.Reserved3 = 0;
	cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

	/* Byte 14 */
	tmp = (uint8_t) ((CSD_Tab[3] & 0x0000FF00) >> 8);
	cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
	cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
	cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
	cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
	cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
	cardinfo->SD_csd.ECC = (tmp & 0x03);

	/* Byte 15 */
	tmp = (uint8_t) (CSD_Tab[3] & 0x000000FF);
	cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
	cardinfo->SD_csd.Reserved4 = 1;

	SD_GetCID();
	u32 *CID_Tab = CSD_Tab;
	Long2Long(spi2_tx, 4);

	/* Byte 0 */
	tmp = (uint8_t) ((CID_Tab[0] & 0xFF000000) >> 24);
	cardinfo->SD_cid.ManufacturerID = tmp;

	/* Byte 1 */
	tmp = (uint8_t) ((CID_Tab[0] & 0x00FF0000) >> 16);
	cardinfo->SD_cid.OEM_AppliID = tmp << 8;

	/* Byte 2 */
	tmp = (uint8_t) ((CID_Tab[0] & 0x000000FF00) >> 8);
	cardinfo->SD_cid.OEM_AppliID |= tmp;

	/* Byte 3 */
	tmp = (uint8_t) (CID_Tab[0] & 0x000000FF);
	cardinfo->SD_cid.ProdName1 = tmp << 24;

	/* Byte 4 */
	tmp = (uint8_t) ((CID_Tab[1] & 0xFF000000) >> 24);
	cardinfo->SD_cid.ProdName1 |= tmp << 16;

	/* Byte 5 */
	tmp = (uint8_t) ((CID_Tab[1] & 0x00FF0000) >> 16);
	cardinfo->SD_cid.ProdName1 |= tmp << 8;

	/* Byte 6 */
	tmp = (uint8_t) ((CID_Tab[1] & 0x0000FF00) >> 8);
	cardinfo->SD_cid.ProdName1 |= tmp;

	/* Byte 7 */
	tmp = (uint8_t) (CID_Tab[1] & 0x000000FF);
	cardinfo->SD_cid.ProdName2 = tmp;

	/* Byte 8 */
	tmp = (uint8_t) ((CID_Tab[2] & 0xFF000000) >> 24);
	cardinfo->SD_cid.ProdRev = tmp;

	/* Byte 9 */
	tmp = (uint8_t) ((CID_Tab[2] & 0x00FF0000) >> 16);
	cardinfo->SD_cid.ProdSN = tmp << 24;

	/* Byte 10 */
	tmp = (uint8_t) ((CID_Tab[2] & 0x0000FF00) >> 8);
	cardinfo->SD_cid.ProdSN |= tmp << 16;

	/* Byte 11 */
	tmp = (uint8_t) (CID_Tab[2] & 0x000000FF);
	cardinfo->SD_cid.ProdSN |= tmp << 8;

	/* Byte 12 */
	tmp = (uint8_t) ((CID_Tab[3] & 0xFF000000) >> 24);
	cardinfo->SD_cid.ProdSN |= tmp;

	/* Byte 13 */
	tmp = (uint8_t) ((CID_Tab[3] & 0x00FF0000) >> 16);
	cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
	cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

	/* Byte 14 */
	tmp = (uint8_t) ((CID_Tab[3] & 0x0000FF00) >> 8);
	cardinfo->SD_cid.ManufactDate |= tmp;

	/* Byte 15 */
	tmp = (uint8_t) (CID_Tab[3] & 0x000000FF);
	cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
	cardinfo->SD_cid.Reserved2 = 1;

	return (errorstatus);
}
