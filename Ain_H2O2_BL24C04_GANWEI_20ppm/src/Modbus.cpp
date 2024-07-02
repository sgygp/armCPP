/*
 * Modbus.cpp
 *
 *  Created on: 2021年3月9日
 *      Author: Administrator
 */

#include "diag/Trace.h"
#include "Modbus.h"
#include "CRC16.h"
#include "Timer.h"
#include"string.h"
#include "diag/trace.h"

Modbus::Modbus(COM *com,u8 no) {
	// TODO 自动生成的构造函数存根
	m_com = com;
	m_NO=no;
}

//void Modbus::run(u32 ms,u8 addr,u32 zhouqi,u16 jicunqiStart,u8 geshu) { //5ms运行一次
//	if (ms % 100 == 0) //每100毫秒执行一次指令
//		DoCommand();
//	if((ms/1000%zhouqi)==0)
//		this->putReadCommand(addr,jicunqiStart,geshu);
//
//	runMain();
//
//}



void Modbus::putCommand(u8 geshu) {
	if (m_commandCishu >= GUZHANGCISHU) {
		m_bMainError = true;
		m_commandCishu = 0;
	}
	m_commandBuf[m_pW][0] = geshu;
	memcpy(m_commandBuf[m_pW] + 1, temp, geshu);
	m_pW = (m_pW + 1) % 10;
	m_commandCishu++;
}
void Modbus::DoCommand() {
	if (m_pR == m_pW)
		return;
	u16 x = m_commandBuf[m_pR][3];
	x <<= 8;
	x += m_commandBuf[m_pR][4];
	m_jicunqiShouAddr = x;
	u8 len = m_commandBuf[m_pR][0];
	memcpy(m_com->m_data[m_NO].m_FA, m_commandBuf[m_pR] + 1, len);
	m_pR = (m_pR + 1) % 10;
	if (m_commandBuf[m_pR][0] == 3)
		m_JieshouOK = false; //先复位
	else if (m_commandBuf[m_pR][0] == 6)
		m_SetOK = false; //先复位
	m_com->FasongX(len);

	static u32 zongshu=0;
	zongshu++;
//	trace_printf("%u\n",zongshu);

}
void Modbus::putSetCommand06(u8 addr, u16 jicunqi, u16* buff,u16 jicunqishu) {
	u8 *buf = temp;
	buf[0] = addr;
	buf[1] = 0x10;
	buf[2] = jicunqi >> 8;
	buf[3] = jicunqi;
	buf[4] = jicunqishu >> 8;
	buf[5] = jicunqishu;
	buf[6]=jicunqishu*2;
	int i=0;
	for(;i<jicunqishu;i++)
	{
		buf[7+i*2]=buff[i]>>8;
		buf[8+i*2]=buff[i];
	}
	CRC16::JisuanCRC(buf, 7+jicunqishu*2, true);
	putCommand(9+jicunqishu*2);

}
void Modbus::putSetCommand03(u8 addr, u16 jicunqi, u16 zhi) {
	u8 *buf = temp;
	buf[0] = addr;
	buf[1] = 0x06;
	buf[2] = jicunqi >> 8;
	buf[3] = jicunqi;
	buf[4] = zhi >> 8;
	buf[5] = zhi;
	CRC16::JisuanCRC(buf, 6, true);
	putCommand(8);
}
void Modbus::putReadCommand(u8 addr, u16 start, u16 geshu) {
	u8 *buf = temp;
	buf[0] = addr;
	buf[1] = 0x03;
	buf[2] = start >> 8;
	buf[3] = start;
	buf[4] = geshu >> 8;
	buf[5] = geshu;
	CRC16::JisuanCRC(buf, 6, true);
	putCommand(8);
}
u16 Modbus::getWord(u8 *b) {
	u16 u = b[0];
	u <<= 8;
	u |= b[1];
	return u;
}
u8 Modbus::readCOM() {
	u32 d1 = 0;
	COMDATA *ddata=&m_com->m_data[m_NO];
	if (!ddata->m_shouPos)
		return false;
	d1 = (Timer::ms5_jishu  - ddata->m_curShou_ms5);
	if (d1 < COM_SHOUKONGXIAN)
		return false;
	u8 len = ddata->m_shouPos;
	ddata->m_shouPos = 0;
	return len;

}
void Modbus::runMain() {
	u8*buf = m_com->m_data[m_NO].m_SHOU;
	u8 len = readCOM();
	if (len < (1 * 2 + 5)) //返回数据至少是1个寄存器的值，7字节
		return;
	m_shebeiAddr = buf[0];
	u16 crc = CRC16::JisuanCRC(buf, len - 2, false);
	u16 bufCRC = buf[len - 1];
	bufCRC <<= 8;
	bufCRC |= buf[len - 2];
	if (crc != bufCRC)
		return;
	m_bMainError = false; //收到CRC通过的数据，立即解除故障状态
	if (buf[1] == 0x03) { //读取
		m_jicunqiShouLen = (len - 5) / 2;
		for (int i = 0; i < m_jicunqiShouLen; i++) {
			m_jicunqiShou[i] = getWord(buf + 3 + 2 * i);
		}
		m_JieshouOK = true;
	} else if (buf[1] == 0x06) //设置成功
			{
		m_SetOK = true;
	}
}
void Modbus::putReturnDataCommand() {
	//调用前提是各个参数已经放到m_jicunqiFa中，
	//寄存器地址在m_jicunqiShouAddr,
	//寄存器个数在m_jicunqiShouLen

	u8 *buf = temp;
	buf[0] = m_com->m_data[m_NO].m_SHOU[0];
	buf[1] = 0x03;
	buf[2] = m_jicunqiShouLen*2;
	for (int i = 0; i < m_jicunqiShouLen; i++) {
		buf[3+ i * 2] = m_jicunqiFa[i] >> 8;
		buf[3 + i * 2 + 1] = m_jicunqiFa[i];
	}
	CRC16::JisuanCRC(buf, 3 + m_jicunqiShouLen * 2, true);
	putCommand(3 + m_jicunqiShouLen * 2 + 2);

}
void Modbus::putReturnSetCommand() {
	//调用前提参数已经设置好 寄存器地址在m_jicunqiShouAddr
	//寄存器值在m_jicunqiShou[0]
//	u8 *buf = temp;
//	buf[0] = *m_addr;
//	buf[1] = 0x06;
//	buf[2] = m_jicunqiShouAddr >> 8;
//	buf[3] = m_jicunqiShouAddr;
//	buf[4] = m_jicunqiShou[0] >> 8;
//	buf[5] = m_jicunqiShou[0];
//	CRC16::JisuanCRC(buf, 6, true);
	memcpy(temp,m_com->m_data[m_NO].m_SHOU,8);
	putCommand(8);

}
void Modbus::runTerminal(u8 addr) {
	u8*buf = m_com->m_data[m_NO].m_SHOU;
	u8 len = readCOM();
	if(len!=8)
		return;
	if (buf[0] != addr&&buf[0]!=0xff)  //主站下发的数据都是8字节
	{
	//	trace_printf("buf[0]=%u\n",buf[0]);
		return;
	}
	u16 crc = CRC16::JisuanCRC(buf, 6, false);
	u16 bufCRC = buf[7];
	bufCRC <<= 8;
	bufCRC |= buf[6];
	if (crc != bufCRC)
		return;
	m_jicunqiShouAddr = getWord(buf + 2);
	if (buf[1] == 0x03) { //收到读取指令
		m_JieshouOK = true;
		m_jicunqiShouLen = getWord(buf + 4);
	} else if (buf[1] == 6) {
		m_jicunqiShou[0] = getWord(buf + 4);
		m_SetOK = true;
	}

}
