#include <Arduino.h>
#include <Wire.h>
#include "const.h"
#include "state_machine.h"

void setup() {
  // put your setup code here, to run once:

  //Begin serial communication
  Serial.begin(115200);
  
  initStateMachine();

  Serial.printf("Initial State: %d\n", getStateMachineState());
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  Serial.printf("Current State: %d\n", getStateMachineState()); 
}