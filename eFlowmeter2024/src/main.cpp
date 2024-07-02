//#include <stdio.h>
//#include <stdlib.h>
#include "diag/Trace.h"
#include <CtrPulse.h>
#include <LCD7565.h>
#include "Timer.h"

#include "Beep.h"
#include "dianzhen.h"
#include "CtrPulse.h"
#include "Yiqi.h"
#include "ADC.h"
#include "RealQuxian.h"
#include "Screen.h"
#include "Diaodian.h"
#include "TimerPWM.h"
#include "BJ.h"
#include "BlinkLed.h"
#include "OutFreq.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
void ChuliBJ(Yiqi &ainstr, Beep &beep, BJ &baojing) {
	if ((ainstr.m_baojingOut & 0x02) != 0) { //高限报警
		if (!ainstr.m_bXY) {
			if (Timer::ms5_jishu % 300 < 150)
				Beep::turnOn();
			else
				Beep::turnOff();
		}
		baojing.turnOn1();
	} else
		baojing.turnOff1();

	if ((ainstr.m_baojingOut & 0x01) != 0) { //低限报警
		if (!ainstr.m_bXY) {
			if (Timer::ms5_jishu % 600 < 300)
				Beep::turnOn();
			else
				Beep::turnOff();
		}
		baojing.turnOn2();
	} else
		baojing.turnOff2();

}

int main(int argc, char* argv[]) {
	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //NVIC_Group:NVIC分组 0~4 总共5组,最高2位抢占
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE); //使能GPIO外设时钟

	Beep beep;
	beep.init();
	Timer timer;
	timer.start();
	Beep::turnOn();
	while(timer.ms5_jishu<300/5);
	Beep::turnOff();

	BlinkLed  m_led;
	m_led.init();

	TimerPWM m_pwm;
	m_pwm.init(4, 10000); //1.8KHz

	BJ m_baojing;
	m_baojing.init();

	Yiqi m_yiqi;
	m_yiqi.init(0);
	RealQuxian m_quxian;
	Screen m_screen(&m_yiqi, &m_quxian);
	m_screen.init();
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) // 上电掉电复位
			{
		if (Yiqi::m_syscanshu.m_bLogo)
			m_screen.LOGO();	//显示开机画面，延时3秒
	}

	ADC m_adc;
	m_adc.start();
	Diaodian m_diaodian;
	m_diaodian.init();
	OutFreq m_outFreq;
	if(m_yiqi.m_yiqicanshu.m_maichongshuchufangshi)
		OutFreq::initPinlv();
	else
		OutFreq::initMaichong();

	u32 ms = 0;
	while (1){

		if (Timer::m_bMS5) {
			Timer::m_bMS5 = false;
			ms += 5;
			if(ms%2000<400)
				m_led.turnOn();
			else
				m_led.turnOff();
			if (ms >= 10000000)
				ms = 0;
			m_adc.run(ms);
			m_screen.run(ms);
			ChuliBJ(m_yiqi,beep,m_baojing);
			if(!m_yiqi.m_yiqicanshu.m_maichongshuchufangshi)
				m_outFreq.out(m_yiqi.FasongMaichong());

			if (ms % 1000 == 0) {
				{
//					trace_printf("%u,%u\n", CtrPulse::jiangeTimeH ,CtrPulse::jiangeTimeH );
					m_yiqi.online(m_adc.m_adZhi);
					m_pwm.outPWM1(m_yiqi.m_outMA);
					if (ms % (3600*1000) == 0) {
						m_yiqi.WriteLeiji();
					}
					if(m_yiqi.m_yiqicanshu.m_maichongshuchufangshi)
						m_outFreq.out(m_yiqi.m_outFreq);

				}
			}
		}

	}
}

#pragma GCC diagnostic pop

