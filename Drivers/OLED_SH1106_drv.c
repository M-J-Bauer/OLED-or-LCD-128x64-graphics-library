/*
 * File:    OLED_SH1106_drv.c
 *
 * Low-level driver functions for monochrome graphic display module, 132 x 64 pixels,
 * customized for SH1106 OLED controller using 2-wire IIC (I2C) MCU interface.
 * This version uses I2C driver functions from MJB's PIC32MX app framework.
 *
 * The SH1106 driver functions work with MJB's GLCD library: LCD_graphics_lib.c
 * Macros providing an API for OLED display functions are defined in LCD_graphics_lib.h.
 * 
 * Originated:  2021,  M.J.Bauer  [www.mjbauer.biz]
 */
#include "OLED_SH1106_drv.h"
#include "I2C_drv.h"

//  Wrappers for driver functions called from LCD_graphics_lib.c
//
void  LCD_Init()
{
    SH1106_Init();
}

void  LCD_ClearGDRAM()
{
    SH1106_ClearGDRAM();
}

void  LCD_WriteBlock(uint16 *scnbuf, uint16 x, uint16 y, uint16 w, uint16 h)
{
    SH1106_WriteBlock(scnbuf, x, y, w, h);
}


/*----------------------------------------------------------------------------------
 * Name               :  SH1106_WriteCommand()
 * Function           :  Write 8-bit command to SH1106 controller
 * Entry arg(s)       :  cmd byte
 * Return             :  --
------------------------------------------------------------------------------------*/
void  SH1106_WriteCommand(uint8 cmd)
{
    if (I2C1MasterStart(SH1106_I2C_ADDRESS))
    {
        I2C1MasterSend(SH1106_MESSAGETYPE_COMMAND);
        I2C1MasterSend(cmd);
        Stop_I2C1();
    }
}


/*----------------------------------------------------------------------------------
 * Name               :  SH1106_WriteData()
 * Function           :  Write 8-bit data to SH1106 controller
 * Entry arg(s)       :  data byte
 * Return             :  --
------------------------------------------------------------------------------------*/
void  SH1106_WriteData(uint8 data)
{
    if (I2C1MasterStart(SH1106_I2C_ADDRESS))
    {
        I2C1MasterSend(SH1106_MESSAGETYPE_DATA);
        I2C1MasterSend(data);
        Stop_I2C1();
    }
}


