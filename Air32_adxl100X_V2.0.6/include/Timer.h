#ifndef TIMER_H_
#define TIMER_H_
#include "air32f10x_conf.h"
#include "Anjian.h"

// ----------------------------------------------------------------------------

class Timer {
public:
	static constexpr u32 FREQUENCY_HZ = 200u; //5ms
	static constexpr u32 MAXSHU = 10000000u;
	static u32 ms5_jishu;
	static bool m_bMS5;

public:
	// Default constructor
	Timer() = default;
	inline void start(void) {
		// Use SysTick as reference for the delay loops.
		ms5_jishu = 0;
		SysTick_Config(SystemCoreClock / FREQUENCY_HZ);
		}

	inline static void tick(void) {
		// Decrement to zero the counter used by the delay routine.
		m_bMS5 = true;
		ms5_jishu++;
		Anjian::run();
//		if (ms5_jishu >= MAXSHU)
//			ms5_jishu = 0;
	}

};

// ----------------------------------------------------------------------------

#endif // TIMER_H_
