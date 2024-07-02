#include "main.h"

#include "string.h"
#include "../KissFFT/kiss_fft.h"
#include "LEDrgb.h"
#include "Tft114.h"
#include <H7ADC.h>
#include "H7UART.h"
#include "W25Qxx.h"
#include "OW25Qxx.h"

#define nFFT 32768

/*
static kiss_fft_cpx cx_in[nFFT];
static kiss_fft_cpx cx_out[nFFT];
static kiss_fft_cpx st_tmp[nFFT+34];
*/
static uint8_t buf[4096] __attribute__((section(".data_CCMRAM")));

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(void)
{
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
	//	trace_printf("SystemCLK=%u Hz\n",SystemCoreClock);

	/*

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
	/*
//	LCD_ShowString(0, 16, 160, 16, 16, (char *)"TFTLCD TEST 240*135");
//
//	LCD_ShowString(0, 32, 160, 16, 16, (char *)"STM32H7B0");
//	LCD_ShowString(0, 48, 160, 24, 24, (char *)"2021/8/20");
//
//	LCD_ShowxNum(100,32,1234,6,24,0);//
	char str[50] = {0};
	 */
	LEDrgb m_led;
	m_led.init();
	H7ADC m_ad;
	m_ad.init(20000);

	H7UART m_uart;
	m_uart.init();
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
//
//	memset(buf,0,256);
//	m_25.W25QXX_Read(buf,32767*256,256);



	OW25Qxx m_o25;
	m_o25.init();


	int id = m_o25.ReadID();
	sprintf((char *)buf,"检测到串行Flash, ID = %08X \r\n", id);
	trace_puts((char*)buf);
/*

	for(int i=0;i<256;i++)
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



//
//	uwTick=0;	for(int i=0;i<1024*1024*8/4096;i++)
//	m_o25.ReadBuffer(buf,4096*i,4096);
//	sprintf((char *)buf,"读取8M字节数据，用时%5dms! \r\n", uwTick);
//	trace_puts((char*)buf);
*/
	memset(buf,0,sizeof(buf));
	m_o25.ReadBuffer(buf,0,4096);

	m_o25.MemoryMapped();
	memset(buf,0,256);
	uint8_t *temp = (uint8_t*)OCTOSPI1_BASE;
	memcpy(buf,temp+1024*1024+128,4096);


	u32 ms=0;
	u32 old=0;
	uwTick=0;
	volatile u32 nTick=0;
	while (1)
	{
		nTick++;
		m_ad.run(ms);
		if(uwTick!=old)//uwTick
		{
			old=uwTick;
			ms++;
			m_uart.run(ms);
			if(ms%1000==0)
			{
				m_led.run();
				nTick=0;
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

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 5;
	//	RCC_OscInitStruct.PLL.PLLN = 112;
	//	RCC_OscInitStruct.PLL.PLLP = 2;
	//	RCC_OscInitStruct.PLL.PLLQ = 2;
	//	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLN = 160;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
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

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
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
    MPU_InitStruct.BaseAddress      = 0x24000000;
    MPU_InitStruct.Size             = MPU_REGION_SIZE_1MB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
    MPU_InitStruct.SubRegionDisable = 0x00;
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
	trace_printf("Wrong parameters value: file %s on line %d\r\n", file, line) ;
}
#endif /* USE_FULL_ASSERT */
