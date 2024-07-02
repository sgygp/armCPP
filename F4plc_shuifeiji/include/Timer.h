#ifndef TIMER_H_
#define TIMER_H_

#include "cmsis_device.h"

// ----------------------------------------------------------------------------

class Timer {
public:
	static constexpr u32 FREQUENCY_HZ = 500u;
	static constexpr u32 MAXSHU = 1000000000u;
	static u32 ms2_jishu;
	static u8 bMS2;

//	static u8 stat; //0时无按键，1时按键有按下
//	static u32 oldKey;
public:
	// Default constructor
	Timer() = default;
	inline void start(void) {
		// Use SysTick as reference for the delay loops.
		ms2_jishu = 0;
		SysTick_Config(SystemCoreClock / FREQUENCY_HZ);
//		stat = 0; //0时无按键，1时按键有按下
//		oldKey = 0;
	}

	inline static void tick(void) {
		// Decrement to zero the counter used by the delay routine.
		bMS2=true;
		ms2_jishu++;
		if (ms2_jishu >= MAXSHU)
			ms2_jishu = 0;
/*

		u32 key = 0;
		u8 len = AntData_PortSet::portIn.size();

		for (u8 i = 0; i < len; i++) {
			 AntData_PortIn in= AntData_PortSet::portIn[i];
			u8 a = GPIO_ReadInputDataBit(in.port, in.pin);
			key <<= 1;
			key |= ((~a) & 0x01);
		}
		if (key != 0) {
			if(stat==0xff)//按下了未松开
				return;
			if (oldKey != 0 && oldKey != key)
				stat = 0;
			else
				stat++;
			if (stat == 6) {
				u8 i = 0;
				for (; i < len; i++) {
					if (key & 0x01)
						break;
					key >>= 1;
				}
				//AntData_PortSet::putKeyWords(i);
				AntData_PortSet::putKeyWords(len - i);
				stat = 0xff;
			}
		} else
			stat = 0;
		oldKey = key;
*/
	}


};

// ----------------------------------------------------------------------------

#endif // TIMER_H_
