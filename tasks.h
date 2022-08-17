#ifndef TASKS_H
#define TASKS_H

#include <stdint.h>
#include "bsp/board.h"
#include "tusb.h"

#include "usb_descriptors.h"

typedef struct TU_ATTR_PACKED
{
  uint16_t buttons;
  uint8_t hat;
  uint8_t  x;
  uint8_t  y;
  uint8_t  z;
  uint8_t  rz;
  uint8_t vendorSpec;
} pokken_controller_report_t;

void tud_mount_cb(void);
void tud_umount_cb(void);
void tud_suspend_cb(bool remote_wakeup_en);
void tud_resume_cb(void);

static void send_hid_report(pokken_controller_report_t report);
void hid_task(pokken_controller_report_t report);
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len);
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen);
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize);

#endif