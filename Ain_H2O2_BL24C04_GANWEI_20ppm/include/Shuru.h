/*
 * Shuru.h
 *
 *  Created on: 2020��2��5��
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
	short m_data;//���ĵ�����
	u8 m_fanhuiHuamian;//Ҫ���صĻ���+���ر����ĵ�ַ
	u8 m_dot;
	bool m_bFuShuru;//�Ƿ���������
	u8 m_guangbiao;//�������λ��0-5
	u8 m_shuzi[6];
	void toShuzi();
	void toData();
private:

};

#endif /* SHURU_H_ */
