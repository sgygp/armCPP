#include "Timer.h"
#include "cortexm/ExceptionHandlers.h"

u32  Timer::m_ms5=0;

// ----- SysTick_Handler() ----------------------------------------------------
extern "C" void SysTick_Handler(void) {
	Timer::m_ms5++;

}

// ----------------------------------------------------------------------------
