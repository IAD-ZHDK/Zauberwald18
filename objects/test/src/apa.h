#include <driver/gpio.h>
#include <stdint.h>

#define APA_DEFAULT_DATA_PIN GPIO_NUM_13
#define APA_DEFAULT_CLOCK_PIN GPIO_NUM_25

typedef struct {
  uint8_t r, g, b;
} apa_pixel_t;

void apa_init(int length, gpio_num_t clock, gpio_num_t data);

void apa_set_one(int i, uint8_t r, uint8_t g, uint8_t b);

void apa_set_all(uint8_t r, uint8_t g, uint8_t b);

void apa_show();
