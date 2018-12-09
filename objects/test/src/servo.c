#include <art32/numbers.h>
#include <driver/ledc.h>

#include "servo.h"

#define SERVO_LEDC_INIT_FREQ (50)
#define SERVO_DUTY_MIN_US (544)
#define SERVO_DUTY_MAX_US (2400)
#define SERVO_SEC_TO_US (1000000)
#define SERVO_MAX_ANGLE (180)

#define SERVO_TIMER LEDC_TIMER_2
#define SERVO_CHANNEL_1 LEDC_CHANNEL_4
#define SERVO_CHANNEL_2 LEDC_CHANNEL_5

float m_full_duty = (1 << LEDC_TIMER_15_BIT) - 1;

static uint32_t servo_angle_to_duty(double angle) {
  // calculate pulse width
  double width = a32_safe_map_d(angle, 0, SERVO_MAX_ANGLE, SERVO_DUTY_MIN_US, SERVO_DUTY_MAX_US);
  return m_full_duty * ((int)width) / (SERVO_SEC_TO_US / SERVO_LEDC_INIT_FREQ);
}

void servo_setup(bool two_servos) {
  // initialize timer
  ledc_timer_config_t t;
  t.duty_resolution = LEDC_TIMER_15_BIT;
  t.freq_hz = SERVO_LEDC_INIT_FREQ;
  t.speed_mode = LEDC_HIGH_SPEED_MODE;
  t.timer_num = SERVO_TIMER;
  ESP_ERROR_CHECK(ledc_timer_config(&t));


  if (!two_servos) {
    //  5volt logic pins are used up for object 5, so need to use a 3.3 pin here
    ledc_channel_config_t ch1;
    ch1.channel = SERVO_CHANNEL_1;
    ch1.duty = 0;  // todo: set init angle
    ch1.gpio_num = GPIO_NUM_25;
    ch1.speed_mode = LEDC_HIGH_SPEED_MODE;
    ch1.timer_sel = SERVO_TIMER;
    ESP_ERROR_CHECK(ledc_channel_config(&ch1));
  } else {
    //  useing the 5volt logic pins for object 3
    ledc_channel_config_t ch1;
    ch1.channel = SERVO_CHANNEL_1;
    ch1.duty = 0;  // todo: set init angle
    ch1.gpio_num = GPIO_NUM_18;
    ch1.speed_mode = LEDC_HIGH_SPEED_MODE;
    ch1.timer_sel = SERVO_TIMER;
    ESP_ERROR_CHECK(ledc_channel_config(&ch1));

    ledc_channel_config_t ch2;
    ch2.channel = SERVO_CHANNEL_2;
    ch2.duty = 0;  // todo: set init angle
    ch2.gpio_num = GPIO_NUM_19;
    ch2.speed_mode = LEDC_HIGH_SPEED_MODE;
    ch2.timer_sel = SERVO_TIMER;
    ESP_ERROR_CHECK(ledc_channel_config(&ch2));
  }
}

void servo_write1(double angle) {
  // calculate duty
  uint32_t duty = servo_angle_to_duty(angle);

  // set duty
  ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, SERVO_CHANNEL_1, duty));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, SERVO_CHANNEL_1));
}

void servo_write2(double angle) {
  // calculate duty
  uint32_t duty = servo_angle_to_duty(angle);

  // set duty
  ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE, SERVO_CHANNEL_2, duty));
  ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE, SERVO_CHANNEL_2));
}
