/*
 * H7UART.cpp
 *
 *  Created on: 2024��4��2��
 *      Author: Administrator
 */

#include "H7UART.h"
#include "string.h"

UART_T H7UART::g_tUart1;
uint8_t H7UART::g_TxBuf1[UART1_TX_BUF_SIZE];        /* ���ͻ����� */
uint8_t H7UART::g_RxBuf1[UART1_RX_BUF_SIZE*2];        /* ���ջ����� */


void H7UART::init() {
	memset(&g_tUart1,0,sizeof(g_tUart1));

	g_tUart1.uart = USART1;                        /* STM32 �����豸 */
	g_tUart1.pTxBuf = g_TxBuf1;                    /* ���ͻ�����ָ�� */
	g_tUart1.pRxBuf = g_RxBuf1;                    /* ���ջ�����ָ�� */
	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;         /* ���ͻ�������С */
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;         /* ���ջ�������С */
	g_tUart1.usTxWrite = 0;                        /* ����FIFOд���� */
	g_tUart1.usTxRead = 0;                        /* ����FIFO������ */
	g_tUart1.usRxWrite = 0;                        /* ����FIFOд���� */
	g_tUart1.usRxRead = 0;                        /* ����FIFO������ */
	g_tUart1.usRxCount = 0;                        /* ���յ��������ݸ��� */
	g_tUart1.usTxCount = 0;                        /* �����͵����ݸ��� */
	g_tUart1.SendBefor = 0;                        /* ��������ǰ�Ļص����� */
	g_tUart1.SendOver = 0;                        /* ������Ϻ�Ļص����� */
	g_tUart1.Sending = 0;                        /* ���ڷ����б�־ */


	/* ���ò����ʡ���żУ�� */
	UART_HandleTypeDef UartHandle;
	 memset(&UartHandle,0,sizeof(UartHandle));


	/*##-1- ���ô���Ӳ������ ######################################*/
	/* �첽����ģʽ (UART Mode) */
	/* ��������:
	          - �ֳ�    = 8 λ
	          - ֹͣλ  = 1 ��ֹͣλ
	          - У��    = ����Parity
	          - ������  = ����BaudRate
	          - Ӳ�������ƹر� (RTS and CTS signals) */
	UartHandle.Instance        = USART1;
	UartHandle.Init.BaudRate   = UART1_BAUD;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	UartHandle.Init.ClockPrescaler = UART_PRESCALER_DIV1;
//	UART_AdvFeatureInitTypeDef uart_advinit;
//	memset(&uart_advinit,0,sizeof(uart_advinit));
//	uart_advinit.AdvFeatureInit = UART_ADVFEATURE_AUTOBAUDRATE_INIT;
//	uart_advinit.AutoBaudRateEnable = UART_ADVFEATURE_AUTOBAUDRATE_ENABLE; // �����Զ������ʼ��
//	uart_advinit.AutoBaudRateMode = UART_ADVFEATURE_AUTOBAUDRATE_ONSTARTBIT; // �ڽ��յ�������ʱ�Զ���Ⲩ����

	UartHandle.AdvancedInit .AdvFeatureInit= UART_ADVFEATURE_NO_INIT;//uart_advinit

	if (HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&UartHandle, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&UartHandle, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&UartHandle) != HAL_OK)
	{
		Error_Handler();
	}





	SET_BIT(USART1->CR1, USART_CR1_RXNEIE); /* ʹ��PE. RX�����ж� */
	SET_BIT(USART1->CR1, USART_CR1_TCIE); /* ʹ��PE. RX�����ж� */

	SET_BIT(USART1->ICR, USART_ICR_TCCF);   /* ���TC������ɱ�־ */
	SET_BIT(USART1->RQR, USART_RQR_RXFRQ);  /* ���RXNE���ձ�־ */

	/* ����NVIC the NVIC for UART */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

}



/*
 *********************************************************************************************************
 *    �� �� ��: UartIRQ
 *    ����˵��: ���жϷ��������ã�ͨ�ô����жϴ�����
 *    ��    ��: _pUart : �����豸
 *    �� �� ֵ: ��
 *********************************************************************************************************
 */
