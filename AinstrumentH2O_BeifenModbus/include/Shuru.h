/*
 * Shuru.h
 *
 *  Created on: 2020年2月5日
 *      Author: Administrator
 */

#ifndef SHURU_H_
#define SHURU_H_
#include "stm32f10x_conf.h"

class Shuru {
public:
	Shuru();
	void getKey(u8 key,bool &m_once);
public:
	short m_data;//更改的数据
	u8 m_fanhuiHuamian;//要返回的画面+返回变量的地址
	u8 m_dot;
	bool m_bFuShuru;//是否允许负输入
	u8 m_guangbiao;//光标所在位置0-5
	u8 m_shuzi[6];
	void toShuzi();
	void toData();
private:

};

#endif /* SHURU_H_ */
