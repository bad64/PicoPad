#include "i2c.h"

#define WRITE_DELAY_US  20000
#define READ_DELAY_US   20000

uint8_t i2cStateMachine(I2C_STATE state, uint8_t* i2cDataBuf)
{
    static uint8_t retval = 0;
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

    if (state == I2C_INIT_PART1)
    {
        retval = i2c_write_timeout_us(i2c0, 0x52, i2cCommandArray[0], 2, false, WRITE_DELAY_US);

        if (retval == 2)
        {
            state = I2C_INIT_PART1_WAIT;
            sleep_us(READ_DELAY_US);
        }
        else if (retval == PICO_ERROR_GENERIC)
        {
            return PICO_ERROR_GENERIC;
        }
        else if (retval == PICO_ERROR_TIMEOUT)
        {
            return PICO_ERROR_TIMEOUT;
        }
    }
    else if (state == I2C_INIT_PART1_WAIT)
    {
        bytesToRead = i2c_get_read_available(i2c0);
        if (bytesToRead != 0)
        {
            retval = i2c_read_timeout_us(i2c0, 0x52, i2cDataBuf, bytesToRead, false, READ_DELAY_US);

            if (retval == 6)
            {
                state = I2C_INIT_PART2_WAIT;
                sleep_us(READ_DELAY_US);
            }
            else if (retval == PICO_ERROR_GENERIC)
            {
                return PICO_ERROR_GENERIC;
            }
            else if (retval == PICO_ERROR_TIMEOUT)
            {
                return PICO_ERROR_TIMEOUT;
            }

            state = I2C_INIT_PART2;
        }
    }
    else if (state == I2C_INIT_PART2)
    {
        retval = i2c_write_timeout_us(i2c0, 0x52, i2cCommandArray[1], 2, false, WRITE_DELAY_US);

        if (retval == 2)
        {
            state = I2C_LOOP_READ;
            sleep_us(READ_DELAY_US);
        }
        else if (retval == PICO_ERROR_GENERIC)
        {
            return PICO_ERROR_GENERIC;
        }
        else if (retval == PICO_ERROR_TIMEOUT)
        {
            return PICO_ERROR_TIMEOUT;
        }
    }
    else if (state == I2C_INIT_PART2_WAIT)
    {
        bytesToRead = i2c_get_read_available(i2c0);
        if (bytesToRead != 0)
        {
            retval = i2c_read_timeout_us(i2c0, 0x52, i2cDataBuf, bytesToRead, false, READ_DELAY_US);

            if (retval == 6)
            {
                state = I2C_INIT_PART2_WAIT;
                sleep_us(READ_DELAY_US);
            }
            else if (retval == PICO_ERROR_GENERIC)
            {
                return PICO_ERROR_GENERIC;
            }
            else if (retval == PICO_ERROR_TIMEOUT)
            {
                return PICO_ERROR_TIMEOUT;
            }

            state = I2C_INIT_PART2;
        }
    }
    else if (state == I2C_LOOP_READ)
    {
        retval = i2c_write_timeout_us(i2c0, 0x52, i2cCommandArray[3], 1, false, WRITE_DELAY_US);

        if (retval == 1)
        {
            state = I2C_LOOP_READ;
            sleep_us(READ_DELAY_US);
        }
        else if (retval == PICO_ERROR_GENERIC)
        {
            return PICO_ERROR_GENERIC;
        }
        else if (retval == PICO_ERROR_TIMEOUT)
        {
            return PICO_ERROR_TIMEOUT;
        }
    }
    else if (state == I2C_LOOP_READ_WAIT)
    {
        bytesToRead = i2c_get_read_available(i2c0);
        if (bytesToRead != 0)
        {
            retval = i2c_read_timeout_us(i2c0, 0x52, i2cDataBuf, bytesToRead, false, READ_DELAY_US);

            if (retval == 6)
            {
                state = I2C_INIT_PART2_WAIT;
                sleep_us(READ_DELAY_US);
            }
            else if (retval == PICO_ERROR_GENERIC)
            {
                return PICO_ERROR_GENERIC;
            }
            else if (retval == PICO_ERROR_TIMEOUT)
            {
                return PICO_ERROR_TIMEOUT;
            }

            state = I2C_INIT_PART2;
        }
    }

    return 0;
}