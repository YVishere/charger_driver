#include "calculation.h"

float calculateVoltageDiff(float voltageHigh, float voltageLow) {
    return voltageHigh - voltageLow;
}

float calculateCurrent(float voltageHigh, float voltageLow) {
    float voltageDiff = calculateVoltageDiff(voltageHigh, voltageLow);
    return voltageDiff / R_CURR_CALC;
}

int convertResistanceToTap(float resistance) {
    return (int)round(resistance * R_TAP/ MAX_DIGI_POT_RES);
}