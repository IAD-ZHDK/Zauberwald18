// Wasser (sanduhr)
#include <naos.h>
#include "mot.h"
#include <art32/numbers.h>
#include <driver/gpio.h>
#include "light.h"
#include "neo3.h"
#include <driver/adc.h>
#include <art32/smooth.h>

//sensor QRD1114
double power = 0;

static a32_smooth_t* o1_smoothing;
static a32_smooth_t* o2_smoothing;



static int get_sensor(int n) {
    switch (n) {
        case 1:
            return adc1_get_raw(ADC1_CHANNEL_4);
        case 2:
            return adc1_get_raw(ADC1_CHANNEL_5);

            default:
            break;
    }

    return 0;
}




void object6_setup() {
    // initialize neo pixel
 //    neoPixelStandard_setup(0,100,255,0,24); //
    o1_smoothing = a32_smooth_new(40); // sensor 1
    o2_smoothing = a32_smooth_new(40);; // sensor 2
    // init neo pixels
    neo3_init(24, NEO3_DEFAULT_PIN); //

    // init lighting
    light_init(0,100,255,0,24);

    mot_init(false);

    //  // smoothing values for object output
    //  o1_smoothing = a32_smooth_new(20);
    //
    //  // install global interrupt service
    //  ESP_ERROR_CHECK(gpio_install_isr_service(0));
    //
    //  // init motor
    //  mot_init(true);
    //
    //  // init neo pixel
    //  neoPixelStandard_setup(254, 254, 0);


    // configure adc channels
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_10));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11));
}

double object6_loop() {

    int top = get_sensor(1);  //
    top = a32_smooth_update(o1_smoothing, top);
    int bottom = get_sensor(2);  //
    bottom = a32_smooth_update(o2_smoothing, bottom);
        if ((top < 60 || bottom < 17) && power<.99)  { // top sensor has a higher baseline
            power += .01;
        } else if (power>.01) {
            power-= .01;
        }

   int motorSpeed = (int)floor(a32_safe_map_d(power, 0, 1, 0, 1023));
  //  int motorSpeed = 1024;
 //   int motorSpeed = 500;
    if (motorSpeed < 100)  { // with less than 300 the dc motors don't rotate
        motorSpeed = 0;
    }
    mot_set(motorSpeed);
    naos_log("top: %d, bottom: %d, power: %f", top, bottom, power);

    //

    //  // read anemometer rate
    //  double rate = a32_constrain_d(anemo_get(), 0, 4);
    //
    //  // smooth rate
    //  rate = a32_smooth_update(o1_smoothing, rate);
    //
    //  naos_log("rate: %f", rate);
    //
    //  // calculate motor speed
    //  int motorSpeed = (int)floor(a32_safe_map_d(rate, 0, 6, 0, 1023));
    //
    //  // set motor speed
    //  mot_set(motorSpeed);
    //
    //  // calculate power
    //  double power = a32_safe_map_d(rate, .6, 10, 0, 1);
    //
    //  // set neo pixel
    //  neoPixelStandard(power);
    //

    // set neo pixel
    light_set(power);
    neo3_show();
    return power;
}


