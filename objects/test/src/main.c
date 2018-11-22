#include <naos.h>
#include <esp_err.h>
#include <driver/gpio.h>
#include <art32/numbers.h>
#include <art32/smooth.h>
#include <math.h>
#include "object1.h"
#include <string.h>
#include "neo.h"

//bool object_a = false;
//static bool debug = false;
static int paramNo = 1;
static long selectedObject = 1;
static float power;
static const char * objectName[6] = {"wind1", "wind2", "solar1", "solar2", "water1", "water2"}; // I guess we don't need this, just get the topics from the bluetooth GUI

// neopixel
//static float neoBrightness = .1;
//static double fadeAmount = 0.01;
static uint16_t  lightAngle = 0;
static float lightAmplitude = .1;
static uint8_t neoR = 254;
static uint8_t neoG = 254;
static uint8_t neoB = 0;

static void loop() {
    float new_power = 0;

    switch(selectedObject) {
        case 1 :
            new_power = object1_loop();
            break;
        case 2 :
            naos_log("object number %d", selectedObject);
            break;
        case 3 :
            naos_log("object number %d", selectedObject);
            break;
        case 4 :
            naos_log("object number %d", selectedObject);
            break;
        case 5 :
            naos_log("object number %d", selectedObject);
            break;
        case 6:
            naos_log("object number %d", selectedObject);
            break;
    }

    if (new_power!= power) { // only update if needed
        naos_publish_d(objectName[selectedObject], new_power, 0, false, NAOS_LOCAL);
        power = new_power;
    }
    // neo pixels for all objects?
    lightAngle += 2;
    lightAngle = lightAngle % 360;

    float neoBrightness = (sin((lightAngle* M_PI /180)-M_PI_2)+1)/2;
    neoBrightness = neoBrightness*lightAmplitude+.02; // add an offset of .02 so it's never off

    if(power>neoBrightness) {
        neoBrightness = power;
    }

    uint8_t _r = (uint8_t)floor(neoR*neoBrightness);
   uint8_t _g = (uint8_t)floor(neoG*neoBrightness);
    uint8_t _b = (uint8_t)floor(neoB*neoBrightness);
    neo_set_all(_r, _g, _b);
    neo_show();
    naos_log("lightAngle %d, angle %f",lightAngle,neoBrightness);
}

static naos_param_t params[] = {
        { .name = "Object-Number", .type = NAOS_LONG, .default_l = 1, .sync_l = &paramNo},
};

static naos_config_t config = {.device_type = "test",
                               .firmware_version = "0.1.0",
                               .loop_callback = loop,
                               .loop_interval = 50,
                               .parameters = params,
                               .num_parameters = 1};
void app_main() {
    // initialize naos

    naos_init(&config);

    // init neo pixel
    neo_init();

// init objects
    if (paramNo<=6){
        selectedObject = paramNo;
    }
    switch(selectedObject) {
        case 1 :
            object1_setup();
            neoR = 254;
            neoG = 254;
            neoB =0;
            break;
        case 2 :
            naos_log("object number %d", selectedObject);
            neoR =254;
            neoG =254;
            neoB =0;
            break;
        case 3 :
            naos_log("object number %d", selectedObject);
            neoR =0;
            neoG =254;
            neoB =0;
            break;
        case 4 :
            naos_log("object number %d", selectedObject);
            neoR =0;
            neoG =254;
            neoB =0;
            break;
        case 5 :
            naos_log("object number %d", selectedObject);
            neoR =254;
            neoG =0;
            neoB =0;
            break;
        case 6 :
            naos_log("object number %d", selectedObject);
            neoR =255;
            neoG =0;
            neoB =0;
            break;
    }

}
