#include "diag/Trace.h"
#include "Pulse.h"
#include "HuanjingShuju.h"
#include "Cunchu.h"

void Pulse::init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	for(int i=0;i<14;i++)
	{
		GPIO_InitStruct.GPIO_Pin = PINXX[i];
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;

		GPIO_Init((GPIO_TypeDef *)(GPIOXX[i]), &GPIO_InitStruct);
	}
}
void Pulse::run(u32 ms2)
{
	for(int i=0;i<14;i++)
	{
		u8 a=GPIO_ReadInputDataBit((GPIO_TypeDef *)GPIOXX[i],PINXX[i])==SET;
		if(oldState[i]==1&&a==0)
		{
			m_pulse[i]++;
		}
		oldState[i]=a;
	}
	ms2++;
	if(ms2%500==0)
	{
		for(int i=0;i<8;i++)
		{
//			u8 tongdao=Cunchu::m_sysCanshu.m_maichongLiuliangNO[i];
//			if(tongdao==0||tongdao>14)
//				continue;
//			HuanjingShuju::m_shuju[32+i]=m_pulse[tongdao-1]*10.0*60.0/Cunchu::m_sysCanshu.m_maichongLiuliangjiMeishengGeshu;
//			if(m_pulse[tongdao-1]!=0)
//				trace_printf("%d:%d,%d\n",i,m_pulse[tongdao-1],HuanjingShuju::m_shuju[32+i]);

		}
		for(int i=0;i<14;i++)
			m_pulse[i]=0;
	}
}
