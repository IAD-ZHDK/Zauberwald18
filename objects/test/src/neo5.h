#include <driver/gpio.h>
#include <stdint.h>

#define NEO5_DEFAULT_PIN GPIO_NUM_15

typedef struct {
    uint8_t r, g, b;
} neo5_pixel_t;

void neo5_init(int length, gpio_num_t pin);

void neo5_set_one(int i, uint8_t r, uint8_t g, uint8_t b);

void neo5_set_all(uint8_t r, uint8_t g, uint8_t b);

void neo5_show();
