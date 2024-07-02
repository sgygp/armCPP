/*
 * tf_spi.h
 *
 *  Created on: 2020年2月27日
 *      Author: Administrator
 */

#ifndef TF_SPI_H_
#define TF_SPI_H_
#include "stm32f10x_conf.h"

/* Exported types ------------------------------------------------------------*/
typedef enum {
	/* SDIO specific error defines */
	SD_CMD_CRC_FAIL = (1), /* Command response received (but CRC check failed) */
	SD_DATA_CRC_FAIL = (2), /* Data bock sent/received (CRC check Failed) */
	SD_CMD_RSP_TIMEOUT = (3), /* Command response timeout */
	SD_DATA_TIMEOUT = (4), /* Data time out */
	SD_TX_UNDERRUN = (5), /* Transmit FIFO under-run */
	SD_RX_OVERRUN = (6), /* Receive FIFO over-run */
	SD_START_BIT_ERR = (7), /* Start bit not detected on all data signals in widE bus mode */
	SD_CMD_OUT_OF_RANGE = (8), /* CMD's argument was out of range.*/
	SD_ADDR_MISALIGNED = (9), /* Misaligned address */
	SD_BLOCK_LEN_ERR = (10), /* Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	SD_ERASE_SEQ_ERR = (11), /* An error in the sequence of erase command occurs.*/
	SD_BAD_ERASE_PARAM = (12), /* An Invalid selection for erase groups */
	SD_WRITE_PROT_VIOLATION = (13), /* Attempt to program a write protect block */
	SD_LOCK_UNLOCK_FAILED = (14), /* Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	SD_COM_CRC_FAILED = (15), /* CRC check of the previous command failed */
	SD_ILLEGAL_CMD = (16), /* Command is not legal for the card state */
	SD_CARD_ECC_FAILED = (17), /* Card internal ECC was applied but failed to correct the data */
	SD_CC_ERROR = (18), /* Internal card controller error */
	SD_GENERAL_UNKNOWN_ERROR = (19), /* General or Unknown error */
	SD_STREAM_READ_UNDERRUN = (20), /* The card could not sustain data transfer in stream read operation. */
	SD_STREAM_WRITE_OVERRUN = (21), /* The card could not sustain data programming in stream mode */
	SD_CID_CSD_OVERWRITE = (22), /* CID/CSD overwrite error */
	SD_WP_ERASE_SKIP = (23), /* only partial address space was erased */
	SD_CARD_ECC_DISABLED = (24), /* Command has been executed without using internal ECC */
	SD_ERASE_RESET = (25), /* Erase sequence was cleared before executing because an out of erase sequence command was received */
	SD_AKE_SEQ_ERROR = (26), /* Error in sequence of authentication. */
	SD_INVALID_VOLTRANGE = (27),
	SD_ADDR_OUT_OF_RANGE = (28),
	SD_SWITCH_ERROR = (29),
	SD_SDIO_DISABLED = (30),
	SD_SDIO_FUNCTION_BUSY = (31),
	SD_SDIO_FUNCTION_FAILED = (32),
	SD_SDIO_UNKNOWN_FUNCTION = (33),

	/* Standard error defines */
	SD_INTERNAL_ERROR,
	SD_NOT_CONFIGURED,
	SD_REQUEST_PENDING,
	SD_REQUEST_NOT_APPLICABLE,
	SD_INVALID_PARAMETER,
	SD_UNSUPPORTED_FEATURE,
	SD_UNSUPPORTED_HW,
	SD_ERROR,
	SD_OK,
} SD_Error;

