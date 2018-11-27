#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <naos.h>

#define ENC_RESOLUTION 200
//#define ENC_PIN GPIO_NUM_33

static volatile float HRZ = 0;
static volatile int pulsCount = 0;
static volatile unsigned long lastHrzCheck = 0;

static void anemo_handler(void *_) { pulsCount++; }

float anemo_getHrz() {
  unsigned long timeElapsed = naos_millis() - lastHrzCheck;
  lastHrzCheck = naos_millis();
  HRZ = pulsCount;
  HRZ = (HRZ / ENC_RESOLUTION) * (1000 / timeElapsed);
  pulsCount = 0;
  return HRZ;
}

void anenmo_init() {
  // configure rotation pin
  gpio_config_t rc;
  rc.pin_bit_mask = GPIO_SEL_33;
  rc.mode = GPIO_MODE_INPUT;
  rc.intr_type = GPIO_INTR_NEGEDGE;  // only on falling edge
  rc.pull_up_en = GPIO_PULLUP_ENABLE;
  rc.pull_down_en = GPIO_PULLDOWN_DISABLE;
  gpio_config(&rc);
  // add interrupt handlers
  gpio_isr_handler_add(GPIO_NUM_33, anemo_handler, NULL);
}
