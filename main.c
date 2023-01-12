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

#if defined(MODE_GENERICBOX)
    #include "modes/GenericBox/GenericBox.h"
    #pragma message "Using Generic Box (B0XX/Frame1) layout"
#endif

#if defined(MODE_WASDBOX)
    #include "modes/WASDBox/WasdBox.h"
    #pragma message "Using WASD-style Box layout"
#endif

#if defined(MODE_NOTSMASHSTICK)
    #include "modes/NotSmashStick/NotSmashStick.h"
    Coordinates coords;
    uint8_t coordsBufferX, coordsBufferY;
    int leverLock;
    uint64_t recalibrateButtonPressBegin, recalibrateButtonPressCounter;

    // Lever info
    #if defined(LEVER_JLM)
        #pragma message "Using NotSmashStick config w/ Sanwa JLM lever configuration template"
    #endif

    #if defined(LEVER_U360)
        #pragma message "Using NotSmashStick config w/ Ultrastik U360 lever configuration template"
    #endif

    #if defined(LEVER_NONE)
        #pragma message "Using NotSmashStick config w/ \"no lever\" configuration template"
    #endif
#endif

#if defined(MODE_I2CSTICK)
    #include "modes/I2CNunchuck/i2cStick.h"
    I2C_STATE i2cState;
    uint8_t i2cDataBuf[6];
    uint8_t i2cWatchdog;

    #pragma message "Using I2C Nunchuk box configuration template"
#endif

int main(void)
{  
    /* Setup block */
    stdio_init_all();

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

    // Bootsel
    if (gpio_get(PIN_START) == 0) reset_usb_boot(0, 0);

    #if defined(MODE_NOTSMASHSTICK)
        leverLock = 0;

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
    #endif

    #if defined(MODE_I2CSTICK)
        mode_i2c_Nunchuk = 1;

        i2c_init(i2c0, 100 * 1000); // Init i2c @ 100KHz

        gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
        gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
        gpio_pull_up(PIN_SDA);
        gpio_pull_up(PIN_SCL);
    #endif
    
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
        doButtons((dummy_report_t*)&report);

        // C-Stick
        doCStick((dummy_report_t*)&report);

        // Then directions
        doLeftStick((dummy_report_t*)&report);
        
        hid_task(report);
    }
}
