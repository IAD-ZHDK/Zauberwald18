// Wind (Berg & Tal): Chen, Lopez

#include "object2.h"
#include <art32/numbers.h>
#include <art32/smooth.h>
#include <driver/gpio.h>
#include <esp_system.h>
#include <math.h>
#include <naos.h>
#include "anemometer.h"
#include "apa.h"

static a32_smooth_t* o1_smoothing;

#define NUMPIXELS 81

int lengthOfImpulse = 0;
int index01 = 0;
int dimension = 9;
bool Ireset = false;
bool start = false;
double input;
double power;
int x;
int y;
int countRe;
int pixels[NUMPIXELS] = {};
float pixelsBackgroundAnim[NUMPIXELS] = {};  // angle for sin wave animation
uint8_t backgroundColors[NUMPIXELS] = {};    // angle for sin wave animation
uint32_t colors[NUMPIXELS] = {};             //
uint32_t red;
uint32_t green;
uint32_t blue;

void object2_setup() {
  // smoothing values for object output
  o1_smoothing = a32_smooth_new(20);

  // install global interrupt service
  ESP_ERROR_CHECK(gpio_install_isr_service(0));

  // init anemometer
  anemo_init(1);

  // init apa pixels
  apa_init(NUMPIXELS, APA_DEFAULT_CLOCK_PIN, APA_DEFAULT_DATA_PIN);
  // setup background animation
  for (int i = 0; i < NUMPIXELS; i++) {
    pixelsBackgroundAnim[i] = i;
  }
}

double object2_loop() {
  // read anemometer rate
  double input = a32_constrain_d(anemo_get(), 0, 5);
  // smooth rate
  input = a32_smooth_update(o1_smoothing, input);
  // calculate power
  power = a32_safe_map_d(input, .6, 5, 0, 1);

  if (power > .4 && start == false) {  // todo: need to check this value
    start = true;
    // naos_log("Ss");
  }
  if (start) {
    create();
  }
  if (Ireset == true) {
    reset(0, 0);
  }
  animate();
  // sin wave pattern in background

  for (int i = 0; i < NUMPIXELS; i++) {
    // check that this pixel isn't active in wind effect
    bool active = false;
    for (int j = 0; j < lengthOfImpulse; j++) {
      int p = pixel(j);
      if (i == p) {
        active = true;
      }
    }

    if (!active) {
      pixelsBackgroundAnim[i] += .04;
      int color = 60 + floor(50 * sin(pixelsBackgroundAnim[i]));     // somewhere between 20 and and 110
      double fadeDown = a32_safe_map_d(power, 0, .4, 0, color / 2);  // subtract power
      color = color - fadeDown;
      // uint32_t c = colors[i];
      // naos_log("power%d", c);

      uint8_t R = color;  // c  >> 16;
      uint8_t G = 0;
      uint8_t B = color;
      backgroundColors[i] = color;
      if (colors[i] == 0) {
        //   colors == R+G+B;
        apa_set_one(i, R, G, B);
      }
    }
  }
  //
  apa_show();
  return power;
}
void create() {
  if (out() != true) {  // check if we have reached the end of the demensions
    next();             // delay(50);
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
  if (y % 2 == 0) {  // what is this for? Every third strip?
    pixels[lengthOfImpulse] = y * dimension + x;
  } else {
    pixels[lengthOfImpulse] = y * dimension + (dimension - 1 - x);
  }
  int rx;
  int ry;
  uint32_t ra = esp_random();  //
  ra = abs(ra >> 30);          // get just two bits from esp_random();
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
  lengthOfImpulse++;
  Ireset = false;
}

int mountainX(int mx, int my, int x, int y) {
  int M1x = mx;
  int M1y = my;
  int distance = sqrt(x - M1x) + sqrt(y - M1y);
  if (distance == 1 && M1x >= x && M1y >= y) {
    if (M1x - x == 1) {
      return 0;
    } else {
      return 1;
    }
  } else if (distance == 2) {
    if (M1x > x && M1y > y) {
      return 1;
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
  int distance = sqrt(x - M1x) + sqrt(y - M1y);
  if (distance == 1 && M1x >= x && M1y >= y) {
    if (M1x - x == 1) {
      return 1;
    } else {
      return 0;
    }
  } else if (distance == 2) {
    if (M1x > x && M1y > y) {
      return 0;
    } else if (M1x > y) {
      return 0;
    } else {
      return 1;
    }
  } else {
    return 2;
  }
}

void animate() {
  if (index01 < lengthOfImpulse) {
    int p = pixel(index01);
    uint32_t c = color(index01);
    uint8_t R = backgroundColors[p];  // c  >> 16;
    uint8_t G = (c >> 8) & 0xff;
    uint8_t B = c & 0xff;
    if (B + backgroundColors[p] < 254) {
      B += backgroundColors[p];
    }
    apa_set_one(p, R, G, B);
    index01++;
  } else {
    index01 = 0;
    setcolor();
  }
}

void setcolor() {
  if (countRe >= lengthOfImpulse && lengthOfImpulse > 3) {
    Ireset = true;  // animation over
  }
  red = 0x000000;
  for (int i = lengthOfImpulse - 1; i >= 0; i--) {
    if (green > 0x000500) {  // fade green first
      green -= 0x000500;
      //  blue += 0x000005;
      //  } else if (power > .4) {  //todo: need to check this value
      //  green = 0x000000;
      //  blue = 0x0000FF;
    } else if (blue > 0x00010) {  // fade blue
      green = 0x000000;
      blue -= 0x00010;
    } else {
      green = 0x000000;
      blue = 0x000000;
      countRe++;
    }
    colors[i] = red + green + blue;
  }
}

void reset(int ix, int iy) {
  x = ix;
  y = iy;
  lengthOfImpulse = 0;
  index01 = 0;
  green = 0x00FF00;
  blue = 0x0000FF;
  red = 0x000000;
  start = false;
  countRe = 0;
  Ireset = false;
}

int pixel(int i) { return pixels[i]; }

int color(int i) { return colors[i]; }
