// Sun (laser)

#include <naos.h>
#include "apds.h"
#include <art32/numbers.h>
#include <driver/gpio.h>
#include "light.h"
#include "neo5.h"
#include "servo.h"
#include "neo3.h"
#include <art32/smooth.h>
#include <esp_system.h>

static a32_smooth_t* o1_smoothing;

static uint8_t servoPos = 90;
static uint8_t servoMax = 160;
static uint8_t servoMin = 20;
static uint32_t activeSun = 1;
static uint32_t random = 1;
static unsigned long previousMillis = 0;
static uint8_t NUMPIXELS = 11;
static uint8_t tower_R = 0;
static uint8_t tower_G = 0;
static uint8_t tower_B = 0;
void object5_setup() {
  // init apds chip
  apds_init();

  // init neo pixels
  neo3_init(29, NEO3_DEFAULT_PIN); //  this object just has 29 neopixels for ambient lighting

  // init lighting
  light_init(255, 200, 0, 0, 28);

  // initialize servos
  servo_setup(false);

  neo5_init(NUMPIXELS, NEO5_DEFAULT_PIN);
  neo5_show(); // clear pixels

  // smoothing values for object output
  o1_smoothing = a32_smooth_new(20);

// cofigure joystick

    gpio_set_direction(GPIO_NUM_32, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_INPUT);

    gpio_set_pull_mode(GPIO_NUM_32, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(GPIO_NUM_33, GPIO_PULLUP_ONLY);


  // prepare in master config for h Bridge
    gpio_config_t master = {.pin_bit_mask = GPIO_SEL_16,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE};
    // configure in master pin
    ESP_ERROR_CHECK(gpio_config(&master));
    ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_16, 1));

    // laser pins setup
  gpio_config_t laser1 = {.pin_bit_mask = GPIO_SEL_19,
          .mode = GPIO_MODE_OUTPUT,
          .pull_up_en = GPIO_PULLUP_ENABLE,
          .pull_down_en = GPIO_PULLDOWN_DISABLE,
          .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser1));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));

  gpio_config_t laser2 = {.pin_bit_mask = GPIO_SEL_18,
          .mode = GPIO_MODE_OUTPUT,
          .pull_up_en = GPIO_PULLUP_ENABLE,
          .pull_down_en = GPIO_PULLDOWN_DISABLE,
          .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser2));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));

  gpio_config_t laser3 = {.pin_bit_mask = GPIO_SEL_23,
          .mode = GPIO_MODE_OUTPUT,
          .pull_up_en = GPIO_PULLUP_ENABLE,
          .pull_down_en = GPIO_PULLDOWN_DISABLE,
          .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser3));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));

  gpio_config_t laser4 = {.pin_bit_mask = GPIO_SEL_17,
          .mode = GPIO_MODE_OUTPUT,
          .pull_up_en = GPIO_PULLUP_ENABLE,
          .pull_down_en = GPIO_PULLDOWN_DISABLE,
          .intr_type = GPIO_INTR_DISABLE};

  ESP_ERROR_CHECK(gpio_config(&laser4));
  ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));
}

double object5_loop() {
  // read red light
  apds_color_t c = apds_read();
  //naos_log("c: %d, r: %d, g: %d, b: %d", c.c, c.r, c.g, c.b);

  // calculate power
    double redPercantage = c.r;
    double ambientLight = c.c;
     redPercantage = redPercantage/ambientLight;
    double power = a32_safe_map_d(redPercantage, 0.45, .9, 0, 1); // taking into account that there is ≈ 30% red in ambient light.
    power = a32_smooth_update(o1_smoothing, power);
    // set up tower glow
   uint16_t redLightIntensity = a32_safe_map_d(power,0,1,0,500);
   //
if (tower_R<redLightIntensity && tower_R<255) {
    tower_R++;
} else if (tower_R>1){
    tower_R--;
}


    if (tower_G<redLightIntensity-250 && tower_G<150) {
        tower_G++;
    } else if (tower_G>1){
        tower_G--;
    }

    if (tower_B<redLightIntensity-400 && tower_B<100) {
        tower_B++;
    } else if (tower_B>1){
        tower_B--;
    }




    for (uint8_t i = 4; i < NUMPIXELS; i++) {
       neo5_set_one(i,tower_R,tower_G,tower_B);
    }

  //  naos_log("redLightIntensity: %d", redLightIntensity);
  // joystick
  if (gpio_get_level(GPIO_NUM_32) == 1 && servoPos < servoMax) { //left TODO  check pin number
    servoPos++;
    servo_write1(servoPos);
  }
  if (gpio_get_level(GPIO_NUM_33) == 1 && servoPos > servoMin) {//right TODO  check pin number
    servoPos--;
      servo_write1(servoPos);
  }
  if(servoPos > servoMax) {
      servoPos--;
  } else if (servoPos < servoMin) {
      servoPos++;
  }

  // naos_log("servoPos: %d", servoPos);
    unsigned long currentMillis = naos_millis();

    if (currentMillis - previousMillis >= 7000) {
        previousMillis = currentMillis;
  // randomly select sun:
    while (activeSun == random) {
        random = esp_random();  //
        random = abs(random>>30); // get just two bits from esp_random();
        random +=1; // make sure we have values between 1 and 4
    }
    naos_log("random: %d,activeSun: %d", random,activeSun);
    activeSun = random;
  // laser/sun code
      switch (activeSun) {
        case 1:
            ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 1));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));
              neo5_set_one(0,255,200,0);
              neo5_set_one(1,0,0,0);
              neo5_set_one(2,0,0,0);
              neo5_set_one(3,0,0,0);
              servoMin = 70; //todo find the servo min and max for all lasers
              servoMax = 100;
              break;
        case 2:
            ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 1));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));
              neo5_set_one(0,0,0,0);
              neo5_set_one(1,255,200,0);
              neo5_set_one(2,0,0,0);
              neo5_set_one(3,0,0,0);
              servoMin = 90;
              servoMax = 160;
              break;
        case 3:
            ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 1));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 0));
              neo5_set_one(0,0,0,0);
              neo5_set_one(1,0,0,0);
              neo5_set_one(2,255,200,0);
              neo5_set_one(3,0,0,0);
              servoMin = 10;
              servoMax = 90;
              break;
        case 4:
            ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_19, 0));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_18, 0));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_23, 0));
              ESP_ERROR_CHECK(gpio_set_level(GPIO_NUM_17, 1));
              neo5_set_one(0,0,0,0);
              neo5_set_one(1,0,0,0);
              neo5_set_one(2,0,0,0);
              neo5_set_one(3,255,200,0);
              servoMin = 90;
              servoMax = 150;
              break;
        default:
          break;
      }
    }

    neo5_show();
  light_set(power);
  neo3_show();
  return power;
}
