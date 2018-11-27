//
// Created by Franzke Luke on 21/11/18.
// wind: Chen_Lopez

#include <art32/numbers.h>
#include <art32/smooth.h>
#include <driver/gpio.h>
#include <math.h>
#include "neoPixelStandard.h"

static uint8_t neoR = 0;
static uint8_t neoG = 0;
static uint8_t neoB = 254;

void object2_setup() { neoPixelStandard_setup(neoR, neoG, neoB); }

float object2_loop() {
  neoPixelStandard(0);
  return 0;
}
