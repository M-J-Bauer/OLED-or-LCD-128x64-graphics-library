/**************************************************************************************************
 *
 * FileName:        main_oled_graphics_demo.c
 *
 * Overview:        Test and demo utility for SH1106 OLED graphics display panel
 * `````````        
 * Processor:       PIC32MX340
 *
 * H/W Platform:    PIC32-MX340 development board or compatible target platform.
 * `````````````     
 * Compiler:        Microchip MPLAB XC32 v1.33 or later (free version).
 *
 * Author:          M.J.Bauer, 2021   [www.mjbauer.biz]
 *
 * Reference:       http:/www.mjbauer.biz/mjb_resources.htm
 *
 * ================================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "main_oled_graphics_demo.h"

BOOL isButtonHit;
unsigned m_ButtonPressTime_ms;

// test image is a bold 'X', each 'dot' is 4 pixels square
// bitmap data, test using w = 16..24 (typ 20), h <= 20
//
uint8 test_image[] ={
    0xF0, 0x00, 0xF0,
    0xF0, 0x00, 0xF0,
    0xF0, 0x00, 0xF0,
    0xF0, 0x00, 0xF0,
    0x0F, 0x0F, 0x00,
    0x0F, 0x0F, 0x00,
    0x0F, 0x0F, 0x00,
    0x0F, 0x0F, 0x00,
    0x00, 0xF0, 0x00,
    0x00, 0xF0, 0x00,
    0x00, 0xF0, 0x00,
    0x00, 0xF0, 0x00,
    0x0F, 0x0F, 0x00,
    0x0F, 0x0F, 0x00,
    0x0F, 0x0F, 0x00,
    0x0F, 0x0F, 0x00,
    0xF0, 0x00, 0xF0,
    0xF0, 0x00, 0xF0,
    0xF0, 0x00, 0xF0,
    0xF0, 0x00, 0xF0
};


int main(void)
{
    static short  screen;
    static uint32 ledPeriodStartTime;
    static char   inbuf[42];
    static short  nextLine_ypos;
    static short  fontID, fontSize;
    static short  lineSpacing;
    
    nextLine_ypos = 0;    // set defaults
    lineSpacing = 10;
    fontID = MONO_8_NORM;
    fontSize = 8;

    InitializeMCUclock();
    Init_MCU_IO_ports(); // Initialize I/O pins (in pic32_low_level.c)
    UART2_init(38400); // for terminal I/O if needed
    Init_I2C1(); // for OLED display module and 3-axis accel. chip

    putstr("\n* PIC32 MCU RESET *\n");
    putstr("* OLED graphics display test & demo app.\n");
    putstr("* Press 'user' button to show next test screen, or...\n");
    putstr("* type a text line here to display on the OLED screen.\n");
    putstr("* To set font, enter: `n  (where n is the font ID code).\n");
    putstr("> ");

    Disp_Init();
    SH1106_Test_Pattern();

    while (1)
    {
        if (isTaskPending_5ms())
        {
            ButtonScan();
        }

        if (isTaskPending_500ms())
        {
            ledPeriodStartTime = milliseconds();
            HEARTBEAT_LED_ON();
        }

        if ((milliseconds() - ledPeriodStartTime) > 100) // 100ms duty expired...
            HEARTBEAT_LED_OFF();

        if (isButtonHit)
        {
            isButtonHit = 0;
            Disp_ClearScreen();

            if (screen == 0) DisplayHomeMessageScreen();
            if (screen == 1) DisplayTheQuickBrownFox();
            if (screen == 2) Disp_TestCharGen_8p(MONO_8_NORM);
            if (screen == 3) Disp_TestCharGen_8p(PROP_8_NORM);
            if (screen == 4) Disp_TestCharGen_12p(PROP_12_NORM, 0); // page 0
            if (screen == 5) Disp_TestCharGen_12p(PROP_12_NORM, 1); // page 1
            if (screen == 6) TestImagesScreenOne();
            if (screen == 7) TestImagesScreenTwo();

            if (++screen >= 8) screen = 0; // repeat test sequence
        }
        
        if (getString(inbuf, 40))  // have string from terminal...
        {
            if (inbuf[0] == '`')  // command message -- set font
            {
                fontID = atoi(&inbuf[1]) & 0xF;
                fontSize = 8 + 4 * (fontID >> 2);   // b3:b2 (0: 8p, 1: 12p, 2: 16p, 3: 24p)
                if (fontID >= 12) fontSize = 24;    // corrected

                if (fontSize == 12) lineSpacing = 16;
                else if (fontSize == 16) lineSpacing = 20;
                else if (fontSize == 24) lineSpacing = 32;
                else  lineSpacing = 10;
            }
            else if (strlen(inbuf) != 0)
            {
                Disp_SetFont(fontID); 
                Disp_PosXY(0, nextLine_ypos);
                Disp_Mode(CLEAR_PIXELS);
                Disp_BlockFill(128, 64);
                Disp_PosXY(0, nextLine_ypos);
                Disp_Mode(SET_PIXELS);
                Disp_PutText(inbuf);
                
                nextLine_ypos += lineSpacing; 
                if (nextLine_ypos + fontSize >= 64)  nextLine_ypos = 0; //wrap to top
            }
            else  nextLine_ypos = 0;
            
            putstr("> ");  //prompt
        }
    }
}


void DisplayHomeMessageScreen()
{
    Disp_Mode(SET_PIXELS);

    Disp_PosXY(0, 0);
    Disp_DrawLineHoriz(128);
    Disp_PosXY(0, 63);
    Disp_DrawLineHoriz(128);
    Disp_PosXY(0, 0);
    Disp_DrawLineVert(64);
    Disp_PosXY(127, 0);
    Disp_DrawLineVert(64);

    Disp_SetFont(PROP_12_NORM);
    Disp_PosXY(18, 24);
    Disp_PutText("Hello, world!");
}

void DisplayTheQuickBrownFox()
{
    int i, x;

    Disp_ClearScreen();
    Disp_Mode(SET_PIXELS);

    Disp_PosXY(0, 22);
    Disp_DrawBar(128, 20);

    Disp_SetFont(PROP_8_NORM);
    Disp_PosXY(16, 0);
    Disp_PutText("The quick brown fox");
    Disp_PosXY(8, 10);
    Disp_PutText("jumps over the lazy dog");

    Disp_SetFont(MONO_16_NORM);
    Disp_Mode(CLEAR_PIXELS);
    Disp_PosXY(4, 24);
    Disp_PutText("0123456789");
    Disp_Mode(SET_PIXELS);

    for (x = 2, i = 0; i < 5; i++, x += 26)
    {
        Disp_PosXY(x, 44);
        Disp_PutImage(test_image, 20, 20);
    }
}

void TestImagesScreenOne()
{
    Disp_Mode(SET_PIXELS);

    Disp_PosXY(0, 0);
    Disp_PutImage((bitmap_t *) big_right_arrow, 21, 18);

    Disp_PosXY(2, 22);
    Disp_PutImage((bitmap_t *) coffee_cup_icon, 39, 41);

    Disp_PosXY(48, 0);
    Disp_DrawLineVert(64);

    Disp_PosXY(54, 10);
    Disp_PutImage((bitmap_t *) treble_clef_16x40, 16, 40);

    Disp_PosXY(80, 0);
    Disp_PutImage((bitmap_t *) chess_knight, 44, 44);

    Disp_SetFont(MONO_8_NORM);
    Disp_PosXY(80, 54);
    Disp_PutText("MJB 2016");
}

void TestImagesScreenTwo()
{
    Disp_Mode(SET_PIXELS);

    Disp_PosXY(0, 14);
    Disp_DrawBar(60, 50);

    Disp_Mode(CLEAR_PIXELS);
    Disp_PosXY(2, 16);
    Disp_PutImage((bitmap_t *) car_front_56x46, 56, 46);

    Disp_Mode(SET_PIXELS);
    Disp_PosXY(62, 2);
    Disp_PutImage((bitmap_t *) tweety_bird_63x54, 63, 54);

    Disp_SetFont(MONO_8_NORM);
    Disp_Mode(FLIP_PIXELS);
    Disp_PosXY(2, 4);
    Disp_PutText("Mix images");
    Disp_PosXY(80, 24);
    Disp_PutText("and");
    Disp_PosXY(83, 34);
    Disp_PutText("text");
}


// Function to test character generator functions in graphics library.
// Font size should be 8 px (mono or prop).
//
void Disp_TestCharGen_8p(uint8 fontID)
{
    uint16 x, y = 0;
    short line, pos;
    uint8 uc = 32; // char code (extended ASCII)

    Disp_ClearScreen();
    Disp_SetFont(fontID);

    for (line = 0; line < 5; line++)
    {
        x = 4;
        y = line * 12;
        Disp_PosXY(x, y);

        for (pos = 0; pos < 20; pos++)
        {
            Disp_PutChar(uc);
            if (++uc > 0x7F) break;
        }
    }
}

// Function to test character generator functions in Graphics library.
// Font size should be 12 px (normal or bold).  
// Arg. 'page' selects 4 'sticks' from the ASCII set...
// If page is 0, display chars 32..95, otherwise display chars 64..127.
//

void Disp_TestCharGen_12p(uint8 fontID, short page)
{
    char uc = 32;
    short count = 0;
    short y_coord = 0;

    if (page != 0) uc = 64;
    Disp_ClearScreen();
    Disp_SetFont(fontID);
    Disp_PosXY(0, 0);

    while (count++ < 64) // show 64 chars over 4 lines
    {
        Disp_PutChar(uc++);
        if ((count % 16) == 0)
        {
            y_coord += 16;
            Disp_PosXY(0, y_coord);
        }
    }
}

/*
  |  Routine to monitor button state and to detect button hits.
  |  Background task called at 5ms intervals.
  |
  |  If a button hit is detected, the flag isButtonHit is set TRUE. Caller must clear flag.
  |  If the button is held pressed, the variable m_ButtonPressTime_ms will be non-zero.
  |  While the button is released, m_ButtonPressTime_ms will be zero (0).
 */
