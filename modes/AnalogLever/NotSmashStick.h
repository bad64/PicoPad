#ifndef ANALOGSTICK_H
#define ANALOGSTICK_H

#include "../GenericModeInterface.h"
#include <lib/analog/analog.h>

#define INPUT_UP        -1
#define INPUT_DOWN      -1
#define INPUT_LEFT      -1
#define INPUT_RIGHT     -1

#define INPUT_A         PIN_A
#define INPUT_B         PIN_1K
#define INPUT_X         PIN_2K
#define INPUT_Y         PIN_4P

#define INPUT_R         PIN_3K
#define INPUT_L         PIN_SELECT
#define INPUT_ZL        -1
#define INPUT_ZR        PIN_4K

#define INPUT_C_UP      PIN_2P
#define INPUT_C_DOWN    PIN_C_DOWN
#define INPUT_C_LEFT    PIN_1P
#define INPUT_C_RIGHT   PIN_3P

#define INPUT_START     PIN_START
#define INPUT_SELECT    -1
#define INPUT_HOME      PIN_HOME

#define INPUT_LS_DP     PIN_LS_DP
#define INPUT_MODX      -1
#define INPUT_MODY      -1

#define INPUT_MS        -1
#define INPUT_LS        -1

uint8_t doHatAnalog(uint16_t r, uint16_t dpadThreshhold, double deg);

#endif