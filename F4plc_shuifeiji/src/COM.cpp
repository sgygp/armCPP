/*
 * COM.cpp
 *
 *  Created on: 2021��3��9��
 *      Author: Administrator
 */

#include "COM.h"
#include "Timer.h"
#include "delay.h"
#include "string.h"
COMDATA COM::m_data0;
COMDATA COM::m_data1;
COMDATA COM::m_data2;

COM::COM(u8 no) {
	// TODO �Զ����ɵĹ��캯�����
	m_no = no;
	m_data = &m_data0;
}

COM::~COM() {
	// TODO �Զ����ɵ������������
}

void COM::init(u32 baud) {
	if (m_no == 0)
		init0(baud);
	else if (m_no == 1)
		init1(baud);
	else if (m_no == 2)
		init2(baud);
	memset(m_data->m_FA, 0, 256);
	memset(m_data->m_SHOU, 0, 256);
	m_data->m_curShou_ms2 = 0;
	m_data->m_faPos = 0;
	m_data->m_shouPos = 0;
	m_data->m_yaofaShu = 0;
}
void COM::init0(u32 baud) {
	m_data = &m_data0;
	//����һ��GPIO�ṹ�����
	GPIO_InitTypeDef GPIO_InitStructure;
	//����һ��USART�ṹ�����
	USART_InitTypeDef USART_InitStructure;
	//ʹ��USART1����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//ʹ��GPIO����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//����GPIO�ṹ����������ø�IO����Ϊ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;	//����IO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//����USART�ṹ�����
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;		   //Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //�շ�ģʽ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //��У׼
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //1λֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //�ֳ�Ϊ8
	//��ȷIOΪ�ι��ܽ�������
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	//���ڽ��г�ʼ��
	USART_Init(USART1, &USART_InitStructure);
	//�����ж�������ṹ���������
	NVIC_InitTypeDef NVIC_InitStructure_usart;
	//����ô����ж����������
	NVIC_InitStructure_usart.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure_usart.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure_usart.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�����
	NVIC_InitStructure_usart.NVIC_IRQChannelSubPriority = 0x02;		//��Ӧ���ȼ�����
	//��ʼ�������ȼ�����
	NVIC_Init(&NVIC_InitStructure_usart);

	//���ô��ڽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//���ջ����������жϣ��յ����ݣ�
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);	//��������ж�
	//ʹ�ܴ���1
	USART_Cmd(USART1, ENABLE);

}
void COM::init1(u32 baud) {
	m_data = &m_data1;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	//C10 C11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//��ȷIOΪ�ι��ܽ�������
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3); //
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);	//��λ����3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);	//ֹͣ��λ

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//485EN A12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		   //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	enableRX1();

//���ô���
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

// Configure USART3
	USART_Init(USART3, &USART_InitStructure);      //���ô���3
// Enable USART3 Receive interrupts ʹ�ܴ��ڽ����ж� �����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //USART_IT_TXEN�������ݼĴ������ж� USART_IT_TC������������ж�
	USART_ITConfig(USART3, USART_IT_TC, ENABLE); //USART_IT_TXEN�������ݼĴ������ж� USART_IT_TC������������ж�
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);	//�������飬TXEӦ����Ĭ��ʹ���жϵģ�����ر�
// Enable the USART3
	USART_Cmd(USART3, ENABLE);      //ʹ�ܴ���3

	NVIC_InitTypeDef NVIC_InitStructure;
