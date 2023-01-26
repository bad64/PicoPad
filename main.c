#include <config/config.h>
#include "modes/FGC/FGC.h"

#if defined(MODE_GENERICBOX_18_BUTTONS) || defined(MODE_GENERICBOX_20_BUTTONS)
    #include "modes/GenericBox/GenericBox.h"
#endif

#if defined(MODE_WASDBOX)
    #include "modes/WASDBox/WasdBox.h"
#endif

#if defined(MODE_NOTSMASHSTICK)
    #include "modes/AnalogLever/NotSmashStick.h"
#endif

#if defined(MODE_I2CSTICK)
    #include "modes/Nunchuk/NunchukBox.h"
#endif

bool fgcMode = false;

// Debug stuff
int retval; // Old reliable !
void haltCatchFire(const char* msg, int errcode)
{
    // TODO: Find a better way to panic
    while(1) {}
}

// Useful variables
pokken_controller_report_t report;

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
    #else
        gpio_init(16);
        gpio_set_dir(16, GPIO_IN);
        gpio_pull_up(16);

        gpio_init(17);
        gpio_set_dir(17, GPIO_IN);
        gpio_pull_up(17);
    #endif

    gpio_init(18);
    gpio_set_dir(18, GPIO_IN);
    gpio_pull_up(18);
    
    gpio_init(19);
    gpio_set_dir(19, GPIO_IN);
    gpio_pull_up(19);

    gpio_init(20);
    gpio_set_dir(20, GPIO_IN);
    gpio_pull_up(20);

    gpio_init(21);
    gpio_set_dir(21, GPIO_IN);
    gpio_pull_up(21);

    gpio_init(22);
    gpio_set_dir(22, GPIO_IN);
    gpio_pull_up(22);

    #if defined(MODE_NOTSMASHSTICK)
        // Init analog pins
        adc_init();
        adc_gpio_init(26);
        adc_gpio_init(27);
        adc_gpio_init(28);

        sleep_ms(50);

        // Init analog struct
        //retval = initCoordsStruct(&coords);
        //if (retval != 0)
        //{
        //    haltCatchFire("Error initializing coordinates struct !", retval);
        //}
    #else
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
        #if defined(MODE_NOTSMASHSTICK)
            doLeftStick((dummy_report_t*)&report);
        #endif
        
        // Send to host
        hid_task(report);
    }
}
