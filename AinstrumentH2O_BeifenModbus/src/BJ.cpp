#include "BJ.h"

// ----------------------------------------------------------------------------

void
BJ::init()
{
  // Enable GPIO Peripheral clock
  RCC_APB2PeriphClockCmd(RCC_BJ1, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure pin in output push/pull mode
  GPIO_InitStructure.GPIO_Pin =PINS_BJ1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PORTS_BJ1, &GPIO_InitStructure);

 RCC_APB2PeriphClockCmd(RCC_BJ2, ENABLE);

  GPIO_InitStructure.GPIO_Pin =PINS_BJ2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PORTS_BJ2, &GPIO_InitStructure);

 }

// ----------------------------------------------------------------------------