void ButtonScan()
{
    static uint32 buttonReleaseTime_ms;
    static uint8 buttonStateLastScan; // 0: Released, 1: Pressed
    static uint8 buttonStateDeglitched;

    if (BUTTON_PRESSED)
    {
        m_ButtonPressTime_ms += 5;
        buttonReleaseTime_ms = 0;
    }
    else // Button released
    {
        buttonReleaseTime_ms += 5;
        m_ButtonPressTime_ms = 0;
    }

    // Apply de-glitch / de-bounce filter
    if (m_ButtonPressTime_ms > 45) buttonStateDeglitched = 1;
    if (buttonReleaseTime_ms > 95) buttonStateDeglitched = 0;

    if (buttonStateDeglitched && buttonStateLastScan == 0)
        isButtonHit = 1;

    buttonStateLastScan = buttonStateDeglitched;
}

/**
 * Function:  getString()
 *
 * Accepts a string (str) via serial port input, up to a limited length (maxlen).
 * This is a non-blocking function which may take multiple calls to receive the string;
 * i.e. it does not wait for user input.
 *
 * When an ASCII CR (13) code is received, the function returns TRUE;  else FALSE.
 * Primitive line editing capability is implemented by processing ASCII backspace (BS)
 * and cancel line (CAN) codes. (See header file for more detail.)
 */
