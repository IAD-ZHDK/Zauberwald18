#ifndef MOT_H
#define MOT_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Initialize motor.
 */
void mot_init(bool twoMotors);

/**
 * Approach specified target.
 *
 * @param speed The target speed.
 * @return
 */
void mot_set(double speed);

#endif  // MOT_H