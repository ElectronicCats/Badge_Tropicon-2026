// WS2812B DMA SPI driver config -- must be before include
#define WS2812DMA_IMPLEMENTATION
#define WSRBG                      // Fix color order: callback is 0xRRGGBB, LED needs GRB
#define NR_LEDS 1

#include "ch32fun.h"
#ifdef DEBUG_PRINT
#include <stdio.h>
#endif
#include "ws2812b_dma_spi_led_driver.h"
#include "tmp102.h"
#include "breathing.h"
#include "color_map.h"

// Current LED color (set by main loop, read by WS2812B callback)
static volatile uint8_t led_r = 0;
static volatile uint8_t led_g = 0;
static volatile uint8_t led_b = 0;

// WS2812B driver callback -- called from DMA ISR for each LED
uint32_t WS2812BLEDCallback(int ledno)
{
    (void)ledno;
    // Pack as 0x00RRGGBB
    return ((uint32_t)led_r << 16) | ((uint32_t)led_g << 8) | (uint32_t)led_b;
}

int main(void)
{
    SystemInit();
    funGpioInitAll();

    // Init peripherals
    tmp102_init();
    breathing_init();
    WS2812BDMAInit();

    int16_t temp_raw = 0;
    uint16_t temp_counter = 0;       // counts 30ms ticks toward 2s
    #define TEMP_READ_INTERVAL 66    // 66 * 30ms ~ 2 seconds

    // Initial temperature read
    temp_raw = tmp102_read_raw();

    while (1)
    {
        // Read temperature every ~2 seconds
        temp_counter++;
        if (temp_counter >= TEMP_READ_INTERVAL)
        {
            temp_counter = 0;
            temp_raw = tmp102_read_raw();
#ifdef DEBUG_PRINT
            // Temperature in 0.0625°C units; print as fixed point
            int16_t whole = temp_raw >> 4;
            int16_t frac = (temp_raw & 0xF) * 625;
            printf("T=%d.%04d raw=%d R=%d G=%d B=%d\n", whole, frac, temp_raw, led_r, led_g, led_b);
#endif
        }

        // Map temperature to base color
        rgb_t base = color_from_temp(temp_raw);

        // Apply breathing brightness
        uint8_t brightness = breathing_step();
        led_r = (uint8_t)(((uint16_t)base.r * brightness) >> 8);
        led_g = (uint8_t)(((uint16_t)base.g * brightness) >> 8);
        led_b = (uint8_t)(((uint16_t)base.b * brightness) >> 8);

        // Send to WS2812B
        while (WS2812BLEDInUse);       // wait for previous DMA to finish
        WS2812BDMAStart(NR_LEDS);

        // ~30ms frame delay (64 steps * 30ms = ~1.9s breathing cycle)
        Delay_Ms(30);
    }
}
