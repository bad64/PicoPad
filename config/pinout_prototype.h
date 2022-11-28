#ifndef PINOUT_H
#define PINOUT_H

#define PIN_DPAD_DOWN       0
#define PIN_DPAD_UP         1
#define PIN_DPAD_LEFT       2
#define PIN_DPAD_RIGHT      3
#define PIN_HOME            4
#define PIN_SELECT          5
#define PIN_START           6
#define PIN_1P              7
#define PIN_2P              8
#define PIN_3P              9
#define PIN_4P              10
#define PIN_1K              11
#define PIN_2K              12
#define PIN_3K              13
#define PIN_4K              14
#define PIN_LS_DP           15
#define PIN_SDA             16
#define PIN_SCL             17

// GC notation
#define PIN_GC_B            PIN_1K
#define PIN_GC_A            19
#define PIN_GC_Y            PIN_4P
#define PIN_GC_X            PIN_2K
#define PIN_GC_L            PIN_SELECT  // It's a long story
#define PIN_GC_R            PIN_4K
//#define PIN_GC_ZL           PIN_UNUSED    // Does not have a physical button mapped
#define PIN_GC_ZR           PIN_3K

#define PIN_GC_CUP          PIN_2P
#define PIN_GC_CDOWN        18
#define PIN_GC_CLEFT        PIN_1P
#define PIN_GC_CRIGHT       PIN_3P

#define PIN_ANALOG_X        26
#define PIN_ANALOG_Y        27

#define PIN_GC_DATA         28

#define ADC_X               0
#define ADC_Y               1
#define ADC_B               2

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

// I2C masks
#define MASK_I2C_Z          0b10000000
#define MASK_I2C_C          0b01000000

// FGC mode bitmasks
#define MASK_1P             MASK_Y
#define MASK_2P             MASK_X
#define MASK_3P             MASK_R
#define MASK_4P             MASK_L
#define MASK_1K             MASK_B
#define MASK_2K             MASK_A
#define MASK_3K             MASK_ZR
#define MASK_4K             MASK_ZL

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

#endif