BOOL getString(char *str, BYTE maxlen)
{
    static int index; // index into array str[] - remembered
    static BYTE count; // number of chars in output string - remembered
    BYTE rxb; // received byte (character)
    BOOL status = FALSE; // return value

    if (UART2_RxDataAvail()) // a byte is available in the UART RX register
    {
        rxb = UART2_getch(); // capture it

        if (rxb == ASCII_CR) // CR code -- got complete command line
        {
            UART2_putstr("\r\n"); // Echo CR + LF
            str[index] = 0; // add NUL terminator
            index = 0; // reset index for next line input
            count = 0;
            status = TRUE;
        }
        else if (rxb >= SPACE && count < maxlen) // printable char
        {
            UART2_putch(rxb); // echo rxb back to user
            str[index] = rxb;
            index++;
            count++;
        }
        else if (rxb == ASCII_BS && count != 0) // Backspace
        {
            UART2_putch(ASCII_BS); // erase offending char
            UART2_putch(SPACE);
            UART2_putch(ASCII_BS); // re-position cursor
            index--; // remove last char in buffer
            count--;
        }
        else if (rxb == ASCII_CAN || rxb == ASCII_ESC) // Cancel line
        {
            UART2_putstr(" ^X\n");
            UART2_putstr("> "); // prompt
            index = 0;
            count = 0;
        }
    }

    return status;
}


// Kernel call-back function... Does nothing in this application.
//

void BackgroundTaskExec()
{
    ; // do nothing
}
