#include "main.h"
#include "gpio.h"

#include "string.h"
#include "../KissFFT/kiss_fft.h"
#include "LEDrgb.h"
#include "stm32h7xx_it.h"
#include "spi.h"
#include "tftlcd.h"
#include "tft_font.h"

#define nFFT 32768

/*
static kiss_fft_cpx cx_in[nFFT];
static kiss_fft_cpx cx_out[nFFT];
static kiss_fft_cpx st_tmp[nFFT+34];

static uint8_t mpudata[1280] __attribute__((section(".data_CCMRAM")));
 */

int main(void)
{
	MPU_Config();
	HAL_Init();//初始化定时器
	SystemClock_Config();//配置时钟并重新初始化定时器
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



	MX_GPIO_Init();
	MX_SPI6_Init();

/*
	LCD_Init();
	LCD_Clear(BLACK); 		//清屏
	POINT_COLOR=YELLOW;
	BACK_COLOR=BLACK;
	LCD_Show_Image(56,10,128,88,(const uint8_t *)JNTG_BMP);

	LCD_ShowString(0, 0, 160, 12, 12, (char *)"Boring_TECH");
//	LCD_ShowString(0, 16, 160, 16, 16, (char *)"TFTLCD TEST 240*135");
//
//	LCD_ShowString(0, 32, 160, 16, 16, (char *)"STM32H7B0");
//	LCD_ShowString(0, 48, 160, 24, 24, (char *)"2021/8/20");
//
//	LCD_ShowxNum(100,32,1234,6,24,0);//

	LEDrgb m_led;
	m_led.init();
	char str[50] = {0};
*/

	u32 ms=0;
	u32 old=0;
	volatile u32 nTick=0;

	while (1)
	{
		nTick++;
		if(uwTick!=old)//uwTick
		{
			old=uwTick;
			ms++;
//			if(ms%20==0)
//			{
//				m_led.run();

//				sprintf(str,"%4dABCD",(int)ms/20);
//				LCD_ShowString(0, 100, 240, 32, 32,(char *) str);
//			}
		if(ms%1000==0)
			nTick=0;
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
	RCC_OscInitStruct.PLL.PLLN = 112;
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
	 */
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

	HAL_MPU_ConfigRegion(&MPU_InitStruct);
	/* Enables the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

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

extern "C"  void Error_Handler(void)
{
	__disable_irq();
	while (1)
	{
	}
}
