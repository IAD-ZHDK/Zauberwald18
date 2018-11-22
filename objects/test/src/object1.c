//
// Created by Franzke Luke on 21/11/18.

#include <driver/gpio.h>
#include <art32/numbers.h>
#include <art32/smooth.h>
#include <math.h>
#include "anemometer.h"
#include "mot.h"



static double HRZ = 0;
static double LastHRZ = 0;
a32_smooth_t* smoothing;

void object1_setup(){
    // smoothing values for object output
    smoothing = a32_smooth_new(20);
    // install global interrupt service
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // init anenometer
    anenom_init();
    // init motor
    mot_init(true);

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
    return power;
}

