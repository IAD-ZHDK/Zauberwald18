#include <art32/numbers.h>
#include <art32/smooth.h>
#include <driver/adc.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <math.h>
#include <naos.h>
#include <string.h>
#include "APA102.h"
#include "object1.h"
#include "object2.h"
#include "object3.h"

// todo: need spi interface for apa102 leds
// todo: need i2c implementation sparkfun RGB light sensor

// bool object_a = false;
// static bool debug = false;
static int paramNo = 1;  // parameter from GUI for object number
static long selectedObject = 1;
static float power;
static const char* objectName[6] = {
    "wind1",  "wind2",  "solar1",
    "solar2", "water1", "water2"};  // I guess we don't need this, just get the topics from the bluetooth GUI

static void loop() {
  float new_power = 0;

  switch (selectedObject) {
    case 1:
      new_power = object1_loop();
      break;
    case 2:
      new_power = object2_loop();
      break;
    case 3:
      //
      //
      new_power = object3_loop();
      break;
    case 4:
      naos_log("object number %d", selectedObject);
      break;
    case 5:
      naos_log("object number %d", selectedObject);
      break;
    case 6:
      naos_log("object number %d", selectedObject);
      break;
  }

  // naos_log("power%f",power);
  if (new_power != power) {  // only update if needed
    naos_publish_d(objectName[selectedObject], new_power, 0, false, NAOS_LOCAL);
    power = new_power;
  }
}

static naos_param_t params[] = {
    {.name = "Object-Number", .type = NAOS_LONG, .default_l = 1, .sync_l = &paramNo},
};

static naos_config_t config = {.device_type = "test",
                               .firmware_version = "0.1.0",
                               .loop_callback = loop,
                               .loop_interval = 20,
                               .parameters = params,
                               .num_parameters = 1};

void app_main() {
  // initialize naos
  naos_init(&config);

  // APA102_init();

  // init objects
  if (paramNo <= 6) {
    selectedObject = paramNo;
  }
  switch (selectedObject) {
    case 1:
      object1_setup();
      break;
    case 2:
      object2_setup();
      break;
    case 3:
      object3_setup();
      break;
    case 4:
      naos_log("object number %d", selectedObject);
      break;
    case 5:
      naos_log("object number %d", selectedObject);
      break;
    case 6:
      naos_log("object number %d", selectedObject);
      break;
  }
}
