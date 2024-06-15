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
#include <vconsole.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static rt_device_t vcom_shell_device = NULL;
void change_shell(void) 
{
    vcom_shell_device = rt_device_find("vcom");
    if (vcom_shell_device) 
    {
        vconsole_switch(vcom_shell_device);
    }
}

int main(void)
{
    change_shell();

    return RT_EOK;
}
