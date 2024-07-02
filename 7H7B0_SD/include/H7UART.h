/*
 * H7UART.h
 *
 *  Created on: 2024��4��2��
 *      Author: Administrator
 */

#ifndef UARTH7_H_
#define UARTH7_H_
#include "main.h"
#define BAOJIANGE 20 //�����ms


#define UART1_BAUD            2000000
#define UART1_TX_BUF_SIZE    1024
#define UART1_RX_BUF_SIZE    1024


/* �����豸�ṹ�� */
typedef struct
{
	USART_TypeDef *uart;        /* STM32�ڲ������豸ָ�� */
	uint8_t *pTxBuf;            /* ���ͻ����� */
	uint8_t *pRxBuf;            /* ���ջ����� */
	uint16_t usTxBufSize;        /* ���ͻ�������С */
	uint16_t usRxBufSize;        /* ���ջ�������С */
	__IO uint16_t usTxWrite;    /* ���ͻ�����дָ�� */
	__IO uint16_t usTxRead;        /* ���ͻ�������ָ�� */
	__IO uint16_t usTxCount;    /* �ȴ����͵����ݸ��� */

	__IO uint16_t usRxWrite;    /* ���ջ�����дָ�� */
	__IO uint16_t usRxRead;        /* ���ջ�������ָ�� */
	__IO uint16_t usRxCount;    /* ��δ��ȡ�������ݸ��� */

	void (*SendBefor)(void);     /* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void);     /* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	uint8_t Sending;            /* ���ڷ����� */
}UART_T;

void UartIRQ(UART_T *_pUart);

class H7UART {
public:

	static UART_T g_tUart1;
	static uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];        /* ���ͻ����� */
	static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE*2];        /* ���ջ����� */


	void init(void);
	bool run(u32 ms);
	void FasongX(int len);

};

#endif /* UARTH7_H_ */
