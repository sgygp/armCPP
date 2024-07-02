/*
 * RtcBkp.cpp
 *
 *  Created on: 2020年12月31日
 *      Author: Administrator
 */

#include "RtcBkp.h"

void RtcBkp::init() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR| RCC_APB1Periph_BKP, ENABLE);	//使能电源和后备接口时钟

}

u32 RtcBkp::getBkpData(u32 bkp_dr) {
	u32 uu = 0;
	PWR_BackupAccessCmd(ENABLE);//使能备份区访问
	BKP_ClearFlag();//清除备份区标志位
	uu = BKP_ReadBackupRegister(bkp_dr);
	PWR_BackupAccessCmd(DISABLE);	//关闭后备寄存器
	return uu;
}

void RtcBkp::setBkpData(u32 bkp_dr,u32 uu) {
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器，以被允许访问RTC相关寄存器
	BKP_ClearFlag();//清除备份区标志位
	BKP_WriteBackupRegister(bkp_dr, uu);
	PWR_BackupAccessCmd(DISABLE);	//关闭后备寄存器
}
