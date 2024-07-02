#include "BJ.h"

// ----------------------------------------------------------------------------

void
BJ::init()
{
  // Enable GPIO Peripheral clock
    GPIO_InitTypeDef GPIO_InitStructure;

  // Configure pin in output push/pull mode
  GPIO_InitStructure.GPIO_Pin =PINS_BJ1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PORTS_BJ1, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =PINS_BJ2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PORTS_BJ2, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =PINS_GUZHANG;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PORTS_GUZHANG, &GPIO_InitStructure);
  outGuzhang(false);
}

void BJ::outGuzhang(bool bGuzhang) {
	if(!bGuzhang)
	    GPIO_SetBits(PORTS_GUZHANG,PINS_GUZHANG );
	else
	    GPIO_ResetBits(PORTS_GUZHANG,PINS_GUZHANG );

}
// ----------------------------------------------------------------------------
