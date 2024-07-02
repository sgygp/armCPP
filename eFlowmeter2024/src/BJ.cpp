#include "BJ.h"

// ----------------------------------------------------------------------------

void BJ::init() {
	// Enable GPIO Peripheral clock

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	PWR_BackupAccessCmd(ENABLE);/* �����޸�RTC�ͺ󱸼Ĵ���*/
	RCC_LSEConfig( RCC_LSE_OFF); /* �ر��ⲿ����ʱ��,����������ͨIO*/
	BKP_TamperPinCmd(DISABLE); /* �ر����ּ�⹦��,����������ͨIO*/

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	PWR_BackupAccessCmd(DISABLE);/* ��ֹ�޸�RTC�ͺ󱸼Ĵ���*/



	// Configure pin in output push/pull mode
	GPIO_InitStructure.GPIO_Pin = PINS_BJ1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORTS_BJ1, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = PINS_BJ2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORTS_BJ2, &GPIO_InitStructure);

}

// ----------------------------------------------------------------------------
