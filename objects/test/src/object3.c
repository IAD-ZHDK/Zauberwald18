// Solar 2 (Sonnenblume): Gorgi, Hirsbrunner, Szakacs

#include <art32/numbers.h>
#include <art32/smooth.h>
#include <driver/adc.h>
#include <driver/gpio.h>
#include <math.h>
#include <naos.h>

#include "light.h"
#include "neo3.h"
#include "servo.h"

static a32_smooth_t* o1_smoothing;

static int servo_horizontal = 90;
static int servo_horizontal_limit_high = 180;
static int servo_horizontal_limit_low = 0;

static int servo_vertical = 90;
static int servo_vertical_limit_high = 160;
static int servo_vertical_limit_low = 30;

static int get_sensor(int n) {
  switch (n) {
    case 1:
      return adc1_get_raw(ADC1_CHANNEL_4);
    case 2:
      return adc1_get_raw(ADC1_CHANNEL_5);
    case 3:
      return adc1_get_raw(ADC1_CHANNEL_6);
    case 4:
      return adc1_get_raw(ADC1_CHANNEL_7);
    default:
      break;
  }

  return 0;
}

void object3_setup() {
  // initialize servos
  servo_setup(true);

  // initialize neo pixels
  neo3_init(36, NEO3_DEFAULT_PIN);

  // initialize lighting
  light_init(254, 200, 0, 0, 35);

  // smoothing values for object output
  o1_smoothing = a32_smooth_new(20);

  // configure adc channels
  ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_10));
  ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11));
  ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11));
  ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11));
  ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11));
}

double object3_loop(double light_base, double light_amplitude) {
  int lt = get_sensor(1);  // top left
  int rt = get_sensor(3);  // top right
  int ld = get_sensor(4);  // down links
  int rd = get_sensor(2);  // down right
                           // naos_log("lt: %d,rt: %d,ld: %d,rd: %d", lt,rt,ld,rd);

  // int dtime = 15; delay time in original code
  int tol = 40;  // tolerance

  // durchschnittswerte rechnen
  int avt = (lt + rt) / 2;  // average top
  int avd = (ld + rd) / 2;  // average bottom
  int avl = (lt + ld) / 2;  // average links
  int avr = (rt + rd) / 2;  // average rechts

  int dvert = avt - avd;   // check difference vertical
  int dhoriz = avl - avr;  // check difference horisontal

  // set servo position
  if (-1 * tol > dvert || dvert > tol) {
    if (avt > avd) {
      // servovertical = ++servovertical;
      servo_vertical++;
    } else if (avt < avd) {
      // servovertical = --servovertical;
      servo_vertical--;
    }
    if (servo_vertical > servo_vertical_limit_high) {
      servo_vertical = servo_vertical_limit_high;
    }
    if (servo_vertical < servo_vertical_limit_low) {
      servo_vertical = servo_vertical_limit_low;
    }
    servo_write1(servo_vertical);
  }

  if (servo_vertical > 90) {
    if (-1 * tol > dhoriz || dhoriz > tol) {
      if (avl > avr) {
        // servo_horizontal = ++servo_horizontal;
        servo_horizontal++;

      } else if (avl < avr) {
        // servo_horizontal = --servo_horizontal;
        servo_horizontal--;

      } else if (avl == avr) {
        // nothing
      }
      if (servo_horizontal > servo_horizontal_limit_high) {
        servo_horizontal = servo_horizontal_limit_high;
      }
      if (servo_horizontal < servo_horizontal_limit_low) {
        servo_horizontal = servo_horizontal_limit_low;
      }
      servo_write2(servo_horizontal);
    }
  } else {
    if (-1 * tol > dhoriz || dhoriz > tol) {
      if (avl > avr) {
        // servo_horizontal = --servo_horizontal;
        servo_horizontal--;

      } else if (avl < avr) {
        // servo_horizontal = ++servo_horizontal;
        servo_horizontal++;

      } else if (avl == avr) {
        // nothing
      }
      if (servo_horizontal > servo_horizontal_limit_high) {
        servo_horizontal = servo_horizontal_limit_high;
      }
      if (servo_horizontal < servo_horizontal_limit_low) {
        servo_horizontal = servo_horizontal_limit_low;
      }
      servo_write2(servo_horizontal);
    }
  }

  double power = avt + avd + avl + avr;
  power = power / 4;
  power = power / 400.0;
  if (power > 1) {
    power = 1;
  }

  power = a32_smooth_update(o1_smoothing, power);
  light_set(power, light_base, light_amplitude);
  neo3_show();

  return power;
}
