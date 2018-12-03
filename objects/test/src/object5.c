// Sun (laser)

#include <naos.h>
#include "apds.h"
#include <art32/numbers.h>
#include <driver/gpio.h>
#include "neoPixelStandard.h"
#include "servo.h"
#include <art32/smooth.h>

static a32_smooth_t* o1_smoothing;

uint8_t servoPos = 255/2;
uint8_t servoMax = 200;
uint8_t servoMin = 50;
int activeSun = 1;

void object5_setup() {
  // init apds chip
  apds_init();
  servo_setup(false); // one servo
  neoPixelStandard_setup(255,255,0);
  // smoothing values for object output
  o1_smoothing = a32_smooth_new(20);
  // laser pins setup
  gpio_config_t laser1 = {.pin_bit_mask = GPIO_SEL_16,
          .mode = GPIO_MODE_OUTPUT,
          .pull_up_en = GPIO_PULLUP_ENABLE,
          .pull_down_en = GPIO_PULLDOWN_DISABLE,
          .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser1));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_16, 0));

  gpio_config_t laser2 = {.pin_bit_mask = GPIO_SEL_18,
          .mode = GPIO_MODE_OUTPUT,
          .pull_up_en = GPIO_PULLUP_ENABLE,
          .pull_down_en = GPIO_PULLDOWN_DISABLE,
          .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser2));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));

  gpio_config_t laser3 = {.pin_bit_mask = GPIO_SEL_19,
          .mode = GPIO_MODE_OUTPUT,
          .pull_up_en = GPIO_PULLUP_ENABLE,
          .pull_down_en = GPIO_PULLDOWN_DISABLE,
          .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser3));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));

  gpio_config_t laser4 = {.pin_bit_mask = GPIO_SEL_23,
          .mode = GPIO_MODE_OUTPUT,
          .pull_up_en = GPIO_PULLUP_ENABLE,
          .pull_down_en = GPIO_PULLDOWN_DISABLE,
          .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser4));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));



}

double object5_loop() {
  // read red light
  apds_color_t c = apds_read();
  naos_log("c: %d, r: %d, g: %d, b: %d", c.c, c.r, c.g, c.b);

  // calculate power
  double power = a32_safe_map_d(c.r, 0, 255, 0, 1);

  // smooth power
  power = a32_smooth_update(o1_smoothing, power);

  // joystick
  if (gpio_get_level(GPIO_NUM_34) == 1 && servoPos < servoMax) { //left TODO  check pin number
    servoPos++;
  }
  if (gpio_get_level(GPIO_NUM_35) == 1 && servoPos > servoMin) {//right TODO  check pin number
    servoPos--;
  }

  servo_write1(servoPos);
  // laser/sun code

  switch (activeSun) {
    case 1:
          gpio_set_level(GPIO_NUM_16, 1);
          gpio_set_level(GPIO_NUM_18, 0);
          gpio_set_level(GPIO_NUM_19, 0);
          gpio_set_level(GPIO_NUM_23, 0);
          break;
    case 2:
          gpio_set_level(GPIO_NUM_16, 0);
          gpio_set_level(GPIO_NUM_18, 1);
          gpio_set_level(GPIO_NUM_19, 0);
          gpio_set_level(GPIO_NUM_23, 0);
          break;
    case 3:
          gpio_set_level(GPIO_NUM_16, 0);
          gpio_set_level(GPIO_NUM_18, 0);
          gpio_set_level(GPIO_NUM_19, 1);
          gpio_set_level(GPIO_NUM_23, 0);
          break;
    case 4:
          gpio_set_level(GPIO_NUM_16, 0);
          gpio_set_level(GPIO_NUM_18, 0);
          gpio_set_level(GPIO_NUM_19, 0);
          gpio_set_level(GPIO_NUM_23, 1);
          break;
    default:
      break;
  }


  neoPixelStandard(power);
  return power;
}
