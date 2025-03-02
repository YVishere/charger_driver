#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <Arduino.h>
#include "const.h"
#include "digitalStuff.h"
#include "analogStuff.h"
#include "calculation.h"
#include "digiPot.h"
#include "STM32TimerInterrupt_Generic.h"

void initStateMachine();
state_t getStateMachineState();
void stateMachine();

extern volatile float soh;
extern volatile float soc;

#endif