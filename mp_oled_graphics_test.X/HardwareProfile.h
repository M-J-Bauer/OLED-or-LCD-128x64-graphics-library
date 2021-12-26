/***************************************************************************************\
 * File:            HardwareProfile.h   (customized for OLED display test/demo app.)
 *
 * Purpose:         Hardware Platform definitions specific to the application
 *
 * Target H/W:      Chua synth dongle (PIC32-MX340) with 1.3" OLED gfx display.
 *                  OLED display controller: SH1106 (IIC). 
 * *                UART2 support for serial debug/trace (optional).
 *
 * Processor:       PIC32MX340F512H
 *
 * Compiler:        Microchip XC32 v2.50++ under MPLAB.X v5.45++
 *
\***************************************************************************************/

#ifndef __HARDWARE_PROFILE_H
#define __HARDWARE_PROFILE_H

#include "../Common/Compiler.h"

#define DEVICE_ID_PIC32MX460F512L  0x00978053
#define DEVICE_ID_PIC32MX340F512H  0x00916053
#define DEVICE_ID_MASK             0x0FFFFFFF
#define FLASH_APP_ENTRY_POINT      0x9D000000
#define GET_DEVICE_ID()            (DEVID)     // Register in MCU flash PM

//========================  Determine Target Platform ===================================
//
#define USING_CHUA_DONGLE_BOARD  1

// Check that a valid processor type is selected in MPLAB'X Project Properties
#ifndef __32MX340F512H__
#error "Processor type PIC32MX340F512H must be selected in project!"
#endif

//========================  Determine RTC Requirements ==================================
//
// Some of MJB's PIC32 projects use an RTCC device. Select from the following:
//#define RTCC_TYPE_MCP79410
//#define RTCC_TYPE_ISL1208

// Kernel RTC software support is needed if a hardware RTCC device is used
#if defined RTCC_TYPE_ISL1208 || defined RTCC_TYPE_MCP79410
#define INCLUDE_KERNEL_RTC_SUPPORT
#endif

#if defined RTCC_TYPE_ISL1208 && defined RTCC_TYPE_MCP79410
#error "System can define only one RTCC device type!"
#endif

//========================  Select LCD (or OLED) Controller Type ========================
//
#define USE_OLED_CONTROLLER_SH1106
//#define USE_LCD_CONTROLLER_ST7920    
//#define USE_LCD_CONTROLLER_KS0108    

//=================  Select Mass Storage Device and MDD File System  ====================
//
//#define USE_USB_MSD_HOST_INTERFACE        // using USB flash drive
//#define USE_MDD_FILE_SYSTEM               // Memory Disk Drive file system (FAT32)
//#define USE_SST25VF016                    // using SPI serial data flash (SDF) memory
//#define USE_SDF_FILE_SYSTEM               // using SDF file system

//========================== Select other custom hardware ===============================
//
//#define USE_4X4_KEYPAD

//=======================================================================================

// Assign PIC32 I2C peripheral channel(s) that the project uses...
#ifdef __32MX460F512L__
#define USE_I2C_CHANNEL_1             // I2C1 for MPR121
#define USE_I2C_CHANNEL_2             
#define EEPROM_I2C_PORT_NUMBER   2    // I2C2 for EEPROM
//
#else  // assume PIC32MX340 
#define USE_I2C_CHANNEL_1             // I2C1 for all I2C devices
#define EEPROM_I2C_PORT_NUMBER   1    
#endif

//================  UART DRIVER BUILD OPTIONS  =======================
//
#define UART1_RX_INTERRUPT_DRIVEN  0   // if 0, use polled RX input
#define UART2_RX_INTERRUPT_DRIVEN  0   // if 0, use polled RX input

#define UART1_TX_USING_QUEUE  0    // if 0, use direct TX functions (MIDI)
#define UART2_TX_USING_QUEUE  0    // if 0, use direct TX functions (CLI)

#define UART1_TXBUFSIZE      128   // TX FIFO buffer size, chars
#define UART2_TXBUFSIZE      128

#define UART1_RXBUFSIZE      256   // RX FIFO buffer size, chars
#define UART2_RXBUFSIZE      256


/*********************************************************************
* SST25VF016 SPI Flash Memory configuration:
* Define here the SPI channel and slave-select (CS#) pin to use.
*
* MJB's Olimex PIC32-MX460 dev board has an SST25VF016B interfaced 
* to SPI channel 1. SPI CS# is RB2.
*
* SPI clk rate, Fclk = FPB / (2 x (SPIxBRG + 1))
* Example:  FPB = 80MHz, SPI1BRG = 1  :: Fclk = 80 / 4 = 20 MHz
*
*********************************************************************/
#if defined USE_SST25VF016  

#define SST25_SPI_CHANNEL     1   // SPI channel for SST25VF016
#define SPI_CHANNEL_1_ENABLE
#define SST25_SPI_SPIBRG      1   // Value put in SPIxBRG reg.

#define SST25_CS_TRIS     TRISBbits.TRISB2  // SPI Flash CS# = RB2
#define SST25_CS_LAT      LATBbits.LATB2

#endif // USE_SST25VF016

//=======================================================================================

#endif // __HARDWARE_PROFILE_H
