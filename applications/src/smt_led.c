#include <rtdevice.h>
#include <rtthread.h>

#define TASK_LED_THREAD_PRIORITY RT_THREAD_PRIORITY_MAX - 1

#define GY33_I2C_BUS_NAME "i2c1" /* 传感器连接的I2C总线设备名称 */
#define GY33_IIC_ADDR 0x5A       /* 从机地址 */

#define GY33_REG_LIGHT_VALUE 0x08
#define GY33_REG_LIGHT_VALUE_SIZE 4
#define GY33_REG_COLOUR_VALUE 0x0f
#define GY33_REG_COLOUR_VALUE_SIZE 1
#define GY33_REG_RGB_VALUE 0x0c
#define GY33_REG_RGB_VALUE_SIZE 3

#define LED_RED "PA.5"
#define LED_GREEN "PA.6"
#define LED_BLUE "PA.7"

enum LED_COLOUR_e { RED, GREEN, BULE, WHITE };

static struct rt_i2c_bus_device *i2c_bus = RT_NULL; /* I2C总线设备句柄 */

rt_base_t pin_red = 0;
rt_base_t pin_green = 0;
rt_base_t pin_blue = 0;

rt_uint16_t rgb_value[GY33_REG_RGB_VALUE_SIZE] = {0};

