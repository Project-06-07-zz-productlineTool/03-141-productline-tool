/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-05-04     RealThread   first version
 */

#include <rtthread.h>
#include <board.h>
#include <drv_common.h>

void rt_hw_board_init()
{
    extern void hw_board_init(char *clock_src, int32_t clock_src_freq, int32_t clock_target_freq);

    /* Heap initialization */
#if defined(RT_USING_HEAP)
    rt_system_heap_init((void *) HEAP_BEGIN, (void *) HEAP_END);
#endif

    hw_board_init(BSP_CLOCK_SOURCE, BSP_CLOCK_SOURCE_FREQ_MHZ, BSP_CLOCK_SYSTEM_FREQ_MHZ);

    /* Set the shell console output device */
#if defined(RT_USING_DEVICE) && defined(RT_USING_CONSOLE)
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

    /* Board underlying hardware initialization */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

}

// void HAL_PCD_MspInit()
// {
// 	GPIO_InitTypeDef  GPIO_InitStruct;
// 	__HAL_RCC_GPIOA_CLK_ENABLE();                   //使能GPIOA时钟
// 	__HAL_RCC_USB_OTG_FS_CLK_ENABLE();              //使能OTG FS时钟

// 	//配置PA11 12
// 	GPIO_InitStruct.Pin=GPIO_PIN_11|GPIO_PIN_12;    //PA11 12
// 	GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;           //复用
// 	GPIO_InitStruct.Pull=GPIO_NOPULL;               //无上下拉
// 	GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;          //高速
// 	GPIO_InitStruct.Alternate=GPIO_AF10_OTG_FS;     //复用为OTG FS
// 	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);         //初始化
// }


// /**
// * @brief PCD MSP Initialization
// * This function configures the hardware resources used in this example
// * @param hpcd: PCD handle pointer
// * @retval None
// */
// void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
// {
//   GPIO_InitTypeDef GPIO_InitStruct = {0};
//   if(hpcd->Instance==USB_OTG_FS)
//   {
//   /* USER CODE BEGIN USB_OTG_FS_MspInit 0 */

//   /* USER CODE END USB_OTG_FS_MspInit 0 */

//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     /**USB_OTG_FS GPIO Configuration
//     PA11     ------> USB_OTG_FS_DM
//     PA12     ------> USB_OTG_FS_DP
//     */
//     GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
//     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//     GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//     /* Peripheral clock enable */
//     __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
//     /* USB_OTG_FS interrupt Init */
//     HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
//     HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
//   /* USER CODE BEGIN USB_OTG_FS_MspInit 1 */

//   /* USER CODE END USB_OTG_FS_MspInit 1 */
//   }

// }