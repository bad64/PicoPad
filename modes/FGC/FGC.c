#include "FGC.h"

uint16_t doButtonsFGC()
{
    uint16_t buttons;

    // Punches
    if (gpio_get(INPUT_FGC_1P) == 0) buttons |= MASK_Y;
    else buttons &= MASK_Y;

    if (gpio_get(INPUT_FGC_2P) == 0) buttons |= MASK_X;
    else buttons &= MASK_X;

    if (gpio_get(INPUT_FGC_3P) == 0) buttons |= MASK_R;
    else buttons &= MASK_R;

    if (gpio_get(INPUT_FGC_4P) == 0) buttons |= MASK_L;
    else buttons &= MASK_L;

    // Kicks
    if (gpio_get(INPUT_FGC_1K) == 0) buttons |= MASK_B;
    else buttons &= MASK_B;

    if (gpio_get(INPUT_FGC_2K) == 0) buttons |= MASK_A;
    else buttons &= MASK_A;

    if (gpio_get(INPUT_FGC_3K) == 0) buttons |= MASK_ZR;
    else buttons &= MASK_ZR;

    if (gpio_get(INPUT_FGC_4K) == 0) buttons |= MASK_ZL;
    else buttons &= MASK_ZL;

    // Other
    if (gpio_get(INPUT_FGC_START) == 0) buttons |= MASK_START;
    else buttons &= MASK_START;

    return buttons;
}

uint8_t doCStickFGC()
{
    // You're a big fool !
    return 0;
}

uint8_t doLeftStickFGC_AllButtons()
{
    uint8_t hat = HAT_NEUTRAL;

    if ((gpio_get(INPUT_FGC_UP) == 0 || gpio_get(PIN_A) == 0) && (gpio_get(INPUT_FGC_DOWN) >= 1))
    {
        if ((gpio_get(INPUT_FGC_LEFT) == 0) && (gpio_get(INPUT_FGC_RIGHT) >= 1)) hat = HAT_UP_LEFT;
        else if ((gpio_get(INPUT_FGC_LEFT) >= 1) && (gpio_get(INPUT_FGC_RIGHT) == 0)) hat = HAT_UP_RIGHT;
        else hat = HAT_UP;
    }
    else if ((gpio_get(INPUT_FGC_UP) >=1 || gpio_get(PIN_A) >= 1) && (gpio_get(INPUT_FGC_DOWN) == 0))
    {
        if ((gpio_get(INPUT_FGC_LEFT) == 0) && (gpio_get(INPUT_FGC_RIGHT) >= 1)) hat = HAT_DOWN_LEFT;
        else if ((gpio_get(INPUT_FGC_LEFT) >= 1) && (gpio_get(INPUT_FGC_RIGHT) == 0)) hat = HAT_DOWN_RIGHT;
        else hat = HAT_DOWN;
    }
    else
    {
        if ((gpio_get(INPUT_FGC_LEFT) == 0) && (gpio_get(INPUT_FGC_RIGHT) >= 1)) hat = HAT_LEFT;
        else if ((gpio_get(INPUT_FGC_LEFT) >= 1) && (gpio_get(INPUT_FGC_RIGHT) == 0)) hat = HAT_RIGHT;
        else hat = HAT_NEUTRAL;
    }

    return hat;
}