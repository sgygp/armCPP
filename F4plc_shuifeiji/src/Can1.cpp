/*
 * Can1.cpp
 *
 *  Created on: 2021��3��5��
 *      Author: Administrator
 */

#include "Can1.h"
#include "HuanjingShuju.h"
#include "Cunchu.h"
CanRxMsg Can_1::CAN_Rx_Msg;
u8 Can_1::error[];

Can_1::Can_1() {
	// TODO �Զ����ɵĹ��캯�����

}

Can_1::~Can_1() {
	// TODO �Զ����ɵ������������
}
void Can_1::init() {
	GPIO_InitTypeDef GPIO_init;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_PinAFConfig(CAN_Tx_Port, GPIO_PinSource0, GPIO_AF_CAN1);
	GPIO_PinAFConfig(CAN_Rx_Port, GPIO_PinSource1, GPIO_AF_CAN1);
	GPIO_init.GPIO_Pin = CAN_Tx_Pin | CAN_Rx_Pin;
	GPIO_init.GPIO_Mode = GPIO_Mode_AF;
	GPIO_init.GPIO_OType = GPIO_OType_PP;
	GPIO_init.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_init.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(CAN_Tx_Port, &GPIO_init);

	CAN_InitTypeDef CAN_init;
	CAN_DeInit(CAN1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	//����ģʽ������
	CAN_init.CAN_Mode = CAN_Mode_Normal;
	//����ģʽ������:������Ϊ����ģʽ,��Ĭģʽ,�ػ�ģʽ,��Ĭ�ػ�ģʽ;

	/*	CAN_init.CAN_TXFP = DISABLE;
	CAN_init.CAN_NART = ENABLE; //ʹ��
	CAN_init.CAN_RFLM = DISABLE;	//���������Ĳ�����
	CAN_init.CAN_ABOM = DISABLE;	//�Զ����߹�������
	 CAN_init.CAN_AWUM = DISABLE;	//�Զ���������
	 CAN_init.CAN_TTCM = DISABLE;
	 */
	//����FIFOʹ��
	CAN_init.CAN_TXFP = ENABLE;

	//���Զ��ط�����ʹ��
	CAN_init.CAN_NART = ENABLE; //ʹ��

	//FIFO��������
	//	 FIFO����������Ҫ���ڹ����������
	//	 �����ʹ��FIFO��������,��FIFOʹ��ʱ,����FIFO��������֮��,��������Ϣ���Ḳ�������յ���Ϣ
	//	 ���ʹ��FIFO��������,����FIFO��������֮��,���������µ���Ϣ��������ԭʼ��������Ϣ
	CAN_init.CAN_RFLM = DISABLE;	//���������Ĳ�����

	//�����Զ��ָ�����
	CAN_init.CAN_ABOM = ENABLE;	//�Զ����߹�������

	CAN_init.CAN_AWUM = ENABLE;	//�Զ���������
	CAN_init.CAN_TTCM = DISABLE;


	//--------------------------
	//����CAN���߱����ʼ���
	/*
	 NominalBitTime = 1*Tq +tBS1 +  tBS2
	 tBS1 = tq x (TS1[3:0] + 1);
	 tBS2 = tq x (TS2[2:0] + 1)��
	 Tq = (BRP[9:0] + 1) x TPCLK;
	 TPCLK = APB ʱ�ӵ�ʱ������;
	 BRP[9:0], TS1[3:0] �� TS2[2:0] �� CAN_BTR �Ĵ����ж���;
	 //-------------------------------------------------------------
	 CAN ������ = RCC_APB1Periph_CAN1 / Prescaler / (SJW + BS1 + BS2);

	 SJW = synchronisation_jump_width
	 BS = bit_segment
	 �����У�����CAN������Ϊ250Kbps
	 CAN ������ = 420000000 / 8 / (12+8 +1) / = 1 Mbps
	 */
	CAN_init.CAN_BS1 = CAN_BS1_12tq;
	CAN_init.CAN_BS2 = CAN_BS2_8tq;
	CAN_init.CAN_SJW = CAN_SJW_1tq;
	CAN_init.CAN_Prescaler = 8;	//2ʱΪ1Mbps
	CAN_Init(CAN1, &CAN_init);
	//------------------------------------
	//	�����ù�����
	CAN_FilterInitTypeDef CAN_FilerStructure;
	CAN_FilerStructure.CAN_FilterNumber = 0;
	CAN_FilerStructure.CAN_FilterMode = CAN_FilterMode_IdMask; //����ģʽ�����⻹���б�ģʽ
	CAN_FilerStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilerStructure.CAN_FilterIdHigh = 0; //��ʶ���Ĵ���18FFDF00
	CAN_FilerStructure.CAN_FilterIdLow = 0;
	CAN_FilerStructure.CAN_FilterMaskIdHigh = 0; //�����˲��Ĵ��� 0�������ģ�1�������
	CAN_FilerStructure.CAN_FilterMaskIdLow = 0;
	CAN_FilerStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0; //������0������FIFO0
	CAN_FilerStructure.CAN_FilterActivation = ENABLE; //ʹ�ܹ�����

	CAN_FilterInit(&CAN_FilerStructure);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

	//	CAN_Filter_init.CAN_FilterMode = CAN_FilterMode_IdMask;//�б�ģʽ
	//	CAN_Filter_init.CAN_FilterScale = CAN_FilterScale_32bit;//
	//	CAN_Filter_init.CAN_FilterNumber = 0;
	//	CAN_Filter_init.CAN_FilterIdHigh = 0x000;
	//	CAN_Filter_init.CAN_FilterIdLow = 0x0101;
	//	CAN_Filter_init.CAN_FilterMaskIdHigh =0x0000;
	//	CAN_Filter_init.CAN_FilterMaskIdLow =0xFFF0;
	//	CAN_FilterInit(&CAN_Filter_init);//��������ֻ����ID0X00000100-0X00000107

	NVIC_InitTypeDef NVIC_init;
	NVIC_init.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_init.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_init.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_Init(&NVIC_init);
}
u8 Can_1::CAN_send_massage(u8 *data, u8 leng, u32 massage_id) {

	u8 tx_cnt;
	u32 tx_time_out_cnt = 0;
	/*
	 ������֡��ID����0x7FFʱ����Ϊ����չ֡,����Ϊ��׼֡
	 */
	if (massage_id > 0x7FF) {
		CAN_Tx_Msg.IDE = CAN_ID_EXT;//����֡������:CAN_Id_Standard   ��׼֡; CAN_Id_Extended  ��չ֡
	} else {
		CAN_Tx_Msg.IDE = CAN_ID_STD;//����֡������:CAN_Id_Standard   ��׼֡; CAN_Id_Extended  ��չ֡
	}
	CAN_Tx_Msg.DLC = leng;	//�������ݳ���

	CAN_Tx_Msg.RTR = CAN_RTR_DATA;//����֡������:CAN_RTR_DATA  ��������֡;CAN_RTR_REMOTE Զ��֡,Զ��֡��û������;
	CAN_Tx_Msg.StdId = massage_id;	//��׼֡��ID,11λ
	CAN_Tx_Msg.ExtId = massage_id;	//��չ֡��ID,29λ
	for (tx_cnt = 0; tx_cnt < leng; tx_cnt++) {
		CAN_Tx_Msg.Data[tx_cnt] = data[tx_cnt];
	}
	do {
		tx_time_out_cnt++;
		msg_box = CAN_Transmit(CAN1, &CAN_Tx_Msg);
	} while (((CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP0) != RESET)
			|| (CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP1) != RESET)
			|| (CAN_GetFlagStatus(CAN1, CAN_FLAG_RQCP2) != RESET))
			&& (tx_time_out_cnt < 10000));

	tx_time_out_cnt++;
	if (tx_time_out_cnt > 10000) {
		return 0;
	} else {
		return 1;
	}
}

