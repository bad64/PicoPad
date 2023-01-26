#include "FGC.h"

void doButtonsFGC(dummy_report_t* report)
{
    // Punches
    if (gpio_get(INPUT_FGC_1P) == 0) report->buttons |= MASK_Y;
    else if (gpio_get(INPUT_FGC_1P) >= 1) report->buttons &= MASK_Y;

    if (gpio_get(INPUT_FGC_2P) == 0) report->buttons |= MASK_X;
    else if (gpio_get(INPUT_FGC_2P) >= 1) report->buttons &= MASK_X;

    if (gpio_get(INPUT_FGC_3P) == 0) report->buttons |= MASK_R;
    else if (gpio_get(INPUT_FGC_3P) >= 1) report->buttons &= MASK_R;

    if (gpio_get(INPUT_FGC_4P) == 0) report->buttons |= MASK_L;
    else if (gpio_get(INPUT_FGC_4P) >= 1) report->buttons &= MASK_L;

    // Kicks
    if (gpio_get(INPUT_FGC_1K) == 0) report->buttons |= MASK_B;
    else if (gpio_get(INPUT_FGC_1K) >= 1) report->buttons &= MASK_B;

    if (gpio_get(INPUT_FGC_2K) == 0) report->buttons |= MASK_A;
    else if (gpio_get(INPUT_FGC_2K) >= 1) report->buttons &= MASK_A;

    if (gpio_get(INPUT_FGC_3K) == 0) report->buttons |= MASK_ZR;
    else if (gpio_get(INPUT_FGC_3K) >= 1) report->buttons &= MASK_ZR;

    if (gpio_get(INPUT_FGC_4K) == 0) report->buttons |= MASK_ZL;
    else if (gpio_get(INPUT_FGC_4K) >= 1) report->buttons &= MASK_ZL;

    // Other
    if (gpio_get(INPUT_START) == 0) report->buttons |= MASK_START;
    else if (gpio_get(INPUT_START) >= 1) report->buttons &= MASK_START;
}

void doLeftStickFGC_AllButtons(dummy_report_t* report)
{
    report->x = NEUTRAL;
    report->y = NEUTRAL;

    if ((gpio_get(INPUT_FGC_UP) == 0 || gpio_get(PIN_A) == 0) && (gpio_get(INPUT_FGC_DOWN) >= 1))
    {
        if ((gpio_get(INPUT_FGC_LEFT) == 0) && (gpio_get(INPUT_FGC_RIGHT) >= 1)) report->hat = HAT_UP_LEFT;
        else if ((gpio_get(INPUT_FGC_LEFT) >= 1) && (gpio_get(INPUT_FGC_RIGHT) == 0)) report->hat = HAT_UP_RIGHT;
        else report->hat = HAT_UP;
    }
    else if ((gpio_get(INPUT_FGC_UP) >=1 || gpio_get(PIN_A) >= 1) && (gpio_get(INPUT_FGC_DOWN) == 0))
    {
        if ((gpio_get(INPUT_FGC_LEFT) == 0) && (gpio_get(INPUT_FGC_RIGHT) >= 1)) report->hat = HAT_DOWN_LEFT;
        else if ((gpio_get(INPUT_FGC_LEFT) >= 1) && (gpio_get(INPUT_FGC_RIGHT) == 0)) report->hat = HAT_DOWN_RIGHT;
        else report->hat = HAT_DOWN;
    }
    else
    {
        if ((gpio_get(INPUT_FGC_LEFT) == 0) && (gpio_get(INPUT_FGC_RIGHT) >= 1)) report->hat = HAT_LEFT;
        else if ((gpio_get(INPUT_FGC_LEFT) >= 1) && (gpio_get(INPUT_FGC_RIGHT) == 0)) report->hat = HAT_RIGHT;
        else report->hat = HAT_NEUTRAL;
    }
}