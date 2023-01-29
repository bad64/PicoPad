#ifndef GENERIC_INPUT_INTERFACE_H
#define GENERIC_INPUT_INTERFACE_H

#include <stdint.h>
#include <config/config.h>

/* Pin definitions */
//#define PINOUT_PROTOTYPE_BOARD
//#define PINOUT_FINAL_BOARD
#define PINOUT_TEST

#if defined(PINOUT_PROTOTYPE_BOARD)
    #include <config/pinout_prototype.h>
#endif
#if defined(PINOUT_TEST)
    #include <config/pinout_emergency.h>
#endif
#if defined (PINOUT_FINAL_BOARD)
    #include <config/pinout_final.h>
#endif

#if !defined(PINOUT_PROTOTYPE_BOARD) && !defined(PINOUT_TEST) && !defined(PINOUT_FINAL_BOARD)
    #error "Please select a board configuration in config/config.h"
#endif

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

// SOCD stuff
#define SOCD_LAST_NONE      0
#define SOCD_LAST_LEFT      1
#define SOCD_LAST_RIGHT     2

// Analog values
// TODO: Most of those are arbitrary, find a mathematical way to assign them
#define NEUTRAL             127
#define WALK_PERCENT        45
#define TIPTOE_PERCENT      10

#define UP_MAX              0
#define UP_HALF             69
#define UP_MIN              110

#define DOWN_MAX            255
#define DOWN_HALF           184
#define DOWN_MIN            139

#define LEFT_MAX            0
#define LEFT_HALF           69
#define LEFT_MIN            110

#define RIGHT_MAX           255
#define RIGHT_HALF          184
#define RIGHT_MIN           139

uint16_t doButtons();
uint16_t doCStick();
uint8_t doHat();
uint16_t doLeftStick();

#endif