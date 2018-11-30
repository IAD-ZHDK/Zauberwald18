#include <stdint.h>

typedef struct {
  uint8_t r, g, b, w;
} neo4_pixel_t;

void neo4_init(int length);

void neo4_set_one(int i, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

void neo4_set_all(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

void neo4_show();
