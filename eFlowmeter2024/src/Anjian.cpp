/*
 * anjian.cpp
 *
 *  Created on: 2019年11月21日
 *      Author: Administrator
 */
#include <Anjian.h>
u8 Anjian::m_pJianW=0;
u8 Anjian::m_pJianR =0;
u32 Anjian::m_bufJian[KEYLEN]={0} ;

Anjian::Anjian() {

}
void Anjian::KeyInit() {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=PPIN1|PPIN2|PPIN3|PPIN4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(PPORT,&GPIO_InitStructure);
	Anjian::m_pJianR=0;
	Anjian::m_pJianW=0;
}
u32 Anjian::GetKey() {
	u32 www;
	if (m_pJianR == m_pJianW)
		return 0;
	else {
		www = m_bufJian[m_pJianR];
		m_pJianR = (m_pJianR + 1) % KEYLEN;
		//trace_printf("%u\n",www);
		return www;
	}
}
void Anjian::doKey(GPIO_TypeDef *port,uint16_t pin,u8 key,u8 &mode,int &jishu) {
	u32 a=0;
	if (GPIO_ReadInputDataBit(port,pin) != SET) {
		jishu++;
		if(mode==0)
		{
			if(jishu>2&&jishu<200)
				mode=1;
		}else	 if(mode==1)
		{
			if(jishu>=200)
			{
				mode=2;
				jishu=0;
			}
		}else if(mode==2)
		{
			if(jishu<500)//2秒内
			{
				if(jishu%100==0)
					a=0x10000|key;
			}else
			{
				if(jishu%50==0)
					a=0x20000|key;
			}
		}
	}else
	{
		if(mode==1)
			a=key;
		mode=0;
		jishu=0;
	}
	if(a)
		PutKey(a);
}

void Anjian::run() {
	static u8 mode[4]={0};//1,首次按下，2启动了长按，0抬起
	static int jishu[4]={0};//按下计数
	doKey(PPORT,PPIN1,ENTER,mode[0],jishu[0]);
	doKey(PPORT,PPIN2,UP,mode[1],jishu[1]);
	doKey(PPORT,PPIN3,DOWN,mode[2],jishu[2]);
	doKey(PPORT,PPIN4,RIGHT,mode[3],jishu[3]);
}
void Anjian::PutKey(u32 key) //从串口中取的命令（触摸指令）
{
	if (!key)
		return;
	m_bufJian[m_pJianW] = key;
	m_pJianW = (m_pJianW + 1) % KEYLEN;

}
