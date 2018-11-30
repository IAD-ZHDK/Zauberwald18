// Wind (Berg & Tal): Chen, Lopez

#include <naos.h>
#include "apa.h"

void object2_setup() {
  // init apa pixels
  apa_init(5, APA_DEFAULT_CLOCK_PIN, APA_DEFAULT_DATA_PIN);
}

double object2_loop() {
  // set pixels
  apa_set_all(0, 0, 255);
  apa_show();

  return 0;
}
