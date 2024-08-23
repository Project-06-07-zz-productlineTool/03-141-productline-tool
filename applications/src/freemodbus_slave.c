/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-06-21     flybreak     first version
 */

#include <rtthread.h>

#include "mb.h"
#include "user_mb_app.h"
#include "smt_adc_get.h"
#include "smt_io_get.h"
#include "led_status.h"
#include "vl53l1_test.h"

#ifdef PKG_MODBUS_SLAVE_SAMPLE
#define SLAVE_ADDR      MB_SAMPLE_SLAVE_ADDR
#define PORT_NUM        MB_SLAVE_USING_PORT_NUM
#define PORT_BAUDRATE   MB_SLAVE_USING_PORT_BAUDRATE
#else
#define SLAVE_ADDR      0x01
#define PORT_NUM        2
#define PORT_BAUDRATE   115200
#endif

#define PORT_PARITY     MB_PAR_EVEN

#define MB_POLL_THREAD_PRIORITY  10
#define MB_SEND_THREAD_PRIORITY  RT_THREAD_PRIORITY_MAX - 1

#define MB_POLL_CYCLE_MS 200

extern USHORT usSRegHoldBuf[S_REG_HOLDING_NREGS];

#define SMT_ADC_NUM 6

/*modbus buf index*/
#define ESC_BOARD_BUF_ADCVALUE_REGIN 0
#define ESC_BOARD_BUF_IOVALUE_REGIN SMT_ADC_NUM
#define TOF2_BOARD_BUF_KEYVALUE_BEGIN (SMT_ADC_NUM + ESC_IO_NUM)
#define TOF2_BOARD_BUF_TOFVALUE_BEGIN (SMT_ADC_NUM + ESC_IO_NUM + TOF2_KEY_NUM)
#define KEY_BOARD_BUF_KEYVALUE_BEGIN (SMT_ADC_NUM + ESC_IO_NUM + TOF2_KEY_NUM + 1)

/*io begin and over num*/
#define ESC_BOARD_IO_GET_BEGIN 0
#define ESC_BOARD_IO_GET_OVER ESC_IO_NUM

#define TOF2_BOARD_KEY_GET_BEGIN ESC_IO_NUM
#define TOF2_BOARD_KEY_GET_OVER (ESC_IO_NUM + TOF2_KEY_NUM)

#define KEY_BOARD_KEY_GET_BEGIN (ESC_IO_NUM + TOF2_KEY_NUM)
#define KEY_BOARD_KEY_GET_OVER (ESC_IO_NUM + TOF2_KEY_NUM + KEYBOARD_KEY_NUM)

static void send_thread_entry(void *parameter) {
  USHORT *usRegHoldingBuf;
  usRegHoldingBuf = usSRegHoldBuf;
  rt_base_t level;

  while (1) {
    /* Test Modbus Master */
    level = rt_hw_interrupt_disable();

    smt_io_get(&usRegHoldingBuf[ESC_BOARD_BUF_IOVALUE_REGIN], ESC_BOARD_IO_GET_BEGIN, ESC_BOARD_IO_GET_OVER);
    smt_io_get(&usRegHoldingBuf[TOF2_BOARD_BUF_KEYVALUE_BEGIN], TOF2_BOARD_KEY_GET_BEGIN, TOF2_BOARD_KEY_GET_OVER);
    smt_io_get(&usRegHoldingBuf[KEY_BOARD_BUF_KEYVALUE_BEGIN], KEY_BOARD_KEY_GET_BEGIN, KEY_BOARD_KEY_GET_OVER);

    smt_adc_get(&usRegHoldingBuf[ESC_BOARD_BUF_ADCVALUE_REGIN], SMT_ADC_NUM);    
    smt_tof2_test(&usRegHoldingBuf[TOF2_BOARD_BUF_TOFVALUE_BEGIN]);
    
    rt_hw_interrupt_enable(level);
    rt_thread_mdelay(1000);
    ledToggle();
  }
}

static void mb_slave_poll(void *parameter)
{
    if (rt_strstr(parameter, "RTU"))
    {
#ifdef PKG_MODBUS_SLAVE_RTU
        eMBInit(MB_RTU, SLAVE_ADDR, PORT_NUM, PORT_BAUDRATE, PORT_PARITY);
#else
        rt_kprintf("Error: Please open RTU mode first");
#endif
    }
    else if (rt_strstr(parameter, "ASCII"))
    {
#ifdef PKG_MODBUS_SLAVE_ASCII
        eMBInit(MB_ASCII, SLAVE_ADDR, PORT_NUM, PORT_BAUDRATE, PORT_PARITY);
#else
        rt_kprintf("Error: Please open ASCII mode first");
#endif
    }
    else if (rt_strstr(parameter, "TCP"))
    {
#ifdef PKG_MODBUS_SLAVE_TCP
        eMBTCPInit(0);
#else
        rt_kprintf("Error: Please open TCP mode first");
#endif
    }
    else
    {
        rt_kprintf("Error: unknown parameter");
    }
    eMBEnable();
    while (1)
    {
        eMBPoll();
        rt_thread_mdelay(MB_POLL_CYCLE_MS);
    }
}

int mb_slave_sample(void)
{
    static rt_uint8_t is_init = 0;
    rt_thread_t tid1 = RT_NULL, tid2 = RT_NULL;

    tid1 = rt_thread_create("md_s_poll", mb_slave_poll, "RTU", 1024, MB_POLL_THREAD_PRIORITY, 10);
    if (tid1 != RT_NULL)
    {
        rt_thread_startup(tid1);
    }
    else
    {
        goto __exit;
    }

    tid2 = rt_thread_create("md_s_send", send_thread_entry, RT_NULL, 512, MB_SEND_THREAD_PRIORITY, 10);
    if (tid2 != RT_NULL)
    {
        rt_thread_startup(tid2);
    }
    else
    {
        goto __exit;
    }

    is_init = 1;
    return RT_EOK;

__exit:
    if (tid1)
        rt_thread_delete(tid1);
    if (tid2)
        rt_thread_delete(tid2);

    return -RT_ERROR;
}

