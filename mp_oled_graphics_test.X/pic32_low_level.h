/**
 *  File:  pic32_low_level.h
 *
 *  Low-level I/O definitions specific to the application hardware platform.
 *
 */
#ifndef PIC32_LOW_LEVEL_H
#define PIC32_LOW_LEVEL_H

#include "../Common/system_def.h"
#include "HardwareProfile.h"

#ifndef ADC_INPUT_SEL
//
#define ADC_INPUT_SEL   AD1CHS              // ADC Input Channel Selector
#define ADC_SAMPLING    AD1CON1bits.SAMP    // ADC Sample/Hold-Start
#define ADC_CONV_DONE   AD1CON1bits.DONE    // ADC Status
#define ADC_RESULT_REG  ADC1BUF0            // ADC Conversion Result Buffer Register
// Boolean values for AD1PCFG bits
#define PIN_CONFIG_ANALOG    0
#define PIN_CONFIG_DIGITAL   1
//
#endif

// Macros to enable/disable audio wave sampling routine (Timer 2 ISR)...
//
#define TIMER2_IRQ_DISABLE()   IEC0bits.T2IE = 0
#define TIMER2_IRQ_ENABLE()    IEC0bits.T2IE = 1
//
#define TIMER3_IRQ_DISABLE()   IEC0bits.T3IE = 0
#define TIMER3_IRQ_ENABLE()    IEC0bits.T3IE = 1

// Macros to set EXT-CS# pin high or low  (RG9/SS2# on UEXT connector)
#define EXT_CS_HIGH()     LATGbits.LATG9 = 1
#define EXT_CS_LOW()      LATFbits.LATG9 = 0

// Macros to use PC13 & PC14 fors GPIO
#ifdef USING_CHUA_DONGLE_BOARD
#define HEARTBEAT_LED_SETUP()     TRISCbits.TRISC13 = 0
#define HEARTBEAT_LED_ON()        LATCbits.LATC13 = 1
#define HEARTBEAT_LED_OFF()       LATCbits.LATC13 = 0
#define BUTTON_PRESSED            (PORTCbits.RC14 == 0)
//
#else // Assume Olimex MX340 dev board
#define TESTPOINT_RC14_SET_HI()   LATCbits.LATC14 = 1
#define TESTPOINT_RC14_SET_LO()   LATCbits.LATC14 = 0
#define TESTPOINT_RC13_PIN        PORTCbits.RC13
#endif

// API functions defined in pic32_low_level.c
void   Init_MCU_IO_ports(void);
void   ADC_Init(void);
uint8  GetHardwareConfig();
uint8  ReverseOrderBits(uint8 bDat);

#endif  // PIC32_LOW_LEVEL_H
