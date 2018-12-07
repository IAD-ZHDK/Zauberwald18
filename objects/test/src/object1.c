// Wind (Schneesturm): Duarte, Prantl, Weber

#include <art32/numbers.h>
#include <art32/smooth.h>
#include <driver/gpio.h>
#include <math.h>
#include <naos.h>

#include "anemometer.h"
#include "mot.h"
#include "light.h"
#include "neo3.h"

static a32_smooth_t* o1_smoothing;

void object1_setup() {
  // smoothing values for object output
  o1_smoothing = a32_smooth_new(20);

  // install global interrupt service
  ESP_ERROR_CHECK(gpio_install_isr_service(0));

  // init anemometer
  anemo_init(GPIO_NUM_33);

  // init motor
  mot_init(true);

  // init neo pixel
  neo3_init(36, NEO3_DEFAULT_PIN);

  // init lighting
  light_init(254, 0, 254, 0, 35); // magenta
}

double object1_loop() {
  // read anemometer rate
  double rate = a32_constrain_d(anemo_get(), 0, 5.5);

  // smooth rate
  rate = a32_smooth_update(o1_smoothing, rate);

  // calculate motor speed
  int motorSpeed = (int)floor(a32_safe_map_d(rate, 0, 6, 0, 1023));
  if (motorSpeed < 300)  { // with less than 300 the dc motors don't rotate
    motorSpeed = 0;
  }

  // set motors speed
  mot_set(motorSpeed);

  // calculate power
  double power = a32_safe_map_d(rate, .6, 10, 0, 1);

  // set neo pixel
  light_set(power);

  // show pixels
  neo3_show();

  return power;
}
