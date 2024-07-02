#ifndef TIMER_H_
#define TIMER_H_

#include "cmsis_device.h"

// ----------------------------------------------------------------------------

class Timer {
public:
	static const u32 FREQUENCY_HZ = 200u;
	static u32 m_ms5;
public:
	// Default constructor
	Timer() = default;

	inline void start(void) {
		SysTick_Config(SystemCoreClock / FREQUENCY_HZ);
	}

};

// ----------------------------------------------------------------------------

#endif // TIMER_H_
