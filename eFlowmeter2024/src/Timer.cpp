#include "Timer.h"
#include "cortexm/ExceptionHandlers.h"
// ----------------------------------------------------------------------------

#if defined(USE_HAL_DRIVER)
extern "C" void HAL_IncTick(void);
#endif

// ----------------------------------------------------------------------------

u32 volatile Timer::ms5_jishu;
u8 Timer::m_anxia;
u32 Timer::m_xiang;
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
