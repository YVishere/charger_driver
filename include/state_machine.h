#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <Arduino.h>
#include "const.h"
#include "digitalStuff.h"
#include "analogStuff.h"

void initStateMachine();
state_t getStateMachineState();
void stateMachine();

#endif