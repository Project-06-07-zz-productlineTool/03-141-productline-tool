#include <rtdevice.h>
#include <rtthread.h>

#include "smt_board.h"
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

static void taskLedToggleEntry() {
  while (1) {
    rt_pin_write(pc13_pin_, 0);
    rt_thread_delay(100);
    rt_pin_write(pc13_pin_, 1);
    rt_thread_delay(100);
  }
}

int taskLedStatusInit(void) {
  rt_thread_t tid1 = RT_NULL;
  ledIoInit();

  tid1 = rt_thread_create("taskLedToggle", taskLedToggleEntry, NULL, 256, TASK_IO_POLL_THREAD_PRIORITY, 10);
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
