#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <naos.h>

#include "enc.h"

#define ENC_RESOLUTION 200

static volatile int16_t enc_total = 0;

static void enc_rotation_handler(void *_) {
  // increment
  enc_total++;
}

void enc_init() {
  // configure rotation pins
  gpio_config_t rc;
  rc.pin_bit_mask = GPIO_SEL_23;
  rc.mode = GPIO_MODE_INPUT;
  rc.intr_type = GPIO_INTR_POSEDGE;
  rc.pull_up_en = GPIO_PULLUP_ENABLE;
  rc.pull_down_en = GPIO_PULLDOWN_DISABLE;
  gpio_config(&rc);

  // add interrupt handlers
  gpio_isr_handler_add(GPIO_NUM_23, enc_rotation_handler, NULL);
}

double enc_get() {
  // prepare mutex
  static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

  // get saved total
  vTaskEnterCritical(&mux);
  double total = enc_total;
  enc_total = 0;
  vTaskExitCritical(&mux);

  // calculate and return real rotation
  return total / ENC_RESOLUTION;
}
