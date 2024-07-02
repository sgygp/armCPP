#include "diag/Trace.h"
#include <Beep.h>
#include <DS18B20.h>
#include <string.h>
#include <Th.h>
#include <TimerPWM.h>
#include "delay.h"
#include "Timer.h"
#include "BlinkLed.h"
#include "Ainstrument.h"
#include "HistoryData.h"
#include "math.h"
#include "Screen.h"
#include "WatchDog.h"
#include "BJ.h"
#include "COM.h"
#include "Modbus.h"
//#include "LaoYingZiWai.h"
//#include "HX711.h"
#include "SSR.h"
#include "II_C.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/*void ChuliBJ(Ainstrument &ainstr, Beep &beep, u32 ms) {
	if ((((u8) (ainstr.getCanshuZhi(107)) & 0x02) != 0) && !ainstr.m_bXY) { //高限报警
		if (ms % 300 < 150)
			Beep::turnOn();
		else
			Beep::turnOff();
	} else if ((((u8) (ainstr.getCanshuZhi(107)) & 0x01) != 0 && !ainstr.m_bXY)
			&& !ainstr.m_bXY) { //低限报警
		if (ms % 600 < 300)
			Beep::turnOn();
		else
			Beep::turnOff();
	}
}*/

void ChuliBJ(u32 ms,Ainstrument &ainstr, Beep &beep, BJ &baojing) {
	if (((u8) (ainstr.getCanshuZhi(107)) & 0x02) != 0) { //高限报警
		if (!ainstr.m_bXY) {
			if (ms % 300 < 150)
				Beep::turnOn();
			else
				Beep::turnOff();
		}
		baojing.turnOn1();
	} else
		baojing.turnOff1();

	if (((u8) (ainstr.getCanshuZhi(107)) & 0x01) != 0) { //低限报警
		if (!ainstr.m_bXY) {
			if (ms % 600 < 300)
				Beep::turnOn();
			else
				Beep::turnOff();
		}
		baojing.turnOn2();
	} else
		baojing.turnOff2();

}


void AinstrRun(Ainstrument &ainstr, float a, float b, float wendH,float wendS) {
	ainstr.setCanshuZhi(101, a);
	ainstr.setCanshuZhi(102, b);//前置级电压
	ainstr.setCanshuZhi(103, wendH); //光源电压
	ainstr.setCanshuZhi(104, wendS); //探测器温度
	ainstr.online();

}


