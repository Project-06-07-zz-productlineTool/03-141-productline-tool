/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-29     misonyo      first implementation.
 */
/*
 * 程序清单： ADC 设备使用例程
 * 例程导出了 adc_sample 命令到控制终端
 * 命令调用格式：adc_sample
 * 程序功能：通过 ADC 设备采样电压值并转换为数值。
 *           示例代码参考电压为3.3V,转换位数为12位。
 */

#include <rtdevice.h>
#include <rtthread.h>

#include "smt_board.h"

#define ADC_DEV_NAME "adc1"    /* ADC 设备名称 */
#define REFER_VOLTAGE 3300     /* 参考电压 3.3V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS (1 << 12) /* 转换位数为12位 */

#define PIN_ESC_DRONE_ONOFF "PA.11"
#define PIN_ESC_SEL_USB "PA.12"

rt_base_t pin_esc_drone_onoff = 0;
rt_base_t pin_esc_sel_usb = 0;

rt_adc_device_t adc_dev_smt;

int smt_adc_init(void) {
  rt_err_t ret = RT_EOK;
  /* 查找设备 */
  adc_dev_smt = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
  if (adc_dev_smt == RT_NULL) {
    rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
    return RT_ERROR;
  }

  return ret;
}

int smt_adc_get(rt_uint16_t *io_data, rt_uint8_t size) {
  rt_err_t ret = RT_EOK;
  rt_uint32_t value;

  for (rt_uint8_t i = 0; i < size; i++) {
    ret = rt_adc_enable(adc_dev_smt, i);
    value = rt_adc_read(adc_dev_smt, i);
    io_data[i] = value * REFER_VOLTAGE / CONVERT_BITS;
    ret = rt_adc_disable(adc_dev_smt, i);
  }
}

static void smtEscIoCtrlInit(void) {
  pin_esc_drone_onoff = rt_pin_get(PIN_ESC_DRONE_ONOFF);
  pin_esc_sel_usb = rt_pin_get(PIN_ESC_SEL_USB);
  rt_pin_mode(pin_esc_drone_onoff, PIN_MODE_OUTPUT);
  rt_pin_mode(pin_esc_sel_usb, PIN_MODE_OUTPUT);
  rt_pin_write(pin_esc_drone_onoff, 1);
  rt_pin_write(pin_esc_sel_usb, 1);
}

static void taskEscAdcEntry() {
  static rt_uint8_t io_control = 0;
  while (1) {
    if (!io_control) {
      io_control = 1;
      rt_pin_write(pin_esc_drone_onoff, 0);
      rt_thread_delay(1000);
      rt_thread_delay(1000);
      rt_thread_delay(1000);
      rt_pin_write(pin_esc_drone_onoff, 1);
      rt_thread_delay(1000);
      rt_thread_delay(1000);
      rt_pin_write(pin_esc_sel_usb, 0);
      rt_thread_delay(1000);
      rt_thread_delay(1000);
      rt_thread_delay(1000);
      rt_pin_write(pin_esc_sel_usb, 1);
    }
    rt_thread_delay(1000);
  }
}

int taskEscAdcInit(void) {
  rt_thread_t tid1 = RT_NULL;
  smtEscIoCtrlInit();

  tid1 = rt_thread_create("taskEscAdcCheck", taskEscAdcEntry, NULL, 512, TASK_IO_POLL_THREAD_PRIORITY, 10);
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
