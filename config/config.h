#ifndef CONFIG_H
#define CONFIG_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

#include "bsp/board.h"
#include "tusb.h"

#include "usb_descriptors.h"
#include "tasks.h"

/* Mode definitions */
#define MODE_GENERICBOX_18_BUTTONS
//#define MODE_GENERICBOX_20_BUTTONS
//#define MODE_WASDBOX
//#define MODE_NOTSMASHSTICK
//#define MODE_I2CSTICK

#if !defined(MODE_GENERICBOX_18_BUTTONS) && !defined(MODE_GENERICBOX_20_BUTTONS) && !defined(MODE_WASDBOX) \
                && !defined(MODE_I2CSTICK) && !defined(MODE_NOTSMASHSTICK)
    #error "Please select a mode configuration in config/config.h"
#endif

/* Analog users: Select your joystick here */
#if defined(MODE_NOTSMASHSTICK)
    /* Lever selection compile-time defines */
    //#define LEVER_JLM
    //#define LEVER_U360

    /* You may want to change these to suit your personal preferences */
    #if defined(LEVER_JLM)
        #define DEADZONE            12
        #define REVERSE_DEADZONE    60
    #endif
    #if defined(LEVER_U360)
        #define DEADZONE            25
        #define REVERSE_DEADZONE    75
    #endif

    #if !defined(LEVER_JLM) && !defined(LEVER_U360)
        #error "Please select your analog lever in config/config.h"
    #endif
#endif

#endif