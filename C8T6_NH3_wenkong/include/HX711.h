/*
 * HX711.h
 *
 *  Created on: 2019Äê11ÔÂ12ÈÕ
 *      Author: Administrator
 */
#include "stm32f10x_conf.h"
#ifndef HX711_H_
#define HX711_H_

#define HX711_RCC 		RCC_APB2Periph_GPIOB
#define HX711_PORT 		GPIOB
#define HX711_DO 			GPIO_Pin_13
#define HX711_CLK 			GPIO_Pin_14

class Hx711 {
public:
	void init(void);
	long ReadHX711(u8 mode);
private:
	void WriteHX711_CLK(bool b);
	u8 ReadHx711_DATA();

};

#endif /* HX711_H_ */
