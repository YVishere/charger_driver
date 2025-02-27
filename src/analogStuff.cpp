#include "analogStuff.h"
#include "calculation.h"

void initAnalog() {
  initADC(ADC1);
}

float readVoltage(uint8_t channel) {
  //read the voltage from the ADC

  //raw ADC is in [0, 1]. Converted to voltage since it takes in 3.3V and outputs 4096 values
  float voltage = readADC(channel) * 3.3 / 4096;
  return voltage;
}

float readUndividedVoltage(uint8_t channel) {
  return readVoltage(channel) * V_DIVIDER;
}