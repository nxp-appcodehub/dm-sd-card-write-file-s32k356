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

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                                        INCLUDE FILES
 ==================================================================================================*/
#include "Mem_43_EEP.h"
#include "Mcu.h"
#include "Platform.h"
#include "Port.h"
#include "Dio.h"
#include "S32K356_SIUL2.h"
#include "OsIf.h"

/* FatFs R0.15 API */
#include "fatfs/ff.h"

/*==================================================================================================
 *                                        LOCAL MACROS
 ==================================================================================================*/

/* LED timing (ms) */
#define LED_BLINK_SHORT     500U
#define LED_BLINK_LONG      1500U

/* AUTOSAR Dio channel symbolic names */
#define LED0_RED_CH         DioConf_DioChannel_USERLED_0_RED
#define LED0_GREEN_CH       DioConf_DioChannel_USERLED_0_GREEN
#define LED0_BLUE_CH        DioConf_DioChannel_USERLED_0_BLUE
#define LED1_RED_CH         DioConf_DioChannel_USERLED_1_RED
#define LED1_GREEN_CH       DioConf_DioChannel_USERLED_1_GREEN
#define LED1_BLUE_CH        DioConf_DioChannel_USERLED_1_BLUE

/* Mem_43_EEP instance */
#define MEM_EEP_INSTANCE    MEM_43_EEP_INSTANCE_0_ID

/*
 * Name and content of the file written to the SD card.
 * FILE.TXT is a valid 8.3 name and works with LFN disabled.
 */
#define DEMO_FILENAME       "FILE.TXT"
#define DEMO_TEXT           \
    "NXP S32K356 SD Card Demo\r\n"              \
    "FatFs R0.15 over AUTOSAR Mem_43_EEP\r\n"  \
    "Write test: OK\r\n"

/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 ==================================================================================================*/
static void DelayMs(int delay);
static void TriggerLed(Dio_ChannelType channel, int delay);
static void AllLedsOff(void);
static void HaltError(void);
static void RunFatFsDemo(void);

/*==================================================================================================
 *                                        GLOBAL FUNCTIONS
 ==================================================================================================*/
int main(void)
{
    /* ============================================================= */
    /* MCU / Clock initialisation                                    */
    /* ============================================================= */
#if (MCU_PRECOMPILE_SUPPORT == STD_ON)
    Mcu_Init(NULL_PTR);
#elif (MCU_PRECOMPILE_SUPPORT == STD_OFF)
    Mcu_Init(&Mcu_Config);
#endif

    Mcu_SetMode(McuModeSettingConf_0);

    Mcu_InitClock(McuClockSettingConfig_0);
    while (MCU_PLL_LOCKED != Mcu_GetPllStatus())
    {
        /* Busy-wait until the System PLL is locked */
    }
    Mcu_DistributePllClock();

    /* ============================================================= */
    /* OS / Platform                                                 */
    /* ============================================================= */
    OsIf_Init(NULL_PTR);
    Platform_Init(NULL_PTR);

    /* ============================================================= */
    /* AUTOSAR Port -- configures LED and general-purpose pins        */
    /* ============================================================= */
    Port_Init(NULL_PTR);

    /* Blue LED on while initialising hardware */
    TriggerLed(LED0_BLUE_CH, LED_BLINK_LONG);

    /* ============================================================= */
    /* Mem_43_EEP -- initialises uSDHC controller and SD card.        */
    /* Must complete before FatFs can issue read/write operations.    */
    /* ============================================================= */
    Mem_43_EEP_Init(NULL_PTR);

    /* ============================================================= */
    /* Run the FatFs write demo.                                     */
    /* Creates FILE.TXT on the SD card with:                         */
    /*   "NXP S32K356 SD Card Demo"                                  */
    /*   "FatFs R0.15 over AUTOSAR Mem_43_EEP"                       */
    /*   "Write test: OK"                                            */
    /* The file is readable by a PC when the card is removed.         */
    /* ============================================================= */
    RunFatFsDemo();

    /* ============================================================= */
    /* Idle loop -- demo complete, LEDs reflect result:              */
    /*   GREEN blink  = FILE.TXT written successfully                */
    /*   Both RED on  = error (HaltError was called)                 */
    /* ============================================================= */
    for (;;)
    {
    }

    return 0;
}

/*==================================================================================================
 *                                        LOCAL FUNCTIONS
 ==================================================================================================*/

static void DelayMs(int delay)
{
    uint32 cur     = OsIf_GetCounter(OSIF_COUNTER_SYSTEM);
    uint32 elapsed = 0U;
    uint32 timeout = OsIf_MicrosToTicks((uint32)delay * 1000U, OSIF_COUNTER_SYSTEM);
    while (elapsed < timeout)
    {
        elapsed += OsIf_GetElapsed(&cur, OSIF_COUNTER_SYSTEM);
    }
}

