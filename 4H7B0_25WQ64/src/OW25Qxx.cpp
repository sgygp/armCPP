/*
 * OW25Qxx.cpp
 *
 *  Created on: 2024年4月4日
 *      Author: Administrator
 */

#include "OW25Qxx.h"
#include "string.h"

void OW25Qxx::init()
{
	memset(&hospi1,0,sizeof(hospi1));
	hospi1.Instance = OCTOSPI1;
	hospi1.Init.FifoThreshold = 8;
	hospi1.Init.DualQuad = HAL_OSPI_DUALQUAD_DISABLE;// 禁止双OSPI模式
	hospi1.Init.MemoryType = HAL_OSPI_MEMTYPE_MICRON;//存储器模式
	hospi1.Init.DeviceSize = 23;//8兆字节
	hospi1.Init.ChipSelectHighTime = 1;//片选保持高电平时间
	hospi1.Init.FreeRunningClock = HAL_OSPI_FREERUNCLK_DISABLE;
	hospi1.Init.ClockMode = HAL_OSPI_CLOCK_MODE_3;
	hospi1.Init.WrapSize = HAL_OSPI_WRAP_NOT_SUPPORTED;
	hospi1.Init.ClockPrescaler = 3;///200/(3+1)=50M
	hospi1.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_NONE;
	hospi1.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_DISABLE;
	hospi1.Init.ChipSelectBoundary = 0;
	hospi1.Init.DelayBlockBypass = HAL_OSPI_DELAY_BLOCK_BYPASSED;
	hospi1.Init.MaxTran = 0;
	hospi1.Init.Refresh = 0;
	if (HAL_OSPI_Init(&hospi1) != HAL_OK)
	{
		Error_Handler();
	}

	OSPIM_CfgTypeDef sOspiManagerCfg ;
	memset(&sOspiManagerCfg,0,sizeof(sOspiManagerCfg));

	sOspiManagerCfg.ClkPort = 1;
	sOspiManagerCfg.NCSPort = 1;
	sOspiManagerCfg.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;
	sOspiManagerCfg.Req2AckTime=1;
	if (HAL_OSPIM_Config(&hospi1, &sOspiManagerCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
}
void OW25Qxx::EraseSector(uint32_t _uiSectorAddr)
{
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));

	/* 写使能 */
	WriteEnable();

	/* 基本配置 */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* 通用配置 */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   = HAL_OSPI_INSTRUCTION_1_LINE;    			/* 1线方式发送指令 */
	sCommand.AddressSize       = HAL_OSPI_ADDRESS_24_BITS;       			/* 32位地址 */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* 禁止地址DTR模式 */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* 无交替字节 */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* 禁止数据DTR模式 */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* 不使用DQS */
	sCommand.SIOOMode          = HAL_OSPI_SIOO_INST_EVERY_CMD;				/* 每次传输都发指令 */

	/* 擦除配置 */
	sCommand.Instruction = SUBSECTOR_ERASE_4_BYTE_ADDR_CMD;   		/* 32bit地址方式的扇区擦除命令，扇区大小4KB*/
	sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;  							/* 地址发送是1线方式 */
	sCommand.Address     = _uiSectorAddr;        									/* 扇区首地址，保证是4KB整数倍 */
	sCommand.DataMode    = HAL_OSPI_DATA_NONE;       							/* 无需发送数据 */
	sCommand.DummyCycles = 0;                    									/* 无需空周期 */

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	AutoPollingMemReady(HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
}

/*
*********************************************************************************************************
*	函 数 名: OSPI_EraseChip
*	功能说明: 整个芯片擦除
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void OW25Qxx::EraseChip(void)
{
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));

	/* 写使能 */
	WriteEnable( );

	/* 基本配置 */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* 通用配置 */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1线方式发送指令 */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* 指令长度8位 */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* 禁止指令DTR模式	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32位地址 */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* 禁止地址DTR模式 */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* 无交替字节 */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* 禁止数据DTR模式 */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* 不使用DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* 每次传输都发指令 */

	/* 擦除配置 */
	sCommand.Instruction = BULK_ERASE_CMD;       						/* 整个芯片擦除命令*/
	sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;  				/* 地址发送是1线方式 */
	sCommand.Address     = 0;                    						/* 地址 */
	sCommand.DataMode    = HAL_OSPI_DATA_NONE;       				/* 无需发送数据 */
	sCommand.DummyCycles = 0;                    						/* 无需空周期 */

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	AutoPollingMemReady(30000 );
}

