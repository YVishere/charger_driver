#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <Arduino.h>
#include "const.h"


// State machine states
typedef enum {
    IDLE,
    SOH_READING,
    CONST_CURR,
    CONST_VOLT,
};

#endif