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
	delay_ms(250); //开机响
	beep.turnOff();

	Timer timer;
	timer.start();

	BlinkLed blinkLed;
	blinkLed.init();

	SPI_DMA_Config();
	spi2_dma_init();	//初始化IO
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

	res = f_mount(&fs[0], "0:", 0); //加载驱动器
	trace_printf("f_mount0=%u\n", res);
	res = f_mount(&fs[1], "1:", 0); //加载驱动器
	trace_printf("f_mount1=%u\n", res);
	f_chdrive("1:");

	/* 为默认驱动器提供工作区域 */
	//f_mkfs() 应在 f_mount()之后，否则出现，无工作区的错位。不经意容易犯错。呵呵。
	/* 用默认参数格式化默认驱动器
	 res = f_mkfs("", 0,work , sizeof work);*/
	//	res = f_mkfs("", 0, work, sizeof work);
	/* 用指定参数格式化默认驱动器
	 MKFS_PARM abc;
	 abc.align=8;//数据对齐到扇区
	 abc.au_size=4096;//簇大小
	 abc.fmt=FM_FAT;
	 abc.n_fat=2;
	 abc.n_root=512;//根目录文件个数
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
		unsigned char BS_jmpBoot[3];		//跳转指令 offset: 0
		unsigned char BS_OEMName[8];		//原始制造商 offset: 3
		unsigned char BPB_BytesPerSec[2];	//每扇区字节数 offset:11
		unsigned char BPB_SecPerClus[1]; 	//每簇扇区数 offset:13
		unsigned char BPB_RsvdSecCnt[2]; 	//保留扇区数目 offset:14
		unsigned char BPB_NumFATs[1]; 		//此卷中FAT表数 offset:16
		unsigned char BPB_RootEntCnt[2];	//FAT32为0 offset:17
		unsigned char BPB_TotSec16[2]; 		//FAT32为0 offset:19
		unsigned char BPB_Media[1];		//存储介质 offset:21
		unsigned char BPB_FATSz16[2];		//FAT32为 0 offset:22
		unsigned char BPB_SecPerTrk[2];		//磁道扇区数 offset:24
		unsigned char BPB_NumHeads[2];		//磁头数 offset:26
		unsigned char BPB_HiddSec[4]; 		//FAT区前隐扇区数 offset:28
		unsigned char BPB_TotSec32[4];		//该卷总扇区数 offset:32
		unsigned char BPB_FATSz32[4]; 		//一个FAT表扇区数 offset:36
		unsigned char BPB_ExtFlags[2];		//FAT32特有 offset:40
		unsigned char BPB_FSVer[2];		//FAT32特有 offset:42
		unsigned char BPB_RootClus[4];		//根目录簇号 offset:44
		unsigned char FSInfo[2];		//保留扇区FSINFO扇区数 offset:48
		unsigned char BPB_BkBootSec[2];		//通常为6 offset:50
		unsigned char BPB_Reserved[12];		//扩展用 offset:52
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
	f_chdir("音乐");
	scan_files(".");

//	res = f_open(&fil, "杨国平.txt", FA_READ|FA_WRITE|FA_OPEN_APPEND);
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

	f_mount(NULL, "0", 0); //NULL为卸载驱动器

	while (1) {
		//__WFI();
		if (Timer::m_bMS) {
			Timer::m_bMS = false;

			if (Timer::ms_jishu % 3000 < 20) //闪烁指示灯
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
