#include <art32/motion.h>
#include <art32/numbers.h>
#include <driver/ledc.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>
#include <naos.h>

#include "mot.h"

static double mot_target = 0;
static a32_motion_t mot_mp;
static bool mot_two_motors;

static void mot_set_raw(int speed) {
  // cap speed
  speed = a32_constrain_i(speed, 0, 1023);

  // set duty
  ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, (uint32_t)speed));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0));

  // set second motor if available
  if (mot_two_motors) {
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, (uint32_t)speed));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1));
  }
}

static void mot_stop() {
  // set zero speed to stop motor
  mot_set(0);

  // reset motion profile
  mot_mp = (a32_motion_t){0};
}

static void mot_task(void* p) {
  for (;;) {
    // configure motion profile
    mot_mp.max_velocity = 1;
    mot_mp.max_acceleration = 0.1;

    // update motion profile (for next ms)
    a32_motion_update(&mot_mp, mot_target, 5);

    // move depending on position
    if (mot_mp.position > 0) {
      mot_set_raw((int)mot_mp.position);
    } else {
      mot_stop();
    }

    // wait 5ms
    naos_delay(5);
  }
}

void mot_init(bool two_motors) {
  // save two motors flag
  mot_two_motors = two_motors;

  // prepare in master config
  gpio_config_t master = {.pin_bit_mask = GPIO_SEL_16,
                          .mode = GPIO_MODE_OUTPUT,
                          .pull_up_en = GPIO_PULLUP_ENABLE,
                          .pull_down_en = GPIO_PULLDOWN_DISABLE,
                          .intr_type = GPIO_INTR_DISABLE};

  // configure in master pin
  ESP_ERROR_CHECK(gpio_config(&master));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_16, 1));

  // prepare in m1b config
  gpio_config_t in_m1b = {.pin_bit_mask = GPIO_SEL_18,
                          .mode = GPIO_MODE_OUTPUT,
                          .pull_up_en = GPIO_PULLUP_DISABLE,
                          .pull_down_en = GPIO_PULLDOWN_ENABLE,
                          .intr_type = GPIO_INTR_DISABLE};

  // configure in m1b pin
  ESP_ERROR_CHECK(gpio_config(&in_m1b));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));

  // prepare ledc timer config
  ledc_timer_config_t t = {.duty_resolution = LEDC_TIMER_10_BIT,
                           .freq_hz = 10000,
                           .speed_mode = LEDC_HIGH_SPEED_MODE,
                           .timer_num = LEDC_TIMER_0};

  // configure ledc timer
  ESP_ERROR_CHECK(ledc_timer_config(&t));

  // prepare m1a ledc channel config
  ledc_channel_config_t m1a = {.duty = 0,
                               .intr_type = LEDC_INTR_DISABLE,
                               .speed_mode = LEDC_HIGH_SPEED_MODE,
                               .timer_sel = LEDC_TIMER_0,
                               .gpio_num = GPIO_NUM_19,
                               .channel = LEDC_CHANNEL_0};

  // configure ledc channel
  ESP_ERROR_CHECK(ledc_channel_config(&m1a));

  if (mot_two_motors) {
    // prepare in m1b config
    gpio_config_t in_m2b = {.pin_bit_mask = GPIO_SEL_23,
                            .mode = GPIO_MODE_OUTPUT,
                            .pull_up_en = GPIO_PULLUP_DISABLE,
                            .pull_down_en = GPIO_PULLDOWN_ENABLE,
                            .intr_type = GPIO_INTR_DISABLE};

    // configure in m2b pin
    ESP_ERROR_CHECK(gpio_config(&in_m2b));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));

    // prepare m1a ledc channel config
    ledc_channel_config_t m1b = {.duty = 0,
                                 .intr_type = LEDC_INTR_DISABLE,
                                 .speed_mode = LEDC_HIGH_SPEED_MODE,
                                 .timer_sel = LEDC_TIMER_0,
                                 .gpio_num = GPIO_NUM_17,
                                 .channel = LEDC_CHANNEL_1};

    // configure ledc channel
    ESP_ERROR_CHECK(ledc_channel_config(&m1b));
  }

  // stop motor
  mot_stop();

  // run async task
  xTaskCreatePinnedToCore(&mot_task, "mot", 8192, NULL, 2, NULL, 1);
}

void mot_set(double speed) {
    mot_target = speed;
}
