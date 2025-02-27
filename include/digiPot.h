#ifndef __DIGIPOT_H__
#define __DIGIPOT_H__

#include <SPI.h>
#include "const.h"

#define MHZ_10        1e7
/**
 * CMDERR will be driven low if the command is invalid 
 * To exit the error condition, the user must take the CS pin to the VIH level.
 */
#define CMD_ERR_MASK    1<<9  


/**
 *  Data is 10 bits long
 *  The data memory is only 9-bits wide, so the MSb is ignored by the device. 
 *  commands concatenate the 8 bits of the data byte with the one
 *  data bit (D8) contained in the command byte to form
 *  9-bits of data (D8:D0). The command byte format
 *  supports up to 9-bits of data so that the 8-bit resistor
 *  network can be set to full scale (100h or greater). This
 *  allows wiper connections to Terminal A and to
 *  Terminal B
 */
typedef enum{
    WIPER0_WRITE = 0x00,
    WIPER1_WRITE = 0x10,
    WIPER2_WRITE = 0x60,
    WIPER3_WRITE = 0x70,
    ERROR_WRITE = 0xFF
} WiperWriteCommand;

typedef enum {
    WIPER0_READ = 0x0C,
    WIPER1_READ = 0x1C,
    WIPER2_READ = 0x6C,
    WIPER3_READ = 0x7C,
    ERROR_READ = 0xFE
} WiperReadCommand;

void initDigPot();
uint8_t getWiper(WiperReadCommand wiper);
uint8_t setWiper(WiperWriteCommand wiper, uint8_t value);

uint8_t dec_wiper(int index);
uint8_t inc_wiper(int index);

#endif