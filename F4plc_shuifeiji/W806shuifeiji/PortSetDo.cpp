/*
 * PortSetDo.cpp
 *
 *  Created on: 2020Äê10ÔÂ13ÈÕ
 *      Author: Administrator
 */
#include <PortSetDo.h>
//vector<AntData_PortIn> PortSetDo::portIn;
vector<AntData_PortOut> PortSetDo::portOut;
//u8 PortSetDo::input_R;
//u8 PortSetDo::input_W;

void PortSetDo::init() {
//	PortSetDo::input_R = 0;
//	PortSetDo::input_W = 0;
/*	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);


	PortSetDo::portIn.push_back(AntData_PortIn(GPIOA, GPIO_Pin_0));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOA, GPIO_Pin_1));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOB, GPIO_Pin_12));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOA, GPIO_Pin_15));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOB, GPIO_Pin_3));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOB, GPIO_Pin_13));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOB, GPIO_Pin_11));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOB, GPIO_Pin_10));

	PortSetDo::portIn.push_back(AntData_PortIn(GPIOE, GPIO_Pin_15));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOE, GPIO_Pin_14));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOE, GPIO_Pin_13));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOE, GPIO_Pin_12));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOE, GPIO_Pin_11));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOE, GPIO_Pin_10));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOE, GPIO_Pin_9));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOE, GPIO_Pin_8));

	PortSetDo::portIn.push_back(AntData_PortIn(GPIOE, GPIO_Pin_7));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOG, GPIO_Pin_1));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOG, GPIO_Pin_0));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOF, GPIO_Pin_15));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOF, GPIO_Pin_14));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOF, GPIO_Pin_13));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOF, GPIO_Pin_12));
	PortSetDo::portIn.push_back(AntData_PortIn(GPIOF, GPIO_Pin_11));
*/


	PortSetDo::portOut.push_back(AntData_PortOut(GPIOA, GPIO_Pin_6));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOC, GPIO_Pin_6));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOC, GPIO_Pin_7));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOA, GPIO_Pin_8));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOB, GPIO_Pin_7));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOB, GPIO_Pin_8));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOB, GPIO_Pin_15));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOD, GPIO_Pin_8));

	PortSetDo::portOut.push_back(AntData_PortOut(GPIOD, GPIO_Pin_9));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOD, GPIO_Pin_10));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOG, GPIO_Pin_2));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOG, GPIO_Pin_3));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOG, GPIO_Pin_4));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOG, GPIO_Pin_5));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOG, GPIO_Pin_6));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOG, GPIO_Pin_7));

	PortSetDo::portOut.push_back(AntData_PortOut(GPIOG, GPIO_Pin_8));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOC, GPIO_Pin_8));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOC, GPIO_Pin_9));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOG, GPIO_Pin_11));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOG, GPIO_Pin_10));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOG, GPIO_Pin_9));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOD, GPIO_Pin_7));
	PortSetDo::portOut.push_back(AntData_PortOut(GPIOD, GPIO_Pin_6));
}
/*
u8 PortSetDo::getKeyWords() {
	if (input_R == input_W)
		return 0;
	u8 x = KeyWords[input_R];
	input_R = (input_R + 1) % KEYSLEN;
	return x;
}
void PortSetDo::putKeyWords(u8 key) {
	if((PortSetDo::input_W+1)%KEYSLEN==PortSetDo::input_R)
		return;
	PortSetDo::KeyWords[PortSetDo::input_W] = key;
	PortSetDo::input_W = (PortSetDo::input_W + 1) % KEYSLEN;
}
*/
void PortSetDo::outOnOff(u8 no, bool isOn) {
	if(no>=24)
		return;
	AntData_PortOut out = portOut[no];
	if (isOn)
		GPIO_SetBits(out.port, out.pin);
	else
		GPIO_ResetBits(out.port, out.pin);

}

