/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "w25qxx.h"
#include "string.h"
#include "tf_spi.h"
extern u8 CardType;
extern u8 spi2_tx[];

/* Definitions of physical drive number for each drive */
#define DEV_W25QXX		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status(BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
	DSTATUS stat=-1;

	switch (pdrv) {
	case DEV_W25QXX:
		return FR_OK;

	case DEV_MMC:
		return FR_OK;

	case DEV_USB:
		return stat;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize(BYTE pdrv /* Physical drive nmuber to identify the drive */
) {
	DSTATUS stat=-1;

	switch (pdrv) {
	case DEV_W25QXX:
		return FR_OK;

	case DEV_MMC:
		return FR_OK;

	case DEV_USB:

		return stat;
	}
	return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT disk_read(BYTE pdrv, /* Physical drive nmuber to identify the drive */
BYTE *buff, /* Data buffer to store read data */
LBA_t sector, /* Start sector in LBA */
UINT count /* Number of sectors to read */
) {
	DRESULT res=-1;

	switch (pdrv) {
	case DEV_W25QXX:
		// translate the arguments here

		res = WZ5QXX_disk_read(buff, sector, count);

		return res;

	case DEV_MMC:
		res = SD_ReadDisk(buff, sector, count);

//		result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB:
		// translate the arguments here

//		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(BYTE pdrv, /* Physical drive nmuber to identify the drive */
const BYTE *buff, /* Data to be written */
LBA_t sector, /* Start sector in LBA */
UINT count /* Number of sectors to write */
) {
	DRESULT res=-1;

	switch (pdrv) {
	case DEV_W25QXX:
		// translate the arguments here

		res = WZ5QXX_disk_write(buff, sector, count);
		// translate the reslut code here
		return res;

	case DEV_MMC:
		// translate the arguments here

		res = SD_WriteDisk(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB:
		// translate the arguments here

	//	result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, /* Physical drive nmuber (0..) */
BYTE cmd, /* Control code */
void *buff /* Buffer to send/receive control data */
) {
	DRESULT res=-1;

	switch (pdrv) {
	case DEV_W25QXX:
		switch (cmd)
		{
		case CTRL_SYNC:
			return RES_OK;
		case GET_SECTOR_COUNT:
			*(DWORD*) buff = DISK_SECTOR_COUNT;
			return RES_OK;
		case GET_BLOCK_SIZE:
			*(WORD*) buff = DISK_BLOCK_SIZE;
			return RES_OK;
		case CTRL_TRIM:
			return RES_OK;
			}
		return res;

	case DEV_MMC:

		switch (cmd)
		{
		case CTRL_SYNC:
			return RES_OK;
		case GET_SECTOR_COUNT:
			*(DWORD*) buff =SD_GetSectorCount();
			return RES_OK;
		case GET_BLOCK_SIZE:
			*(WORD*) buff = DISK_BLOCK_SIZE;
			return RES_OK;
		case CTRL_TRIM:
			return RES_OK;
		case CTRL_POWER:
			break;
		case CTRL_LOCK:
			break;
		case CTRL_EJECT:
			break;
			/* MMC/SDC command */
		case MMC_GET_TYPE:
			*(u8*)buff=CardType;
			break;
		case MMC_GET_CSD:
			SD_GetCSD();
			memcpy((u8*)buff,spi2_tx,16);
			break;
		case MMC_GET_CID:
			SD_GetCID();
			memcpy((u8*)buff,spi2_tx,16);
			break;
		case MMC_GET_OCR:
			break;
		case MMC_GET_SDSTAT:
			break;
		}
		return res;
		// Process of the command for the MMC/SD card

		return res;

	case DEV_USB:

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}

