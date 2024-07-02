#include <DS18B20_40M.h>
#include <string.h>
#include <stdlib.h>
constexpr u8 DS18B20::CRCTABLE[];
u8 DS18B20::CRC_18B20(u8 dizhi, u8 x) {
	u8 crc_data = 0;
	for (int i = 0; i < x; i++)  //查表校验
		crc_data = CRCTABLE[crc_data ^ temp_buff[dizhi + i]];
	return (crc_data);
}
void xiaoYanshi()
{
	delay_us(2);
}

/**************************************************************************************
 * 描  述 : 配置DS18B20用到的I/O口 初始化
 **************************************************************************************/
DS18B20::DS18B20(u32 rcc, GPIO_TypeDef *port, u16 pin) {
	this->rcc = rcc;
	this->port = port;
	this->pin = pin;
	m_wendu=0;
	RCC_APB2PeriphClockCmd(rcc, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = pin ;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //推挽输出
	GPIO_Init(port, &GPIO_InitStructure);
	GPIO_SetBits(port, pin);	 //DS18B20数据引脚初始化配置为高电平输出


}

/*
主机将总线拉低至少480us，然后释放总线，等待15—60us后，存在的从机会拉低总线60—240us以响应主机，之后从机将释放总线
注意：初始化时从机不可操作，从机应该是自动释放总线，所以主机释放总线后，延时70us，主机读取总线数据即可，再延时500us满足时间要求，在500us内从机会自动释放
 * */
void DS18B20::DS18B20_Rst(void) {
	DS18B20_Mode_Out_OD();     //主机设置为推挽输出

	DS18B20_DATA_OUT(LOW);     //主机至少产生480us的低电平复位信号
	delay_us(750);
	DS18B20_DATA_OUT(HIGH);   //主机在产生复位信号后，需将总线拉高
	delay_us(15+15);   //从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲

}
/*
  主机将总线拉低60—120us，然后释放总线，表示发送0；
  主机将总线拉低1—15us，然后释放总线，表示发送1 。  从机将在总线拉低30us后（典型值）读取电平，整个时间片应大于60us
 */
void DS18B20::DS18B20_Write_Byte(u8 dat) {
	u8 i, testb;
	DS18B20_Mode_Out_OD();

	for (i = 0; i < 8; i++) {
		testb = dat & 0x01;
		dat = dat >> 1;
		if (testb) {
			DS18B20_DATA_OUT(LOW);
			delay_us(2);   //1us < 这个延时 < 15us

			DS18B20_DATA_OUT(HIGH);
			delay_us(70);    //58us+8us>60us
		} else {
			DS18B20_DATA_OUT(LOW);
			/* 60us < Tx 0 < 120us */
			delay_us(70);

			DS18B20_DATA_OUT(HIGH);
			/* 1us < Trec(恢复时间) < 无穷大*/
			delay_us(2);
		}

	}
}
/*
 主机将总线拉低1—15us，然后释放总线，并在拉低后15us内读取总线电平（尽量贴近15us末尾），读取为低电平则为接收0，读取为高电平则为接收1，整个时间片应大于60us。
其实就是主机拉低总线，然后一会释放总线，此时若从机想发送0，会拉住总线不让主机释放；若从机发送1，则不理会主机释放总线，总线就拉高了。
 */

u8 DS18B20::DS18B20_Read_Bit(void) {
	u8 tempBit;

	DS18B20_Mode_Out_OD();
	DS18B20_DATA_OUT(LOW);
	DS18B20_Mode_IPU();
	if (DS18B20_DATA_IN(pin) == SET)
		tempBit = 1;
	else
		tempBit = 0;

	/* 这个延时参数请参考时序图 */
	delay_us(50);

	return tempBit;
	;
}

/**************************************************************************************
 * 描  述 : 从DS18B20读一个字节，低位先行
 * 入  参 : 无
 * 返回值 : u8
 **************************************************************************************/
u8 DS18B20::DS18B20_Read_Byte(void) {
	u8 i;
	u8 tempU8=0;
	for (i = 0; i < 8; i++) {
		tempU8 = (tempU8) | (DS18B20_Read_Bit() << i);
	}
	return tempU8;
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
	DS18B20_Mode_IPU();    //主机设置为上拉输入
	delay_us(240);
	DS18B20_Write_Byte(0XCC); /* 跳过 ROM */
	DS18B20_Write_Byte(0X44); /* 开始转换 */

	DS18B20_Rst();
	DS18B20_Mode_IPU();    //主机设置为上拉输入
	delay_us(240);
	DS18B20_Write_Byte(0XCC); /* 跳过 ROM */
	DS18B20_Write_Byte(0XBE); /* 读温度值 */

	for (int i = 0; i < 9; i++)
		temp_buff[i] = DS18B20_Read_Byte();

	if (CRC_18B20(0, 9) == 0) {
		s_tem = temp_buff[ 1] << 8;
		s_tem = s_tem | temp_buff[ 0];

		ff = s_tem * 0.0625;
		m_wendu = ff;
	}else
	{
		if(error<GUZHANGJISHU)
			error++;
		else
			m_wendu = 99.999;
	}

	return;
}

/*************************************END OF FILE******************************/

