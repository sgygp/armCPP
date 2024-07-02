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
//COMDATA COM::m_data1;
COMDATA COM::m_data2;
//COMDATA COM::m_data3;

COM::COM(u8 no) {
	// TODO 自动生成的构造函数存根
	m_NO = no;
	if(no==0)
		m_data =&m_data2;
	else if(no==1)
		m_data = &m_data2;
	else if(no==2)
		m_data = &m_data2;
}

COM::~COM() {
	// TODO 自动生成的析构函数存根
}

void COM::init(u32 baud) {
	if (m_NO == 0)
		init1(baud);
	else if (m_NO == 1)
		init2(baud);
	else if (m_NO == 2)
		init3(baud);
	memset(m_data->m_FA, 0, sizeof(m_data->m_FA));
	memset(m_data->m_SHOU, 0, sizeof(m_data->m_SHOU));
	m_data->m_curShou_ms5 = 0;
	m_data->m_faPos = 0;
	m_data->m_shouPos = 0;
	m_data->m_yaofaShu = 0;
}
void COM::init1(u32 baud) {
	m_data = &m_data2;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	USART_ClockInitTypeDef USART_ClockInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the PWR/BKP Clock */

	RCC_APB2PeriphClockCmd(
	RCC_APB2Periph_USART1 , ENABLE); //使能USART1,GPIOA,C时钟
/*	//USART1_TX   GPIOA.9

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //初始化GPIOA.9
	//USART1_RX      GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    //PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //初始化GPIOA.10
*/

	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);  //引脚复用配置

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);    //初始化GPIOA.9

	//USART1_RX      GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;    //PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);    //初始化GPIOA.10


	USART_DeInit(USART1);          //将外设baud寄存器重设为缺省值
	USART_InitStructure.USART_BaudRate = baud;     //设置串口1波特率为19200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //设置一个帧中传输数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //定义发送的停止位数目为1
	USART_InitStructure.USART_Parity = USART_Parity_No;     //奇偶失能
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;  //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //发送接收使能
	USART_Init(USART1, &USART_InitStructure);

//	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;     //时钟低电平活动
//	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low; //设置SCLK引脚上时钟输出的极性为低电平
//	USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;   //时钟第一个边沿进行数据捕获
//	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable; //最后一位数据的时钟脉冲不从SCLK输出
//	USART_ClockInit(USART1, &USART_ClockInitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //使能USART1接收中断
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //使能USART1接收中断
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);	//经过试验，TXE应该是默认使能中断的，必须关闭
	USART_Cmd(USART1, ENABLE);      //使能USART1外设

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void COM::init2(u32 baud) {
	m_data = &m_data2;
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure USART2 Tx (PB.10) as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);	//复位串口2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);	//停止复位

	//配置串口
	USART_InitStructure.USART_BaudRate = baud;
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

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
void COM::init3(u32 baud) {
	m_data = &m_data2;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO
	//使能串口的RCC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能UART2所在GPIOB的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//串口使用的GPIO口配置
	// Configure USART3 Rx (PB.11) as input floating
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Configure USART3 Tx (PB.10) as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);	//复位串口3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);	//停止复位

	//配置串口
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// Configure USART3
	USART_Init(USART3, &USART_InitStructure);      //配置串口3
	// Enable USART3 Receive interrupts 使能串口接收中断 发送中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//USART_IT_TXEN发送数据寄存器空中断 USART_IT_TC发送数据完成中断
	USART_ITConfig(USART3, USART_IT_TC, ENABLE);//USART_IT_TXEN发送数据寄存器空中断 USART_IT_TC发送数据完成中断
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);	//经过试验，TXE应该是默认使能中断的，必须关闭
	// Enable the USART3
	USART_Cmd(USART3, ENABLE);      //使能串口3

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void COM::FasongX(u16 geshu) {

	if (geshu == 0)
		return;
/*
	if(geshu>10)
		geshu=10;
*/

	USART_TypeDef *uartx;
	if (m_NO == 0)
		uartx = USART1;
	else if (m_NO == 1) {
		uartx = USART2;
	} else if (m_NO == 2) {
		uartx = USART3;
	} else
		return;

	for (int i = 0; i < 1000; i++)        //检测是否发送完成
			{
		if (m_data->m_bFasongWan)
			break;
		delay_us(1);
	}

	m_data->m_yaofaShu = geshu;
	m_data->m_bFasongWan = false;
	m_data->m_faPos = 1;
	USART_SendData(uartx, m_data->m_FA[0]);
}


extern "C" void USART1_IRQHandler(void) {
	COMDATA *ddd = &COM::m_data2;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		ddd->m_SHOU[ddd->m_shouPos] = USART_ReceiveData(USART1); //读取接收到的数据
		ddd->m_shouPos++;

		ddd->m_curShou_ms5 = Timer::ms5_jishu;
	}
	if (USART_GetITStatus(USART1, USART_IT_ORE) == SET) {
		USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_ORE);
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
		USART_ClearFlag(USART1, USART_FLAG_NE); //一定要清除接收中断
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
extern "C" void USART2_IRQHandler(void)                    //串口2中断服务程序
{
	COMDATA *ddd = &COM::m_data2;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		ddd->m_SHOU[ddd->m_shouPos] = USART_ReceiveData(USART2); //读取接收到的数据
		ddd->m_shouPos++;

		ddd->m_curShou_ms5 = Timer::ms5_jishu;
	}
	if (USART_GetITStatus(USART2, USART_IT_ORE) == SET) {
		USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2, USART_IT_ORE);
	}
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
		USART_ClearFlag(USART2, USART_FLAG_NE); //一定要清除接收中断
	if (USART_GetITStatus(USART2, USART_IT_TC) == SET) {
		USART_ClearITPendingBit(USART2, USART_IT_TC);

		if (ddd->m_faPos < ddd->m_yaofaShu) //
				{
			USART_SendData(USART2, ddd->m_FA[ddd->m_faPos]);
			ddd->m_faPos++;
		} else
			ddd->m_bFasongWan = true;

	}
}


extern "C" void USART3_IRQHandler(void)                    //串口3中断服务程序
{
	COMDATA *ddd = &COM::m_data2;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		ddd->m_SHOU[ddd->m_shouPos] = USART_ReceiveData(USART3); //读取接收到的数据
		ddd->m_shouPos++;

		ddd->m_curShou_ms5 = Timer::ms5_jishu;
	}
	if (USART_GetITStatus(USART3, USART_IT_ORE) == SET) {
		USART_ReceiveData(USART3);
		USART_ClearITPendingBit(USART3, USART_IT_ORE);
	}
	if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
		USART_ClearFlag(USART3, USART_FLAG_NE); //一定要清除接收中断
	if (USART_GetITStatus(USART3, USART_IT_TC) == SET) {
		USART_ClearITPendingBit(USART3, USART_IT_TC);

		if (ddd->m_faPos < ddd->m_yaofaShu) //
				{
			USART_SendData(USART3, ddd->m_FA[ddd->m_faPos]);
			ddd->m_faPos++;
		} else
			ddd->m_bFasongWan = true;

	}
}
