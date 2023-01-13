#ifndef ANALOGLEVER_H
#define ANALOGLEVER_H

#include <stdint.h>
#include <math.h>

#include "hardware/adc.h"

#include "config/config.h"
//#pragma message "AnalogLever library v0.0.1"

/****************************************************
 * General analog header to include in your project *
*****************************************************/

/* Axis struct */
typedef struct {
    // Defines a 1-dimensional axis
    int8_t channel;
    int16_t filtered;
    int16_t center;
    int16_t offset;
    int16_t minimum;
    int16_t maximum;
    int16_t* sampleArray;
    uint8_t calibrated;
} Axis;

int16_t axisGetValue(Axis* self);

/* Polar coordinates struct */
typedef struct {
    // Defines a polar coordinates system
    uint16_t r;
    uint16_t rmax;
    uint16_t dzThreshhold;
    uint16_t dpadThreshhold;
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
    uint8_t x;
    uint8_t y;
    PolarSystem polar;
} Coordinates;

int16_t initCoordsStruct(Coordinates* self);
int16_t updateCoordinates(Coordinates* self);
int16_t updateCoordinatesI2C(Coordinates* self, uint8_t xbuf, uint8_t ybuf);
void recalibrate(Coordinates* self);

long map(long x, long in_min, long in_max, long out_min, long out_max);

#endif
