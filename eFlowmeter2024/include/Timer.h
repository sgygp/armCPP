#ifndef TIMER_H_
#define TIMER_H_

#include "cmsis_device.h"
#include "beep.h"
#include "Anjian.h"

// ----------------------------------------------------------------------------

class Timer {
public:
	static constexpr u32 FREQUENCY_HZ = 200u;
	static constexpr u32 MAXSHU = 4000000000u;
	static constexpr u16 XIANGSHI = 20u;
	static volatile u32 ms5_jishu;
	static u8 m_anxia;
	static u32 m_xiang;
	static bool m_bMS5;

public:
	// Default constructor
	Timer() = default;
	inline void start(void) {
		// Use SysTick as reference for the delay loops.
		ms5_jishu = 0;
		m_anxia = 0;
		m_xiang = 0;
		SysTick_Config(SystemCoreClock / FREQUENCY_HZ);
		Anjian::KeyInit();
	}

	inline static void tick(void) {
		// Decrement to zero the counter used by the delay routine.
		m_bMS5 = true;
		ms5_jishu++;
		if (ms5_jishu >= MAXSHU)
			ms5_jishu = 0;


		Anjian::run();


		if (m_anxia == 2)    //蜂鸣器正在响
				{
			if ((((ms5_jishu + MAXSHU) - m_xiang) % MAXSHU) >= XIANGSHI) {
				m_anxia = 0;
				Beep::turnOff();
			}

		} else if (m_anxia == 1)    //蜂鸣器开始响
				{
			m_xiang = ms5_jishu;
			m_anxia = 2;
			Beep::turnOn();
		}

	}

};

// ----------------------------------------------------------------------------

#endif // TIMER_H_
