#include "driver/ledc.h"
#ifndef NAOS_PROJECT_SERVOIOT_H
#define NAOS_PROJECT_SERVOIOT_H

void servo1Setup();
void servo1Write(float angle);
void servo2Setup();
void servo2Write(float angle);

#endif  // NAOS_PROJECT_SERVOIOT_H
