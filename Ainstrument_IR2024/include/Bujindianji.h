#include "stm32f10x_conf.h"
#ifndef BUJINDIANJI_H_
#define BUJINDIANJI_H_

#define _DIANJIPORT_ GPIOB
#define A1 GPIO_Pin_6
#define A2 GPIO_Pin_7
#define B1 GPIO_Pin_8
#define B2 GPIO_Pin_9

class Bujindianji {
public:
	static u32 m_curStep;
	static u8 pABCD,pinA, pinB, pinC, pinD;

public:
	Bujindianji()=default;
	static void init() ;
	static void Set0(u16 pin) {
		GPIO_SetBits(_DIANJIPORT_, pin);
	}
	static void Set1(u16 pin) {
		GPIO_ResetBits(_DIANJIPORT_, pin);
	}
	static void Go1();
	static void Run();
	static void SetTime(u16 clk, u16 load);

};

#endif /* BUJINDIANJI_H_ */
