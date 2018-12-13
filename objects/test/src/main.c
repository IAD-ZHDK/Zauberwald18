#include <naos.h>

#include "object1.h"
#include "object2.h"
#include "object3.h"
#include "object4.h"
#include "object5.h"
#include "object6.h"

#define PUBLISH_INTERVAL 30

static long object_number = 0;
static double multiplier = 1;
static double light_base = 0.2;
static double light_amplitude = 0.4;

static double last_power;
static uint32_t last_publish;

static void loop() {
  // prepare last_power
  double new_power = 0;

  // run object code
  switch (object_number) {
    case 1:
      new_power = object1_loop(light_base, light_amplitude);
      break;
    case 2:
      new_power = object2_loop(light_base, light_amplitude);
      break;
    case 3:
      new_power = object3_loop(light_base, light_amplitude);
      break;
    case 4:
      new_power = object4_loop(light_base, light_amplitude);
      break;
    case 5:
      new_power = object5_loop(light_base, light_amplitude);
      break;
    case 6:
      new_power = object6_loop(light_base, light_amplitude);
      break;
    default:
      break;
  }

  // multiply
  last_power *= multiplier;
  // check if power changed
  if (new_power != last_power && naos_millis() > last_publish + PUBLISH_INTERVAL) {
    naos_publish_d("value", new_power, 0, false, NAOS_LOCAL);
    last_power = new_power;
    last_publish = naos_millis();
  }
}

static naos_param_t params[] = {
    {.name = "object-number", .type = NAOS_LONG, .default_l = 0}, // require reboot
    {.name = "multiplier", .type = NAOS_DOUBLE, .default_d = 1.0, .sync_d = &multiplier},
    {.name = "light-base", .type = NAOS_DOUBLE, .default_d = 0.2, .sync_d = &light_base},
    {.name = "light-amplitude", .type = NAOS_DOUBLE, .default_d = 0.4, .sync_d = &light_amplitude},
};

static naos_config_t config = {.device_type = "zw18",
                               .firmware_version = "0.1.0",
                               .loop_callback = loop,
                               .loop_interval = 20,
                               .parameters = params,
                               .num_parameters = 4};

void app_main() {
  // initialize naos
  naos_init(&config);

  // get selected object
  object_number = naos_get_l("object-number");

  // run object setup
  switch (object_number) {
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
      object4_setup();
      break;
    case 5:
      object5_setup();
      break;
    case 6:
      object6_setup();
      break;
    default:
      break;
  }
}