/* SDIO Commands  Index */
#define SDIO_GO_IDLE_STATE                       ((uint8_t)0)
#define SDIO_SEND_OP_COND                        ((uint8_t)1)
#define SDIO_ALL_SEND_CID                        ((uint8_t)2)
#define SDIO_SET_REL_ADDR                        ((uint8_t)3) /* SDIO_SEND_REL_ADDR for SD Card */
#define SDIO_SET_DSR                             ((uint8_t)4)
#define SDIO_SDIO_SEN_OP_COND                    ((uint8_t)5)
#define SDIO_HS_SWITCH                           ((uint8_t)6)
#define SDIO_SEL_DESEL_CARD                      ((uint8_t)7)
#define SDIO_HS_SEND_EXT_CSD                     ((uint8_t)8)
#define SDIO_SEND_CSD                            ((uint8_t)9)
#define SDIO_SEND_CID                            ((uint8_t)10)
#define SDIO_READ_DAT_UNTIL_STOP                 ((uint8_t)11) /* SD Card doesn't support it */
#define SDIO_STOP_TRANSMISSION                   ((uint8_t)12)
#define SDIO_SEND_STATUS                         ((uint8_t)13)
#define SDIO_HS_BUSTEST_READ                     ((uint8_t)14)
#define SDIO_GO_INACTIVE_STATE                   ((uint8_t)15)
#define SDIO_SET_BLOCKLEN                        ((uint8_t)16)
#define SDIO_READ_SINGLE_BLOCK                   ((uint8_t)17)
#define SDIO_READ_MULT_BLOCK                     ((uint8_t)18)
#define SDIO_HS_BUSTEST_WRITE                    ((uint8_t)19)
#define SDIO_WRITE_DAT_UNTIL_STOP                ((uint8_t)20) /* SD Card doesn't support it */
#define SDIO_SET_BLOCK_COUNT                     ((uint8_t)23) /* SD Card doesn't support it */
#define SDIO_WRITE_SINGLE_BLOCK                  ((uint8_t)24)
#define SDIO_WRITE_MULT_BLOCK                    ((uint8_t)25)
#define SDIO_PROG_CID                            ((uint8_t)26) /* reserved for manufacturers */
#define SDIO_PROG_CSD                            ((uint8_t)27)
#define SDIO_SET_WRITE_PROT                      ((uint8_t)28)
#define SDIO_CLR_WRITE_PROT                      ((uint8_t)29)
#define SDIO_SEND_WRITE_PROT                     ((uint8_t)30)
#define SDIO_SD_ERASE_GRP_START                  ((uint8_t)32) /* To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SDIO_SD_ERASE_GRP_END                    ((uint8_t)33) /* To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SDIO_ERASE_GRP_START                     ((uint8_t)35) /* To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SDIO_ERASE_GRP_END                       ((uint8_t)36) /* To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SDIO_ERASE                               ((uint8_t)38)
#define SDIO_FAST_IO                             ((uint8_t)39) /* SD Card doesn't support it */
#define SDIO_GO_IRQ_STATE                        ((uint8_t)40) /* SD Card doesn't support it */
#define SDIO_LOCK_UNLOCK                         ((uint8_t)42)
#define SDIO_APP_CMD                             ((uint8_t)55)
#define SDIO_GEN_CMD                             ((uint8_t)56)
#define SDIO_NO_CMD                              ((uint8_t)64)

/* Following commands are SD Card Specific commands.
 SDIO_APP_CMD should be sent before sending these
 commands. */
#define SDIO_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)  /* For SD Card only */
#define SDIO_SD_APP_STAUS                        ((uint8_t)13) /* For SD Card only */
#define SDIO_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22) /* For SD Card only */
#define SDIO_SD_APP_OP_COND                      ((uint8_t)41) /* For SD Card only */
#define SDIO_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42) /* For SD Card only */
#define SDIO_SD_APP_SEND_SCR                     ((uint8_t)51) /* For SD Card only */
#define SDIO_SDIO_RW_DIRECT                      ((uint8_t)52) /* For SD I/O Card only */
#define SDIO_SDIO_RW_EXTENDED                    ((uint8_t)53) /* For SD I/O Card only */

/* Following commands are SD Card Specific security commands.
 SDIO_APP_CMD should be sent before sending these commands. */
#define SDIO_SD_APP_GET_MKB                      ((uint8_t)43) /* For SD Card only */
#define SDIO_SD_APP_GET_MID                      ((uint8_t)44) /* For SD Card only */
#define SDIO_SD_APP_SET_CER_RN1                  ((uint8_t)45) /* For SD Card only */
#define SDIO_SD_APP_GET_CER_RN2                  ((uint8_t)46) /* For SD Card only */
#define SDIO_SD_APP_SET_CER_RES2                 ((uint8_t)47) /* For SD Card only */
#define SDIO_SD_APP_GET_CER_RES1                 ((uint8_t)48) /* For SD Card only */
#define SDIO_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18) /* For SD Card only */
#define SDIO_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25) /* For SD Card only */
#define SDIO_SD_APP_SECURE_ERASE                 ((uint8_t)38) /* For SD Card only */
#define SDIO_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49) /* For SD Card only */
#define SDIO_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48) /* For SD Card only */

typedef struct /* Card Specific Data */
{
	__IO uint8_t CSDStruct; /* CSD structure */
	__IO uint8_t SysSpecVersion; /* System specification version */
	__IO uint8_t Reserved1; /* Reserved */
	__IO uint8_t TAAC; /* Data read access-time 1 */
	__IO uint8_t NSAC; /* Data read access-time 2 in CLK cycles */
	__IO uint8_t MaxBusClkFrec; /* Max. bus clock frequency */
	__IO uint16_t CardComdClasses; /* Card command classes */
	__IO uint8_t RdBlockLen; /* Max. read data block length */
	__IO uint8_t PartBlockRead; /* Partial blocks for read allowed */
	__IO uint8_t WrBlockMisalign; /* Write block misalignment */
	__IO uint8_t RdBlockMisalign; /* Read block misalignment */
	__IO uint8_t DSRImpl; /* DSR implemented */
	__IO uint8_t Reserved2; /* Reserved */
	__IO uint32_t DeviceSize; /* Device Size */
	__IO uint8_t MaxRdCurrentVDDMin; /* Max. read current @ VDD min */
	__IO uint8_t MaxRdCurrentVDDMax; /* Max. read current @ VDD max */
	__IO uint8_t MaxWrCurrentVDDMin; /* Max. write current @ VDD min */
	__IO uint8_t MaxWrCurrentVDDMax; /* Max. write current @ VDD max */
	__IO uint8_t DeviceSizeMul; /* Device size multiplier */
	__IO uint8_t EraseGrSize; /* Erase group size */
	__IO uint8_t EraseGrMul; /* Erase group size multiplier */
	__IO uint8_t WrProtectGrSize; /* Write protect group size */
	__IO uint8_t WrProtectGrEnable; /* Write protect group enable */
	__IO uint8_t ManDeflECC; /* Manufacturer default ECC */
	__IO uint8_t WrSpeedFact; /* Write speed factor */
	__IO uint8_t MaxWrBlockLen; /* Max. write data block length */
	__IO uint8_t WriteBlockPaPartial; /* Partial blocks for write allowed */
	__IO uint8_t Reserved3; /* Reserded */
	__IO uint8_t ContentProtectAppli; /* Content protection application */
	__IO uint8_t FileFormatGrouop; /* File format group */
	__IO uint8_t CopyFlag; /* Copy flag (OTP) */
	__IO uint8_t PermWrProtect; /* Permanent write protection */
	__IO uint8_t TempWrProtect; /* Temporary write protection */
	__IO uint8_t FileFormat; /* File Format */
	__IO uint8_t ECC; /* ECC code */
	__IO uint8_t CSD_CRC; /* CSD CRC */
	__IO uint8_t Reserved4; /* always 1*/
} SD_CSD;

