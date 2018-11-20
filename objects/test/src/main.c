#include <naos.h>
#include <esp_err.h>
#include <driver/gpio.h>
#include <art32/numbers.h>
#include <art32/smooth.h>
#include <math.h>
#include "anemometer.h"
#include "enc.h"
#include "mot.h"
#include "neo.h"

bool object_a = false;
uint8_t fadeBrightness = 30;
int fadeAmount = 1;
double HRZ = 0;
double LastHRZ = 0;
a32_smooth_t* smoothing;
float power;
static void loop() {

    HRZ = anenom_getHrz();
    if (abs(HRZ-LastHRZ)>4) {
       HRZ =  LastHRZ;// remove any spikes in sensor values
    }
    LastHRZ = HRZ;
    HRZ = a32_smooth_update(smoothing,HRZ);

    int motorSpeed = floor(a32_safe_map_f(HRZ, 0, 6, 0, 1023));

    mot_set(motorSpeed);

    // neoPixels
    fadeBrightness+=fadeAmount;
    if (fadeBrightness>=60) {
        fadeAmount = - 1;
    }
    if (fadeBrightness<=8) {
        fadeAmount = 1;
    }
    if (motorSpeed/4 > 50) {
        fadeBrightness = motorSpeed/4;
    }

    naos_log("HRZ %f motorSpeed %d fadeBrightness  %d", HRZ, motorSpeed, fadeBrightness);
    neo_set_all(fadeBrightness, fadeBrightness, 0); // yellow for wind
    neo_show();


    if (object_a) {
        float new_power = a32_safe_map_f(HRZ, .6, 10, 0, 1);
        if (new_power!= power) { // only update if needed
            naos_publish_d("solar", power, 0, false, NAOS_LOCAL);
            power = new_power;
        }
    }
}

static naos_param_t params[] = {
    { .name = "object-a", .type = NAOS_BOOL, .default_b = false, .sync_b = &object_a },
};

static naos_config_t config = {.device_type = "test",
                               .firmware_version = "0.1.0",
                               .loop_callback = loop,
                               .loop_interval = 50,
                               .parameters = params,
                               .num_parameters = 1};

void app_main() {

    // smoothing values for object output
    smoothing = a32_smooth_new(20);
    // install global interrupt service
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

  // initialize naos

  naos_init(&config);

  // init anenometer
  //enc_init(enc);
    anenom_init();
  // init motor
  mot_init(true);

  // init neo pixel
  neo_init();
}
