#ifndef ANALOGLEVER_H
#define ANALOGLEVER_H

#include <stdint.h>
#include <math.h>

#include "hardware/adc.h"

//#pragma message "AnalogLever library v0.0.1"

/****************************************************
 * General analog header to include in your project *
*****************************************************/

// Lever selection compile-time defines
#define LEVER_JLM
//#define LEVER_U360

#if !defined(LEVER_JLM) && !defined(LEVER_U360)
    #error "You need to define either lever in Analog.h"
#endif

/* Axis struct */
typedef struct {
    // Defines a 1-dimensional axis
    uint8_t channel;
    uint16_t center;
    uint16_t offset;
    uint16_t minimum;
    uint16_t maximum;
    uint16_t* sampleArray;
} Axis;

uint16_t axisGetValue(Axis* self);

/* Polar coordinates struct */
typedef struct {
    // Defines a polar coordinates system
    uint16_t r;
    uint16_t rmax;
    uint16_t dzThreshhold;
    uint16_t rdzThreshhold;
    double theta;
    double deg;
} PolarSystem;

uint16_t getRMax(int16_t x, int16_t y);
void convertToPolar(PolarSystem* self, int16_t x, int16_t y);

/* Coordinates struct */
typedef struct {
    Axis _x;
    Axis _y;
    int16_t x;
    int16_t y;
    PolarSystem polar;
} Coordinates;

int16_t initCoordsStruct(Coordinates* self);
int16_t updateCoordinates(Coordinates* self);

long map(long x, long in_min, long in_max, long out_min, long out_max);

#endif
