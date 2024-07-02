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
COMDATA COM::m_data[1];

COM::COM() {
	// TODO �Զ����ɵĹ��캯�����
}

COM::~COM() {
	// TODO �Զ����ɵ������������
}

void COM::init() {
//	init1(115200);
//	init2(19200);
	init3(115200);
//	init4(115200);
//	init5(115200);
//	init6(115200);
	memset(m_data, 0, sizeof(COMDATA)*1);
}


/**************************************************************************************/
void COM::init3(u32 bound) {

	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;    //����һ���ṹ�������������ʼ��GPIO
	//ʹ�ܴ��ڵ�RCCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //ʹ��UART3����GPIOB��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//����ʹ�õ�GPIO������
	// Configure USART3 Tx (PB.10) as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Configure USART3 Rx (PB.11) as input floating
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);	//��λ����3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);	//ֹͣ��λ

	//���ô���
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// Configure USART3
	USART_Init(USART3, &USART_InitStructure);      //���ô���3
	// Enable USART3 Receive interrupts ʹ�ܴ��ڽ����ж� �����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//USART_IT_TXEN�������ݼĴ������ж� USART_IT_TC������������ж�
	USART_ITConfig(USART3, USART_IT_TC, ENABLE);//USART_IT_TXEN�������ݼĴ������ж� USART_IT_TC������������ж�
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);	//�������飬TXEӦ����Ĭ��ʹ���жϵģ�����ر�
	// Enable the USART3
	USART_Cmd(USART3, ENABLE);      //ʹ�ܴ���3

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); //NVIC_Group:NVIC���� 0~4 �ܹ�5��,���3λ��ռ
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);

}



void COM::FasongX(u8 no,u8 geshu) {
	if (geshu == 0)
		return;
	USART_TypeDef *uartx;
	switch (no) {
	case 0:
		uartx = USART3;
		break;
/*
	case 1:
		uartx = USART2;
		break;
	case 2:
		uartx = USART3;
		break;
	case 3:
		uartx = UART4;
		break;
	case 4:
		uartx = UART5;
		break;
	case 5:
		uartx = USART6;
		break;
*/
	default:
			return;
	}
	for (int i = 0; i < 1000; i++)        //����Ƿ������
			{
		if (!m_data[no].m_bFasongzhong)
			break;
		delay_us(1);
	}
	GPIO_SetBits(GPIOB,GPIO_Pin_1);

	m_data[no].m_yaofaShu = geshu;
	m_data[no].m_bFasongzhong = true;
	m_data[no].m_faPos = 1;
	USART_SendData(uartx, m_data[no].m_FA[0]);
}

extern "C" void USART3_IRQHandler(void)                    //����2�жϷ������
{
	COMDATA *ddd = &COM::m_data[0];
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		ddd->m_SHOU[ddd->m_shouPos] = USART_ReceiveData(USART3);  //��ȡ���յ�������
		ddd->m_shouPos++;
		ddd->m_curShou_ms5 = Timer::ms5_jishu;
	}
	if (USART_GetITStatus(USART3, USART_IT_TC) == SET) {
		USART_ClearITPendingBit(USART3, USART_IT_TC);

		if (ddd->m_faPos < ddd->m_yaofaShu) //
				{
			USART_SendData(USART3, ddd->m_FA[ddd->m_faPos]);
			ddd->m_faPos++;
		} else {
			ddd->m_bFasongzhong = false;
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		}
	}
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET) {
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_ORE);
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
		USART_ClearFlag(USART1, USART_FLAG_RXNE); //һ��Ҫ��������ж�
}

