/*
 * SDmmc.cpp
 *
 *  Created on: 2024年4月9日
 *      Author: Administrator
 */

#include "SDmmc.h"
#include "string.h"
u8 SDmmc::bOK=false;
SD_HandleTypeDef SDmmc::hsd1;

u8 SD_CardType;
u32 SD_Sector_Count=0;
HAL_SD_CardCIDTypeDef SDCard_CID;
HAL_SD_CardCSDTypeDef SDCard_CSD;

u8 SDmmc::init(void)
{
	memset(&hsd1,0,sizeof(hsd1));
	hsd1.Instance = SDMMC1;
	hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
	hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
	hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
	hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd1.Init.ClockDiv = 9;

	if (HAL_SD_Init(&hsd1) != HAL_OK)
		return MSD_ERROR_SD_NOT_PRESENT;
	 uint8_t sd_state = MSD_OK;

	if(HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER)
	      sd_state = MSD_ERROR;
	if(!sd_state)
	{
		test();
		bOK=true;
	}
   return sd_state;
}

void SDmmc::test(void) {
	uint64_t CardCap;      	//SD卡容量
	HAL_SD_CardInfoTypeDef  SDCardInfo;
	HAL_SD_GetCardCID(&hsd1,&SDCard_CID);	//获取CID
	HAL_SD_GetCardInfo(&hsd1,&SDCardInfo);                    //获取SD卡信息
	HAL_SD_GetCardCSD(&hsd1,&SDCard_CSD);

	CardCap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.BlockSize);	//计算SD卡容量

	trace_printf("CardCap:%6d%06dByte\r\n",u32(CardCap/1000000u),u32(CardCap%1000000u));
	SD_CardType=SDCardInfo.CardType;
	SD_Sector_Count=SDCardInfo.BlockNbr;

	switch(SD_CardType)
	{
	case CARD_SDSC:
	{
		if(SDCardInfo.CardVersion == CARD_V1_X)
			trace_printf("Card Type:SDSC V1\r\n");
		else if(SDCardInfo.CardVersion == CARD_V2_X)
			trace_printf("Card Type:SDSC V2\r\n");
	}
	break;
	case CARD_SDHC_SDXC:
		trace_printf("Card Type:SDHC\r\n");
		break;
	default:
		break;
	}

//	  if (HAL_SD_WriteBlocks(&hsd1, (u8 *)"1234", 63, 1,1000) != HAL_OK)
//	  {
//		trace_printf("error\r\n");
//	  }
//	  u8 buf[512]={0};
//	 if (HAL_SD_ReadBlocks(&hsd1,buf,63,1,1000) == HAL_OK)
//	 {
//		trace_puts((char *)buf);
//	 }

}

extern "C" void HAL_SD_MspInit(SD_HandleTypeDef* sdHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct ;
	memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct ;
	memset(&PeriphClkInitStruct,0,sizeof(PeriphClkInitStruct));
	if(sdHandle->Instance==SDMMC1)
	{
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC;
		PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
			Error_Handler();
		}

		/* SDMMC1 clock enable */
		__HAL_RCC_SDMMC1_CLK_ENABLE();

		/**SDMMC1 GPIO Configuration
    PC8     ------> SDMMC1_D0
    PC9     ------> SDMMC1_D1
    PC10     ------> SDMMC1_D2
    PC11     ------> SDMMC1_D3
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
				|GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
}

extern "C" void HAL_SD_MspDeInit(SD_HandleTypeDef* sdHandle)
{

	if(sdHandle->Instance==SDMMC1)
	{
		/* Peripheral clock disable */
		__HAL_RCC_SDMMC1_CLK_DISABLE();

		/**SDMMC1 GPIO Configuration
    PC8     ------> SDMMC1_D0
    PC9     ------> SDMMC1_D1
    PC10     ------> SDMMC1_D2
    PC11     ------> SDMMC1_D3
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD
		 */
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
				|GPIO_PIN_12);

		HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

	}
}




/*
__weak uint8_t BSP_SD_ITConfig(void)
{
  return (uint8_t)0;
}
__weak uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
  uint8_t sd_state = MSD_OK;

  if (HAL_SD_ReadBlocks(&SDmmc::hsd1, (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  return sd_state;
}
__weak uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
  uint8_t sd_state = MSD_OK;

  if (HAL_SD_WriteBlocks(&SDmmc::hsd1, (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  return sd_state;
}
__weak uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{
  uint8_t sd_state = MSD_OK;

    if (HAL_SD_ReadBlocks_DMA(&SDmmc::hsd1, (uint8_t *)pData, ReadAddr, NumOfBlocks) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  return sd_state;
}
__weak uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{
  uint8_t sd_state = MSD_OK;

  if (HAL_SD_WriteBlocks_DMA(&SDmmc::hsd1, (uint8_t *)pData, WriteAddr, NumOfBlocks) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  return sd_state;
}
__weak uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
  uint8_t sd_state = MSD_OK;

  if (HAL_SD_Erase(&SDmmc::hsd1, StartAddr, EndAddr) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  return sd_state;
}
__weak uint8_t BSP_SD_GetCardState(void)
{
  return ((HAL_SD_GetCardState(&SDmmc::hsd1) == HAL_SD_CARD_TRANSFER ) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}
__weak void BSP_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo)
{

  HAL_SD_GetCardInfo(&SDmmc::hsd1, CardInfo);
}
__weak void BSP_SD_ReadCpltCallback(void)
{

}
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
  BSP_SD_ReadCpltCallback();
}
__weak void BSP_SD_AbortCallback(void)
{

}
__weak void BSP_SD_WriteCpltCallback(void)
{

}
__weak uint8_t BSP_SD_IsDetected(void)
{
  __IO uint8_t status = SD_PRESENT;
  return status;
}
void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
  BSP_SD_AbortCallback();
}

void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
  BSP_SD_WriteCpltCallback();
}
*/
extern "C" uint8_t SD_ReadDisk(u8 *buf,u32 sector,u16 count)
{
	if(!SDmmc::bOK)
		return -1;
	else
	return HAL_SD_ReadBlocks(&SDmmc::hsd1,buf,sector,count,100000);
}
extern "C" uint8_t SD_WriteDisk(u8 *buf,u32 sector,u16 count)
{
//	trace_printf("sector=%d,count=%d\n",sector,count);
	if(!SDmmc::bOK)
		return -1;
	else
	return HAL_SD_WriteBlocks(&SDmmc::hsd1,buf,sector,count,100000);
}
