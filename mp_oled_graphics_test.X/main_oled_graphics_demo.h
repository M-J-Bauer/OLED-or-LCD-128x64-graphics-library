/****************************************************************************************
 *
 * FileName:   main_oled_graphics_demo.h
 *
 * File contains build options and particular def's for the application.
 * See also: "HardwareProfile.h"
 *
 * =======================================================================================
 */
#ifndef _MAIN_APP_H
#define _MAIN_APP_H

#include "../Common/system_def.h"
#include "HardwareProfile.h"

#include "../Drivers/I2C_drv.h"
#include "../Drivers/UART_drv.h"
#include "../Drivers/OLED_SH1106_drv.h"

#include "pic32_low_level.h"
#include "LCD_graphics_lib.h"
#include "gfx_image_data.h"

// =======================================================================================
//                       FIRMWARE VERSION NUMBER AND BUILD OPTIONS
//
#define BUILD_VER_MAJOR   1
#define BUILD_VER_MINOR   0
#define BUILD_VER_DEBUG   0

//#define BUILD_OLED_GRAPHICS_TEST 
//
// =======================================================================================

#ifndef ASCII_NUL    // Common ASCII control codes
//````````````````````````````````````````````````````
#define ASCII_NUL       0    // NUL
#define ASCII_ACK       6    // Acknowledge
#define ASCII_BS        8    // Backspace, Ctrl+H
#define ASCII_HT        9    // Horiz. Tab
#define ASCII_TAB       9    // Horiz. Tab (alt.)
#define ASCII_LF       10    // Line Feed
#define ASCII_CR       13    // Carriage Return [Enter]
#define ASCII_DC2      18    // Device Control 2, Ctrl+R
#define ASCII_NAK      21    // Negative acknowledge
#define ASCII_CAN      24    // Cancel line, Ctrl+X
#define ASCII_ESC      27    // Escape, Ctrl+Z ?
#define ASCII_SP       32    // SPACE
#endif
// Alternative names for ASCII codes
#ifndef SPACE
#define NUL            0     // ASCII NUL code (alt.)
#define SPACE          32    // ASCII space code (alt.)
#endif

#define putChar(b)    UART2_putch(b)
#define putstr(s)     UART2_putstr(s)   // using UART2 for console I/O
#define putNewLine()  UART2_putstr("\r\n")

void  DisplayHomeMessageScreen();
void  DisplayTheQuickBrownFox();
void  TestImagesScreenOne();
void  TestImagesScreenTwo();
void  Disp_TestCharGen(uint8 fontID);
void  Disp_TestCharGen_8p(uint8 fontID);
void  Disp_TestCharGen_12p(uint8 fontID, short page);
void  ButtonScan();
BOOL  getString(char *str, BYTE maxlen);
void  BackgroundTaskExec();

#endif // _MAIN_APP_H
