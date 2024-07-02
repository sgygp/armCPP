#include "spi.h"
#include "stm32h7xx_it.h"

SPI_HandleTypeDef hspi6;

void MX_SPI6_Init(void)
{
	hspi6.Instance = SPI6;
	hspi6.Init.Mode = SPI_MODE_MASTER;
	hspi6.Init.Direction = SPI_DIRECTION_1LINE;
	hspi6.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi6.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi6.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi6.Init.NSS = SPI_NSS_SOFT;
	hspi6.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi6.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi6.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi6.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi6.Init.CRCPolynomial = 0x0;
	hspi6.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	hspi6.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	hspi6.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	hspi6.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi6.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi6.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	hspi6.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	hspi6.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	hspi6.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
	hspi6.Init.IOSwap = SPI_IO_SWAP_DISABLE;
	if (HAL_SPI_Init(&hspi6) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_SPI_MspDeInit(&hspi6);

	HAL_SPI_MspInit(&hspi6);
	__HAL_SPI_ENABLE(&hspi6);

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	if(spiHandle->Instance==SPI6)
	{
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SPI6;
		PeriphClkInitStruct.Spi6ClockSelection = RCC_SPI6CLKSOURCE_D3PCLK1;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
			Error_Handler();
		}

		/* SPI6 clock enable */
		__HAL_RCC_SPI6_CLK_ENABLE();

		/**SPI6 GPIO Configuration
		PB3     ------> SPI6_SCK
		PB5     ------> SPI6_MOSI
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF8_SPI6;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USER CODE BEGIN SPI6_MspInit 1 */

		/* USER CODE END SPI6_MspInit 1 */
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

	if(spiHandle->Instance==SPI6)
	{
		/* USER CODE BEGIN SPI6_MspDeInit 0 */

		/* USER CODE END SPI6_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_SPI6_CLK_DISABLE();

		/**SPI6 GPIO Configuration
		PB3     ------> SPI6_SCK
		PB5     ------> SPI6_MOSI
		 */
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_5);

		/* USER CODE BEGIN SPI6_MspDeInit 1 */

		/* USER CODE END SPI6_MspDeInit 1 */
	}
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
