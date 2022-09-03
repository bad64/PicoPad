#ifndef COMMS_H
#define COMMS_H

#include <stdint.h>
#include "hardware/i2c.h"

typedef enum {
    I2C_INIT_PART1,
    I2C_INIT_PART1_WAIT,
    I2C_INIT_PART2,
    I2C_INIT_PART2_WAIT,
    I2C_LOOP_READ,
    I2C_LOOP_READ_WAIT,
} I2C_STATE;

uint8_t i2cStateMachine(I2C_STATE state, uint8_t* i2cDataBuf);

#endif