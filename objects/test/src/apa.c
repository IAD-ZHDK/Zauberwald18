#include <driver/spi_master.h>

#include "apa.h"

int apa_length;

apa_pixel_t* apa_pixels;

spi_device_handle_t apa_device;

// TODO: Make async if too slow.

void apa_init(int length, gpio_num_t clock, gpio_num_t data) {
  // set length
  apa_length = length;

  // allocate pixels
  apa_pixels = calloc((size_t)length, sizeof(apa_pixel_t));

  // prepare bus config
  spi_bus_config_t bus = {
      .miso_io_num = -1,
      .mosi_io_num = data,
      .sclk_io_num = clock,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      //.max_transfer_sz = 4,
  };

  // prepare interface config
  spi_device_interface_config_t dev = {
      .mode = 0,
      .clock_speed_hz = 1000000, // TODO: Faster?
      .spics_io_num = -1,
      .queue_size = 1,
  };

  // Initialize the SPI bus
  ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, &bus, 1));

  // Attach the LCD to the SPI bus
  ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, &dev, &apa_device));

  // clear all pixels
  apa_set_all(0, 0, 0);
}

void apa_set_one(int i, uint8_t r, uint8_t g, uint8_t b) {
  apa_pixels[i].r = r;
  apa_pixels[i].g = g;
  apa_pixels[i].b = b;
}

void apa_set_all(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < apa_length; i++) {
    apa_set_one(i, r, g, b);
  }
}

void apa_show() {
  // send start frame marker
  spi_transaction_t start = {
      .flags = SPI_TRANS_USE_TXDATA,
      .tx_data = { 0x00, 0x00, 0x00, 0x00 },
      .length = 32,
  };
  ESP_ERROR_CHECK(spi_device_transmit(apa_device, &start));

  // prepare pixel transaction
  spi_transaction_t pixel = {
      .flags = SPI_TRANS_USE_TXDATA,
      .tx_data = {0xFF, 0x00, 0x00, 0x00},
      .length = 32,
  };

  // update all pixels
  for(int i=apa_length-1; i>=0; i--) {
    // set color
    pixel.tx_data[1] = apa_pixels[i].b;
    pixel.tx_data[2] = apa_pixels[i].g;
    pixel.tx_data[3] = apa_pixels[i].r;

    // send pixel
    ESP_ERROR_CHECK(spi_device_transmit(apa_device, &pixel));
  }

  // prepare send frame
  spi_transaction_t end = {
      .flags = SPI_TRANS_USE_TXDATA,
      .tx_data = { 0x00, 0x00, 0x00, 0x00 },
      .length = 32,
  };
  ESP_ERROR_CHECK(spi_device_transmit(apa_device, &end));
}
