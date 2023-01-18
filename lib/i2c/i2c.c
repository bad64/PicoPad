#include "i2c.h"

#define REASONABLE_TIMEOUT      20000
#define CONVERSION_DELAY_US     200

int8_t i2cStateMachine(I2C_STATE* state, uint8_t* i2cDataBuf)
{
    static int8_t retval = 0;
    static uint8_t bytesToRead;
    static uint8_t i2cCommandArray[4][2];

    i2cCommandArray[0][0] = 0xF0;
    i2cCommandArray[0][1] = 0x55;

    i2cCommandArray[1][0] = 0xFB;
    i2cCommandArray[1][1] = 0x00;

    i2cCommandArray[2][0] = 0x40;
    i2cCommandArray[2][1] = 0x00;

    i2cCommandArray[3][0] = 0x00;
    i2cCommandArray[3][1] = 0x00;

    if (*state == I2C_INIT_PART1)
    {
        // Write 0xF0, 0x55 to 0x52
        // Then wait about 10ms
        retval = i2c_write_timeout_us(i2c0, 0x52, i2cCommandArray[0], 2, false, REASONABLE_TIMEOUT);

        if (retval == 2)
        {
            *state = I2C_INIT_PART2;
            sleep_ms(10);
        }
        else if (retval == PICO_ERROR_GENERIC)
        {
            return PICO_ERROR_GENERIC;
        }
        else if (retval == PICO_ERROR_TIMEOUT)
        {
            return PICO_ERROR_TIMEOUT;
        }
        else
        {
            return retval;
        }
    }
    else if (*state == I2C_INIT_PART2)
    {
        // Write 0xFB, 0x00 to 0x52
        // Then wait about 20ms
        retval = i2c_write_blocking(i2c0, 0x52, i2cCommandArray[1], 2, false);

        if (retval == 2)
        {
            *state = I2C_LOOP_READ;
            sleep_ms(20);
        }
        else if (retval == PICO_ERROR_GENERIC)
        {
            return PICO_ERROR_GENERIC;
        }
        else if (retval == PICO_ERROR_TIMEOUT)
        {
            return PICO_ERROR_TIMEOUT;
        }
        else
        {
            return retval;
        }
    }
    else if (*state == I2C_LOOP_READ)
    {
        // Write 0x00 to 0x52
        // Then wait 200us
        retval = i2c_write_timeout_us(i2c0, 0x52, 0x00, 1, false, REASONABLE_TIMEOUT);

        if (retval == 1)
        {
            *state = I2C_LOOP_READ_WAIT;
            sleep_us(CONVERSION_DELAY_US);
        }
        else if (retval == PICO_ERROR_GENERIC)
        {
            return PICO_ERROR_GENERIC;
        }
        else if (retval == PICO_ERROR_TIMEOUT)
        {
            return PICO_ERROR_TIMEOUT;
        }
        else
        {
            return retval;
        }
    }
    else if (*state == I2C_LOOP_READ_WAIT)
    {
        // Read from 0x52
        retval = i2c_read_timeout_us(i2c0, 0x52, i2cDataBuf, 6, false, REASONABLE_TIMEOUT);

        if (retval == PICO_ERROR_GENERIC)
        {
            return PICO_ERROR_GENERIC;
        }
        else if (retval == PICO_ERROR_TIMEOUT)
        {
            return PICO_ERROR_TIMEOUT;
        }
        else
        {
            *state = I2C_LOOP_READ;
        }
    }

    return 0;
}