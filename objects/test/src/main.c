#include <naos.h>

#include "object1.h"
#include "object2.h"
#include "object3.h"

// todo: need spi interface for apa102 leds
// todo: need i2c implementation sparkfun RGB light sensor

static long selected_object = 0;
static double power;

static void loop() {
  // prepare power
  double new_power = 0;

  // run object code
  switch (selected_object) {
    case 1:
      new_power = object1_loop();
      break;
    case 2:
      new_power = object2_loop();
      break;
    case 3:
      new_power = object3_loop();
      break;
    case 4:
      naos_log("object number %d", selected_object);
      break;
    case 5:
      naos_log("object number %d", selected_object);
      break;
    case 6:
      naos_log("object number %d", selected_object);
      break;
    default:
      break;
  }

  // naos_log("power%f",power);

  // check if power changed
  if (new_power != power) {
    naos_publish_d("value", new_power, 0, false, NAOS_LOCAL);
    power = new_power;
  }
}

static naos_param_t params[] = {
    {.name = "Object-Number", .type = NAOS_LONG, .default_l = 0},
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

  // get selected object
  selected_object = naos_get_l("Object-Number");

  // run object setup
  switch (selected_object) {
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
      naos_log("object number: %d", selected_object);
      break;
    case 5:
      naos_log("object number: %d", selected_object);
      break;
    case 6:
      naos_log("object number: %d", selected_object);
      break;
    default:
      break;
  }
}
