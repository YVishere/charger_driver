#include "digitalStuff.h"

volatile enableStruct enables = {0, 0, 0, 0};

void initDigital() {
    pinMode(EN0, OUTPUT);
    pinMode(EN1, OUTPUT);
    pinMode(EN2, OUTPUT);
    pinMode(EN3, OUTPUT);

    //initialize all enables to low
    digitalWrite(EN0, LOW);
    digitalWrite(EN1, LOW);
    digitalWrite(EN2, LOW);
    digitalWrite(EN3, LOW);
}

void enable0(bool enable) {
    digitalWrite(EN0, enable);
    enables.enable0 = enable;
}

void enable1(bool enable) {
    digitalWrite(EN1, enable);
    enables.enable1 = enable;
}

void enable2(bool enable) {
    digitalWrite(EN2, enable);
    enables.enable2 = enable;
}

void enable3(bool enable) {
    digitalWrite(EN3, enable);
    enables.enable3 = enable;
}