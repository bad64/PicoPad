#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

#include "bsp/board.h"
#include "tusb.h"

#include "usb_descriptors.h"
#include "tasks.h"

#include "Analog/Analog.h"
#include "comms/i2c.h"

// Pin definitions
#define PIN_DPAD_DOWN       0
#define PIN_DPAD_UP         1
#define PIN_DPAD_LEFT       2
#define PIN_DPAD_RIGHT      3
#define PIN_HOME            4
#define PIN_SELECT          5
#define PIN_START           6
#define PIN_1P              7
#define PIN_2P              8
#define PIN_3P              9
#define PIN_4P              10
#define PIN_1K              11
#define PIN_2K              12
#define PIN_3K              13
#define PIN_4K              14
#define PIN_LS_DP           15
#define PIN_SDA             16
#define PIN_SCL             17

// GC notation
#define PIN_GC_B            PIN_1K
#define PIN_GC_A            18
#define PIN_GC_Y            PIN_4P
#define PIN_GC_X            PIN_2K
#define PIN_GC_L            PIN_SELECT  // It's a long story
#define PIN_GC_R            PIN_4K
//#define PIN_GC_ZL           PIN_UNUSED    // Does not have a physical button mapped
#define PIN_GC_ZR           PIN_3K

#define PIN_GC_CUP          PIN_2P
#define PIN_GC_CDOWN        19
#define PIN_GC_CLEFT        PIN_1P
#define PIN_GC_CRIGHT       PIN_3P

#define PIN_ANALOG_X        26
#define PIN_ANALOG_Y        27

#define PIN_GC_DATA         28

#define ADC_X               0
#define ADC_Y               1
#define ADC_B               2

// Bitmasks
#define MASK_B              0b0000000000000001
#define MASK_A              0b0000000000000010
#define MASK_Y              0b0000000000000100
#define MASK_X              0b0000000000001000
#define MASK_L              0b0000000000010000
#define MASK_R              0b0000000000100000
#define MASK_ZL             0b0000000001000000  // Unused
#define MASK_ZR             0b0000000010000000
#define MASK_START          0b0000000100000000
#define MASK_SELECT         0b0000001000000000
#define MASK_L3             0b0000010000000000  // Unused
#define MASK_R3             0b0000100000000000  // Unused
#define MASK_HOME           0b0001000000000000
#define MASK_CAPTURE        0b0010000000000000
#define MASK_UNUSED1        0b0100000000000000
#define MASK_UNUSED2        0b1000000000000000

// Hat
#define HAT_NEUTRAL         -1
#define HAT_UP              0
#define HAT_UP_RIGHT        1
#define HAT_RIGHT           2
#define HAT_DOWN_RIGHT      3
#define HAT_DOWN            4
#define HAT_DOWN_LEFT       5
#define HAT_LEFT            6
#define HAT_UP_LEFT         7

// Mode masks
#define MODE_SPLIT_DPAD     0b0001
#define MODE_I2C_NUNCHUK    0b0010
#define MODE_UNUSED1        0b0100
#define MODE_UNUSED2        0b1000

uint8_t mode;

// Debug stuff
// NOTE: The Pico is powerful, yes, but *maybe* don't run all the tests at the same time
//#define BUTTONS_DEBUG
//#define HAT_DEBUG
//#define ANALOG_DEBUG
//#define CSTICK_DEBUG

#if defined(BUTTONS_DEBUG) || defined(HAT_DEBUG) || defined(ANALOG_DEBUG) || defined(CSTICK_DEBUG)
    uint16_t delay = 2048;
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

i2c_inst_t* i2c;
uint8_t i2cDataBuf[6];

