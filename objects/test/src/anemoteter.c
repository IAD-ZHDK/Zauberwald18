#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <naos.h>

#define ANEMO_RESOLUTION 200

static volatile int anemo_pulse_count = 0;
static volatile unsigned long anemo_last_check = 0;

static void anemo_handler(void *_) { anemo_pulse_count++; }

double anemo_get() {
  // prepare mutex
  static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

  // get and reset pulses
  vTaskEnterCritical(&mux);
  int pulses = anemo_pulse_count;
  anemo_pulse_count = 0;
  vTaskExitCritical(&mux);

  // get elapsed time
  unsigned long elapsed_time = naos_millis() - anemo_last_check;

  // reset time
  anemo_last_check = naos_millis();

  // calculate rate
  double rate = ((float)pulses / (float)ANEMO_RESOLUTION) * (1000.0 / (float)elapsed_time);

  return rate;
}

void anemo_init(uint8_t pinSelect) {
  gpio_num_t pin = GPIO_NUM_33;

  gpio_config_t rc;
  rc.pin_bit_mask = GPIO_SEL_33;  //
  if (pinSelect == 1) {
    pin = GPIO_NUM_19;
    rc.pin_bit_mask = GPIO_SEL_19;  //
  }
  // configure rotation pin
  rc.mode = GPIO_MODE_INPUT;
  rc.intr_type = GPIO_INTR_NEGEDGE;  // only on falling edge
  rc.pull_up_en = GPIO_PULLUP_ENABLE;
  rc.pull_down_en = GPIO_PULLDOWN_DISABLE;
  ESP_ERROR_CHECK(gpio_config(&rc));

  // add interrupt handlers
  ESP_ERROR_CHECK(gpio_isr_handler_add(pin, anemo_handler, NULL));
}
