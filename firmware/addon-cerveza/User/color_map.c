#include "color_map.h"

// Temperature thresholds in TMP102 raw units (1 unit = 0.0625C)
// 5C  = 80,  8C = 128,  12C = 192,  15C = 240
#define TEMP_5C   80
#define TEMP_8C  128
#define TEMP_12C 192
#define TEMP_15C 240

static uint8_t interpolate(uint8_t a, uint8_t b, int16_t val, int16_t lo, int16_t hi)
{
    // Linear interpolation: a at lo, b at hi
    int16_t range = hi - lo;
    if (range == 0) return b;
    int16_t pos = val - lo;
    return (uint8_t)(((int16_t)a * (range - pos) + (int16_t)b * pos) / range);
}

rgb_t color_from_temp(int16_t temp_raw)
{
    rgb_t c;

    if (temp_raw < TEMP_5C) {
        // Cold: solid blue
        c.r = 0; c.g = 0; c.b = 255;
    } else if (temp_raw < TEMP_8C) {
        // Transition blue -> green
        c.r = 0;
        c.g = interpolate(0, 255, temp_raw, TEMP_5C, TEMP_8C);
        c.b = interpolate(255, 0, temp_raw, TEMP_5C, TEMP_8C);
    } else if (temp_raw < TEMP_12C) {
        // Room temp: solid green
        c.r = 0; c.g = 255; c.b = 0;
    } else if (temp_raw < TEMP_15C) {
        // Transition green -> red
        c.r = interpolate(0, 255, temp_raw, TEMP_12C, TEMP_15C);
        c.g = interpolate(255, 0, temp_raw, TEMP_12C, TEMP_15C);
        c.b = 0;
    } else {
        // Warm: solid red
        c.r = 255; c.g = 0; c.b = 0;
    }

    return c;
}
