
#include <stdio.h>
#include <stdlib.h>
//#include "diag/Trace.h"
#include "Timer.h"
#include "Blinkled.h"
#include "RCC256.h"
#include "math.h"
#include "delay.h"
#include	"Adc32.h"
#include "string.h"
#include "COM.h"
#include "TongxunChuli.h"
#include "Set.h"
#include "RtcBkp.h"
#include "SPI2C.h"
#include "W25X16.h"
#include "Anjian.h"
#include "Sleep.h"
#include "RTCwakeup.h"
#include "malloc.h"
#include "Screen.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char* argv[])//�����壬8MHz����2.9mA��72MHz����11mA��256MHz����31mA
{
	//	trace_puts("Hello ARM World!");
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR| RCC_APB1Periph_BKP, ENABLE);	//ʹ�ܵ�Դ�ͺ󱸽ӿ�ʱ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC256::init();
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks); //��ȡʱ��Ƶ��
	SystemCoreClock=clocks.SYSCLK_Frequency;

	Set m_set;
	m_set.init();
	m_set.setOn();


	Anjian::KeyInit();
	Timer m_timer;
	m_timer.start();
	BlinkLed m_led;
	m_led.init();

	RTC_wakeup m_wake;
	//�˶β������ӻ��ѹ��ܣ�OK
	/*
	m_wake.setWakeup();

	while (ms<3000) {
		if (Timer::m_bMS5) {
			Timer::m_bMS5=false;
			ms +=5;
			if(ms%500==0)
				m_led.turnALL_();
		}
	}

	Sleep::sleep();
	 */
	SPI2_C::init();
	W25X16 m_25;
	/*���²���FLASH�洢���Ķ�д������OK
	m_25.SPI_FLASH_ChipErase();
	for(int i=0;i<256;i++)
		m_25.spi1_tx[4+i]=i;
	m_25.SPI_FLASH_PageWriteData(0,256);

	memset(m_25.spi1_tx+4,0,256);
	m_25.SPI_FLASH_PageReadData(0,256);
	 */
	Screen m_screen(&m_25);
	uint32_t ms = 0;

	if(Anjian::isKeyLED())
	{
		m_wake.setOFF();

SHOW:		m_screen.init();
		while(Anjian::isKey1()||Anjian::isKeyLED());
		delay_ms(1000);
		ms=0;
		Anjian::KeyInit();
		while(1){
			__WFI();
			if (Timer::m_bMS5) {
				Timer::m_bMS5=false;
				ms +=5;
				m_screen.show();
				if ((ms % 500) ==0)
				{
					BlinkLed::turnALL_();

				}
				if(Anjian::isKey1()||ms>60000)//���¹ػ��������޲���LED��������Ӻ�ػ�
					break;
			}

		}
		Sleep::sleep();
	}

	bool isAuto=!Anjian::isKey1();

	volatile u32 cpuTick=0;
	int keys=0;

	if(Anjian::isKey2())
	{
		m_wake.setOFF();
		COM m_com1(1);
		m_com1.init(2000000);
		TongxunChuli m_tongxun;
		m_tongxun.init(&m_com1,&m_25);
		while (1) {
			__WFI();
			if (Timer::m_bMS5) {
				Timer::m_bMS5=false;
				ms +=5;
				if ((ms % 300) ==0)
					BlinkLed::turnR_();
				if(!m_tongxun.run())
					break;
				if(Anjian::isKey1())//10���ڰ���̧����Ϊ����������������
				{
					keys++;
				}else
				{
					if(keys>10)//��⵽��������������
						break;
					keys=0;
				}

			}
		}

	}else
	{
		u8 	m_mode=0;//0ֹͣ������1�ֶ�������2�ж��Ƿ��Զ�����
		while (ms<10000) {
			__WFI();
			if (Timer::m_bMS5) {
				Timer::m_bMS5=false;
				ms +=5;

				if(ms<3000)
				{
					if(!Anjian::isKey1())//3���ڰ���̧����Ϊ����������������
					{
						m_mode=2;
						break;
					}
					BlinkLed::turnALL_();//������˸�൱��ͬʱ��
				}else
				{
					if ((ms % 100) ==0)//������˸
						BlinkLed::turnALL_();
					if(!Anjian::isKey1())//�ֶ�����
					{
						m_mode=1;
						break;
					}
				}
			}
		}
		if((m_mode==2)&&!isAuto)//&&(RtcBkp::getBkpData(BKP_DR1)!=1))//�Զ�����ģʽ�£��ֶ���һ��Ҳ������������
			Sleep::sleep();


		if(!m_mode)
		{
			RtcBkp::setBkpData(BKP_DR1,0);//�رն�ʱ���Ѳ���
			RtcBkp::setBkpData(BKP_DR2,0);//�رն�ʱ���Ѳ���
			m_wake.setOFF();
			BlinkLed::turnR_On();
			while(Anjian::isKey1())
				;
			Sleep::sleep();
		}else
		{
			m_wake.setWakeup();
			BlinkLed::turnG_On();
			int m_curCishu=0;
			if(RtcBkp::getBkpData(BKP_DR1)==0)
			{
				RtcBkp::setBkpData(BKP_DR1,1);
				RtcBkp::setBkpData(BKP_DR2,0);
				m_25.SPI_FLASH_ChipErase();
			}else
			{
				m_curCishu=RtcBkp::getBkpData(BKP_DR2);
				m_curCishu=m_curCishu+1;
				if(m_curCishu>=(8192-1))
					Sleep::sleep();

				//				m_25.SPI_FLASH_BlockErase(STARTADDR+(m_curCishu%48)*0x20000) ;//�����һ��64K
				//				m_25.SPI_FLASH_BlockErase(STARTADDR+(m_curCishu%48)*0x20000+0x10000) ;
				m_25.SPI_FLASH_BlockErase(STARTADDR) ;//�����һ��64K
				m_25.SPI_FLASH_BlockErase(STARTADDR+0x10000) ;
			}




			Adc32 m_ad;
			m_ad.init(1000,true);//
			while(!Adc32::bOK);
			float  ff=0;
			for(int i=0;i<AD_LEN;i++)
				ff+=Adc32::data_table[i];
			ff/=AD_LEN;
			/*
			//����һ��
			////////////�¶�(��C)={(V25-VSENSE)/Avg_Slope}+25 ʵ��25��1750����
			//����12λ��ADC��3.3V��ADCֵΪxfff,�¶�Ϊ25��ʱ��Ӧ�ĵ�ѹֵΪ1.43V��0x6EE
			#define V25 0X6EE
			// ÿ���϶�4.3mV ��Ӧÿ���϶�0X05
			#define AVG_SLOPE 0x05
			ff=(V25-ff)/AVG_SLOPE+25;

			 */
			//���㷽��2
			//ff =(float) ff/4096*3.3;//ʵ�ʵ�ѹֵ
			ff = (1.43 - ff*3.3/4096)/0.0043 + 25 ;
			RtcBkp::setBkpData(BKP_DR20,ff*10.0);



			int xHz=RtcBkp::getBkpData(BKP_DR10);
			if(xHz<600||xHz>50000)
			{
				xHz=20000;
				RtcBkp::setBkpData(BKP_DR10,xHz);
			}
			Adc32::bOK=false;
			m_ad.init(xHz,false);//
			u32 pR=0;
			ms=0;
			while (1) {
				cpuTick++;
				if(Adc32::bOK)
				{
					Adc32::bOK=false;
					//trace_printf("%u\n", Timer::ms5_jishu/200);
					memcpy(m_25.spi1_tx+4,Adc32::data_table,256);
					//m_25.SPI_FLASH_PageWriteData(STARTADDR+(m_curCishu%48)*0x20000+pR,256);
					m_25.SPI_FLASH_PageWriteData(STARTADDR+pR,256);
					pR+=256;//128������
					if(pR>=0x20000)//ÿ�βɼ�64K������,��128K�ֽڣ�48�ι�6M
					{
						BlinkLed::turnR_On();
						RtcBkp::setBkpData(BKP_DR2,m_curCishu);
						m_25.WriteBenciSudu();
						goto SHOW;
						//break;
					}
				}

				if (Timer::m_bMS5) {
					Timer::m_bMS5=false;
					ms += 5;
					if ((ms % 2000) < 500)
						BlinkLed::turnG_On();
					else
						BlinkLed::turnG_Off();

					if (ms % 1000 == 0) {
						cpuTick=0;//1���ӣ�72M:cpuTick= 8997184 128:cpuTick= 15997256;256:31998661
					}
					//     trace_printf("Second %u\n", seconds);
				}
			}
		}

	}

	Sleep::sleep();




	// Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
