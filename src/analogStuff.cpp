#include "analogStuff.h"

void initAnalog() {
  initADC(ADC1);
}

float readVoltage(uint8_t channel) {
  //read the voltage from the ADC

  //To-do: raw ADC is in [0, 1]. Convert to voltage
  float voltage = readADC(channel);
  return voltage;
}