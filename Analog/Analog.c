#include "Analog.h"
#include <stdlib.h>

#define PI                  3.1416
#define NUMBER_OF_SAMPLES   10
#define DEADZONE            3
#define REVERSE_DEADZONE    60

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    // Literally the Arduino map() function
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t axisGetValue(Axis* self)
{
    adc_select_input(self->channel);
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        self->sampleArray[i] = adc_read() + self->offset;
    }

    static uint16_t averageBuf;
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        averageBuf += self->sampleArray[i];
    }

    return map(averageBuf / NUMBER_OF_SAMPLES, self->minimum, self->maximum, 0, 255);
}

void convertToPolar(PolarSystem* self, uint16_t x, uint16_t y)
{
    self->r = sqrt( pow(x, 2) + pow(y, 2) );
    self->theta = atan2(y, x);
    self->deg = (self->theta + PI) * (180 / (1 * PI));
}

void updateCoordinates(Coordinates* self)
{
    static uint16_t xbuf, ybuf;

    xbuf = axisGetValue(&self->_x);
    ybuf = axisGetValue(&self->_y);

    convertToPolar(&self->polar, xbuf - 127, ybuf - 127);   // Take 127 off to obtain a zero-centered set of coordinates

    if (self->polar.r < DEADZONE)
    {
        xbuf = 0;
        ybuf = 0;
    }
    else if (self->polar.r > REVERSE_DEADZONE)
    {
        self->polar.r = 127;    // Max theoretical absolute value of x and y
    }

    // Update values
    self->x = 127 + self->polar.r * cos(self->polar.theta);
    self->y = 127 + self->polar.r * sin(self->polar.theta);

    // Boundary checking, values must not wrap around an uint8_t
    if (self->x < 0) self->x = 0;
    else if (self->x > 255) self->x = 255;

    if (self->y < 0) self->y = 0;
    else if (self->y > 255) self->y = 255;

    // Re-convert with corrected values so theta && deg make sense
    convertToPolar(&self->polar, self->x - 127, self->y - 127);
}

void initCoordsStruct(Coordinates* self)
{
    /* X axis */
    self->_x.channel = 0;
    self->_x.center = 2048;

    #if defined(LEVER_JLM)
        // Empirical values; will need to be adjusted on a case by case basis
        //self->_x.minimum = self->_x.center - 275;
        //self->_x.maximum = self->_x.center + 275;
        self->_x.minimum = 1024;
        self->_x.maximum = 3072;
        self->_x.offset = -13;
    #endif

    #if defined(LEVER_U360)
        // Ditto
        self->_x.minimum = self->_x.center - 360;
        self->_x.maximum = self->_x.center + 360;
        self->_x.offset = 0;
    #endif

    self->_x.sampleArray = (uint16_t*)malloc(sizeof(uint16_t) * NUMBER_OF_SAMPLES);

    /* Y axis */
    self->_y.channel = 1;
    self->_y.center = 5048;

    #if defined(LEVER_JLM)
        // Same as above
        self->_y.minimum = self->_y.center - 275;
        self->_y.maximum = self->_y.center + 275;
        self->_y.offset = -20;
    #endif

    #if defined(LEVER_U360)
        // Also here
        self->_y.minimum = self->_y.center - 360;
        self->_y.maximum = self->_y.center + 360;
        self->_y.offset = 74;
    #endif

    self->_y.sampleArray = (uint16_t*)malloc(sizeof(uint16_t) * NUMBER_OF_SAMPLES);
}