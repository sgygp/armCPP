/*
 * XiangGuan.h
 *
 *  Created on: 2021Äê8ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef XiangGuan_H_
#define XiangGuan_H_

#include  "COM.h"

#define ERROR_XiangGuan 1000
class XiangGuan {
public:
	XiangGuan(COM *_com);
	virtual ~XiangGuan();
	COM *m_com1;
	void run(void);
	float  m_A=0;
	float  m_R=0;
	float m_CC=0;
	float m_T=0;
	bool m_bERROR=false;

	void sendRead();
	float getFloat(u8 x);
	float getZhi(char *p,const char *s);


private:
	u8 m_buf[100];
	int m_errJishi=0;
};

#endif /* XiangGuan_H_ */
