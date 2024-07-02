/*
 * W25X16.cpp
 *
 *  Created on: 2021��3��4��
 *      Author: Administrator
 */
//MOSI MISO��һ��
#include "W25X16.h"
#include "SPI2C.h"
#include "delay.h"
#include "string.h"
#include "RtcBkp.h"
#include "math.h"
#include "shuju.h"
#include "malloc.h"
#include "kiss_fft.h"

kiss_fft_cpx cx_in[nFFT];
kiss_fft_cpx cx_out[nFFT];
kiss_fft_cpx st_tmp[nFFT+34];

#define SUDUGAOPIN 1//���ô���ʹ���ٶȸ�Ƶ���ȣ�ע�͵��ü��ٶȸ�Ƶ��ֵ

W25X16::W25X16() {
	// TODO �Զ����ɵĹ��캯�����
	spi1_tx=	(u8*)SPI2_C::tx;
	jiasudu=0;
	sudu=0;

}

W25X16::~W25X16() {
	// TODO �Զ����ɵ������������
}
void W25X16::SPI_Write(u16 dataGeshu, u16 clkGeshu) {
	SPI2_C::SPI_ReadWrite_Byte(dataGeshu+clkGeshu,false);
}
void W25X16::WaitForWriteEnd() {

	for (int i=0; i < 10000000; i++) {                  //��Ƭ�����ȴ��40��
		spi1_tx[0] = W25X_readRegstatus;
		SPI_Write(1, 1);
		if ((spi1_tx[1] & 0x01) != 1)
			break;
		delay_us(10);
	}
}
void W25X16::SPI_FLASH_ChipErase() {
	spi1_tx[0] = W25X_WriteEnable;
	SPI_Write(1, 0);
	//	WaitForWriteEnd();
	spi1_tx[0] = W25X_chipErase;
	SPI_Write(1, 0);
	WaitForWriteEnd();
}

u8 W25X16::SPI_FLASH_readdeviceID() {
	spi1_tx[0] = W25X_DeviceID;
	SPI_Write(1, 4);
	return spi1_tx[4];
}
u32 W25X16::SPI_FLASH_madeID() {
	spi1_tx[0] = W25X_madeID;
	SPI_Write(1, 3);
	u32 temp = (spi1_tx[1] << 16) | (spi1_tx[2] << 8) | spi1_tx[3];
	return temp;
}
void W25X16::SPI_FLASH_SectorErase(u32 SectorAddr) {//4K ��������
	spi1_tx[0] = W25X_WriteEnable;
	SPI_Write(1, 0);
	//	WaitForWriteEnd();
	spi1_tx[0] = W25X_sectorID;
	spi1_tx[1] = SectorAddr >> 16;
	spi1_tx[2] = SectorAddr >> 8;
	spi1_tx[3] = SectorAddr;
	SPI_Write(4, 0);
	WaitForWriteEnd();
}
void W25X16::SPI_FLASH_PageWriteData(u32 SectorAddr, int x) {//256�ֽ�ҳд
	spi1_tx[0] = W25X_WriteEnable;
	SPI_Write(1, 0);
	//	WaitForWriteEnd();
	spi1_tx[0] = W25X_PageProgram;
	spi1_tx[1] = SectorAddr >> 16;
	spi1_tx[2] = SectorAddr >> 8;
	spi1_tx[3] = SectorAddr;
	SPI_Write(4+ x,0);
	WaitForWriteEnd();
}
void W25X16::SPI_FLASH_PageReadData(u32 SectorAddr, int x) {
	spi1_tx[0] = W25X_ReadData;
	spi1_tx[1] = SectorAddr >> 16;
	spi1_tx[2] = SectorAddr >> 8;
	spi1_tx[3] = SectorAddr;
	SPI_Write(4, x);
}
void W25X16::SPI_FLASH_BlockErase(u32 SectorAddr) {//64K ��������
	spi1_tx[0] = W25X_WriteEnable;
	SPI_Write(1, 0);
	//	WaitForWriteEnd();
	spi1_tx[0] = W25X_64KblockID;
	spi1_tx[1] = SectorAddr >> 16;
	spi1_tx[2] = SectorAddr >> 8;
	spi1_tx[3] = SectorAddr;
	SPI_Write(4, 0);
	WaitForWriteEnd();
}

