#include <rtdevice.h>
#include <rtthread.h>
#include "smt_io_get.h"

rt_base_t pc13_pin_ = 0;

void ledIoInit(void) {
  pc13_pin_ = rt_pin_get("PC.13");
  rt_pin_mode(pc13_pin_, PIN_MODE_OUTPUT);
}

void ledToggle(void) {
  static rt_uint8_t status = 0;
  if (status) {
    status = 0;
  } else {
    status = 1;
  }
  rt_pin_write(pc13_pin_, status);
}
