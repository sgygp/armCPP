/*
 * Can1.h
 *
 *  Created on: 2021Äê3ÔÂ5ÈÕ
 *      Author: Administrator
 */

#ifndef CAN1_H_
#define CAN1_H_
#include "stm32f4xx_conf.h"
#define CAN_Tx_Port  GPIOD
#define CAN_Tx_Pin    GPIO_Pin_1
#define CAN_Rx_Port  GPIOD
#define CAN_Rx_Pin    GPIO_Pin_0

#define CAN_TONGXUANERROR 5

class Can_1 {
public:
	Can_1();
	virtual ~Can_1();

	static CanRxMsg CAN_Rx_Msg;
	CanTxMsg CAN_Tx_Msg;
	u8 msg_box = 0;
	u8 std = 0;
	static u8 error[2];
	u8 tempBuf[16];

public:
	void init();
	void out(u32 a);
	u8 CAN_send_massage(u8 *data, u8 leng, u32 massage_id);
};

#endif /* CAN1_H_ */
