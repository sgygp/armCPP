/*
 * KeyMove.h
 *
 *  Created on: 2020��2��5��
 *      Author: Administrator
 */

#ifndef KEYMOVE_H_
#define KEYMOVE_H_
#include "stm32f10x_conf.h"

class KeyMove {
public:
	KeyMove();
	void move(u8 m_show,int &m_curItem,u32 jian,bool &m_once);//&�������ݷ�ʽΪ����
};

#endif /* KEYMOVE_H_ */
