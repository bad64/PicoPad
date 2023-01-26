#ifndef GENERICBOX_H
#define GENERICBOX_H

#include "../GenericModeInterface.h"

// SOCD default check
#if !defined(SOCD_LRN) && !defined(SOCD_2IP) && !defined(SOCD_LMAO)
    #define SOCD_LRN
#endif

#define INPUT_UP        PIN_4K
#define INPUT_DOWN      PIN_DPAD_DOWN
#define INPUT_LEFT      PIN_DPAD_UP
#define INPUT_RIGHT     PIN_DPAD_RIGHT

#define INPUT_A         PIN_A
#define INPUT_B         PIN_1K
#define INPUT_X         PIN_2K
#define INPUT_Y         PIN_2P

#define INPUT_R         PIN_1P
#define INPUT_L         PIN_DPAD_LEFT
#define INPUT_ZL        -1
#define INPUT_ZR        PIN_3K

#define INPUT_C_UP      PIN_C_UP
#define INPUT_C_DOWN    PIN_C_DOWN
#define INPUT_C_LEFT    PIN_C_LEFT
#define INPUT_C_RIGHT   PIN_C_RIGHT

#define INPUT_START     PIN_START
#define INPUT_SELECT    -1
#define INPUT_HOME      -1

#define INPUT_LS_DP     PIN_LS_DP
#define INPUT_MODX      PIN_MODX
#define INPUT_MODY      PIN_MODY

#if defined(MODE_GENERICBOX_20_BUTTONS)
    #define INPUT_MS    PIN_3P
    #define INPUT_LS    PIN_4P
#endif

// Analog values
#define NEUTRAL         127
#define WALK_PERCENT    45
#define TIPTOE_PERCENT  10

#define UP_MAX          0;
#define UP_HALF         NEUTRAL - (NEUTRAL * (WALK_PERCENT/100));
#define UP_MIN          NEUTRAL - (NEUTRAL * (TIPTOE_PERCENT/100));

#define DOWN_MAX        255;
#define DOWN_HALF       NEUTRAL + (NEUTRAL * (WALK_PERCENT/100));
#define DOWN_MIN        NEUTRAL + (NEUTRAL * (TIPTOE_PERCENT/100));

#define LEFT_MAX        0;
#define LEFT_HALF       NEUTRAL - (NEUTRAL * (WALK_PERCENT/100));
#define LEFT_MIN        NEUTRAL - (NEUTRAL * (TIPTOE_PERCENT/100));

#define RIGHT_MAX       255;
#define RIGHT_HALF      NEUTRAL + (NEUTRAL * (WALK_PERCENT/100));
#define RIGHT_MIN       NEUTRAL + (NEUTRAL * (TIPTOE_PERCENT/100));

#endif