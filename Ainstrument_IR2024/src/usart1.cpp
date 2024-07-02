#include <usart1.h>
#include "Timer.h"
#include "DwinDGUS.h"
u8 DwinDGUS::m_USART1_SHOU[];
u8 DwinDGUS::m_USART1_FA[];
u8 DwinDGUS::m_USART1_shouPos;
u32 DwinDGUS::m_USART1_curShou_ms;
bool DwinDGUS::m_bOK;

/**************************************************************************************
 * 描  述 : 初始化USART1并配置USART1中断优先级
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void DMA_USART1_Init(u32 bound) {
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the PWR/BKP Clock */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE); //使能USART1,GPIOA,C时钟
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //初始化GPIOA.9

	//USART1_RX      GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    //PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);    //初始化GPIOA.10

	USART_DeInit(USART1);          //将外设USART1寄存器重设为缺省值
	USART_InitStructure.USART_BaudRate = bound;     //设置串口1波特率为19200
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

//	USART_ITConfig(USART1, USART_IT_TC, ENABLE);  //使能USART1发送中断
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);            //使能USART1发送DMA请求
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //使能USART1接收中断
	USART_Cmd(USART1, ENABLE);      //使能USART1外设

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //NVIC_Group:NVIC分组 0~4 总共5组,最高2位抢占
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*
	 *  DMA设置：
	 *  设置DMA源：内存地址&串口数据寄存器地址
	 *  方向：内存-->外设
	 *  每次传输位：8bit
	 *  传输大小DMA_BufferSize=SENDBUFF_SIZE
	 *  地址自增模式：外设地址不增，内存地址自增1
	 *  DMA模式：一次传输，非循环
	 *  优先级：高
	 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);            //开启DMA时钟
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel4);    //串口1的DMA传输通道是通道4
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) (&USART1->DR);     //设置DMA源
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32) DwinDGUS::m_USART1_FA; //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;             //外设作为DMA的目的端
	DMA_InitStructure.DMA_BufferSize = 1;           //传输大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //外设地址不增加
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //内存地址自增1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据宽度8bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //内存数据宽度8bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;           //(DMA传送优先级为高)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        //非内存到内存
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);       //使能DMA通道4传输完成中断

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void DMA1_USART1_TX(u8 geshu) {
//	DMA_ClearFlag(DMA1_FLAG_TC4);      //清DMA发送完成标志
//	DMA_Cmd(DMA1_Channel4, DISABLE);   //停止DMA
	DMA1_Channel4->CNDTR = geshu;   //重设传输长度
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DwinDGUS::m_bOK = false;
}

extern "C" void DMA1_Channel4_IRQHandler(void)  //DMA中断
		{
	//清除标志位
	DMA_ClearFlag(DMA1_FLAG_TC4);
	DwinDGUS::m_bOK = true;
	DMA_Cmd(DMA1_Channel4, DISABLE);

}

extern "C" void USART1_IRQHandler(void)                    //串口1中断服务程序
		{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		DwinDGUS::m_USART1_SHOU[DwinDGUS::m_USART1_shouPos] = USART_ReceiveData(
		USART1);  //读取接收到的数据
		DwinDGUS::m_USART1_shouPos++;
		DwinDGUS::m_USART1_curShou_ms = Timer::ms5_jishu;
	}
	//溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET) {
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_ORE);
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
		USART_ClearFlag(USART1, USART_FLAG_RXNE); //一定要清除接收中断
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

