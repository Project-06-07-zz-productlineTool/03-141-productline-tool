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

#include <rtthread.h>
#include <rtdevice.h>

#define ADC_DEV_NAME        "adc1"      /* ADC 设备名称 */
#define REFER_VOLTAGE       3300         /* 参考电压 3.3V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS        (1 << 12)   /* 转换位数为12位 */

rt_adc_device_t adc_dev_smt;

int smt_adc_init(void)
{
    rt_err_t ret = RT_EOK;
    /* 查找设备 */
    adc_dev_smt = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev_smt == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }

    return ret;
}

int smt_adc_get(rt_uint16_t *io_data,rt_uint8_t size)
{
    rt_err_t ret = RT_EOK;
    rt_uint32_t value;

    for (rt_uint8_t i = 0; i < size;i++)
    {
        ret = rt_adc_enable(adc_dev_smt, i);
        value = rt_adc_read(adc_dev_smt, i);
        io_data[i] = value * REFER_VOLTAGE / CONVERT_BITS;
        ret = rt_adc_disable(adc_dev_smt, i);
    }
}