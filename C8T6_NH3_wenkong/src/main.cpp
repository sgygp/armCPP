#include <DS18B20_40M.h>
#include "diag/Trace.h"
#include "Timer.h"
#include  "delay.h"
#include "HX711.h"
#include "SSR.h"
#include "HeartPID.h"
#include "Set.h"
#include "BlinkLed.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

u8 m_type = 0;
int main(int argc, char* argv[]) {
	trace_printf("System clock: %u Hz\n", SystemCoreClock);
	delay_ms(50);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	Timer m_timer;
	m_timer.start();

	DS18B20 m_18b20(RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_12);
	m_18b20.get_Temp();	//第一次不大准
	Set m_set;
	m_set.init();
	Hx711 m_711;
	m_711.init();
	SSR_gutaijidianqi gutaijidianqi;
	gutaijidianqi.init();
	BlinkLed m_led;
	m_led.init();
	HeartPID m_pid;

	float m_wenduDS=0;
	float m_wenduPT=0;
	float m_wenduSV=0;
	u32 ms=0;
	u32 old=0;
	while (1) {
		__WFI();
		if(old!=Timer::m_ms5)
		{
			old=Timer::m_ms5;
			ms+=5;
			if (ms % 1000 < m_pid.getHeartOut()) //加热
				gutaijidianqi.turnOn();
			else
				gutaijidianqi.turnOff();
			m_led.run(ms,m_pid.m_piancha1);
			if(ms%1000==0)
			{
				m_wenduSV=m_set.getSet();

				m_18b20.get_Temp();
				m_wenduDS=m_18b20.m_wendu;
				m_wenduPT=m_711.ReadHX711(1)*55.0/5978356.0;
				trace_printf("PT100=: %u,%u\n", int(m_wenduPT*100), int(m_wenduDS*100));

				if(m_wenduPT>70)
					m_pid.pid(m_wenduDS,m_wenduSV);
				else
					m_pid.pid(m_wenduPT,m_wenduSV);

			}

		}

	}

}

