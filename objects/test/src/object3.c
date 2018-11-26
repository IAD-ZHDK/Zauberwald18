//
// Created by Franzke Luke on 21/11/18.
// solar 1:
#include "servoIOT.h"
#include <driver/gpio.h>
#include <art32/numbers.h>
#include <art32/smooth.h>
#include <math.h>
#include "neoPixelStandard.h"


static uint8_t neoR = 0;
static uint8_t neoG = 0;
static uint8_t neoB = 254;
static float count = 60;
static int direction = 2;

void object3_setup(){
    servo1Setup();
    servo2Setup();
   // neoPixelStandard_setup(neoR,neoG,neoB);
}

float object3_loop() {

  //  neoPixelStandard(0);

    if(count>=170) {
        direction = -2;
    }
    if(count<=10) {
        direction = +2;
    }
    count+= direction;
    servo1Write(count);
    servo2Write(count);
    return 0;
}

