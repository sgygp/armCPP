/*
 * WatchDog.cpp
 *
 *  Created on: 2020��2��22��
 *      Author: Administrator
 */
//��ʼ���������Ź�
//prv:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//load:�Զ���װ��ֵ,0~0XFFF.
//��Ƶ����=4*2^prv.�����ֵֻ����256!
//load:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prv)*load)/32 (ms).

#include "WatchDog.h"

void WatchDog::init(u8 prv, u16 load) {
	//1.ȡ���Ĵ���д������0x5555ʹ��
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶ�IWDG->PR IWDG->RLR��д

	//2. ����IWDG��Ƶϵ����дPR
	IWDG_SetPrescaler(prv);

	//3. ����IWDGװ��ֵ
	IWDG_SetReload(load);

	//4. ι����д0xAAAA��KR������ֵ
	IWDG_ReloadCounter(); //reload

	//5. ʹ�ܿ��Ź���д0xCCCC��KR
	IWDG_Enable();
}
void WatchDog::run() {
//	if((ms%500)<20)
		IWDG_ReloadCounter();
}
