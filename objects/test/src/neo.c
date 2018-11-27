#include "neo.h"
#include <driver/rmt.h>

#define NEO_PIXEL_COUNT 30

rmt_item32_t neo_items[NEO_PIXEL_COUNT * 24 + 1];

neo_pixel_t neo_pixels[NEO_PIXEL_COUNT];

void neo_init() {
  // prepare config
  rmt_config_t config;
  config.rmt_mode = RMT_MODE_TX;
  config.channel = RMT_CHANNEL_0;
  config.gpio_num = GPIO_NUM_13;
  config.mem_block_num = 1;
  config.clk_div = 8;
  config.tx_config.loop_en = 0;
  config.tx_config.carrier_en = 0;
  config.tx_config.idle_output_en = 1;
  config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
  config.tx_config.carrier_freq_hz = 100;
  config.tx_config.carrier_level = RMT_CARRIER_LEVEL_LOW;
  config.tx_config.carrier_duty_percent = 50;

  // configure rmt controller
  ESP_ERROR_CHECK(rmt_config(&config));

  // install rmt driver
  ESP_ERROR_CHECK(rmt_driver_install(RMT_CHANNEL_0, 0, 0));

  // clear all pixels
  neo_set_all(0, 0, 0);
}

void neo_set_one(int i, uint8_t r, uint8_t g, uint8_t b) {
  neo_pixels[i].r = r;
  neo_pixels[i].g = g;
  neo_pixels[i].b = b;
}

void neo_set_all(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    neo_set_one(i, r, g, b);
  }
}

void neo_show() {
  // prepare iterator
  rmt_item32_t* item = neo_items;

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    // get pixel value
    uint32_t pixel = (neo_pixels[i].g << 16) | (neo_pixels[i].r << 8) | neo_pixels[i].b;

    // update rmt items
    for (int j = 23; j >= 0; j--) {
      if (pixel & (1 << j)) {
        // write high
        item->level0 = 1;
        item->duration0 = 9;  // 900ns (900ns +/- 150ns per datasheet)
        item->level1 = 0;
        item->duration1 = 3;  // 300ns (350ns +/- 150ns per datasheet)
      } else {
        // write low
        item->level0 = 1;
        item->duration0 = 3;  // 300ns (350ns +/- 150ns per datasheet)
        item->level1 = 0;
        item->duration1 = 9;  // 900ns (900ns +/- 150ns per datasheet)
      }

      // advance iterator
      item++;
    }
  }

  // write terminator
  item->level0 = 0;
  item->duration0 = 0;
  item->level1 = 0;
  item->duration1 = 0;

  // show the pixels
  ESP_ERROR_CHECK(rmt_write_items(RMT_CHANNEL_0, neo_items, NEO_PIXEL_COUNT * 24, true));
}