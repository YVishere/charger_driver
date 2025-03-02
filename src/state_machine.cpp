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

//SOH and SOC var
volatile float soh = -1;
volatile float soc = -1;

//Timer
STM32TimerInterrupt SOCTimer(TIM7);

void initStateMachine() {
    //initialize digital pins
    initDigital();
    //initialize analog pins
    initAnalog();
    //initialize digipot
    initDigPot();

    //initialize timer
    if (SOCTimer.attachInterrupt(100,handle_stateOfCharge)){
        printf("Timer attached\n");
    }
    else{
        printf("Timer not attached\n");
    }
}

void handle_stateOfCharge(){
    //Calculate state of charge of the battery pack based on how close the current output for the entire battery is to 0
    float sum = 0;
    for (int i = settings; i >= 0; i--) {
        float current = -calculateCurrent(readUndividedVoltage(pin_volt_HS[i]), readUndividedVoltage(pin_volt_LS[i]));
        sum += current;
    }

    soc = sum*100/MAX_CURRENT;
}

state_t getStateMachineState() {
    return state;
}

void stateMachine() {
    static bool first = true;
    switch (state) {
        case SET_SETTINGS:
            //Round Robin A2D converting on pins closer to battery
            if (readVoltage(V_8_HS) > VOLT_THRES) {
                state = VOC_MEASURE;
                settings = S_2;
                events.state_change = 1;
            }
            else if (readVoltage(V_12_HS) > VOLT_THRES) {
                state = VOC_MEASURE;
                settings = S_3;
                events.state_change = 1;
            }
            else if (readVoltage(V_16_HS) > VOLT_THRES) {
                state = VOC_MEASURE;
                settings = S_4;
                events.state_change = 1;
            }
        break;
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
                curr_across[i] = -calculateCurrent(readUndividedVoltage(pin_volt_HS[i]), readUndividedVoltage(pin_volt_LS[i]));
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

            //measure on outside of resistor
            float out_vol = readUndividedVoltage(pin_volt_HS[state - STATE_TO_INDEX_OFFSET]);
            //measure on inside of resistor
            float in_vol = readUndividedVoltage(pin_volt_LS[state - STATE_TO_INDEX_OFFSET]);

            //calculate current
            float curr = calculateCurrent(in_vol, out_vol);

            if (curr > 2){
                //Faulty battery
                state = ERROR;
                events.state_change = 1;
                events.error = 1;
            }

            if (out_vol > 4.2){
                dec_wiper(state - STATE_TO_INDEX_OFFSET);
            }
            else{
                inc_wiper(state - STATE_TO_INDEX_OFFSET);
            }
            
            if (curr < 0.1){
                state = CONST_VOLT_2S;
                events.state_change = 1;
            }
        break;
        case CONST_VOLT_2S:
            //constant voltage mode

            //measure on outside of resistor
            float out_vol = readUndividedVoltage(pin_volt_HS[state - STATE_TO_INDEX_OFFSET]);
            //measure on inside of resistor
            float in_vol = readUndividedVoltage(pin_volt_LS[state - STATE_TO_INDEX_OFFSET]);

            //calculate current
            float curr = calculateCurrent(in_vol, out_vol);

            if (curr > 2){
                //Faulty battery
                state = ERROR;
                events.state_change = 1;
                events.error = 1;
            }

            if (out_vol > 8.4){
                dec_wiper(state - STATE_TO_INDEX_OFFSET);
            }
            else{
                inc_wiper(state - STATE_TO_INDEX_OFFSET);
            }

            if (curr < 0.1){
                state = CONST_VOLT_3S;
                events.state_change = 1;
            }
        break;
        case CONST_VOLT_3S:
            //constant voltage mode

            //measure on outside of resistor
            float out_vol = readUndividedVoltage(pin_volt_HS[state - STATE_TO_INDEX_OFFSET]);
            //measure on inside of resistor
            float in_vol = readUndividedVoltage(pin_volt_LS[state - STATE_TO_INDEX_OFFSET]);

            //calculate current
            float curr = calculateCurrent(in_vol, out_vol);

            if (curr > 2){
                //Faulty battery
                state = ERROR;
                events.state_change = 1;
                events.error = 1;
            }

            if (out_vol > 12.6){
                dec_wiper(state - STATE_TO_INDEX_OFFSET);
            }
            else{
                inc_wiper(state - STATE_TO_INDEX_OFFSET);
            }

            if (curr < 0.1){
                state = CONST_VOLT_4S;
                events.state_change = 1;
            }
        break;
        case CONST_VOLT_4S:
            //constant voltage mode

            //measure on outside of resistor
            float out_vol = readUndividedVoltage(pin_volt_HS[state - STATE_TO_INDEX_OFFSET]);
            //measure on inside of resistor
            float in_vol = readUndividedVoltage(pin_volt_LS[state - STATE_TO_INDEX_OFFSET]);

            //calculate current
            float curr = calculateCurrent(in_vol, out_vol);

            if (curr > 2){
                //Faulty battery
                state = ERROR;
                events.state_change = 1;
                events.error = 1;
            }

            if (out_vol > 16.8){
                dec_wiper(state - STATE_TO_INDEX_OFFSET);
            }
            else{
                inc_wiper(state - STATE_TO_INDEX_OFFSET);
            }

            if (curr < 0.1){
                state = CHARGED;
                events.state_change = 1;
            }
        break;
        case CHARGED:
            //Verify if the battery pack is actually charged

            //If charged, last state
            //If not, go back to constant voltage 1S

            for (int i = settings; i >= 0; i--) {
                float out_vol = readUndividedVoltage(pin_volt_HS[i]);
                float in_vol = readUndividedVoltage(pin_volt_LS[i]);
                float curr = calculateCurrent(in_vol, out_vol);

                if (curr > 0.1){
                    state = CONST_VOLT_1S;
                    events.state_change = 1;
                    return;
                }
            }
        break;
        case ERROR:
            //Error state
            events.faulty_batt = 1;
        break;
        default:
            //IDLE state

            //Check if a battery is connected
            if (readUndividedVoltage(V_4_HS) > VOLT_THRES
                    || readUndividedVoltage(V_8_HS) > VOLT_THRES
                    || readUndividedVoltage(V_12_HS) > VOLT_THRES
                    || readUndividedVoltage(V_16_HS) > VOLT_THRES) {
                state = SET_SETTINGS;
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