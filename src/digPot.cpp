/**
 *   It is recommended that while the CS pin is
 *   active, only one type of command should
 *   be issued. When changing commands, it
 *   is recommended to take the CS pin
 *   inactive then force it back to the active
 *   state.
 * 
 *   SPI.h is a blovking call so I dont need to worry about
 *   stacking requests
 * 
*/

#include "digiPot.h"

//declare functions
void resetWiper0();
void resetWiper1();
void resetWiper2();
void resetWiper3();

// DigiPot supports mode 0,0 and 1,1
SPISettings settings(MHZ_10, MSBFIRST, SPI_MODE0);

// DATA_send[7:0] --> data byte, DATA_send[15:8] --> command byte
// Send data --> DATA_rec = SPI.transfer(DATA_send)
// Digital write CS pins low to send data and high to stop sending data
// SPI.beginTransaction(settings) and SPI.endTransaction() to change settings
// SPI.end to stop SPI
void initDigPot() {
    pinMode(SPI_CS, OUTPUT);
    digitalWrite(SPI_CS, HIGH);

    SPI.begin();

    resetWiper0();
    resetWiper1();
    resetWiper2();
    resetWiper3();
}

uint8_t getWiper(WiperReadCommand wiper) {
    SPI.beginTransaction(settings);
    digitalWrite(SPI_CS, LOW);

    // Send the command byte and read the response byte
    uint16_t data = (wiper << 8) | 0x00;
    uint16_t resp = SPI.transfer16(data);

    digitalWrite(SPI_CS, HIGH);
    SPI.endTransaction();

    // Check for command error
    if (resp & CMD_ERR_MASK) {
        // Transaction failed
        return 0;
    }

    return resp & 0xFF; // Return the lower 8 bits of the response
}

uint8_t setWiper(WiperWriteCommand wiper, uint8_t value) {
    SPI.beginTransaction(settings);
    digitalWrite(SPI_CS, LOW);

    // Concatenate the command and data bytes into a single 16-bit value
    uint16_t data = (wiper << 8) | value;

    // Send the 16-bit value
    uint16_t resp = SPI.transfer16(data);

    digitalWrite(SPI_CS, HIGH);
    SPI.endTransaction();

    // Check for command error
    if (resp & CMD_ERR_MASK) {
        // Transaction failed
        return 0;
    }

    // Transaction success
    return 1;
}

uint8_t dec_wiper(int index) {
    if (index < 0 || index > 3) {
        // Invalid index
        return 0;
    }

    uint8_t wiper = getWiper(index_to_readWiper(index));
    
    if (wiper != 0){
        setWiper(index_to_writeWiper(index), wiper - 1);
        return wiper - 1;
    }

    return 0;
}

uint8_t inc_wiper(int index) {
    if (index < 0 || index > 3) {
        // Invalid index
        return 0;
    }

    uint8_t wiper = getWiper(index_to_readWiper(index));
    
    if (wiper != 0){
        setWiper(index_to_writeWiper(index), wiper + 1);
        return wiper + 1;
    }

    return 0;
}

WiperReadCommand index_to_readWiper(int index) {
    switch (index) {
        case 3:
            return WIPER3_READ;
        case 2:
            return WIPER2_READ;
        case 1:
            return WIPER1_READ;
        case 0:
            return WIPER0_READ;
        default:
            // Invalid index
            return ERROR_READ;
    }
}

WiperWriteCommand index_to_writeWiper(int index) {
    switch (index) {
        case 3:
            return WIPER3_WRITE;
        case 2:
            return WIPER2_WRITE;
        case 1:
            return WIPER1_WRITE;
        case 0:
            return WIPER0_WRITE;
        default:
            // Invalid index
            return ERROR_WRITE;
    }
}

void resetWiper0() {
    setWiper(WIPER0_WRITE, 0x00);
}

void resetWiper1() {
    setWiper(WIPER1_WRITE, 0x00);
}

void resetWiper2() {
    setWiper(WIPER2_WRITE, 0x00);
}

void resetWiper3() {
    setWiper(WIPER3_WRITE, 0x00);
}

void endSPI() {
    SPI.end();
}