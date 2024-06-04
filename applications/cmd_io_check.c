#include <rtdevice.h>
#include <rtthread.h>

#define IO_NUM 5

static void io_check(void) {
  rt_bool_t io_check_result = 1;
  rt_int8_t io_value[5] = {0};
  rt_base_t pb0_pin = rt_pin_get("PB.0");
  rt_base_t io_num_temp = pb0_pin;

  for (rt_int8_t i = 0; i < IO_NUM; i++) {
    io_num_temp = i + pb0_pin;
    rt_pin_mode(io_num_temp, PIN_MODE_INPUT);
    io_value[i] = rt_pin_read(io_num_temp);
    if (io_value[i] != 1) {
      io_check_result = 0;
    }
  }

  if (io_check_result == 0) {
    rt_kprintf("io_check fail!\r\n");
    for (rt_int8_t i = 0; i < IO_NUM; i++) {
      if (io_value[i] != 1) {
        rt_kprintf("PB%d no equal 1\r\n", i);
      }
    }
  } else {
    rt_kprintf("io_check success\r\n");
  }
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(io_check, pin beep sample);
