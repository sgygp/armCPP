/*
 * WatchDog.cpp
 *
 *  Created on: 2020年2月22日
 *      Author: Administrator
 */
//初始化独立看门狗
//prv:分频数:0~7(只有低3位有效!)
//load:自动重装载值,0~0XFFF.
//分频因子=4*2^prv.但最大值只能是256!
//load:重装载寄存器值:低11位有效.
//时间计算(大概):Tout=((4*2^prv)*load)/32 (ms).

#include "WatchDog.h"

void WatchDog::init(u8 prv, u16 load) {
	//1.取消寄存器写保护：0x5555使能
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对IWDG->PR IWDG->RLR的写

	//2. 设置IWDG分频系数：写PR
	IWDG_SetPrescaler(prv);

	//3. 设置IWDG装载值
	IWDG_SetReload(load);

	//4. 喂狗：写0xAAAA到KR，赋初值
	IWDG_ReloadCounter(); //reload

	//5. 使能看门狗：写0xCCCC到KR
	IWDG_Enable();
}
void WatchDog::run() {
//	if((ms%500)<20)
		IWDG_ReloadCounter();
}
