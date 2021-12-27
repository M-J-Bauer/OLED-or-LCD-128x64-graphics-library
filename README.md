# OLED-LCD-128x64-graphics-library
C function library for OLED or LCD monochrome graphics module, 128 x 64 pixels

This repository contains a suite of C functions intended for embedded microcontroller applications
using a monochrome graphics LCD or OLED display module with 128(h) x 64(v) pixels.

Low-level driver functions are provided to support display modules with these controller devices:
ST7920 (LCD), KS0107/KS0108 (LCD), SH1106 (OLED, IIC).  Drivers are customized for Microchip PIC32
MCU devices, but may be easily adapted to other platforms.

A test & demo program is provided to run on a PIC32 MCU target. The program was built and tested
under MPLAB.X IDE v5.45 (XC-32 compiler v2.50, free version). The project is configured to use an
OLED display module with SH1106 controller and IIC (I2C/TWI) connection to the microcontroller.
The project can be easily adapted to use an alternative display module simply by swapping the
driver code module to suit the required devices.

This graphics library uses a RAM buffer of size 1024 bytes in the microcontroller. Using a buffer
maximizes the speed of display update operations.

Functions are provided to display text in various font sizes, e.g. 8, 12, 16 and 24 pixels, some
mono-spaced and others proportionally spaced. Some fonts allow bold type also. The font size
includes lower-case descenders. Upper-case letters are 7, 10, 14 and 20 pixels (resp.) in height.

A function, LCD_PutImage(), displays a bitmap image of any arbitrary size (w x h) at the current
cursor coordinates (x, y). Another function, LCD_BlockFill() renders a rectangular block of pixels.
Macros are defined to draw a single pixel, a horizontal line, vetical line, or any arbitrary size
bar, using the same function, LCD_BlockFill().

Functions to draw lines (other than horizontal or vertical) or other shapes, e.g. polygons, circles,
ellipses, etc, are *not* included in this library, but C-code algorithms for such shapes can be
found easily on the web if needed for your application.

A synopsis of MJB's LCD/OLED graphics functions may be found in file: "LCD_Graphics_Lib.h".

_Arduino version:_

There is a version of this library already adapted to drive a 1.3" OLED display module (SH1106 controller)
under the Arduino IDE. Source code for the library will be placed in a separate GitHub repository shortly. 
(Meanwhile, it's simply a matter of replacing the PIC32 I2C function calls with equivalent functions from
the Arduino "Wire" (I2C) library.)

