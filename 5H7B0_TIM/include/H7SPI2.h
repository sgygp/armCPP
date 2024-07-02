/*
 * H7SPI2.h
 *
 *  Created on: 2024Äê4ÔÂ3ÈÕ
 *      Author: Administrator
 */

#ifndef H7SPI2_H_
#define H7SPI2_H_
#include "main.h"

#define SPI2_CS1_Pin GPIO_PIN_12
#define SPI2_CS1_GPIO_Port GPIOB

class H7_SPI2 {
public:
	static SPI_HandleTypeDef hspi2;
	static void init();
	static uint8_t SPI2_ReadWriteByte(uint8_t TxData);


};

#endif /* H7SPI2_H_ */
