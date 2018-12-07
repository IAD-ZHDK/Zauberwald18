// Wasser (Kugelbahn)

#include <art32/numbers.h>
#include <string.h>

#include "neo4.h"
#include "light.h"
#include "neo3.h"
#include "mot.h"
#include "enc.h"
#include "neo5.h"

#define OBJECT4_RESERVOIR_FILL_RATE 0.1
#define OBJECT4_VALVE_CLOSE_RATE 0.02
#define OBJECT4_PIPE_LENGTH 60
#define OBJECT4_RESERVOIR_LENGTH 19
#define OBJECT4_PIPE_LIMIT 24

double object4_reservoir = 0;
double object4_valve = 0;
double object4_bucket = 0;
bool object4_pipe[OBJECT4_PIPE_LENGTH] = {0};

void object4_setup() {
  // install global interrupt service
  ESP_ERROR_CHECK(gpio_install_isr_service(0));

  // initialize encoder
  enc_init();

  // initialize motor
  mot_init(false);

  // initialize neo pixel
  neo5_init(24, GPIO_NUM_32);
  neo5_set_all(0, 127, 127);
  neo5_show();

  // initialize neo pixel
  neo3_init(40, NEO3_DEFAULT_PIN);

  // initialize lighting
  light_init(0, 100, 255, 0, 39);

  // initialize neo pixel (10, 22, 28, 19)
  neo4_init(79, GPIO_NUM_25);
  neo4_set_all(0, 0, 0, 0);
  neo4_show();
}

double object4_loop() {
  // read encoder
  double rot = enc_get();

  // add water to reservoir
  object4_reservoir = a32_constrain_d(object4_reservoir + OBJECT4_RESERVOIR_FILL_RATE, 0, OBJECT4_RESERVOIR_LENGTH);

  // open/close valve
  if(rot > 0) {
    object4_valve = a32_constrain_d(object4_valve + rot, 0, 1);
  } else if (rot == 0) {
    object4_valve = a32_constrain_d(object4_valve - OBJECT4_VALVE_CLOSE_RATE, 0, 1);
  }

  // move water
  for (int k = OBJECT4_PIPE_LENGTH-1; k > 0; k--){
    object4_pipe[k] = object4_pipe[k-1];
  }
  object4_pipe[0] = false;

  // fill bucket
  object4_bucket = a32_constrain_d(object4_bucket + object4_valve, 0, 1);
  if(object4_bucket >= 1 && object4_reservoir > 1) {
    object4_pipe[0] = true;
    object4_bucket -= 1;
    object4_reservoir -= 1;
  }

  // set pipe pixels
  for (int i=0; i<OBJECT4_PIPE_LENGTH; i++) {
    if (object4_pipe[i]) {
      neo4_set_one(OBJECT4_PIPE_LENGTH - i, 0, 127, 127, 0);
    } else {
      neo4_set_one(OBJECT4_PIPE_LENGTH - i, 0, 0, 0, 0);
    }
  }

  // set reservoir pixels
  for(int i=0; i<OBJECT4_RESERVOIR_LENGTH; i++) {
    if(i <= object4_reservoir) {
      neo4_set_one(OBJECT4_PIPE_LENGTH + i, 0, 127, 127, 0);
    } else {
      neo4_set_one(OBJECT4_PIPE_LENGTH + i, 0, 0, 0, 0);
    }
  }

  // show pixels
  neo4_show();

  // calculate strength
  int total = 0;
  for (int i=0; i<OBJECT4_PIPE_LENGTH; i++) {
    total += object4_pipe[i] ? 1 : 0;
  };

  naos_log("tot: %d", total);

  // calculate motor speed
  int speed = 0;
  if (object4_valve > 0) {
    speed = (int)a32_safe_map_d(total, 0, OBJECT4_PIPE_LIMIT, 768, 1023);
  }

  // set motor speed
  mot_set(speed);

  // calculate power
  double power = a32_safe_map_d(total, 0, OBJECT4_PIPE_LIMIT, 0, 1);

  // power pixels
  neo5_set_all(0, (uint8_t)(power * 127 + 30), (uint8_t)(power * 127 + 30));
  neo5_show();

  // set lighting
  light_set(power);

  return power;
}
