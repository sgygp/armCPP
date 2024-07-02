/*
 * anjian.h
 *
 *  Created on: 2019Äê11ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef ANJIAN_H_
#define ANJIAN_H_
#include "air32f10x_conf.h"

#define ANJIAN_PORT GPIOA
#define ANJIAN_PIN1 	GPIO_Pin_0
#define ANJIAN_PIN2 	GPIO_Pin_1

#define ANJIAN_PORTOLED GPIOA
#define ANJIAN_PINOLED 	GPIO_Pin_9

#define ENTER			1
#define KEYLEN 20


class Anjian {
public:
	Anjian();
public:
	static	void KeyInit();
	static	bool	 isKey1();
	static	bool	 isKey2();
	static	bool	 isKeyLED();

	static u8 m_pJianW;
	static u8 m_pJianR ;
	static u32 m_bufJian[KEYLEN] ;
	static void init();
	static u32 GetKey();
	static void PutKey(u32 key);
	static void run();
	static void doKey(GPIO_TypeDef *port,uint16_t pin,u8 key,u8 &mode,int &jishu);
};

#endif /* ANJIAN_H_ */
