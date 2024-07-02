/*
 * OW25Qxx.cpp
 *
 *  Created on: 2024��4��4��
 *      Author: Administrator
 */

#include "OW25Qxx.h"
#include "string.h"

void OW25Qxx::init()
{
	memset(&hospi1,0,sizeof(hospi1));
	hospi1.Instance = OCTOSPI1;
	hospi1.Init.FifoThreshold = 8;
	hospi1.Init.DualQuad = HAL_OSPI_DUALQUAD_DISABLE;// ��ֹ˫OSPIģʽ
	hospi1.Init.MemoryType = HAL_OSPI_MEMTYPE_MICRON;//�洢��ģʽ
	hospi1.Init.DeviceSize = 23;//8���ֽ�
	hospi1.Init.ChipSelectHighTime = 1;//Ƭѡ���ָߵ�ƽʱ��
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

	/* дʹ�� */
	WriteEnable();

	/* �������� */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* ͨ������ */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   = HAL_OSPI_INSTRUCTION_1_LINE;    			/* 1�߷�ʽ����ָ�� */
	sCommand.AddressSize       = HAL_OSPI_ADDRESS_24_BITS;       			/* 32λ��ַ */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* ��ֹ��ַDTRģʽ */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* �޽����ֽ� */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* ��ֹ����DTRģʽ */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* ��ʹ��DQS */
	sCommand.SIOOMode          = HAL_OSPI_SIOO_INST_EVERY_CMD;				/* ÿ�δ��䶼��ָ�� */

	/* �������� */
	sCommand.Instruction = SUBSECTOR_ERASE_4_BYTE_ADDR_CMD;   		/* 32bit��ַ��ʽ�������������������С4KB*/
	sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;  							/* ��ַ������1�߷�ʽ */
	sCommand.Address     = _uiSectorAddr;        									/* �����׵�ַ����֤��4KB������ */
	sCommand.DataMode    = HAL_OSPI_DATA_NONE;       							/* ���跢������ */
	sCommand.DummyCycles = 0;                    									/* ��������� */

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	AutoPollingMemReady(HAL_OSPI_TIMEOUT_DEFAULT_VALUE);
}

/*
*********************************************************************************************************
*	�� �� ��: OSPI_EraseChip
*	����˵��: ����оƬ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OW25Qxx::EraseChip(void)
{
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));

	/* дʹ�� */
	WriteEnable( );

	/* �������� */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* ͨ������ */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1�߷�ʽ����ָ�� */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* ָ���8λ */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* ��ָֹ��DTRģʽ	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32λ��ַ */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* ��ֹ��ַDTRģʽ */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* �޽����ֽ� */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* ��ֹ����DTRģʽ */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* ��ʹ��DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* ÿ�δ��䶼��ָ�� */

	/* �������� */
	sCommand.Instruction = BULK_ERASE_CMD;       						/* ����оƬ��������*/
	sCommand.AddressMode = HAL_OSPI_ADDRESS_NONE;  				/* ��ַ������1�߷�ʽ */
	sCommand.Address     = 0;                    						/* ��ַ */
	sCommand.DataMode    = HAL_OSPI_DATA_NONE;       				/* ���跢������ */
	sCommand.DummyCycles = 0;                    						/* ��������� */

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	AutoPollingMemReady(30000 );
}

