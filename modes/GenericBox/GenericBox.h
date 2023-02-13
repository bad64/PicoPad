#ifndef GENERICBOX_H
#define GENERICBOX_H

#include "../GenericModeInterface.h"

// SOCD default check
#if !defined(SOCD_LRN) && !defined(SOCD_2IP) && !defined(SOCD_YOLO)
    #define SOCD_LRN
#endif

typedef enum {
    SOCD_PROXY_LRN,
    SOCD_PROXY_2IP,
    SOCD_PROXY_YOLO
} SOCD_PROXY;

#if defined(MODE_GENERICBOX_18_BUTTONS) || defined(MODE_GENERICBOX_20_BUTTONS)
    #define INPUT_UP        PIN_4K
    #define INPUT_DOWN      PIN_DPAD_DOWN
    #define INPUT_LEFT      PIN_DPAD_UP
    #define INPUT_RIGHT     PIN_DPAD_RIGHT
    #define INPUT_L         PIN_DPAD_LEFT
#endif
#if defined(MODE_WASDBOX)
    #define INPUT_UP        PIN_DPAD_UP
    #define INPUT_DOWN      PIN_DPAD_DOWN
    #define INPUT_LEFT      PIN_DPAD_LEFT
    #define INPUT_RIGHT     PIN_DPAD_RIGHT
    #define INPUT_L         PIN_4K
#endif

#define INPUT_A         PIN_A
#define INPUT_B         PIN_1K
#define INPUT_X         PIN_2K
#define INPUT_Y         PIN_2P

#define INPUT_R         PIN_1P
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
#define INPUT_MODX      PIN_SELECT
#define INPUT_MODY      PIN_HOME

#if defined(MODE_GENERICBOX_20_BUTTONS) || defined(MODE_WASDBOX)
    #define INPUT_MS    PIN_3P
    #define INPUT_LS    PIN_4P
#endif

#endif