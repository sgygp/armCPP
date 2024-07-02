#include "diag/Trace.h"
#include "AIR720.h"
#include "string.h"
#include "CalendarTimer.h"


u16 Air720::XINHAO = 0;
long Air720::JINGDU = 0;     //��������վ1��ַ
long Air720::WEIDU = 0;
uint64_t Air720::IMEI = 0;
u8 Air720::m_iccid[20]={0};

bool Air720::m_OK=true;//��ȡ����ʱ������ź��������Ƿ����
bool Air720::m_bShuju=true;//30������������ͨѶ

void  Air720::getIMEI(u8 *buf)
{
	int i=0;
	uint64_t lll=0;
	while(buf[i]>0x20)
	{
		lll*=10;lll+=(buf[i]-'0');
		i++;
	}
	Air720::IMEI=lll;
	trace_printf("%lld\n",Air720::IMEI);

}
void  Air720::getLoction(u8 *buf)
{
	/*
	 036.9003039,118.7580669
	 */
	if(buf[3]=='\r'&&buf[4]=='\n')
		return;
	long uu=0;
	for(int i=0;i<11;i++)
	{
		if(buf[i]=='.')
			continue;
		uu*=10;
		uu+=buf[i]-'0';
	}
	Air720::WEIDU=uu; 
	uu=0;
	for(int i=12;i<12+11;i++)
	{
		if(buf[i]=='.')
			continue;
		uu*=10;
		uu+=buf[i]-'0';
	}
	Air720::JINGDU=uu; 
	trace_printf("%ld,%ld\n",JINGDU,WEIDU);

}
void  Air720::getICCID(u8 *buf)
{
	trace_printf("%s",buf);
	/*
	u8 x;
	u16 *a=(u16*)buf;
	for(int i=0;i<10;i++)
	{
		x=a[i]>>8;
		a[i]<<=8;
		a[i]|=x;
	}
	 */
	memcpy(Air720::m_iccid,buf,20);
	Air720::m_OK=true;
	return;
}
bool Air720::isBaohanString(u8 *buf,const char *cc)
{
	for(int i=0;i<100;i++)
	{
		if(cc[i]<0x20)
			break;
		if(buf[i]!=(u8)cc[i]){
			return false;
		}
	}
	return true;
}
void Air720::setTime(u8 *buf)
{//��buf�н����������գ���ֵ��time��Ȼ��д��ʱ��оƬ
	if(!(buf[0]=='2'&&buf[1]=='0'&&buf[4+3*0]==','&&buf[4+3*1]==','&&buf[4+3*2]==','&&buf[4+3*3]==','&&buf[4+3*4]==','&&buf[4+3*5]=='\r'&&buf[4+3*5+1]=='\n'))
		return;
	int year = 100+(buf[2]-'0')*10+buf[3]-'0'; //��1900������������
	int mon = (buf[5]-'0')*10+buf[6]-'0'-1;    // 0-11 10
	int mday =(buf[8]-'0')*10+buf[9]-'0';   // 1-31
	int hour = (buf[11]-'0')*10+buf[12]-'0';   // 0-23
	int min =(buf[14]-'0')*10+buf[15]-'0';    // 0-59
	int sec = (buf[17]-'0')*10+buf[18]-'0';    //�룬������Χ�� 0 �� 59���������� 61


	CalendarTimer::SetCalendarTime(year,mon,mday,hour,min,sec);
}
