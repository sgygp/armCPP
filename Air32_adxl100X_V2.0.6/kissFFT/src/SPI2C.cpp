/*
 * SPIC.cpp
 *
 *  Created on: 2023年4月18日
 *      Author: Administrator
 */
//
//unsigned char ADXL345RXBuff[250];     //DMA 接收缓存
//unsigned char ADXL345TXBuff[10];      //DMA 发送缓存
#include <SPI2C.h>
//#include "diag/Trace.h"

#include "string.h"
#include "delay.h"

u8 SPI2_C::tx[]={0};
u8 SPI2_C::m_bOK = 0;

void SPI2_C::init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); //使能SPI2时钟

	GPIO_InitStructure.GPIO_Pin = SPI2_CLK_PIN | SPI2_MISO_PIN | SPI2_MOSI_PIN; //PB3~5复用功能输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_PORT, &GPIO_InitStructure);//初始化GPIOB

	GPIO_InitStructure.GPIO_Pin = GPIO_SPI2NSS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI2_PORT, &GPIO_InitStructure); //初始化
	CS_H2;

	//这里只针对SPI口初始化
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE); //复位SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE); //停止复位SPI2


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设




	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_StructInit(&DMA_InitStructure);
	DMA_DeInit(DMA1_Channel4);
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(SPI2->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) tx; //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;             //外设作为DMA的目的端
	DMA_InitStructure.DMA_BufferSize = sizeof(tx);           //传输大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //外设地址不增加
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //内存地址自增1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据宽度8bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //内存数据宽度8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;           //(DMA传送优先级为高)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        //非内存到内存
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);				//使能DMA通道5 TX
	//	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);       //使能DMA通道5 TX中断

	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(SPI2->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) tx;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);                   //使能DMA通道2 RX
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);       //使能DMA通道2 RX中断

	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	//	NVIC_Init(&NVIC_InitStructure);

}
/*
void SPI2_C::SPI2_SetSpeed(u8 SPI_BaudRatePrescaler) {
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	//判断有效性
	SPI2->CR1 &= 0XFFC7;	//位3-5清零，用来设置波特率
	SPI2->CR1 |= SPI_BaudRatePrescaler;	//设置SPI2速度
	SPI_Cmd(SPI2, ENABLE); //使能SPI2
}
*/

u8 SPI2_C::SPI_ReadWrite_Byte(u16 size, bool isFillFF)         //每次最大不能超过512字节,返回最后一个收到的数据
{
	if (size > (sizeof(tx)) || size == 0)
		return 0;
	if (isFillFF)
		memset(tx, 0xff, size);
	m_bOK = 0;
	CS_L2;
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	DMA1_Channel4->CNDTR = size;
	DMA1_Channel5->CNDTR = size;
//	DMA_SetCurrDataCounter(DMA1_Channel4,size);
//	DMA_SetCurrDataCounter(DMA1_Channel5,size);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	for (int i = 0; i < 10000; i++) {                  //等待最长100毫秒
		if (m_bOK) {
//			trace_printf("%d\n",i);
			break;
		}
		delay_us(10);
	}
	CS_H2;
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	return tx[size - 1];
}



extern "C" void DMA1_Channel4_IRQHandler(void)  //DMA接收中断
{
	DMA_ClearFlag(DMA1_FLAG_TC4);
	SPI2_C::m_bOK = 1;

}
