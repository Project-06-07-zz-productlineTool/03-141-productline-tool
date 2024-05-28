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
#define CHANNEL_NUM 6

static rt_uint32_t true_value_sheet[CHANNEL_NUM] = {2750,1650,2200,2750,1650,1650};
static rt_uint32_t err_range = 100;

rt_uint32_t adc_vol[CHANNEL_NUM] = {0};
rt_adc_device_t adc_dev;

void match_and_report(void) {
    
    rt_err_t ret = RT_EOK;
    rt_uint32_t value;

    for (rt_uint8_t i = 0; i < CHANNEL_NUM;i++)
    {
        ret = rt_adc_enable(adc_dev, i);
        value = rt_adc_read(adc_dev, i);
        adc_vol[i] = value * REFER_VOLTAGE / CONVERT_BITS;
        ret = rt_adc_disable(adc_dev, i);

        if(abs(true_value_sheet[i] - adc_vol[i]) <  err_range)
        {
            rt_kprintf("channel %d check pass, value is %d mv\r\n",i,adc_vol[i]);
        }
        else 
        {
            rt_kprintf("channel %d check error, value is %d mv\r\n",i,adc_vol[i]);
        }
        
    }
}

static int adc_check(int argc, char *argv[])
{
 
    rt_uint32_t value, vol;
    rt_err_t ret = RT_EOK;

    /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }

    match_and_report();

    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(adc_check, adc voltage convert sample);
