// Wasser (Kugelbahn)

#include <naos.h>

#include "apds.h"

void object5_setup() {
  // init apds chip
  apds_init();
}

double object5_loop() {
  // read red light
  apds_color_t c = apds_read();
  naos_log("c: %d, r: %d, g: %d, b: %d", c.c, c.r, c.g, c.b);

  return 0;
}
