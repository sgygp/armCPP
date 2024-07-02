/*
 * PortSet.h
 *
 *  Created on: 2020年10月13日PortIn
 *      Author: Administrator
 */

#ifndef ANTDATA_PORTSET_H_
#define ANTDATA_PORTSET_H_
#include "stm32f4xx.h"
#include <vector>
using namespace std;
#define KEYSLEN 20


class PortIn {
public:
	GPIO_TypeDef *port;
	uint16_t pin;
public:
	PortIn(GPIO_TypeDef *port, uint16_t pin) {
		this->port = port;
		this->pin = pin;
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(port, &GPIO_InitStructure);
	}
};
class PortOut {
public:
	GPIO_TypeDef *port;
	uint16_t pin;
public:
	PortOut(GPIO_TypeDef *port, uint16_t pin) {
		this->port = port;
		this->pin = pin;
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(port, &GPIO_InitStructure);
	}
};


class PortSet {
public:
	static u16 KeyWords[KEYSLEN];//代表那个键按下了一次有效。
	static u8 input_R;
	static u8 input_W;
	static u8 m_bYuxue;
	static u32 m_OUT[16];//最大16个从站，0本站
public:
	static vector<PortIn> portIn;
	static vector<PortOut> portOut;
	static void init();
	static u16 getKeyWords();
	static void putKeyWords(u16 key);
	static void outOnOff(u8 no, bool isOn);
};

#endif /* ANTDATA_PORTSET_H_ */
