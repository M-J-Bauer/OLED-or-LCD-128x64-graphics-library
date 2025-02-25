# OLED or LCD 128x64px graphics library
C function library for OLED or LCD monochrome graphics module, 128 x 64 pixels

This repository contains a suite of C functions intended for embedded microcontroller applications
using a monochrome graphics LCD or OLED display module with 128(h) x 64(v) pixels.

Low-level driver functions are provided to support display modules with these controller devices:
ST7920 (LCD), KS0107/KS0108 (LCD), SH1106 (OLED, IIC).  IIC bus driver code is customized for Microchip
PIC devices, but may be easily adapted to other MCU types.

A test & demo program is provided to run on a PIC32 MCU target. The program was built and tested
under MPLAB.X IDE v5.45 (XC-32 compiler v2.50, free version). The project is configured to use an
OLED display module with SH1106 controller and IIC (I2C/TWI) connection to the microcontroller.

This graphics library uses a RAM buffer of size 1024 bytes in the microcontroller. Using a buffer
maximizes the speed of display update operations.

Functions are provided to display text in various font sizes, e.g. 8, 12, 16 and 24 pixels, some
mono-spaced and others proportionally spaced. Some fonts support bold weight. The font size
includes lower-case descenders. Upper-case letters are 7, 10, 14 and 20 pixels (resp.) in height.

A function, LCD_PutImage(), displays a bitmap image of any arbitrary size (w x h) at the current
cursor coordinates (x, y). Another function, LCD_BlockFill() renders a rectangular block of pixels.
Macros are defined to draw a single pixel, a horizontal line, vetical line, or any arbitrary size
bar, using the same function, LCD_BlockFill().

Functions to draw lines (other than horizontal or vertical) or other shapes, e.g. polygons, circles,
ellipses, etc, are *not* included in this library, but C-code algorithms for such shapes can be
found on the web if needed for your application.

A synopsis of MJB's LCD/OLED graphics functions may be found in file: "LCD_Graphics_Lib.h".

# Microchip/Atmel Studio version, AVR-8 target:

Library already adapted to run on 8-bit AVR devices driving an IIC OLED display with SH1106 controller. 
Customized for AVR applications being developed in 'Microchip/Atmel Studio for AVR and SAM Devices':

https://github.com/M-J-Bauer/OLED-Display-SH1106-I2C-128x64-Graphics-Library-AVR8

# Arduino IDE version, AVR or SAMD21 target:

Library customized for IIC OLED display (with SH1106 controller) for applications being developed
in the Arduino IDE. Suitable for a variety of target MCU devices including AVR and SAMD. Extract the
graphics library files (oled_display_lib.ino, *.h) from the Arduino application here:

https://github.com/M-J-Bauer/ItsyBitsy-SAMD21-Synth/tree/main/Arduino-sketch