//ʹ�ܴ��ڵ�RCCʱ��
// Enable the USART3 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;      //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
void COM::init2(u32 baud) {
	m_data = &m_data2;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	RCC_AHB1PeriphClockCmd(
	RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//C12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//D2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   //����
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//��ȷIOΪ�ι��ܽ�������
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); //
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, ENABLE);	//��λ����5
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, DISABLE);	//ֹͣ��λ

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//485EN A12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		   //����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	enableRX2();

	//���ô���
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// Configure UART5
	USART_Init(UART5, &USART_InitStructure);      //���ô���5
	// Enable UART5 Receive interrupts ʹ�ܴ��ڽ����ж� �����ж�
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//USART_IT_TXEN�������ݼĴ������ж� USART_IT_TC������������ж�
	USART_ITConfig(UART5, USART_IT_TC, ENABLE);	//USART_IT_TXEN�������ݼĴ������ж� USART_IT_TC������������ж�
	USART_ITConfig(UART5, USART_IT_TXE, DISABLE);	//�������飬TXEӦ����Ĭ��ʹ���жϵģ�����ر�
	// Enable the UART5
	USART_Cmd(UART5, ENABLE);      //ʹ�ܴ���5

	NVIC_InitTypeDef NVIC_InitStructure;
	//ʹ�ܴ��ڵ�RCCʱ��
	// Enable the UART5 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;      //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
void COM::FasongX(u8 geshu) {
	if (geshu == 0)
		return;
	USART_TypeDef *uartx;
	if (m_no == 0)
		uartx = USART1;
	else if (m_no == 1) {
		uartx = USART3;
		enableTX1();
	} else if (m_no == 2) {
		uartx = UART5;
		enableTX2();
	} else
		return;
	for (int i = 0; i < 1000; i++)        //����Ƿ������
			{
		if (m_data->m_bFasongWan)
			break;
		Delay::delay_us(1);
	}
	m_data->m_yaofaShu = geshu;
	m_data->m_bFasongWan = false;
	m_data->m_faPos = 1;
	USART_SendData(uartx, m_data->m_FA[0]);
}

void COM::FasongX(u8* data, u8 geshu) {
	memcpy(m_data->m_FA,data,geshu);
	FasongX(geshu);
}

extern "C" void USART1_IRQHandler(void) {
	COMDATA *ddd = &COM::m_data0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		ddd->m_SHOU[ddd->m_shouPos] = USART_ReceiveData(USART1); //��ȡ���յ�������
		ddd->m_shouPos++;
		ddd->m_curShou_ms2 = Timer::ms2_jishu;
	}
	if (USART_GetITStatus(USART1, USART_IT_TC) == SET) {
		USART_ClearITPendingBit(USART1, USART_IT_TC);

		if (ddd->m_faPos < ddd->m_yaofaShu) //
				{
			USART_SendData(USART1, ddd->m_FA[ddd->m_faPos]);
			ddd->m_faPos++;
		} else
			ddd->m_bFasongWan = true;

	}

}

extern "C" void USART3_IRQHandler(void)                    //����3�жϷ������
		{
	COMDATA *ddd = &COM::m_data1;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		ddd->m_SHOU[ddd->m_shouPos] = USART_ReceiveData(USART3);  //��ȡ���յ�������
		ddd->m_shouPos++;
		ddd->m_curShou_ms2 = Timer::ms2_jishu;
	}
	if (USART_GetITStatus(USART3, USART_IT_TC) == SET) {
		USART_ClearITPendingBit(USART3, USART_IT_TC);

		if (ddd->m_faPos < ddd->m_yaofaShu) //
				{
			USART_SendData(USART3, ddd->m_FA[ddd->m_faPos]);
			ddd->m_faPos++;
		} else {
			COM::enableRX1();
			ddd->m_bFasongWan = true;
		}
	}
}

extern "C" void UART5_IRQHandler(void)                    //����5�жϷ������
		{
	COMDATA *ddd = &COM::m_data2;
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		ddd->m_SHOU[ddd->m_shouPos] = USART_ReceiveData(UART5);  //��ȡ���յ�������
		ddd->m_shouPos++;
		ddd->m_curShou_ms2 = Timer::ms2_jishu;
	}
	if (USART_GetITStatus(UART5, USART_IT_TC) == SET) {
		USART_ClearITPendingBit(UART5, USART_IT_TC);

		if (ddd->m_faPos < ddd->m_yaofaShu) //
				{
			USART_SendData(UART5, ddd->m_FA[ddd->m_faPos]);
			ddd->m_faPos++;
		} else {
			COM::enableRX2();
			ddd->m_bFasongWan = true;
		}
	}
}
