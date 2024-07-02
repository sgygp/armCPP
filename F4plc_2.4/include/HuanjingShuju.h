/*
 * HuanjingShuju.h
 *
 *  Created on: 2021��3��14��
 *      Author: Administrator
 */

#ifndef HUANJINGSHUJU_H_
#define HUANJINGSHUJU_H_
#include "Modbus.h"

class HuanjingShuju {
public:
	HuanjingShuju(Modbus * mm);
public :
	static u16 m_data1_4[4][10];
	static u16 m_data5[20];
	static u8 m_Error[5];	//��MODBUSģʽʱ�����5���豸����·���ڵ���GUZHANGCISHU�κ���ΪͨѶ����
							//���豸���� =0�޹��ϣ�=GUZHANGCISHUͨѶ���ϣ�=-1(0xff)������

	static u8 m_OK[5];//�����Ƿ�׼����
	Modbus *m_modbus;
	void run(u32 ms);
private:
	u8 m_curItem=0;
};

#endif /* HUANJINGSHUJU_H_ */
