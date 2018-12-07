#include <math.h>
#include <naos.h>

#include "neo3.h"

static uint8_t light_r = 0;
static uint8_t light_g = 0;
static uint8_t light_b = 0;

static int light_first = 0;
static int light_last = 0;

static uint16_t light_angle = 0;
static double light_amplitude = .09;

void light_init(uint8_t r, uint8_t g, uint8_t b, int first, int last) {
  // set color
  light_r = r;
  light_g = g;
  light_b = b;

  // set pixels
  light_first = first;
  light_last = last;
}

void light_set(double power) {
  // calculate angle
  light_angle += 1;
  light_angle = (uint16_t)(light_angle % 360);

  // calculate brightness
  double brightness = (sin((light_angle * M_PI / 180) - M_PI_2) + 1) / 2;
  brightness = brightness * light_amplitude + .04;  // add an offset of .04 so it's never off

  // limit brightness
  if (power > brightness) {
    brightness = power;
  }

  // set color
  uint8_t r = (uint8_t)floor(light_r * brightness);
  uint8_t g = (uint8_t)floor(light_g * brightness);
  uint8_t b = (uint8_t)floor(light_b * brightness);

  // set all pixels
  neo3_set_range(r, g, b, light_first, light_last);
}