/*
*********************************************************************************************************
*	�� �� ��: OSPI_WriteBuffer
*	����˵��: ҳ��̣�ҳ��С256�ֽڣ�����ҳ������д��
*	��    ��: _pBuf : ����Դ��������
*			  _uiWriteAddr ��Ŀ�������׵�ַ����ҳ�׵�ַ������0�� 256, 512�ȡ�
*			  _usWriteSize �����ݸ��������ܳ���ҳ���С����Χ1 - 256��
*	�� �� ֵ: 1:�ɹ��� 0��ʧ��
*********************************************************************************************************
*/
uint8_t OW25Qxx::WriteBuffer(uint8_t *_pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize)
{
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));

	/* дʹ�� */
	WriteEnable( );

	/* �������� */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* ͨ������ */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1�߷�ʽ����ָ�� */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* ָ���8λ */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* ��ָֹ��DTRģʽ	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32λ��ַ */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* ��ֹ��ַDTRģʽ */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* �޽����ֽ� */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* ��ֹ����DTRģʽ */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* ��ʹ��DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	/* ������һ������ */

	/* д�������� */
	sCommand.Instruction = QUAD_IN_FAST_PROG_4_BYTE_ADDR_CMD; 				/* 32bit��ַ��4�߿���д������ */
	sCommand.DummyCycles = 0;                    											/* ����Ҫ������ */
	sCommand.AddressMode = HAL_OSPI_ADDRESS_1_LINE;  									/* 4�ߵ�ַ��ʽ */
	sCommand.DataMode    = HAL_OSPI_DATA_4_LINES;    									/* 4�����ݷ�ʽ */
	sCommand.NbData      = _usWriteSize;         											/* д���ݴ�С */
	sCommand.Address     = _uiWriteAddr;         											/* д���ַ */

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
        //return 0;
		Error_Handler();
	}

	/* �������� */
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
*	�� �� ��: OSPI_ReadBuffer
*	����˵��: ������ȡ�����ֽڣ��ֽڸ������ܳ���оƬ������
*	��    ��: _pBuf : ����Դ��������
*			  _uiReadAddr ����ʼ��ַ��
*			  _usSize �����ݸ���, ���Դ���PAGE_SIZE, ���ǲ��ܳ���оƬ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OW25Qxx::ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
{

	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));


	/* �������� */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* ͨ������ */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1�߷�ʽ����ָ�� */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* ָ���8λ */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* ��ָֹ��DTRģʽ	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32λ��ַ */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* ��ֹ��ַDTRģʽ */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* �޽����ֽ� */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* ��ֹ����DTRģʽ */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* ��ʹ��DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* ÿ�δ��䶼��ָ�� */

	/* ��ȡ���� */
	sCommand.Instruction = QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD; 			/* 32bit��ַ��4�߿��ٶ�ȡ���� */
	sCommand.DummyCycles = 6;                    											/* ������ */
	sCommand.AddressMode = HAL_OSPI_ADDRESS_4_LINES; 									/* 4�ߵ�ַ */
	sCommand.DataMode    = HAL_OSPI_DATA_4_LINES;   								  /* 4������ */
	sCommand.NbData      = _uiSize;              											/* ��ȡ�����ݴ�С */
	sCommand.Address     = _uiReadAddr;          											/* ��ȡ���ݵ���ʼ��ַ */

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	/* ��ȡ */
	if (HAL_OSPI_Receive(&hospi1, _pBuf, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

//	AutoPollingMemReady();
}

/*
*********************************************************************************************************
*	�� �� ��: WriteEnable
*	����˵��: дʹ��
*	��    ��: hOSPI  OSPI_HandleTypeDef�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OW25Qxx::WriteEnable(void)
{
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));

	/* �������� */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* ͨ������ */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1�߷�ʽ����ָ�� */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* ָ���8λ */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* ��ָֹ��DTRģʽ	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32λ��ַ */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* ��ֹ��ַDTRģʽ */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* �޽����ֽ� */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* ��ֹ����DTRģʽ */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* ��ʹ��DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* ÿ�δ��䶼��ָ�� */

	/* дʹ�� */
	sCommand.Instruction       = WRITE_ENABLE_CMD;  									/* дʹ��ָ�� */
	sCommand.AddressMode       = HAL_OSPI_ADDRESS_NONE; 							/* �����ַ */
	sCommand.DataMode          = HAL_OSPI_DATA_NONE;    							/* �������� */
	sCommand.DummyCycles       = 0;                 									/* ������  */

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AutoPollingMemReady
*	����˵��: �ȴ�OSPI Flash��������Ҫ����Flash������ҳ���ʱʹ��
*	��    ��: hOSPI  OSPI_HandleTypeDef���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OW25Qxx::AutoPollingMemReady(u32 timeout_ms)
{
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));
	OSPI_AutoPollingTypeDef sConfig ;
	memset(&sConfig,0,sizeof(sConfig));


	/* �������� */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* ͨ������ */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1�߷�ʽ����ָ�� */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* ָ���8λ */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* ��ָֹ��DTRģʽ	 */
	sCommand.Address            = 0x0;                                // ��ַ0
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32λ��ַ */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* ��ֹ��ַDTRģʽ */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* �޽����ֽ� */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* ��ֹ����DTRģʽ */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* ��ʹ��DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* ÿ�δ��䶼��ָ�� */

	/* ��ȡ״̬*/
	sCommand.Instruction       = READ_STATUS_REG_CMD; 							/* ��ȡ״̬���� */
	sCommand.AddressMode       = HAL_OSPI_ADDRESS_NONE;   					/* �����ַ */
	sCommand.DataMode          = HAL_OSPI_DATA_1_LINE;    					/* 1������ */
	sCommand.DummyCycles       = 0;                   							/* ��������� */
	sCommand.NbData             = 1;

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	/* ����λ���õ�bit0��ƥ��λ�ȴ�bit0Ϊ0�������ϲ�ѯ״̬�Ĵ���bit0���ȴ���Ϊ0 */
	sConfig.Mask            = 0x01;						//	ƥ��ֵ
	sConfig.Match           = 0x00;
	sConfig.MatchMode       = HAL_OSPI_MATCH_MODE_AND; //	������
	sConfig.Interval        = 0x10;						//	��ѯ���
	sConfig.AutomaticStop   = HAL_OSPI_AUTOMATIC_STOP_ENABLE;	// �Զ�ֹͣģʽ

	if (HAL_OSPI_AutoPolling(&hospi1, &sConfig, timeout_ms) != HAL_OK)
	{
		Error_Handler();
	}
}

