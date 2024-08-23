#include "smt_io_get.h"
#include <rtdevice.h>
#include <rtthread.h>

rt_base_t pb0_pin = 0;

void smt_io_init(void) {
  pb0_pin = rt_pin_get("PB.0");
  rt_base_t io_num_temp = 0;
  for (rt_int8_t i = 0; i < ESC_IO_NUM; i++) {
    io_num_temp = i + pb0_pin;
    rt_pin_mode(io_num_temp, PIN_MODE_INPUT);
  }

  for (rt_int8_t i = ESC_IO_NUM; i < IO_NUM; i++) {
    io_num_temp = i + pb0_pin;
    rt_pin_mode(io_num_temp, PIN_MODE_INPUT_PULLUP);
  }
}

int smt_io_get(rt_uint16_t *io_data,rt_uint8_t begin_num, rt_uint8_t over_num) {
  rt_base_t io_num_temp = 0;
  rt_uint8_t buf_index = 0 ;
  for (rt_int8_t i = begin_num; i < over_num; i++) {
    io_num_temp = i + pb0_pin;
    io_data[buf_index++] = (rt_uint16_t)rt_pin_read(io_num_temp);
  }
}