/*
*********************************************************************************************************
*	函 数 名: OSPI_WriteBuffer
*	功能说明: 页编程，页大小256字节，任意页都可以写入
*	形    参: _pBuf : 数据源缓冲区；
*			  _uiWriteAddr ：目标区域首地址，即页首地址，比如0， 256, 512等。
*			  _usWriteSize ：数据个数，不能超过页面大小，范围1 - 256。
*	返 回 值: 1:成功， 0：失败
*********************************************************************************************************
*/
uint8_t OW25Qxx::WriteBuffer(uint8_t *_pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize)
{
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));

	/* 写使能 */
	WriteEnable( );

	/* 基本配置 */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* 通用配置 */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1线方式发送指令 */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* 指令长度8位 */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* 禁止指令DTR模式	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32位地址 */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* 禁止地址DTR模式 */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* 无交替字节 */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* 禁止数据DTR模式 */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* 不使用DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	/* 仅发送一次命令 */

	/* 写序列配置 */
	sCommand.Instruction = QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD; 				/* 32bit地址的4线快速写入命令 */
	sCommand.DummyCycles = 0;                    											/* 不需要空周期 */
	sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;  									/* 4线地址方式 */
	sCommand.DataMode    = HAL_OSPI_DATA_4_LINES;    									/* 4线数据方式 */
	sCommand.NbData      = _usWriteSize;         											/* 写数据大小 */
	sCommand.Address     = _uiWriteAddr;         											/* 写入地址 */

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
        //return 0;
		Error_Handler();
	}

	/* 启动传输 */
	if (HAL_OSPI_Transmit(&hospi1, _pBuf, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
        //return 0;
		Error_Handler();

	}

	AutoPollingMemReady( HAL_OSPI_TIMEOUT_DEFAULT_VALUE);

	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: OSPI_ReadBuffer
*	功能说明: 连续读取若干字节，字节个数不能超出芯片容量。
*	形    参: _pBuf : 数据源缓冲区。
*			  _uiReadAddr ：起始地址。
*			  _usSize ：数据个数, 可以大于PAGE_SIZE, 但是不能超出芯片总容量。
*	返 回 值: 无
*********************************************************************************************************
*/
void OW25Qxx::ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
{

	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));


	/* 基本配置 */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* 通用配置 */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1线方式发送指令 */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* 指令长度8位 */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* 禁止指令DTR模式	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32位地址 */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* 禁止地址DTR模式 */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* 无交替字节 */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* 禁止数据DTR模式 */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* 不使用DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* 每次传输都发指令 */

	/* 读取数据 */
	sCommand.Instruction = QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD; 			/* 32bit地址的4线快速读取命令 */
	sCommand.DummyCycles = 6;                    											/* 空周期 */
	sCommand.AddressMode = HAL_OSPI_ADDRESS_4_LINES; 									/* 4线地址 */
	sCommand.DataMode    = HAL_OSPI_DATA_4_LINES;   								  /* 4线数据 */
	sCommand.NbData      = _uiSize;              											/* 读取的数据大小 */
	sCommand.Address     = _uiReadAddr;          											/* 读取数据的起始地址 */

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	/* 读取 */
	if (HAL_OSPI_Receive(&hospi1, _pBuf, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

//	AutoPollingMemReady();
}

/*
*********************************************************************************************************
*	函 数 名: WriteEnable
*	功能说明: 写使能
*	形    参: hOSPI  OSPI_HandleTypeDef句柄。
*	返 回 值: 无
*********************************************************************************************************
*/
void OW25Qxx::WriteEnable(void)
{
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));

	/* 基本配置 */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* 通用配置 */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1线方式发送指令 */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* 指令长度8位 */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* 禁止指令DTR模式	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32位地址 */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* 禁止地址DTR模式 */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* 无交替字节 */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* 禁止数据DTR模式 */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* 不使用DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* 每次传输都发指令 */

	/* 写使能 */
	sCommand.Instruction       = WRITE_ENABLE_CMD;  									/* 写使能指令 */
	sCommand.AddressMode       = HAL_OSPI_ADDRESS_NONE; 							/* 无需地址 */
	sCommand.DataMode          = HAL_OSPI_DATA_NONE;    							/* 无需数据 */
	sCommand.DummyCycles       = 0;                 									/* 空周期  */

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
}

