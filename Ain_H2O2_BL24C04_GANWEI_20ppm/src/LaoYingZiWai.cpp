/*
 * LaoYingZiWai.cpp
 *
 *  Created on: 2022年8月8日
 *      Author: Administrator
 */

#include "LaoYingZiWai.h"
#include "string.h"

LaoYingZiWai::LaoYingZiWai(Modbus *mm) {
	// TODO 自动生成的构造函数存根
	m_modbus = mm;

}

LaoYingZiWai::~LaoYingZiWai() {
	// TODO 自动生成的析构函数存根
}

void LaoYingZiWai::run(u32 ms) {
	m_modbus->runMain();
	if(ms%200==0)
		m_modbus->DoCommand();
	if ((ms % 1000) == 0) {
			m_modbus->putReadCommand(0x80, 0X7D, 9);
			if(m_error<GUZHANGCISHU_)
				m_error++;
	}else 	if ((ms % 1000) == 500) {
			m_modbus->putReadCommand(0x80, 0X32, 6);
	}
	if (m_modbus->m_JieshouOK) {
		m_modbus->m_JieshouOK = false; //用完复位
		if (m_modbus->m_shebeiAddr== 0x80) {

			if(m_modbus->m_jicunqiShouAddr==0x7D)
			{
				u16 *u=m_modbus->m_jicunqiShou;
				u16 a=u[0];u[0]=u[1];u[1]=a;
				for(int i=0;i<3;i++)
				{
					a=u[i*2+3+0];u[i*2+3+0]=u[i*2+3+1];u[i*2+3+1]=a;
				}
				memcpy((void *)&m_shuju, &m_modbus->m_jicunqiShou, 4);
				m_shuju.m_flag=m_modbus->m_jicunqiShou[2];
				memcpy((void *)&m_shuju.m_nongdu, &m_modbus->m_jicunqiShou[3], 6*2);
				m_error=0;
			}else if(m_modbus->m_jicunqiShouAddr==0x32)
			{
				u16 *u=m_modbus->m_jicunqiShou;
				u16 a;
				for(int i=0;i<3;i++)
				{
					a=u[i*2+0];u[i*2+0]=u[i*2+1];u[i*2+1]=a;
				}
				memcpy((void *)&m_shuju.m_xishu, m_modbus->m_jicunqiShou, 6*2);
			}
		}
	}
}
void LaoYingZiWai::JiaoLing(){
	this->m_modbus->putSetCommand03(0x80,0x30,1);
}
void LaoYingZiWai::XieXishu(u8 no,float benciXishu){

	benciXishu*=m_shuju.m_xishu[no];
	u16*xx=(u16 *)&benciXishu;

	u16 a=xx[0];
	xx[0]=xx[1];
	xx[1]=a;
	this->m_modbus->putSetCommand06(0x80,0x32+no*2,xx,2);

}
