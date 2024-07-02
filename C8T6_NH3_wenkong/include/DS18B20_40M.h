#ifndef __DS18B20_H
#define	__DS18B20_H

#include "stm32f10x_conf.h"
#include "delay.h"

#define GUZHANGJISHU 5

#define HIGH  1
#define LOW   0

class DS18B20 {
public:
	float m_wendu;
	constexpr static u8 CRCTABLE[256] = {
			0, 94, 188, 226, 97, 63, 221, 131, 194,
			156, 126, 32, 163, 253, 31, 65, 157, 195, 33, 127, 252, 162, 64, 30, 95,
			1, 227, 189, 62, 96, 130, 220, 35, 125, 159, 193, 66, 28, 254, 160, 225,
			191, 93, 3, 128, 222, 60, 98, 190, 224, 2, 92, 223, 129, 99, 61, 124,
			34, 192, 158, 29, 67, 161, 255, 70, 24, 250, 164, 39, 121, 155, 197,
			132, 218, 56, 102, 229, 187, 89, 7, 219, 133, 103, 57, 186, 228, 6, 88,
			25, 71, 165, 251, 120, 38, 196, 154, 101, 59, 217, 135, 4, 90, 184, 230,
			167, 249, 27, 69, 198, 152, 122, 36, 248, 166, 68, 26, 153, 199, 37,
			123, 58, 100, 134, 216, 91, 5, 231, 185, 140, 210, 48, 110, 237, 179,
			81, 15, 78, 16, 242, 172, 47, 113, 147, 205, 17, 79, 173, 243, 112, 46,
			204, 146, 211, 141, 111, 49, 178, 236, 14, 80, 175, 241, 19, 77, 206,
			144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238, 50, 108, 142, 208,
			83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115, 202, 148, 118,
			40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139, 87, 9, 235,
			181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22, 233, 183,
			85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168, 116, 42,
			200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53 };
public:
	DS18B20(u32 rcc, GPIO_TypeDef *port, u16 pin);
	void get_Temp(void);
private:
	u32 rcc;
	GPIO_TypeDef *port;
	u16 pin;
	u8 error=0;

	GPIO_InitTypeDef GPIO_InitStructure;
	u8 temp_buff[9]; //�洢��ȡ���¶�����9�ֽڣ�ID����8�ֽ�
private:
	inline void DS18B20_DATA_OUT(bool a) {
		if (a)
			GPIO_SetBits(port, pin);
		else
			GPIO_ResetBits(port, pin);
	}
	inline u8 DS18B20_DATA_IN(u16 ppin) {
		return GPIO_ReadInputDataBit(port, ppin);
	}
	inline void DS18B20_Mode_IPU() {
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //��������
		GPIO_Init(port, &GPIO_InitStructure);

	}
	inline void DS18B20_Mode_Out_OD() {
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   //�������
		GPIO_Init(port, &GPIO_InitStructure);

	}
	void DS18B20_Rst();
	u8 DS18B20_Read_Bit();
	u8 DS18B20_Read_Byte();
	void DS18B20_Write_Byte(u8 dat);
	u8 CRC_18B20(u8 dizhi, u8 x);
};

#endif /* __DS18B20_H */