int main(int argc, char *argv[]) {
	delay_ms(50); //等待电源稳定
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //NVIC_Group:NVIC分组 0~4 总共5组,最高2位抢占
	Beep beep;
	beep.init();
	Beep::turnOn();
	delay_ms(250); //开机响
	Beep::turnOff();
/*

	volatile u32 x=12345678;
	II_C m_iic;
	m_iic.init();
	m_iic.Write24C04((u8 *)&x,0,4);
	m_iic.Read24Cxx((u8 *)&x,0,4);
*/








	BJ m_baojing;
	m_baojing.init();

	Ainstrument ainstr1(0); //参数从0开始保存
//	Ainstrument ainstr2(512); //参数从512开始保存
//	Ainstrument ainstr3(1024); //参数从512开始保存
	ainstr1.init();
//	ainstr2.init();
//	ainstr3.init();
//	ainstr1.MorenCanshu();
//	ainstr2.MorenCanshu();
	HistoryData m_history1(&ainstr1); //曲线初始化为24个正弦波
//	HistoryData m_history2(&ainstr2); //曲线初始化为24个正弦波
//	HistoryData m_history3(&ainstr3);//曲线初始化为0
	Screen m_screen(&ainstr1, &ainstr1,&ainstr1, &m_history1, &m_history1,&m_history1);
	m_screen.start(); //先显示0号画面

	TimerPWM timerPWM;
	timerPWM.init(4, 10000); //1.8KHz

	Timer timer;
	timer.start();
	BlinkLed blinkLed;
	blinkLed.init();
	delay_ms(3000); //开机画面延时3秒

	WatchDog dog;
	dog.init(IWDG_Prescaler_32, 4000); //

	DS18B20 m_ds(RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_6, 1); //定义了4个18B20，PB12-PB15

	m_ds.init();

	m_baojing.turnOn1();

	float baifenbi=0;
	u32 ms=0;


	COM m_com(0);
	m_com.init(9600);
	Modbus m_modbus(&m_com);//串口里面就定义了一个，所以必须是0
	Th m_xiangguan(&m_modbus);


//	LaoYingZiWai m_ziwai(&m_modbus);
//	Ainstrument::setZiwai(&m_ziwai);

//	Hx711 m_hx711;
//	m_hx711.init();
	SSR_gutaijidianqi gutaijidianqi;
	gutaijidianqi.init();





	u32  m_cpuJishu=0;
	while (1) {
		//__WFI();
		m_cpuJishu++;
		if (Timer::m_bMS) {
			Timer::m_bMS = false;
			dog.run();
			ms += 5;
//			m_ziwai.run(ms);
			if (ms % 1000 < ainstr1.getCanshuZhi(105)) //加热
				gutaijidianqi.turnOn();
			else
				gutaijidianqi.turnOff();

			if (ms % 3000 < 20) //闪烁指示灯
				blinkLed.turnOn();
			else
				blinkLed.turnOff();

			ChuliBJ(ms,ainstr1, beep, m_baojing); //因为报警蜂鸣器的响声是以毫秒为单位，必须放在这里

			if (ms % 7500 == 0) //每7.5秒存储曲线
			{
				m_history1.WriteData(
						ainstr1.getHanliang()
						* pow(10, ainstr1.getCanshuZhi(2)));
//				m_history2.WriteData(
//						ainstr2.getHanliang()
//						* pow(10, ainstr2.getCanshuZhi(2)));
//				m_history3.WriteData(
//						ainstr3.getHanliang()
//						* pow(10, ainstr3.getCanshuZhi(2)));
				m_screen.gengxinQuxian();
			}

//			ChuliBJ(ainstr1, beep, ms); //因为报警蜂鸣器的响声是以毫秒为单位，必须放在这里
//			ChuliBJ(ainstr2, beep, ms);
//			ChuliBJ(ainstr3, beep, ms);
			if (ainstr1.getCanshuZhi(107) == 0) {
				if (Timer::m_anxia == 0) //防止蜂鸣器在ON的时候报警解除后继续响
					Beep::turnOff();
			}
			m_xiangguan.run(ms);

			if (ms % 1000 == 0) { //一秒一次

				baifenbi=m_cpuJishu/5100.0;
				if(baifenbi>1000)
					baifenbi=1000;
				baifenbi=1000-baifenbi;
				ainstr1.setCanshuZhi(109, baifenbi);

				m_cpuJishu=0;
				//m_ds.get_Temp();
				m_screen.run();
//				ff = m_hx711.ReadHX711(ainstr1.getCanshuZhi(3))/256;
//				AinstrRun(ainstr1, ff/8 , 1, m_ds.m_wendu[0], 0); //0为恒温温度，1为探测器温度
				AinstrRun(ainstr1,m_xiangguan.m_data[10]/1.0, m_xiangguan.m_data[0]/1.0,
						m_xiangguan.m_data[1]/100.0,m_xiangguan.m_data[3]/100.0);


				timerPWM.outPWM(0,ainstr1.getCanshuZhi(106));


/*
				if(m_xiangguan.m_R<0.01	||(m_xiangguan.m_T>55&&m_xiangguan.m_T<99.9))
					m_baojing.outGuzhang(true);
				else
					m_baojing.outGuzhang(false);
*/
			}


			m_screen.show();
		}
	}

}

#pragma GCC diagnostic pop
