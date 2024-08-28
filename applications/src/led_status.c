#include <rtdevice.h>
#include <rtthread.h>

#include "smt_board.h"
#include "smt_io_get.h"

#define LOG_TAG "example"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

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
  rt_uint16_t count = 0;
  rt_uint16_t printf_count = 0;
  while (1) {
    rt_pin_write(pc13_pin_, 0);
    rt_thread_delay(100);
    rt_pin_write(pc13_pin_, 1);
    rt_thread_delay(100);

    if (count < 5) {
      count++;
      printf_count++;
    } else {
      count = 0;
      LOG_D("LOG_D(%d): RT-Thread is an open source IoT operating system from China.", printf_count);
      rt_kprintf("this is rt_kprintf\r\n");
    }
  }
}

int taskLedStatusInit(void) {
  rt_thread_t tid1 = RT_NULL;
  ledIoInit();

  tid1 = rt_thread_create("LedStatus", taskLedToggleEntry, NULL, 1024, TASK_IO_POLL_THREAD_PRIORITY, 10);
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
