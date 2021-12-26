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

Here is a synopsis of MJB's LCD/OLED graphics library:

//---------- Controller low-level functions, defined in driver module -------------------
//
extern  bool  LCD_Init(void);      // LCD controller initialisation
extern  void  LCD_ClearGDRAM();
extern  void  LCD_WriteBlock(uint16 *scnBuf, uint16 x, uint16 y, uint16 w, uint16 h);

//---------- LCD function & macro library (API) -----------------------------------------
//
void    LCD_ClearScreen(void);            // Clear LCD GDRAM and MCU RAM buffers
void    LCD_Mode(uint8 mode);             // Set pixel write mode (set, clear, flip)
void    LCD_PosXY(uint16 x, uint16 y);    // Set graphics cursor position to (x, y)
uint16  LCD_GetX(void);                   // Get cursor pos x-coord
uint16  LCD_GetY(void);                   // Get cursor pos y-coord
void    LCD_SetFont(uint8 font_ID);       // Set font for char or text display
uint8   LCD_GetFont();                    // Get current font ID
void    LCD_PutChar(char uc);             // Show ASCII char at (x, y)
void    LCD_PutText(char *str);           // Show text string at (x, y)
void    LCD_PutDigit(uint8 bDat);         // Show hex/decimal digit value (1 char)
void    LCD_PutHexByte(uint8 bDat);       // Show hexadecimal byte value (2 chars)

void    LCD_PutDecimalWord(uint16 val, uint8 fieldSize);  // Show uint16 in decimal
void    LCD_BlockFill(uint16 w, uint16 h);   // Fill area, w x h pixels, at cursor (x, y)
uint8   LCD_PutImage(bitmap_t *image, uint16 w, uint16 h);  // Show bitmap image at (x, y)
uint16 *LCD_ScreenCapture();              // Return a pointer to the screen buffer

// These macros draw various objects at the current graphics cursor position...
#define LCD_PutPixel()           LCD_BlockFill(1, 1)
#define LCD_DrawBar(w, h)        LCD_BlockFill(w, h)
#define LCD_DrawLineHoriz(len)   LCD_BlockFill(len, 1)
#define LCD_DrawLineVert(len)    LCD_BlockFill(1, len)

//---------- Aliases for OLED (or other 128 x 64 pixel display) ----------------------------
//
#define Disp_GetMaxX()      (127)                   // Screen width, pixels
#define Disp_GetMaxY()      (63)                    // Screen height, pixels
#define Disp_Init()         LCD_Init()              // Controller initialisation
#define Disp_ClearScreen()  LCD_ClearScreen()       // Clear GDRAM and MCU RAM buffers
#define Disp_Mode(mode)     LCD_Mode(mode)          // Set pixel write mode (set, clear, flip)
#define Disp_PosXY(x, y)    LCD_PosXY(x, y)         // Set graphics cursor position
#define Disp_GetX()         LCD_GetX()              // Get cursor pos'n x-coord
#define Disp_GetY()         LCD_GetY()              // Get cursor pos'n y-coord
#define Disp_SetFont(font)  LCD_SetFont(font)       // Set font for text
#define Disp_GetFont()      LCD_GetFont()           // Get current font ID
#define Disp_PutChar(c)     LCD_PutChar(c)          // Show ASCII char at (x, y)
#define Disp_PutText(s)     LCD_PutText(s)          // Show text string at (x, y)
#define Disp_PutDigit(d)    LCD_PutDigit(d)         // Show hex/decimal digit (1 char)
#define Disp_PutHexByte(h)  LCD_PutHexByte(h)       // Show hexadecimal byte (2 chars)

#define Disp_PutDecimal(w, n)     LCD_PutDecimalWord(w, n)  // Show uint16 in decimal (n places)
#define Disp_BlockFill(w, h)      LCD_BlockFill(w, h)       // Fill area w x h pixels at (x, y)
#define Disp_PutImage(img, w, h)  LCD_PutImage(img, w, h)   // Show bitmap image at (x, y)
#define Disp_ScreenCapture()      LCD_ScreenCapture()       // Return a pointer to the screen buffer

#define Disp_PutPixel()           LCD_BlockFill(1, 1)
#define Disp_DrawBar(w, h)        LCD_BlockFill(w, h)
#define Disp_DrawLineHoriz(len)   LCD_BlockFill(len, 1)
#define Disp_DrawLineVert(len)    LCD_BlockFill(1, len)

