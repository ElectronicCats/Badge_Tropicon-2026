#ifndef COLOR_MAP_H
#define COLOR_MAP_H

#include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

// Map temperature (in raw TMP102 units, 0.0625C per LSB) to RGB color.
// Beer theme: <5C=blue, 5-8C=blue->green, 8-12C=green, 12-15C=green->red, >15C=red.
rgb_t color_from_temp(int16_t temp_raw);

#endif
