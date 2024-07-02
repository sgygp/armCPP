#include <DS18B20_40M.h>
#include <string.h>
#include <stdlib.h>
constexpr u8 DS18B20::CRCTABLE[];
u8 DS18B20::CRC_18B20(u8 dizhi, u8 x) {
	u8 crc_data = 0;
	for (int i = 0; i < x; i++)  //���У��
		crc_data = CRCTABLE[crc_data ^ temp_buff[dizhi + i]];
	return (crc_data);
}
void xiaoYanshi()
{
	delay_us(2);
}

/**************************************************************************************
 * ��  �� : ����DS18B20�õ���I/O�� ��ʼ��
 **************************************************************************************/
DS18B20::DS18B20(u32 rcc, GPIO_TypeDef *port, u16 pin) {
	this->rcc = rcc;
	this->port = port;
	this->pin = pin;
	m_wendu=0;
	RCC_APB2PeriphClockCmd(rcc, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = pin ;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //�������
	GPIO_Init(port, &GPIO_InitStructure);
	GPIO_SetBits(port, pin);	 //DS18B20�������ų�ʼ������Ϊ�ߵ�ƽ���


}

/*
������������������480us��Ȼ���ͷ����ߣ��ȴ�15��60us�󣬴��ڵĴӻ�����������60��240us����Ӧ������֮��ӻ����ͷ�����
ע�⣺��ʼ��ʱ�ӻ����ɲ������ӻ�Ӧ�����Զ��ͷ����ߣ����������ͷ����ߺ���ʱ70us��������ȡ�������ݼ��ɣ�����ʱ500us����ʱ��Ҫ����500us�ڴӻ����Զ��ͷ�
 * */
void DS18B20::DS18B20_Rst(void) {
	DS18B20_Mode_Out_OD();     //��������Ϊ�������

	DS18B20_DATA_OUT(LOW);     //�������ٲ���480us�ĵ͵�ƽ��λ�ź�
	delay_us(750);
	DS18B20_DATA_OUT(HIGH);   //�����ڲ�����λ�źź��轫��������
	delay_us(15+15);   //�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������

}
/*
  ��������������60��120us��Ȼ���ͷ����ߣ���ʾ����0��
  ��������������1��15us��Ȼ���ͷ����ߣ���ʾ����1 ��  �ӻ�������������30us�󣨵���ֵ����ȡ��ƽ������ʱ��ƬӦ����60us
 */
void DS18B20::DS18B20_Write_Byte(u8 dat) {
	u8 i, testb;
	DS18B20_Mode_Out_OD();

	for (i = 0; i < 8; i++) {
		testb = dat & 0x01;
		dat = dat >> 1;
		if (testb) {
			DS18B20_DATA_OUT(LOW);
			delay_us(2);   //1us < �����ʱ < 15us

			DS18B20_DATA_OUT(HIGH);
			delay_us(70);    //58us+8us>60us
		} else {
			DS18B20_DATA_OUT(LOW);
			/* 60us < Tx 0 < 120us */
			delay_us(70);

			DS18B20_DATA_OUT(HIGH);
			/* 1us < Trec(�ָ�ʱ��) < �����*/
			delay_us(2);
		}

	}
}
/*
 ��������������1��15us��Ȼ���ͷ����ߣ��������ͺ�15us�ڶ�ȡ���ߵ�ƽ����������15usĩβ������ȡΪ�͵�ƽ��Ϊ����0����ȡΪ�ߵ�ƽ��Ϊ����1������ʱ��ƬӦ����60us��
��ʵ���������������ߣ�Ȼ��һ���ͷ����ߣ���ʱ���ӻ��뷢��0������ס���߲��������ͷţ����ӻ�����1������������ͷ����ߣ����߾������ˡ�
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

	/* �����ʱ������ο�ʱ��ͼ */
	delay_us(50);

	return tempBit;
	;
}

/**************************************************************************************
 * ��  �� : ��DS18B20��һ���ֽڣ���λ����
 * ��  �� : ��
 * ����ֵ : u8
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
 * ��  �� : ��DS18B20��ȡ�¶�ֵ
 * ��  �� : ��  
 * ����ֵ : float 
 **************************************************************************************/
void DS18B20::get_Temp(void) {
	short s_tem;
	float ff = 0;

	DS18B20_Rst();
	DS18B20_Mode_IPU();    //��������Ϊ��������
	delay_us(240);
	DS18B20_Write_Byte(0XCC); /* ���� ROM */
	DS18B20_Write_Byte(0X44); /* ��ʼת�� */

	DS18B20_Rst();
	DS18B20_Mode_IPU();    //��������Ϊ��������
	delay_us(240);
	DS18B20_Write_Byte(0XCC); /* ���� ROM */
	DS18B20_Write_Byte(0XBE); /* ���¶�ֵ */

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

