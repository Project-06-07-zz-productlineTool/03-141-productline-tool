#ifndef _SMT_IO_GET_
#define _SMT_IO_GET_

#include "rtdef.h"

// ESC IO = 5 
// tof2 IO = 2
#define IO_NUM 7

int smt_io_get(rt_uint16_t *io_data,rt_uint8_t size);

#endif 
