/*
 * H7ADC.cpp
 *
 *  Created on: 2024年4月1日
 *      Author: Administrator
 */

#include "H7ADC.h"
#include "string.h"
ALIGN_32BYTES (uint16_t   H7ADC::aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE])  __attribute__((section(".data_EXTMEMB0")));
DMA_HandleTypeDef H7ADC::hdma_adc1;
bool H7ADC::bAD_OK=false;
void H7ADC::init(void) {
	memset(&hadc1,0,sizeof(hadc1));
	memset(&hdma_adc1,0,sizeof(hdma_adc1));

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	hdma_adc1.Instance = DMA1_Stream0;
	hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
	hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_adc1.Init.Mode = DMA_CIRCULAR;
	hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
	hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);

	/* DMA interrupt init */
	/* DMA1_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);



	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_16B;
	//hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	//	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.NbrOfConversion = 2;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
	hadc1.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure the ADC multi-mode
	 */
	ADC_MultiModeTypeDef multimode;
	ADC_ChannelConfTypeDef sConfig;
	memset(&multimode,0,sizeof(multimode));
	memset(&sConfig,0,sizeof(sConfig));
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_18;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_810CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfig.Channel = ADC_CHANNEL_19;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}




	BSP_AdcStart();
}

void H7ADC::run(u32 ms) {
	static int cishu=0;
	static u32 he1=0,he2=0;
	if(H7ADC::bAD_OK)
	{
		H7ADC::bAD_OK=false;
		for(u16 i=0;i<ADC_CONVERTED_DATA_BUFFER_SIZE/2;i++)
		{
			he1 += aADCxConvertedData[i*2];
			he2 += aADCxConvertedData[i*2+1];
		}
		cishu++;
	}

	int f1,f2;
	if(ms%1000==900)
	{
		f1 = he1 /cishu/ (ADC_CONVERTED_DATA_BUFFER_SIZE/2);
		f2 = he2 /cishu/ (ADC_CONVERTED_DATA_BUFFER_SIZE/2);
		cishu=0;
		he1=0;he2=0;
		trace_printf("%d,%d\n",f1,f2);
	}

}

void H7ADC::BSP_AdcStart(void) {
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);

	HAL_ADC_Start_DMA(&hadc1,
			(uint32_t *)&aADCxConvertedData,
			ADC_CONVERTED_DATA_BUFFER_SIZE
	);
}
extern "C" void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer: 32 bytes
		 使数据缓存无效，以便在ADC转换数据缓冲区的下半部分获得SRAM的更新内容:32字节*/
	SCB_InvalidateDCache_by_Addr((uint32_t *) &H7ADC::aADCxConvertedData[0], ADC_CONVERTED_DATA_BUFFER_SIZE);
}

/**
 * @brief  Conversion DMA half-transfer callback in non-blocking mode
 * @param  hadc: ADC handle
 * @retval None
 */
extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes */
	SCB_InvalidateDCache_by_Addr((uint32_t *) &H7ADC::aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE/2], ADC_CONVERTED_DATA_BUFFER_SIZE);
	H7ADC::bAD_OK=true;
}


extern "C" void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct ;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));
	memset(&PeriphClkInitStruct,0,sizeof(PeriphClkInitStruct));

	if(adcHandle->Instance==ADC1)
	{
		/* USER CODE BEGIN ADC1_MspInit 0 */

		/* USER CODE END ADC1_MspInit 0 */

		/** Initializes the peripherals clock
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
		PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_CLKP;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
			Error_Handler();
		}

		/* ADC1 clock enable */
		__HAL_RCC_ADC12_CLK_ENABLE();
		/**ADC1 GPIO Configuration
    PA5     ------> ADC1_INP19
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USER CODE BEGIN ADC1_MspInit 1 */

		/* USER CODE END ADC1_MspInit 1 */
	}
}

extern "C" void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

	if(adcHandle->Instance==ADC1)
	{
		/* USER CODE BEGIN ADC1_MspDeInit 0 */

		/* USER CODE END ADC1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_ADC12_CLK_DISABLE();

		/**ADC1 GPIO Configuration
    PA5     ------> ADC1_INP19
		 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);

		/* ADC1 DMA DeInit */
		HAL_DMA_DeInit(adcHandle->DMA_Handle);
		/* USER CODE BEGIN ADC1_MspDeInit 1 */

		/* USER CODE END ADC1_MspDeInit 1 */
	}
}


