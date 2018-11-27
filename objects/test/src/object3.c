//
// Created by Franzke Luke on 21/11/18.
// solar 1:
#include "servoIOT.h"
#include <driver/gpio.h>
#include <art32/numbers.h>
#include <art32/smooth.h>
#include <math.h>
#include "neoPixelStandard.h"
#include <driver/adc.h>
#include <naos.h>


static uint8_t neoR = 0;
static uint8_t neoG = 0;
static uint8_t neoB = 254;

int getsenor(int sensorNo);


static int servohorizontal = 90;

static int servohorizontalLimitHigh = 180;
static int servohorizontalLimitLow = 0;

static int servovertical = 90;

static int servoverticalLimitHigh = 160;
static int servoverticalLimitLow = 30;



void object3_setup(){
    servo1Setup();
    servo2Setup();
   // neoPixelStandard_setup(neoR,neoG,neoB);
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_11);

    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_11);

    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_11);

    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_11);
}

float object3_test() {
    servo1Write(30);
    servo2Write(0);
    //  neoPixelStandard(0);
    int lt = getsenor(1); // top left
    int rt = getsenor(3); // top right
    int ld = getsenor(4); // down links
    int rd = getsenor(2); // down right
    naos_log("lt: %d,rt: %d,ld: %d,rd: %d", lt,rt,ld,rd);

    //definition der Toleranz und Geschwindigkeit
    //int dtime = 15;
    int tol = 15; // tolerance

    //durchschnittswerte rechnen
    int avt = (lt + rt) / 2; // durchschnitt top
    int avd = (ld + rd) / 2; // durchschnitt bottom
    int avl = (lt + ld) / 2; // durchschnitt links
    int avr = (rt + rd) / 2; // durchschnitt rechts

    int dvert = avt - avd; // prüfung der differenz vertikal
    int dhoriz = avl - avr;// prüfung der differenz horizontal
    naos_log("dvert: %d,dhoriz: %d", dvert,dhoriz);
    return 0;
}
float object3_loop() {
  //  neoPixelStandard(0);
    int lt = getsenor(1); // top left
    int rt = getsenor(3); // top right
    int ld = getsenor(4); // down links
    int rd = getsenor(2); // down right
   // naos_log("lt: %d,rt: %d,ld: %d,rd: %d", lt,rt,ld,rd);

    //definition der Toleranz und Geschwindigkeit
    //int dtime = 15;
    int tol = 40; // tolerance

    //durchschnittswerte rechnen
    int avt = (lt + rt) / 2; // durchschnitt top
    int avd = (ld + rd) / 2; // durchschnitt bottom
    int avl = (lt + ld) / 2; // durchschnitt links
    int avr = (rt + rd) / 2; // durchschnitt rechts

    int dvert = avt - avd; // prüfung der differenz vertikal
    int dhoriz = avl - avr;// prüfung der differenz horizontal


    //berechnung und anpassung der servopositionen
    if (-1 * tol > dvert || dvert > tol) {
        if (avt > avd) {
            //servovertical = ++servovertical;
            servovertical++;
        } else if (avt < avd)
        {
            //servovertical = --servovertical;
            servovertical--;

        }
        if (servovertical > servoverticalLimitHigh) {
            servovertical = servoverticalLimitHigh;
        }
        if (servovertical < servoverticalLimitLow) {
            servovertical = servoverticalLimitLow;
        }
      //  vertical.write(servovertical);
        servo1Write(servovertical);
    }

    if (servovertical > 90) {
        if (-1 * tol > dhoriz || dhoriz > tol) {
            if (avl > avr) {
                //servohorizontal = ++servohorizontal;
                servohorizontal++;

            } else if(avl < avr) {
                // servohorizontal = --servohorizontal;
                servohorizontal--;

            } else if(avl == avr) {
                // nothing
            }
            if (servohorizontal > servohorizontalLimitHigh) {
                servohorizontal = servohorizontalLimitHigh;
            }
            if (servohorizontal < servohorizontalLimitLow) {
                servohorizontal = servohorizontalLimitLow;
            }
            //  horizontal.write(servohorizontal);
            servo2Write(servohorizontal);
        }
    } else {
        if (-1 * tol > dhoriz || dhoriz > tol) {
            if (avl > avr) {
                // servohorizontal = --servohorizontal;
                servohorizontal--;

            } else if (avl < avr) {
                // servohorizontal = ++servohorizontal;
                servohorizontal++;

            } else if (avl == avr) {
                // nothing
            }
            if (servohorizontal > servohorizontalLimitHigh){
                servohorizontal = servohorizontalLimitHigh;
            }
            if (servohorizontal < servohorizontalLimitLow) {
                servohorizontal = servohorizontalLimitLow;
            }
            //   horizontal.write(servohorizontal);
            servo2Write(servohorizontal);
        }
    }
//  delay(dtime);
    float msg = avt + avd + avl + avr;
    msg = msg/4;
    msg = msg / 400.0;
    if (msg > 1) {
        msg = 1;
    }
    return msg;
}

int getsenor(int sensorNo) {
    int val = 0;
    switch(sensorNo) {
        case 1 :
            val = adc1_get_raw(ADC1_CHANNEL_4);
            break;
        case 2 :
            val = adc1_get_raw(ADC1_CHANNEL_5);
            break;
        case 3 :
            val = adc1_get_raw(ADC1_CHANNEL_6);
            break;
        case 4 :
           val = adc1_get_raw(ADC1_CHANNEL_7);
            break;
    }
return val;
}

