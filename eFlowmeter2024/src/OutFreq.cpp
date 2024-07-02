/*
 * OutFreq.cpp
 *
 *  Created on: 2024年3月21日
 *      Author: Administrator
 */


//////////////////////////////////////////////////////////////////////////////////////////////
//电磁流量计频率输出的范围为100-5000Hz
//使用管脚PA2, TIM2-CH3

#include "OutFreq.h"

OutFreq::OutFreq() {
	// TODO 自动生成的构造函数存根

}

void OutFreq::initMaichong() {//初始为10分之一，0.1Hz
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);	 //使能定时器2时钟

	//引脚功能配置
	GPIO_InitStructure.GPIO_Pin = OUT_PIN; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(OUT_PORT, &GPIO_InitStructure);
	GPIO_SetBits(OUT_PORT,OUT_PIN);

}

void OutFreq::initPinlv() {//初始为10分之一，0.1Hz

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	 //使能定时器2时钟

	//引脚功能配置
	GPIO_InitStructure.GPIO_Pin = OUT_PIN; //TIM2_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  //复用推挽输出！！
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(OUT_PORT, &GPIO_InitStructure);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	//初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = 7200*5-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = 10000*2-1;   //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCStructInit(&TIM_OCInitStructure);
	//初始化TIM2 Channel3 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Prescaler/2;	//设置占空比时间
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);    //根据T指定的参数初始化外设TIM2 OC3

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR3上的预装载寄存器
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
}

void OutFreq::out(float pinlv) {
	if(pinlv<0.1)
		pinlv=0.1;
	int Prescaler=72000000/pinlv/65536;
	Prescaler++;
	int arr=72000000/(Prescaler*pinlv);
	TIM_PrescalerConfig(TIM2,Prescaler,TIM_PSCReloadMode_Immediate);
	TIM_SetAutoreload(TIM2,arr);
	TIM_SetCompare3(TIM2,arr/2);
}

void OutFreq::out(bool bDidianping) {
	if(bDidianping)
		GPIO_SetBits(OUT_PORT,OUT_PIN);
	else
		GPIO_ResetBits(OUT_PORT,OUT_PIN);
}
