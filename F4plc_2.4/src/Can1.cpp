/*
 * Can1.cpp
 *
 *  Created on: 2021年3月5日
 *      Author: Administrator
 */

#include "Can1.h"
#include "PortSet.h"
CanRxMsg Can_1::CAN_Rx_Msg;
u8 Can_1::error[];
u32 Can_1::m_key=0;
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
	CAN_FilterInitTypeDef CAN_Filter_init;
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
	CAN_Filter_init.CAN_FilterActivation = ENABLE;	//过滤器使能
//选择使用哪个FIFO,可为CAN_FilterFIFO0,CAN_FilterFIFO1;
	CAN_Filter_init.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;

	CAN_Filter_init.CAN_FilterMode = CAN_FilterMode_IdList;	//掩码模式,此处可配置为掩码模式和列表模式
	CAN_Filter_init.CAN_FilterScale = CAN_FilterScale_16bit;	//
	CAN_Filter_init.CAN_FilterNumber = 0;
	CAN_Filter_init.CAN_FilterIdHigh = 0X101 << 5;
	CAN_Filter_init.CAN_FilterMaskIdHigh = 0X102 << 5;
	CAN_Filter_init.CAN_FilterIdLow = 0X103 << 5;
	CAN_Filter_init.CAN_FilterMaskIdLow = 0X104 << 5;
	CAN_FilterInit(&CAN_Filter_init);

//	CAN_Filter_init.CAN_FilterMode = CAN_FilterMode_IdMask;//列表模式
//	CAN_Filter_init.CAN_FilterScale = CAN_FilterScale_32bit;//
//	CAN_Filter_init.CAN_FilterNumber = 0;
//	CAN_Filter_init.CAN_FilterIdHigh = 0x000;
//	CAN_Filter_init.CAN_FilterIdLow = 0x0101;
//	CAN_Filter_init.CAN_FilterMaskIdHigh =0x0000;
//	CAN_Filter_init.CAN_FilterMaskIdLow =0xFFF0;
//	CAN_FilterInit(&CAN_Filter_init);//此种配置只接收ID0X00000100-0X00000107

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_init;
	NVIC_init.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_init.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_init.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_Init(&NVIC_init);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
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
//	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) == SET) {
		CAN_Receive(CAN1, CAN_FIFO0, &Can_1::CAN_Rx_Msg);
		if (Can_1::CAN_Rx_Msg.DLC != 4)
			return;
		Can_1::m_key =(Can_1::CAN_Rx_Msg.StdId<<24)
				+(u32(Can_1::CAN_Rx_Msg.Data[2])<<16)
				+(u32(Can_1::CAN_Rx_Msg.Data[1])<<8)
				+Can_1::CAN_Rx_Msg.Data[0];
		if (Can_1::CAN_Rx_Msg.StdId>0x100&&int(Can_1::CAN_Rx_Msg.StdId-0x100)<16)
			Can_1::error[Can_1::CAN_Rx_Msg.StdId - 0x101] = 0;
//	}
}

void Can_1::Out(u32 ms10, u8 zhanshu) {

	if(zhanshu>14)
		zhanshu=14;
	int x=ms10%(zhanshu*2);
	int y=x/2;
	y++;
	if(x%2==0)//发送一次站号
		{
			tempBuf[0]=y;
			tempBuf[1]=255-(y);
			CAN_send_massage(tempBuf,2,0x400);
		}
	else  //再发送一次站数据
		{
			u32 a=PortSet::m_OUT[y];
//			u32 a=0x01234567;
			tempBuf[0]=a&0xff;
			a>>=8;
			tempBuf[1]=a&0xff;
			a>>=8;
			tempBuf[2]=a&0xff;
			a>>=8;
			tempBuf[3]=a&0x0f;;
			tempBuf[3]|=(y<<4);
			CAN_send_massage(tempBuf,4,0x08);


		}
}
/*
 CAN 的各个成员变量的意义：
 1、 CAN_TTCM：该成员用于配置 CAN 的时间触发通信模式。在此模式下，CAN 使用它内部定时器产生时间戳，被保存在 CAN_RDTxR、CAN_TDTxR 寄存器中。内部定时器在每个 CAN 位时间累加，在接收和发送的帧起始位被采样，并生成时间戳。我们没有用该模式。
 2、 CAN_ABOM：当 CAN 检测到发送错误（TEC）或接收错误（REC）超过一定值时，会自动进入离线状态。在离线状态中，CAN 不能接收或发送报文。其中的发送错误或接收错误的计算原则由 CAN 协议规定，是 CAN 硬件检测的，不需要软件干预。软件可干预的是通过此 CAN_ABOM 参数选择是否使用自动离线管理，决定 CAN 硬件在什么条件下可以退出离线状态。若我们把该成员变量赋值为 ENABLE，这使用硬件自动离线管理。一旦硬件检测到 128 次 11 位连续的隐性位，则自动退出离线状态。若我们赋值为 DISABLE，离线状态由软件管理。首先由软件对 CAN_MCR 寄存器的 INRQ 位进行置“1”随后清“0”，再等到硬件 128 次 11 位连续的隐性位，才退出离线状态。
 3、 CAN_AWUM：本成员选择是否开启自动唤醒功能。若使能了自动唤醒功能，并且CAN 处于睡眠模式检测到 CAN 总线活动时会自动进入正常模式以便收发数据。若禁止此功能则由软件配置才可以退出睡眠模式。
 4、 CAN_NART：本成员用于选择是否禁止报文自动重传。按照 CAN 的标准，CAN 发送失败时会自动重传至成功为止。本参数赋值 ENABLE，即禁止自动重传，若赋值为 DISABLE，则允许自动重传。
 5、 CAN_RFLM：本成员用于配置接收 FIFO 是否锁定。若选择 ENABLE，则当 FIFO 溢出时会丢弃下一个接收的报文。若选择 DISABLE，当 FIFO 溢出时下一个接收到的报文会覆盖原报文。

 */