int W25X16::JisuanJiasuduSudu(int weizhi)//���ط����
{
	float *shuju=(float *)cx_in;
	//	float *sudufloat=(float *)cx_out;
	//	float *weiyifloat=(float *)st_tmp;

	float He=0;
	weizhi=STARTADDR+nFFT*2*weizhi;
	for(int i=0;i<nFFT/128;i++)//������
	{
		SPI_FLASH_PageReadData(weizhi+i*256,256);
		//memcpy(&shuju[i*128],spi1_tx+4,256);
		for(int j=0;j<128;j++)
		{
			shuju[i*128+j]=*(short *)(spi1_tx+4+j*2);
			He+=shuju[i*128+j];
		}
	}
	float pingjunJiasudu=He/nFFT;

	float caiyangpinlv=RtcBkp::getBkpData(BKP_DR10);
	if(caiyangpinlv<600||caiyangpinlv>50000)
		caiyangpinlv=20000;

	int k=RtcBkp::getBkpData(BKP_DR11);
	if(k>999||k<=0)
		k=100;
	float xishu=9.8/(YUANMA_1G*k/100.0);//��λm/s2
	for(int i=0;i<nFFT;i++)
	{
		shuju[i]-=pingjunJiasudu;
		shuju[i]*=xishu;
		jiasudu+=fabs(shuju[i]);
	}
	jiasudu/=nFFT;
	//	u16 nnn=64;
	//	sudu=Shuju::jifen(shuju,sudufloat,nnn,caiyangpinlv);
	//	weiyi=Shuju::jifen(sudufloat,weiyifloat,nnn,caiyangpinlv);


	///////////////////////�������ٶ�
	float *fengWeizhi=(float *)st_tmp;
	int zuidaX=Shuju::qiuFeng(shuju,fengWeizhi,nFFT);
	/*	if(zuidaX>1)
	{
		float xishu1=1000/caiyangpinlv;//��λ����/��
		sudu=Shuju::jifen(shuju,sudufloat,fengWeizhi,xishu1);
	}
	else
		sudu=0;

	///////////////////////������λ��
	//	int shangshengX=Shuju::qiuGuoling(sudufloat,fengWeizhi,nFFT);
	int shangshengX=Shuju::qiuFeng(sudufloat,fengWeizhi,nFFT);
	if(shangshengX>1)
	{
		float xishu2=1000.0/caiyangpinlv;//��λum
		weiyi=Shuju::jifen(sudufloat,shuju,fengWeizhi,xishu2);
	}
	else
		weiyi=0;
	 */	return zuidaX-1;

}
void W25X16::WriteBenciSudu() {
	u16 jiasudu_sudu[30]={0};
	float jiasuduHe=0;
	float suduHe=0;
	float weiyiHe=0;
	int cishu=0x10000/nFFT;
	int fengzongshu=0;
	for(int i=0;i<cishu;i++)//�ܹ�64K��
	{
		fengzongshu+=JisuanJiasuduSudu(i);
		jiasuduHe+=jiasudu;
		suduHe+=sudu;
		weiyiHe+=weiyi;
	}
	jiasuduHe/=cishu;
	suduHe/=cishu;
	weiyiHe/=cishu;
	fengzongshu/=cishu;


	jiasudu_sudu[0]=jiasuduHe*1.41421356*10;//��Чֵ���Ը�2
	//	jiasudu_sudu[1]=suduHe*10;
	//	jiasudu_sudu[2]=weiyiHe*1.41421356*2;
	jiasudu_sudu[3]=fengzongshu*RtcBkp::getBkpData(BKP_DR10)/nFFT;
	jiasudu_sudu[4]=RtcBkp::getBkpData(BKP_DR20);//T

	jiasudu_sudu[5]=RtcBkp::getBkpData(BKP_DR10);//F
	jiasudu_sudu[6]=RtcBkp::getBkpData(BKP_DR12);//B
	jiasudu_sudu[7]=RtcBkp::getBkpData(BKP_DR32);//L
	jisuanFFT();
	getJieguoFromFFT(jiasudu_sudu);
	getJieguo2(&jiasudu_sudu[9]);
	getJieguo(&jiasudu_sudu[10]);


	int zongcishu=RtcBkp::getBkpData(BKP_DR2);
	memcpy(spi1_tx+4,jiasudu_sudu,60);
	SPI_FLASH_PageWriteData(zongcishu*256,60);
	//		memset(spi1_tx,0,256);
	//		SPI_FLASH_PageReadData(zongcishu*256,20);
	//		memcpy(shuju,spi1_tx+4,20);

}

void W25X16::readShuju(u16* shuju, int cishu) {
	SPI_FLASH_PageReadData(cishu*256,60);
	memcpy(shuju,spi1_tx+4,60);
}

