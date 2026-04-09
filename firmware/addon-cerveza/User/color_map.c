#include "color_map.h"

// Temperature thresholds in TMP102 raw units (1 unit = 0.0625C)
// Compensated for glass + PCB offset (~+5-9C over liquid temp)
// 14C = 224,  18C = 288,  23C = 368
#define TEMP_14C  224
#define TEMP_18C  288
#define TEMP_23C  368

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

    if (temp_raw < TEMP_14C) {
        // Beer is cold (<7C): solid blue
        c.r = 0; c.g = 0; c.b = 255;
    } else if (temp_raw < TEMP_18C) {
        // Beer is ideal (7-12C): transition blue -> green
        c.r = 0;
        c.g = interpolate(0, 255, temp_raw, TEMP_14C, TEMP_18C);
        c.b = interpolate(255, 0, temp_raw, TEMP_14C, TEMP_18C);
    } else if (temp_raw < TEMP_23C) {
        // Beer warming up (12-16C): transition green -> red
        c.r = interpolate(0, 255, temp_raw, TEMP_18C, TEMP_23C);
        c.g = interpolate(255, 0, temp_raw, TEMP_18C, TEMP_23C);
        c.b = 0;
    } else {
        // Beer is warm (>16C): solid red - get another one!
        c.r = 255; c.g = 0; c.b = 0;
    }

    return c;
}
