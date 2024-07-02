/*
 * HistoryData.h
 *
 *  Created on: 2020年1月31日
 *      Author: Administrator
 */
#ifndef HISTORYDATA_H_
#define HISTORYDATA_H_

#include "stm32f10x.h"
#include "Ainstrument.h"

class HistoryData {
public:
	static constexpr u16 DATALEN=11520;
	u16 m_data[DATALEN]; //存储数据 480*24（小时）    3600*24/7.5（7.5秒存储一次）
private:
	int m_pW;
	int m_pR;
	bool m_bAutoShuxin;
	u16 m_pingmuShijian;
	int m_xiangqianYe;
public:
	HistoryData(Ainstrument *ainstr);
	HistoryData();
	virtual ~HistoryData();
	void WriteData(u16 xx);
	u16 getPingmuShijian();
	void setPingmuShijian(u16 xx);

	int getPW() const {
		return m_pW;
	}

	int getXiangqianYe() const {
		return m_xiangqianYe;
	}

	void setXiangqianYe(int xianqianYe) {
		m_xiangqianYe = xianqianYe;
	}

	int getPR() const {
		return m_pR;
	}

	void setPR(int pR) {
		m_pR = pR;
	}

	bool isBAutoShuxin() const {
		return m_bAutoShuxin;
	}

	void setBAutoShuxin(bool bAutoShuxin) {
		m_bAutoShuxin = bAutoShuxin;
	}
};

#endif /* HISTORYDATA_H_ */
