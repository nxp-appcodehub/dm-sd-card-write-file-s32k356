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

/* FatFs configuration file for S32K356 (single SD volume, no LFN, no RTOS). */

#ifndef _FFCONF_H_
#define _FFCONF_H_

/*==================================================================================================
 *                                         REVISION ID
 *                            Must match FF_DEFINED in ff.h (R0.15 = 80286)
 ==================================================================================================*/
#define FFCONF_DEF  80286

/*==================================================================================================
 *                                   FUNCTION CONFIGURATIONS
 ==================================================================================================*/

/* Read/Write filesystem (0 = R/W, 1 = read-only) */
#define FF_FS_READONLY  0

/* API minimization: 0 = all basic functions enabled */
#define FF_FS_MINIMIZE  0

/* f_gets / f_puts / f_printf: disabled (0) */
#define FF_USE_STRFUNC  0

/* Filtered directory read (f_findfirst / f_findnext): disabled */
#define FF_USE_FIND     0

/* f_mkfs: disabled - card is pre-formatted by PC */
#define FF_USE_MKFS     0

/* Fast seek: disabled */
#define FF_USE_FASTSEEK 0

/* f_expand: disabled */
#define FF_USE_EXPAND   0

/* f_chmod / f_utime: disabled */
#define FF_USE_CHMOD    0

/* f_getlabel / f_setlabel: disabled */
#define FF_USE_LABEL    0

/* f_forward: disabled */
#define FF_USE_FORWARD  0

/* f_lseek64 (64-bit offset): disabled */
#define FF_USE_SEEK64   0

/*==================================================================================================
 *                              LOCALE AND NAMESPACE CONFIGURATIONS
 ==================================================================================================*/

/* OEM code page: 437 = U.S. (correct for FAT32 cards formatted by a PC) */
#define FF_CODE_PAGE    437

/*
 * LFN: disabled.
 * HELLO.TXT / FILE.TXT are valid 8.3 names - LFN is not needed.
 * Keeping LFN off avoids pulling in ffunicode.c for the ff_oem2uni /
 * ff_uni2oem / ff_wtoupper tables (saves ~50 KB flash).
 */
#define FF_USE_LFN      0
#define FF_MAX_LFN      255

/* Character encoding on the API (only relevant when LFN is enabled) */
#define FF_LFN_UNICODE  0

/* LFN working-buffer sizes (only relevant when LFN is enabled) */
#define FF_LFN_BUF      255
#define FF_SFN_BUF      12

/* Relative path: disabled */
#define FF_FS_RPATH     0

/*==================================================================================================
 *                                 DRIVE/VOLUME CONFIGURATIONS
 ==================================================================================================*/

/* Single SD card slot -> one volume */
#define FF_VOLUMES      1

/* Numeric drive ID only (no string volume IDs) */
#define FF_STR_VOLUME_ID    0
#define FF_VOLUME_STRS      "SD"

/* Multi-partition: disabled (single FAT partition found automatically) */
#define FF_MULTI_PARTITION  0

/* Fixed 512-byte sectors (SD/eMMC) */
#define FF_MIN_SS   512
#define FF_MAX_SS   512

/* ATA-TRIM: disabled */
#define FF_USE_TRIM     0

/* FSINFO trust: 0 = trust both free-cluster count and last-alloc cluster */
#define FF_FS_NOFSINFO  0

/*==================================================================================================
 *                                    SYSTEM CONFIGURATIONS
 ==================================================================================================*/

/* Tiny buffer: disabled (normal mode - each FIL has its own sector buffer) */
#define FF_FS_TINY      0

/* exFAT: disabled */
#define FF_FS_EXFAT     0

/* exFAT NORTC: not needed when exFAT is disabled */
#define FF_FS_NORTCEX   0

/*
 * No RTC available on this demo path.
 * FF_FS_NORTC = 1: skip get_fattime().
 * All new/modified objects get the fixed timestamp below.
 */
#define FF_FS_NORTC     1
#define FF_NORTC_MON    1
#define FF_NORTC_MDAY   1
#define FF_NORTC_YEAR   2026

/* File lock: disabled (single-threaded demo) */
#define FF_FS_LOCK      0

/*
 * Re-entrancy: disabled.
 * The demo runs on a single core with no RTOS, so no mutex is needed.
 * FF_FS_TIMEOUT and FF_SYNC_t have no effect when FF_FS_REENTRANT = 0.
 */
#define FF_FS_REENTRANT 0
#define FF_FS_TIMEOUT   1000
#define FF_SYNC_t       BYTE

/*--- End of configuration options ---*/

#endif /* _FFCONF_H_ */
