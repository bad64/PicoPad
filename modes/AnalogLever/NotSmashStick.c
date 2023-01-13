#include "NotSmashStick.h"

#if defined(LEVER_JLM)
    #pragma message "Using NotSmashStick config w/ Sanwa JLM lever configuration template"
#endif

#if defined(LEVER_U360)
    #pragma message "Using NotSmashStick config w/ Ultrastik U360 lever configuration template"
#endif

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

    if (gpio_get(PIN_START) == 0) report->buttons |= MASK_START;
    else if (gpio_get(PIN_START) >= 1) report->buttons &= ~MASK_START;

    if (gpio_get(PIN_HOME) == 0) report->buttons |= MASK_HOME;
    else if (gpio_get(PIN_HOME) >= 1) report->buttons &= ~MASK_HOME;
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

void doLeftStick(dummy_report_t* report, Coordinates coords)
{
    if (gpio_get(INPUT_LS_DP) == 0)
    {
        report->x = 127;
        report->y = 127;

        if (coords.polar.r >= coords.polar.dpadThreshhold)
        {
            #if defined(LEVER_JLM)
                if (((coords.polar.deg > 337.5) && (coords.polar.deg < 360)) || ((coords.polar.deg >= 0) && (coords.polar.deg < 22.5))) report->hat = HAT_LEFT;
                else if ((coords.polar.deg >= 22.5) && (coords.polar.deg < 67.5)) report->hat = HAT_UP_LEFT;
                else if ((coords.polar.deg >= 67.5) && (coords.polar.deg < 112.5)) report->hat = HAT_UP;
                else if ((coords.polar.deg >= 112.5) && (coords.polar.deg < 157.5)) report->hat = HAT_UP_RIGHT;
                else if ((coords.polar.deg >= 157.5) && (coords.polar.deg < 202.5)) report->hat = HAT_RIGHT;
                else if ((coords.polar.deg >= 202.5) && (coords.polar.deg < 247.5)) report->hat = HAT_DOWN_RIGHT;
                else if ((coords.polar.deg >= 247.5) && (coords.polar.deg < 292.5)) report->hat = HAT_DOWN;
                else if ((coords.polar.deg >= 292.5) && (coords.polar.deg < 337.5)) report->hat = HAT_DOWN_LEFT;
            #endif
            #if defined(LEVER_U360)
            if (((coords.polar.deg > 337.5) && (coords.polar.deg < 360)) || ((coords.polar.deg >= 0) && (coords.polar.deg < 22.5))) report->hat = HAT_LEFT;
                else if ((coords.polar.deg >= 22.5) && (coords.polar.deg < 67.5)) report->hat = HAT_DOWN_LEFT;
                else if ((coords.polar.deg >= 67.5) && (coords.polar.deg < 112.5)) report->hat = HAT_DOWN;
                else if ((coords.polar.deg >= 112.5) && (coords.polar.deg < 157.5)) report->hat = HAT_DOWN_RIGHT;
                else if ((coords.polar.deg >= 157.5) && (coords.polar.deg < 202.5)) report->hat = HAT_RIGHT;
                else if ((coords.polar.deg >= 202.5) && (coords.polar.deg < 247.5)) report->hat = HAT_UP_RIGHT;
                else if ((coords.polar.deg >= 247.5) && (coords.polar.deg < 292.5)) report->hat = HAT_UP;
                else if ((coords.polar.deg >= 292.5) && (coords.polar.deg < 337.5)) report->hat = HAT_UP_LEFT;
            #endif
            else report->hat = HAT_NEUTRAL;
        }
        else report->hat = HAT_NEUTRAL;
    }
    else
    {
        report->hat = HAT_NEUTRAL;
        report->x = (uint8_t)coords.x;
        report->y = (uint8_t)coords.y;
    }
}