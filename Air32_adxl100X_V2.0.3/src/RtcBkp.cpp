/*
 * RtcBkp.cpp
 *
 *  Created on: 2020��12��31��
 *      Author: Administrator
 */

#include "RtcBkp.h"

void RtcBkp::init() {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR| RCC_APB1Periph_BKP, ENABLE);	//ʹ�ܵ�Դ�ͺ󱸽ӿ�ʱ��

}

u32 RtcBkp::getBkpData(u32 bkp_dr) {
	u32 uu = 0;
	PWR_BackupAccessCmd(ENABLE);//ʹ�ܱ���������
	BKP_ClearFlag();//�����������־λ
	uu = BKP_ReadBackupRegister(bkp_dr);
	PWR_BackupAccessCmd(DISABLE);	//�رպ󱸼Ĵ���
	return uu;
}

void RtcBkp::setBkpData(u32 bkp_dr,u32 uu) {
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������Ա��������RTC��ؼĴ���
	BKP_ClearFlag();//�����������־λ
	BKP_WriteBackupRegister(bkp_dr, uu);
	PWR_BackupAccessCmd(DISABLE);	//�رպ󱸼Ĵ���
}
