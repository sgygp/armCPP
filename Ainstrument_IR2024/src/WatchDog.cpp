/*
 * WatchDog.cpp
 *
 *  Created on: 2020��2��22��
 *      Author: Administrator
 */

#include "WatchDog.h"

void WatchDog::init(u8 prv, u16 load) {
	// ʹ�� Ԥ��Ƶ�Ĵ���PR����װ�ؼĴ���RLR��д
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable);

	// ����Ԥ��Ƶ��ֵ
	IWDG_SetPrescaler(prv);

	// ������װ�ؼĴ���ֵ
	IWDG_SetReload(load);

	// ����װ�ؼĴ�����ֵ�ŵ���������
	IWDG_ReloadCounter();

	// ʹ�� IWDG
	IWDG_Enable();

}
void WatchDog::run() {
//	if((ms%500)<20)
		IWDG_ReloadCounter();
}
