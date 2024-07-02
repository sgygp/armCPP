/*
 * COM.cpp
 *
 *  Created on: 2021年3月9日
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
	// TODO 自动生成的构造函数存根
	m_no = no;
	m_data = &m_data0;
}

COM::~COM() {
	// TODO 自动生成的析构函数存根
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
	//声明一个GPIO结构体变量
	GPIO_InitTypeDef GPIO_InitStructure;
	//声明一个USART结构体变量
	USART_InitTypeDef USART_InitStructure;
	//使能USART1外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//使能GPIO外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//定义GPIO结构体变量，复用该IO口作为串口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;	//两个IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         //复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//定义USART结构体变量
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;		   //硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //收发模式
	USART_InitStructure.USART_Parity = USART_Parity_No;    //无校准
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //字长为8
	//明确IO为何功能进行配置
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	//串口进行初始化
	USART_Init(USART1, &USART_InitStructure);
	//串口中断优先组结构体变量声明
	NVIC_InitTypeDef NVIC_InitStructure_usart;
	//定义该串口中断优先组分组
	NVIC_InitStructure_usart.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure_usart.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure_usart.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级设置
	NVIC_InitStructure_usart.NVIC_IRQChannelSubPriority = 0x02;		//响应优先级设置
	//初始化该优先级分组
	NVIC_Init(&NVIC_InitStructure_usart);

	//配置串口接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//接收缓冲区不空中断（收到数据）
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);	//发送完成中断
	//使能串口1
	USART_Cmd(USART1, ENABLE);

}
void COM::init1(u32 baud) {
	m_data = &m_data1;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;	//C10 C11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         //复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//明确IO为何功能进行配置
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3); //
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);	//复位串口3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);	//停止复位

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//485EN A12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		   //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	enableRX1();

//配置串口
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

// Configure USART3
	USART_Init(USART3, &USART_InitStructure);      //配置串口3
// Enable USART3 Receive interrupts 使能串口接收中断 发送中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //USART_IT_TXEN发送数据寄存器空中断 USART_IT_TC发送数据完成中断
	USART_ITConfig(USART3, USART_IT_TC, ENABLE); //USART_IT_TXEN发送数据寄存器空中断 USART_IT_TC发送数据完成中断
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);	//经过试验，TXE应该是默认使能中断的，必须关闭
// Enable the USART3
	USART_Cmd(USART3, ENABLE);      //使能串口3

	NVIC_InitTypeDef NVIC_InitStructure;
//使能串口的RCC时钟
// Enable the USART3 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;      //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //子优先级2
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         //复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//D2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         //复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		   //上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//明确IO为何功能进行配置
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); //
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, ENABLE);	//复位串口5
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, DISABLE);	//停止复位

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//485EN A12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;    //50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		   //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	enableRX2();

	//配置串口
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// Configure UART5
	USART_Init(UART5, &USART_InitStructure);      //配置串口5
	// Enable UART5 Receive interrupts 使能串口接收中断 发送中断
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//USART_IT_TXEN发送数据寄存器空中断 USART_IT_TC发送数据完成中断
	USART_ITConfig(UART5, USART_IT_TC, ENABLE);	//USART_IT_TXEN发送数据寄存器空中断 USART_IT_TC发送数据完成中断
	USART_ITConfig(UART5, USART_IT_TXE, DISABLE);	//经过试验，TXE应该是默认使能中断的，必须关闭
	// Enable the UART5
	USART_Cmd(UART5, ENABLE);      //使能串口5

	NVIC_InitTypeDef NVIC_InitStructure;
	//使能串口的RCC时钟
	// Enable the UART5 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;      //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //子优先级2
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
	for (int i = 0; i < 1000; i++)        //检测是否发送完成
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
		ddd->m_SHOU[ddd->m_shouPos] = USART_ReceiveData(USART1); //读取接收到的数据
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

extern "C" void USART3_IRQHandler(void)                    //串口3中断服务程序
		{
	COMDATA *ddd = &COM::m_data1;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		ddd->m_SHOU[ddd->m_shouPos] = USART_ReceiveData(USART3);  //读取接收到的数据
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

extern "C" void UART5_IRQHandler(void)                    //串口5中断服务程序
		{
	COMDATA *ddd = &COM::m_data2;
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		ddd->m_SHOU[ddd->m_shouPos] = USART_ReceiveData(UART5);  //读取接收到的数据
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
