#include "breathing.h"

// 64-step sine table, values 0-255 (half-wave, mirrored for full cycle)
// sin(i * PI / 64) * 255, with minimum brightness of 8 so LED never fully off
// 50% max brightness: sin(i * PI / 64) * 127, minimum 4
static const uint8_t sine_table[64] = {
      4,   6,   9,  12,  16,  21,  25,  30,
     35,  40,  45,  50,  55,  60,  65,  70,
     74,  79,  83,  87,  90,  94,  97, 99,
    102, 104, 106, 108, 109, 110, 111, 112,
    112, 112, 111, 110, 109, 108, 106, 104,
    102,  99,  97,  94,  90,  87,  83,  79,
     74,  70,  65,  60,  55,  50,  45,  40,
     35,  30,  25,  21,  16,  12,   9,   6
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
