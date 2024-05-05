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
    int count = 1;
    change_shell();
    while (count++)
    {
//        LOG_D("Hello RT-Thread!");
//        rt_kprintf("this is rt_kprintf");
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}

// #include <rtthread.h>
// #include <rtdevice.h>

// rt_device_t dev = RT_NULL;
// static struct rt_semaphore rx_sem;

// static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
// {
//     /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
//     rt_sem_release(&rx_sem);
//     rt_kprintf("rx:%d...\n", size);
//     return RT_EOK;
// }

// static void serial_thread_entry(void *parameter)
// {
//     char ch;

//     while (1)
//     {
//         /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
//         while (rt_device_read(dev, -1, &ch, 1) != 1)
//         {
//             /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
//             rt_sem_take(&rx_sem, RT_WAITING_FOREVER);

//         }
//         rt_kprintf("read:%d\n", ch);
//     }
// }


// int main(void)
// {

//     char buf[] = "hello rt-thread!\r\n";

//     dev = rt_device_find("vcom");

//     rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

//     if (dev)
//         // rt_device_open(dev, RT_DEVICE_FLAG_RDWR);
//         rt_device_open(dev, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
//     else
//         return -RT_ERROR;

//     rt_device_set_rx_indicate(dev, uart_input);

//     rt_thread_t thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 25, 10);
//     rt_thread_startup(thread);

//     while (1)
//     {
//         rt_device_write(dev, 0, buf, rt_strlen(buf));
//         rt_thread_mdelay(1000);

//     }

//     return RT_EOK;
// }
