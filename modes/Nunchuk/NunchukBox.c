#include "NunchukBox.h"
#if defined(MODE_I2CSTICK)

#pragma message "Using Nunchuk Box layout"

uint16_t doButtons()
{
    uint16_t buttons;

    if (gpio_get(INPUT_A) == 0) buttons |= MASK_A;
    else buttons &= MASK_A;

    if (gpio_get(INPUT_B) == 0) buttons |= MASK_B;
    else buttons &= MASK_B;

    if (gpio_get(INPUT_X) == 0) buttons |= MASK_X;
    else buttons &= MASK_X;

    if (gpio_get(INPUT_Y) == 0) buttons |= MASK_Y;
    else buttons &= MASK_Y;

    if (gpio_get(INPUT_R) == 0) buttons |= MASK_R;
    else buttons &= MASK_R;

    if (gpio_get(INPUT_L) == 0) buttons |= MASK_L;
    else buttons &= MASK_L;

    if (gpio_get(INPUT_ZR) == 0) buttons |= MASK_ZR;
    else buttons &= MASK_ZR;

    if (gpio_get(INPUT_START) == 0)
    {
        if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) >= 1)) buttons |= MASK_SELECT;
        else if ((gpio_get(INPUT_MODX) >= 1) && (gpio_get(INPUT_MODY) == 0)) buttons |= MASK_HOME;
        else if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) == 0))
        {
            buttons |= MASK_START;
            buttons |= MASK_SELECT;
        }
        else if ((gpio_get(INPUT_MODX) >= 1) && (gpio_get(INPUT_MODY) >= 1)) buttons |= MASK_START;
    }
    else if (gpio_get(INPUT_START) >= 1)
    {
        buttons &= MASK_START;
        buttons &= MASK_SELECT;
        buttons &= MASK_HOME;
    }

    return buttons;
}

uint16_t doCStick()
{
    uint8_t z = 0;
    uint8_t rz = 0;
    uint16_t cstick = 0;

    if ((gpio_get(INPUT_C_UP) == 0) && (gpio_get(INPUT_C_DOWN) == 1)) // C-Down && not C-Up
    {
        rz = 0;

        if ((gpio_get(INPUT_C_LEFT) == 0) && (gpio_get(INPUT_C_RIGHT) == 1)) z = 0;
        else if ((gpio_get(INPUT_C_LEFT) == 1) && (gpio_get(INPUT_C_RIGHT) == 0)) z = 255;
        else z = 127;
    }
    else if ((gpio_get(INPUT_C_UP) == 1) && (gpio_get(INPUT_C_DOWN) == 0)) // C-Up & not C-Down
    {
        rz = 255;

        if ((gpio_get(INPUT_C_LEFT) == 0) && (gpio_get(INPUT_C_RIGHT) == 1)) z = 0;
        else if ((gpio_get(INPUT_C_LEFT) == 1) && (gpio_get(INPUT_C_RIGHT) == 0)) z = 255;
        else z = 127;
    }
    else // Neither C-Up nor C-Down or both of them at once
    {
        rz = 127;

        if ((gpio_get(INPUT_C_LEFT) == 0) && (gpio_get(INPUT_C_RIGHT) == 1)) z = 0;
        else if ((gpio_get(INPUT_C_LEFT) == 1) && (gpio_get(INPUT_C_RIGHT) == 0)) z = 255;
        else z = 127;
    }

    cstick |= (z << 8);
    cstick |= rz;
    return cstick;
}

uint16_t doLeftStick()
{
    return 0;
}

#endif