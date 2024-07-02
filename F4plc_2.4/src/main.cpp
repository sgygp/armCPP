#include "diag/Trace.h"
#include <AntLock.h>
#include <PortSet.h>
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
#include "DACF4.h"
#include "HuanjingShuju.h"
#include "BenjiTerminal.h"
#include "delay.h"
#include "Device.h"
//#include "Pulse.h"
#include "AIR720.h"
Device m_devices[__DEVICES__];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
void chuliCanJieshou(u16 id, u32 canX)
{
	static u8 anxiacishu[15][32]={0};
	id = id >> 4;
	if(!id)
		return;
	id--;
	u16 command = 0;

	u16 yuxueNO=Cunchu::m_sysCanshu.m_yuxue;

	for(int i = 0; i < 32; i++)
	{
		command = (id + 1) * 100 + i + 1;
		if(command==yuxueNO)
		{
			if(canX & (1 << i))
				PortSet::m_bYuxue=1;
			else
				PortSet::m_bYuxue=0;
			continue;
		}
		if(canX & (1 << i))//有按键按下
				{
			if(anxiacishu[id][i]<100)
				anxiacishu[id][i]++;
				}
		else
		{
			if(anxiacishu[id][i]>1)//单次忽略
				PortSet::putKeyWords(command);
			anxiacishu[id][i]=0;

		}
	}
}

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
	Delay::delay_ms(10);
	Timer timer;
	timer.start(); //		包含了PortSet::init();
	BlinkLed m_led;
	m_led.init();

	CalendarTimer m_CalendarTimer;
	m_CalendarTimer.init();
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

