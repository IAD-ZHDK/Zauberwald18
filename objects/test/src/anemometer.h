#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <naos.h>

#include <stdbool.h>


/**
 * Callback is execute with rotational change up to a frequency of 1ms.
 *
 * @param r The measured rotations.
 */
typedef void (*enc_callback_t)(double r);

/**
 * Initialize the encoder sub system.
 *
 * @param cb The encoder callback.
 */
void anenmo_init();
/**
 * get the HRZ from encoder
 */
float anemo_getHrz();

