#include "state_machine.h"

volatile state_t state = IDLE;
volatile events_t events = {0,0};
volatile settings_t settings = S_4;

//Hashing pins for cell voltage calculation
volatile float pin_volt_HS[4] = {V_4_HS, V_8_HS, V_12_HS, V_16_HS};
volatile float pin_volt_LS[4] = {V_4_LS, V_8_LS, V_12_LS, V_16_LS};

//Hashing current
volatile float curr_across[4] = {0, 0, 0, 0};

//Array for each cell voltage OC
volatile float cell_voltage_OC[4] = {0, 0, 0, 0};
volatile float cell_voltage_UL[4] = {0, 0, 0, 0};
volatile float max_vol_reading__soh = -1;

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
    static bool first = true;
    switch (state) {
        case VOC_MEASURE:
            // Measure and store away each cell voltage
            // Leave state when done.
            for (int i = settings; i >= 0; i--) {
                cell_voltage_OC[i] = readUndividedVoltage(pin_volt_HS[i]);
            }

            //Enable top most buck as determined by 4S/3S/2S variable during state transition
            enableTopMost();

            state = CONST_CURR;
            events.state_change = 1;
        break;
        case CONST_CURR:
            //constant current mode

            //measure on outside of resistor)
            for (int i = settings; i >= 0; i--) {
                cell_voltage_UL[i] = readUndividedVoltage(pin_volt_HS[i]);
            }

            //We will read values for SOH only on the first run
            if (first){
                soh_calc();
                first = false;
            }

            //calculate current
            for (int i = settings; i >= 0; i--) {
                curr_across[i] = -calculateCurrent(readUndividedVoltage(pin_volt_HS[i]), pin_volt_LS[i]);
            }

            for (int i = settings; i >= 0; i--) {
                if (curr_across[i] < 2){
                    uint8_t resp = inc_wiper(i);
                    if (!resp){
                        //Error
                        state = IDLE;
                        events.state_change = 1;
                        events.error = 1;
                        return;
                    }
                }  
                if (curr_across[i] > 2){
                    uint8_t resp = dec_wiper(i);
                    if (!resp){
                        //Error
                        state = IDLE;
                        events.state_change = 1;
                        events.error = 1;
                        return;
                    }
                }
            }

            //Check if we can move on to constant voltage
            int diff;
            for (int i = settings; i >= 0; i--){
                if (i == 0){
                    diff = cell_voltage_UL[i];
                }
                else {
                    diff = cell_voltage_UL[i] - cell_voltage_UL[i-1];
                }

                if (diff > VOLT_DIFF_THRESH){
                    //Done with Constant Current
                    state = CONST_VOLT_1S;
                    events.state_change = 1;
                    return;
                }
            }
            
        break;
        case CONST_VOLT_1S:
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

///////////////////
// Helper methods
//////////////////

int argMin(float *arr, int size){
    int minIndex = 0;
    float minValue = *arr;
    for (int i = 1; i < size; i++) {
        if (*(arr+i) < minValue) {
            minValue = *(arr+i);
            minIndex = i;
        }
    }
    return minIndex;
}

void enableTopMost(){
    if (settings == S_4) {
        enable3(1);
    }
    else if (settings == S_3) {
        enable2(1);
    }
    else if (settings == S_2) {
        enable1(1);
    }
}

void soh_calc(){
    //Second reading for SOH
    for (int i = settings; i >= 0; i--) {
        cell_voltage_UL[i] = readUndividedVoltage(pin_volt_LS[i]);
    }
    float diff;
    //Calculate the difference between the two readings
    for (int i = settings; i >= 0; i--) {
        diff = cell_voltage_UL[i] - cell_voltage_OC[i];
        if (diff > max_vol_reading__soh) {
            max_vol_reading__soh = diff;
        }
    }
}