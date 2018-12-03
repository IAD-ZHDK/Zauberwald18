// Solar (Sonnenblume)

#include <art32/numbers.h>
#include <art32/smooth.h>
#include <driver/adc.h>
#include <driver/gpio.h>
#include <math.h>
#include <naos.h>

#include "neoPixelStandard.h"
#include "servo.h"

static uint8_t neoR = 254;
static uint8_t neoG = 254;
static uint8_t neoB = 0;


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
  // neoPixelStandard_setup(neoR,neoG,neoB);
    // smoothing values for object output
    o1_smoothing = a32_smooth_new(20);

  // configure adc channels
  ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_10));
  ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11));
  ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11));
  ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11));
  ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11));
}

double object3_loop() {
  //  neoPixelStandard(0);
  int lt = get_sensor(1);  // top left
  int rt = get_sensor(3);  // top right
  int ld = get_sensor(4);  // down links
  int rd = get_sensor(2);  // down right
                           // naos_log("lt: %d,rt: %d,ld: %d,rd: %d", lt,rt,ld,rd);

  // definition der Toleranz und Geschwindigkeit
  // int dtime = 15;
  int tol = 40;  // tolerance

  // durchschnittswerte rechnen
  int avt = (lt + rt) / 2;  // durchschnitt top
  int avd = (ld + rd) / 2;  // durchschnitt bottom
  int avl = (lt + ld) / 2;  // durchschnitt links
  int avr = (rt + rd) / 2;  // durchschnitt rechts

  int dvert = avt - avd;   // prüfung der differenz vertikal
  int dhoriz = avl - avr;  // prüfung der differenz horizontal

  // berechnung und anpassung der servopositionen
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
    //  vertical.write(servovertical);
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
      //  horizontal.write(servo_horizontal);
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
      //   horizontal.write(servo_horizontal);
      servo_write2(servo_horizontal);
    }
  }

  float power = avt + avd + avl + avr;
    power = power / 4;
    power = power / 400.0;
  if (power > 1) {
      power = 1;
  }
    power = a32_smooth_update(o1_smoothing, power);

  return power;
}
