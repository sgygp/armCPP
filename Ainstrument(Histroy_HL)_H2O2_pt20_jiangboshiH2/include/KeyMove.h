/*
 * KeyMove.h
 *
 *  Created on: 2020年2月5日
 *      Author: Administrator
 */

#ifndef KEYMOVE_H_
#define KEYMOVE_H_
#include "stm32f10x_conf.h"

class KeyMove {
public:
	KeyMove();
	void move(u8 m_show,int &m_curItem,u32 jian,bool &m_once);//&参数传递方式为引用
};

#endif /* KEYMOVE_H_ */
