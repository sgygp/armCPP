/*
 * W25Qxx.h
 *
 *  Created on: 2024��4��3��
 *      Author: Administrator
 */

#ifndef W25QXX_H_
#define W25QXX_H_
#include "main.h"
#include "H7SPI2.h"


//W25Xϵ��/Qϵ��оƬ�б�
#define W25Q80 		0XEF13
#define W25Q16 		0XEF14
#define W25Q32 		0XEF15
#define W25Q64 		0XEF16
#define W25Q128	0XEF17
#define W25Q256 	0XEF18

extern uint16_t W25QXX_TYPE;					//����W25QXXоƬ�ͺ�

//W25QXX��Ƭѡ�ź�
#define W25QXX_CS(n)  (n?HAL_GPIO_WritePin(SPI2_CS1_GPIO_Port,SPI2_CS1_Pin,GPIO_PIN_SET):HAL_GPIO_WritePin(SPI2_CS1_GPIO_Port,SPI2_CS1_Pin,GPIO_PIN_RESET))

//////////////////////////////////////////////////////////////////////////////////
//ָ���
#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg1		0x05
#define W25X_ReadStatusReg2		0x35
#define W25X_ReadStatusReg3		0x15
#define W25X_WriteStatusReg1    0x01
#define W25X_WriteStatusReg2    0x31
#define W25X_WriteStatusReg3    0x11
#define W25X_ReadData			0x03
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0xC7
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown	0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID	0x90
#define W25X_JedecDeviceID		0x9F
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9


class W25Qxx {
public:
	void init();
	uint16_t  W25QXX_ReadID(void);  	    		//��ȡFLASH ID
	uint8_t W25QXX_ReadSR(uint8_t regno);             //��ȡ״̬�Ĵ���
	void W25QXX_4ByteAddr_Enable(void);     //ʹ��4�ֽڵ�ַģʽ
	void W25QXX_Write_SR(uint8_t regno,uint8_t sr);   //д״̬�Ĵ���
	void W25QXX_Write_Enable(void);  		//дʹ��
	void W25QXX_Write_Disable(void);		//д����
	void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
	void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //��ȡflash
	void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//д��flash
	void W25QXX_Erase_Chip(void);    	  	//��Ƭ����
	void W25QXX_Erase_Sector(uint32_t Dst_Addr);	//��������
	void W25QXX_Wait_Busy(void);           	//�ȴ�����
	void W25QXX_PowerDown(void);        	//�������ģʽ
	void W25QXX_WAKEUP(void);				//����
	void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
};

#endif /* W25QXX_H_ */
