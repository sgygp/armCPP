#ifndef _COMMAND_H_
#define _COMMAND_H_

static const char AT_BLUETOOTH_READBLE[]={"AT+TM\r\n"};//应该返回TM+XXXXX
//static const char AT_BLUETOOTH_SET9600[]={"AT+CT01\r\n"};
//static const char AT_BLUETOOTH_SET115200[]={"AT+CT05\r\n"};
//static const char AT_BLUETOOTH_RESET[]={"AT+CZ\r\n"};
static const char AT_BLUETOOTH_SETBLE_NAME[]={"AT+BMpengbaomu_BLE\r\n"};
static const char AT_BLUETOOTH_SETSPP_NAME[]={"AT+BDpengbaomu_SPP\r\n"};
static const char AT_BLUETOOTH_CLOSEINFO[]={"AT+CR00\r\n"};//关闭回传信息

static const char AT_IDTU_IMEI[] = "rrpc,getimei\r\n";
static const char AT_IDTU_ICCID[] = "rrpc,geticcid\r\n";
static const char AT_IDTU_CSQ[] = "rrpc,getcsq\r\n";
static const char AT_IDTU_LOC[] = "rrpc,getlocation\r\n";
static const char AT_IDTU_TIME[] = "rrpc,gettime\r\n";
//static const char AT_IDTU_BAT[] = "rrpc,getvbatt\r\n";



#endif /* _COMMAND_H_ */
