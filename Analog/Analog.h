#ifndef ANALOGLEVER_H
#define ANALOGLEVER_H

#include <stdint.h>
#include <math.h>

#pragma message "AnalogLever library v0.0.1"

/****************************************************
 * General analog header to include in your project *
*****************************************************/

// Lever selection compile-time defines
#define LEVER_JLM
//#define LEVER_U360

#if !defined(LEVER_JLM) && !defined(LEVER_U360)
    #error "You need to define either lever in Analog.h"
#endif

typedef struct {
    // Defines a 1-dimensional axis
    uint8_t channel;
    uint16_t value;
    uint16_t center;
    uint16_t offset;
    uint16_t minimum;
    uint16_t maximum;
    uint8_t numberOfSamples;
    uint16_t* sampleArray;
} Axis;

typedef struct {
    uint16_t r;
    double theta;
    double deg;
} PolarSystem;

#endif
