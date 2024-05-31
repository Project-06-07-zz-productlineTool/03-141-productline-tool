#include <rtdevice.h>
#include <rtthread.h>

static void io_check(void) {
  rt_base_t pb0_pin = rt_pin_get("PB.0");
  rt_pin_mode(pb0_pin, PIN_MODE_INPUT);
  rt_int8_t pb0_value = rt_pin_read(pb0_pin);
  rt_kprintf("PB.0 value is %d\r\n", pb0_value);

  rt_base_t pb1_pin = rt_pin_get("PB.1");
  rt_pin_mode(pb1_pin, PIN_MODE_INPUT);
  rt_int8_t pb1_value = rt_pin_read(pb1_pin);
  rt_kprintf("PB.1 value is %d\r\n", pb1_value);

  rt_base_t pb2_pin = rt_pin_get("PB.2");
  rt_pin_mode(pb2_pin, PIN_MODE_INPUT);
  rt_int8_t pb2_value = rt_pin_read(pb2_pin);
  rt_kprintf("PB.2 value is %d\r\n", pb2_value);

  rt_base_t pb3_pin = rt_pin_get("PB.3");
  rt_pin_mode(pb3_pin, PIN_MODE_INPUT);
  rt_int8_t pb3_value = rt_pin_read(pb3_pin);
  rt_kprintf("PB.3 value is %d\r\n", pb3_value);

  rt_base_t pb4_pin = rt_pin_get("PB.4");
  rt_pin_mode(pb4_pin, PIN_MODE_INPUT);
  rt_int8_t pb4_value = rt_pin_read(pb4_pin);
  rt_kprintf("PB.4 value is %d\r\n", pb4_value);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(io_check, pin beep sample);
