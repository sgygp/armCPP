#include "DS18B20.h"
#include <string.h>
#include <stdlib.h>
constexpr u8 DS18B20::CRCTABLE[];
u8 DS18B20::CRC_18B20(u8 dizhi, u8 x) {
	u8 crc_data = 0;
	for (int i = 0; i < x; i++)  //查表校验
		crc_data = CRCTABLE[crc_data ^ temp_buff[dizhi + i]];
	return (crc_data);
}

/**************************************************************************************
 * 描  述 : 配置DS18B20用到的I/O口 初始化
 **************************************************************************************/
DS18B20::DS18B20(u32 rcc, GPIO_TypeDef *port, u16 pin, u8 m_geshu) {
	this->rcc = rcc;
	this->port = port;
	this->pin = pin;
	this->m_geshu = m_geshu;
	temp_buff = (u8*)malloc(9 * m_geshu);  //temp_buff[geshu][9]
	tempBit = (u8*)malloc(m_geshu);
	tempU8 = (u8*)malloc(m_geshu);
	error = (u8*)malloc(m_geshu);
	m_wendu = (float *)malloc(m_geshu*4);
	for(u8 i=0;i<m_geshu;i++)
	{
		m_wendu[i]=0;
		error[i]=0;
	}
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(rcc, ENABLE);

	for (int i = 0; i < m_geshu; i++) {
		GPIO_InitStructure.GPIO_Pin = pin << i;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(port, &GPIO_InitStructure);
		GPIO_SetBits(port, pin << i);	 //DS18B20数据引脚初始化配置为高电平输出
	}

}

/**************************************************************************************
 * 描  述 : 主机给从机发送复位脉冲
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void DS18B20::DS18B20_Rst(void) {
	DS18B20_Mode_Out_OD();     //主机设置为推挽输出

	DS18B20_DATA_OUT(LOW);     //主机至少产生480us的低电平复位信号
	delay_us(750);
	DS18B20_DATA_OUT(HIGH);   //主机在产生复位信号后，需将总线拉高
	delay_us(15+15);   //从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
}

/**************************************************************************************
 * 描  述 : 检测从机给主机返回的存在脉冲
 * 入  参 : 无
 * 返回值 : 0：成功   1：失败  
 **************************************************************************************/
u8 DS18B20::DS18B20_Presence(void) {

	DS18B20_Mode_IPU();    //主机设置为上拉输入
	delay_us(240);
	return 1;
//	u8 pulse_time = 0;
//
//	/* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号
//	 * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
//	 */
//	while (DS18B20_DATA_IN(pin) && pulse_time < 100) {
//		pulse_time++;
//		delay_us(1);
//	}
//
//	if (pulse_time >= 100)  //经过100us后，存在脉冲都还没有到来
//		return 1;             //读取失败
//	else
//		pulse_time = 0;    //清零计时变量
//
//	while (!DS18B20_DATA_IN(pin) && pulse_time < 240) // 存在脉冲到来，且存在的时间不能超过240us
//	{
//		pulse_time++;
//		delay_us(1);
//	}
//	if (pulse_time >= 240) // 存在脉冲到来，且存在的时间超过了240us
//		return 1;        //读取失败
//	else
//		return 0;
}

/**************************************************************************************
 * 描  述 : 从DS18B20读取一个bit
 * 入  参 : 无
 * 返回值 : u8 
 **************************************************************************************/
void DS18B20::DS18B20_Read_Bit(void) {

	/* 读0和读1的时间至少要大于60us */
	DS18B20_Mode_Out_OD();
	/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
	DS18B20_DATA_OUT(LOW);
	delay_us(2+2);

	/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	DS18B20_Mode_IPU();
	delay_us(10);
	for (int i = 0; i < m_geshu; i++) {
		if (DS18B20_DATA_IN(pin << i) == SET)
			tempBit[i] = 1;
		else
			tempBit[i] = 0;
	}

	/* 这个延时参数请参考时序图 */
	delay_us(40);

	return;
}

/**************************************************************************************
 * 描  述 : 从DS18B20读一个字节，低位先行
 * 入  参 : 无
 * 返回值 : u8  
 **************************************************************************************/
void DS18B20::DS18B20_Read_Byte(void) {
	u8 i, j;
	memset(tempU8, 0, m_geshu);
	for (i = 0; i < 8; i++) {
		DS18B20_Read_Bit();		//从DS18B20读取一个bit
		for (j = 0; j < m_geshu; j++) {
			tempU8[j] = (tempU8[j]) | (tempBit[j] << i);
		}
	}
	return;
}

/**************************************************************************************
 * 描  述 : 写一个字节到DS18B20，低位先行
 * 入  参 : u8
 * 返回值 : 无  
 **************************************************************************************/
void DS18B20::DS18B20_Write_Byte(u8 dat) {
	u8 i, testb;
	DS18B20_Mode_Out_OD();

	for (i = 0; i < 8; i++) {
		testb = dat & 0x01;
		dat = dat >> 1;
		/* 写0和写1的时间至少要大于60us */
		if (testb) {
			DS18B20_DATA_OUT(LOW);
			delay_us(5);   //1us < 这个延时 < 15us

			DS18B20_DATA_OUT(HIGH);
			delay_us(70);    //58us+8us>60us
		} else {
			DS18B20_DATA_OUT(LOW);
			/* 60us < Tx 0 < 120us */
			delay_us(70);

			DS18B20_DATA_OUT(HIGH);
			/* 1us < Trec(恢复时间) < 无穷大*/
			delay_us(5);
		}
	}
}

/**************************************************************************************
 * 描  述 : DS18B20初始化函数
 * 入  参 : 无
 * 返回值 : u8
 **************************************************************************************/
void DS18B20::init(void) {
	for (int i = 0; i < 10; i++) {
		DS18B20_Rst();
		if (DS18B20_Presence() == 0)
			break;
	}
}

/**************************************************************************************
 * 描  述 : 从DS18B20读取温度值
 * 入  参 : 无  
 * 返回值 : float 
 **************************************************************************************/
void DS18B20::get_Temp(void) {
	short s_tem;
	float ff = 0;

	DS18B20_Rst();
	DS18B20_Presence();
	DS18B20_Write_Byte(0XCC); /* 跳过 ROM */
	DS18B20_Write_Byte(0X44); /* 开始转换 */

	DS18B20_Rst();
	DS18B20_Presence();
	DS18B20_Write_Byte(0XCC); /* 跳过 ROM */
	DS18B20_Write_Byte(0XBE); /* 读温度值 */

	for (int i = 0; i < 9; i++) {
		DS18B20_Read_Byte();
		for (int j = 0; j < m_geshu; j++) {
			temp_buff[j * 9 + i] = tempU8[j];
		}
	}
	for (int j = 0; j < m_geshu; j++) {
		if (CRC_18B20(j * 9, 9) == 0) {
			s_tem = temp_buff[j * 9 + 1] << 8;
			s_tem = s_tem | temp_buff[j * 9 + 0];

//	if( s_tem < 0 )		/* 负温度 */
//		f_tem = (~s_tem+1) * 0.0625;
//	else
			ff = s_tem * 0.0625;
			m_wendu[j] = ff;
			error[j]=0;
		}	else
		{
			if(error[j]<GUZHANGJISHU)
				error[j]++;
			else
				m_wendu[j] = 99.999;
		}
	}

	return;
}

/*************************************END OF FILE******************************/

