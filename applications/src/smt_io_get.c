#include "smt_io_get.h"

#include <rtdevice.h>
#include <rtthread.h>

#define TASK_IO_POLL_THREAD_PRIORITY RT_THREAD_PRIORITY_MAX - 1

rt_base_t pb0_pin = 0;
rt_uint8_t io_value_cache_[IO_NUM] = {0};

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

int smt_io_get(rt_uint16_t *io_data, rt_uint8_t begin_num, rt_uint8_t over_num) {
  rt_base_t io_num_temp = 0;
  rt_uint8_t buf_index = 0;
  for (rt_int8_t i = begin_num; i < over_num; i++) {
    // io_num_temp = i + pb0_pin;
    // io_data[buf_index++] = (rt_uint16_t)rt_pin_read(io_num_temp);
    io_data[buf_index++] = io_value_cache_[i];
  }
}

static void taskSmtIoPollEntry(void) {
  rt_uint8_t io_value_this = 0;
  rt_base_t io_num_temp = 0;
  while (1) {
    for (rt_uint8_t i = 0; i < IO_NUM; i++) {
      io_num_temp = i + pb0_pin;
      io_value_this = rt_pin_read(io_num_temp);
      if (io_value_cache_[i] != io_value_this) {
        rt_thread_delay(20);
        io_value_this = rt_pin_read(io_num_temp);
        if (io_value_this != io_value_cache_[i]) {
          io_value_cache_[i] = io_value_this;
        }
      }
      rt_thread_delay(50);
    }
  }
}

void taskSmtIOPollInit(void) {
  rt_thread_t tid1 = RT_NULL;

  tid1 = rt_thread_create("taskSmtIoPoll", taskSmtIoPollEntry, NULL, 512, TASK_IO_POLL_THREAD_PRIORITY, 10);
  if (tid1 != RT_NULL) {
    rt_thread_startup(tid1);
  } else {
    goto __exit;
  }

  return RT_EOK;

__exit:
  if (tid1) rt_thread_delete(tid1);

  return -RT_ERROR;
}
