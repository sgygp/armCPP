//#include "stm32f10x_conf.h"
// ----------------------------------------------------------------------------
//#include <stdio.h>
//#include <stdlib.h>
#include "diag/Trace.h"

#include <Beep.h>
#include <DS18B20.h>
#include <Exti.h>
#include <string.h>
#include <Timer2PWM.h>
#include <usart1.h>
#include "delay.h"
#include "Timer.h"
#include "BlinkLed.h"
#include "Ainstrument.h"
#include"Adc_Dma.h"
#include "SSR.h"
#include "HistoryData.h"
#include "math.h"
#include "Screen.h"
#include "WatchDog.h"
#include "BJ.h"
#include "Bujindianji.h"
#include "Timer3PWM.h"

//#define _CO2_  1

//#include <time.h>

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void ChuliBJ(u32 ms,Ainstrument &ainstr, Beep &beep, BJ &baojing) {
	if (((u8) (ainstr.getCanshuZhi(107)) & 0x02) != 0) { //���ޱ���
		if (!ainstr.m_bXY) {
			if (ms % 300 < 150)
				Beep::turnOn();
			else
				Beep::turnOff();
		}
		baojing.turnOn1();
	} else
		baojing.turnOff1();

	if (((u8) (ainstr.getCanshuZhi(107)) & 0x01) != 0) { //���ޱ���
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

void AinstrRun(Ainstrument &ainstr, u32 a, u32 b, float wendH, float wenduS) {
	ainstr.setCanshuZhi(101, b);
	ainstr.setCanshuZhi(102, a);
	ainstr.setCanshuZhi(103,wendH ); //��ǰ���ƺ��µ��¶�
	ainstr.setCanshuZhi(104,wenduS ); //̽�����¶�
	ainstr.online();

}
float m_zhuansu = 0;


int main(int argc, char *argv[]) {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Timer timer;
	timer.start();

	Beep beep;
	beep.init();

	BlinkLed blinkLed;
	blinkLed.init();

	SSR_gutaijidianqi gutaijidianqi;
	gutaijidianqi.init();

	Ainstrument ainstr1(0); //��������0
	ainstr1.init();
	//	Ainstrument ainstr2(1000); //��������1K(FM24CL16 �ڲ�2K�ռ�)
	//	ainstr2.init();
	HistoryData m_history1; //���߳�ʼ��Ϊ0
	//	HistoryData m_history1(&ainstr1); //���߳�ʼ��Ϊ24�����Ҳ�
	//	HistoryData m_history2(&ainstr2); //���߳�ʼ��Ϊ24�����Ҳ�
	Screen m_screen(&ainstr1, NULL, &m_history1, NULL);
	m_screen.start(); //����ʾ0�Ż���

	BJ m_baojing;
	m_baojing.init();
	Timer2PWM timer2;
	timer2.init(4, 10000);
	AdcDMA m_adc;
	m_adc.start();

	float Hz=ainstr1.getCanshuZhi(3);

	if(Hz>0.1)
	{
		Timer3PWM m_pinlv;
		m_pinlv.init(Hz);//����2��Ƶ��
	}/*else
	{
		Bujindianji::init();
		for(int i=0;i<51;i++)
		{

			Bujindianji::SetTime(72,750-i*10); //����ֵ250��1000000/250=4000��ÿ�룻4000/400��1Ȧ200����1.8�Ȳ����ǣ�=10HZ=1000ת
			delay_ms(50);
		}
	}*/


	delay_ms(3000); //����������ʱ3��

	WatchDog dog;
	dog.init(IWDG_Prescaler_8, 4000); //400����Ϊ��ʱ

	DS18B20 m_ds(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_5, 2); //������4��18B20��PB12-PB15
	m_ds.init();

	EXTI_Configration(); //��ת��

	u32 ms=0;
	float baifenbi=0;

	u32  m_cpuJishu=0;

	u8 bYongTanceqiWenduHenwen=ainstr1.getCanshuZhi(23);
	while (1) {
//		__WFI();
		m_cpuJishu++;
		if (Timer::m_bMS) {
			Timer::m_bMS = false;
			dog.run();
			ms+=5;



/*
			if (ms % 1000 == 150) { //һ��һ��
				baifenbi=m_cpuJishu/50000;
				if(baifenbi>100)
					baifenbi=100;
				trace_printf("CPU used=%u%%\n",100-baifenbi);
			}
*/





				if (ms % 3000 < 20) //��˸ָʾ��
					blinkLed.turnOn();
				else
					blinkLed.turnOff();

				if (ms % 7500 == 3000) { //ÿ7.5��洢����
					m_history1.WriteData(
							ainstr1.getHanliang()
							* pow(10, ainstr1.getCanshuZhi(2)));
					//				m_history2.WriteData(
					//						ainstr2.getHanliang()
					//								* pow(10, ainstr2.getCanshuZhi(2)));
					m_screen.gengxinQuxian();
/*
					if(m_zhuansu==0&&Hz<0.1)//�������
					{
						for(int i=0;i<51;i++)
						{

							Bujindianji::SetTime(72,750-i*10); //����ֵ250��1000000/250=4000��ÿ�룻4000/400��1Ȧ200����1.8�Ȳ����ǣ�=10HZ=1000ת
							dog.run();
							delay_ms(50);
						}

					}
*/

				}
				ChuliBJ(ms,ainstr1, beep, m_baojing); //��Ϊ�������������������Ժ���Ϊ��λ�������������
				//			ChuliBJ(ainstr2, beep);
				if (ainstr1.getCanshuZhi(107) == 0) {
					if (Timer::m_anxia == 0) //��ֹ��������ON��ʱ�򱨾�����������
						Beep::turnOff();
				}

				if (ms % 1000 < ainstr1.getCanshuZhi(105)) //����
					gutaijidianqi.turnOn();
				else
					gutaijidianqi.turnOff();

				if (ms % 1000 == 150) { //һ��һ��
					//����CPU������
					ainstr1.setCanshuZhi(108, m_zhuansu*60.0);
					m_zhuansu=0;
					baifenbi=m_cpuJishu/5000.0;
					if(baifenbi>1000)
						baifenbi=1000;
					baifenbi=1000-baifenbi;
					ainstr1.setCanshuZhi(109, baifenbi);

					m_cpuJishu=0;
					m_ds.get_Temp();
					m_screen.run();
				}

				if (AdcDMA::m_adOK) { //һ��һ��
					if(bYongTanceqiWenduHenwen)
						AinstrRun(ainstr1, AdcDMA::m_adZhi[0],  AdcDMA::m_adZhi[1], m_ds.m_wendu[0],
								m_ds.m_wendu[1]); //0Ϊ�����¶ȣ�1Ϊ̽�����¶�,CO2
					else
						AinstrRun(ainstr1, AdcDMA::m_adZhi[0],  AdcDMA::m_adZhi[1], m_ds.m_wendu[1],
								m_ds.m_wendu[0]); //0Ϊ�����¶ȣ�1Ϊ̽�����¶� ,���
					timer2.outPWM3(ainstr1.getCanshuZhi(106));
					AdcDMA::m_adOK = false;

				}
				m_screen.show();

				if(AdcDMA::m_adZhi[0]>4090
						||AdcDMA::m_adZhi[0]<1000
						||AdcDMA::m_adZhi[1]>4090
						||AdcDMA::m_adZhi[1]<1000
						||(m_ds.m_wendu[0]>55&&m_ds.m_wendu[0]<99.9)
						||(m_ds.m_wendu[1]>55&&m_ds.m_wendu[1]<99.9))
					m_baojing.outGuzhang(true);
				else
					m_baojing.outGuzhang(false);



		}
	}
}

#pragma GCC diagnostic pop

//tm tm1;
//tm1.tm_hour = 0;
//tm1.tm_min = 0;
//tm1.tm_sec = 0;
//tm1.tm_year = 2020 - 1900;
//tm1.tm_mon = 1; //2��
//tm1.tm_mday = 13;
//time_t tt = mktime(&tm1);
//tt+=3600*24;
//tm *tm2 = gmtime(&tt);
//
//trace_printf("sec=%u-%u-%u %u\n", tm1.tm_year+1900, tm1.tm_mon, tm1.tm_mday,
//		tm1.tm_wday);
//trace_printf("sec=%u-%u-%u %u\n", tm2->tm_year+1900, tm2->tm_mon, tm2->tm_mday,
//		tm2->tm_wday);

//			if (Timer::ms_jishu % 200 == 0) {
//				//AD����Ƶ��Ϊ12M/(239.5+12.5)=47.619K ��������8192����1���ܽӽ�6�Σ�������200����ִ��һ���ۼ�
//				}

//	Quxian0_100 m_quxian;
//	float abc=60;
//	float quxian=0;
//	m_quxian.getQuxianChangshu(abc,50,&quxian);
//	trace_printf("m_quxian=%u\n", u32(quxian * 1000.0));

//	char abc[256];
//	for (int i = 0; i < 256; i++)
//		abc[i] = 0;
//SPI_FLASH_SectorErase(0);
//	SPI_FLASH_PageWriteData(0,abc,256);

//			SPI_FLASH_PageReadData(0, abc, 256);
//			trace_printf("[0]=%u,[4]=%u\n", *(u32 *) abc, *(u32*) (abc + 4));
//			trace_printf("ID=%u,madeID=%u;Second %u\n",
//					SPI_FLASH_readdeviceID(), SPI_FLASH_madeID(), seconds);

