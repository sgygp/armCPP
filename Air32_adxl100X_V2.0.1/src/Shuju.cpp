/*
 * Shuju.cpp
 *
 *  Created on: 2023年5月9日
 *      Author: Administrator
 */

#include "Shuju.h"

Shuju::Shuju() {
	// TODO 自动生成的构造函数存根

}
///200.3;		20.23
void Shuju::toJiasuduSudu(u8* buf, u16* data,int geshu,  int beilv) {
	u16 a=0;
	for(int i=0;i<geshu;i++)
	{
		a=data[i];
		if(a>9999)
			a=9999;
		if(beilv==10)
		{
			if(a>=1000)
				buf[0]=a/1000+'0';
			else
				buf[0]=' ';
			if(a>=100)
				buf[1]=a/100%10+'0';
			else
				buf[1]=' ';
			buf[2]=a/10%10+'0';
			buf[4]=a/1%10+'0';

			buf[3]='.';
		}else if(beilv==100)
		{
			if(a>=1000)
				buf[0]=a/1000+'0';
			else
				buf[0]=' ';
			buf[1]=a/100%10+'0';
			buf[2]='.';
			buf[3]=a/10%10+'0';
			buf[4]=a/1%10+'0';
		}else if(beilv==1)
		{
			if(a>=1000)
				buf[1]=a/1000+'0';
			else
				buf[1]=' ';
			if(a>=100)
				buf[2]=a/100%10+'0';
			else
				buf[2]=' ';
			if(a>=10)
				buf[3]=a/10%10+'0';
			else
				buf[3]=' ';
			buf[4]=a/1%10+'0';

			buf[0]=' ';
		}
		buf[5]=';';
		buf+=6;
	}
}


void Shuju::tx2buf(u8 *buf,u8 *tx){
	short *tt=(short*)tx;
	short x;
	for(int i=0;i<128;i++)
	{
		x=tt[i];
		if(x<0)
		{
			x=-x;
			buf[i*6]='-';
		}else
			buf[i*6]=' ';
		int a=0;
		for(;a<4;a++)
		{
			buf[i*6+1+3-a]='0'+x%10;x/=10;
		}
		for( a=0;a<3;a++)
		{
			if(buf[i*6+a+1]=='0')
				buf[i*6+a+1]=' ';
			else
				break;
		}
		if(buf[i*6]=='-')
		{
			buf[i*6]=' ';
			buf[i*6+a]='-';
		}
		buf[i*6+5]=';';
	}
}
void Shuju::tx2buf2(u8 *buf,u8 *tx){
	u8 x;
	for(int i=0;i<256;i++)
	{
		x=tx[i];
		int a=0;
		for(;a<3;a++)
		{
			buf[i*4+2-a]='0'+x%10;x/=10;
		}
		buf[i*4+3]=',';
	}
}
void Shuju::tx2buf3(u8 *buf,short *tx){
	u8 x;
	for(int i=0;i<256;i++)
	{
		x=tx[i];
		int a=0;
		for(;a<3;a++)
		{
			buf[i*4+2-a]='0'+x%10;x/=10;
		}
		buf[i*4+3]=',';
	}
}
short Shuju::Iabs(short a,short b)
{
	if(a>b)
		return a-b;
	else
		return b-a;
}
bool Shuju::isNumber(u8 ch){

	if(ch>='0'&&ch<='9')
		return true;
	else
		return false;
}


float Shuju::jifenX(float *shujuSRC,float *shujuTo,u16 start ,u16 end,float k)
{
	if(!(end>start+1))
		return 0;
	shujuTo[start]=0;
	float he=0;
	for(int i=start+1;i<end;i++)
	{
		shujuTo[i]=shujuTo[i-1]+(shujuSRC[i-1]*k);
		if(shujuTo[i]<0)
			he-=shujuTo[i];
		else
			he+=shujuTo[i];
	}
	he/=(end-start-1);
	return he;
}

float Shuju::jifen(float *shujuSRC,float *shujuTo,float *feng,float k)
{
	int i=0;
	float he=0;
	while(feng[i]!=0&&feng[i+1]!=0)
	{

		he+=jifenX(shujuSRC,shujuTo,feng[i],feng[i+1],k);
		i++;
	}
	if(i!=0)
		he/=i;
	return he;
}

