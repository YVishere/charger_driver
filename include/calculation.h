#ifndef __CALCULATION_H
#define __CALCULATION_H

#include "math.h"
#include "const.h"

//constants
#define R_CURR_CALC     0.1
#define V_DIVIDER       16.8/3        //16.8V -> 3V ; Vx_HS/LS = V_x_x-4 * 3/16.8

float calculateVoltageDiff(float voltageHigh, float voltageLow);
float calculateCurrent(float voltageHigh, float voltageLow);

#endif