#include "servoIOT.h"
#include <esp_types.h>
#include <string.h>
#include <stdlib.h>
#include "rom/gpio.h"
#include "driver/ledc.h"
#include "driver/pcnt.h"
#include "soc/ledc_struct.h"
#include <naos.h>

#define SERVO_LEDC_INIT_FREQ (50)
#define SERVO_DUTY_MIN_US    (544)
#define SERVO_DUTY_MAX_US    (2400)
#define SERVO_SEC_TO_US      (1000000)
#define m_max_angle (180)

ledc_mode_t speed_mode = LEDC_LOW_SPEED_MODE;
float m_full_duty;
ledc_timer_t tim_idx1 = LEDC_TIMER_2;
ledc_timer_t tim_idx2 = LEDC_TIMER_3;
ledc_channel_t m1_chn = LEDC_CHANNEL_4;
ledc_channel_t m2_chn = LEDC_CHANNEL_5;

void servo1Setup() {
    m_full_duty = (1 << LEDC_TIMER_15_BIT) - 1;
    ledc_timer_config_t ledc_timer;
    ledc_timer.duty_resolution = LEDC_TIMER_15_BIT; // resolution of PWM duty
    ledc_timer.freq_hz = SERVO_LEDC_INIT_FREQ;                     // frequency of PWM signal
    ledc_timer.speed_mode = speed_mode;           // timer mode
    ledc_timer.timer_num = tim_idx1;            // timer index
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_ch;
    ledc_ch.channel    = m1_chn;
    ledc_ch.duty       = 0;          //todo: set init angle
    ledc_ch.gpio_num   = GPIO_NUM_25;
    ledc_ch.speed_mode = speed_mode;
    ledc_ch.timer_sel  = tim_idx1;
    ledc_channel_config(&ledc_ch);
}

void servo1Write(float angle)  {
    float angle_us = angle / m_max_angle * (SERVO_DUTY_MAX_US - SERVO_DUTY_MIN_US) + SERVO_DUTY_MIN_US;
    naos_log("angle us: %f", angle_us);
    uint32_t duty = m_full_duty * ((int)angle_us) / (SERVO_SEC_TO_US / SERVO_LEDC_INIT_FREQ);
    naos_log("duty: %d", duty);
    ledc_set_duty(speed_mode, m1_chn, duty);
    ledc_update_duty(speed_mode, m1_chn);
}

void servo2Setup() {
    m_full_duty = (1 << LEDC_TIMER_15_BIT) - 1;
    ledc_timer_config_t ledc_timer;
    ledc_timer.duty_resolution = LEDC_TIMER_15_BIT; // resolution of PWM duty
    ledc_timer.freq_hz = SERVO_LEDC_INIT_FREQ;                     // frequency of PWM signal
    ledc_timer.speed_mode = speed_mode;           // timer mode
    ledc_timer.timer_num = tim_idx2;            // timer index
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_ch;
    ledc_ch.channel    = m2_chn;
    ledc_ch.duty       = 0;          //todo: set init angle
    ledc_ch.gpio_num   = GPIO_NUM_15;
    ledc_ch.speed_mode = speed_mode;
    ledc_ch.timer_sel  = tim_idx2;
    ledc_channel_config(&ledc_ch);
}

void servo2Write(float angle)  {
    float angle_us = angle / m_max_angle * (SERVO_DUTY_MAX_US - SERVO_DUTY_MIN_US) + SERVO_DUTY_MIN_US;
    naos_log("angle us: %f", angle_us);
    uint32_t duty = m_full_duty * ((int)angle_us) / (SERVO_SEC_TO_US / SERVO_LEDC_INIT_FREQ);
    naos_log("duty: %d", duty);
    ledc_set_duty(speed_mode, m2_chn, duty);
    ledc_update_duty(speed_mode, m2_chn);
}
