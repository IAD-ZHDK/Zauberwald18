//
// Created by Franzke Luke on 21/11/18.
// wind: Duarte_Prantl_Weber
//
#include <driver/gpio.h>
#include <art32/numbers.h>
#include <art32/smooth.h>
#include <math.h>
#include "anemometer.h"
#include "mot.h"
#include "neoPixelStandard.h"

static double HRZ = 0;
static double LastHRZ = 0;
static a32_smooth_t* smoothing;

// neopixel
static uint8_t neoR = 254;
static uint8_t neoG = 254;
static uint8_t neoB = 0;

void object1_setup(){
    // smoothing values for object output
    smoothing = a32_smooth_new(20);
    // install global interrupt service
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    // init anenometer
    anenom_init();
    // init motor
    mot_init(true);
    // init neo pixel
    neoPixelStandard_setup(neoR,neoG,neoB);
}

float object1_loop() {
    HRZ = anenom_getHrz();
    if (abs(HRZ-LastHRZ)>4) {
        HRZ =  LastHRZ;// remove any spikes in sensor values
    }
    LastHRZ = HRZ;
    HRZ = a32_smooth_update(smoothing,HRZ);

    int motorSpeed = floor(a32_safe_map_f(HRZ, 0, 6, 0, 1023));

    mot_set(motorSpeed);

    float power = a32_safe_map_f(HRZ, .6, 10, 0, 1);
    neoPixelStandard(power);
    return power;

}

