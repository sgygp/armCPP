
#ifndef TIMER_H_
#define TIMER_H_

#include "cmsis_device.h"
#include "beep.h"

// ----------------------------------------------------------------------------

class Timer {
public:
	static constexpr u32 FREQUENCY_HZ = 1000u;
	static constexpr u32 MAXSHU = 100000000u;
	static constexpr u16 XIANGSHI = 30u;
	static u32 ms_jishu;
	static u8 m_anjian;
	static u32 m_xiang;
	static bool m_bMS;

public:
	// Default constructor
	Timer() = default;
	inline void start(void) {
		// Use SysTick as reference for the delay loops.
		ms_jishu = 0;
		m_anjian = 0;
		m_xiang = 0;
		SysTick_Config(SystemCoreClock / FREQUENCY_HZ);
	}

	inline static void tick(void) {
		// Decrement to zero the counter used by the delay routine.
		m_bMS = true;
		ms_jishu++;
		if (ms_jishu >= MAXSHU)
			ms_jishu = 0;
		if (m_anjian == 2)    //蜂鸣器正在响
				{
			GPIO_ResetBits(PORTS, PINS);
			if ((((ms_jishu + MAXSHU) - m_xiang) % MAXSHU) >= XIANGSHI)
				m_anjian = 3;

		} else if (m_anjian == 1)    //蜂鸣器开始响
				{
			m_xiang = ms_jishu;
			m_anjian = 2;
		} else if (m_anjian == 3) {
			GPIO_SetBits(PORTS, PINS);
			m_anjian = 0;
		}

	}

};

// ----------------------------------------------------------------------------

#endif // TIMER_H_
