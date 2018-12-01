#include <driver/i2c.h>
#include <naos.h>

#include "apds.h"

#define APDS_I2C_ADDR 0x39

static void apds_write_data(uint8_t reg, uint8_t *buf, int len) {
  // prepare command
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();

  // generate start sequence
  ESP_ERROR_CHECK(i2c_master_start(cmd));

  // write address
  ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (APDS_I2C_ADDR << 1) | I2C_MASTER_WRITE, true));

  // write register
  ESP_ERROR_CHECK(i2c_master_write_byte(cmd, reg, true));

  // write data
  for (int i = 0; i < len; i++) {
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, buf[i], true));
  }

  // generate stop sequence
  ESP_ERROR_CHECK(i2c_master_stop(cmd));

  // issue config command
  ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 2000 / portTICK_RATE_MS));

  // cleanup command
  i2c_cmd_link_delete(cmd);
}

static void apds_write_byte(uint8_t reg, uint8_t val) {
  // write one byte
  apds_write_data(reg, &val, 1);
}

static void apds_read_data(uint8_t reg, uint8_t *buf, int len) {
  // prepare command
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();

  // generate start sequence
  ESP_ERROR_CHECK(i2c_master_start(cmd));

  // write address
  ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (APDS_I2C_ADDR << 1) | I2C_MASTER_WRITE, true));

  // write register
  ESP_ERROR_CHECK(i2c_master_write_byte(cmd, reg, true));

  // generate start sequence
  ESP_ERROR_CHECK(i2c_master_start(cmd));

  // write address
  ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (APDS_I2C_ADDR << 1) | I2C_MASTER_READ, true));

  // read data
  for (int i = 0; i < len; i++) {
    ESP_ERROR_CHECK(i2c_master_read_byte(cmd, (buf + i), i == len - 1));
  }

  // generate stop sequence
  ESP_ERROR_CHECK(i2c_master_stop(cmd));

  // issue config command
  ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 2000 / portTICK_RATE_MS));

  // cleanup command
  i2c_cmd_link_delete(cmd);
}

static void apds_read_two_bytes(uint8_t reg, uint16_t *val) {
  // read registers
  uint8_t ret[2];
  apds_read_data(reg, ret, 2);

  // assemble value
  *val = (ret[1] << 8) | ret[0];
}

void apds_init() {
  // prepare i2c config
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = GPIO_NUM_21;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_io_num = GPIO_NUM_22;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = 100000;

  // configure i2c port
  ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));

  // install i2c driver
  ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0));

  // disable device
  apds_write_byte(0x80, 0x0);

  // set adc integration time to 2.78ms
  apds_write_byte(0x81, 0xFF);

  // set wait time to 2.78ms
  apds_write_byte(0x82, 0xFF);

  // set config one (no wait)
  apds_write_byte(0x8D, 0x60);

  // set color gain to 4x
  apds_write_byte(0x8F, 0x1);

  // set config two
  apds_write_byte(0x90, 0x1);

  // set config three (no SAI)
  apds_write_byte(0x9F, 0x0);

  // clear all interrupts
  apds_write_data(0xE7, NULL, 0);

  // enable power and ambient light sensor
  apds_write_byte(0x80, 0x3);
}

apds_color_t apds_read() {
  // prepare color
  apds_color_t c;

  // read colors
  apds_read_two_bytes(0x94, &c.c);
  apds_read_two_bytes(0x96, &c.r);
  apds_read_two_bytes(0x98, &c.g);
  apds_read_two_bytes(0x9A, &c.b);

  return c;
}
