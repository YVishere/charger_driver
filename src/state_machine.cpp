#include "state_machine.h"

volatile state_t state = IDLE;

void initStateMachine() {
    //initialize digital pins
    initDigital();
    //initialize analog pins
    initAnalog();
}

state_t getStateMachineState() {
    return state;
}

void stateMachine() {
    switch (state) {
        case SOH_READING:
            //read the state of health
            break;
        case CONST_CURR:
            //constant current mode
            break;
        case CONST_VOLT:
            //constant voltage mode
            break;
        default:
            //IDLE state
            break;
    }
}