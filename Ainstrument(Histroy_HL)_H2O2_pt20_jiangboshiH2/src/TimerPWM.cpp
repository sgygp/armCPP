/*
 * Pwm_Timer2.cpp
 *
 *  Created on: 2019年4月24日
 *      Author: Administrator
 */
#include <TimerPWM.h>

void TimerPWM::init(u16 psc, u16 arr) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);	//使能定时器1时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8, ENABLE);	//使能定时器1时钟

	//设置该引脚为复用输出功能,输出TIM2 CH1的PWM脉冲波形	GPIOA.0
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  //初始化GPIO

	//初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr - 1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCStructInit(&TIM_OCInitStructure);
	//初始化TIM Channel 1-4 PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
//	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC1
//	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC2
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC3
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC4

//	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM1_CH1上的预装载寄存器
//	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM1_CH2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM1_CH3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM1_CH4上的预装载寄存器

	TIM_Cmd(TIM8, ENABLE);  //使能TIM1
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_CtrlPWMOutputs(TIM8, ENABLE);        //MOE 主输出使能,高级定时器必须开启这个

}
void TimerPWM::outPWM(u8 no,u16 x) {
	if (x > 10000)
		x = 10000;
	switch(no)
	{


	case 0:
		TIM_SetCompare4(TIM8, x);
		break;
	case 1:
		TIM_SetCompare3(TIM8, x);
		break;
/*	case 2:
		TIM_SetCompare2(TIM8, x);
		break;
	case 3:
		TIM_SetCompare1(TIM8, x);
		break;
*/	}


}
