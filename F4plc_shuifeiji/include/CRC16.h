/*
 * CRC16.h
 *
 *  Created on: 2020Äê9ÔÂ28ÈÕ
 *      Author: Administrator
 */

#ifndef CRC16_H_
#define CRC16_H_

class  CRC16{
public:
	static unsigned short m_crc;
public :
	static unsigned short JisuanCRC(unsigned char *buf,unsigned char  geshu,bool write);
	static void _crc(unsigned char  x);
};



#endif /* CRC16_H_ */
