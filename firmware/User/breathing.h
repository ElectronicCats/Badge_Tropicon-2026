#ifndef BREATHING_H
#define BREATHING_H

#include <stdint.h>

// Initialize breathing state
void breathing_init(void);

// Advance one step. Call every ~30ms for a ~2s full cycle.
// Returns brightness 0-255.
uint8_t breathing_step(void);

#endif
