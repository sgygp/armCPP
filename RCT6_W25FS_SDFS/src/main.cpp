// ----------------------------------------------------------------------------

//#include <stdio.h>
//#include <stdlib.h>
#include "diag/Trace.h"

#include "Timer.h"
#include "BlinkLed.h"
#include "Beep.h"
#include "delay.h"
#include<time.h>
#include <string.h>
#include "ff.h"
#include "diskio.h"
#include "w25qxx.h"
#include "tf_spi.h"

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

FRESULT scan_files(const char *path) {
	FRESULT res;
	FILINFO fno;
	DIR dir;

	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0)
				break;
			if (fno.fname[0] == '.')
				continue;
			if (fno.fattrib & AM_DIR) {
				trace_printf("%s/%s/\n", path, fno.fname);
				res = scan_files(fno.fname);
				if (res != FR_OK)
					break;
			} else {
				trace_printf("%s/%s\n", path, fno.fname);
			}
		}
	}

	return res;
}

int main(int argc, char *argv[]) {

	// Send a greeting to the trace device (skipped on Release).
	trace_puts("Hello ARM World!");

	// At this stage the system clock should have already been configured
	// at high speed.
	trace_printf("System clock: %u Hz\n", SystemCoreClock);
	Beep beep;
	beep.init();
	beep.turnOn();
	delay_ms(250); //������
	beep.turnOff();

	Timer timer;
	timer.start();

	BlinkLed blinkLed;
	blinkLed.init();

	SPI_DMA_Config();
	spi2_dma_init();	//��ʼ��IO
	for (int i = 0; i < 3; i++)
		SD_Init_Config();

	FATFS fs[2];
	BYTE work[4096]; /* Work area (larger is better for processing time) */
//	DIR dirs;
//	FILINFO finfo;
//	BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
	FRESULT res;
	FIL fil;
	unsigned int bw;

	res = f_mount(&fs[0], "0:", 0); //����������
	trace_printf("f_mount0=%u\n", res);
	res = f_mount(&fs[1], "1:", 0); //����������
	trace_printf("f_mount1=%u\n", res);
	f_chdrive("1:");

	/* ΪĬ���������ṩ�������� */
	//f_mkfs() Ӧ�� f_mount()֮�󣬷�����֣��޹������Ĵ�λ�����������׷����Ǻǡ�
	/* ��Ĭ�ϲ�����ʽ��Ĭ��������
	 res = f_mkfs("", 0,work , sizeof work);*/
	//	res = f_mkfs("", 0, work, sizeof work);
	/* ��ָ��������ʽ��Ĭ��������
	 MKFS_PARM abc;
	 abc.align=8;//���ݶ��뵽����
	 abc.au_size=4096;//�ش�С
	 abc.fmt=FM_FAT;
	 abc.n_fat=2;
	 abc.n_root=512;//��Ŀ¼�ļ�����
	 res = f_mkfs("", &abc,work , sizeof work);
	 trace_printf("f_mkfs=%u\n", res);



	 // Create a file as new
	 res = f_open(&fil, "hello.txt", FA_CREATE_NEW | FA_WRITE);
	 trace_printf("f_open=%u\n", res);
	 res = f_write(&fil, "Hello, World!\r\n", 15, &bw);
	 trace_printf("bw=%u\n", bw);
	 f_close(&fil);
	 trace_printf("f_close=%u\n", bw);

	 res =f_open(&fil,"ygp.txt",FA_CREATE_NEW | FA_WRITE);
	 trace_printf("f_open=%u\n", res);
	 res = f_write(&fil, "17090145098!\r\n", 13, &bw);
	 trace_printf("bw=%u\n", bw);
	 f_close(&fil);*/
	struct FAT32_DBR {
		unsigned char BS_jmpBoot[3];		//��תָ�� offset: 0
		unsigned char BS_OEMName[8];		//ԭʼ������ offset: 3
		unsigned char BPB_BytesPerSec[2];	//ÿ�����ֽ��� offset:11
		unsigned char BPB_SecPerClus[1]; 	//ÿ�������� offset:13
		unsigned char BPB_RsvdSecCnt[2]; 	//����������Ŀ offset:14
		unsigned char BPB_NumFATs[1]; 		//�˾���FAT���� offset:16
		unsigned char BPB_RootEntCnt[2];	//FAT32Ϊ0 offset:17
		unsigned char BPB_TotSec16[2]; 		//FAT32Ϊ0 offset:19
		unsigned char BPB_Media[1];		//�洢���� offset:21
		unsigned char BPB_FATSz16[2];		//FAT32Ϊ 0 offset:22
		unsigned char BPB_SecPerTrk[2];		//�ŵ������� offset:24
		unsigned char BPB_NumHeads[2];		//��ͷ�� offset:26
		unsigned char BPB_HiddSec[4]; 		//FAT��ǰ�������� offset:28
		unsigned char BPB_TotSec32[4];		//�þ��������� offset:32
		unsigned char BPB_FATSz32[4]; 		//һ��FAT�������� offset:36
		unsigned char BPB_ExtFlags[2];		//FAT32���� offset:40
		unsigned char BPB_FSVer[2];		//FAT32���� offset:42
		unsigned char BPB_RootClus[4];		//��Ŀ¼�غ� offset:44
		unsigned char FSInfo[2];		//��������FSINFO������ offset:48
		unsigned char BPB_BkBootSec[2];		//ͨ��Ϊ6 offset:50
		unsigned char BPB_Reserved[12];		//��չ�� offset:52
		unsigned char BS_DrvNum[1];		//offset:64
		unsigned char BS_Reserved1[1];		//offset:65
		unsigned char BS_BootSig[1];		//offset:66
		unsigned char BS_VolID[4];		//offset:67
		unsigned char BS_FilSysType[11];	//offset:71
		unsigned char BS_FilSysType1[8];	//"FAT32 " offset:82
	};
	SD_CardInfo m_CardInfo;
	trace_printf("sectors=%u\n", SD_GetSectorCount());
	if (SD_GetCardInfo(&m_CardInfo) == SD_OK)
		trace_printf("Rongliang=%u%u\n", u32(m_CardInfo.CardCapacity / 1000000),
				u32(m_CardInfo.CardCapacity % 1000000));
	f_chdir("����");
	scan_files(".");

//	res = f_open(&fil, "���ƽ.txt", FA_READ|FA_WRITE|FA_OPEN_APPEND);
//	trace_printf("f_open=%u\n", res);
//	memset(work, 0, sizeof(work));
//	f_read(&fil, work, 0X500, &bw);
//	f_lseek(&fil,fil.fptr);
//	res=f_write(&fil, "17090145098!\r\n", 13, &bw);
//	f_close(&fil);
//	trace_printf("f_close=%s\n", work);

	f_chdrive("0:");
	res = f_open(&fil, "ygp.txt", FA_READ);
	trace_printf("f_open=%u\n", res);
	memset(work, 0, sizeof(work));
	f_read(&fil, work, 512, &bw);
	f_close(&fil);
	trace_printf("f_close=%s\n", work);

	f_mount(NULL, "0", 0); //NULLΪж��������

	while (1) {
		//__WFI();
		if (Timer::m_bMS) {
			Timer::m_bMS = false;

			if (Timer::ms_jishu % 3000 < 20) //��˸ָʾ��
				blinkLed.turnOn();
			else
				blinkLed.turnOff();

			if (Timer::ms_jishu % 1000 == 0) {
//				u32 u=SPI_FLASH_readdeviceID();
//				trace_printf("readdeviceID=%u\n",u);
//				u=SPI_FLASH_madeID();
//				trace_printf("madeID=%u\n",u);
			}
		}

	}
	// Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
