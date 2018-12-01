#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint16_t c, r, g, b;
} apds_color_t;

/**
 * Initializes the APDS9960 sensor and color reading.
 */
void apds_init();

/**
 * Read the color registers.
 *
 * @return The currently sensed color.
 */
apds_color_t apds_read();
