#ifndef GENERIC_INPUT_INTERFACE_H
#define GENERIC_INPUT_INTERFACE_H

#include <stdint.h>

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

// Dummy report struct
typedef struct TU_ATTR_PACKED
{
  uint16_t buttons;
  uint8_t hat;
  uint8_t x;
  uint8_t y;
  uint8_t z;
  uint8_t rz;
  uint8_t vendorSpec;
} dummy_report_t;

#endif