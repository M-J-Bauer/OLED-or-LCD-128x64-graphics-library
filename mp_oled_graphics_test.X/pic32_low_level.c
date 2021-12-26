/**
 * Module:   pic32_low_level.c
 *
 * Content:  Low-level I/O functions specific to the application hardware platform.
 *
 */
#include <stdlib.h>
#include <string.h>
#include "pic32_low_level.h"


void  Init_MCU_IO_ports(void)
{
    // Unless reconfigured subsequently, e.g. by a device driver ...
    TRISB = 0xFFFF;           // PORTB pins are all inputs (incl. ADC)
    TRISD = 0x0000;           // PORTD pins are all outputs
    TRISE = 0x0000;           // PORTE pins are all outputs

    // RB3:RB0 (4 pins) are configured as ADC inputs by default, whereas...
    AD1PCFGbits.PCFG8 = 1;    // RB8 is a logic input (HW.CFG0 jumper)
    AD1PCFGbits.PCFG9 = 1;    // RB9 is a logic input (HW.CFG1 jumper)
    AD1PCFG |= 0xFC00;        // RB15:RB10 are logic inputs
	
    HEARTBEAT_LED_SETUP();    // Set up output pin for LED
}


// Initialize the ADC for manual sampling mode
//
void  ADC_Init(void)
{
    AD1CON1 = 0;            // reset
    AD1CON2 = 0;            // Vref+ = AVdd, Vref- = AVss (defaults)
    AD1CON3 = 0x0002;       // Tad = 4 * Tcy
    AD1CON1 = 0x8000;       // Turn on A/D module, use manual convert
    AD1CSSL = 0;            // No scanned inputs
}


/*
 * Function:     Get hardware configuration jumper setting.
 *
 * Return val:   (uint8) H/W config jumpers = RB9:RB8 (2 bit value)
 */
uint8  GetHardwareConfig()
{
    return  (uint8) (PORTB >> 8) & 3;  // only 2 bits wanted
}


/*
 * Function reverses the order of bits in the byte passed as argument.
 */
uint8  ReverseOrderBits(uint8 bDat)
{
    int    i;
    uint8  revByte = 0x00;

    for (i = 0;  i < 8;  i++)
    {
        revByte <<= 1;
        if (bDat & 1) revByte |= 0x01;
        bDat >>= 1;
    }

    return  revByte;
}

