#include "GenericBox.h"
#if defined(MODE_GENERICBOX_18_BUTTONS) || defined(MODE_GENERICBOX_20_BUTTONS) || defined(MODE_WASDBOX)

#if defined(MODE_GENERICBOX_18_BUTTONS)
    #pragma message "Using Generic Box layout (18 buttons)"
#endif

#if defined(MODE_GENERICBOX_20_BUTTONS)
    #pragma message "Using Generic Box layout (20 buttons)"
#endif

#if defined(MODE_WASDBOX)
    #pragma message "Using WASD-style Box layout (20 buttons)"
#endif

uint16_t doButtons()
{
    static uint16_t buttons;

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

    if (gpio_get(INPUT_START) == 0) buttons |= MASK_START;
    else buttons &= MASK_START;

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

uint8_t doHat()
{
    uint8_t hat = HAT_NEUTRAL;

    if ((gpio_get(INPUT_UP) == 0) && (gpio_get(INPUT_DOWN) >= 1))
    {
        if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1)) hat = HAT_UP_LEFT;
        else if ((gpio_get(INPUT_LEFT) >= 1) && (gpio_get(INPUT_RIGHT) == 0)) hat = HAT_UP_RIGHT;
        else hat = HAT_UP;
    }
    else if ((gpio_get(INPUT_UP) >= 1) && (gpio_get(INPUT_DOWN) == 0))
    {
        if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1)) hat = HAT_DOWN_LEFT;
        else if ((gpio_get(INPUT_LEFT) >= 1) && (gpio_get(INPUT_RIGHT) == 0)) hat = HAT_DOWN_RIGHT;
        else hat = HAT_DOWN;
    }
    else
    {
        if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1)) hat = HAT_LEFT;
        else if ((gpio_get(INPUT_LEFT) >= 1) && (gpio_get(INPUT_RIGHT) == 0)) hat = HAT_RIGHT;
        else hat = HAT_NEUTRAL;
    }

    return hat;
}

uint16_t doLeftStick()
{
    uint8_t x = 0;
    uint8_t y = 0;
    uint16_t lstick = 0;
    
    if ((gpio_get(INPUT_MODX) >= 1) && (gpio_get(INPUT_MODY) >= 1))         // No modifiers
    {
        if ((gpio_get(INPUT_UP) == 0) && (gpio_get(INPUT_DOWN) >= 1))           // Up && not Down
        {
            if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))        // Left && not Right
            {
                y = UP_MAX;
                x = LEFT_MAX;
            }
            else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))   // Not Left && Right
            {
                y = UP_MAX;
                x = RIGHT_MAX;
            }
            else                                                                    // (Left && Right) || (not Left && not Right)
            {
                y = UP_MAX;
                x = NEUTRAL;
            }
        }
        else if ((gpio_get(INPUT_UP) >= 1) && (gpio_get(INPUT_DOWN) == 0))      // Not up && Down
        {
            if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = DOWN_MAX;
                x = LEFT_MAX;
            }
            else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = DOWN_MAX;
                x = RIGHT_MAX;
            }
            else
            {
                y = DOWN_MAX;
                x = NEUTRAL;
            }
        }
        else                                                                    // (Up && Down) || (not Up && not Down)
        {
            if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = NEUTRAL;
                x = LEFT_MAX;
            }
            else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = NEUTRAL;
                x = RIGHT_MAX;
            }
            else
            {
                y = NEUTRAL;
                x = NEUTRAL;
            }
        }
    }
    else if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) >= 1))      // ModX = walking speed
    {
        if ((gpio_get(INPUT_UP) == 0) && (gpio_get(INPUT_DOWN) >= 1))
        {
            if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = UP_HALF;
                x = LEFT_HALF;
            }
            else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = UP_HALF;
                x = RIGHT_HALF;
            }
            else
            {
                y = UP_HALF;
                x = NEUTRAL;
            }
        }
        else if ((gpio_get(INPUT_UP) >= 1) && (gpio_get(INPUT_DOWN) == 0))
        {
            if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = DOWN_HALF;
                x = LEFT_HALF;
            }
            else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = DOWN_HALF;
                x = RIGHT_HALF;
            }
            else
            {
                y = DOWN_HALF;
                x = NEUTRAL;
            }
        }
        else
        {
            if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = NEUTRAL;
                x = LEFT_HALF;
            }
            else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = NEUTRAL;
                x = RIGHT_HALF;
            }
            else
            {
                y = NEUTRAL;
                x = NEUTRAL;
            }
        }
    }
    else if ((gpio_get(INPUT_MODX) >= 1) && (gpio_get(INPUT_MODY) == 0))    // ModY = tiptoeing speed
    {
        if ((gpio_get(INPUT_UP) == 0) && (gpio_get(INPUT_DOWN) >= 1))
        {
            if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = UP_MIN;
                x = LEFT_MIN;
            }
            else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = UP_MIN;
                x = RIGHT_MIN;
            }
            else
            {
                y = UP_MIN;
                x = NEUTRAL;
            }
        }
        else if ((gpio_get(INPUT_UP) >= 1) && (gpio_get(INPUT_DOWN) == 0))
        {
            if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = DOWN_MIN;
                x = LEFT_MIN;
            }
            else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = DOWN_MIN;
                x = RIGHT_MIN;
            }
            else
            {
                y = DOWN_MIN;
                x = NEUTRAL;
            }
        }
        else
        {
            if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = NEUTRAL;
                x = LEFT_MIN;
            }
            else if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1))
            {
                y = NEUTRAL;
                x = RIGHT_MIN;
            }
            else
            {
                y = NEUTRAL;
                x = NEUTRAL;
            }
        }
    }
    else if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) == 0))    // TODO: Is that even a valid state to begin with ?
    {

    }

    lstick |= (x << 8);
    lstick |= y;
    return lstick;
}

#endif