#include <config/config.h>

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

    gpio_init(18);
    gpio_set_dir(18, GPIO_IN);
    gpio_pull_up(18);
    
    gpio_init(19);
    gpio_set_dir(19, GPIO_IN);
    gpio_pull_up(19);

    // Set GC output line
    //gpio_set_dir(29, GPIO_OUT);

    // Mode specific stuff
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
    #endif

    #if defined(MODE_I2CSTICK)
        i2c_init(i2c0, 100 * 1000); // Init i2c @ 100KHz

        gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
        gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
        gpio_pull_up(PIN_SDA);
        gpio_pull_up(PIN_SCL);
    #endif

    // Bootsel
    if (gpio_get(INPUT_START) == 0) reset_usb_boot(0, 0);
    
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
        #if defined(MODE_GENERICBOX_18_BUTTONS) || defined(MODE_GENERICBOX_20_BUTTONS) || defined(MODE_WASDBOX)
            doLeftStick((dummy_report_t*)&report);
        #endif
        #if defined(MODE_NOTSMASHSTICK)
            doLeftStick((dummy_report_t*)&report);
        #endif
        
        // Send to host
        hid_task(report);
    }
}
