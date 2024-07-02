#include "delay.h"
#include "Timer.h"


void Delay::delay_us(u16 us) {
	u16 i;

	do {
		i = 6;
		while (i--)
			asm("nop");
		//__nop();
	} while (--us);
}
/**************************************************************************************
 * ��  �� : ������ʱ
 * ��  �� : dly:��ʱ������ֵԽ����ʱʱ��Խ��
 * ����ֵ : ��
 **************************************************************************************/
void Delay::delay2_ms(u16 ms) {
	u32 ms5 = Timer::ms5_jishu;
	u32 uu;
	do {
		uu = Timer::ms5_jishu;
		uu += Timer::MAXSHU - ms5;
		uu %= Timer::MAXSHU;
	} while (uu < (ms / 5));
}

void Delay::delay_ms(u16 ms) {
	do {
		delay_us(250);
		delay_us(250);
		delay_us(250);
		delay_us(250);
	} while (--ms);
}
