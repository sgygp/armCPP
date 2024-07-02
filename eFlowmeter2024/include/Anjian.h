/*
 * anjian.h
 *
 *  Created on: 2019Äê11ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef ANJIAN_H_
#define ANJIAN_H_
#include "stm32f10x_conf.h"

#define PPORT GPIOB
//#define PPIN0  GPIO_Pin_5
#define PPIN1  GPIO_Pin_6
#define PPIN2  GPIO_Pin_7
#define PPIN3  GPIO_Pin_8
#define PPIN4  GPIO_Pin_9

#define ENTER			1
#define UP				2
#define DOWN			4
#define RIGHT			8
#define LEFT			16
#define RETURN			0X10001

#define KEYLEN 20

class Anjian {
public:
	Anjian();
public:
	static	void KeyInit();

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
