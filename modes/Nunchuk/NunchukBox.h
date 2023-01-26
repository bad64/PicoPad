#ifndef NUNCHUKBOX_H
#define NUNCHUKBOX_H

#include "../GenericModeInterface.h"
#include <lib/i2c/i2c.h>

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