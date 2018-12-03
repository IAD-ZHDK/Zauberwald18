/**
 * Initialize the anemometer.
 */
void anemo_init(gpio_num_t pin);

/**
 * Returns the rotation frequency of the anemometer in Hz.
 */
double anemo_get();
