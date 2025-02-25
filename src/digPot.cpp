/**
 *   It is recommended that while the CS pin is
 *   active, only one type of command should
 *   be issued. When changing commands, it
 *   is recommended to take the CS pin
 *   inactive then force it back to the active
 *   state.
*/

#include "digiPot.h"

//declare functions
void resetWiper0();
void resetWiper1();
void resetWiper2();
void resetWiper3();

//DigiPot supports mode 0,0 and 1,1
SPISettings settings(MHZ_10, MSBFIRST, SPI_MODE0);

//DATA_send[7:0] --> data byte, DATA_send[15:8] --> command byte
//Send data --> DATA_rec = SPI.transfer(DATA_send)
//Digital write CS pins low to send data and high to stop sending data
//Spi.beingTransaction(settings) and Spi.endTransaction() to change settings
//SPI.end to stop SPI
void initDigPot() {
    pinMode(SPI_CS, OUTPUT);
    digitalWrite(SPI_CS, HIGH);

    SPI.begin();

    resetWiper0();
    resetWiper1();
    resetWiper2();
    resetWiper3();
}

uint8_t getWiper(WiperReadCommand wiper){
    SPI.beginTransaction(settings);
    digitalWrite(SPI_CS, LOW);

    uint16_t data = (wiper << 8) | 0x00;

    uint8_t resp = SPI.transfer(data);

    digitalWrite(SPI_CS, HIGH);
    SPI.endTransaction();

    if (!(resp & CMD_ERR_MASK)) {
        //Transaction fail
        return resp;
    }

    return resp;
}


uint8_t setWiper(WiperWriteCommand wiper, uint8_t value){
    SPI.beginTransaction(settings);
    digitalWrite(SPI_CS, LOW);

    uint16_t data = (wiper << 8) | value;

    uint16_t resp = SPI.transfer(data);

    digitalWrite(SPI_CS, HIGH);
    SPI.endTransaction();

    if (resp & CMD_ERR_MASK) {
        //Transaction success
        return 1;
    }
    return 0;
}

void resetWiper0(){
    SPI.beginTransaction(settings);
    digitalWrite(SPI_CS, LOW);
    SPI.transfer(WIPER0_WRITE);
    SPI.transfer(0x00);
    digitalWrite(SPI_CS, HIGH);
    SPI.endTransaction();
}

void resetWiper1(){
    SPI.beginTransaction(settings);
    digitalWrite(SPI_CS, LOW);
    SPI.transfer(WIPER1_WRITE);
    SPI.transfer(0x00);
    digitalWrite(SPI_CS, HIGH);
    SPI.endTransaction();
}

void resetWiper2(){
    SPI.beginTransaction(settings);
    digitalWrite(SPI_CS, LOW);
    SPI.transfer(WIPER2_WRITE);
    SPI.transfer(0x00);
    digitalWrite(SPI_CS, HIGH);
    SPI.endTransaction();
}

void resetWiper3(){
    SPI.beginTransaction(settings);
    digitalWrite(SPI_CS, LOW);
    SPI.transfer(WIPER3_WRITE);
    SPI.transfer(0x00);
    digitalWrite(SPI_CS, HIGH);
    SPI.endTransaction();
}

void endSPI() {
    SPI.end();
}
