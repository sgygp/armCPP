#include "Timer.h"

// ----------------------------------------------------------------------------

#if defined(USE_HAL_DRIVER)
extern "C" void HAL_IncTick(void);
#endif

// ----------------------------------------------------------------------------

u32 Timer::ms5_jishu;
bool Timer::m_bMS5;

// ----- SysTick_Handler() ----------------------------------------------------

extern "C" void SysTick_Handler(void)
{
#if defined(USE_HAL_DRIVER)
  HAL_IncTick();
#endif
	Timer::tick();
}

// ----------------------------------------------------------------------------
