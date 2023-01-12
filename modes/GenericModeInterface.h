#ifndef GENERIC_INPUT_INTERFACE_H
#define GENERIC_INPUT_INTERFACE_H

#include <stdint.h>

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


void doButtons(dummy_report_t* report);
void doLeftStick(dummy_report_t* report);
void doCStick(dummy_report_t* report);

#endif