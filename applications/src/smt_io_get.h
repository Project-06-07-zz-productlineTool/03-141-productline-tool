#ifndef _SMT_IO_GET_
#define _SMT_IO_GET_

#include "rtdef.h"

#define ESC_IO_NUM 5
#define TOF2_KEY_NUM 2
#define KEYBOARD_KEY_NUM 3

#define IO_NUM (ESC_IO_NUM + TOF2_KEY_NUM + KEYBOARD_KEY_NUM)

int smt_io_get(rt_uint16_t *io_data,rt_uint8_t begin_num, rt_uint8_t over_num);
void taskSmtIOPollInit(void);

#endif 
