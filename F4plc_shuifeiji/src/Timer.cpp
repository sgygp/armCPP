#include "Timer.h"
#include "cortexm/ExceptionHandlers.h"

// ----------------------------------------------------------------------------

#if defined(USE_HAL_DRIVER)
extern "C" void HAL_IncTick(void);
#endif

// ----------------------------------------------------------------------------

u32 Timer::ms2_jishu;
u8 Timer::bMS2;
//u32 Timer::oldKey;
//u8 Timer::stat;

// ----- SysTick_Handler() ----------------------------------------------------

extern "C" void SysTick_Handler(void)
{
#if defined(USE_HAL_DRIVER)
  HAL_IncTick();
#endif
	Timer::tick();
}

// ----------------------------------------------------------------------------
