/*
 * TongxunChuli.cpp
 *
 *  Created on: 2023年5月30日
 *      Author: Administrator
 */

#include "TongxunChuli.h"
#include "string.h"
#include "delay.h"
#include "Timer.h"
#include "RtcBkp.h"
#include "Shuju.h"

TongxunChuli::TongxunChuli() {
	// TODO 自动生成的构造函数存根

}
void TongxunChuli::init(COM* com,W25X16 *w25) {
	m_com=com;
	m_25=w25;
}
u8 TongxunChuli::readCOM() {
	u32 d1 = 0;
	if (!m_com->m_data->m_shouPos)
		return 0;
	d1 = (Timer::ms5_jishu + Timer::MAXSHU - m_com->m_data->m_curShou_ms5);
	d1 = d1 % Timer::MAXSHU;
	if (d1 < COM_SHOUKONGXIAN)
		return false;
	u8 shoupos = m_com->m_data->m_shouPos;
	m_com->m_data->m_shouPos = 0;
	return shoupos;
}

bool TongxunChuli::run() {

	static int shengyuCishu=0;
	static int weizhi=0;
	static int shengyuSuduJiasuducishu=0;
	static u8 AorS=0;
	static u16 shuju[128]={0};

	int x;
	u8 *buf=m_com->m_data->m_SHOU;
	if(shengyuCishu&&m_com->m_data->m_bFasongWan)//还有未发送完的数据
	{
		int m_curCishu=0x20000/0x100-shengyuCishu;
		int duquWeizhi=weizhi+m_curCishu*0x100;//每次256个字节
		m_25->SPI_FLASH_PageReadData(duquWeizhi,256);
		Shuju::tx2buf(m_com->m_data->m_FA,m_25->spi1_tx+4);
		shengyuCishu--;
		m_com->FasongX(768);
	}else
	{
		if(shengyuCishu==0)
			weizhi=0;
	}
	if(shengyuSuduJiasuducishu&&m_com->m_data->m_bFasongWan)//还有未发送完的数据
	{
		int addr=RtcBkp::getBkpData(BKP_DR2)+1-shengyuSuduJiasuducishu;

		int benci;
		if(shengyuSuduJiasuducishu>128)
		{
			benci=128;
			shengyuSuduJiasuducishu-=128;
		}else
		{
			benci=shengyuSuduJiasuducishu;
			shengyuSuduJiasuducishu=0;
		}

		for(int i=0;i<benci;i++)
		{
			m_25->SPI_FLASH_PageReadData((addr+i)*256,60);
			shuju[i]=*(u16*)&m_25->spi1_tx[4+AorS*2];
		}
		if(AorS==4||AorS==2)
			Shuju::toJiasuduSudu(m_com->m_data->m_FA,shuju,benci,1);
		//		if(AorS==3)
		//			Shuju::toJiasuduSudu(m_com->m_data->m_FA,shuju,benci,10);
		//		else	if(AorS==2)
		//			Shuju::toJiasuduSudu(m_com->m_data->m_FA,shuju,benci,1);
		//		else	if(AorS==1)
		//			Shuju::toJiasuduSudu(m_com->m_data->m_FA,shuju,benci,10);
		else
			Shuju::toJiasuduSudu(m_com->m_data->m_FA,shuju,benci,10);
		m_com->FasongX(benci*6);
	}




	x=readCOM();
	if(!x)
		return true;
	if((x==1||x==3)&&(buf[0]=='x'||buf[0]=='X'))
	{
		int zongcishu=RtcBkp::getBkpData(BKP_DR2)+1;
		bool bYoushu=false;
		u16 aaa[4]={1000,100,10,1};
		for(int i=0;i<4;i++)
		{
			int tt=zongcishu/aaa[i]%10;
			if(tt||bYoushu||i==3)
			{
				m_com->m_data->m_FA[i+2]='0'+tt;
				bYoushu=true;
			}else
				m_com->m_data->m_FA[i+2]=' ';

		}
		m_com->m_data->m_FA[0]='x';
		m_com->m_data->m_FA[1]='=';
		m_com->m_data->m_FA[6]=0x0d;
		m_com->m_data->m_FA[7]=0x0a;
		m_com->FasongX(8);
	}else 	if(buf[0]=='v'||buf[0]=='V')//V0101  10.1
	{
		if((x==1||x==3))
		{
			shengyuSuduJiasuducishu=RtcBkp::getBkpData(BKP_DR2)+1;
			AorS=10;//傅里叶频率
		}
		else if((x==2||x==4)&&buf[1]=='?')
		{
			int k=0;
			for(int j=0;j<8;j++)
			{
				k=RtcBkp::getBkpData(BKP_DR35+4*j);
				for(int i=1;i<5;i++)
				{
					m_com->m_data->m_FA[j*5+5-i]=k%10+'0';k/=10;
				}
				m_com->m_data->m_FA[j*5]=' ';
			}
			m_com->m_data->m_FA[0]='V';
			m_com->m_data->m_FA[41]=0x0d;
			m_com->m_data->m_FA[42]=0x0a;
			m_com->FasongX(43);
		}else if(x>=5)//V0101  10.1
		{
			u16 jieguo[8]={0};
			int m=1;
			for(;m<x;m++)
			{
				if(!Shuju::isNumber(buf[m]))
					break;
			}
			int n=(m-1)/4;
			if(n>8)
				n=8;
			int aa=0;
			for(m=0;m<n;m++)
			{
				for(int i=1;i<5;i++)
				{
					aa*=10;aa+=(buf[m*4+i]-'0');
				}
				jieguo[m]=aa;
				aa=0;
			}
			for(int i=0;i<8;i++)
			{
				RtcBkp::setBkpData(BKP_DR35+4*i,jieguo[i]);
			}
			m_com->m_data->m_FA[0]='V';
			m_com->m_data->m_FA[1]='-';
			m_com->m_data->m_FA[2]='O';
			m_com->m_data->m_FA[3]='K';
			m_com->m_data->m_FA[4]=0x0d;
			m_com->m_data->m_FA[5]=0x0a;
			m_com->FasongX(6);

		}




















	}else 	if((x==1||x==3)&&(buf[0]=='a'||buf[0]=='A'))
	{
		shengyuSuduJiasuducishu=RtcBkp::getBkpData(BKP_DR2)+1;
		AorS=0;//传输加速度
	}else 	if((x==1||x==3)&&(buf[0]=='s'||buf[0]=='S'))
	{
		shengyuSuduJiasuducishu=RtcBkp::getBkpData(BKP_DR2)+1;
		AorS=1;//传输速度
	}else 	if((x==1||x==3)&&(buf[0]=='d'||buf[0]=='D'))
	{
		shengyuSuduJiasuducishu=RtcBkp::getBkpData(BKP_DR2)+1;
		AorS=2;//传输位移

	}else 	if((x==1||x==3)&&(buf[0]=='t'||buf[0]=='T'))
	{
		shengyuSuduJiasuducishu=RtcBkp::getBkpData(BKP_DR2)+1;
		AorS=4;//传输CPU温度
	}else 	if((x==1||x==3)&&(buf[0]=='n'||buf[0]=='N'))
	{
		shengyuSuduJiasuducishu=RtcBkp::getBkpData(BKP_DR2)+1;
		AorS=3;//传输峰个数
	}else 	if((x==1||x==3)&&(buf[0]=='b'||buf[0]=='B'))
	{
		shengyuSuduJiasuducishu=RtcBkp::getBkpData(BKP_DR2)+1;
		AorS=9;//幅值和
	}else 	if((x==1||x==3)&&(buf[0]=='u'||buf[0]=='U'))
	{
		shengyuSuduJiasuducishu=RtcBkp::getBkpData(BKP_DR2)+1;
		AorS=11;//傅里叶赋值
	}else 	if((x==2||x==4)&&((buf[0]=='o'||buf[0]=='O')&&(buf[1]=='k'||buf[1]=='K')))
	{
		BlinkLed::turnR_On();
		delay_ms(3000);
		int a=buf[0]-'0';
		a*=10;a+=(buf[1]-'1');
		a%=48;

		shengyuCishu=0x20000/0x100;//一次读取256字节，总共读取128K,可以再次传输
		//		weizhi=((RtcBkp::getBkpData(BKP_DR2)+48-a)%48)*0x20000+0x200000;//	起始位置2M
		weizhi=0x200000;//	起始位置2M
	}else if(((buf[0]=='l'||buf[0]=='L')))//
	{
		if((x==3||x==5)&&Shuju::isNumber(buf[1])&&Shuju::isNumber(buf[2]))
		{
			int aa=0;
			for(int i=1;i<3;i++)
			{
				aa*=10;aa+=(buf[i]-'0');
			}
			RtcBkp::setBkpData(BKP_DR32,aa);
			m_com->m_data->m_FA[0]='L';

			for(int i=1;i<3;i++)
				m_com->m_data->m_FA[i]=m_com->m_data->m_SHOU[i];
			m_com->m_data->m_FA[3]=',';
			m_com->m_data->m_FA[4]='O';
			m_com->m_data->m_FA[5]='K';
			m_com->m_data->m_FA[6]=0x0d;
			m_com->m_data->m_FA[7]=0x0a;
			m_com->FasongX(8);

		}else if(buf[1]=='?')
		{
			m_com->m_data->m_FA[0]='L';
			int l=RtcBkp::getBkpData(BKP_DR32);
			if(l>99||l<=0)
			{
				l=3;
				RtcBkp::setBkpData(BKP_DR32,l);

			}

			for(int i=1;i<3;i++)
			{
				m_com->m_data->m_FA[3-i]=l%10+'0';l/=10;
			}
			m_com->m_data->m_FA[3]=0x0d;
			m_com->m_data->m_FA[4]=0x0a;
			m_com->FasongX(5);

		}
	}else if(((buf[0]=='b'||buf[0]=='B')))//
	{
		if((x==5||x==7)&&Shuju::isNumber(buf[1])&&Shuju::isNumber(buf[2])&&Shuju::isNumber(buf[3])&&Shuju::isNumber(buf[4]))
		{
			int aa=0;
			for(int i=1;i<5;i++)
			{
				aa*=10;aa+=(buf[i]-'0');
			}
			RtcBkp::setBkpData(BKP_DR12,aa);
			m_com->m_data->m_FA[0]='B';

			for(int i=1;i<5;i++)
				m_com->m_data->m_FA[i]=m_com->m_data->m_SHOU[i];
			m_com->m_data->m_FA[5]=',';
			m_com->m_data->m_FA[6]='O';
			m_com->m_data->m_FA[7]='K';
			m_com->m_data->m_FA[8]=0x0d;
			m_com->m_data->m_FA[9]=0x0a;
			m_com->FasongX(10);

		}else if(buf[1]=='?')
		{
			m_com->m_data->m_FA[0]='B';
			int k=RtcBkp::getBkpData(BKP_DR12);
			//			if(k>999||k<=0)
			//			{
			//				k=600;
			//				RtcBkp::setBkpData(BKP_DR12,k);
			//			}

			for(int i=1;i<5;i++)
			{
				m_com->m_data->m_FA[5-i]=k%10+'0';k/=10;
			}
			m_com->m_data->m_FA[5]=0x0d;
			m_com->m_data->m_FA[6]=0x0a;
			m_com->FasongX(7);

		}
		else
		{
			m_com->m_data->m_FA[0]='n';
			m_com->m_data->m_FA[1]='o';
			m_com->m_data->m_FA[2]='S';
			m_com->m_data->m_FA[3]='E';
			m_com->m_data->m_FA[4]='T';
			m_com->m_data->m_FA[5]=0x0d;
			m_com->m_data->m_FA[6]=0x0a;
			m_com->FasongX(7);

		}
	}else if(((buf[0]=='k'||buf[0]=='K')))//
	{
		if((x==4||x==6)&&Shuju::isNumber(buf[1])&&Shuju::isNumber(buf[2])&&Shuju::isNumber(buf[3]))
		{
			int aa=0;
			for(int i=1;i<4;i++)
			{
				aa*=10;aa+=(buf[i]-'0');
			}
			RtcBkp::setBkpData(BKP_DR11,aa);
			m_com->m_data->m_FA[0]='K';

			for(int i=1;i<4;i++)
				m_com->m_data->m_FA[i]=m_com->m_data->m_SHOU[i];
			m_com->m_data->m_FA[4]=',';
			m_com->m_data->m_FA[5]='O';
			m_com->m_data->m_FA[6]='K';
			m_com->m_data->m_FA[7]=0x0d;
			m_com->m_data->m_FA[8]=0x0a;
			m_com->FasongX(9);

		}else if(buf[1]=='?')
		{
			m_com->m_data->m_FA[0]='K';
			int k=RtcBkp::getBkpData(BKP_DR11);
			if(k>999||k<=0)
			{
				k=100;
				RtcBkp::setBkpData(BKP_DR11,k);
			}

			for(int i=1;i<4;i++)
			{
				m_com->m_data->m_FA[4-i]=k%10+'0';k/=10;
			}
			m_com->m_data->m_FA[4]=0x0d;
			m_com->m_data->m_FA[5]=0x0a;
			m_com->FasongX(6);

		}
		else
		{
			m_com->m_data->m_FA[0]='n';
			m_com->m_data->m_FA[1]='o';
			m_com->m_data->m_FA[2]='S';
			m_com->m_data->m_FA[3]='E';
			m_com->m_data->m_FA[4]='T';
			m_com->m_data->m_FA[5]=0x0d;
			m_com->m_data->m_FA[6]=0x0a;
			m_com->FasongX(7);

		}
	}	else if(((buf[0]=='f'||buf[0]=='F')))//低16位
	{
		if((x==6||x==8)&&Shuju::isNumber(buf[1])&&Shuju::isNumber(buf[2])&&Shuju::isNumber(buf[3])&&Shuju::isNumber(buf[4])&&Shuju::isNumber(buf[5]))
		{
			int aa=0;
			for(int i=1;i<6;i++)
			{
				aa*=10;aa+=(buf[i]-'0');
			}
			RtcBkp::setBkpData(BKP_DR10,aa);
			m_com->m_data->m_FA[0]='F';
			for(int i=1;i<6;i++)
				m_com->m_data->m_FA[i]=m_com->m_data->m_SHOU[i];
			m_com->m_data->m_FA[6]=',';
			m_com->m_data->m_FA[7]='O';
			m_com->m_data->m_FA[8]='K';
			m_com->m_data->m_FA[9]=0x0d;
			m_com->m_data->m_FA[10]=0x0a;
			m_com->FasongX(11);
		}else if(buf[1]=='?')
		{
			m_com->m_data->m_FA[0]='F';
			int k=RtcBkp::getBkpData(BKP_DR10);
			if(k>99999||k<=0)
			{
				k=20000;
				RtcBkp::setBkpData(BKP_DR10,k);
			}

			for(int i=1;i<6;i++)
			{
				m_com->m_data->m_FA[6-i]=k%10+'0';k/=10;
			}
			m_com->m_data->m_FA[6]=0x0d;
			m_com->m_data->m_FA[7]=0x0a;
			m_com->FasongX(8);

		}else
		{
			m_com->m_data->m_FA[0]='n';
			m_com->m_data->m_FA[1]='o';
			m_com->m_data->m_FA[2]='S';
			m_com->m_data->m_FA[3]='E';
			m_com->m_data->m_FA[4]='T';
			m_com->m_data->m_FA[5]=0x0d;
			m_com->m_data->m_FA[6]=0x0a;
			m_com->FasongX(7);

		}
	}else 	if((x==3||x==5)&&buf[0]=='E'&&buf[1]=='N'&&buf[2]=='D')
	{
		return false;
	}

	return true;
}
