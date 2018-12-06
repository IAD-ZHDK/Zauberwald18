// Wasser (Kugelbahn)

#include "neo4.h"
#include "neoPixelStandard.h"

void object4_setup() {
  // initialize neo pixel
  neo4_init(280, NEO4_DEFAULT_PIN);
  neoPixelStandard_setup(0,100,255,40); //  this object  has 40 neopixels for ambient lighting
  neo4_set_all(120, 0, 0, 0);
  neo4_show();

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
}

double object4_loop() {
  double power = 0;
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
