/*
 * H7UART.h
 *
 *  Created on: 2024年4月2日
 *      Author: Administrator
 */

#ifndef UARTH7_H_
#define UARTH7_H_
#include "main.h"
#define BAOJIANGE 20 //包间隔ms


#define UART1_BAUD            2000000
#define UART1_TX_BUF_SIZE    1024
#define UART1_RX_BUF_SIZE    1024


/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;        /* STM32内部串口设备指针 */
	uint8_t *pTxBuf;            /* 发送缓冲区 */
	uint8_t *pRxBuf;            /* 接收缓冲区 */
	uint16_t usTxBufSize;        /* 发送缓冲区大小 */
	uint16_t usRxBufSize;        /* 接收缓冲区大小 */
	__IO uint16_t usTxWrite;    /* 发送缓冲区写指针 */
	__IO uint16_t usTxRead;        /* 发送缓冲区读指针 */
	__IO uint16_t usTxCount;    /* 等待发送的数据个数 */

	__IO uint16_t usRxWrite;    /* 接收缓冲区写指针 */
	__IO uint16_t usRxRead;        /* 接收缓冲区读指针 */
	__IO uint16_t usRxCount;    /* 还未读取的新数据个数 */

	void (*SendBefor)(void);     /* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void);     /* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	uint8_t Sending;            /* 正在发送中 */
}UART_T;

void UartIRQ(UART_T *_pUart);

class H7UART {
public:

	static UART_T g_tUart1;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];        /* 发送缓冲区 */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE*2];        /* 接收缓冲区 */


	void init(void);
	bool run(u32 ms);
	void FasongX(int len);

};

#endif /* UARTH7_H_ */