extern "C" void CAN1_RX0_IRQHandler(void) {
	CAN_Receive(CAN1, CAN_FIFO0, &Can_1::CAN_Rx_Msg);
	u8 len=Can_1::CAN_Rx_Msg.DLC ;
		return;
	 u8 *tt=Can_1::CAN_Rx_Msg.Data;

	u32 ID=Can_1::CAN_Rx_Msg.StdId;
	if(ID>=0x101&&ID<=0x102&&len==8)
	{
		int ADitem=(ID-0x101)*4;
		u16 x=0;
		for(int i=0;i<4;i++)
		{
			x=tt[i*2];x<<=8;x|=tt[i*2+1];
			HuanjingShuju::m_AD[ADitem+i]=x;
			float ff=x;
			ff-=MA_4;
			ff/=MA_CHA;
			if(ff<0)
				ff=0;
			else if(ff>1)
				ff=1;
			ff*=Cunchu::m_sysCanshu.m_yalichuanganqiLiangcheng;
			HuanjingShuju::m_shuju[32+8+ADitem+i]=ff;
		}
	}else if(ID==0x103)//�ھ�·ADֵ��166�Ĵ���
	{
		u16 x=0;
		x=tt[0];x<<=8;x|=tt[1];
		HuanjingShuju::m_shuju[32+166-140]=x;

	}else if(ID==0x104)//ECPHֵ
	{
		u8 *a=(u8*)&HuanjingShuju::m_shuju[32+156-140];
		a[1]=tt[0];
		a[0]=tt[1];
		a=(u8*)&HuanjingShuju::m_shuju[32+158-140];
		a[1]=tt[2];
		a[0]=tt[3];
		a=(u8*)&HuanjingShuju::m_shuju[32+157-140];
		a[1]=tt[4];
		a[0]=tt[5];
		a=(u8*)&HuanjingShuju::m_shuju[32+159-140];
		a[1]=tt[6];
		a[0]=tt[7];
	}
}

