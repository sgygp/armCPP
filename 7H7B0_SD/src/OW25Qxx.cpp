/*
 * OW25Qxx.cpp
 *
 *  Created on: 2024��4��4��
 *      Author: Administrator
 */

#include "OW25Qxx.h"
#include "string.h"
OSPI_HandleTypeDef OW25Qxx::hospi1;
u32 OW25Q_Sector_Count;

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
	hospi1.Init.ClockPrescaler = 7;///200/(3+1)=50M
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

	u8  xxx[5];
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

	/* ��ȡJEDEC ID */
	sCommand.Instruction 					= 0X90;         		/* ��ȡID���� */
	sCommand.AddressMode 				= HAL_OSPI_ADDRESS_NONE;    /* 1�ߵ�ַ */
	sCommand.DataMode 					= HAL_OSPI_DATA_1_LINE;     /* 1�ߵ�ַ */
	sCommand.DummyCycles 				= 0;                    		/* �޿����� */
	sCommand.NbData 						= 5;                        /* ��ȡ�������� */

	if (HAL_OSPI_Command(&OW25Qxx::hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_OSPI_Receive(&OW25Qxx::hospi1, xxx, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		Error_Handler();
	}

	//#define W25Q80 		0XEF13
	//#define W25Q16 		0XEF14
	//#define W25Q32 		0XEF15
	//#define W25Q64 		0XEF16
	//#define W25Q128	0XEF17
	//#define W25Q256 	0XEF18

	u32 a=xxx[3];a<<=8;
	u8 b=xxx[4];
	b&=0x0f;;//0X13��Ӧ1M�ֽڣ�0x14:2M,0x15:4M,0x16:8M,0x17:16M,0x18:32M,
	a=(0x100000<<(b-3));//���ֽ�
	a/=512;//����512,תΪ����

	OW25Q_Sector_Count=a;



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
u8 OW25Qxx::ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize)
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
	return  HAL_OSPI_Receive(&hospi1, _pBuf, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) ;
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
//u8 tempStart[4096] __attribute__ ((at(0x38000000)));
//u8 tempEnd[4096] __attribute__ ((at(0x38000000+4096)));

//u8 tempp[4096] __attribute__((section(".EXTMEMB1")));
u8 tempp[4096];

u8 OW25QXX_disk_write1(u8 *buf, u32 sector){//д8������
	u8 a=0;
	for(int i=0;i<16;i++)
	{
		OW25Qxx::WriteBuffer(buf+256*i,sector*512+i*256,256);
	}
	OW25QXX_disk_read(tempp,sector,8);
	OW25Qxx::ReadBuffer(tempp,132*512,512);
	return a;
}
extern "C" u8 OW25QXX_disk_write(u8 *buf, u32 sector, u16 count){
	trace_printf("sector=%d,count=%d\n",sector,count);
	if(!count)
		return 1;
	if(sector+count>OW25Q_Sector_Count)
		return 1;
	int a=sector%8;
	int startXieCount=0;
	if(a)//��ʼ��ַ����4096�ֽڶ���
	{
		OW25Qxx::ReadBuffer(tempp,(sector/8*8)*512,8*512);
		if(count<=(8-a))
			startXieCount=count;
		else
			startXieCount=8-a;
		memcpy(tempp+a*512,buf,startXieCount*512);
		u32 yaocachuAddr=(sector/8)*4096;
		OW25Qxx::EraseSector(yaocachuAddr);
		OW25QXX_disk_write1(tempp,sector/8*8);
		sector=sector/8*8+8;
	}
	////////////////////�м䲿��
	int sectCount=count-startXieCount;
	int i=0;
	while(sectCount>=8)
	{
		OW25Qxx::EraseSector((sector+i*8)*512);
		OW25QXX_disk_write1(buf+(startXieCount+8*i)*512,sector+i*8);
		i++;
		sectCount-=8;
	}
	/////////////��������

	if(sectCount)
	{
		OW25Qxx::ReadBuffer(tempp,(sector+8*i)*512,8*512);
		memcpy(tempp,buf+(startXieCount+8*i)*512,sectCount*512);
		OW25Qxx::EraseSector((sector+i*8)*512);
		OW25QXX_disk_write1(tempp,sector+i*8);//д8������
	}
	return 0;
//	return OW25Qxx::WriteBuffer(buf,sector*512,count*512);
}
extern "C" u8 OW25QXX_disk_read(u8 *buf, u32 sector, u16 count){
	if(!count)
		return 1;
	if(sector+count>OW25Q_Sector_Count)
		return 1;
	return OW25Qxx::ReadBuffer(buf,sector*512,count*512);
}
