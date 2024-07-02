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
	if ((((u8) (ainstr.getCanshuZhi(107)) & 0x02) != 0) && !ainstr.m_bXY) { //���ޱ���
		if (ms % 300 < 150)
			Beep::turnOn();
		else
			Beep::turnOff();
	} else if ((((u8) (ainstr.getCanshuZhi(107)) & 0x01) != 0 && !ainstr.m_bXY)
			&& !ainstr.m_bXY) { //���ޱ���
		if (ms % 600 < 300)
			Beep::turnOn();
		else
			Beep::turnOff();
	}
}*/

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


void AinstrRun(Ainstrument &ainstr, float a, float b, float wendH,float wendS) {
	ainstr.setCanshuZhi(101, a);
	ainstr.setCanshuZhi(102, b);//ǰ�ü���ѹ
	ainstr.setCanshuZhi(103, wendH); //��Դ��ѹ
	ainstr.setCanshuZhi(104, wendS); //̽�����¶�
	ainstr.online();

}


int main(int argc, char *argv[]) {
	delay_ms(50); //�ȴ���Դ�ȶ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //NVIC_Group:NVIC���� 0~4 �ܹ�5��,���2λ��ռ
	Beep beep;
	beep.init();
	Beep::turnOn();
	delay_ms(250); //������
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

	Ainstrument ainstr1(0); //������0��ʼ����
//	Ainstrument ainstr2(512); //������512��ʼ����
//	Ainstrument ainstr3(1024); //������512��ʼ����
	ainstr1.init();
//	ainstr2.init();
//	ainstr3.init();
//	ainstr1.MorenCanshu();
//	ainstr2.MorenCanshu();
	HistoryData m_history1(&ainstr1); //���߳�ʼ��Ϊ24�����Ҳ�
//	HistoryData m_history2(&ainstr2); //���߳�ʼ��Ϊ24�����Ҳ�
//	HistoryData m_history3(&ainstr3);//���߳�ʼ��Ϊ0
	Screen m_screen(&ainstr1, &ainstr1,&ainstr1, &m_history1, &m_history1,&m_history1);
	m_screen.start(); //����ʾ0�Ż���

	TimerPWM timerPWM;
	timerPWM.init(4, 10000); //1.8KHz

	Timer timer;
	timer.start();
	BlinkLed blinkLed;
	blinkLed.init();
	delay_ms(3000); //����������ʱ3��

	WatchDog dog;
	dog.init(IWDG_Prescaler_32, 4000); //

	DS18B20 m_ds(RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_6, 1); //������4��18B20��PB12-PB15

	m_ds.init();

	m_baojing.turnOn1();

	float baifenbi=0;
	u32 ms=0;


	COM m_com(0);
	m_com.init(9600);
	Modbus m_modbus(&m_com);//��������Ͷ�����һ�������Ա�����0
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
			if (ms % 1000 < ainstr1.getCanshuZhi(105)) //����
				gutaijidianqi.turnOn();
			else
				gutaijidianqi.turnOff();

			if (ms % 3000 < 20) //��˸ָʾ��
				blinkLed.turnOn();
			else
				blinkLed.turnOff();

			ChuliBJ(ms,ainstr1, beep, m_baojing); //��Ϊ�������������������Ժ���Ϊ��λ�������������

			if (ms % 7500 == 0) //ÿ7.5��洢����
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

//			ChuliBJ(ainstr1, beep, ms); //��Ϊ�������������������Ժ���Ϊ��λ�������������
//			ChuliBJ(ainstr2, beep, ms);
//			ChuliBJ(ainstr3, beep, ms);
			if (ainstr1.getCanshuZhi(107) == 0) {
				if (Timer::m_anxia == 0) //��ֹ��������ON��ʱ�򱨾�����������
					Beep::turnOff();
			}
			m_xiangguan.run(ms);

			if (ms % 1000 == 0) { //һ��һ��

				baifenbi=m_cpuJishu/5100.0;
				if(baifenbi>1000)
					baifenbi=1000;
				baifenbi=1000-baifenbi;
				ainstr1.setCanshuZhi(109, baifenbi);

				m_cpuJishu=0;
				//m_ds.get_Temp();
				m_screen.run();
//				ff = m_hx711.ReadHX711(ainstr1.getCanshuZhi(3))/256;
//				AinstrRun(ainstr1, ff/8 , 1, m_ds.m_wendu[0], 0); //0Ϊ�����¶ȣ�1Ϊ̽�����¶�
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