void UartIRQ(UART_T *_pUart)
{
	uint32_t isrflags   = READ_REG(_pUart->uart->ISR);
	/* ��������ж�  */
	if ((isrflags & USART_ISR_RXNE_RXFNE) != RESET)//�������ݼĴ�������
	{
		/* �Ӵ��ڽ������ݼĴ�����ȡ���ݴ�ŵ�����FIFO */
		uint8_t ch;

		ch = READ_REG(_pUart->uart->RDR);               /* �����ڽ������ݼĴ��� */
		_pUart->pRxBuf[_pUart->usRxWrite] = ch;         /* ���봮�ڽ���FIFO */
		if (++_pUart->usRxWrite >= _pUart->usRxBufSize) /* ����FIFO��дָ��+1 */
		{
			_pUart->usRxWrite = 0;
		}
		if (_pUart->usRxCount < _pUart->usRxBufSize)    /* ͳ��δ������ֽڸ��� */
		{
			_pUart->usRxCount++;
		}

	}

	/* �����ͻ��������ж� */
	if((isrflags & USART_ISR_TC) != RESET)
	{
		if (_pUart->usTxCount == 0)  /* ���ͻ������������ݿ�ȡ */
		{
			_pUart->Sending = 0;
		}
		else  /*���������ݵȴ����� */
		{
			_pUart->Sending = 1;
			_pUart->uart->TDR = _pUart->pTxBuf[_pUart->usTxRead];
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;
		}

	}


	/* ����жϱ�־ */
	SET_BIT(_pUart->uart->ICR, UART_CLEAR_PEF);//��żУ�����
	SET_BIT(_pUart->uart->ICR, UART_CLEAR_FEF);//֡����
	SET_BIT(_pUart->uart->ICR, UART_CLEAR_NEF);//��������
	SET_BIT(_pUart->uart->ICR, UART_CLEAR_OREF);//��ʱ����
	SET_BIT(_pUart->uart->ICR, UART_CLEAR_IDLEF);//����
	SET_BIT(_pUart->uart->ICR, UART_CLEAR_TCF);//�������
	SET_BIT(_pUart->uart->ICR, UART_CLEAR_LBDF);//LIN Break
	SET_BIT(_pUart->uart->ICR, UART_CLEAR_CTSF);//CTS�ж�
	SET_BIT(_pUart->uart->ICR, UART_CLEAR_CMF);//Character match
	SET_BIT(_pUart->uart->ICR, UART_CLEAR_WUF);//����
	SET_BIT(_pUart->uart->ICR, UART_CLEAR_TXFECF);//���ճ�ʱ
}

bool H7UART::run(u32 ms) {
	static int pOldRXW=0;
	static int jishi=0;;
	int len=g_tUart1.usRxCount;
	if((pOldRXW==this->g_tUart1.usRxWrite)&&len!=0)
	{
		jishi++;
		if(jishi>BAOJIANGE)
		{
			HAL_NVIC_DisableIRQ(USART1_IRQn);
			memcpy(&g_tUart1.pRxBuf[g_tUart1.usRxBufSize],g_tUart1.pRxBuf ,len);
			g_tUart1.usRxWrite = 0;                        /* ����FIFOд���� */
			g_tUart1.usRxRead = 0;                        /* ����FIFO������ */
			g_tUart1.usRxCount = 0;                        /* ���յ��������ݸ��� */
			HAL_NVIC_EnableIRQ(USART1_IRQn);
			memcpy(g_tUart1.pTxBuf ,&g_tUart1.pRxBuf[g_tUart1.usRxBufSize],len);
			FasongX(len);
			return true;//�����Ѿ��ƶ�
		}
	}else
		jishi=0;

	pOldRXW=this->g_tUart1.usRxWrite;
	return false;
}


void H7UART::FasongX(int len) {
	while(g_tUart1.Sending);
	g_tUart1.usTxRead=1;
	g_tUart1.uart->TDR = g_tUart1.pTxBuf[0];
	g_tUart1.usTxCount=len-1;
}
extern "C" void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct ;
  memset(&PeriphClkInitStruct,0,sizeof(PeriphClkInitStruct));
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16910CLKSOURCE_D2PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

extern "C" void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
