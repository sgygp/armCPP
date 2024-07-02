/*
 * LaoYingZiWai.h
 *
 *  Created on: 2022Äê8ÔÂ8ÈÕ
 *      Author: Administrator
 */

#ifndef LAOYINGZIWAI_H_
#define LAOYINGZIWAI_H_
#define GUZHANGCISHU_ 5
#include "Modbus.h"

typedef struct {
float m_yali=0;
u32 m_flag=0;
float m_nongdu[3]={0};
float m_xishu[3]={0};
}LAOYING;

class LaoYingZiWai {
public:
	LaoYingZiWai(Modbus * mm);
	virtual ~LaoYingZiWai();
	Modbus *m_modbus;
	void run(u32 ms);
	int m_error=0;
	LAOYING m_shuju;

	void JiaoLing();
	void XieXishu(u8 no,float benciXishu);
};

#endif /* LAOYINGZIWAI_H_ */
