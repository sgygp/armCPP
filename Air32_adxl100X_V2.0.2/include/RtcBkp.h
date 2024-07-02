/*
 * RtcBkp.h
 *
 *  Created on: 2020Äê12ÔÂ31ÈÕ
 *      Author: Administrator
 */

#ifndef RTCBKP_H_
#define RTCBKP_H_
#include "air32f10x_conf.h"

class RtcBkp {
public:
	RtcBkp() = default;
	static void init();
//	static u32 getBkpData();
	static u32 getBkpData(u32 bkp_dr);
	static void setBkpData(u32 bkp_dr,u32 uu);
//	static void setBkpData(u32 uu);

};

#endif /* RTCBKP_H_ */