/*
*********************************************************************************************************
*	函 数 名: AutoPollingMemReady
*	功能说明: 等待OSPI Flash就绪，主要用于Flash擦除和页编程时使用
*	形    参: hOSPI  OSPI_HandleTypeDef句柄
*	返 回 值: 无
*********************************************************************************************************
*/
void OW25Qxx::AutoPollingMemReady(u32 timeout_ms)
{
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));
	OSPI_AutoPollingTypeDef sConfig ;
	memset(&sConfig,0,sizeof(sConfig));


	/* 基本配置 */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* 通用配置 */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1线方式发送指令 */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* 指令长度8位 */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* 禁止指令DTR模式	 */
	sCommand.Address            = 0x0;                                // 地址0
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32位地址 */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* 禁止地址DTR模式 */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* 无交替字节 */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* 禁止数据DTR模式 */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* 不使用DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* 每次传输都发指令 */

	/* 读取状态*/
	sCommand.Instruction       = READ_STATUS_REG_CMD; 							/* 读取状态命令 */
	sCommand.AddressMode       = HAL_OSPI_ADDRESS_NONE;   					/* 无需地址 */
	sCommand.DataMode          = HAL_OSPI_DATA_1_LINE;    					/* 1线数据 */
	sCommand.DummyCycles       = 0;                   							/* 无需空周期 */
	sCommand.NbData             = 1;

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	/* 屏蔽位设置的bit0，匹配位等待bit0为0，即不断查询状态寄存器bit0，等待其为0 */
	sConfig.Mask            = 0x01;						//	匹配值
	sConfig.Match           = 0x00;
	sConfig.MatchMode       = HAL_OSPI_MATCH_MODE_AND; //	与运算
	sConfig.Interval        = 0x10;						//	轮询间隔
	sConfig.AutomaticStop   = HAL_OSPI_AUTOMATIC_STOP_ENABLE;	// 自动停止模式

	if (HAL_OSPI_AutoPolling(&hospi1, &sConfig, timeout_ms) != HAL_OK)
	{
		Error_Handler();
	}
}

/*
*********************************************************************************************************
*    函 数 名: OSPI_MemoryMapped
*    功能说明: OSPI内存映射，地址 0x90000000
*    形    参: 无
*    返 回 值: 无
*********************************************************************************************************
*/
void OW25Qxx::MemoryMapped(void)
{
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));
	OSPI_MemoryMappedTypeDef sMemMappedCfg;
	memset(&sMemMappedCfg,0,sizeof(sMemMappedCfg));

	/* 基本配置 */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* 通用配置 */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1线方式发送指令 */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* 指令长度8位 */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* 禁止指令DTR模式	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32位地址 */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* 禁止地址DTR模式 */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* 无交替字节 */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* 禁止数据DTR模式 */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* 不使用DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* 每次传输都发指令 */

	/* 全部采用4线 */
	sCommand.Instruction = QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD; 			/* 快速读取命令 */
	sCommand.AddressMode = HAL_OSPI_ADDRESS_4_LINES;                	/* 4个地址线 */
	sCommand.DataMode = HAL_OSPI_DATA_4_LINES;                   			/* 4个数据线 */
	sCommand.DummyCycles = 6;                                   			/* 空周期 */
	sCommand.Address	=0;
	sCommand.NbData    = 1;

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		 Error_Handler();
	}

	/* 关闭溢出计数 */
	sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;	// 禁用超时计数器, nCS
	sMemMappedCfg.TimeOutPeriod = 0;																		// 超时判断周期

	if (HAL_OSPI_MemoryMapped(&hospi1, &sMemMappedCfg) != HAL_OK)
	{
		 Error_Handler();
	}


