/*
 * anjian.cpp
 *
 *  Created on: 2019年11月21日
 *      Author: Administrator
 */
#include <Anjian.h>
#include "Timer.h"
Anjian::Anjian() {
	m_pJianW = 0;
	m_pJianR = 0;
	for (int i = 0; i < 10; i++)
		m_bufJian[i] = 0;
	m_ms_shoucianxia = -1;
	m_ms_anxiahou = -1;

}

void Anjian::KeyInit() {
	m_ms_shoucianxia = -1;
	m_ms_anxiahou = -1;

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = KEYPIN0; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = KEYPIN1 | KEYPIN2 | KEYPIN3	| KEYPIN4; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_ResetBits(_KEYGND_);
}
u32 Anjian::GetKey() {
	u32 www;
	if (m_pJianR == m_pJianW)
		return 0;
	else {
		www = m_bufJian[m_pJianR];
		m_pJianR = (m_pJianR + 1) % 10;
		return www;
	}
}
u8 Anjian::PanduanKey() {
	u8 a = 0;
	u32 t1 = 0;
	t1 = (Timer::ms5_jishu + Timer::MAXSHU - m_ms_shoucianxia);
	t1 = t1 % Timer::MAXSHU;
	u32 t2 = 0;
	t2 = (Timer::ms5_jishu + Timer::MAXSHU - m_ms_anxiahou);
	t2 = t2 % Timer::MAXSHU;

	if (GPIO_ReadInputDataBit(_KEY1_) == RESET) {
		if (t2 > ANJIANJIANGE) {
			if (m_ms_shoucianxia == -1)
				m_ms_shoucianxia = Timer::ms5_jishu;
			else {
				if (t1 > ANJIANYANSHIKAISHI) {
					a |= ENTER;
					m_ms_shoucianxia = -1;
					m_ms_anxiahou = Timer::ms5_jishu;
				}
			}
		}
	} else if (GPIO_ReadInputDataBit(_KEY2_) == RESET) {
		if (t2 > ANJIANJIANGE) {
			if (m_ms_shoucianxia == -1)
				m_ms_shoucianxia = Timer::ms5_jishu;
			else {
				if (t1 > ANJIANYANSHIKAISHI) {
					a |= UP;
					m_ms_shoucianxia = -1;
					m_ms_anxiahou = Timer::ms5_jishu;
				}
			}
		}
	} else if (GPIO_ReadInputDataBit(_KEY3_) == RESET) {
		if (t2 > ANJIANJIANGE) {
			if (m_ms_shoucianxia == -1)
				m_ms_shoucianxia = Timer::ms5_jishu;
			else {
				if (t1 > ANJIANYANSHIKAISHI) {
					a |= DOWN;
					m_ms_shoucianxia = -1;
					m_ms_anxiahou = Timer::ms5_jishu;
				}
			}
		}

	} else if (GPIO_ReadInputDataBit(_KEY4_) == RESET) {
		if (t2 > ANJIANJIANGE) {
			if (m_ms_shoucianxia == -1)
				m_ms_shoucianxia = Timer::ms5_jishu;
			else {
				if (t1 > ANJIANYANSHIKAISHI) {
					a |= RIGHT;
					m_ms_shoucianxia = -1;
					m_ms_anxiahou = Timer::ms5_jishu;
				}
			}
		}
	}

	return a;

}

void Anjian::SendKey(u32 key, int curItem) //按键转换为串口命令（触摸指令）
		{
	if (curItem < 0)
		return;
	m_bufJian[m_pJianW] = key;
	m_pJianW = (m_pJianW + 1) % 10;
//	Timer::m_anxia = 1;
}
void Anjian::PutKey(u32 key) //从串口中取的命令（触摸指令）
		{
	if (!key) //如果串口没读到键值
		key = PanduanKey(); //判断是否有按键按下
	if (!key)
		return;
	m_bufJian[m_pJianW] = key;
	m_pJianW = (m_pJianW + 1) % 10;
	Timer::m_anxia = 1;
}

