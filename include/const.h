#ifndef CONST_H
#define CONST_H

#include <Arduino.h>

//////////////
//Pin Macros
//////////////

//Digital Pins

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
#define VOLT_DIFF_THRESH    4.15
#define VOLT_THRES          0.01
#define R_TAP               257-1     //We dont want RB to be 0 --> inf Voltage
#define MAX_DIGI_POT_RES    5e3
#define MAX_CURRENT         2

///////////////////////
// State machine stuff
///////////////////////

//Settings
typedef enum{
    S_4 = 3,
    S_3 = 2,
    S_2 = 1
} settings_t;

extern volatile settings_t settings_fmware;

// State machine states

#define STATE_TO_INDEX_OFFSET       4
typedef enum {
    IDLE = 0,
    SET_SETTINGS = 1,
    VOC_MEASURE = 2,
    CONST_CURR = 3,
    CONST_VOLT_1S = 4,
    CONST_VOLT_2S = 5,
    CONST_VOLT_3S = 6,
    CONST_VOLT_4S = 7,
    CHARGED = 8,
    ERROR_BATT = 9,
} state_t;

extern volatile state_t state;

///////////
// Events
///////////
typedef struct {
    bool state_change: 1;
    bool error:1;
    bool faulty_batt:1;
} events_t;

extern volatile events_t events;

#endif