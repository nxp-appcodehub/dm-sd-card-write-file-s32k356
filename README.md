# NXP Application Code Hub
[<img src="https://mcuxpresso.nxp.com/static/icon/nxp-logo-color.svg" width="100"/>](https://www.nxp.com)

## Write a file to microSD Card
This example uses the FatFs R0.15 filesystem layered over the AUTOSAR RTD `Mem_43_EEP` driver (uSDHC) to write a file.txt to a microSD card on the FRDM-A-S32K356. It mounts a pre-formatted FAT16/FAT32 volume, creates a file in the root directory, writes a short text string, then safely closes and unmounts the card so the file is readable on a PC. Onboard RGB LEDs indicate the result (green = success, red = error).
[<p align="center"><img src="images/FRDM-A-S32K356_MicroSD.png" width="600"/></p>](./images/FRDM-A-S32K356_MicroSD.png)

#### Boards: FRDM-A-S32K356
#### Categories: SDMMC
#### Peripherals: SDMMC, SIUL2, SDIO
#### Toolchains: S32 Design Studio IDE

## Table of Contents
1. [Software and Tools](#step1)
2. [Hardware](#step2)
3. [Setup](#step3)
4. [Results](#step4)
5. [Support](#step5)
6. [Release Notes](#step6)

## 1. Software and Tools<a name="step1"></a>
This example was developed using the FRDM Automotive Bundle for S32K3 + S32M27. To download and install the complete software and tools ecosystem, use the following link:
- [ FRDM Automotive S32K3 + S32M27 Board Installation Package](https://www.nxp.com/app-autopackagemgr/automotive-software-package-manager:AUTO-SW-PACKAGE-MANAGER?currentTab=0&selectedDevices=S32K3&applicationVersionID=203)

## 2. Hardware<a name="step2"></a>
- Type-C USB cable
- Personal Computer
- [FRDM-A-S32K356](https://www.nxp.com/design/design-center/development-boards-and-designs/FRDM-A-S32K356)[<p align="center"><img src="https://www.nxp.com/assets/images/en/dev-board-image/FRDM-A-S32K356-TOP.jpg" width="500"/></p>](https://www.nxp.com/assets/images/en/dev-board-image/FRDM-A-S32K356-TOP.jpg)

## 3. Setup<a name="step3"></a>
### 3.1 Import the project to S32 Design Studio IDE

1. Open S32 Design Studio IDE. In the Dashboard Panel, choose **Import project from Application Code Hub**.
   [<p align="center"><img src="images/import_project_1.png" width="400"/></p>](./images/import_project_1.png)

2. Find the demo by searching: [dm-sd-card-write-file-s32k356](https://mcuxpresso.nxp.com/appcodehub?search=dm-sd-card-write-file-s32k356)
3. Open the project, click the **GitHub link**, S32 Design Studio IDE will automatically retrieve project attributes, then click **Next>**.
    [<p align="center"><img src="images/import_project_3.png" width="600"></p>](./images/import_project_3.png)

4. Select **main** branch and then click **Next>**.

5. Select your local path for the repo in the **Destination->Directory:** window. The S32 Design Studio IDE will clone the repo into this path, click **Next>**.

6. Select **Import existing Eclipse projects** then click **Next>**.

7. Select the project in this repo (only one project in this repo) then click **Finish**.

### 3.2 Generating, Building and Running the Example
1. In Project Explorer, right-click the project and select **Update Code and Build Project**. This will generate the configuration (Pins, Clocks, Peripherals), update the source code and build the project using the active configuration (e.g. Debug_FLASH).
Make sure the build completes successfully and the *.elf file is generated without errors.
[<p align="center"><img src="images/update_and_build.png" width="250"/></p>](./images/update_and_build.png)
Press **Yes** in the **SDK Component Management** pop-up window to continue.

2. Go to **Debug** and select **Debug Configurations**. There will be a debug configuration for this project:
[<p align="center"><img src="images/Debug_config.png" width="250"/></p>](./images/Debug_config.png)

        Configuration Name                  Description
        -------------------------------     -----------------------
        $(example)_debug_flash_pemicro      Debug the FLASH configuration using PEmicro probe

    Select the desired debug configuration and click on **Debug**. Now the perspective will change to the **Debug Perspective**.
    Use the controls to control the program flow.

## 4. Results<a name="step4"></a>
1. On power-up, the firmware initializes the MCU, clocks, and I/O, then turns **LED0 blue** on for ~1.5 s while the uSDHC controller and SD card are initialized (`Mem_43_EEP_Init`).
2. The demo performs an LBA-0 boot-sector sanity check (verifies the `0x55AA` signature), mounts the pre-formatted FAT16/FAT32 volume, then creates/overwrites `file.txt` in the root directory and writes some sample text.
3. The file is closed and the volume is unmounted (FAT tables and directory entry are flushed) so `file.txt` is readable when the card is inserted into a PC.
4. On success, **LED0 blinks green once** (~1.5 s) and the firmware enters an idle loop. If any step fails, **both red LEDs turn on** and the firmware halts.
5. To verify: power the board, wait for the green blink, remove the card, and open `file.txt` on a PC to confirm the three lines above:
   [<p align="center"><img src="images/FRDM-A-S32K356_MicroSD_file.txt.png" width="300"/></p>](./images/FRDM-A-S32K356_MicroSD_file.txt.png)

**LED status summary:**
- blue = SD/hardware initialization in progress; 
- green (LED0) = file written successfully; 
- both red on = fatal error (firmware halts).

## 5. Support<a name="step5"></a>
For general technical questions related to NXP microcontrollers, please use the *[NXP Community Forum](https://community.nxp.com/)*.

#### Project Metadata

<!----- Boards ----->
[![Board badge](https://img.shields.io/badge/Board-FRDM&ndash;A&ndash;S32K356-blue)](https://mcuxpresso.nxp.com/appcodehub?hwBoard=FRDM-A-S32K356)

<!----- Categories ----->
[![Category badge](https://img.shields.io/badge/Category-SDMMC-yellowgreen)](https://mcuxpresso.nxp.com/appcodehub?category=sdmmc)

<!----- Peripherals ----->
[![Peripheral badge](https://img.shields.io/badge/Peripheral-SDMMC-yellow)](https://mcuxpresso.nxp.com/appcodehub?peripheral=sdmmc)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-SIUL2-yellow)](https://mcuxpresso.nxp.com/appcodehub?peripheral=siul2)
[![Peripheral badge](https://img.shields.io/badge/Peripheral-SDIO-yellow)](https://mcuxpresso.nxp.com/appcodehub?peripheral=sdio)

<!----- Toolchains ----->
[![Toolchain badge](https://img.shields.io/badge/Toolchain-S32%20DESIGN%20STUDIO%20IDE-orange)](https://mcuxpresso.nxp.com/appcodehub?toolchain=s32_design_studio_ide)

Questions regarding the content/correctness of this example can be entered as Issues within this GitHub repository.

> **Note**: For more general technical questions regarding NXP Microcontrollers and differences in expected functionality, enter your questions on the [NXP Community Forum](https://community.nxp.com/)

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/NXP_Semiconductors)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/nxp-semiconductors)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/nxpsemi/)
[![Follow us on Twitter](https://img.shields.io/badge/X-Follow%20us%20on%20X-black.svg)](https://x.com/NXP)

## 6. Release Notes<a name="step6"></a>
| Version | Description / Update                           | Date                           |
|:-------:|------------------------------------------------|-------------------------------:|
| 1.0     | Initial release on Application Code Hub        | September 2<sup>nd</sup>, 2026 |