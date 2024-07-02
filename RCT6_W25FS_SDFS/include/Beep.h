/*
 * Beep.h
 *
 *  Created on: 2020Äê1ÔÂ30ÈÕ
 *      Author: Administrator
 */

#ifndef Beep_
#define Beep_


#include "stm32f10x.h"

// ----------------------------------------------------------------------------
#define PORTS               GPIOA
#define PINS               GPIO_Pin_4
#define RCC_CLK             RCC_APB2Periph_GPIOA

class Beep
{
public:
	Beep() = default;

  void
  init();

  inline void
  __attribute__((always_inline))
  turnOn()
  {
    GPIO_ResetBits(PORTS,PINS );
  }

  inline void
  __attribute__((always_inline))
  turnOff()
  {
    GPIO_SetBits(PORTS,PINS );
  }
};

// ----------------------------------------------------------------------------




#endif /* SSR_H_ */
