#ifndef _SMT_ADC_GET_
#define _SMT_ADC_GET_

#include "rtdef.h"

int smt_adc_init(void);

int smt_adc_get(rt_uint16_t *io_data,rt_uint8_t size);

#endif 
