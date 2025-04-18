#include "UART_intf.h"

void initUART() {
    //Just in case since USART1 is possible on multiple pins
    Serial1.setTx(PA_9);
    Serial1.setRx(PA_10);

    //Serial1 since I am using USART1
    Serial1.begin(9600);
}

void readUART(){
    //Read the serial data
    if (Serial1.available() > 0) {
        char data = Serial1.read();
        switch (data) {
            case 's':
                Serial1.printf("State: %d\n", getStateMachineState());
                break;
            default:
                break;
        }
    }
}

void writeUART(){
    Serial1.printf("SOC: %f\n", soc);
    Serial1.printf("SOH: %f\n", soh);
}