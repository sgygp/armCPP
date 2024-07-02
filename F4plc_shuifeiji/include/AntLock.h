/*
 * AntLock.h
 *
 *  Created on: 2020年12月15日
 *      Author: Administrator
 */

#ifndef ANTLOCK_H_
#define ANTLOCK_H_
#include "stm32f4xx_conf.h"
/*
 解锁步骤：最多解锁6次，数据有效期为1年内的天数，或者永久有效。
 此番操作的目的就是可以明着告诉客户，改写的内容。他们以后再想这样改也无效了。
 屏幕上做一个下发命令页面，可以填写寄存器地址，跟寄存器值。远控直接操作寄存器即可
 寄存器地址，用4+5+7（日月年）的方式做成16位数值即可。这个值最小是01日 01月 00年组合，即0x1080，十进制4224，我程序里面正常用4224以下的寄存器。
 程序运行步骤：
 1.收到改写寄存器值命令，判断寄存器地址， 此地址跟日期后6位换算后相同，继续。否则退出
 2.此地址跟6个u16类型值（Cunchu::m_canshu1[44-55]），相比较。
 如果有相同（此日期已经解锁过）退出，如果找不到空位（数据为0）也就是说已解锁6次退出，找个空位保存下此值。继续
 3.判断寄存器的值，如果为999，设为永久解锁。如果大于1且小于366，设为运行天数。其他值退出
 4.改写天数

 设备再次锁住后，客户破解一般会根据你下发的内容，重新下发一次。
 他们发现不成功首先会想到是不是跟日期有关，把日期改为以前解锁时的日期。
 但是这时候我们内部已保存下这一天已解锁过了。不能再解了。
 他们必须用新日期，新寄存器修改。这个他们就难猜了。
 */

class Ant_Lock {
public:
	static u32	m_secs ; //锁住开机可以运行半天，设为3600*12为到期立即停机
	static void run();

	static u16 getWordRiqi(RTC_DateTypeDef* DataStructure) {

		u16 uu = 0;
		u8 day = DataStructure->RTC_Date;
		u8 mon = DataStructure->RTC_Month;
		u8 year = DataStructure->RTC_Year;
		if(year>=100)
			year-=100;
		uu |= mon;
		uu <<= 5;
		uu |= day;
		uu <<= 7;
		uu |= year;
		return uu;

	}



};

#endif /* ANTLOCK_H_ */
