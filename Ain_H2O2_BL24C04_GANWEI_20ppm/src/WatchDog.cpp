/*
 * WatchDog.cpp
 *
 *  Created on: 2020年2月22日
 *      Author: Administrator
 */

#include "WatchDog.h"

void WatchDog::init(u8 prv, u16 load) {
	// 使能 预分频寄存器PR和重装载寄存器RLR可写
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable);

	// 设置预分频器值
	IWDG_SetPrescaler(prv);

	// 设置重装载寄存器值
	IWDG_SetReload(load);

	// 把重装载寄存器的值放到计数器中
	IWDG_ReloadCounter();

	// 使能 IWDG
	IWDG_Enable();

}
void WatchDog::run() {
//	if((ms%500)<20)
		IWDG_ReloadCounter();
}