static void TriggerLed(Dio_ChannelType channel, int delay)
{
    AllLedsOff();
    Dio_WriteChannel(channel, STD_HIGH);
    DelayMs(delay);
    Dio_WriteChannel(channel, STD_LOW);
}

static void AllLedsOff(void)
{
    Dio_WriteChannel(LED0_RED_CH,   STD_LOW);
    Dio_WriteChannel(LED0_GREEN_CH, STD_LOW);
    Dio_WriteChannel(LED0_BLUE_CH,  STD_LOW);
    Dio_WriteChannel(LED1_RED_CH,   STD_LOW);
    Dio_WriteChannel(LED1_GREEN_CH, STD_LOW);
    Dio_WriteChannel(LED1_BLUE_CH,  STD_LOW);
}

/* Halt with both red LEDs on -- unrecoverable error */
static void HaltError(void)
{
    AllLedsOff();
    Dio_WriteChannel(LED0_RED_CH, STD_HIGH);
    Dio_WriteChannel(LED1_RED_CH, STD_HIGH);
    while (1) {}
}

/*==================================================================================================
 * RunFatFsDemo
 *
 * Mounts the FAT filesystem that is already present on the SD card (pre-formatted
 * by a PC as FAT16 or FAT32), creates FILE.TXT in the root directory, writes a
 * short text string, closes the file and unmounts the volume.
 *
 * On success: blinks the green LED once (LED_BLINK_LONG).
 * On error:   calls HaltError() -- both red LEDs on, halts.
 *
 * Design notes
 * ------------
 * - The card must be pre-formatted FAT16/FAT32 by a PC.  f_mkfs is NOT used
 *   (FF_USE_MKFS = 0 in ffconf.h).
 * - FA_CREATE_ALWAYS overwrites any existing FILE.TXT, so the demo is
 *   repeatable without manual card cleanup.
 * - f_mount(NULL,...) at the end flushes FAT tables and the directory entry,
 *   making the file visible when the card is inserted into a PC.
 * - The LBA-0 boot-signature check provides an early sanity check that
 *   Mem_43_EEP address 0 really maps to SD card LBA 0.  If the signature is
 *   missing the card may be unformatted or the address mapping is wrong.
 ==================================================================================================*/
static void RunFatFsDemo(void)
{
    FATFS   fs;
    FIL     fil;
    FRESULT fr;
    UINT    bw;

    /* ------------------------------------------------------------------
     * Step 1: LBA-0 boot-sector sanity check
     * ------------------------------------------------------------------ */
    {
        static uint8_t lba0[512] __attribute__((aligned(4)));
        Std_ReturnType st;

        st = Mem_43_EEP_Read(MEM_EEP_INSTANCE, 0U, lba0, 512U);
        if (st == E_OK)
        {
            Mem_43_EEP_JobResultType res;
            do
            {
                Mem_43_EEP_MainFunction();
                res = Mem_43_EEP_GetJobResult(MEM_EEP_INSTANCE);
            }
            while (res == MEM_43_EEP_JOB_PENDING);

            if (res != MEM_43_EEP_JOB_OK)
            {
                HaltError();
            }
        }
        else
        {
            HaltError();
        }

        /* Check 0x55AA boot signature at offset 510 */
        if ((lba0[510] != 0x55U) || (lba0[511] != 0xAAU))
        {
            HaltError();
        }
    }

    /* ------------------------------------------------------------------
     * Step 2: Mount the existing FAT filesystem
     *   opt = 1: force immediate mount (reads BPB, sets up FATFS struct)
     * ------------------------------------------------------------------ */
    fr = f_mount(&fs, "", 1);
    if (fr != FR_OK)
    {
        HaltError();
    }

    /* ------------------------------------------------------------------
     * Step 3: Create / overwrite FILE.TXT and write sample text
     * ------------------------------------------------------------------ */
    fr = f_open(&fil, DEMO_FILENAME, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK)
    {
        (void)f_mount(NULL, "", 0);
        HaltError();
    }

    fr = f_write(&fil, DEMO_TEXT, sizeof(DEMO_TEXT) - 1U, &bw);
    if ((fr != FR_OK) || (bw != (sizeof(DEMO_TEXT) - 1U)))
    {
        (void)f_close(&fil);
        (void)f_mount(NULL, "", 0);
        HaltError();
    }

    fr = f_close(&fil);
    if (fr != FR_OK)
    {
        (void)f_mount(NULL, "", 0);
        HaltError();
    }

    /* ------------------------------------------------------------------
     * Step 4: Unmount -- flushes FAT tables and directory entry
     *   Ensures FILE.TXT is visible when the card is read by a PC.
     * ------------------------------------------------------------------ */
    (void)f_mount(NULL, "", 0);

    /* ------------------------------------------------------------------
     * Step 5: Signal success -- green LED blink
     * ------------------------------------------------------------------ */
    TriggerLed(LED0_GREEN_CH, LED_BLINK_LONG);
}

#ifdef __cplusplus
}
#endif