void Can_1::out(u32 a) {
	tempBuf[3]=a&0xff;
	a>>=8;
	tempBuf[2]=a&0xff;
	a>>=8;
	tempBuf[1]=a&0xff;
	a>>=8;
	tempBuf[0]=a&0xff;;
	CAN_send_massage(tempBuf,4,0x100);
}
/*
 CAN �ĸ�����Ա���������壺
 1�� CAN_TTCM���ó�Ա�������� CAN ��ʱ�䴥��ͨ��ģʽ���ڴ�ģʽ�£�CAN ʹ�����ڲ���ʱ������ʱ������������� CAN_RDTxR��CAN_TDTxR �Ĵ����С��ڲ���ʱ����ÿ�� CAN λʱ���ۼӣ��ڽ��պͷ��͵�֡��ʼλ��������������ʱ���������û���ø�ģʽ��
 2�� CAN_ABOM���� CAN ��⵽���ʹ���TEC������մ���REC������һ��ֵʱ�����Զ���������״̬��������״̬�У�CAN ���ܽ��ջ��ͱ��ġ����еķ��ʹ������մ���ļ���ԭ���� CAN Э��涨���� CAN Ӳ�����ģ�����Ҫ�����Ԥ������ɸ�Ԥ����ͨ���� CAN_ABOM ����ѡ���Ƿ�ʹ���Զ����߹������� CAN Ӳ����ʲô�����¿����˳�����״̬�������ǰѸó�Ա������ֵΪ ENABLE����ʹ��Ӳ���Զ����߹���һ��Ӳ����⵽ 128 �� 11 λ����������λ�����Զ��˳�����״̬�������Ǹ�ֵΪ DISABLE������״̬�������������������� CAN_MCR �Ĵ����� INRQ λ�����á�1������塰0�����ٵȵ�Ӳ�� 128 �� 11 λ����������λ�����˳�����״̬��
 3�� CAN_AWUM������Աѡ���Ƿ����Զ����ѹ��ܡ���ʹ�����Զ����ѹ��ܣ�����CAN ����˯��ģʽ��⵽ CAN ���߻ʱ���Զ���������ģʽ�Ա��շ����ݡ�����ֹ�˹�������������òſ����˳�˯��ģʽ��
 4�� CAN_NART������Ա����ѡ���Ƿ��ֹ�����Զ��ش������� CAN �ı�׼��CAN ����ʧ��ʱ���Զ��ش����ɹ�Ϊֹ����������ֵ ENABLE������ֹ�Զ��ش�������ֵΪ DISABLE���������Զ��ش���
 5�� CAN_RFLM������Ա�������ý��� FIFO �Ƿ���������ѡ�� ENABLE���� FIFO ���ʱ�ᶪ����һ�����յı��ġ���ѡ�� DISABLE���� FIFO ���ʱ��һ�����յ��ı��ĻḲ��ԭ���ġ�

 */

