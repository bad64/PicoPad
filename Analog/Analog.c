#include "Analog.h"
#include <stdlib.h>

#define PI                  3.1416
#define NUMBER_OF_SAMPLES   10
#define DEADZONE            10
#define REVERSE_DEADZONE    60

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
        self->sampleArray[i] = adc_read() + self->offset;
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
    /* X axis */
    int16_t xbuf;
    self->_x.channel = 0;
    self->_x.center = 2048;

    #if defined(LEVER_JLM)
        // Empirical values; will need to be adjusted on a case by case basis
        self->_x.minimum = 1348;
        self->_x.maximum = 2748;
    #endif

    #if defined(LEVER_U360)
        // Ditto
        self->_x.minimum = self->_x.center - 360;
        self->_x.maximum = self->_x.center + 360;
    #endif

    adc_select_input(self->_x.channel);
    xbuf = adc_read();
    self->_x.offset = self->_x.center - xbuf;

    self->_x.sampleArray = (uint16_t*)malloc(sizeof(uint16_t) * NUMBER_OF_SAMPLES);
    if (self->_x.sampleArray == NULL) return -1;

    /* Y axis */
    int16_t ybuf;
    self->_y.channel = 1;
    self->_y.center = 2048;

    #if defined(LEVER_JLM)
        // Same as above
        self->_y.minimum = 1348;
        self->_y.maximum = 2748;
    #endif

    #if defined(LEVER_U360)
        // Also here
        self->_y.minimum = self->_y.center - 360;
        self->_y.maximum = self->_y.center + 360;
    #endif

    adc_select_input(self->_y.channel);
    ybuf = adc_read();
    self->_y.offset = self->_y.center - ybuf;

    self->_y.sampleArray = (uint16_t*)malloc(sizeof(uint16_t) * NUMBER_OF_SAMPLES);
    if (self->_y.sampleArray == NULL) return -2;

    // Set polar thressholds
    self->polar.rmax = getRMax(127, 0);
    self->polar.dzThreshhold = (self->polar.rmax * DEADZONE) / 100;
    self->polar.rdzThreshhold = (self->polar.rmax * REVERSE_DEADZONE) / 100;

    return 0;
}

int16_t updateCoordinates(Coordinates* self)
{
    int16_t xbuf, ybuf;

    xbuf = axisGetValue(&self->_x);
    ybuf = axisGetValue(&self->_y);

    xbuf = map(xbuf, self->_x.minimum, self->_x.maximum, -127, 127);
    ybuf = map(ybuf, self->_y.minimum, self->_y.maximum, 127, -127);

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

    self->y = map(ybuf, -127, 127, 0, 255);
    if (self->y < 1) self->y = 0;
    else if (self->y > 254) self->y = 255;

    return 0;
}