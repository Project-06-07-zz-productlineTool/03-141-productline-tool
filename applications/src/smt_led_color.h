#ifndef _SMT_LED_TEST_
#define _SMT_LED_TEST_

#include "rtdef.h"

void smt_led_colour_check(rt_uint16_t *result);
void smt_led_board_init(void);

void taskSmtLedInit(void);

#endif