/*----------------------------------------------------------------------------------
 * Name               :  SH1106_Init()
 * Function           :  Initialize 2 x SH1106 controller chips.
 * Input              :  --
 * Return             :  --
 * Note               :  SH1106 GDRAM not cleared
------------------------------------------------------------------------------------*/
void  SH1106_Init(void)
{
    SH1106_WriteCommand(SH1106_DISPLAYOFF);                    // 0xAE
    SH1106_WriteCommand(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
    SH1106_WriteCommand(0x80);                                 // (suggested)
    SH1106_WriteCommand(SH1106_SETMULTIPLEX);                  // 0xA8
    SH1106_WriteCommand(0x3F);
    SH1106_WriteCommand(SH1106_SETDISPLAYOFFSET);              // 0xD3
    SH1106_WriteCommand(0x00);                                 // no offset

    SH1106_WriteCommand(SH1106_SETSTARTLINE | 0);              // line #0
    SH1106_WriteCommand(SH1106_CHARGEPUMP);                    // 0x8D
    SH1106_WriteCommand(0x10);
    SH1106_WriteCommand(SH1106_MEMORYMODE);                    // 0x20
    SH1106_WriteCommand(0x00);                                 // use KS0108 format
    SH1106_WriteCommand(SH1106_SEGREMAP | 1);                  // 0xA0
    SH1106_WriteCommand(SH1106_COMSCANDEC);
    
    SH1106_WriteCommand(SH1106_SETCOMPINS);                    // 0xDA
    SH1106_WriteCommand(0x12);
    SH1106_WriteCommand(SH1106_SETCONTRAST);                   // 0x81
    SH1106_WriteCommand(0x3F);                                 // POR value = 0x80
    SH1106_WriteCommand(SH1106_SETPRECHARGE);                  // 0xD9
    SH1106_WriteCommand(0x22);
    SH1106_WriteCommand(SH1106_SETVCOMDETECT);                 // 0xDB
    SH1106_WriteCommand(0x40);
    
    SH1106_WriteCommand(SH1106_DISPLAYALLON_RESUME);           // 0xA4
    SH1106_WriteCommand(SH1106_NORMALDISPLAY);                 // 0xA6
    SH1106_WriteCommand(SH1106_DISPLAYON);                     // 0xAF
}


/*----------------------------------------------------------------------------------
 * Name               :  SH1106_Wake()
 * Function           :  Wake SH1106 controller from sleep state (display ON).
 * Input              :  --
 * Return             :  --
 * Note               :  Alt. macro:  OLED_Display_Wake()
------------------------------------------------------------------------------------*/
void  SH1106_Wake(void)
{
    SH1106_WriteCommand(SH1106_DISPLAYON);
}


/*----------------------------------------------------------------------------------
 * Name               :  SH1106_Sleep()
 * Function           :  Put SH1106 controller into sleep state (display OFF).
 * Input              :  --
 * Return             :  --
 * Note               :  Alt. macro:  OLED_Display_Sleep()
------------------------------------------------------------------------------------*/
void  SH1106_Sleep(void)
{
    SH1106_WriteCommand(SH1106_DISPLAYOFF); 
}


/*----------------------------------------------------------------------------------
 * Name               :  SH1106_SetContrast()
 * Function           :  Set OLED display contrast (brightness).
 * Input              :  level_pc:  contrast level (0..100%)
 * Return             :  --
------------------------------------------------------------------------------------*/
void  SH1106_SetContrast(unsigned level_pc)
{
    if (level_pc > 100)  level_pc = 100;
    SH1106_WriteCommand(SH1106_SETCONTRAST); 
    SH1106_WriteCommand((level_pc * 255) / 100);
}


/*----------------------------------------------------------------------------------
 * Name               :  SH1106_ClearGDRAM()
 *
 * Function           :  Clear entire SH1106 controller graphics data RAM.
 *                       MCU RAM screen buffer is not cleared. (Use Disp_ClearScreen())
 *
 * GDRAM format in SH1106 controller (132 x 64 pixels):
 * ````````````````````````````````````````````````````
 *               0 -------------- Segment --------------- 131
 *      Page 0   ||||||||||||||||||||||||||||||||||||||||||||
 *      Page 1   ||||||||||||||||||||||||||||||||||||||||||||
 *      Page 2   ||||||||||||||||||||||||||||||||||||||||||||
 *      Page 3   ||||||||||||||||||||||||||||||||||||||||||||
 *      Page 4   ||||||||||||||||||||||||||||||||||||||||||||
 *      Page 5   ||||||||||||||||||||||||||||||||||||||||||||
 *      Page 6   ||||||||||||||||||||||||||||||||||||||||||||
 *      Page 7   ||||||||||||||||||||||||||||||||||||||||||||
 *
 * Legend: Page = 8 lines.  Data byte (|) = 8 pixels vertical, bit 0 is at the top.
 * 
 * NB: The first 2 pixels on each line (segments 0 and 1) are not displayed.
------------------------------------------------------------------------------------*/
void  SH1106_ClearGDRAM(void)
{
    int   page, partition, segment;  // 16 segments per partition

    for (page = 0;  page < 8;  page++)
    {
        // Set GDRAM start of page address -- there are 8 pages
        SH1106_WriteCommand(SH1106_PAGEADDR + page);

        for (partition = 0;  partition < 9;  partition++)  
        {
            // Set column/segment address for this partition
            SH1106_WriteCommand(SH1106_SETCOLUMNADDRLOW);  // AddrLow = 0
            SH1106_WriteCommand(SH1106_SETCOLUMNADDRHIGH + partition);
            // write 16 segments (max.) in auto-increment mode
            I2C1MasterStart(SH1106_I2C_ADDRESS);
            I2C1MasterSend(SH1106_MESSAGETYPE_DATA);

            for (segment = 0;  segment < 16;  segment++)
            {
//              if (partition == 8 && segment > 3) break;  // done 132 segments
                I2C1MasterSend(0);
            }
            Stop_I2C1();
        }
    }
}


/*----------------------------------------------------------------------------------
 * Function   :  SH1106_WriteBlock()
 *
 * Overview   :  Copies a rectangular block of pixels from an external screen
 *               buffer in MCU RAM to the SH1106 controller GDRAM (graphics data RAM).
 *               In general, the GDRAM pixel format differs from the MCU RAM screen
 *               buffer, so a transformation of some sort needs to be performed.
 *               The screen buffer format is optimized for speed in display functions.
 *
 * Input      :  scnBuf = address of screen buffer in MCU RAM
 *               x, y = pixel coords of upper LHS of block to be copied
 *               w, h = width and height (pixels) of block to be copied
 *
 * Return     :  --
 *
 * Notes:     :  The (external) screen buffer is formatted as 8 "columns" horizonally
 *               by 64 "rows" vertically. Each column has 16 pixels. The leftmost
 *               pixel is stored in the MS bit (bit15) of the column.
 *               For details, see MJB's LCD graphics library (LCD_graphics_lib.c).
------------------------------------------------------------------------------------*/
void  SH1106_WriteBlock(uint16 *scnBuf, uint16 x, uint16 y, uint16 w, uint16 h)
{
    uint8   page;                   // page address in SH1106 (1 page = 8 rows)
    uint8   x_coord;                // pixel horizontal coordinate
    int     row, col;               // row and column word index for screen buffer
    int     firstPage;              // first (top) page to be modified
    int     lastPage;               // last (bottom) page to be modified
    int     firstCol;               // first (leftmost) column in a row to be modified
    int     lastCol;                // last (rightmost) column in a row to be modified
    int     offset;                 // address offset of pixel word in scnBuf
    uint8   segAddr;                // segment address for write
    uint8   pixels;                 // 8 pixels aligned vertically (LS bit at top)
    uint8   bitmask;
    uint16  pixelWord[8];           // 8 rows x 16 pixels in a "column" to be modified
    uint16  *pBuf;

    if (y > 63)  y = 0;             // prevent writing past end-of-screen (vert.)
    if ((y + h) > 63)  h = 63 - y;

    if (x > 127)  x = 0;            // prevent writing past end-of-row (horiz.)
    if ((x + w) > 128)  w = 128 - x;

    firstPage = y / 8;
    lastPage = (y + h - 1) / 8;

    firstCol = x / 16;
    lastCol = (x + w - 1) / 16;

    for (page = firstPage;  page <= lastPage;  page++)
    {
        SH1106_WriteCommand(SH1106_PAGEADDR | page);   // set page address (0..7)
        
        for (col = firstCol;  col <= lastCol && col < 8;  col++)
        {
            // Grab a bunch of 16 (H) x 8 (V) pixels from buffer...
            offset = page * 64 + col;  // 64 words per buffer "page"
            pBuf = scnBuf + offset;
            for (row = 0;  row < 8;  row++)  // 8 rows in SH1106 page
            {
                pixelWord[row] = *pBuf;
                pBuf += 8;  // next row (advance 8 col's)
            }

            segAddr = col * 16 + 2;   // the first 2 pixels on any line are duds!
            SH1106_WriteCommand(SH1106_SETCOLUMNADDRLOW + (segAddr & 0xF)); 
            SH1106_WriteCommand(SH1106_SETCOLUMNADDRHIGH + (segAddr >> 4)); 
            // Write bunch of pixels, 16 (H) x 8 (V) to SH1106 controller
            I2C1MasterStart(SH1106_I2C_ADDRESS);
            I2C1MasterSend(SH1106_MESSAGETYPE_DATA);
            
            for (x_coord = 0;  x_coord < 16;  x_coord++)
            {
                // transpose horiz pixels in pixelWord[8] to vertical pixelByte
                pixels = 0;
                bitmask = 0x01;
                for (row = 0;  row < 8;  row++)
                {
                    if (pixelWord[row] & 0x8000) pixels |= bitmask;
                    bitmask = bitmask << 1;   // next pixel down
                    pixelWord[row] = pixelWord[row] << 1;  // next pixel right
                }
                // write 8 pixels (vertical) to SH1106 controller
                I2C1MasterSend(pixels);  // segment address auto increments
            }
            Stop_I2C1();
        }
    }
}


/*----------------------------------------------------------------------------------
 * Name               :  SH1106_Test_Pattern()
 *
 * Function           :  Low-level test of SH1106 graphics write sequence.
 *                       The MCU RAM buffer is bypassed, i.e. not used.
 *                       The screen should show a pattern of diagonal lines,
 *                       slope -45 degrees, thickness 1 pixel, spaced 8 pixels apart,
 *                       within an area 128 x 64 pixels.
 * Input              :  --
 * Note               :  May be called without first calling SH1106_ClearGDRAM(),
 *                       but then random pixels may appear in segments 130, 131.
------------------------------------------------------------------------------------*/
void  SH1106_Test_Pattern()
{
    int    page, column, line;  // a "column" is 8 pixels wide in this context
    uint8  segAddr;
    uint8  pixels;

    for (page = 0;  page < 8;  page++)
    {
        // Set GDRAM start of page address -- there are 8 pages
        SH1106_WriteCommand(SH1106_PAGEADDR + page);

        for (column = 0;  column < 16;  column++)   // 16 "columns" of 8 pix
        {
            // Set segment address for this column
            segAddr = column * 8 + 2;
            SH1106_WriteCommand(SH1106_SETCOLUMNADDRLOW + (segAddr & 0xF)); 
            SH1106_WriteCommand(SH1106_SETCOLUMNADDRHIGH + (segAddr >> 4));
            
            // write 8 segments in auto-increment mode
            I2C1MasterStart(SH1106_I2C_ADDRESS);
            I2C1MasterSend(SH1106_MESSAGETYPE_DATA);
            pixels = 0x01;  // top pixel
            
            for (line = 0;  line < 8;  line++)  // 8 lines x 8 pix
            {
                I2C1MasterSend(pixels);
                pixels = pixels << 1;   // move 1 pixel down
            }
            Stop_I2C1();
        }
    }
}

