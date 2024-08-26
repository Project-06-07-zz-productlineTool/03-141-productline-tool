/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-05-04     RT-Thread    first version
 */

#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "freemodbus_slave.h"
#include "led_status.h"
#include "smt_esc_board.h"
#include "smt_io_get.h"
#include "smt_led_color.h"

static rt_device_t vcom_shell_device = NULL;
void change_shell(void) {
  vcom_shell_device = rt_device_find("vcom");
  if (vcom_shell_device) {
    vconsole_switch(vcom_shell_device);
  }
}

int bspInit(void) {
  change_shell();
  smt_io_init();
  smt_adc_init();
  mb_slave_sample();
  ledIoInit();
  smt_led_board_init();
  return RT_EOK;
}

int taskInit(void) {
  taskSmtLedInit();
  taskSmtIOPollInit();
  return RT_EOK;
}

int main(void) {
  bspInit();
  taskInit();

  return RT_EOK;
}
