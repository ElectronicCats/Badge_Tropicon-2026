#include "breathing.h"

// 64-step sine table, values 0-255 (half-wave, mirrored for full cycle)
// sin(i * PI / 64) * 255, with minimum brightness of 8 so LED never fully off
static const uint8_t sine_table[64] = {
      8,  12,  18,  25,  33,  42,  51,  61,
     71,  81,  91, 101, 111, 121, 131, 140,
    149, 158, 166, 174, 181, 188, 194, 199,
    204, 209, 213, 216, 219, 221, 223, 224,
    225, 224, 223, 221, 219, 216, 213, 209,
    204, 199, 194, 188, 181, 174, 166, 158,
    149, 140, 131, 121, 111, 101,  91,  81,
     71,  61,  51,  42,  33,  25,  18,  12
};

static uint8_t step_index = 0;

void breathing_init(void)
{
    step_index = 0;
}

uint8_t breathing_step(void)
{
    uint8_t val = sine_table[step_index];
    step_index = (step_index + 1) & 0x3F; // wrap at 64
    return val;
}
