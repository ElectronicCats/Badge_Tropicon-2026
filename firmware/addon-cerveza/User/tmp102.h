#ifndef TMP102_H
#define TMP102_H

#include <stdint.h>

// Initialize I2C1 for TMP102 communication (PC1=SDA, PC2=SCL)
void tmp102_init(void);

// Read temperature in units of 0.0625C (fixed-point)
// Returns raw 12-bit value. Divide by 16 for degrees C.
int16_t tmp102_read_raw(void);

#endif
