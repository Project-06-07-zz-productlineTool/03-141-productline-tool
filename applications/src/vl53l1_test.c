#include <rtdevice.h>
#include <rtthread.h>

#define VL53l1CB_I2C_BUS_NAME "i2c1" /* 传感器连接的I2C总线设备名称 */
#define VL53l1CB_IIC_ADDR 0x29       /* 从机地址 */
#define VL53l1CB_ID_REG 0x0F01
#define VL53l1CB_ID_RESULT 0xCCEA

static struct rt_i2c_bus_device *i2c_bus = RT_NULL; /* I2C总线设备句柄 */
/* 写传感器寄存器 */
static rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint16_t reg, rt_uint8_t *data) {
  rt_uint8_t buf[3];
  struct rt_i2c_msg msgs;

  buf[0] = reg;  // cmd
  buf[1] = reg >> 8;

  msgs.addr = VL53l1CB_IIC_ADDR;
  msgs.flags = RT_I2C_WR;
  msgs.buf = buf;
  msgs.len = 2;

  /* 调用I2C设备接口传输数据 */
  if (rt_i2c_transfer(bus, &msgs, 1) == 1) {
    return RT_EOK;
  } else {
    return -RT_ERROR;
  }
}

/* 读传感器寄存器数据 */
static rt_err_t read_regs(struct rt_i2c_bus_device *bus, rt_uint8_t len, rt_uint8_t *buf) {
  struct rt_i2c_msg msgs;

  msgs.addr = VL53l1CB_IIC_ADDR;
  msgs.flags = RT_I2C_RD;
  msgs.buf = buf;
  msgs.len = len;

  /* 调用I2C设备接口传输数据 */
  if (rt_i2c_transfer(bus, &msgs, 1) == 1) {
    return RT_EOK;
  } else {
    return -RT_ERROR;
  }
}

void smt_tof2_test(rt_uint16_t *result) {
  rt_uint8_t temp[2] = {0, 0};
  rt_uint16_t result_temp;
  /* 查找I2C总线设备，获取I2C总线设备句柄 */
  i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(VL53l1CB_I2C_BUS_NAME);

  write_reg(i2c_bus, VL53l1CB_ID_REG, 0); /* 发送命令 */
  rt_thread_mdelay(10);
  read_regs(i2c_bus, 2, temp); /* 获取传感器数据 */

  result_temp = temp[0] | temp[1] << 8;
  if (VL53l1CB_ID_RESULT == result_temp) {
    *result = 1;
  } else {
    *result = 0;
  }
}