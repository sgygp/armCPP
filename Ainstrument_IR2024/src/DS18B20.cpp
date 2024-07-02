#include "DS18B20.h"
#include <string.h>
#include <stdlib.h>
constexpr u8 DS18B20::CRCTABLE[];
u8 DS18B20::CRC_18B20(u8 dizhi, u8 x) {
	u8 crc_data = 0;
	for (int i = 0; i < x; i++)  //���У��
		crc_data = CRCTABLE[crc_data ^ temp_buff[dizhi + i]];
	return (crc_data);
}

/**************************************************************************************
 * ��  �� : ����DS18B20�õ���I/O�� ��ʼ��
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
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(port, &GPIO_InitStructure);
		GPIO_SetBits(port, pin << i);	 //DS18B20�������ų�ʼ������Ϊ�ߵ�ƽ���
	}

}

/**************************************************************************************
 * ��  �� : �������ӻ����͸�λ����
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void DS18B20::DS18B20_Rst(void) {
	DS18B20_Mode_Out_OD();     //��������Ϊ�������

	DS18B20_DATA_OUT(LOW);     //�������ٲ���480us�ĵ͵�ƽ��λ�ź�
	delay_us(750);
	DS18B20_DATA_OUT(HIGH);   //�����ڲ�����λ�źź��轫��������
	delay_us(15+15);   //�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
}

/**************************************************************************************
 * ��  �� : ���ӻ����������صĴ�������
 * ��  �� : ��
 * ����ֵ : 0���ɹ�   1��ʧ��  
 **************************************************************************************/
u8 DS18B20::DS18B20_Presence(void) {

	DS18B20_Mode_IPU();    //��������Ϊ��������
	delay_us(240);
	return 1;
//	u8 pulse_time = 0;
//
//	/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź�
//	 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
//	 */
//	while (DS18B20_DATA_IN(pin) && pulse_time < 100) {
//		pulse_time++;
//		delay_us(1);
//	}
//
//	if (pulse_time >= 100)  //����100us�󣬴������嶼��û�е���
//		return 1;             //��ȡʧ��
//	else
//		pulse_time = 0;    //�����ʱ����
//
//	while (!DS18B20_DATA_IN(pin) && pulse_time < 240) // �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us
//	{
//		pulse_time++;
//		delay_us(1);
//	}
//	if (pulse_time >= 240) // �������嵽�����Ҵ��ڵ�ʱ�䳬����240us
//		return 1;        //��ȡʧ��
//	else
//		return 0;
}

/**************************************************************************************
 * ��  �� : ��DS18B20��ȡһ��bit
 * ��  �� : ��
 * ����ֵ : u8 
 **************************************************************************************/
void DS18B20::DS18B20_Read_Bit(void) {

	/* ��0�Ͷ�1��ʱ������Ҫ����60us */
	DS18B20_Mode_Out_OD();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	DS18B20_DATA_OUT(LOW);
	delay_us(2+2);

	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	DS18B20_Mode_IPU();
	delay_us(10);
	for (int i = 0; i < m_geshu; i++) {
		if (DS18B20_DATA_IN(pin << i) == SET)
			tempBit[i] = 1;
		else
			tempBit[i] = 0;
	}

	/* �����ʱ������ο�ʱ��ͼ */
	delay_us(40);

	return;
}

/**************************************************************************************
 * ��  �� : ��DS18B20��һ���ֽڣ���λ����
 * ��  �� : ��
 * ����ֵ : u8  
 **************************************************************************************/
void DS18B20::DS18B20_Read_Byte(void) {
	u8 i, j;
	memset(tempU8, 0, m_geshu);
	for (i = 0; i < 8; i++) {
		DS18B20_Read_Bit();		//��DS18B20��ȡһ��bit
		for (j = 0; j < m_geshu; j++) {
			tempU8[j] = (tempU8[j]) | (tempBit[j] << i);
		}
	}
	return;
}

/**************************************************************************************
 * ��  �� : дһ���ֽڵ�DS18B20����λ����
 * ��  �� : u8
 * ����ֵ : ��  
 **************************************************************************************/
void DS18B20::DS18B20_Write_Byte(u8 dat) {
	u8 i, testb;
	DS18B20_Mode_Out_OD();

	for (i = 0; i < 8; i++) {
		testb = dat & 0x01;
		dat = dat >> 1;
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb) {
			DS18B20_DATA_OUT(LOW);
			delay_us(5);   //1us < �����ʱ < 15us

			DS18B20_DATA_OUT(HIGH);
			delay_us(70);    //58us+8us>60us
		} else {
			DS18B20_DATA_OUT(LOW);
			/* 60us < Tx 0 < 120us */
			delay_us(70);

			DS18B20_DATA_OUT(HIGH);
			/* 1us < Trec(�ָ�ʱ��) < �����*/
			delay_us(5);
		}
	}
}

/**************************************************************************************
 * ��  �� : DS18B20��ʼ������
 * ��  �� : ��
 * ����ֵ : u8
 **************************************************************************************/
void DS18B20::init(void) {
	for (int i = 0; i < 10; i++) {
		DS18B20_Rst();
		if (DS18B20_Presence() == 0)
			break;
	}
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
	DS18B20_Presence();
	DS18B20_Write_Byte(0XCC); /* ���� ROM */
	DS18B20_Write_Byte(0X44); /* ��ʼת�� */

	DS18B20_Rst();
	DS18B20_Presence();
	DS18B20_Write_Byte(0XCC); /* ���� ROM */
	DS18B20_Write_Byte(0XBE); /* ���¶�ֵ */

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

//	if( s_tem < 0 )		/* ���¶� */
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

