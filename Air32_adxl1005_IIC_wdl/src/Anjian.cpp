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
	GPIO_InitStructure.GPIO_Pin=ANJIAN_PIN1|ANJIAN_PIN2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ANJIAN_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=ANJIAN_PINOLED;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ANJIAN_PORTOLED,&GPIO_InitStructure);
	Anjian::m_pJianR=0;
	Anjian::m_pJianW=0;
}

bool Anjian::isKey1() {
	return GPIO_ReadInputDataBit(ANJIAN_PORT,ANJIAN_PIN1);
}
bool Anjian::isKey2() {
	bool a=GPIO_ReadInputDataBit(ANJIAN_PORT,ANJIAN_PIN2);
	return a;
}
bool Anjian::isKeyLED() {
	bool a=GPIO_ReadInputDataBit(ANJIAN_PORTOLED,ANJIAN_PINOLED);
	return !a;
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
			if(jishu<200)//2秒内
			{
				if(jishu%100==0)
					a=0x10000|key;
			}else
			{
				if(jishu%100==0)
					a=0x40000|key;
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
	static u8 mode=0;//1,首次按下，2启动了长按，0抬起
	static int jishu=0;//按下计数
	doKey(ANJIAN_PORTOLED,ANJIAN_PINOLED,ENTER,mode,jishu);
//	static u8 mode2=0;//1,首次按下，2启动了长按，0抬起
//	static int jishu2=0;//按下计数
//	doKey(KEY_PORT2,KEY_PIN2,UP,mode2,jishu2);
	//	if (GPIO_ReadInputDataBit(_KEY2_) == SET) {
	//		if (t2 > ANJIANJIANGE) {
	//			if (m_ms_shoucianxia == -1)
	//				m_ms_shoucianxia = Timer::ms5_jishu;
	//			else {
	//				if (t1 > ANJIANYANSHIKAISHI) {
	//					//					a |= UP;
	//					m_ms_shoucianxia = -1;
	//					m_ms_anxiahou = Timer::ms5_jishu;
	//				}
	//			}
	//		}
	//	}

}
void Anjian::PutKey(u32 key) //从串口中取的命令（触摸指令）
{
	if (!key)
		return;
	m_bufJian[m_pJianW] = key;
	m_pJianW = (m_pJianW + 1) % KEYLEN;

}
