#include "BlinkLed.h"

// ----------------------------------------------------------------------------
bool BlinkLed::isON1=false;
bool BlinkLed::isON2=false;
bool BlinkLed::isON3=false;
void BlinkLed::init()
{

	GPIO_InitTypeDef GPIO_InitStructure;
	// Configure pin in output push/pull mode
	GPIO_InitStructure.GPIO_Pin =PINS_LEDR|PINS_LEDG;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //
	GPIO_Init(PORTS_LED, &GPIO_InitStructure);

	// Start with led turned off
	turnR_Off();
	turnG_Off();
}

// ----------------------------------------------------------------------------
