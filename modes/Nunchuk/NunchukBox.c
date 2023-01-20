#include "NunchukBox.h"
#if defined(MODE_I2CSTICK)

#pragma message "Using Nunchuk Box layout"

I2C_STATE state;
uint8_t i2cDataBuf[6];

void doButtons(dummy_report_t* report)
{
    if (gpio_get(INPUT_A) == 0) report->buttons |= MASK_A;
    else if (gpio_get(INPUT_A) >= 1) report->buttons &= MASK_A;

    if (gpio_get(INPUT_B) == 0) report->buttons |= MASK_B;
    else if (gpio_get(INPUT_B) >= 1) report->buttons &= MASK_B;

    if (gpio_get(INPUT_X) == 0) report->buttons |= MASK_X;
    else if (gpio_get(INPUT_X) >= 1) report->buttons &= MASK_X;

    if (gpio_get(INPUT_Y) == 0) report->buttons |= MASK_Y;
    else if (gpio_get(INPUT_Y) >= 1) report->buttons &= MASK_Y;

    if (gpio_get(INPUT_R) == 0) report->buttons |= MASK_R;
    else if (gpio_get(INPUT_R) >= 1) report->buttons &= MASK_R;

    if (gpio_get(INPUT_L) == 0) report->buttons |= MASK_L;
    else if (gpio_get(INPUT_L) >= 1) report->buttons &= MASK_L;

    if (gpio_get(INPUT_ZR) == 0) report->buttons |= MASK_ZR;
    else if (gpio_get(INPUT_ZR) >= 1) report->buttons &= MASK_ZR;

    if (gpio_get(INPUT_START) == 0)
    {
        if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) >= 1)) report->buttons |= MASK_SELECT;
        else if ((gpio_get(INPUT_MODX) >= 1) && (gpio_get(INPUT_MODY) == 0)) report->buttons |= MASK_HOME;
        else if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) == 0))
        {
            report->buttons |= MASK_START;
            report->buttons |= MASK_SELECT;
        }
        else if ((gpio_get(INPUT_MODX) >= 1) && (gpio_get(INPUT_MODY) >= 1)) report->buttons |= MASK_START;
    }
    else if (gpio_get(INPUT_START) >= 1)
    {
        report->buttons &= MASK_START;
        report->buttons &= MASK_SELECT;
        report->buttons &= MASK_HOME;
    }
}

void doCStick(dummy_report_t* report)
{
    if ((gpio_get(INPUT_C_UP) == 0) && (gpio_get(INPUT_C_DOWN) == 1)) // C-Down && not C-Up
    {
        report->rz = 0;

        if ((gpio_get(INPUT_C_LEFT) == 0) && (gpio_get(INPUT_C_RIGHT) == 1)) report->z = 0;
        else if ((gpio_get(INPUT_C_LEFT) == 1) && (gpio_get(INPUT_C_RIGHT) == 0)) report->z = 255;
        else report->z = 127;
    }
    else if ((gpio_get(INPUT_C_UP) == 1) && (gpio_get(INPUT_C_DOWN) == 0)) // C-Up & not C-Down
    {
        report->rz = 255;

        if ((gpio_get(INPUT_C_LEFT) == 0) && (gpio_get(INPUT_C_RIGHT) == 1)) report->z = 0;
        else if ((gpio_get(INPUT_C_LEFT) == 1) && (gpio_get(INPUT_C_RIGHT) == 0)) report->z = 255;
        else report->z = 127;
    }
    else // Neither C-Up nor C-Down or both of them at once
    {
        report->rz = 127;

        if ((gpio_get(INPUT_C_LEFT) == 0) && (gpio_get(INPUT_C_RIGHT) == 1)) report->z = 0;
        else if ((gpio_get(INPUT_C_LEFT) == 1) && (gpio_get(INPUT_C_RIGHT) == 0)) report->z = 255;
        else report->z = 127;
    }
}

void doLeftStick(dummy_report_t* report)
{
    // Read from i2c
    i2cStateMachine(&state, i2cDataBuf);

    if (gpio_get(INPUT_LS_DP) == 0)                                         // Dpad mode
    {
        report->x = NEUTRAL;
        report->y = NEUTRAL;
    }
    else if (gpio_get(INPUT_LS_DP) >= 1)                                    // Analog mode
    {
        report->hat = HAT_NEUTRAL;
    }
}

#endif