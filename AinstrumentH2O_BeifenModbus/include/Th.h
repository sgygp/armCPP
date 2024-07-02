/*
 * Th.h
 *
 *  Created on: 2021��8��13��
 *      Author: Administrator
 */

#ifndef Th_H_
#define Th_H_

#include "Modbus.h"

#define ERROR_Th 1000
class Th {
public:
	Th(Modbus *m_mod);
	virtual ~Th();
	Modbus *m_modbus;
	void run(u32 ms);
	bool m_bERROR=false;

	u16 m_data[20]={0};//
	/*
	 *		0��	ǰ�ü���ѹ
	 *		1:		��Դ��ѹ
	 *		3:		������¶�
	 * 		10:	ADֵ
	 * */

private:
	int m_errJishi=0;

};

#endif /* Th_H_ */
