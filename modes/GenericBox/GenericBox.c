#include "GenericBox.h"

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
    // TODO: ModX = walk, ModY = tiptoe
    if ((gpio_get(INPUT_DOWN) == 0) && (gpio_get(INPUT_UP) == 1)) // Down && not Up
    {
        if ((gpio_get(INPUT_LEFT) == 0) && (gpio_get(INPUT_RIGHT) >= 1)) // Left && not Right
        {
            if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) >= 1)) // MODX && not MODY
            {

            }
            else if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) >= 1)) // Not MODX && MODY
            {

            }
            else if ((gpio_get(INPUT_MODX) == 0) && (gpio_get(INPUT_MODY) == 0)) // MODX && MODY
            {

            }
            else // Neither MODX nor MODY
            {

            }
        }
        else if ((gpio_get(INPUT_LEFT) >= 1) && (gpio_get(INPUT_RIGHT) == 0)) // Not Left && Right
        {

        }
        else    // Either Left && Right or not Left && not Right
        {

        }
    }
    else if ((gpio_get(INPUT_DOWN) == 1) && (gpio_get(INPUT_UP) == 0)) // Not Down && Up
    {
        
    }
    else // Neither up nor down or both at once
    {

    }
}