int Shuju::qiuFeng(float* shuju, float* feng, int len) {
	int zuidaX=0;
	//	bool bGuolingDown=false;
	bool bGuolingUp=false;
	feng[0]=0;
	feng[1]=0;
	float max=0;
	int jilu=0;
	for(int i=2;i<len-2;i++)
	{
		feng[i]=0;
		if(bGuolingUp)
		{
			if(shuju[i]>max)
			{
				max=shuju[i];
				jilu=i;
			}
			if(shuju[i]<0)
			{
				bGuolingUp=false;
				feng[zuidaX]=jilu;
				zuidaX++;

			}

		}else if(!bGuolingUp)
		{
			if(shuju[i]>0)
			{
				bGuolingUp=true;
				max=0;
			}

		}
	}
	return zuidaX;
}
int Shuju::qiuGuoling(float* shuju, u16* feng, int len) {
	int shangshengX=0;
	bool bZuida=false;
	bool bZuixiao=false;
	feng[0]=0;
	feng[1]=0;
	for(int i=2;i<len-2;i++)
	{
		feng[i]=0;
		//shuju[i]-=pingjunJiasudu;
		if(bZuixiao&&shuju[i]>=0&&shuju[i-1]<0)
		{
			feng[shangshengX]=i;
			shangshengX++;
			bZuida=false;
			bZuixiao=false;
		}else if(!bZuida)
		{
			if(shuju[i]>0&&shuju[i-1]<=shuju[i]&&shuju[i+1]<=shuju[i]&&shuju[i-2]<=shuju[i]&&shuju[i+2]<=shuju[i])
				bZuida=true;
		}else if(!bZuixiao)
		{
			if(shuju[i]<0&&shuju[i-1]>=shuju[i]&&shuju[i+1]>=shuju[i]&&shuju[i-2]>=shuju[i]&&shuju[i+2]>=shuju[i])
				bZuixiao=true;
		}
	}
	return shangshengX;
}

float Shuju::jifen(float* shujuSRC, float* shujuTo, u16 nnn,u16 caiyangpinlv) {
	if(nnn<10)
		nnn=32;

	float hhhh=0;
	float *s;
	float max=0;
	int p=0;
	float x;
	float y;
	for(int j=0;j<65536/nnn;j++)
	{
		s=&shujuSRC[j*nnn];
		max=0;
		p=0;
		for(int i=0;i<nnn;i++)
		{
			if(s[i]>max)
			{
				max=s[i];
				p=i;
			}
		}

		x=s[p];
		y=x;
		s[p]=0;
		for(int i=p-1;i>=0;i--)
		{
			x=s[i];
			s[i]=s[i+1]-y*1000/caiyangpinlv;
			y=x;

		}
		for(int i=p+1;i<nnn;i++)
			s[i]=s[i-1]+s[i]*1000/caiyangpinlv;
		float h1=0;
		for(int i=0;i<nnn;i++)
		{
			if(s[i]<0)
				h1-=s[i];
			else
				h1+=s[i];
		}
		h1/=nnn;
		hhhh+=h1;
	}
	hhhh/=(65536.0/nnn);
	return hhhh;
}

void Shuju::TianjiaShuju(u8* tt, u8 x, u16 zhi, u8 weishu) {
	int zuida=1;
	for(int i=0;i<weishu;i++)
		zuida*=10;
	if(zhi>=zuida)
		zhi=zuida-1;
	for(int i=0;i<weishu;i++)
	{
		tt[x+weishu-i-1]=zhi%10+'0';zhi/=10;
	}
}
/*
u16 Shuju::JishuanPinlv128(short *buf)
{
	 int a=0;
	 int b=0;
	 int start=0;
	 int end=0;
	 int cishu=0;
	 u8 i=0;
	for(;i<128;i++)
		a+=buf[i];
	short zhongxindian=a/128;

	for(i=0;i<128;i++)
	{
		a=buf[i]-zhongxindian;
		if((b<0&&a>0)||(a==0&&b!=0)||(b>0&&a<0))
		{
			if(cishu==0)
				start=i;
			else
			{
				end=i;
			}
			cishu++;
		}
		b=a;
	}
	float pinlv=0;
	if(cishu!=0)
	{
		pinlv=(end-start)/((cishu-1)/2);
		pinlv=3200/pinlv;
	}
	return pinlv;
}
u16 Shuju::JishuanMianji128(short *buf,int zhongxindian,int *jifen)
{
	long fuduHe=0;
	for(int i=0;i<128;i++)
	{
		fuduHe+=Iabs(buf[i],zhongxindian);
	}
 *jifen+=fuduHe;
	return fuduHe/128;
}

void Shuju:: Zhuanhuan(int *iii,u8 *buf)
{
	int x=0;
	for(int i=0;i<11;i++)
	{
		x=iii[i];
		if(x<0)
		{
			x=0-x;
			buf[10*i]='-';
		}else
			buf[10*i]=' ';

		for(int j=7;j>0;j--)
		{
			buf[(10*i)+j]=x%10+'0';x/=10;
		}
		for(int j=1;j<7;j++)
		{
			if(buf[(10*i)+j]!='0')
			{
				if(buf[(10*i)]=='-')
				{
					buf[(10*i)]=' ';
					buf[10*i+j-1]='-';
				}
				break;
			}else
				buf[(10*i)+j]=' ';
		}
		buf[10*i+8]='\r';
		buf[10*i+9]='\n';
	}
}
 */
