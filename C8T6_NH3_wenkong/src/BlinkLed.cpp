#include "BlinkLed.h"

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

void BlinkLed::run(u32 ms, float piancha) {
	int x=piancha*1000;
	if(piancha<=-0.5)
	{
		x=0-x;
		if(x>3000)
			x=3000;
		if(ms%x==0)
			turnG_();

	}else if(piancha>=0.5)
	{
		if(x>3000)
			x=3000;
		if(ms%x==0)
			turnR_();

	}else
	{
		if(x<2)
			x=2;
		if ((ms % x) ==0)//½»ÌæÉÁË¸
			turnALL_();
	}

}
// ----------------------------------------------------------------------------
