//#include <stdio.h>    //使用printf需引用
#include "stm32f10x_conf.h"
#ifndef USART1__H_
#define USART1__H_

void DMA_USART1_Init(u32 bound);
void DMA1_USART1_TX(u8 geshu);
#endif
