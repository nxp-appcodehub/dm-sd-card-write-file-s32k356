/*==================================================================================================
 * Project : RTD AUTOSAR 4.9
 * Platform : CORTEXM
 * Peripheral : S32K3XX
 * Dependencies : none
 *
 * Autosar Version : 4.9.0
 * Autosar Revision : ASR_REL_4_9_REV_0000
 * Autosar Conf.Variant :
 * SW Version : 7.0.1
 * Build Version : S32K3_RTD_7_0_1_D2602_ASR_REL_4_9_REV_0000_20260206
 *
 * Copyright 2020 - 2026 NXP
 *
 *   NXP Proprietary. This software is owned or controlled by NXP and may only be
 *   used strictly in accordance with the applicable license terms. By expressly
 *   accepting such terms or by downloading, installing, activating and/or otherwise
 *   using the software, you are agreeing that you have read, and that you agree to
 *   comply with and are bound by, such license terms. If you do not agree to be
 *   bound by the applicable license terms, then you may not retain, install,
 *   activate or otherwise use the software.
 ==================================================================================================*/

/* FatFs R0.15 disk I/O glue layer for S32K356 using AUTOSAR Mem_43_EEP over uSDHC.
 * Implements the FatFs disk I/O callbacks (disk_initialize, disk_status,
 * disk_read, disk_write, disk_ioctl). Mem_43_EEP_Init() must be called in
 * main() before any FatFs operation. */

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                                        INCLUDE FILES
 ==================================================================================================*/
#include "ffconf.h"       /* FatFs configuration options (must come first) */
#include "ff.h"           /* Obtains LBA_t and other FatFs integer types   */
#include "diskio.h"       /* FatFs disk I/O prototypes and status constants */
#include "Mem_43_EEP.h"   /* AUTOSAR RTD Mem_43_EEP API                    */

/*==================================================================================================
 *                                       PRIVATE CONSTANTS
 ==================================================================================================*/

/* Physical drive number for the single SD card slot */
#define DISKIO_PDRV         0U

/* Mem_43_EEP instance */
#define DISKIO_MEM_INST     MEM_43_EEP_INSTANCE_0_ID

/* SD card sector size in bytes */
#define DISKIO_SECTOR_SIZE  512U

/*==================================================================================================
 *                                        PRIVATE DATA
 ==================================================================================================*/

/* Current drive status */
static volatile DSTATUS g_diskStat = STA_NOINIT;

/*==================================================================================================
 *                                       PRIVATE HELPERS
 ==================================================================================================*/

/* Poll Mem_43_EEP until the current job finishes.
 * Returns MEM_43_EEP_JOB_OK on success, or the actual (failing) job result. */
static Mem_43_EEP_JobResultType diskio_PollUntilDone(void)
{
    Mem_43_EEP_JobResultType result;
    do
    {
        Mem_43_EEP_MainFunction();
        result = Mem_43_EEP_GetJobResult(DISKIO_MEM_INST);
    }
    while (result == MEM_43_EEP_JOB_PENDING);
    return result;
}

/*==================================================================================================
 *                                        GLOBAL FUNCTIONS
 *                          FatFs disk I/O callbacks (R0.15 signatures)
 ==================================================================================================*/

/*--------------------------------------------------------------------------------------------------
 * disk_status
 *--------------------------------------------------------------------------------------------------*/
DSTATUS disk_status(BYTE pdrv)
{
    if (pdrv != (BYTE)DISKIO_PDRV)
    {
        return STA_NOINIT;
    }
    return g_diskStat;
}

/*--------------------------------------------------------------------------------------------------
 * disk_initialize
 *  Mem_43_EEP_Init() must already have been called by main() before reaching here.
 *--------------------------------------------------------------------------------------------------*/
DSTATUS disk_initialize(BYTE pdrv)
{
    if (pdrv != (BYTE)DISKIO_PDRV)
    {
        return STA_NOINIT;
    }

    /* Clear NOINIT to signal FatFs that the drive is ready.
     * The actual hardware initialisation was done by Mem_43_EEP_Init() in main(). */
    g_diskStat &= (DSTATUS)(~(DSTATUS)STA_NOINIT);

    return g_diskStat;
}

/*--------------------------------------------------------------------------------------------------
 * disk_read
 *  R0.15: sector parameter is LBA_t (32-bit on this target).
 *--------------------------------------------------------------------------------------------------*/
DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
{
    uint32         addr;
    uint32         len;
    Std_ReturnType st;

    if (pdrv != (BYTE)DISKIO_PDRV)             { return RES_PARERR; }
    if (count == 0U)                           { return RES_PARERR; }
    if ((g_diskStat & STA_NOINIT) != 0U)       { return RES_NOTRDY; }

    addr = (uint32)sector * DISKIO_SECTOR_SIZE;
    len  = (uint32)count  * DISKIO_SECTOR_SIZE;

    st = Mem_43_EEP_Read(DISKIO_MEM_INST, addr, (Mem_43_EEP_DataType *)buff, len);
    if (st != (Std_ReturnType)E_OK)                     { return RES_ERROR; }
    if (diskio_PollUntilDone() != MEM_43_EEP_JOB_OK)    { return RES_ERROR; }

    return RES_OK;
}

/*--------------------------------------------------------------------------------------------------
 * disk_write
 *  R0.15: sector parameter is LBA_t (32-bit on this target).
 *--------------------------------------------------------------------------------------------------*/
DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count)
{
    uint32         addr;
    uint32         len;
    Std_ReturnType st;

    if (pdrv != (BYTE)DISKIO_PDRV)             { return RES_PARERR; }
    if (count == 0U)                           { return RES_PARERR; }
    if ((g_diskStat & STA_NOINIT) != 0U)       { return RES_NOTRDY; }
    if ((g_diskStat & STA_PROTECT) != 0U)      { return RES_WRPRT;  }

    addr = (uint32)sector * DISKIO_SECTOR_SIZE;
    len  = (uint32)count  * DISKIO_SECTOR_SIZE;

    st = Mem_43_EEP_Write(DISKIO_MEM_INST, addr,
                          (const Mem_43_EEP_DataType *)buff, len);
    if (st != (Std_ReturnType)E_OK)                     { return RES_ERROR; }
    if (diskio_PollUntilDone() != MEM_43_EEP_JOB_OK)    { return RES_ERROR; }

    return RES_OK;
}

/*--------------------------------------------------------------------------------------------------
 * disk_ioctl
 *--------------------------------------------------------------------------------------------------*/
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    if (pdrv != (BYTE)DISKIO_PDRV)             { return RES_PARERR; }
    if ((g_diskStat & STA_NOINIT) != 0U)       { return RES_NOTRDY; }

    switch (cmd)
    {
        case CTRL_SYNC:
            /* All I/O is already synchronous (poll-until-done above). */
            return RES_OK;

        case GET_SECTOR_SIZE:
            /* Fixed 512-byte sectors; not normally called when FF_MIN_SS == FF_MAX_SS. */
            if (buff == NULL) { return RES_PARERR; }
            *(WORD *)buff = (WORD)DISKIO_SECTOR_SIZE;
            return RES_OK;

        case GET_BLOCK_SIZE:
            /* Erase block size in sectors. 1 = unknown (safe default). */
            if (buff == NULL) { return RES_PARERR; }
            *(DWORD *)buff = 1U;
            return RES_OK;

        default:
            return RES_PARERR;
    }
}

#ifdef __cplusplus
}
#endif
