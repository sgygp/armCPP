/*
 * H7ADC.h
 *
 *  Created on: 2024Äê4ÔÂ1ÈÕ
 *      Author: Administrator
 */

#ifndef ADCH7_H_
#define ADCH7_H_
#include "main.h"
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)  64)   /* Size of array aADCxConvertedData[] */

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

class H7ADC {
public:
	 __IO uint16_t AdcValue;
	static ALIGN_32BYTES (uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]) __attribute__((section(".data_EXTMEMB0")));
//	static uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]  __attribute__((section(".data_EXTMEMB0")));
	ADC_HandleTypeDef hadc1;
	static DMA_HandleTypeDef hdma_adc1;
	static bool bAD_OK;

	void run(u32 ms);
	void init(void);
	void BSP_AdcStart(void);
};

#endif /* ADCH7_H_ */
