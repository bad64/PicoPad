#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "bsp/board.h"
#include "tusb.h"

#include "usb_descriptors.h"
#include "tasks.h"

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

#define PIN_GC_A            18

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

// Struct representing an analog stick
typedef union {
    struct {
        int8_t x : 8;
        int8_t y : 8;
    } fields;
    int16_t bits;
} Analog;

Analog leftStick;

typedef union {
    struct {
        int8_t x : 8;
        int8_t y : 8;
    } fields;
    uint16_t bits;
} DigitalToAnalog;

DigitalToAnalog cStick;

// Struct representing a 16 button array
typedef union {
    struct {
        uint8_t b : 1;
        uint8_t a : 1;
        uint8_t x : 1;
        uint8_t y : 1;
        uint8_t l : 1;
        uint8_t r : 1;
        uint8_t zl : 1;
        uint8_t zr : 1;
        uint8_t start : 1;
        uint8_t select : 1;
        uint8_t home : 1;
        uint8_t screenshot : 1;
        // Last two bits are unused
    } fields;
    uint16_t bits;
} Buttons;

Buttons buttons;

int main(void)
{  
    // Setup block
    stdio_init_all();

    // Init GPIO
    for (int i = 0; i < 16; i++)
    {
        gpio_set_dir(i, GPIO_IN);
        gpio_pull_up(i);
    }
    gpio_set_dir(18, GPIO_IN);
    gpio_pull_up(18);
    gpio_set_dir(19, GPIO_IN);
    gpio_pull_up(19);

    // Set GC output line
    //gpio_set_dir(29, GPIO_OUT);

    // Init analog pins
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);

    // Init USB
    board_init();
    tusb_init();

    // Loop block
    while (1)
    {
        tud_task();

        // Read input into appropriate structs
        if ((gpio_get(PIN_GC_CUP) == 0) && (gpio_get(PIN_GC_CDOWN) == 1)) // C-Up && not C-Down
        {
            cStick.fields.y = 127;

            if ((gpio_get(PIN_GC_CLEFT) == 0) && (gpio_get(PIN_GC_CRIGHT) == 1)) cStick.fields.x = -127;
            else if ((gpio_get(PIN_GC_CLEFT) == 1) && (gpio_get(PIN_GC_CRIGHT) == 0)) cStick.fields.x = 127;
            else cStick.fields.x = 0;
        }
        else if ((gpio_get(PIN_GC_CUP) == 1) && (gpio_get(PIN_GC_CDOWN) == 0)) // C-Down && not C_Up
        {
            cStick.fields.y = -127;

            if ((gpio_get(PIN_GC_CLEFT) == 0) && (gpio_get(PIN_GC_CRIGHT) == 1)) cStick.fields.x = -127;
            else if ((gpio_get(PIN_GC_CLEFT) == 1) && (gpio_get(PIN_GC_CRIGHT) == 0)) cStick.fields.x = 127;
            else cStick.fields.x = 0;
        }
        else // Neither C-Up nor C-Down or both of them at once
        {
            cStick.fields.y = 0;

            if ((gpio_get(PIN_GC_CLEFT) == 0) && (gpio_get(PIN_GC_CRIGHT) == 1)) cStick.fields.x = -127;
            else if ((gpio_get(PIN_GC_CLEFT) == 1) && (gpio_get(PIN_GC_CRIGHT) == 0)) cStick.fields.x = 127;
            else cStick.fields.x = 0;
        }

        buttons.fields.b = !gpio_get(PIN_1K);
        buttons.fields.a = !gpio_get(PIN_GC_A);
        buttons.fields.x = !gpio_get(PIN_2K);
        buttons.fields.y = !gpio_get(PIN_4P);
        //buttons.fields.l = !gpio_get(PIN_SELECT); // How to deal with Melee people 101
        buttons.fields.r = !gpio_get(PIN_4K);
        //buttons.fields.zl = !gpio_get(PIN_UNDEFINED);
        buttons.fields.zr = !gpio_get(PIN_3K);
        
        buttons.fields.start = !gpio_get(PIN_START);
        buttons.fields.select = !gpio_get(PIN_SELECT);
        // L3 and R3 should go here
        buttons.fields.home = !gpio_get(PIN_HOME);

        // Debug thing
        buttons.fields.x = 1;

        // Pack into the relevant structs and send to host
        hid_task(leftStick.bits, cStick.bits, buttons.bits);
    }
}
