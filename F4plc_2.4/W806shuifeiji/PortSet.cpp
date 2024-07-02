/*
 * PortSet.cpp
 *
 *  Created on: 2020Äê10ÔÂ13ÈÕ
 *      Author: Administrator
 */
#include "PortSet.h"
vector<PortIn> PortSet::portIn;
vector<PortOut> PortSet::portOut;
u16 PortSet::KeyWords[KEYSLEN];
u32 PortSet::m_OUT[16]= {0};
u8 PortSet::input_R;
u8 PortSet::input_W;
u8 PortSet::m_bYuxue;


void PortSet::init() {
	PortSet::input_R = 0;
	PortSet::input_W = 0;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	PortSet::portIn.push_back(PortIn(GPIOA, GPIO_Pin_0));
	PortSet::portIn.push_back(PortIn(GPIOA, GPIO_Pin_1));
	PortSet::portIn.push_back(PortIn(GPIOB, GPIO_Pin_12));
	PortSet::portIn.push_back(PortIn(GPIOA, GPIO_Pin_15));
	PortSet::portIn.push_back(PortIn(GPIOB, GPIO_Pin_3));
	PortSet::portIn.push_back(PortIn(GPIOB, GPIO_Pin_13));
	PortSet::portIn.push_back(PortIn(GPIOB, GPIO_Pin_11));
	PortSet::portIn.push_back(PortIn(GPIOB, GPIO_Pin_10));

	PortSet::portIn.push_back(PortIn(GPIOE, GPIO_Pin_15));
	PortSet::portIn.push_back(PortIn(GPIOE, GPIO_Pin_14));
	PortSet::portIn.push_back(PortIn(GPIOE, GPIO_Pin_13));
	PortSet::portIn.push_back(PortIn(GPIOE, GPIO_Pin_12));
	PortSet::portIn.push_back(PortIn(GPIOE, GPIO_Pin_11));
	PortSet::portIn.push_back(PortIn(GPIOE, GPIO_Pin_10));
	PortSet::portIn.push_back(PortIn(GPIOE, GPIO_Pin_9));
	PortSet::portIn.push_back(PortIn(GPIOE, GPIO_Pin_8));

	PortSet::portIn.push_back(PortIn(GPIOE, GPIO_Pin_7));
	PortSet::portIn.push_back(PortIn(GPIOG, GPIO_Pin_1));
	PortSet::portIn.push_back(PortIn(GPIOG, GPIO_Pin_0));
	PortSet::portIn.push_back(PortIn(GPIOF, GPIO_Pin_15));
	PortSet::portIn.push_back(PortIn(GPIOF, GPIO_Pin_14));
	PortSet::portIn.push_back(PortIn(GPIOF, GPIO_Pin_13));
	PortSet::portIn.push_back(PortIn(GPIOF, GPIO_Pin_12));
	PortSet::portIn.push_back(PortIn(GPIOF, GPIO_Pin_11));


	PortSet::portOut.push_back(PortOut(GPIOA, GPIO_Pin_6));
	PortSet::portOut.push_back(PortOut(GPIOC, GPIO_Pin_6));
	PortSet::portOut.push_back(PortOut(GPIOC, GPIO_Pin_7));
	PortSet::portOut.push_back(PortOut(GPIOA, GPIO_Pin_8));
	PortSet::portOut.push_back(PortOut(GPIOB, GPIO_Pin_7));
	PortSet::portOut.push_back(PortOut(GPIOB, GPIO_Pin_8));
	PortSet::portOut.push_back(PortOut(GPIOB, GPIO_Pin_15));
	PortSet::portOut.push_back(PortOut(GPIOD, GPIO_Pin_8));

	PortSet::portOut.push_back(PortOut(GPIOD, GPIO_Pin_9));
	PortSet::portOut.push_back(PortOut(GPIOD, GPIO_Pin_10));
	PortSet::portOut.push_back(PortOut(GPIOG, GPIO_Pin_2));
	PortSet::portOut.push_back(PortOut(GPIOG, GPIO_Pin_3));
	PortSet::portOut.push_back(PortOut(GPIOG, GPIO_Pin_4));
	PortSet::portOut.push_back(PortOut(GPIOG, GPIO_Pin_5));
	PortSet::portOut.push_back(PortOut(GPIOG, GPIO_Pin_6));
	PortSet::portOut.push_back(PortOut(GPIOG, GPIO_Pin_7));

	PortSet::portOut.push_back(PortOut(GPIOG, GPIO_Pin_8));
	PortSet::portOut.push_back(PortOut(GPIOC, GPIO_Pin_8));
	PortSet::portOut.push_back(PortOut(GPIOC, GPIO_Pin_9));
	PortSet::portOut.push_back(PortOut(GPIOG, GPIO_Pin_11));
	PortSet::portOut.push_back(PortOut(GPIOG, GPIO_Pin_10));
	PortSet::portOut.push_back(PortOut(GPIOG, GPIO_Pin_9));
	PortSet::portOut.push_back(PortOut(GPIOD, GPIO_Pin_7));
	PortSet::portOut.push_back(PortOut(GPIOD, GPIO_Pin_6));
}
u16 PortSet::getKeyWords() {
	if (input_R == input_W)
		return 0;
	u32 x = KeyWords[input_R];
	input_R = (input_R + 1) % KEYSLEN;
	return x;
}
void PortSet::putKeyWords(u16 key) {
	if((PortSet::input_W+1)%KEYSLEN==PortSet::input_R)
		return;
	PortSet::KeyWords[PortSet::input_W] = key;
	PortSet::input_W = (PortSet::input_W + 1) % KEYSLEN;
}
void PortSet::outOnOff(u8 no, bool isOn) {
	if(no>=24)
		return;
	PortOut out = portOut[no];
	if (isOn)
		GPIO_SetBits(out.port, out.pin);
	else
		GPIO_ResetBits(out.port, out.pin);

}

