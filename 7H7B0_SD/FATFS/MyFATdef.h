/*
 * MyFATdef.c
 *
 *  Created on: 2024��4��11��
 *      Author: Administrator
 */

#ifndef MYFATDEF_H_
#define MYFATDEF_H_
#include "main.h"

#define 	DISK_BLOCK_SIZE 8 //ÿ���ذ����������� 0 �Զ�
extern uint8_t  OW25QXX_disk_read(uint8_t *buf, uint32_t sector, uint16_t count);
extern uint8_t  OW25QXX_disk_write(uint8_t *buf, uint32_t sector, uint16_t count);

extern uint8_t SD_ReadDisk(u8 *buf,u32 sector,u16 count);
extern uint8_t SD_WriteDisk(u8 *buf,u32 sector,u16 count);

#endif /* MYFATDEF_C_ */
