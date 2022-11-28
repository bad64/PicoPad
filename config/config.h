#ifndef CONFIG_H
#define CONFIG_H

/* Pin definitions */
#define PINOUT_PROTOTYPE_BOARD

#ifdef PINOUT_PROTOTYPE_BOARD
    #include "config/pinout_prototype.h"
#else
    #include "config/pinout_final.h"
#endif

/* Lever selection compile-time defines */
//#define LEVER_JLM
//#define LEVER_U360
#define LEVER_NONE

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

/* Debug modes */
/* NOTE: The Pico is powerful, yes, but *maybe* don't run all the tests at the same time */
//#define BUTTONS_DEBUG
//#define HAT_DEBUG
//#define ANALOG_DEBUG
#define I2C_DEBUG
//#define CSTICK_DEBUG

#endif