typedef struct {
	u8 ManufacturerID;       // 生产标识ID
	u16 OEM_AppliID;          // OEM/应用 ID
	u32 ProdName1;            // 产品名称1
	u8 ProdName2;            // 产品名称2
	u8 ProdRev;              // 产品版本
	u32 ProdSN;               // 产品序号
	u8 Reserved1;            // 保留
	u16 ManufactDate;         // 生产日期
	u8 CID_CRC;              // CID CRC
	u8 Reserved2;            // always 1
} SD_CID;

typedef struct {
	SD_CSD SD_csd;
	SD_CID SD_cid;
	uint64_t CardCapacity; /* Card Capacity */
	uint32_t CardBlockSize; /* Card Block Size */
	uint16_t RCA; /* 神马来的？ */
	uint8_t CardType;
} SD_CardInfo;

/* Exported constants --------------------------------------------------------*/
#define SD_DMA_MODE                     ((uint32_t)0x00000000)
#define SD_INTERRUPT_MODE               ((uint32_t)0x00000001)
#define SD_POLLING_MODE                 ((uint32_t)0x00000002)

/* Supported Memory Cards
 #define SDIO_STD_CAPACITY_SD_CARD_V1_1     ((uint32_t)0x0)
 #define SDIO_STD_CAPACITY_SD_CARD_V2_0     ((uint32_t)0x1)
 #define SDIO_HIGH_CAPACITY_SD_CARD         ((uint32_t)0x2)
 #define SDIO_MULTIMEDIA_CARD               ((uint32_t)0x3)
 #define SDIO_SECURE_DIGITAL_IO_CARD        ((uint32_t)0x4)
 #define SDIO_HIGH_SPEED_MULTIMEDIA_CARD    ((uint32_t)0x5)
 #define SDIO_SECURE_DIGITAL_IO_COMBO_CARD  ((uint32_t)0x6)
 #define SDIO_HIGH_CAPACITY_MMC_CARD        ((uint32_t)0x7)
 */

#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06
// SD卡指令表
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ,SEND_IF_COND
#define CMD9    9       //命令9 ,读CSD数据
#define CMD10   10      //命令10,读CID数据
#define CMD12   12      //命令12,停止数据传输
#define CMD16   16      //命令16,设置SectorSize 返回0x00
#define CMD17   17      //命令17,读sector
#define CMD18   18      //命令18,读 Multi sector
#define CMD23   23      //命令23,设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24,写sector
#define CMD25   25      //命令25,写Multi sector
#define CMD41   41      //命令41,返回0x00
#define CMD55   55      //命令55,返回0x01
#define CMD58   58      //命令58,读OCR信息
#define CMD59   59      //命令59,使能/禁止CRC,应返回0x00//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF//SD卡回应标志字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
#define GPIO_NSS    GPIO_Pin_12
#define GPIO_SCK    GPIO_Pin_13
#define GPIO_MISO   GPIO_Pin_14
#define GPIO_MOSI   GPIO_Pin_15

#define CS_H        GPIO_SetBits(GPIOB,GPIO_NSS)
#define CS_L        GPIO_ResetBits(GPIOB,GPIO_NSS)

#define 	SPI2_TDR (u32)(SPI2_BASE+0x0C)
#define 	SPI2_RDR (u32)(SPI2_BASE+0x0C)

#ifdef __cplusplus
extern "C" {
#endif
u8 SD_GetCID();
u8 SD_GetCSD();
u8 SD_Init_Config(void);
void spi2_dma_init(void);
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo);
u32 SD_GetSectorCount();
u8 SD_ReadDisk(u8 *buf, u32 sector, u8 cnt);
u8 SD_WriteDisk(const u8 *buf, u32 sector, u8 cnt);
#ifdef __cplusplus
}
#endif

#endif /* TF_SPI_H_ */
