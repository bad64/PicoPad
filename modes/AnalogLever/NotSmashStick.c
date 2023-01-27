#include "NotSmashStick.h"

#if defined(MODE_ANALOGSTICK)

#if defined(LEVER_JLM)
    #pragma message "Using ANALOGSTICK config w/ Sanwa JLM lever configuration template"
#endif

#if defined(LEVER_U360)
    #pragma message "Using ANALOGSTICK config w/ Ultrastik U360 lever configuration template"
#endif

uint16_t doButtons()
{
    static uint16_t buttons = 0;

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

    if (gpio_get(PIN_START) == 0) buttons |= MASK_START;
    else buttons &= ~MASK_START;

    if (gpio_get(PIN_HOME) == 0) buttons |= MASK_HOME;
    else buttons &= ~MASK_HOME;

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

uint8_t doHatAnalog(uint16_t r, uint16_t dpadThreshhold, double deg)
{
    uint8_t hat;

    if (r >= dpadThreshhold)
    {
        #if defined(LEVER_JLM)
            if (((deg > 337.5) && (deg < 360)) || ((deg >= 0) && (deg < 22.5))) hat = HAT_LEFT;
            else if ((deg >= 22.5) && (deg < 67.5)) hat = HAT_UP_LEFT;
            else if ((deg >= 67.5) && (deg < 112.5)) hat = HAT_UP;
            else if ((deg >= 112.5) && (deg < 157.5)) hat = HAT_UP_RIGHT;
            else if ((deg >= 157.5) && (deg < 202.5)) hat = HAT_RIGHT;
            else if ((deg >= 202.5) && (deg < 247.5)) hat = HAT_DOWN_RIGHT;
            else if ((deg >= 247.5) && (deg < 292.5)) hat = HAT_DOWN;
            else if ((deg >= 292.5) && (deg < 337.5)) hat = HAT_DOWN_LEFT;
            else hat = HAT_NEUTRAL;
        #endif
        #if defined(LEVER_U360)
        if (((deg > 337.5) && (deg < 360)) || ((deg >= 0) && (deg < 22.5))) hat = HAT_LEFT;
            else if ((deg >= 22.5) && (deg < 67.5)) hat = HAT_DOWN_LEFT;
            else if ((deg >= 67.5) && (deg < 112.5)) hat = HAT_DOWN;
            else if ((deg >= 112.5) && (deg < 157.5)) hat = HAT_DOWN_RIGHT;
            else if ((deg >= 157.5) && (deg < 202.5)) hat = HAT_RIGHT;
            else if ((deg >= 202.5) && (deg < 247.5)) hat = HAT_UP_RIGHT;
            else if ((deg >= 247.5) && (deg < 292.5)) hat = HAT_UP;
            else if ((deg >= 292.5) && (deg < 337.5)) hat = HAT_UP_LEFT;
            else hat = HAT_NEUTRAL;
        #endif
    }
    else hat = HAT_NEUTRAL;

    return hat;
}

uint16_t doLeftStick()
{
    // Unneeded, kept for compatibility/silencing warnings
    return 0;
}

#endif