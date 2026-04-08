#ifndef COLOR_MAP_H
#define COLOR_MAP_H

#include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

// Map temperature (in raw TMP102 units, 0.0625C per LSB) to RGB color.
// Beer theme: <8C=blue, 8-15C=green, >15C=red, with smooth transitions.
rgb_t color_from_temp(int16_t temp_raw);

#endif
