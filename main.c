#include <config/config.h>
#include "modes/FGC/FGC.h"

#if defined(MODE_GENERICBOX_18_BUTTONS) || defined(MODE_GENERICBOX_20_BUTTONS) || defined(MODE_WASDBOX)
    #include "modes/GenericBox/GenericBox.h"
#endif

#if defined(MODE_ANALOGSTICK)
    #include "modes/AnalogLever/NotSmashStick.h"
#endif

#if defined(MODE_I2CSTICK)
    #include "modes/Nunchuk/NunchukBox.h"
#endif

// Some warning suppression
#ifndef INPUT_A
    #define INPUT_A         -1
#endif

#ifndef INPUT_START
    #define INPUT_START     -1
#endif

// Debug stuff
int retval; // Old reliable !
void haltCatchFire(const char* msg, int errcode)
{
    // TODO: Find a better way to panic
    while(1) {}
}

// Useful variables
pokken_controller_report_t report;
bool fgcMode = false;

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

    #if defined(MODE_I2CSTICK)
        i2c_init(i2c0, 100 * 1000); // Init i2c @ 100KHz

        gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
        gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
        gpio_pull_up(PIN_SDA);
        gpio_pull_up(PIN_SCL);

        I2C_STATE i2cState;
        uint8_t i2cDataBuf[6];
        uint8_t i2cWatchdog;
    #else
        gpio_init(16);
        gpio_set_dir(16, GPIO_IN);
        gpio_pull_up(16);

        gpio_init(17);
        gpio_set_dir(17, GPIO_IN);
        gpio_pull_up(17);
    #endif

    for (int i = 18, i < 23; i++)
    {
        gpio_init(i);
        gpio_set_dir(i, GPIO_IN);
        gpio_pull_up(i);
    }

    #if defined(MODE_ANALOGSTICK)
        // Init analog pins
        adc_init();
        adc_gpio_init(26);
        adc_gpio_init(27);
        adc_gpio_init(28);

        sleep_ms(50);

        Coordinates coords;
        retval = initCoordsStruct(&coords);
        if (retval != 0) haltCatchFire("Error initializing coordinates struct !", retval); 
    #else
        // We don't need analog, let them be normal I/O pins
        gpio_init(26);
        gpio_set_dir(26, GPIO_IN);
        gpio_pull_up(26);

        gpio_init(27);
        gpio_set_dir(27, GPIO_IN);
        gpio_pull_up(27);

        gpio_init(28);
        gpio_set_dir(28, GPIO_IN);
        gpio_pull_up(28);
    #endif

    // Set GC output line
    //gpio_set_dir(29, GPIO_OUT);

    // Bootsel
    if (gpio_get(INPUT_START) == 0) reset_usb_boot(0, 0);

    // FGC mode ?
    if (gpio_get(INPUT_A) == 0) fgcMode = true;
    
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
        if (fgcMode == true) report.buttons = doButtonsFGC();
        else report.buttons = doButtons();

        // C-Stick
        if (fgcMode == true) { ;; }     // FGC don't need no right stick yo
        else
        {
           uint16_t cstick = doCStick();
           report.z = (uint8_t)(cstick >> 8);
           report.rz = (uint8_t)(cstick & 0xFF);
        }

        // Then directions
        #if defined(MODE_GENERICBOX_18_BUTTONS) || defined(MODE_GENERICBOX_20_BUTTONS) || defined(MODE_WASDBOX)
            if (fgcMode == true)
            {
                // Center the analog, we d-pad now
                report.x = NEUTRAL;
                report.y = NEUTRAL;
                    
                report.hat = doLeftStickFGC_AllButtons();
            }
            else
            {
                if (gpio_get(INPUT_LS_DP) == 0)
                {
                    report.x = NEUTRAL;
                    report.y = NEUTRAL;
                    
                    report.hat = doHat();
                }
                else
                {
                    report.hat = HAT_NEUTRAL;
                    
                    uint16_t lstick = doLeftStick();
                    report.x = (uint8_t)(lstick >> 8);
                    report.y = (uint8_t)(lstick & 0xFF);
                }
            }
        #endif
        #if defined(MODE_ANALOGSTICK)
            // TODO: Recalibration
            updateCoordinates(&coords);

            if (gpio_get(INPUT_LS_DP) == 0)
            {
                report.x = 0;
                report.y = 0;

                report.hat = doHatAnalog(coords.polar.r, coords.polar.dpadThreshhold, coords.polar.deg);
            }
            else
            {
                report.hat = HAT_NEUTRAL;

                report.x = (uint8_t)coords.x;
                report.y = (uint8_t)coords.y;
            }
        #endif
        #if defined(MODE_I2CSTICK)
            i2cWatchdog = i2cStateMachine(&i2cState, i2cDataBuf);

            if (i2cWatchdog == PICO_ERROR_GENERIC) reset_usb_boot(0, 0);
            else if (i2cWatchdog == PICO_ERROR_TIMEOUT) reset_usb_boot(0, 0);
            else
            {
                if (gpio_get(INPUT_LS_DP) == 0)
                {
                    report.x = 0;
                    report.y = 0;

                    // TODO
                }
                else
                {
                    report.hat = HAT_NEUTRAL;

                    report.x = (uint8_t)i2cDataBuf[0];
                    report.y = (uint8_t)i2cDataBuf[1];
                }
                // TODO: Button handler ?
            }
        #endif
        
        // Send to host
        hid_task(report);
    }
}
