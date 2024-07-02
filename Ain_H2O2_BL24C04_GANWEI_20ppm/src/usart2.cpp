#include <usart2.h>
#include "Timer.h"
#include "DwinDGUS.h"
u8 DwinDGUS::m_SHOU[];
u8 DwinDGUS::m_FA[];
u8 DwinDGUS::m_shouPos;
u32 DwinDGUS::m_curShou_ms;
bool DwinDGUS::m_bOK;
u8 DwinDGUS::m_yaofaShu;
u8 DwinDGUS::m_faPos;

/**************************************************************************************
 * 描  述 : 初始化USART1并配置USART1中断优先级
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void USART2_Init(u32 bound) {
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO
	//使能串口的RCC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); //使能UART2所在GPIOB的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	//串口使用的GPIO口配置
	// Configure USART2 Rx (PB.11) as input floating
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure USART2 Tx (PB.10) as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);	//复位串口2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);	//停止复位

	//配置串口
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// Configure USART2
	USART_Init(USART2, &USART_InitStructure);      //配置串口2
	// Enable USART2 Receive interrupts 使能串口接收中断 发送中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//USART_IT_TXEN发送数据寄存器空中断 USART_IT_TC发送数据完成中断
	USART_ITConfig(USART2, USART_IT_TC, ENABLE);//USART_IT_TXEN发送数据寄存器空中断 USART_IT_TC发送数据完成中断
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);	//经过试验，TXE应该是默认使能中断的，必须关闭
	// Enable the USART2
	USART_Cmd(USART2, ENABLE);      //使能串口2

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //NVIC_Group:NVIC分组 0~4 总共5组,最高2位抢占
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_TX(u8 geshu) {
	if (geshu > 0) {
		DwinDGUS::m_bOK=false;
		DwinDGUS::m_yaofaShu = geshu;
		DwinDGUS::m_faPos = 0;
		USART_SendData(USART2, DwinDGUS::m_FA[DwinDGUS::m_faPos]);
		DwinDGUS::m_faPos++;
	}
}

extern "C" void USART2_IRQHandler(void)                    //串口2中断服务程序
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		DwinDGUS::m_SHOU[DwinDGUS::m_shouPos] = USART_ReceiveData(
		USART2);  //读取接收到的数据
		DwinDGUS::m_shouPos++;
		DwinDGUS::m_curShou_ms = Timer::ms5_jishu;
	}
	//溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET)
	{
		USART_ReceiveData(USART2);
		USART_ClearFlag(USART2, USART_FLAG_ORE);
	}
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
		USART_ClearFlag(USART2, USART_FLAG_RXNE); //一定要清除接收中断
	if (USART_GetITStatus(USART2, USART_IT_TC) == SET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_TC);

		if (DwinDGUS::m_faPos < DwinDGUS::m_yaofaShu) //
		{
			USART_SendData(USART2, DwinDGUS::m_FA[DwinDGUS::m_faPos]);
			DwinDGUS::m_faPos++;
		}else
			DwinDGUS::m_bOK=true;
	}
}