int main(void)
{  
    /* Setup block */
    stdio_init_all();
    mode = 0;

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
    // Physical Dpad
    if (gpio_get(PIN_GC_B) == 0) mode |= MODE_SPLIT_DPAD;
    // i2c Nunchuk mode
    if (gpio_get(PIN_START) == 0)
    {
        mode |= MODE_I2C_NUNCHUK;

        i2c_init(i2c0, 100 * 1000); // Init i2c @ 100KHz

        gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
        gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
        gpio_pull_up(PIN_SDA);
        gpio_pull_up(PIN_SCL);

        initNunchuk(i2c0);
    }

    // Init analog pins
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);

    // Init analog struct
    initCoordsStruct(&coords);

    // Init USB
    board_init();
    tusb_init();

    /* Loop block */
    while (1)
    {
        tud_task();

        // Read input into appropriate struct
        // Buttons first
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
            if (gpio_get(PIN_1K) == 0) report.buttons |= MASK_B;
            else if (gpio_get(PIN_1K) >= 1) report.buttons &= ~MASK_B;

            if (gpio_get(PIN_GC_A) == 0) report.buttons |= MASK_A;
            else if (gpio_get(PIN_1K) >= 1) report.buttons &= ~MASK_A;

            if (gpio_get(PIN_GC_Y) == 0) report.buttons |= MASK_Y;
            else if (gpio_get(PIN_1K) >= 1) report.buttons &= ~MASK_Y;

            if (gpio_get(PIN_GC_X) == 0) report.buttons |= MASK_X;
            else if (gpio_get(PIN_1K) >= 1) report.buttons &= ~MASK_X;

            if (gpio_get(PIN_GC_L) == 0) report.buttons |= MASK_L;
            else if (gpio_get(PIN_1K) >= 1)report.buttons &= ~MASK_L;

            if (gpio_get(PIN_GC_R) == 0) report.buttons |= MASK_R;
            else if (gpio_get(PIN_1K) >= 1) report.buttons &= ~MASK_R;

            if (gpio_get(PIN_GC_ZR) == 0) report.buttons |= MASK_ZR;
            else if (gpio_get(PIN_1K) >= 1) report.buttons &= ~MASK_ZR;

            if (gpio_get(PIN_START) == 0) report.buttons |= MASK_START;
            else if (gpio_get(PIN_1K) >= 1) report.buttons &= ~MASK_START;

            if (gpio_get(PIN_SELECT) == 0) report.buttons |= MASK_SELECT;
            else if (gpio_get(PIN_1K) >= 1) report.buttons &= ~MASK_SELECT;
        #endif

        // Handling the left stick
        if (mode &= MODE_SPLIT_DPAD)
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
            updateCoordinates(&coords);
            if (gpio_get(PIN_LS_DP) == 0)
            {
                // D-Pad
                report.x = 127;     // Center analog
                report.y = 127;

                #if defined(HAT_DEBUG)
                    if (hatCounter == delay)
                    {
                        hatCounter = 0;
                        report.hat++;
                    }
                    else hatCounter++;
                #else
                    if (((coords.polar.deg > 337.5) && (coords.polar.deg < 360)) || ((coords.polar.deg >= 0) && (coords.polar.deg < 22.5))) report.hat = HAT_LEFT;
                    else if ((coords.polar.deg >= 22.5) && (coords.polar.deg < 67.5)) report.hat = HAT_UP_LEFT;
                    else if ((coords.polar.deg >= 67.5) && (coords.polar.deg < 112.5)) report.hat = HAT_UP;
                    else if ((coords.polar.deg >= 112.5) && (coords.polar.deg < 157.5)) report.hat = HAT_UP_RIGHT;
                    else if ((coords.polar.deg >= 157.5) && (coords.polar.deg < 202.5)) report.hat = HAT_RIGHT;
                    else if ((coords.polar.deg >= 202.5) && (coords.polar.deg < 247.5)) report.hat = HAT_DOWN_RIGHT;
                    else if ((coords.polar.deg >= 247.5) && (coords.polar.deg < 292.5)) report.hat = HAT_DOWN;
                    else if ((coords.polar.deg >= 292.5) && (coords.polar.deg < 337.5)) report.hat = HAT_DOWN_LEFT;
                    else report.hat = HAT_NEUTRAL;
                #endif
            }
            else
            {
                // Analog
                report.hat = -1;    // Center D-Pad

                #if defined(ANALOG_DEBUG)
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

                    if (analogCounter == delay)
                    {
                        if (analogIdx + 1 < 9) analogIdx++;
                        else analogIdx = 0;
                        analogCounter = 0;
                    }
                    else analogCounter++;

                    report.x = testcoords[analogIdx].x;
                    report.y = testcoords[analogIdx].y;
                #else
                    if ((mode & MODE_I2C_NUNCHUK) == 1)
                    {
                        static int retval;
                        retval = readNunchuk(i2c, i2cDataBuf);
                        if (retval == 0)
                        {
                            //report.x = i2cDataBuf[0];
                            //report.y = i2cDataBuf[1];
                            report.x = 191;
                            report.y = 127;
                        }
                        else
                        {
                            // How to define error codes without UART or a pin-controlled LED 101
                            retval = retval * -1;
                            switch (retval)
                            {
                                case 1:
                                    report.x = 0;
                                    report.y = 0;
                                    break;
                                case 2:
                                    report.x = 127;
                                    report.y = 0;
                                    break;
                                case 3:
                                    report.x = 255;
                                    report.y = 0;
                                    break;
                                case 4:
                                    report.x = 0;
                                    report.y = 127;
                                    break;
                                case 5:
                                    // Might want to not use that one
                                    report.x = 127;
                                    report.y = 127;
                                    break;
                                case 6:
                                    report.x = 255;
                                    report.y = 127;
                                    break;
                                case 7:
                                    report.x = 0;
                                    report.y = 255;
                                    break;
                                case 8:
                                    report.x = 127;
                                    report.y = 255;
                                    break;
                                case 9:
                                    report.x = 255;
                                    report.y = 255;
                                    break;
                                default:
                                    report.x = 191;
                                    report.y = 191;
                                    break;
                            }
                        }

                        // TODO: Z && C ?
                    }
                    else if ((mode & MODE_I2C_NUNCHUK) == 0)
                    {
                        report.x = (uint8_t)coords.x;
                        report.y = (uint8_t)coords.y;
                    }
                    else    // Should not happen
                    {
                        report.x = 127;
                        report.y = 127;
                    }
                #endif
            }
        }

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
                report.rz = 255;

                if ((gpio_get(PIN_GC_CLEFT) == 0) && (gpio_get(PIN_GC_CRIGHT) == 1)) report.z = 0;
                else if ((gpio_get(PIN_GC_CLEFT) == 1) && (gpio_get(PIN_GC_CRIGHT) == 0)) report.z = 255;
                else report.z = 127;
            }
            else if ((gpio_get(PIN_GC_CUP) == 1) && (gpio_get(PIN_GC_CDOWN) == 0)) // C-Up & not C-Down
            {
                report.rz = 0;

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

        // Pack into the relevant structs and send to host
        hid_task(report);
    }
}
