#include <driver/gpio.h>
#include <stdint.h>

#define NEO3_DEFAULT_PIN GPIO_NUM_13

typedef struct {
  uint8_t r, g, b;
} neo3_pixel_t;

void neo3_init(int length, gpio_num_t pin);

void neo3_set_one(int i, uint8_t r, uint8_t g, uint8_t b);

void neo3_set_range(uint8_t r, uint8_t g, uint8_t b, int first, int last);

void neo3_set_all(uint8_t r, uint8_t g, uint8_t b);

void neo3_show();
