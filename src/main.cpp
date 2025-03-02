#include <Arduino.h>
#include <Wire.h>
#include "const.h"
#include "state_machine.h"
#include "UART_intf.h"

void setup() {
  // put your setup code here, to run once:

  //Begin serial communication
  Serial.begin(115200);
  
  initStateMachine();
  initUART();

  Serial.printf("Initial State: %d\n", getStateMachineState());
}

void loop() {
  // put your main code here, to run repeatedly:
  stateMachine(); 
  writeUART();
}