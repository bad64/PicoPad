#ifndef CONFIG_H
#define CONFIG_H

// Switch bitmasks
#define MASK_Y              0b0000000000000001
#define MASK_B              0b0000000000000010
#define MASK_A              0b0000000000000100
#define MASK_X              0b0000000000001000
#define MASK_L              0b0000000000010000
#define MASK_R              0b0000000000100000
#define MASK_ZL             0b0000000001000000
#define MASK_ZR             0b0000000010000000
#define MASK_SELECT         0b0000000100000000
#define MASK_START          0b0000001000000000
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

/* Pin definitions */
#define PINOUT_PROTOTYPE_BOARD

#ifdef PINOUT_PROTOTYPE_BOARD
    #include "config/pinout_prototype.h"
#else
    #include "config/pinout_final.h"
#endif

/* Modes definitions */
#define MODE_GENERICBOX
//#define MODE_WASDBOX
//#define MODE_NOTSMASHSTICK
//#define MODE_I2CSTICK

#if defined(MODE_NOTSMASHSTICK)
    #include "Analog/Analog.h"

    /* Lever selection compile-time defines */
    //#define LEVER_JLM
    #define LEVER_U360
    //#define LEVER_NONE

    /* You may want to change these to suit your personal preferences */
    #if defined(LEVER_JLM)
        #define DEADZONE            12
        #define REVERSE_DEADZONE    60
    #endif
    #if defined(LEVER_U360)
        #define DEADZONE            25
        #define REVERSE_DEADZONE    75
    #endif
    #if defined(LEVER_NONE)
        #define DEADZONE            1
        #define REVERSE_DEADZONE    1
    #endif
#endif

#if defined(MODE_I2CSTICK)
    #include "comms/i2c.h"
#endif

/* Debug modes */
/* NOTE: The Pico is powerful, yes, but *maybe* don't run all the tests at the same time */
//#define BUTTONS_DEBUG
//#define HAT_DEBUG
//#define ANALOG_DEBUG
//#define I2C_DEBUG
//#define CSTICK_DEBUG

#endif