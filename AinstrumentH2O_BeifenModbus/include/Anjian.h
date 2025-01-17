/*
 * anjian.h
 *
 *  Created on: 2019年11月21日
 *      Author: Administrator
 */

#ifndef ANJIAN_H_
#define ANJIAN_H_
#include "stm32f10x_conf.h"

#define ANJIANYANSHIKAISHI (10/5)
#define ANJIANJIANGE (300/5)
#define ENTER			1
#define  UP				2
#define  DOWN			4
#define  RIGHT			8
#define LEFT				16

#define KEYPIN1 GPIO_Pin_6
#define KEYPIN2 GPIO_Pin_5
#define KEYPIN3 GPIO_Pin_4
#define KEYPIN4 GPIO_Pin_1
#define KEYPIN0 GPIO_Pin_7

#define _KEY1_ GPIOA,KEYPIN1
#define _KEY2_ GPIOA,KEYPIN2
#define _KEY3_ GPIOA,KEYPIN3
#define _KEY4_ GPIOA,KEYPIN4

#define _KEYGND_ GPIOA,KEYPIN0


class Anjian {
public:
	Anjian();
public:
	void KeyInit();
	u32 GetKey();
	void PutKey(u32 key); //从串口中取的命令（触摸指令）
	void SendKey(u32 key, int curItem); //按键转换为串口命令（触摸指令）

private:
	u8 m_pJianW;
	u8 m_pJianR ;
	u32 m_bufJian[10] ;
	int m_ms_shoucianxia ;
	int m_ms_anxiahou;
private:
	u8 PanduanKey();

};

#endif /* ANJIAN_H_ */
