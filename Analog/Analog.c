#include "Analog.h"
#include <stdlib.h>

#define PI                  3.1416
#define NUMBER_OF_SAMPLES   10

// You may want to change these to suit your personal preferences
#if defined(LEVER_JLM)
    #define DEADZONE            12
    #define REVERSE_DEADZONE    60
#endif
#if defined(LEVER_U360)
    #define DEADZONE            20
    #define REVERSE_DEADZONE    70
#endif

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    // Literally the Arduino map() function
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int16_t axisGetValue(Axis* self)
{
    adc_select_input(self->channel);
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        self->sampleArray[i] = adc_read();
        if (self->sampleArray[i] <= self->minimum) self->sampleArray[i] = self->minimum;
        else if (self->sampleArray[i] >= self->maximum) self->sampleArray[i] = self->maximum;
    }

    int16_t averageBuf = 0;
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        averageBuf += self->sampleArray[i];
    }

    self->filtered = averageBuf / NUMBER_OF_SAMPLES;
    return self->filtered;
}

uint16_t getRMax(int16_t x, int16_t y)
{
    return sqrt( pow(x, 2) + pow(y, 2) );
}

void convertToPolar(PolarSystem* self, int16_t x, int16_t y)
{
    self->r = sqrt( pow(x, 2) + pow(y, 2) );
    self->theta = atan2(y, x);
    self->deg = (self->theta + PI) * (180 / (1 * PI));
}

int16_t initCoordsStruct(Coordinates* self)
{
    self->_x.calibrated = 0;
    self->_y.calibrated = 0;

    /* X axis */
    int16_t xbuf;
    self->_x.channel = 0;

    #if defined(LEVER_JLM)
        // Empirical values; might need to be adjusted on a case by case basis
        self->_x.center = 2048;
        self->_x.minimum = 1348;
        self->_x.maximum = 2748;
    #endif

    #if defined(LEVER_U360)
        // Ditto
        self->_x.center = 1580;
        self->_x.minimum = 90;
        self->_x.maximum = 2865;
    #endif

    self->_x.sampleArray = (uint16_t*)malloc(sizeof(uint16_t) * NUMBER_OF_SAMPLES);
    if (self->_x.sampleArray == NULL) return -1;

    self->_x.offset = 0;

    /* Y axis */
    int16_t ybuf;
    self->_y.channel = 1;

    #if defined(LEVER_JLM)
        // Same as above
        self->_y.center = 2048;
        self->_y.minimum = 1348;
        self->_y.maximum = 2748;
    #endif

    #if defined(LEVER_U360)
        // Also here
        self->_y.center = 1720;
        self->_y.minimum = 264;
        self->_y.maximum = 2940;
    #endif

    self->_y.sampleArray = (uint16_t*)malloc(sizeof(uint16_t) * NUMBER_OF_SAMPLES);
    if (self->_y.sampleArray == NULL) return -2;

    self->_y.offset = 0;

    // Set polar thressholds
    self->polar.rmax = getRMax(127, 0);
    self->polar.dzThreshhold = (self->polar.rmax * DEADZONE) / 100;
    self->polar.rdzThreshhold = (self->polar.rmax * REVERSE_DEADZONE) / 100;
    self->polar.dpadThreshhold = 3 * self->polar.dzThreshhold;

    return 0;
}

int16_t updateCoordinates(Coordinates* self)
{
    int16_t xbuf, ybuf;

    xbuf = axisGetValue(&self->_x);
    ybuf = axisGetValue(&self->_y);

    xbuf = map(xbuf, self->_x.minimum, self->_x.maximum, -127, 127) + self->_x.offset;
    #if defined(LEVER_JLM)
        ybuf = map(ybuf, self->_y.minimum, self->_y.maximum, 127, -127) + self->_y.offset;
    #endif
    #if defined(LEVER_U360)
        // U360 has a reversed Y-axis
        ybuf = map(ybuf, self->_y.minimum, self->_y.maximum, 127, -127) - self->_y.offset;
    #endif

    convertToPolar(&self->polar, xbuf, ybuf);

    if (self->polar.r < self->polar.dzThreshhold)
    {
        xbuf = 0;
        ybuf = 0;
    }
    else if (self->polar.r > self->polar.rdzThreshhold)
    {
        self->polar.r = self->polar.rmax;
        xbuf = (self->polar.r * cos(self->polar.theta));
        ybuf = (self->polar.r * sin(self->polar.theta));
    }

    // Update values
    self->x = map(xbuf, -127, 127, 0, 255);
    
    if (self->x < 1) self->x = 0;
    else if (self->x > 254) self->x = 255;

    #if defined(LEVER_JLM)
        self->y = map(ybuf, -127, 127, 0, 255);
    #endif
    #if defined(LEVER_U360)
        self->y = map(ybuf, 127, -127, 0, 255);
    #endif

    if (self->y < 1) self->y = 0;
    else if (self->y > 254) self->y = 255;

    if (self->_x.calibrated == 0)
    {
        self->_x.offset = 127 - self->x;
        self->_x.calibrated = 1;
    }

    if (self->_y.calibrated == 0)
    {
        self->_y.offset = 127 - self->y;
        self->_y.calibrated = 1;
    }

    return 0;
}

void recalibrate(Coordinates* self)
{
    initCoordsStruct(self);
}