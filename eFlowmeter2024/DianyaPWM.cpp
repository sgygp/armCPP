/*
 * DianyaPWM.cpp
 *
 *  Created on: 2021年9月12日
 *      Author: Administrator
 */

#include <DianyaPWM.h>

DianyaPWM::DianyaPWM() {
	// TODO 自动生成的构造函数存根

}

DianyaPWM::~DianyaPWM() {
	// TODO 自动生成的析构函数存根
}

void DianyaPWM::init(u16 psc, u16 arr) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	 //使能定时器1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIO外设时钟

	//引脚功能配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出！！
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	//初始化TIM8
	TIM_TimeBaseStructure.TIM_Period = arr - 1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;   //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;    //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCStructInit(&TIM_OCInitStructure);
	//初始化TIM8 Channel2、Channel3 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = arr / 2;	//设置占空比时间
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);    //根据T指定的参数初始化外设TIM8 OC2

	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器

	TIM_ARRPreloadConfig(TIM1, ENABLE);                        // 使能TIM8重载寄存器ARR
	TIM_Cmd(TIM1, ENABLE);                   // 使能定时器8
	TIM_CtrlPWMOutputs(TIM1, ENABLE);        // 注意： 配置定时器8的PWM模式，必须加上这句话！！
}
void DianyaPWM::out(u16 zhi) {
	TIM_SetCompare3(TIM1, zhi);        //*10000
}
