#include <usart1.h>
#include "Timer.h"
#include "DwinDGUS.h"
u8 DwinDGUS::m_USART1_SHOU[];
u8 DwinDGUS::m_USART1_FA[];
u8 DwinDGUS::m_USART1_shouPos;
u32 DwinDGUS::m_USART1_curShou_ms;
bool DwinDGUS::m_bOK;

/**************************************************************************************
 * ��  �� : ��ʼ��USART1������USART1�ж����ȼ�
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void DMA_USART1_Init(u32 bound) {
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the PWR/BKP Clock */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE); //ʹ��USART1,GPIOA,Cʱ��
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //��ʼ��GPIOA.9

	//USART1_RX      GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    //PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //��ʼ��GPIOA.10

	USART_DeInit(USART1);          //������USART1�Ĵ�������Ϊȱʡֵ
	USART_InitStructure.USART_BaudRate = bound;     //���ô���1������Ϊ19200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //����һ��֡�д�������λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //���巢�͵�ֹͣλ��ĿΪ1
	USART_InitStructure.USART_Parity = USART_Parity_No;     //��żʧ��
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;  //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //���ͽ���ʹ��
	USART_Init(USART1, &USART_InitStructure);

//	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;     //ʱ�ӵ͵�ƽ�
//	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low; //����SCLK������ʱ������ļ���Ϊ�͵�ƽ
//	USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;   //ʱ�ӵ�һ�����ؽ������ݲ���
//	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable; //���һλ���ݵ�ʱ�����岻��SCLK���
//	USART_ClockInit(USART1, &USART_ClockInitStructure);

//	USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //ʹ��USART1�����ж�
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);            //ʹ��USART1����DMA����
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //ʹ��USART1�����ж�
	USART_Cmd(USART1, ENABLE);      //ʹ��USART1����

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //NVIC_Group:NVIC���� 0~4 �ܹ�5��,���2λ��ռ
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*
	 *  DMA���ã�
	 *  ����DMAԴ���ڴ��ַ&�������ݼĴ�����ַ
	 *  �����ڴ�-->����
	 *  ÿ�δ���λ��8bit
	 *  �����СDMA_BufferSize=SENDBUFF_SIZE
	 *  ��ַ����ģʽ�������ַ�������ڴ��ַ����1
	 *  DMAģʽ��һ�δ��䣬��ѭ��
	 *  ���ȼ�����
	 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);            //����DMAʱ��
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel4);    //����1��DMA����ͨ����ͨ��4
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) (&USART1->DR);     //����DMAԴ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32) DwinDGUS::m_USART1_FA; //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;             //������ΪDMA��Ŀ�Ķ�
	DMA_InitStructure.DMA_BufferSize = 1;           //�����С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //�����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //�ڴ��ַ����1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݿ��8bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�ڴ����ݿ��8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular ����ͣ�ش��ͣ�
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;           //(DMA�������ȼ�Ϊ��)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        //���ڴ浽�ڴ�
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);       //ʹ��DMAͨ��4��������ж�

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void DMA1_USART1_TX(u8 geshu) {
//	DMA_ClearFlag(DMA1_FLAG_TC4);      //��DMA������ɱ�־
//	DMA_Cmd(DMA1_Channel4, DISABLE);   //ֹͣDMA
	DMA1_Channel4->CNDTR = geshu;   //���贫�䳤��
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DwinDGUS::m_bOK = false;
}

extern "C" void DMA1_Channel4_IRQHandler(void)  //DMA�ж�
		{
	//�����־λ
	DMA_ClearFlag(DMA1_FLAG_TC4);
	DwinDGUS::m_bOK = true;
	DMA_Cmd(DMA1_Channel4, DISABLE);

}

extern "C" void USART1_IRQHandler(void)                    //����1�жϷ������
		{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		DwinDGUS::m_USART1_SHOU[DwinDGUS::m_USART1_shouPos] = USART_ReceiveData(
		USART1);  //��ȡ���յ�������
		DwinDGUS::m_USART1_shouPos++;
		DwinDGUS::m_USART1_curShou_ms = Timer::ms5_jishu;
	}
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET) {
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_ORE);
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
		USART_ClearFlag(USART1, USART_FLAG_RXNE); //һ��Ҫ��������ж�
//	if (USART_GetITStatus(USART1, USART_IT_TC) == SET)
//	{
//		USART_ClearITPendingBit(USART1, USART_IT_TC);
//
//		if (DwinDGUS::m_USART1_faPos < DwinDGUS::m_USART1_yaofaShu) //
//		{
//			USART_SendData(USART1, DwinDGUS::m_USART1_FA[DwinDGUS::m_USART1_faPos]);
//			DwinDGUS::m_USART1_faPos++;
//		}
//	}
}

