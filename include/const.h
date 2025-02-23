#ifndef CONST_H
#define CONST_H

#include <Arduino.h>

//////////////
//Pin Macros
//////////////

//Digital Pins

#define UART_RX     PA_9
#define UART_TX     PA_10

#define EN0         PA_12
#define EN1         PB_0
#define EN2         PC_14
#define EN3         PC_15

#define SPI_CS      PA_11
#define SPI_MOSI    PB_5
#define SPI_MISO    PB_4
#define SCLK        PB_3

//Analog Pins
#define A0          ADC_CHANNEL_5
#define A1          ADC_CHANNEL_6
#define A2          ADC_CHANNEL_8
#define A3          ADC_CHANNEL_9
#define A4          ADC_CHANNEL_10
#define A5          ADC_CHANNEL_11
#define A6          ADC_CHANNEL_12
#define A7          ADC_CHANNEL_7

/////////////////////
//Macros for values
/////////////////////
#define VOLT_THRES          4.2
#define R_TAP               257-1     //We dont want RB to be 0 --> inf Voltage
#define MAX_DIGI_POT_RES    10e3

///////////////////////
// State machine stuff
///////////////////////
// State machine states
typedef enum {
    IDLE = 0,
    SOH_READING = 1,
    CONST_CURR = 2,
    CONST_VOLT = 3,
} state_t;

extern volatile state_t state;

#endif