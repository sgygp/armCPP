/*
 * SPIC.cpp
 *
 *  Created on: 2023��4��18��
 *      Author: Administrator
 */
//
//unsigned char ADXL345RXBuff[250];     //DMA ���ջ���
//unsigned char ADXL345TXBuff[10];      //DMA ���ͻ���
#include <SPI2C.h>
//#include "diag/Trace.h"

#include "string.h"
#include "delay.h"

u8 SPI2_C::tx[]={0};
u8 SPI2_C::m_bOK = 0;

void SPI2_C::init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); //ʹ��SPI2ʱ��

	GPIO_InitStructure.GPIO_Pin = SPI2_CLK_PIN | SPI2_MISO_PIN | SPI2_MOSI_PIN; //PB3~5���ù������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_PORT, &GPIO_InitStructure);//��ʼ��GPIOB

	GPIO_InitStructure.GPIO_Pin = GPIO_SPI2NSS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI2_PORT, &GPIO_InitStructure); //��ʼ��
	CS_H2;

	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE); //��λSPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE); //ֹͣ��λSPI2


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����




	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_StructInit(&DMA_InitStructure);
	DMA_DeInit(DMA1_Channel4);
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(SPI2->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) tx; //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;             //������ΪDMA��Ŀ�Ķ�
	DMA_InitStructure.DMA_BufferSize = sizeof(tx);           //�����С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //�����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //�ڴ��ַ����1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݿ��8bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�ڴ����ݿ��8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular ����ͣ�ش��ͣ�
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;           //(DMA�������ȼ�Ϊ��)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        //���ڴ浽�ڴ�
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);				//ʹ��DMAͨ��5 TX
	//	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);       //ʹ��DMAͨ��5 TX�ж�

	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(SPI2->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) tx;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);                   //ʹ��DMAͨ��2 RX
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);       //ʹ��DMAͨ��2 RX�ж�

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
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));	//�ж���Ч��
	SPI2->CR1 &= 0XFFC7;	//λ3-5���㣬�������ò�����
	SPI2->CR1 |= SPI_BaudRatePrescaler;	//����SPI2�ٶ�
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI2
}
*/

u8 SPI2_C::SPI_ReadWrite_Byte(u16 size, bool isFillFF)         //ÿ������ܳ���512�ֽ�,�������һ���յ�������
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
	for (int i = 0; i < 10000; i++) {                  //�ȴ��100����
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



extern "C" void DMA1_Channel4_IRQHandler(void)  //DMA�����ж�
{
	DMA_ClearFlag(DMA1_FLAG_TC4);
	SPI2_C::m_bOK = 1;

}
