/*
 * Shuju.h
 *
 *  Created on: 2023年5月9日
 *      Author: Administrator
 */

#ifndef SHUJU_H_
#define SHUJU_H_
#include "air32f10x_conf.h"

class Shuju {
public:
	Shuju();
	static void toJiasuduSudu(u8 *buf,long *data,int geshu,int beilv);
	static void tx2buf(u8 *buf,u8 *tx);
	static void tx2buf2(u8 *buf,u8 *tx);
	static void tx2buf3(u8 *buf,short *tx);
	static short Iabs(short a,short b);
	static bool isNumber(u8 ch);

	static int qiuFeng(float * shuju,float *feng,int len);//返回峰值个数
	static int qiuGuoling(float * shuju,u16 *feng,int len);//返回上升个数
	static float jifen(float *shujuSRC,float *shujuTo,float *feng,float k);
	static float jifenX(float *shujuSRC,float *shujuTo,u16 start ,u16 end,float k);
//	static float  jifen(float *shujuSRC,float *shujuTo,u16 nnn,u16 caiyangpinlv);

	static void TianjiaShuju(u8 *tt,u8 x,u16 zhi,u8 weishu);


//	static u16 JishuanMianji128(short *buf,int zhongxindian,int *jifen);
//	static u16 JishuanPinlv128(short *buf);
//	static void Zhuanhuan(int *iii,u8 *buf);
};

#endif /* SHUJU_H_ */
