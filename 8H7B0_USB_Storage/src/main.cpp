#include "usb_device.h"
#include "main.h"

#include "string.h"
#include "../KissFFT/kiss_fft.h"
#include "LEDrgb.h"
//#include "Tft114.h"
//#include <H7ADC.h>
//#include "H7UART.h"
//#include "W25Qxx.h"
#include "OW25Qxx.h"
#include "H7Power.h"
#include "SDmmc.h"
#include "ff.h"
#include "diskio.h"
#include "Week.h"


/*
#define nFFT 32768
static kiss_fft_cpx cx_in[nFFT];
static kiss_fft_cpx cx_out[nFFT];
static kiss_fft_cpx st_tmp[nFFT+34];
*/

//static uint8_t work[4096] __attribute__((section(".data_CCMRAM")));
static uint8_t work[4096];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void TestFATFS()
{

	FATFS fs[2];
//	DIR dirs;
//	FILINFO finfo;
//	BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
	FRESULT res;
	FIL fil;
	memset(&fil,0,sizeof(fil));
	unsigned int bw;

	res = f_mount(&fs[0], "0:", 0); //加载驱动器
	trace_printf("f_mount0=%u\n", res);
	res = f_mount(&fs[1], "1:", 0); //加载驱动器
	trace_printf("f_mount1=%u\n", res);
	f_chdrive("1:");
	u32 fre_clust;
	FATFS *fs1;
	res = f_getfree("/", &fre_clust, &fs1);

	DIR dir;
	res=f_opendir(&dir,"DWIN_SET");
	FILINFO ff;
	while(1)
	{
		res=f_readdir(&dir,&ff);
		if(res||ff.fname[0]==0)
			break;
		trace_puts(ff.fname);
	}


	/* 为默认驱动器提供工作区域 */
	//f_mkfs() 应在 f_mount()之后，否则出现，无工作区的错位。不经意容易犯错。呵呵。
	/* 用默认参数格式化默认驱动器*/
//	 res = f_mkfs("", 0,work , sizeof work);
	//	res = f_mkfs("", 0, work, sizeof work);
	// 用指定参数格式化默认驱动器
//	 MKFS_PARM abc;
//	 abc.align=8;//数据对齐到扇区
//	 abc.au_size=4096;//簇大小
//	 abc.fmt=FM_FAT;
//	 abc.n_fat=2;
//	 abc.n_root=512;//根目录文件个数
//	 res = f_mkfs("", &abc,work , sizeof work);

//	 trace_printf("f_mkfs=%u\n", res);


	 res =f_open(&fil,"ygp.txt",FA_CREATE_ALWAYS | FA_WRITE);
	 trace_printf("f_open=%u\n", res);
	 res = f_write(&fil, "17090145098!\r\n", 14, &bw);
	 trace_printf("bw=%u\n", bw);
	 f_close(&fil);

	 res =f_open(&fil,"123.txt",FA_CREATE_ALWAYS | FA_WRITE);
	 trace_printf("f_open=%u\n", res);
	 res = f_write(&fil, "123!\r\n", 6, &bw);
	 trace_printf("bw=%u\n", bw);
	 f_close(&fil);
/*
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
*/
//	SD_CardInfo m_CardInfo;
//	trace_printf("sectors=%u\n", SD_GetSectorCount());
//	if (SD_GetCardInfo(&m_CardInfo) == SD_OK)
//		trace_printf("Rongliang=%u%u\n", u32(m_CardInfo.CardCapacity / 1000000),
//				u32(m_CardInfo.CardCapacity % 1000000));
//	f_chdir("音乐");
//	scan_files(".");

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
	trace_printf("file=%s\n", work);

	f_mount(NULL, "0", 0); //NULL为卸载驱动器

}

