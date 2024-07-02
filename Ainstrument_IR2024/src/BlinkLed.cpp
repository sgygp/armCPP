#include "BlinkLed.h"

// ----------------------------------------------------------------------------

void
BlinkLed::init()
{

  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure pin in output push/pull mode
  GPIO_InitStructure.GPIO_Pin =PINS_LED;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PORTS_LED, &GPIO_InitStructure);

  // Start with led turned off
  turnOff();
}

// ----------------------------------------------------------------------------
