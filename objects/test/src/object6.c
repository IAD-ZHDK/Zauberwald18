// Wasser (sanduhr)
#include <naos.h>
#include "mot.h"
#include <art32/numbers.h>
#include <driver/gpio.h>
#include "neoPixelStandard.h"
#include <driver/adc.h>
//sensor QRD1114

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
     neoPixelStandard_setup(0,100,255,24); //
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
    double power = 0;
  // mot_set(400);

    int top = get_sensor(1);  //
    int bottom = get_sensor(2);  //

     naos_log("top: %d, bottom: %d", top, bottom);
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

    neoPixelStandard(power);
    return power;
}


