// Solar 1 (Laserspiel): Buck, Lustenberger, Tanner

#include <art32/numbers.h>
#include <art32/smooth.h>
#include <driver/gpio.h>
#include <esp_system.h>
#include <naos.h>

#include "light.h"
#include "neo3.h"
#include "neo5.h"
#include "servo.h"

#define NUM_PIXELS 11
#define SUN_CHANGE 5000
#define JOYSTICK_SPEED 0.5
#define POWER_APPLY_RATE 0.01

static double power = 0;
static double position = 90;
static uint32_t active_sun = 1;
static uint32_t last_change = 0;

void object5_setup() {
  // init neo pixels
  neo3_init(29, NEO3_DEFAULT_PIN);  // this object just has 29 neopixels for ambient lighting

  // init lighting
  light_init(255, 200, 0, 0, 28);

  // initialize servos
  servo_setup(false);

  // initialize tower pixels and clear pixels
  neo5_init(NUM_PIXELS, NEO5_DEFAULT_PIN);
  neo5_show();

  // configure joystick left
  gpio_config_t left = {.pin_bit_mask = GPIO_SEL_32,
                        .mode = GPIO_MODE_INPUT,
                        .pull_up_en = GPIO_PULLUP_ENABLE,
                        .pull_down_en = GPIO_PULLDOWN_DISABLE,
                        .intr_type = GPIO_INTR_DISABLE};
  ESP_ERROR_CHECK(gpio_config(&left));

  // configure joystick right
  gpio_config_t right = {.pin_bit_mask = GPIO_SEL_33,
                         .mode = GPIO_MODE_INPUT,
                         .pull_up_en = GPIO_PULLUP_ENABLE,
                         .pull_down_en = GPIO_PULLDOWN_DISABLE,
                         .intr_type = GPIO_INTR_DISABLE};
  ESP_ERROR_CHECK(gpio_config(&right));

  // enable h-bridge
  gpio_config_t master = {.pin_bit_mask = GPIO_SEL_16,
                          .mode = GPIO_MODE_OUTPUT,
                          .pull_up_en = GPIO_PULLUP_ENABLE,
                          .pull_down_en = GPIO_PULLDOWN_DISABLE,
                          .intr_type = GPIO_INTR_DISABLE};
  ESP_ERROR_CHECK(gpio_config(&master));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_16, 1));

  // configure sun 1
  gpio_config_t sun1 = {.pin_bit_mask = GPIO_SEL_18,
                        .mode = GPIO_MODE_OUTPUT,
                        .pull_up_en = GPIO_PULLUP_ENABLE,
                        .pull_down_en = GPIO_PULLDOWN_DISABLE,
                        .intr_type = GPIO_INTR_DISABLE};
  ESP_ERROR_CHECK(gpio_config(&sun1));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));

  // configure sun 2
  gpio_config_t sun2 = {.pin_bit_mask = GPIO_SEL_19,
                        .mode = GPIO_MODE_OUTPUT,
                        .pull_up_en = GPIO_PULLUP_ENABLE,
                        .pull_down_en = GPIO_PULLDOWN_DISABLE,
                        .intr_type = GPIO_INTR_DISABLE};
  ESP_ERROR_CHECK(gpio_config(&sun2));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));

  // configure sun 3
  gpio_config_t sun3 = {.pin_bit_mask = GPIO_SEL_17,
                        .mode = GPIO_MODE_OUTPUT,
                        .pull_up_en = GPIO_PULLUP_ENABLE,
                        .pull_down_en = GPIO_PULLDOWN_DISABLE,
                        .intr_type = GPIO_INTR_DISABLE};
  ESP_ERROR_CHECK(gpio_config(&sun3));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));

  // configure sun 4
  gpio_config_t sun4 = {.pin_bit_mask = GPIO_SEL_23,
                        .mode = GPIO_MODE_OUTPUT,
                        .pull_up_en = GPIO_PULLUP_ENABLE,
                        .pull_down_en = GPIO_PULLDOWN_DISABLE,
                        .intr_type = GPIO_INTR_DISABLE};
  ESP_ERROR_CHECK(gpio_config(&sun4));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));
}

double object5_loop(double light_base, double light_amplitude) {
  // get current time
  uint32_t now = naos_millis();

  // check if sun needs to be changed
  if (last_change == 0 || now - last_change >= SUN_CHANGE) {
    // save time
    last_change = now;

    // randomly select next sun
    uint32_t next_sun = active_sun;
    while (next_sun == active_sun) {
      next_sun = (esp_random() >> 30) + 1;
    }

    // set new sun
    active_sun = next_sun;

    // activate next sun
    switch (active_sun) {
      case 1:
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 1));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));
        neo5_set_one(3, 255, 200, 0);
        neo5_set_one(2, 0, 0, 0);
        neo5_set_one(1, 0, 0, 0);
        neo5_set_one(0, 0, 0, 0);
        break;
      case 2:
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 1));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));
        neo5_set_one(3, 0, 0, 0);
        neo5_set_one(2, 255, 200, 0);
        neo5_set_one(1, 0, 0, 0);
        neo5_set_one(0, 0, 0, 0);
        break;
      case 3:
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 1));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));
        neo5_set_one(3, 0, 0, 0);
        neo5_set_one(2, 0, 0, 0);
        neo5_set_one(1, 255, 200, 0);
        neo5_set_one(0, 0, 0, 0);
        break;
      case 4:
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 1));
        neo5_set_one(3, 0, 0, 0);
        neo5_set_one(2, 0, 0, 0);
        neo5_set_one(1, 0, 0, 0);
        neo5_set_one(0, 255, 200, 0);
        break;
      default:
        break;
    }
  }

  // change position based on joystick
  if (gpio_get_level(GPIO_NUM_32) == 0) {
    position += JOYSTICK_SPEED;
  } else if (gpio_get_level(GPIO_NUM_33) == 0) {
    position -= JOYSTICK_SPEED;
  }

  // apply sun bounds (prevent lasers from leaving the white background)
  switch (active_sun) {
    case 1:
      position = a32_constrain_d(position, 57, 85.2);
      break;
    case 2:
      position = a32_constrain_d(position, 68.3, 97.4);
      break;
    case 3:
      position = a32_constrain_d(position, 84, 112.8);
      break;
    case 4:
      position = a32_constrain_d(position, 91, 120.5);
      break;
    default:
      position = a32_constrain_d(position, 57, 120.5);
      break;
  }

  // write position
  servo_write1(180 - position);

  // calculate current power
  double current_power = 0;
  switch (active_sun) {
    case 1:
      current_power = 1 - fabs(a32_safe_map_d(position, 67.6, 71.4, -1, 1));
      break;
    case 2:
      current_power = 1 - fabs(a32_safe_map_d(position, 80.9, 84, -1, 1));
      break;
    case 3:
      current_power = 1 - fabs(a32_safe_map_d(position, 94, 100, -1, 1));
      break;
    case 4:
      current_power = 1 - fabs(a32_safe_map_d(position, 101.7, 105.8, -1, 1));
      break;
    default:
      break;
  }

  // slowly apply power
  if (current_power > power) {
    power += POWER_APPLY_RATE;
  } else if (current_power <= power) {
    power -= POWER_APPLY_RATE;
  }

  // check power bounds
  power = a32_constrain_d(power, 0, 1);

  // set tower light
  neo5_set_range((uint8_t)(power * 200), (uint8_t)(power * 150), 0, 4, NUM_PIXELS - 1);

  // set light
  light_set(power, light_base, light_amplitude);

  // set neo pixels
  neo5_show();
  neo3_show();

  return power;
}
