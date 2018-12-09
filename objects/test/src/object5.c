// Sun (laser)

#include <naos.h>
#include <art32/numbers.h>
#include <art32/smooth.h>
#include <esp_system.h>
#include <driver/gpio.h>

#include "light.h"
#include "neo5.h"
#include "servo.h"
#include "neo3.h"

#define NUM_PIXELS 11

static uint8_t servoPos = 90;
static uint8_t servoMax = 160;
static uint8_t servoMin = 20;
static uint32_t activeSun = 1;
static uint32_t random = 1;
static unsigned long last_change = 0;
static uint8_t tower_R = 0;
static uint8_t tower_G = 0;
static uint8_t tower_B = 0;

void object5_setup() {
  // init neo pixels
  neo3_init(29, NEO3_DEFAULT_PIN); // this object just has 29 neopixels for ambient lighting

  // init lighting
  light_init(255, 200, 0, 0, 28);

  // initialize servos
  servo_setup(false);

  // initialize tower pixels and clear pixels
  neo5_init(NUM_PIXELS, NEO5_DEFAULT_PIN);
  neo5_show();

  // configure joystick
  ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_32, GPIO_MODE_INPUT));
  ESP_ERROR_CHECK(gpio_set_direction(GPIO_NUM_33, GPIO_MODE_INPUT));
  ESP_ERROR_CHECK(gpio_set_pull_mode(GPIO_NUM_32, GPIO_PULLUP_ONLY));
  ESP_ERROR_CHECK(gpio_set_pull_mode(GPIO_NUM_33, GPIO_PULLUP_ONLY));

  // prepare in master config for h Bridge
  gpio_config_t master = {.pin_bit_mask = GPIO_SEL_16,
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};

  // configure in master pin
  ESP_ERROR_CHECK(gpio_config(&master));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_16, 1));

  // laser pins setup
  gpio_config_t laser1 = {.pin_bit_mask = GPIO_SEL_19,
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser1));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));

  gpio_config_t laser2 = {.pin_bit_mask = GPIO_SEL_18,
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser2));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));

  gpio_config_t laser3 = {.pin_bit_mask = GPIO_SEL_23,
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser3));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));

  gpio_config_t laser4 = {.pin_bit_mask = GPIO_SEL_17,
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser4));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));
}

double object5_loop() {

  double power = 0;

  // set up tower glow
  uint16_t tower_red_light = (uint16_t) a32_safe_map_d(power, 0, 1, 0, 500);

  //naos_log("red light %f, power %f", red_light, power);

  if (tower_R < tower_red_light && tower_R < 255) {
    tower_R++;
  } else if (tower_R > 0) {
    tower_R--;
  }

  if (tower_G < tower_red_light - 250 && tower_G < 150) {
    tower_G++;
  } else if (tower_G > 0) {
    tower_G--;
  }

  if (tower_B < tower_red_light - 400 && tower_B < 100) {
    tower_B++;
  } else if (tower_B > 0) {
    tower_B--;
  }

  for (uint8_t i = 4; i < NUM_PIXELS; i++) {
    neo5_set_one(i, tower_R, tower_G, tower_B);
  }

  // joystick
  if (gpio_get_level(GPIO_NUM_32) == 1 && servoPos < servoMax) { //left TODO  check pin number
    servoPos++;
    servo_write1(servoPos);
  }

  if (gpio_get_level(GPIO_NUM_33) == 1 && servoPos > servoMin) {//right TODO  check pin number
    servoPos--;
    servo_write1(servoPos);
  }

  if (servoPos > servoMax) {
    servoPos--;
  } else if (servoPos < servoMin) {
    servoPos++;
  }

  // get current time
  unsigned long now = naos_millis();

  if (last_change == 0 || now - last_change >= 20000) {
    // save time
    last_change = now;

    // randomly select sun:
    while (activeSun == random) {
      random = abs(esp_random() >> 30); // get just two bits from esp_random();
      random += 1; // make sure we have values between 1 and 4
    }

    activeSun = random;

    // laser/sun code
    switch (activeSun) {
      case 1:
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 1));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));
        neo5_set_one(0, 255, 200, 0);
        neo5_set_one(1, 0, 0, 0);
        neo5_set_one(2, 0, 0, 0);
        neo5_set_one(3, 0, 0, 0);
        //   servoMin = 70; //todo find the servo min and max for all lasers
        //    servoMax = 100;
        break;
      case 2:
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 1));
        neo5_set_one(0, 0, 0, 0);
        neo5_set_one(1, 255, 200, 0);
        neo5_set_one(2, 0, 0, 0);
        neo5_set_one(3, 0, 0, 0);
        //     servoMin = 90;
        //   servoMax = 160;
        break;
      case 3:
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 1));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));
        neo5_set_one(0, 0, 0, 0);
        neo5_set_one(1, 0, 0, 0);
        neo5_set_one(2, 255, 200, 0);
        neo5_set_one(3, 0, 0, 0);
        //    servoMin = 10;
        //    servoMax = 90;
        break;
      case 4:
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 1));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));
        ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));
        neo5_set_one(0, 0, 0, 0);
        neo5_set_one(1, 0, 0, 0);
        neo5_set_one(2, 0, 0, 0);
        neo5_set_one(3, 255, 200, 0);
        //  servoMin = 90;
        //   servoMax = 150;
        break;
      default:
        break;
    }
  }

  neo5_show();
  light_set(power);
  neo3_show();

  return power;
}
