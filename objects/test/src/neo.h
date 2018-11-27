#include <stdint.h>

typedef struct {
  uint8_t r, g, b;
} neo_pixel_t;

void neo_init();

void neo_set_one(int i, uint8_t r, uint8_t g, uint8_t b);

void neo_set_all(uint8_t r, uint8_t g, uint8_t b);

void neo_show();
