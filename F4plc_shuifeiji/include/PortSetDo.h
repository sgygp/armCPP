/*
 * PortSetDo.h
 *
 *  Created on: 2020年10月13日AntData_PortIn
 *      Author: Administrator
 */

#ifndef ANTDATA_PORTSET_H_
#define ANTDATA_PORTSET_H_
#include "stm32f4xx.h"
#include <vector>
using namespace std;
#define KEYSLEN 20

/*

class AntData_PortIn {
public:
	GPIO_TypeDef *port;
	uint16_t pin;
public:
	AntData_PortIn(GPIO_TypeDef *port, uint16_t pin) {
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
*/
class AntData_PortOut {
public:
	GPIO_TypeDef *port;
	uint16_t pin;
public:
	AntData_PortOut(GPIO_TypeDef *port, uint16_t pin) {
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


class PortSetDo {
//public:
//	static u8 KeyWords[KEYSLEN];//代表那个键按下了一次有效。
//	static u8 input_R;
//	static u8 input_W;
public:
//	static vector<AntData_PortIn> portIn;
	static vector<AntData_PortOut> portOut;
	static void init();
//	static u8 getKeyWords();
//	static void putKeyWords(u8 key);
	static void outOnOff(u8 no, bool isOn);
};

#endif /* ANTDATA_PORTSET_H_ */
