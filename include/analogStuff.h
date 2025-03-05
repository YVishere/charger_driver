#ifndef __ANALOG_STUFF_H
#define __ANALOG_STUFF_H

#include "adc.h"
#include "const.h"

//Macros for better abstraction
#define V_16_HS     A0
#define V_16_LS     A3

#define V_12_HS     A6
#define V_12_LS     A2

#define V_8_HS      A5
#define V_8_LS      A1

#define V_4_HS    A4
#define V_4_LS    A7

void initAnalog();

float readVoltage(int channel);
float readUndividedVoltage(int channel);

#endif