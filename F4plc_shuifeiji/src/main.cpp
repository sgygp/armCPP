#include "diag/Trace.h"
#include <AntLock.h>
#include <PortSet.h>
#include "PortSetDo.h"
#include <stdio.h>
#include <stdlib.h>
#include"string.h"
#include "WatchDog.h"
#include "Cunchu.h"
#include "Timer.h"
#include "BlinkLed.h"
#include "COM.h"
#include "Modbus.h"
#include "Can1.h"
#include "CalendarTimer.h"
#include "AdcDma10.h"
#include "DACF4.h"
#include "HuanjingShuju.h"
#include "BenjiTerminal.h"
#include "delay.h"
#include "Device.h"
#include "Pulse.h"
#include "AIR720.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char *argv[]) {
	trace_puts("Hello ARM World!");
	trace_printf("System clock: %u Hz\n", SystemCoreClock);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	PortSetDo::init();
	Delay::delay_ms(10);
	Timer timer;
	timer.start(); //		包含了AntData_PortSet::init();
	BlinkLed m_led;
	m_led.init();

	CalendarTimer m_CalendarTimer;
	m_CalendarTimer.init();
//	m_CalendarTimer.SetCalendarTime(21,03,20,10,45,0x00);//21-3-3 21:9:0
	/*
	AdcDma10 m_ad;
	m_ad.init();
	DAC_F4 m_dac;
	m_dac.init();
	 */
	Can_1 m_can;
	m_can.init();

	W25X16::init();
	Cunchu::read();

	WatchDog m_dog;
	m_dog.init(IWDG_Prescaler_256, 4000); //64*3000/40

	Device m_device;
	Pulse m_pulse;
	m_pulse.init();

	COM m_com1(0);
	COM m_com3(1);
	COM m_com5(2);
	m_com1.init(115200);//187500
	m_com3.init(9600);
	m_com5.init(115200);
	Modbus m_modbus1(&m_com1);
	Modbus m_modbus3(&m_com3);
	Modbus m_modbus5(&m_com5);


	BenjiTerminal m_termianlBendi(&m_modbus1, &m_device, 0);
	HuanjingShuju m_huanjing(&m_modbus3);
	BenjiTerminal m_4G(&m_modbus5, &m_device, 1);



	uint32_t ms2 = 0;
	u32 cpuT = 0;
	u8 oldSec = 0;

	u8 xx=0;
	while (1) {
		//__WFI();
		cpuT++;
		if (Timer::bMS2) {
			Timer::bMS2 = false;
			ms2 ++;
			m_dog.run();
			if (ms2 % 250 < 100) //2秒
				m_led.turnOn();
			else
				m_led.turnOff();

			m_huanjing.run(ms2);
			m_termianlBendi.run();
			m_4G.run();
			m_pulse.run(ms2);
			if(ms2 % 25 == 0 /*&& zhanshu > 0*/)
				m_can.out(PortSet::m_OUT[1]);

			m_CalendarTimer.GetCalendarTime();
			if (oldSec != m_CalendarTimer.RTC_TimeStructure.RTC_Seconds) {
				oldSec = m_CalendarTimer.RTC_TimeStructure.RTC_Seconds;
				Ant_Lock::run();
				m_device.run();
				cpuT /= 200000;
				if(cpuT > 100)
					cpuT = 100;
				Device::cpuUsed = 100 - cpuT;
//				trace_printf("CPU used %d %%",Device::cpuUsed );
				cpuT=0;


				xx=0;
				//			    if(!MCP2515::m_bOK)//CAN故障
				//			    	xx|=1<<0;
				if(!Air720::m_OK)//4G模块故障
					xx|=1<<1;
				if(!Air720::m_bShuju)//4G模块正常，但是30分钟内未收到数据
					xx|=1<<2;
				if(!HuanjingShuju::m_OK)//4G模块故障
					xx|=1<<3;
				Device::sysShebeiZhuangtai=xx|(Device::baohuZhuangtai<<4);
				if(Device::sysShebeiZhuangtai)
					m_led.turnError();
				else
					m_led.turnNormal();

			}
			//			m_dac.out(0, ms % 4095);
			//			m_dac.out(1, 4095 - ms % 4095);

		}
	}
}

#pragma GCC diagnostic pop
