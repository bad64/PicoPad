#include "Analog.h"
#include <stdlib.h>

#define PI                  3.1416
#define NUMBER_OF_SAMPLES   10
#define DEADZONE            5
#define REVERSE_DEADZONE    50

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
        if (self->sampleArray[i] < self-> minimum) self->sampleArray[i] = self->minimum;
        else if (self->sampleArray[i] < self-> maximum) self->sampleArray[i] = self->maximum;
    }

    uint16_t averageBuf = 0;
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        averageBuf += self->sampleArray[i];
    }

    //return map(averageBuf / NUMBER_OF_SAMPLES, self->minimum, self->maximum, -127, 127);
    return averageBuf / NUMBER_OF_SAMPLES;
}

void convertToPolar(PolarSystem* self, uint16_t x, uint16_t y)
{
    self->r = sqrt( pow(x, 2) + pow(y, 2) );
    self->theta = atan2(y, x);
    self->deg = (self->theta + PI) * (180 / (1 * PI));
}

int16_t initCoordsStruct(Coordinates* self)
{
    /* X axis */
    self->_x.channel = 0;
    self->_x.center = 2048;

    #if defined(LEVER_JLM)
        // Empirical values; will need to be adjusted on a case by case basis
        self->_x.minimum = 1348;
        self->_x.maximum = 2748;
        self->_x.offset = -64;
    #endif

    #if defined(LEVER_U360)
        // Ditto
        self->_x.minimum = self->_x.center - 360;
        self->_x.maximum = self->_x.center + 360;
        self->_x.offset = 0;
    #endif

    self->_x.sampleArray = (uint16_t*)malloc(sizeof(uint16_t) * NUMBER_OF_SAMPLES);
    if (self->_x.sampleArray == NULL) return -1;

    /* Y axis */
    self->_y.channel = 1;
    self->_y.center = 2048;

    #if defined(LEVER_JLM)
        // Same as above
        self->_y.minimum = 2748;
        self->_y.maximum = 1348;
        self->_y.offset = -30;
    #endif

    #if defined(LEVER_U360)
        // Also here
        self->_y.minimum = self->_y.center - 360;
        self->_y.maximum = self->_y.center + 360;
        self->_y.offset = 74;
    #endif

    self->_y.sampleArray = (uint16_t*)malloc(sizeof(uint16_t) * NUMBER_OF_SAMPLES);
    if (self->_y.sampleArray == NULL) return -2;

    return 0;
}

int16_t updateCoordinates(Coordinates* self)
{
    static int16_t xbuf, ybuf;

    xbuf = axisGetValue(&self->_x) - self->_x.center;
    ybuf = axisGetValue(&self->_y) - self->_y.center;

    convertToPolar(&self->polar, xbuf, ybuf);

    // TODO: only map at the end of this function

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
    self->x = self->polar.r * cos(self->polar.theta);
    self->y = self->polar.r * sin(self->polar.theta);

    // Boundary checking, values must not wrap around an uint8_t
    if (self->x <= 0) self->x = 0;
    else if (self->x >= 255) self->x = 255;

    if (self->y <= 0) self->y = 0;
    else if (self->y >= 255) self->y = 255;

    // Re-convert with corrected values so theta && deg make sense
    convertToPolar(&self->polar, self->x - 127, self->y - 127);

    return 0;
}