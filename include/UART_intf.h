#ifndef __UART_INTF_H
#define __UART_INTF_H

#include <Arduino.h>  
#include "const.h"
#include "state_machine.h"

void initUART();
void readUART();
void writeUART();

#endif