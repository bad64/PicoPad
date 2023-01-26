#ifndef FGC_H
#define FGC_H

#include "../GenericModeInterface.h"

// SOCD default check
#if !defined(SOCD_LRN) && !defined(SOCD_2IP) && !defined(SOCD_LMAO)
    #define SOCD_LRN
#endif

#if defined(MODE_GENERICBOX_18_BUTTONS) || defined(MODE_GENERICBOX_20_BUTTONS) || defined(MODE_WASDBOX)
    #if defined(MODE_GENERICBOX_18_BUTTONS) || defined(MODE_GENERICBOX_20_BUTTONS)
        #define INPUT_FGC_UP    PIN_MODX
    #endif
    
    #if defined(MODE_WASDBOX)
        #define INPUT_FGC_UP    PIN_DPAD_UP
    #endif

    #define INPUT_FGC_DOWN      PIN_DPAD_DOWN
    #define INPUT_FGC_LEFT      PIN_DPAD_LEFT
    #define INPUT_FGC_RIGHT     PIN_DPAD_RIGHT
#endif

#if defined(MODE_I2CSTICK) || defined(MODE_ANALOGSTICK)
    #define INPUT_FGC_UP        -1
    #define INPUT_FGC_DOWN      -1
    #define INPUT_FGC_LEFT      -1
    #define INPUT_FGC_RIGHT     -1
#endif

#define INPUT_FGC_1P            PIN_1P
#define INPUT_FGC_2P            PIN_2P
#define INPUT_FGC_3P            PIN_3P
#define INPUT_FGC_4P            PIN_4P

#define INPUT_FGC_1K            PIN_1K
#define INPUT_FGC_2K            PIN_2K
#define INPUT_FGC_3K            PIN_3K
#define INPUT_FGC_4K            PIN_4K

#define INPUT_START             PIN_START
#define INPUT_SELECT            -1
#define INPUT_HOME              -1

// Analog values
#define NEUTRAL                 127

uint16_t doButtonsFGC();
uint8_t doLeftStickFGC_AllButtons();

#endif