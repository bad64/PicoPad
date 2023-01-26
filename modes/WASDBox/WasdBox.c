#include "WasdBox.h"
#if defined(MODE_WASDBOX)

#pragma message "Using WASD-style Box layout"

uint16_t doButtons()
{
    uint16_t buttons = 0;

    if (gpio_get(INPUT_A) == 0) buttons |= MASK_A;
    else if (gpio_get(INPUT_A) >= 1) buttons &= MASK_A;

    if (gpio_get(INPUT_B) == 0) buttons |= MASK_B;
    else if (gpio_get(INPUT_B) >= 1) buttons &= MASK_B;

    if (gpio_get(INPUT_X) == 0) buttons |= MASK_X;
    else if (gpio_get(INPUT_X) >= 1) buttons &= MASK_X;

    if (gpio_get(INPUT_Y) == 0) buttons |= MASK_Y;
    else if (gpio_get(INPUT_Y) >= 1) buttons &= MASK_Y;

    if (gpio_get(INPUT_R) == 0) buttons |= MASK_R;
    else if (gpio_get(INPUT_R) >= 1) buttons &= MASK_R;

    if (gpio_get(INPUT_L) == 0) buttons |= MASK_L;
    else if (gpio_get(INPUT_L) >= 1) buttons &= MASK_L;

    if (gpio_get(INPUT_ZR) == 0) buttons |= MASK_ZR;
    else if (gpio_get(INPUT_ZR) >= 1) buttons &= MASK_ZR;

    if (gpio_get(INPUT_START) == 0)
    {
        if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) >= 1)) buttons |= MASK_SELECT;
        else if ((gpio_get(INPUT_MODX) >= 1) && (gpio_get(INPUT_MODY) == 0)) buttons |= MASK_HOME;
        else if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) == 0)) buttons |= MASK_START;
        else if ((gpio_get(INPUT_MODX) >= 1) && (gpio_get(INPUT_MODY) >= 1))
        {
            buttons |= MASK_START;
            buttons |= MASK_SELECT;
        }
    }
    else if (gpio_get(INPUT_START) >= 1)
    {
        buttons &= MASK_START;
        buttons &= MASK_SELECT;
        buttons &= MASK_HOME;
    }

    // TODO: Mid && Light shields for GC ?

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

void doLeftStick(dummy_report_t* report)
{
    if (gpio_get(INPUT_LS_DP) == 0)                                         // Dpad mode
    {
            report->x = NEUTRAL;
            report->y = NEUTRAL;

            if ((gpio_get(INPUT_UP) == 0) && (gpio_get(INPUT_DOWN) >= 1))
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1)) report->hat = HAT_UP_LEFT;
                else if ((gpio_get(INPUT_LEFT) >= 1) && (gpio_get(INPUT_RIGHT) == 0)) report->hat = HAT_UP_RIGHT;
                else report->hat = HAT_UP;
            }
            else if ((gpio_get(INPUT_UP) >= 1) && (gpio_get(INPUT_DOWN) == 0))
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1)) report->hat = HAT_DOWN_LEFT;
                else if ((gpio_get(INPUT_LEFT) >= 1) && (gpio_get(INPUT_RIGHT) == 0)) report->hat = HAT_DOWN_RIGHT;
                else report->hat = HAT_DOWN;
            }
            else
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1)) report->hat = HAT_LEFT;
                else if ((gpio_get(INPUT_LEFT) >= 1) && (gpio_get(INPUT_RIGHT) == 0)) report->hat = HAT_RIGHT;
                else report->hat = HAT_NEUTRAL;
            }
    }
    else if (gpio_get(INPUT_LS_DP) >= 1)                                    // Analog mode
    {
       report-> hat = HAT_NEUTRAL;
        
        if ((gpio_get(INPUT_MODX) >= 1) && (gpio_get(INPUT_MODY) >= 1))         // No modifiers
        {
            if ((gpio_get(INPUT_UP) == 0) && (gpio_get(INPUT_DOWN) >= 1))           // Up && not Down
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))        // Left && not Right
                {
                    report->y = UP_MAX;
                    report->x = LEFT_MAX;
                }
                else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))   // Not Left && Right
                {
                    report->y = UP_MAX;
                    report->x = RIGHT_MAX;
                }
                else                                                                    // (Left && Right) || (not Left && not Right)
                {
                    report->y = UP_MAX;
                    report->x = NEUTRAL;
                }
            }
            else if ((gpio_get(INPUT_UP) >= 1) && (gpio_get(INPUT_DOWN) == 0))      // Not up && Down
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = DOWN_MAX;
                    report->x = LEFT_MAX;
                }
                else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = DOWN_MAX;
                    report->x = RIGHT_MAX;
                }
                else
                {
                    report->y = DOWN_MAX;
                    report->x = NEUTRAL;
                }
            }
            else                                                                    // (Up && Down) || (not Up && not Down)
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = NEUTRAL;
                    report->x = LEFT_MAX;
                }
                else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = NEUTRAL;
                    report->x = RIGHT_MAX;
                }
                else
                {
                    report->y = NEUTRAL;
                    report->x = NEUTRAL;
                }
            }
        }
        else if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) >= 1))      // ModX = walking speed
        {
            if ((gpio_get(INPUT_UP) == 0) && (gpio_get(INPUT_DOWN) >= 1))
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = UP_HALF;
                    report->x = LEFT_HALF;
                }
                else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = UP_HALF;
                    report->x = RIGHT_HALF;
                }
                else
                {
                    report->y = UP_HALF;
                    report->x = NEUTRAL;
                }
            }
            else if ((gpio_get(INPUT_UP) >= 1) && (gpio_get(INPUT_DOWN) == 0))
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = DOWN_HALF;
                    report->x = LEFT_HALF;
                }
                else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = DOWN_HALF;
                    report->x = RIGHT_HALF;
                }
                else
                {
                    report->y = DOWN_HALF;
                    report->x = NEUTRAL;
                }
            }
            else
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = NEUTRAL;
                    report->x = LEFT_HALF;
                }
                else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = NEUTRAL;
                    report->x = RIGHT_HALF;
                }
                else
                {
                    report->y = NEUTRAL;
                    report->x = NEUTRAL;
                }
            }
        }
        else if ((gpio_get(INPUT_MODX) >= 1) && (gpio_get(INPUT_MODY) == 0))    // ModY = tiptoeing speed
        {
            if ((gpio_get(INPUT_UP) == 0) && (gpio_get(INPUT_DOWN) >= 1))
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = UP_MIN;
                    report->x = LEFT_MIN;
                }
                else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = UP_MIN;
                    report->x = RIGHT_MIN;
                }
                else
                {
                    report->y = UP_MIN;
                    report->x = NEUTRAL;
                }
            }
            else if ((gpio_get(INPUT_UP) >= 1) && (gpio_get(INPUT_DOWN) == 0))
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = DOWN_MIN;
                    report->x = LEFT_MIN;
                }
                else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = DOWN_MIN;
                    report->x = RIGHT_MIN;
                }
                else
                {
                    report->y = DOWN_MIN;
                    report->x = NEUTRAL;
                }
            }
            else
            {
                if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = NEUTRAL;
                    report->x = LEFT_MIN;
                }
                else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
                {
                    report->y = NEUTRAL;
                    report->x = RIGHT_MIN;
                }
                else
                {
                    report->y = NEUTRAL;
                    report->x = NEUTRAL;
                }
            }
        }
        else if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) == 0))    // TODO: Is that even a valid state to begin with ?
        {

        }
    }
}

#endif