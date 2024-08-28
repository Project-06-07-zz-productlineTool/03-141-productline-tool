/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <board.h>
#include <rtconfig.h>

/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev stm32_onchip_flash_64k;

/* flash device table */
#define FAL_FLASH_DEV_TABLE    \
  {                            \
      &stm32_onchip_flash_64k, \
  }
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                          \
  {                                                                             \
      {FAL_PART_MAGIC_WORD, "data", "onchip_flash_64k", 0, 64 * 1024, 0}, \
  }
#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
