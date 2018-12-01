// Wind (Berg & Tal): Chen, Lopez

#include <art32/numbers.h>
#include <art32/smooth.h>
#include <driver/gpio.h>
#include <esp_system.h>
#include <math.h>
#include <naos.h>

#include "anemometer.h"
#include "apa.h"
#include "object2.h"

static a32_smooth_t* o1_smoothing;

#define NUMPIXELS 81

int sizeImpuls01 = 0;
int index01 = 0;
int dimension = 9;
bool Ireset = false;
bool start = false;
int input;
int x;
int y;
int countRe;
int pixels[NUMPIXELS] = {};
uint32_t colors[NUMPIXELS] = {};
uint32_t red;
uint32_t blue;

void object2_setup() {
  // smoothing values for object output
  o1_smoothing = a32_smooth_new(20);

  // install global interrupt service
  ESP_ERROR_CHECK(gpio_install_isr_service(0));

  // init anemometer
  anemo_init();

  // init apa pixels
  apa_init(NUMPIXELS, APA_DEFAULT_CLOCK_PIN, APA_DEFAULT_DATA_PIN);
}

double object2_loop() {
  // set pixels
  apa_set_all(0, 0, 255);
  apa_show();

  // read anemometer rate
  double rate = a32_constrain_d(anemo_get(), 0, 4);
  // smooth rate
  rate = a32_smooth_update(o1_smoothing, rate);
  // naos_log("rate: %f", rate);
  // calculate power
  double power = a32_safe_map_d(rate, .6, 10, 0, 1);

  // Serial.println(input);
  if (input > .3) {  // need to check this value
    start = true;
    naos_log("Ss");
  }
  if (start) {
    create();
  }
  if (Ireset == true) {
    reset(0, 0);
  }
  animate();

  return power;
}
void create() {
  if (out() != true) {
    next();  // delay(50);
  }
}

bool out() {
  if (x >= dimension || y >= dimension) {
    return true;
  } else {
    return false;
  }
}

void next() {
  if (y % 2 == 0) {
    pixels[sizeImpuls01] = y * dimension + x;
  } else {
    pixels[sizeImpuls01] = y * dimension + (dimension - 1 - x);
  }
  int rx;
  int ry;
  int ra = esp_random() * 3;  // todo: check that how this random works
  if (ra == 0) {
    rx = 1;
    ry = 0;
  } else if (ra == 1) {
    rx = 1;
    ry = 1;
  } else {
    rx = 0;
    ry = 1;
  }
  int m1x = mountainX(3, 2, x, y);
  int m1y = mountainY(3, 2, x, y);
  int m2x = mountainX(5, 7, x, y);
  int m2y = mountainY(5, 7, x, y);
  int m3x = mountainX(6, 4, x, y);
  int m3y = mountainY(6, 4, x, y);
  if (m1x < 2) {
    rx = m1x;
    ry = m1y;
  }
  if (m2x < 2) {
    rx = m2x;
    ry = m2y;
  }
  if (m3x < 2) {
    rx = m3x;
    ry = m3y;
  }
  x += rx;
  y += ry;
  sizeImpuls01++;
  Ireset = false;
}

int mountainX(int mx, int my, int x, int y) {
  int M1x = mx;
  int M1y = my;
  int d1 = sqrt(x - M1x) + sqrt(y - M1y);
  if (d1 == 1 && M1x >= x && M1y >= y) {
    if (M1x - x == 1) {
      return 0;
    } else {
      return 1;
    }
  } else if (d1 == 2) {
    if (M1x > x && M1y > y) {
      return 01;
    } else if (M1x > x) {
      return 0;
    } else {
      return 1;
    }
  } else {
    return 2;
  }
}

int mountainY(int mx, int my, int x, int y) {
  int M1x = mx;
  int M1y = my;
  int d1 = sqrt(x - M1x) + sqrt(y - M1y);
  if (d1 == 1 && M1x >= x && M1y >= y) {
    if (M1x - x == 1) {
      return 1;
    } else {
      return 0;
    }
  } else if (d1 == 2) {
    if (M1x > x && M1y > y) {
      return 0;
    } else if (M1x > y) {
      return 0;
    } else {
      return 1;
    }
  }

  else {
    return 2;
  }
}

void animate() {
  if (index01 < sizeImpuls01) {
    int p = pixel(index01);
    uint32_t c = color(index01);
    // strip.setPixelColor(p,c); todo put new apa102 code here
    index01++;
  } else {
    index01 = 0;
    setcol();
  }
}

void setcol() {
  if (countRe >= sizeImpuls01 && sizeImpuls01 > 3) {
    Ireset = true;
  }
  for (int i = sizeImpuls01 - 1; i >= 0; i--) {
    if (red > 0x000600) {
      red -= 0x000500;
      blue += 0x000005;
    } else if (input > 30) {
      red = 0x000000;
      blue = 0x0000FF;
    } else if (blue > 0x000045) {
      red = 0x000000;
      blue -= 0x000044;
    } else {
      red = 0x000000;
      blue = 0x000000;
      countRe++;
    }
    colors[i] = red + blue;
  }
}

void reset(int ix, int iy) {
  x = ix;
  y = iy;
  sizeImpuls01 = 0;
  index01 = 0;
  red = 0x00FF00;
  blue = 0x0000FF;
  start = false;
  countRe = 0;
  Ireset = false;
}

int pixel(int i) { return pixels[i]; }

int color(int i) { return colors[i]; }
