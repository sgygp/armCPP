/*
 * Can1.cpp
 *
 *  Created on: 2021年3月5日
 *      Author: Administrator
 */

#include "Can1.h"
#include "HuanjingShuju.h"
#include "Cunchu.h"
CanRxMsg Can_1::CAN_Rx_Msg;
u8 Can_1::error[];

Can_1::Can_1() {
	// TODO 自动生成的构造函数存根

}

Can_1::~Can_1() {
	// TODO 自动生成的析构函数存根
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
	//工作模式的设置
	CAN_init.CAN_Mode = CAN_Mode_Normal;
	//工作模式的配置:可配置为正常模式,静默模式,回环模式,静默回环模式;

	/*	CAN_init.CAN_TXFP = DISABLE;
	CAN_init.CAN_NART = ENABLE; //使能
	CAN_init.CAN_RFLM = DISABLE;	//缓冲区报文不锁定
	CAN_init.CAN_ABOM = DISABLE;	//自动离线管理启用
	 CAN_init.CAN_AWUM = DISABLE;	//自动唤醒启用
	 CAN_init.CAN_TTCM = DISABLE;
	 */
	//发送FIFO使能
	CAN_init.CAN_TXFP = ENABLE;

	//不自动重发功能使能
	CAN_init.CAN_NART = ENABLE; //使能

	//FIFO锁定功能
	//	 FIFO锁定功能主要用于管理接收邮箱
	//	 如果不使能FIFO锁定功能,当FIFO使能时,则在FIFO邮箱已满之后,后续的信息将会覆盖最后接收的信息
	//	 如果使能FIFO锁定功能,则在FIFO邮箱已满之后,将丢弃最新的消息，保留最原始的三则消息
	CAN_init.CAN_RFLM = DISABLE;	//缓冲区报文不锁定

	//总线自动恢复功能
	CAN_init.CAN_ABOM = ENABLE;	//自动离线管理启用

	CAN_init.CAN_AWUM = ENABLE;	//自动唤醒启用
	CAN_init.CAN_TTCM = DISABLE;


	//--------------------------
	//关于CAN总线比特率计算
	/*
	 NominalBitTime = 1*Tq +tBS1 +  tBS2
	 tBS1 = tq x (TS1[3:0] + 1);
	 tBS2 = tq x (TS2[2:0] + 1)，
	 Tq = (BRP[9:0] + 1) x TPCLK;
	 TPCLK = APB 时钟的时间周期;
	 BRP[9:0], TS1[3:0] 和 TS2[2:0] 在 CAN_BTR 寄存器中定义;
	 //-------------------------------------------------------------
	 CAN 波特率 = RCC_APB1Periph_CAN1 / Prescaler / (SJW + BS1 + BS2);

	 SJW = synchronisation_jump_width
	 BS = bit_segment
	 本例中，设置CAN波特率为250Kbps
	 CAN 波特率 = 420000000 / 8 / (12+8 +1) / = 1 Mbps
	 */
	CAN_init.CAN_BS1 = CAN_BS1_12tq;
	CAN_init.CAN_BS2 = CAN_BS2_8tq;
	CAN_init.CAN_SJW = CAN_SJW_1tq;
	CAN_init.CAN_Prescaler = 8;	//2时为1Mbps
	CAN_Init(CAN1, &CAN_init);
	//------------------------------------
	//	不启用过滤器
	CAN_FilterInitTypeDef CAN_FilerStructure;
	CAN_FilerStructure.CAN_FilterNumber = 0;
	CAN_FilerStructure.CAN_FilterMode = CAN_FilterMode_IdMask; //屏蔽模式，另外还有列表模式
	CAN_FilerStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilerStructure.CAN_FilterIdHigh = 0; //标识符寄存器18FFDF00
	CAN_FilerStructure.CAN_FilterIdLow = 0;
	CAN_FilerStructure.CAN_FilterMaskIdHigh = 0; //屏蔽滤波寄存器 0代表不关心，1代表关心
	CAN_FilerStructure.CAN_FilterMaskIdLow = 0;
	CAN_FilerStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0; //过滤器0关联到FIFO0
	CAN_FilerStructure.CAN_FilterActivation = ENABLE; //使能过滤器

	CAN_FilterInit(&CAN_FilerStructure);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

	//	CAN_Filter_init.CAN_FilterMode = CAN_FilterMode_IdMask;//列表模式
	//	CAN_Filter_init.CAN_FilterScale = CAN_FilterScale_32bit;//
	//	CAN_Filter_init.CAN_FilterNumber = 0;
	//	CAN_Filter_init.CAN_FilterIdHigh = 0x000;
	//	CAN_Filter_init.CAN_FilterIdLow = 0x0101;
	//	CAN_Filter_init.CAN_FilterMaskIdHigh =0x0000;
	//	CAN_Filter_init.CAN_FilterMaskIdLow =0xFFF0;
	//	CAN_FilterInit(&CAN_Filter_init);//此种配置只接收ID0X00000100-0X00000107

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
	 当数据帧的ID大于0x7FF时就认为是拓展帧,否则都为标准帧
	 */
	if (massage_id > 0x7FF) {
		CAN_Tx_Msg.IDE = CAN_ID_EXT;//发送帧的类型:CAN_Id_Standard   标准帧; CAN_Id_Extended  拓展帧
	} else {
		CAN_Tx_Msg.IDE = CAN_ID_STD;//发送帧的类型:CAN_Id_Standard   标准帧; CAN_Id_Extended  拓展帧
	}
	CAN_Tx_Msg.DLC = leng;	//发送数据长度

	CAN_Tx_Msg.RTR = CAN_RTR_DATA;//发送帧的类型:CAN_RTR_DATA  发送数据帧;CAN_RTR_REMOTE 远程帧,远程帧中没有数据;
	CAN_Tx_Msg.StdId = massage_id;	//标准帧的ID,11位
	CAN_Tx_Msg.ExtId = massage_id;	//拓展帧的ID,29位
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
	}else if(ID==0x103)//第九路AD值给166寄存器
	{
		u16 x=0;
		x=tt[0];x<<=8;x|=tt[1];
		HuanjingShuju::m_shuju[32+166-140]=x;

	}else if(ID==0x104)//ECPH值
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
 CAN 的各个成员变量的意义：
 1、 CAN_TTCM：该成员用于配置 CAN 的时间触发通信模式。在此模式下，CAN 使用它内部定时器产生时间戳，被保存在 CAN_RDTxR、CAN_TDTxR 寄存器中。内部定时器在每个 CAN 位时间累加，在接收和发送的帧起始位被采样，并生成时间戳。我们没有用该模式。
 2、 CAN_ABOM：当 CAN 检测到发送错误（TEC）或接收错误（REC）超过一定值时，会自动进入离线状态。在离线状态中，CAN 不能接收或发送报文。其中的发送错误或接收错误的计算原则由 CAN 协议规定，是 CAN 硬件检测的，不需要软件干预。软件可干预的是通过此 CAN_ABOM 参数选择是否使用自动离线管理，决定 CAN 硬件在什么条件下可以退出离线状态。若我们把该成员变量赋值为 ENABLE，这使用硬件自动离线管理。一旦硬件检测到 128 次 11 位连续的隐性位，则自动退出离线状态。若我们赋值为 DISABLE，离线状态由软件管理。首先由软件对 CAN_MCR 寄存器的 INRQ 位进行置“1”随后清“0”，再等到硬件 128 次 11 位连续的隐性位，才退出离线状态。
 3、 CAN_AWUM：本成员选择是否开启自动唤醒功能。若使能了自动唤醒功能，并且CAN 处于睡眠模式检测到 CAN 总线活动时会自动进入正常模式以便收发数据。若禁止此功能则由软件配置才可以退出睡眠模式。
 4、 CAN_NART：本成员用于选择是否禁止报文自动重传。按照 CAN 的标准，CAN 发送失败时会自动重传至成功为止。本参数赋值 ENABLE，即禁止自动重传，若赋值为 DISABLE，则允许自动重传。
 5、 CAN_RFLM：本成员用于配置接收 FIFO 是否锁定。若选择 ENABLE，则当 FIFO 溢出时会丢弃下一个接收的报文。若选择 DISABLE，当 FIFO 溢出时下一个接收到的报文会覆盖原报文。

 */

