#include <naos.h>

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
void enc_init(enc_callback_t cb);