void W25X16::jisuanFFT() {
	float *shuju=(float *)st_tmp;
	float He=0;
	int weizhi=STARTADDR+0x10000-nFFT;
	for(int i=0;i<nFFT/128;i++)//������
	{
		SPI_FLASH_PageReadData(weizhi+i*256,256);
		//memcpy(&shuju[i*128],spi1_tx+4,256);
		for(int j=0;j<128;j++)
		{
			shuju[i*128+j]=*(short *)(spi1_tx+4+j*2);
			He+=shuju[i*128+j];
		}
	}
	float pingjunJiasudu=He/nFFT;

	float caiyangpinlv=RtcBkp::getBkpData(BKP_DR10);
	if(caiyangpinlv<600||caiyangpinlv>50000)
		caiyangpinlv=20000;

	int k=RtcBkp::getBkpData(BKP_DR11);
	if(k>999||k<=0)
	{
		k=100;
		RtcBkp::setBkpData(BKP_DR11,k);
	}
	float xishu=9.8/(YUANMA_1G*k/100.0);//��λm/s2
	for(int i=0;i<nFFT;i++)
	{
		shuju[i]-=pingjunJiasudu;
		shuju[i]*=xishu;
		cx_in[i].r=shuju[i];
		cx_in[i].i=0;
	}
	memset(cx_out,0,sizeof(cx_out));
	size_t aaa=100000;
	kiss_fft_cfg cfg = kiss_fft_alloc( nFFT ,0 ,st_tmp,&aaa);
	kiss_fft( cfg , cx_in , cx_out );

	for( int i=0;i<nFFT/2;i++)
		cx_in[i].i=sqrtf(cx_out[i].i*cx_out[i].i+cx_out[i].r*cx_out[i].r)*2/nFFT;
}

void W25X16::getJieguo2(u16 *jieguo) {
	float findPINLV=RtcBkp::getBkpData(BKP_DR12);
	int caiyangpinlv=RtcBkp::getBkpData(BKP_DR10);
	float  pinlv=0;
	float he=0;
	float danci=0;

	for( int i=0;i<nFFT/2;i++)
	{
		pinlv=i*1.0*caiyangpinlv/nFFT;
		if(pinlv<findPINLV)
			continue;
		danci=cx_in[i].i;//sqrtf(cx_out[i].i*cx_out[i].i+cx_out[i].r*cx_out[i].r)*2/nFFT;
#ifdef SUDUGAOPIN
		danci/=(2*3.14159265*pinlv);
#endif
		he=he+(danci*danci);
	}
#ifdef SUDUGAOPIN
	he=sqrtf(he)*10*1000.0/1.414;
#else
	he=sqrtf(he)*10;
#endif
	*jieguo=he;//����һλС��


}

void W25X16::getJieguo(u16 *jieguo) {
	float zhi=0;

	for(int j=0;j<8;j++)
	{
		zhi=RtcBkp::getBkpData(BKP_DR35+j*4);
		if(!zhi)
			break;
		float findPINLV=zhi;///10.0;
		findPINLV*=nFFT;
		findPINLV/=RtcBkp::getBkpData(BKP_DR10);

		int l=RtcBkp::getBkpData(BKP_DR32);
		if(l>99||l<=0)
		{
			l=3;
			RtcBkp::setBkpData(BKP_DR32,l);
		}
		int a=findPINLV-l;
		int b=findPINLV+l;
		if(a<1)
			a=1;
		else if(a>nFFT/2)
			a=nFFT/2;
		if(b<1)
			b=1;
		else if(b>nFFT/2)
			b=nFFT/2;
		float max=0;
		float pinlv=0;
		for( int i=a;i<b;i++)
		{
			zhi=cx_in[i].i;
			if(zhi>max)
			{
				max=zhi;
				pinlv=i;
			}

		}
		pinlv=(pinlv*RtcBkp::getBkpData(BKP_DR10))/nFFT;

		jieguo[2*j]=pinlv*10;
		jieguo[2*j+1]=max*1000;
	}

}

void W25X16::getJieguoFromFFT(u16 *suduweiyi) {
	float zongzhi=0;
	float ff;
	float dancizhi;
	float pinlv=0;
	float m_sudu=0;
	float m_weiyi=0;
	float f2=0;
	int caiyangpinlv=RtcBkp::getBkpData(BKP_DR10);
	if(caiyangpinlv<600||caiyangpinlv>50000)
		caiyangpinlv=20000;



	for(int i=0;i<nFFT/2;i++)
	{
		pinlv=i*1.0*caiyangpinlv/nFFT;
		if(pinlv<FFT_START)
			continue;
		else if(pinlv>FFT_END)
			break;
		dancizhi=cx_in[i].i;
		zongzhi+=(dancizhi*dancizhi);
		float aa=2*3.14159265*pinlv;
		ff=dancizhi/aa;
		m_sudu+=ff*ff;

		f2=ff;
		f2/=aa;
		m_weiyi+=(f2*f2);
	}

	//     suduweiyi[0]=sqrtf(zongzhi)*10;
	suduweiyi[1]=sqrtf(m_sudu)*10*1000.0/1.414;
	suduweiyi[2]=sqrtf(m_weiyi)*1000*1000.0*2.0;
}
