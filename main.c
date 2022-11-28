#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

#include "bsp/board.h"
#include "tusb.h"

#include "usb_descriptors.h"
#include "tasks.h"

#include "config/config.h"
#include "Analog/Analog.h"
#include "comms/i2c.h"

// Modes
uint8_t mode_splitDpad;
uint8_t mode_i2c_Nunchuk;
uint8_t mode_fgc;

// Debug stuff
int retval; // Old reliable !
void haltCatchFire(const char* msg, int errcode)
{
    // TODO: Find a better way to panic
    while(1) {}
}

// Lever info
#if defined(LEVER_JLM)
    #pragma message "Using Sanwa JLM lever configuration template"
#endif

#if defined(LEVER_U360)
    #pragma message "Using Ultrastik U360 lever configuration template"
#endif

#if defined(LEVER_NONE)
    #pragma message "Using \"no lever\" configuration template"
#endif

#if defined(BUTTONS_DEBUG) || defined(HAT_DEBUG) || defined(ANALOG_DEBUG) || defined(CSTICK_DEBUG)
    uint16_t delay = 2048;
#endif

// Pinout info
#if defined(PINOUT_PROTOTYPE_BOARD)
    #pragma message "Using prototype board pinout"
#else
    #pragma message "Using final board pinout"
#endif

// Debug flags
#if defined(BUTTONS_DEBUG)
    #pragma message "Debug: Buttons"
#endif

#if defined(HAT_DEBUG)
    #pragma message "Debug: Hat"
#endif

#if defined(ANALOG_DEBUG)
    #pragma message "Debug: Analog"
#endif

#if defined(I2C_DEBUG)
    #pragma message "Debug: I2C"
#endif

#if defined(CSTICK_DEBUG)
    #pragma message "Debug: C-Stick"
#endif

#if defined(BUTTONS_DEBUG)
    int masks[8] = { MASK_B, MASK_A, MASK_Y, MASK_X, MASK_L, MASK_R, MASK_ZR, MASK_START };
    int buttons[] = { PIN_GC_B, PIN_GC_A, PIN_GC_Y, PIN_GC_X, PIN_GC_L, PIN_GC_R, PIN_GC_ZR, PIN_START };
    uint8_t buttonsIdx = 0;
    uint16_t buttonsCounter = 0;
#endif

#if defined(ANALOG_DEBUG) || defined(CSTICK_DEBUG)
    typedef struct {
        uint8_t x;
        uint8_t y;
    } testCoords;
#endif

#if defined(HAT_DEBUG)
    uint16_t hatCounter = 0;
#endif

#if defined(ANALOG_DEBUG)
    uint8_t analogIdx = 0;
    uint16_t analogCounter = 0;
#endif

#if defined(CSTICK_DEBUG)
    uint8_t cstickIdx = 0;
    uint16_t cstickCounter = 0;
#endif

// Useful variables
pokken_controller_report_t report;

Coordinates coords;
uint8_t coordsBufferX, coordsBufferY;
int leverLock;
uint64_t recalibrateButtonPressBegin, recalibrateButtonPressCounter;

I2C_STATE i2cState;
uint8_t i2cDataBuf[6];
uint8_t i2cWatchdog;

