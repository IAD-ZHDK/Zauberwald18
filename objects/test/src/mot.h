#include <stdbool.h>
#include <stdint.h>

/**
 * Initialize motor.
 *
 * @param two_motors Enable two motors.
 */
void mot_init(bool two_motors);

/**
 * Set motor speed.
 *
 * @param speed The target speed between 0 and 1023.
 */
void mot_set(double speed);
