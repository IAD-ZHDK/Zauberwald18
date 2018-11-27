//
// Created by Franzke Luke on 26/11/18.
//

// standard neopixel lighting for most objects
#include <math.h>
#include "neo.h"
// neopixel
// static float neoBrightness = .1;
// static double fadeAmount = 0.01;
static uint16_t lightAngle = 0;
static double lightAmplitude = .09;
static uint8_t neoR = 0;
static uint8_t neoG = 0;
static uint8_t neoB = 0;

void neoPixelStandard(double power) {
  lightAngle += 3;
  lightAngle = lightAngle % 360;

  double neoBrightness = (sin((lightAngle * M_PI / 180) - M_PI_2) + 1) / 2;
  neoBrightness = neoBrightness * lightAmplitude + .03;  // add an offset of .02 so it's never off

  if (power > neoBrightness) {
    neoBrightness = power;
  }
  uint8_t _r = (uint8_t)floor(neoR * neoBrightness);
  uint8_t _g = (uint8_t)floor(neoG * neoBrightness);
  uint8_t _b = (uint8_t)floor(neoB * neoBrightness);
  neo_set_all(_r, _g, _b);
  neo_show();
  //  naos_log("power%f",power);
}

void neoPixelStandard_setup(uint8_t R, uint8_t G, uint8_t B) {
  neo_init();
  // init neo pixel
  neoR = R;
  neoG = G;
  neoB = B;
}