int main(void)
{
//	long long x=12345671234567;
//	x++;
//	trace_printf("%d,%d\n",x/10000000,x%10000000);
//	trace_printf("%d,%d\n",u32(x/10000000),x%10000000);

	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	MPU_Config();
	HAL_Init();//初始化定时器
	SystemClock_Config();//配置时钟并重新初始化定时器
	PeriphCommonClock_Config();
	LEDrgb m_led;
	m_led.init();
	trace_printf("week=%d\n", Week::getWeek(1975,4,25));



/*测试待机模式下闹钟唤醒*/
	H7Power m_power;
	m_power.init(10);
/*
				if(ms%5000==0)
					HAL_PWR_EnterSTANDBYMode();
*/




	//trace_printf("SystemCLK=%u Hz\n",SystemCoreClock);

/*测试KissFFT
	int len=sizeof(cx_out);
	memset(cx_out,0,len);
	for(int i=0;i<nFFT;i++)
	{
		cx_in[i].r=15 + 10*sinf(2*M_PI*i*100/nFFT) +  5.5*sinf(2*M_PI*i*150/nFFT); //生成实部
		cx_in[i].i=0;//虚部全部为0
	}
	size_t aaa=1000000;
	kiss_fft_cfg cfg = kiss_fft_alloc( nFFT ,0 ,st_tmp,&aaa);//0为正向变换，1为反向变换
	int ms=uwTick;
	kiss_fft( cfg , cx_in , cx_out );
	trace_printf("%u ms\n",uwTick-ms);
 */

/* 测试液晶
	Tft114 m_lcd;
	m_lcd.init();
	m_lcd.LCD_Clear(BLACK);
	m_lcd.POINT_COLOR=YELLOW;
	m_lcd.BACK_COLOR=BLACK;
	m_lcd.LCD_Show_Image(56,0,128,88,(const uint8_t *)JNTG_BMP);
	m_lcd.POINT_COLOR=GREEN;
	m_lcd.LCD_Show_HZ16(52,90,8,JNTG_HZ);
	m_lcd.POINT_COLOR=GBLUE;
	m_lcd.LCD_ShowString(60-16, 108, 200, 135, 12, (char *)"SHANDONG SHOUGUANG JUNENG");
	m_lcd.LCD_ShowString(72-16, 120, 200, 135, 12, (char *)"SPECIAL STEEL CO.,LTD");
*/


/*ADC+串口
	H7ADC m_ad;
	m_ad.init(20000);
	H7UART m_uart;
	m_uart.init();
*/
/*测试普通spi FLAH
	W25Qxx m_25;
	m_25.init();

//	for(int i=0;i<256;i++)
//		buf[i]=i*2;
//	uwTick=0;
//	m_25.W25QXX_Erase_Chip();
//	trace_printf("Erase_Chip=%u ms\n",uwTick);
//	uwTick=0;
//	for(int i=0;i<32768;i++)
//	m_25.W25QXX_Write_Page(buf,i*256,256);
//	trace_printf("Write 1024 Page=%u ms\n",uwTick);
//	memset(buf,0,256);
//	m_25.W25QXX_Read(buf,32767*256,256);
*/

//测试OSPI FLASH
 	OW25Qxx m_o25;
	m_o25.init();

	u8 buf[256];
	int id = m_o25.ReadID();
	sprintf((char *)buf,"检测到串行Flash, ID = %08X \r\n", id);
	trace_puts((char*)buf);
//	m_o25.EraseChip();
/*	for(int i=0;i<256;i++)
		buf[i]=i;
	uwTick=0;
	m_o25.EraseChip();
//	sprintf((char *)buf,"擦除8M字节数据，用时%5dms! \r\n", uwTick);
//	trace_puts((char*)buf);
//	m_o25.EraseSector(0);
//	m_o25.WriteBuffer(buf,0,256);
//	memset(buf,0,256);
//	m_o25.ReadBuffer(buf,0,256);
	uwTick=0;
	for(int i=0;i<1024*1024*8/256;i++)
		m_o25.WriteBuffer(buf,i*256,256);
//	sprintf((char *)buf,"写入8M字节数据，用时%5dms! \r\n", uwTick);
//	trace_puts((char*)buf);

//	uwTick=0;	for(int i=0;i<1024*1024*8/4096;i++)
//	m_o25.ReadBuffer(buf,4096*i,4096);
//	sprintf((char *)buf,"读取8M字节数据，用时%5dms! \r\n", uwTick);
//	trace_puts((char*)buf);
	memset(buf,0,sizeof(buf));
	m_o25.ReadBuffer(buf,0,4096);

	m_o25.MemoryMapped();
	memset(buf,0,256);
	uint8_t *temp = (uint8_t*)OCTOSPI1_BASE;
	memcpy(buf,temp+1024*1024+128,4096);
*/
/*
*/	SDmmc m_sd;
	if(m_sd.init()!=HAL_OK)
		;
//	TestFATFS();



		 MX_USB_DEVICE_Init();


	u32 ms=0;
	u32 old=0;
	uwTick=0;
	volatile u32 nTick=0,xx=0;

	while (1)
	{
		nTick++;
		//m_ad.run(ms);
		if(uwTick!=old)//uwTick
		{
			old=uwTick;
			ms++;
			//m_uart.run(ms);
			if(ms%1000==0)
			{
				m_power.getRTCtime();
				m_led.run();
				nTick=0;

				if(ms==3000)
					TestFATFS();
			}
		}
	}
}


/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	memset(&RCC_OscInitStruct,0,sizeof(RCC_OscInitStruct));

	RCC_ClkInitTypeDef RCC_ClkInitStruct ;
	memset(&RCC_ClkInitStruct,0,sizeof(RCC_ClkInitStruct));

	/** Supply configuration update enable
	 */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

	  /*AXI clock gating */
	  RCC->CKGAENR = 0xFFFFFFFF;


	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
	  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	  RCC_OscInitStruct.PLL.PLLM = 5;
	  RCC_OscInitStruct.PLL.PLLN = 112;
	  RCC_OscInitStruct.PLL.PLLP = 2;
	  RCC_OscInitStruct.PLL.PLLQ = 12;
	  RCC_OscInitStruct.PLL.PLLR = 2;
	  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
	  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	  RCC_OscInitStruct.PLL.PLLFRACN = 0;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Initializes the CPU, AHB and APB buses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
	                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
	  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct ;
	memset(&MPU_InitStruct,0,sizeof(MPU_InitStruct));

	/* Disables the MPU */
	HAL_MPU_Disable();

	/** Initializes and configures the Region and the memory to be protected

	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress = 0x0;
	MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
	MPU_InitStruct.SubRegionDisable = 0x87;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);*/

    /* 配置AXI SRAM的MPU属性为Write back, Read allocate，Write allocate */
    MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
//    MPU_InitStruct.BaseAddress      = 0x24000000;
//    MPU_InitStruct.Size             = MPU_REGION_SIZE_1MB;
//    MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.BaseAddress = 0x0;
	MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
	MPU_InitStruct.SubRegionDisable = 0x87;

    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
     MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);


	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}
void PeriphCommonClock_Config(void)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct ;
	memset(&PeriphClkInitStruct,0,sizeof(PeriphClkInitStruct));

	/** Initializes the peripherals clock
	 */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CKPER;
	PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
}
void Error_Handler(void)
{
	__disable_irq();
	while (1)
	{
	}
}


#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
	//trace_printf("Wrong parameters value: file %s on line %d\r\n", file, line) ;
}
#endif /* USE_FULL_ASSERT */
