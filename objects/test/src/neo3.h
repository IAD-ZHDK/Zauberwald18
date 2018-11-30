#include <stdint.h>

typedef struct {
  uint8_t r, g, b;
} neo3_pixel_t;

void neo3_init(int length);

void neo3_set_one(int i, uint8_t r, uint8_t g, uint8_t b);

void neo3_set_all(uint8_t r, uint8_t g, uint8_t b);

void neo3_show();