int main(void)
{  
    /* Setup block */
    stdio_init_all();
    mode_splitDpad = 0;
    mode_i2c_Nunchuk = 0;
    mode_fgc = 0;

    leverLock = 0;

    // Init GPIO
    for (int i = 0; i < 16; i++)
    {
        gpio_init(i);
        gpio_set_dir(i, GPIO_IN);
        gpio_pull_up(i);
    }

    gpio_init(18);
    gpio_set_dir(18, GPIO_IN);
    gpio_pull_up(18);
    
    gpio_init(19);
    gpio_set_dir(19, GPIO_IN);
    gpio_pull_up(19);

    // Set GC output line
    //gpio_set_dir(29, GPIO_OUT);

    /* ALTERNATE MODES */
    // Bootsel
    if (gpio_get(PIN_SELECT) == 0) reset_usb_boot(0, 0);
    // FGC Mode
    if (gpio_get(PIN_GC_A) == 0) mode_fgc = 1;
    // Physical Dpad
    if (gpio_get(PIN_GC_B) == 0) mode_splitDpad = 1;
    // i2c Nunchuk mode
    #if !defined(I2C_DEBUG)     // If the debug parameter isn't defined, default to polling START at boot
        if (gpio_get(PIN_START) == 0)
        {
            mode_i2c_Nunchuk = 1;

            i2c_init(i2c0, 100 * 1000); // Init i2c @ 100KHz

            gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
            gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
            gpio_pull_up(PIN_SDA);
            gpio_pull_up(PIN_SCL);
        }
    #else
        mode_i2c_Nunchuk = 1;

        i2c_init(i2c0, 100 * 1000); // Init i2c @ 100KHz

        gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
        gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
        gpio_pull_up(PIN_SDA);
        gpio_pull_up(PIN_SCL);
    #endif

    // Init analog pins
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);

    sleep_ms(50);

    // Init analog struct
    retval = initCoordsStruct(&coords);
    if (retval != 0)
    {
        haltCatchFire("Error initializing coordinates struct !", retval);
    }

    // Init USB
    board_init();
    tusb_init();

    // Init report struct
    report.x = 127;
    report.y = 127;
    report.z = 127;
    report.rz = 127;
    report.hat = -1;
    report.buttons = 0;

    /* Loop block */
    while (1)
    {
        tud_task();

        // Read input into appropriate struct
        // Buttons first
        if (mode_fgc == 0)
        {
            #if defined(BUTTONS_DEBUG)
                if (buttonsCounter >= delay)
                {
                    if ((report.buttons & masks[buttonsIdx]) == 0)
                    {
                        report.buttons |= masks[buttonsIdx];
                    }
                    else if ((report.buttons & masks[buttonsIdx]) >= 1)
                    {
                        report.buttons &= ~masks[buttonsIdx];

                        if (buttonsIdx + 1 < 8) buttonsIdx++;
                        else buttonsIdx = 0;
                    }

                    buttonsCounter = 0;
                }
                else buttonsCounter++;
            #else
                if (gpio_get(PIN_GC_B) == 0) report.buttons |= MASK_B;
                else if (gpio_get(PIN_GC_B) >= 1) report.buttons &= ~MASK_B;

                if (gpio_get(PIN_GC_A) == 0) report.buttons |= MASK_A;
                else if (gpio_get(PIN_GC_A) >= 1) report.buttons &= ~MASK_A;

                if (gpio_get(PIN_GC_Y) == 0) report.buttons |= MASK_Y;
                else if (gpio_get(PIN_GC_Y) >= 1) report.buttons &= ~MASK_Y;

                if (gpio_get(PIN_GC_X) == 0) report.buttons |= MASK_X;
                else if (gpio_get(PIN_GC_X) >= 1) report.buttons &= ~MASK_X;

                if (gpio_get(PIN_GC_L) == 0) report.buttons |= MASK_L;
                else if (gpio_get(PIN_GC_L) >= 1)report.buttons &= ~MASK_L;

                if (gpio_get(PIN_GC_R) == 0) report.buttons |= MASK_R;
                else if (gpio_get(PIN_GC_R) >= 1) report.buttons &= ~MASK_R;

                if (gpio_get(PIN_GC_ZR) == 0) report.buttons |= MASK_ZR;
                else if (gpio_get(PIN_GC_ZR) >= 1) report.buttons &= ~MASK_ZR;

                if (gpio_get(PIN_START) == 0) report.buttons |= MASK_START;
                else if (gpio_get(PIN_START) >= 1) report.buttons &= ~MASK_START;

                if (gpio_get(PIN_HOME) == 0) report.buttons |= MASK_HOME;
                else if (gpio_get(PIN_HOME) >= 1) report.buttons &= ~MASK_HOME;
            #endif

            // C-Stick
            #if defined(CSTICK_DEBUG)
                static testCoords testcoords[9];

                testcoords[0].y = 255;
                testcoords[0].x = 0;

                testcoords[1].y = 255;
                testcoords[1].x = 127;

                testcoords[2].y = 255;
                testcoords[2].x = 255;

                testcoords[3].y = 127;
                testcoords[3].x = 0;

                testcoords[4].y = 127;
                testcoords[4].x = 127;

                testcoords[5].y = 127;
                testcoords[5].x = 255;

                testcoords[6].y = 0;
                testcoords[6].x = 0;

                testcoords[7].y = 0;
                testcoords[7].x = 127;

                testcoords[8].y = 0;
                testcoords[8].x = 255;

                if (cstickCounter == delay)
                {
                    if (cstickIdx + 1 < 9) cstickIdx++;
                    else cstickIdx = 0;
                    cstickCounter = 0;
                }
                else cstickCounter++;

                report.z = testcoords[cstickIdx].x;
                report.rz = testcoords[cstickIdx].y;
            #else
                if ((gpio_get(PIN_GC_CUP) == 0) && (gpio_get(PIN_GC_CDOWN) == 1)) // C-Down && not C-Up
                {
                    report.rz = 0;

                    if ((gpio_get(PIN_GC_CLEFT) == 0) && (gpio_get(PIN_GC_CRIGHT) == 1)) report.z = 0;
                    else if ((gpio_get(PIN_GC_CLEFT) == 1) && (gpio_get(PIN_GC_CRIGHT) == 0)) report.z = 255;
                    else report.z = 127;
                }
                else if ((gpio_get(PIN_GC_CUP) == 1) && (gpio_get(PIN_GC_CDOWN) == 0)) // C-Up & not C-Down
                {
                    report.rz = 255;

                    if ((gpio_get(PIN_GC_CLEFT) == 0) && (gpio_get(PIN_GC_CRIGHT) == 1)) report.z = 0;
                    else if ((gpio_get(PIN_GC_CLEFT) == 1) && (gpio_get(PIN_GC_CRIGHT) == 0)) report.z = 255;
                    else report.z = 127;
                }
                else // Neither C-Up nor C-Down or both of them at once
                {
                    report.rz = 127;

                    if ((gpio_get(PIN_GC_CLEFT) == 0) && (gpio_get(PIN_GC_CRIGHT) == 1)) report.z = 0;
                    else if ((gpio_get(PIN_GC_CLEFT) == 1) && (gpio_get(PIN_GC_CRIGHT) == 0)) report.z = 255;
                    else report.z = 127;
                }
            #endif
        }
        else if (mode_fgc == 1)
        {
            if (gpio_get(PIN_GC_CLEFT) == 0) report.buttons |= MASK_1P;
            else if (gpio_get(PIN_GC_CLEFT) >= 1) report.buttons &= ~MASK_1P;

            if (gpio_get(PIN_GC_CUP) == 0) report.buttons |= MASK_2P;
            else if (gpio_get(PIN_GC_CUP) >= 1) report.buttons &= ~MASK_2P;

            if (gpio_get(PIN_GC_CRIGHT) == 0) report.buttons |= MASK_3P;
            else if (gpio_get(PIN_GC_CRIGHT) >= 1) report.buttons &= ~MASK_3P;

            if (gpio_get(PIN_GC_Y) == 0) report.buttons |= MASK_4P;
            else if (gpio_get(PIN_GC_Y) >= 1) report.buttons &= ~MASK_4P;

            if (gpio_get(PIN_GC_B) == 0) report.buttons |= MASK_1K;
            else if (gpio_get(PIN_GC_B) >= 1) report.buttons &= ~MASK_1K;

            if (gpio_get(PIN_GC_X) == 0) report.buttons |= MASK_2K;
            else if (gpio_get(PIN_GC_X) >= 1) report.buttons &= ~MASK_2K;

            if (gpio_get(PIN_GC_ZR) == 0) report.buttons |= MASK_3K;
            else if (gpio_get(PIN_GC_ZR) >= 1) report.buttons &= ~MASK_3K;

            if (gpio_get(PIN_GC_R) == 0) report.buttons |= MASK_4K;
            else if (gpio_get(PIN_GC_R) >= 1) report.buttons &= ~MASK_4K;

            if ((gpio_get(PIN_START) == 0) && (gpio_get(PIN_SELECT) == 0))
            {
                report.buttons |= MASK_HOME;
                report.buttons &= ~MASK_START;
                report.buttons &= ~MASK_SELECT;
            }
            else
            {
                report.buttons &= ~MASK_HOME;

                if (gpio_get(PIN_START) == 0) report.buttons |= MASK_START;
                else if (gpio_get(PIN_START) >= 1) report.buttons &= ~MASK_START;

                if (gpio_get(PIN_SELECT) == 0) report.buttons |= MASK_SELECT;
                else if (gpio_get(PIN_SELECT) >= 1) report.buttons &= ~MASK_SELECT;
            }

            report.z = 127;
            report.rz = 127;
        }
        // Recalibration check
        if (report.buttons & (MASK_X | MASK_Y | MASK_START))
        {
            if (recalibrateButtonPressBegin == -1)
            {
                recalibrateButtonPressBegin = time_us_64();
            }
            else
            {
                if (recalibrateButtonPressCounter == -1)
                {
                    recalibrateButtonPressCounter = recalibrateButtonPressBegin;
                }
                else
                {
                    if (recalibrateButtonPressCounter - recalibrateButtonPressBegin >= 3000000)
                    {
                        recalibrate(&coords);
                        recalibrateButtonPressBegin = -1;
                        recalibrateButtonPressCounter = -1;
                    }
                    else
                    {
                        recalibrateButtonPressCounter = time_us_64();
                    }
                }
            }
        }
        else
        {
            recalibrateButtonPressBegin = -1;
            recalibrateButtonPressCounter = -1;
        }

        // Handling the left stick
        if (mode_splitDpad == 1)
        {
            report.x = 127;
            report.y = 127;

            // TODO: SOCD modes, this is LR=N
            if ((gpio_get(PIN_DPAD_UP) == 0) && (gpio_get(PIN_DPAD_DOWN) == 1)) // Up && not Down
            {
                if ((gpio_get(PIN_DPAD_LEFT) == 0) && (gpio_get(PIN_DPAD_RIGHT) == 1)) report.hat = HAT_UP_LEFT;
                else if ((gpio_get(PIN_DPAD_LEFT) == 1) && (gpio_get(PIN_DPAD_RIGHT) == 0)) report.hat = HAT_UP_RIGHT;
                else report.hat = HAT_UP;
            }
            else if ((gpio_get(PIN_DPAD_UP) == 1) && (gpio_get(PIN_DPAD_DOWN) == 0)) // Down && not Up
            {
                if ((gpio_get(PIN_DPAD_LEFT) == 0) && (gpio_get(PIN_DPAD_RIGHT) == 1)) report.hat = HAT_DOWN_LEFT;
                else if ((gpio_get(PIN_DPAD_LEFT) == 1) && (gpio_get(PIN_DPAD_RIGHT) == 0)) report.hat = HAT_DOWN_RIGHT;
                else report.hat = HAT_DOWN;
            }
            else // Neither Up nor Down or both of them at once
            {
                if ((gpio_get(PIN_DPAD_LEFT) == 0) && (gpio_get(PIN_DPAD_RIGHT) == 1)) report.hat = HAT_LEFT;
                else if ((gpio_get(PIN_DPAD_LEFT) == 1) && (gpio_get(PIN_DPAD_RIGHT) == 0)) report.hat = HAT_RIGHT;
                else report.hat = HAT_NEUTRAL;
            }
        }
        else
        {
            if (mode_i2c_Nunchuk == 1)
            {
                i2cWatchdog = i2cStateMachine(&i2cState, i2cDataBuf);

                if (i2cWatchdog == PICO_ERROR_GENERIC)
                {
                    reset_usb_boot(0, 0);
                }
                else if (i2cWatchdog == PICO_ERROR_TIMEOUT)
                {
                    reset_usb_boot(0, 0);
                }
                else
                {
                    retval = updateCoordinatesI2C(&coords, i2cDataBuf[0], i2cDataBuf[1]);
                    if (retval != 0)
                    {
                        haltCatchFire("Error updating coordinates struct from I2C !", retval);
                    }
                }
            }
            else
            {
                retval = updateCoordinates(&coords);
                if (retval != 0)
                {
                    haltCatchFire("Error updating coordinates struct !", retval);
                }
            }
        }

        if ((gpio_get(PIN_LS_DP) == 0) || ((i2cDataBuf[5] & 0b00000010) == 0))
        {
            report.x = 127;
            report.y = 127;

            if (coords.polar.r >= coords.polar.dpadThreshhold)
            {
                if (((coords.polar.deg > 337.5) && (coords.polar.deg < 360)) || ((coords.polar.deg >= 0) && (coords.polar.deg < 22.5))) report.hat = HAT_LEFT;
                else if ((coords.polar.deg >= 22.5) && (coords.polar.deg < 67.5)) report.hat = HAT_UP_LEFT;
                else if ((coords.polar.deg >= 67.5) && (coords.polar.deg < 112.5)) report.hat = HAT_UP;
                else if ((coords.polar.deg >= 112.5) && (coords.polar.deg < 157.5)) report.hat = HAT_UP_RIGHT;
                else if ((coords.polar.deg >= 157.5) && (coords.polar.deg < 202.5)) report.hat = HAT_RIGHT;
                else if ((coords.polar.deg >= 202.5) && (coords.polar.deg < 247.5)) report.hat = HAT_DOWN_RIGHT;
                else if ((coords.polar.deg >= 247.5) && (coords.polar.deg < 292.5)) report.hat = HAT_DOWN;
                else if ((coords.polar.deg >= 292.5) && (coords.polar.deg < 337.5)) report.hat = HAT_DOWN_LEFT;
                else report.hat = HAT_NEUTRAL;
            }
            else report.hat = HAT_NEUTRAL;
        }
        else
        {
            report.hat = HAT_NEUTRAL;
            report.x = (uint8_t)coords.x;
            report.y = (uint8_t)coords.y;
        }

        // Pack into the relevant structs and send to host
        hid_task(report);
    }
}
