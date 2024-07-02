/*
 * SDmmc.h
 *
 *  Created on: 2024Äê4ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef SDMMC_H_
#define SDMMC_H_
#include "main.h"
#define BSP_SD_CardInfo HAL_SD_CardInfoTypeDef

/* Exported constants --------------------------------------------------------*/
/**
  * @brief  SD status structure definition
  */
#define   MSD_OK                        ((uint8_t)0x00)
#define   MSD_ERROR                     ((uint8_t)0x01)
#define   MSD_ERROR_SD_NOT_PRESENT      ((uint8_t)0x02)

/**
  * @brief  SD transfer state definition
  */
#define   SD_TRANSFER_OK                ((uint8_t)0x00)
#define   SD_TRANSFER_BUSY              ((uint8_t)0x01)

#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)
#define SD_DATATIMEOUT           ((uint32_t)100000000)

/* USER CODE BEGIN BSP_H_CODE */
#define SD_DetectIRQHandler()             HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8)

extern "C" uint8_t SD_ReadDisk(u8 *buf,u32 addr,u16 count);
extern "C" uint8_t SD_WriteDisk(u8 *buf,u32 addr,u16 count);
class SDmmc {
public:
	static SD_HandleTypeDef hsd1;
	u8 init(void);
	void test(void);
};

#endif /* SDMMC_H_ */
