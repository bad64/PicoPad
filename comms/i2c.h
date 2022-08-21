#ifndef COMMS_H
#define COMMS_H

#include <stdint.h>
#include "hardware/i2c.h"

int initNunchuk(i2c_inst_t* t);
int writeNunchuk(i2c_inst_t* t, uint8_t b);
int readNunchuk(i2c_inst_t* t, uint8_t* buffer);

#endif