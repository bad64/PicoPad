#include "i2c.h"

uint8_t i2cCommands[3][2] = {{0}};

int initNunchuk(i2c_inst_t* t)
{
    // Encrypted comms
    i2cCommands[0][0] = 0xF0;
    i2cCommands[0][1] = 0x55;
    i2cCommands[1][0] = 0xFB;
    i2cCommands[1][1] = 0x00;

    if (i2c_write_blocking(t, 0x52, i2cCommands[0], 2, false) != 2) return -1;
    if (i2c_write_blocking(t, 0x52, i2cCommands[1], 2, false) != 2) return -2;

    // Unencrypted comms
    i2cCommands[2][0] = 0x40;
    i2cCommands[2][1] = 0x00;

    //if (i2c_write_blocking(t, 0x52, i2cCommands[2], 2, false) != 2) return -1;
    //if (writeNunchuk(t, i2cCommands[2][1]) != 0) return -2;

    return 0;
}

int writeNunchuk(i2c_inst_t* t, uint8_t b)
{
    static uint8_t buffer[1];
    buffer[0] = b;

    if (i2c_write_blocking(t, 0x52, buffer, 1, false) != 1) return -3;

    return 0;
}

int readNunchuk(i2c_inst_t* t, uint8_t* buffer)
{
    if (writeNunchuk(t, i2cCommands[1][1]) != 0) return -4;
    if (i2c_read_blocking(t, 0x52, buffer, 6, false) != 6) return -6;

    return 0;
}