//	Pulse m_pulse;
//	m_pulse.init();

	COM m_com1(0);
	COM m_com3(1);
	COM m_com5(2);
	m_com1.init(115200);//187500
	m_com3.init(9600);
	m_com5.init(115200);
	Modbus m_modbus1(&m_com1);
	Modbus m_modbus3(&m_com3);
	Modbus m_modbus5(&m_com5);


	BenjiTerminal m_termianlBendi(&m_modbus1, m_devices, 0);
	HuanjingShuju m_huanjing(&m_modbus3);//接A1B1
	BenjiTerminal m_4G(&m_modbus5, m_devices, 1);//接A2B2

	int zhanshu = sizeof(Device);
	for(int i = 0; i < __DEVICES__; i++)
	{
		m_devices[i] = Device(i, (i < 46 || (i >= 84 && i < 92)) ? true : false); //前46个,84-91设备是电机型,其他都是开关型
		//////////////以下统计站数量 0只有主机，1一个从站。。。
		if(Cunchu::m_Canshu[i].inNo1 > zhanshu)
			zhanshu = Cunchu::m_Canshu[i].inNo1;
		if(Cunchu::m_Canshu[i].inNo2 > zhanshu)
			zhanshu = Cunchu::m_Canshu[i].inNo2;
		if(Cunchu::m_Canshu[i].inNo3 > zhanshu)
			zhanshu = Cunchu::m_Canshu[i].inNo3;
		if(Cunchu::m_Canshu[i].outNo1 > zhanshu)
			zhanshu = Cunchu::m_Canshu[i].outNo1;
		if(Cunchu::m_Canshu[i].outNo2 > zhanshu)
			zhanshu = Cunchu::m_Canshu[i].outNo2;
	}
	zhanshu /= 100;
	//	Cunchu::m_Canshu[1].qiyong = 1;
	//	Cunchu::m_Canshu[1].inNo1 = 214;
	//	Cunchu::m_Canshu[1].inNo2 = 213;
	//	Cunchu::m_Canshu[1].inNo3 = 212;
	//	Cunchu::m_Canshu[1].outNo1 = 201;
	Cunchu::m_Canshu[0].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[1].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[2].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[3].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[4].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[5].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[6].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[7].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[8].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[9].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[10].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[11].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[12].m_ctrType |=0x8000;
	Cunchu::m_Canshu[13].m_ctrType |=0x8000;
	Cunchu::m_Canshu[14].m_ctrType |=0x8000;
	Cunchu::m_Canshu[15].m_ctrType |=0x8000;
	Cunchu::m_Canshu[16].m_ctrType |=0x8000;
	Cunchu::m_Canshu[17].m_ctrType |=0x8000;
	Cunchu::m_Canshu[18].m_ctrType |=0x8000;
	Cunchu::m_Canshu[19].m_ctrType |=0x8000;
	Cunchu::m_Canshu[20].m_ctrType |=0x8000;
	Cunchu::m_Canshu[21].m_ctrType |=0x8000;
	Cunchu::m_Canshu[22].m_ctrType |=0x8000;
	Cunchu::m_Canshu[23].m_ctrType |=0x8000;
	Cunchu::m_Canshu[24].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[25].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[26].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[27].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[28].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[29].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[30].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[31].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[32].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[33].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[34].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[35].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[36].m_ctrType |=0x8000;
	Cunchu::m_Canshu[37].m_ctrType |=0x8000;
	Cunchu::m_Canshu[38].m_ctrType |=0x8000;
	Cunchu::m_Canshu[39].m_ctrType |=0x8000;
	Cunchu::m_Canshu[40].m_ctrType |=0x8000;
	Cunchu::m_Canshu[41].m_ctrType |=0x8000;
	Cunchu::m_Canshu[42].m_ctrType |=0x8000;
	Cunchu::m_Canshu[43].m_ctrType |=0x8000;
	Cunchu::m_Canshu[44].m_ctrType |=0x8000;
	Cunchu::m_Canshu[45].m_ctrType |=0x8000;
	Cunchu::m_Canshu[46].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[47].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[48].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[49].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[50].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[51].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[52].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[53].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[54].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[55].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[56].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[57].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[58].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[59].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[60].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[61].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[62].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[63].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[64].m_ctrType |=0x8000;
	Cunchu::m_Canshu[65].m_ctrType |=0x8000;
	Cunchu::m_Canshu[66].m_ctrType |=0x8000;
	Cunchu::m_Canshu[67].m_ctrType |=0x8000;
	Cunchu::m_Canshu[68].m_ctrType |=0x8000;
	Cunchu::m_Canshu[69].m_ctrType |=0x8000;
	Cunchu::m_Canshu[70].m_ctrType |=0x8000;
	Cunchu::m_Canshu[71].m_ctrType |=0x8000;
	Cunchu::m_Canshu[72].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[73].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[74].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[75].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[76].m_ctrType |=0x8000;
	Cunchu::m_Canshu[77].m_ctrType |=0x8000;
	Cunchu::m_Canshu[78].m_ctrType |=0x8000;
	Cunchu::m_Canshu[79].m_ctrType |=0x8000;
	Cunchu::m_Canshu[80].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[81].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[82].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[83].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[84].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[85].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[86].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[87].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[88].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[89].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[90].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[91].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[92].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[93].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[94].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[95].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[96].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[97].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[98].m_ctrType &=0x00FF;
	Cunchu::m_Canshu[99].m_ctrType &=0x00FF;

	trace_printf("Number of slave stations = %d.\n",zhanshu);


    u8 curItem = 0;
	u32 ms10 = 0;
	u32 oldms10=0;
	u32 cpuT = 0;
	u8 oldSec = 0;
	u8 x=0;
	while (1) {
		//__WFI();
		cpuT++;
		if (oldms10!=Timer::ms10_jishu) {
			oldms10=Timer::ms10_jishu;
			ms10 ++;
			m_dog.run();
			if (ms10 % 200 < 50) //2秒
				m_led.turnOn();
			else
				m_led.turnOff();

			m_huanjing.run(ms10);
			m_termianlBendi.run();
			m_4G.run();
//			m_pulse.run(ms10);
			if(ms10 % 5 == 0 && zhanshu > 0)
				m_can.Out(ms10 / 5, zhanshu);
			if(Can_1::m_key)
			{
				chuliCanJieshou(Can_1::m_key>>24,Can_1::m_key&0x00ffffff);
				Can_1::m_key=0;
			}
//				m_can.CAN_send_massage((u8*)&PortSet::m_OUT[1],	4, 0x201); //4个字节，32路输出
		    curItem = ms10 % __DEVICES__;
		    m_devices[curItem].run();

			m_CalendarTimer.GetCalendarTime();
			if (oldSec != m_CalendarTimer.RTC_TimeStructure.RTC_Seconds) {
				oldSec = m_CalendarTimer.RTC_TimeStructure.RTC_Seconds;

				Ant_Lock::run();
				Device::KeyWord = PortSet::getKeyWords();
				Device::m_command = Device::getCommand(); //此命令来自MODBUS协议设置
				Ant_Lock::run();
				cpuT /= 200000;
				if(cpuT > 100)
					cpuT = 100;
				Cunchu::cpuUsed = 100 - cpuT;
				cpuT = 0;


				x=0;
				for(int i=0;i<5;i++)
					x+=HuanjingShuju::m_OK[i];
				if(x)
					Cunchu::benjiZhuangtai |= 8;
				else
					Cunchu::benjiZhuangtai &= ~8;

				if(!Air720::m_OK)
					Cunchu::benjiZhuangtai |= 2;
				else
					Cunchu::benjiZhuangtai &= ~2;

				if(!Air720::m_bShuju)
					Cunchu::benjiZhuangtai |= 4;
				else
					Cunchu::benjiZhuangtai &= ~4;
				if(Cunchu::benjiZhuangtai)
					m_led.turnError();
				else
					m_led.turnNormal();
			}
		}
	}
}

#pragma GCC diagnostic pop