/*
*********************************************************************************************************
*    �� �� ��: OSPI_MemoryMapped
*    ����˵��: OSPI�ڴ�ӳ�䣬��ַ 0x90000000
*    ��    ��: ��
*    �� �� ֵ: ��
*********************************************************************************************************
*/
void OW25Qxx::MemoryMapped(void)
{
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));
	OSPI_MemoryMappedTypeDef sMemMappedCfg;
	memset(&sMemMappedCfg,0,sizeof(sMemMappedCfg));

	/* �������� */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* ͨ������ */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1�߷�ʽ����ָ�� */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* ָ���8λ */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* ��ָֹ��DTRģʽ	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32λ��ַ */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* ��ֹ��ַDTRģʽ */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* �޽����ֽ� */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* ��ֹ����DTRģʽ */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* ��ʹ��DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* ÿ�δ��䶼��ָ�� */

	/* ȫ������4�� */
	sCommand.Instruction = QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD; 			/* ���ٶ�ȡ���� */
	sCommand.AddressMode = HAL_OSPI_ADDRESS_4_LINES;                	/* 4����ַ�� */
	sCommand.DataMode = HAL_OSPI_DATA_4_LINES;                   			/* 4�������� */
	sCommand.DummyCycles = 6;                                   			/* ������ */
	sCommand.Address	=0;
	sCommand.NbData    = 1;

	if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		 Error_Handler();
	}

	/* �ر�������� */
	sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;	// ���ó�ʱ������, nCS
	sMemMappedCfg.TimeOutPeriod = 0;																		// ��ʱ�ж�����

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
*	�� �� ��: sf_ReadID
*	����˵��: ��ȡ����ID
*	��    ��: ��
*	�� �� ֵ: 32bit������ID (���8bit��0����ЧIDλ��Ϊ24bit��
*********************************************************************************************************
*/
uint32_t OW25Qxx::ReadID(void)
{
	uint32_t uiID;
	OSPI_RegularCmdTypeDef sCommand;
	memset(&sCommand,0,sizeof(sCommand));
	uint8_t buf[3];

	/* �������� */
	sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;         /* ͨ������ */
	sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;                /* flash ID */
	sCommand.InstructionMode   	= HAL_OSPI_INSTRUCTION_1_LINE;    		/* 1�߷�ʽ����ָ�� */
	sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;        /* ָ���8λ */
	sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;   /* ��ָֹ��DTRģʽ	 */
	sCommand.AddressSize       	= HAL_OSPI_ADDRESS_24_BITS;       		/* 32λ��ַ */
	sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;       /* ��ֹ��ַDTRģʽ */
	sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;  		/* �޽����ֽ� */
	sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;          /* ��ֹ����DTRģʽ */
	sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;               /* ��ʹ��DQS */
	sCommand.SIOOMode          	= HAL_OSPI_SIOO_INST_EVERY_CMD;	 			/* ÿ�δ��䶼��ָ�� */

	/* ��ȡJEDEC ID */
	sCommand.Instruction 				= READ_ID_CMD2;         		/* ��ȡID���� */
	sCommand.AddressMode 				= HAL_OSPI_ADDRESS_NONE;    /* 1�ߵ�ַ */
	sCommand.DataMode 					= HAL_OSPI_DATA_1_LINE;     /* 1�ߵ�ַ */
	sCommand.DummyCycles 				= 0;                    		/* �޿����� */
	sCommand.NbData 						= 3;                        /* ��ȡ�������� */

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
