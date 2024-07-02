#include <H7UART.h>
#include "main.h"
#include "H7ADC.h"
extern "C" __attribute__((isr)) void SysTick_Handler(void)
{
	HAL_IncTick();
}
extern "C" __attribute__((isr)) void DMA1_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&H7ADC::hdma_adc1);
}

extern "C" __attribute__((isr)) void USART1_IRQHandler(void)
{
	UartIRQ(&H7UART::g_tUart1);
}


extern "C" __attribute__((isr)) void NMI_Handler(void)
{
	while (1)
	{
	}
}

/**
 * @brief This function handles Hard fault interrupt.
 */
extern "C" __attribute__((isr)) void HardFault_Handler(void)
{
	while (1)
	{
	}
}
extern "C" __attribute__((isr)) void MemManage_Handler(void)
{
	while (1)
	{
	}
}

extern "C" __attribute__((isr)) void BusFault_Handler(void)
{
	while (1)
	{
	}
}

extern "C" __attribute__((isr)) void UsageFault_Handler(void)
{
	while (1)
	{
	}
}
extern "C" __attribute__((isr)) void SVC_Handler(void)
{
}

extern "C" __attribute__((isr)) void DebugMon_Handler(void)
{
}

extern "C" __attribute__((isr)) void PendSV_Handler(void)
{
}