//	OSPI_HyperbusCmdTypeDef a;
//	a.AddressSpace=HAL_OSPI_MEMORY_ADDRESS_SPACE;
//	a.AddressSize=HAL_OSPI_ADDRESS_32_BITS;
//	a.Address=0;
//	a.NbData=1;
//	a.DQSMode=HAL_OSPI_DQS_DISABLE;
//	if (HAL_OSPI_HyperbusCmd(&hospi1, &a, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//		 Error_Handler();

}

/*
*********************************************************************************************************
*	函 数 名: sf_ReadID
*	功能说明: 读取器件ID
*	形    参: 无
*	返 回 值: 32bit的器件ID (最高8bit填0，有效ID位数为24bit）
*********************************************************************************************************
*/
uint32_t OW25Qxx::ReadID(void)
{
	uint32_t uiID;
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));
	uint8_t buf[3];

	/* 基本配置 */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* 通用配置 */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1线方式发送指令 */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* 指令长度8位 */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* 禁止指令DTR模式	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32位地址 */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* 禁止地址DTR模式 */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* 无交替字节 */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* 禁止数据DTR模式 */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* 不使用DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* 每次传输都发指令 */

	/* 读取JEDEC ID */
	sCommand.Instruction 				= READ_ID_CMD2;         		/* 读取ID命令 */
	sCommand.AddressMode 				= HAL_OSPI_ADDRESS_NONE;    /* 1线地址 */
	sCommand.DataMode 					= HAL_OSPI_DATA_1_LINE;     /* 1线地址 */
	sCommand.DummyCycles 				= 0;                    		/* 无空周期 */
	sCommand.NbData 						= 3;                        /* 读取三个数据 */

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
       Error_Handler();
	}

	if (HAL_OSPI_Receive(&hospi1, buf, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
       Error_Handler();
	}

	uiID = (buf[0] << 16) | (buf[1] << 8 ) | buf[2];

	return uiID;
}



extern "C"  void HAL_OSPI_MspInit(OSPI_HandleTypeDef* ospiHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct ;
	memset(&PeriphClkInitStruct,0,sizeof(PeriphClkInitStruct));
	if(ospiHandle->Instance==OCTOSPI1)
	{
		/* USER CODE BEGIN OCTOSPI1_MspInit 0 */

		/* USER CODE END OCTOSPI1_MspInit 0 */

		/** Initializes the peripherals clock
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_OSPI;
		PeriphClkInitStruct.OspiClockSelection = RCC_OSPICLKSOURCE_D1HCLK;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
			Error_Handler();
		}

		/* OCTOSPI1 clock enable */
		__HAL_RCC_OCTOSPIM_CLK_ENABLE();
		__HAL_RCC_OSPI1_CLK_ENABLE();

		__HAL_RCC_GPIOE_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
		/**OCTOSPI1 GPIO Configuration
    PE2     ------> OCTOSPIM_P1_IO2
    PB2     ------> OCTOSPIM_P1_CLK
    PB10     ------> OCTOSPIM_P1_NCS
    PD11     ------> OCTOSPIM_P1_IO0
    PD12     ------> OCTOSPIM_P1_IO1
    PD13     ------> OCTOSPIM_P1_IO3
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF9_OCTOSPIM_P1;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

		/* USER CODE BEGIN OCTOSPI1_MspInit 1 */

		/* USER CODE END OCTOSPI1_MspInit 1 */
	}
}
extern "C" void HAL_OSPI_MspDeInit(OSPI_HandleTypeDef* ospiHandle)
{

  if(ospiHandle->Instance==OCTOSPI1)
  {
  /* USER CODE BEGIN OCTOSPI1_MspDeInit 0 */

  /* USER CODE END OCTOSPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_OCTOSPIM_CLK_DISABLE();
    __HAL_RCC_OSPI1_CLK_DISABLE();

    /**OCTOSPI1 GPIO Configuration
    PE2     ------> OCTOSPIM_P1_IO2
    PB2     ------> OCTOSPIM_P1_CLK
    PB10     ------> OCTOSPIM_P1_NCS
    PD11     ------> OCTOSPIM_P1_IO0
    PD12     ------> OCTOSPIM_P1_IO1
    PD13     ------> OCTOSPIM_P1_IO3
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2|GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13);

  /* USER CODE BEGIN OCTOSPI1_MspDeInit 1 */

  /* USER CODE END OCTOSPI1_MspDeInit 1 */
  }
}
