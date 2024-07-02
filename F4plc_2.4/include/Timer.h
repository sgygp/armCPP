#ifndef TIMER_H_
#define TIMER_H_

#include "cmsis_device.h"
#include "PortSet.h"

// ----------------------------------------------------------------------------

class Timer {
public:
	static constexpr u32 FREQUENCY_HZ = 100u;
	static constexpr u32 MAXSHU = 1000000000u;
	static u32 ms10_jishu;

	static u8 stat; //0时无按键，1时按键有按下
	static u32 oldKey;
public:
	// Default constructor
	Timer() = default;
	inline void start(void) {
		// Use SysTick as reference for the delay loops.
		ms10_jishu = 0;
		SysTick_Config(SystemCoreClock / FREQUENCY_HZ);
//		stat = 0; //0时无按键，1时按键有按下
//		oldKey = 0;
		PortSet::init();
	}

	inline static void tick(void) {
		// Decrement to zero the counter used by the delay routine.
		ms10_jishu++;
		if (ms10_jishu >= MAXSHU)
			ms10_jishu = 0;

		u32 key = 0;
		u8 len = PortSet::portIn.size();

		for (u8 i = 0; i < len; i++) {
			 PortIn in= PortSet::portIn[i];
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
				//PortSet::putKeyWords(i);
				PortSet::putKeyWords(len - i);
				stat = 0xff;
			}
		} else
			stat = 0;
		oldKey = key;

	}


};

// ----------------------------------------------------------------------------

#endif // TIMER_H_
