#include "calculation.h"

float calculateVoltageDiff(float voltageHigh, float voltageLow) {
    return voltageHigh - voltageLow;
}

float calculateCurrent(float voltageHigh, float voltageLow) {
    float voltageDiff = calculateVoltageDiff(voltageHigh, voltageLow);
    return voltageDiff / R_CURR_CALC;
}