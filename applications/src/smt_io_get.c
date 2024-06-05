#include <rtdevice.h>
#include <rtthread.h>

#define IO_NUM 5
rt_base_t pb0_pin = 0;

void smt_io_init(void) {
  pb0_pin = rt_pin_get("PB.0");
  rt_base_t io_num_temp = 0;
  for (rt_int8_t i = 0; i < IO_NUM; i++) {
    io_num_temp = i + pb0_pin;
    rt_pin_mode(io_num_temp, PIN_MODE_INPUT);
  }
}

int smt_io_get(rt_uint16_t *io_data, rt_uint8_t size) {
  rt_base_t io_num_temp = 0;
  for (rt_int8_t i = 0; i < size; i++) {
    io_num_temp = i + pb0_pin;
    io_data[i] = (rt_uint16_t)rt_pin_read(io_num_temp);
  }
}