/* 写传感器寄存器 */
static rt_err_t set_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t data) {
  rt_uint8_t buf[3];
  struct rt_i2c_msg msgs;

  buf[0] = reg;  // cmd
  buf[1] = data;
  // buf[2] = data[1];

  msgs.addr = GY33_IIC_ADDR;
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

/* 写传感器寄存器 */
static rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint16_t reg, rt_uint8_t *data) {
  rt_uint8_t buf[3];
  struct rt_i2c_msg msgs;

  buf[0] = reg;  // cmd
  // buf[1] = reg >> 8;

  msgs.addr = GY33_IIC_ADDR;
  msgs.flags = RT_I2C_WR;
  msgs.buf = buf;
  msgs.len = 1;

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

  msgs.addr = GY33_IIC_ADDR;
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

void smt_led_board_init(void) {
  pin_red = rt_pin_get(LED_RED);
  pin_green = rt_pin_get(LED_GREEN);
  pin_blue = rt_pin_get(LED_BLUE);
  rt_pin_mode(pin_red, PIN_MODE_OUTPUT);
  rt_pin_mode(pin_green, PIN_MODE_OUTPUT);
  rt_pin_mode(pin_blue, PIN_MODE_OUTPUT);
}

static void led_ctrl(enum LED_COLOUR_e led_colour) {
  switch (led_colour) {
    case RED:
      rt_pin_write(pin_red, 1);
      rt_pin_write(pin_green, 0);
      rt_pin_write(pin_blue, 0);
      break;
    case GREEN:
      rt_pin_write(pin_red, 0);
      rt_pin_write(pin_green, 1);
      rt_pin_write(pin_blue, 0);
      break;
    case BULE:
      rt_pin_write(pin_red, 0);
      rt_pin_write(pin_green, 0);
      rt_pin_write(pin_blue, 1);
      break;
    case WHITE:
      rt_pin_write(pin_red, 1);
      rt_pin_write(pin_green, 1);
      rt_pin_write(pin_blue, 1);
    default:
      break;
  }
}

void smt_led_colour_check(rt_uint16_t *result) {
  for (rt_uint8_t i = 0; i < GY33_REG_RGB_VALUE_SIZE; i++) {
    result[i] = rgb_value[i];
  }
}

static void rgbValueSet(rt_uint16_t *buf_rgb) {
  for (rt_uint8_t i = 0; i < GY33_REG_RGB_VALUE_SIZE; i++) {
    rgb_value[i] = buf_rgb[i];
  }
}

// 中值滤波函数
rt_uint16_t median_filter(rt_uint16_t *data, rt_uint8_t length) {
  // 如果数组长度为0，返回0
  if (length == 0) {
    return 0;
  }

  // 创建一个临时数组来存放数据
  rt_uint16_t *temp = (rt_uint16_t *)rt_malloc(length * sizeof(rt_uint16_t));
  if (temp == NULL) {
    // fprintf(stderr, "Memory allocation failed.\n");
    return 0;
  }

  // 将输入数据复制到临时数组
  for (rt_uint8_t i = 0; i < length; i++) {
    temp[i] = data[i];
  }

  // 排序临时数组
  for (rt_uint8_t i = 0; i < length - 1; i++) {
    for (rt_uint8_t j = i + 1; j < length; j++) {
      if (temp[i] > temp[j]) {
        rt_uint16_t t = temp[i];
        temp[i] = temp[j];
        temp[j] = t;
      }
    }
  }

  // 计算中值
  rt_uint16_t median;
  if (length % 2 == 0) {
    median = temp[length / 2 - 1];  // 偶数长度，返回中间的较左边的值
  } else {
    median = temp[length / 2];  // 奇数长度，返回中间的值
  }

  // 释放临时数组内存
  rt_free(temp);
  return median;
}

void get_colcur_time(rt_uint16_t *buf, rt_uint8_t times) {
  rt_uint8_t buf_temp_all_reg[16] = {0};
  for (rt_uint8_t i = 0; i < times; i++) {
    write_reg(i2c_bus, 0x00, 0);
    read_regs(i2c_bus, 16, buf_temp_all_reg);
    buf[i] = buf_temp_all_reg[0x0a] << 8 | buf_temp_all_reg[0x0b];
    rt_thread_delay(100);
  }
}

#define FILTER_TIMES 10

static void task_smt_led_entry() {
  rt_uint8_t temp_light[GY33_REG_LIGHT_VALUE_SIZE] = {0};
  rt_uint8_t temp_colour[GY33_REG_COLOUR_VALUE_SIZE] = {0};
  rt_uint8_t temp_rgb[GY33_REG_RGB_VALUE_SIZE] = {0};
  rt_uint16_t led_colcur_temperature[3] = {0};
  rt_uint16_t led_colcur_stash = 0;
  rt_uint8_t buf_temp_all_reg[16] = {0};
  rt_uint16_t filter_buf[FILTER_TIMES] = {0};

  i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(GY33_I2C_BUS_NAME);

  if (i2c_bus == RT_NULL) {
    rt_kprintf("can't find device!\n");
  }
  rt_uint8_t cmd_calibration_reg = 0x10;
  rt_uint8_t cmd_calibration_data = 0x31;
  led_ctrl(WHITE);
  rt_thread_delay(100);
  set_reg(i2c_bus, cmd_calibration_reg, 0Xa1); /* 发送命令 */
  rt_thread_delay(1000);

  while (1) {
    led_ctrl(RED);
    get_colcur_time(filter_buf, FILTER_TIMES);
    led_colcur_temperature[0] = median_filter(filter_buf, FILTER_TIMES);
    rgbValueSet(led_colcur_temperature);

    led_ctrl(GREEN);
    get_colcur_time(filter_buf, FILTER_TIMES);
    led_colcur_temperature[1] = median_filter(filter_buf, FILTER_TIMES);
    rgbValueSet(led_colcur_temperature);

    led_ctrl(BULE);
    get_colcur_time(filter_buf, FILTER_TIMES);
    led_colcur_temperature[2] = median_filter(filter_buf, FILTER_TIMES);
    rgbValueSet(led_colcur_temperature);
  }

  while (1) {
    led_ctrl(RED);
    rt_thread_delay(100);
    write_reg(i2c_bus, 0x00, 0);
    read_regs(i2c_bus, 16, buf_temp_all_reg);
    led_colcur_stash = buf_temp_all_reg[0x0a] << 8 | buf_temp_all_reg[0x0b];
    if (led_colcur_stash < 30000) {
      led_colcur_temperature[0] = led_colcur_stash;
    }

    led_ctrl(GREEN);
    rt_thread_delay(100);
    write_reg(i2c_bus, 0x00, 0);
    read_regs(i2c_bus, 16, buf_temp_all_reg);
    led_colcur_stash = buf_temp_all_reg[0x0a] << 8 | buf_temp_all_reg[0x0b];

    if (led_colcur_stash < 30000) {
      led_colcur_temperature[1] = led_colcur_stash;
    }

    led_ctrl(BULE);
    rt_thread_delay(100);
    write_reg(i2c_bus, 0x00, 0);
    read_regs(i2c_bus, 16, buf_temp_all_reg);
    led_colcur_stash = buf_temp_all_reg[0x0a] << 8 | buf_temp_all_reg[0x0b];

    if (led_colcur_stash < 30000) {
      led_colcur_temperature[2] = led_colcur_stash;
    }

    rgbValueSet(led_colcur_temperature);
  }
}

void taskSmtLedInit(void) {
  rt_thread_t tid1 = RT_NULL;

  tid1 = rt_thread_create("task_stmled", task_smt_led_entry, NULL, 512, TASK_LED_THREAD_PRIORITY, 10);
  if (tid1 != RT_NULL) {
    rt_thread_startup(tid1);
  } else {
    goto __exit;
  }

  return RT_EOK;

__exit:
  if (tid1) rt_thread_delete(tid1);

  return -RT_ERROR;
}
