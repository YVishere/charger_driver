#ifndef __DIGITAL_STUFF_H
#define __DIGITAL_STUFF_H

#include "const.h"

//Struct to store enables
typedef struct {
  bool enable0 : 1;
  bool enable1 : 1;
  bool enable2 : 1;
  bool enable3 : 1;
} enableStruct;

extern volatile enableStruct enables;

void initDigital();
void enable0(bool enable);
void enable1(bool enable);
void enable2(bool enable);
void enable3(bool enable);

#endif