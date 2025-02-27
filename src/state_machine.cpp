#include "state_machine.h"

volatile state_t state = IDLE;
volatile events_t events = {0,0};
volatile settings_t settings = S_4;

//Hashing pins for cell voltage calculation
float pin_volt_HS[4] = {V_4_HS, V_8_HS, V_12_HS, V_16_HS};
float pin_volt_LS[4] = {V_4_LS, V_8_LS, V_12_LS, V_16_LS};

//Array for each cell voltage OC
float cell_voltage_OC[4] = {0, 0, 0, 0};

float soh_reading1;
float temp;

void initStateMachine() {
    //initialize digital pins
    initDigital();
    //initialize analog pins
    initAnalog();
    //initialize digipot
    initDigPot();
}

state_t getStateMachineState() {
    return state;
}

void stateMachine() {
    switch (state) {
        case VOC_MEASURE:
            // Measure and store away each cell voltage
            // Leave state when done.
            for (int i = settings; i >= 0; i--) {
                cell_voltage_OC[i] = readUndividedVoltage(pin_volt_HS[i]);
            }
            state = CONST_CURR;
        break;
        case CONST_CURR:
            //constant current mode
            enable3(1);

            float vol0 = readUndividedVoltage(V_4_HS);
            float vol1 = readUndividedVoltage(V_8_HS) - readUndividedVoltage(V_4_HS);
            float vol2 = readUndividedVoltage(V_12_HS) - readUndividedVoltage(V_8_HS);
            float vol3 = readUndividedVoltage(V_16_HS) - readUndividedVoltage(V_12_HS);

            if (vol0 > VOLT_THRES || vol1 > VOLT_THRES || vol2 > VOLT_THRES || vol3 > VOLT_THRES) {
                state = CONST_VOLT;
                events.state_change = 1;
            }
        break;
        case CONST_VOLT:
            //constant voltage mode
        break;
        default:
            //IDLE state
            //Round Robin A2D converting on pins closer to battery
            if (readVoltage(V_16_HS) > VOLT_THRES) {
                state = VOC_MEASURE;
                settings = S_4;
                events.state_change = 1;
            }
            else if (readVoltage(V_12_HS) > VOLT_THRES) {
                state = VOC_MEASURE;
                settings = S_3;
                events.state_change = 1;
            }
            else if (readVoltage(V_8_HS) > VOLT_THRES) {
                state = VOC_MEASURE;
                settings = S_2;
                events.state_change = 1;
            }
        break;
    }
}