# Add-On Cerveza Firmware Design

## Overview

Firmware for the CH32V003F4U6 RISC-V MCU on the Addon-Cerveza board. The LED (WS2812B) always reflects the beer temperature read from the TMP102 sensor, displayed with a breathing animation effect.

## Hardware

- **MCU:** CH32V003F4U6 (TSSOP-20), 48MHz, 16KB flash, 2KB RAM
- **LED:** 1x WS2812B (addressable RGB)
- **Sensor:** TMP102 (I2C digital temperature, 12-bit, 0.0625C resolution)
- **Power:** 3.3V from battery cell via slide switch (SW2)
- **Reset:** Push button SW7 on NRST (hardware reset only)

## Pin Mapping

| GPIO | Pin | Function       | Peripheral     |
|------|-----|----------------|----------------|
| PC1  | 11  | I2C SDA        | I2C1           |
| PC2  | 12  | I2C SCL        | I2C1           |
| PC6  | 16  | WS2812B DIN    | SPI1 MOSI      |
| PD0  | 8   | TMP102 ALERT   | GPIO IN (unused v1) |
| PD1  | 18  | SWDIO          | Debug          |
| PC5  | 15  | SWCLK          | Debug          |
| PD7  | 4   | NRST           | Hardware reset |

## SDK

**ch32v003fun** (cnlohr) as a git submodule. Provides startup, linker script, SPI DMA WS2812B example, and minichlink for flashing.

## Project Structure

```
firmware/
  Makefile
  ch32v003fun/          # git submodule
  User/
    main.c              # Application entry point and main loop
    ws2812b.h/.c        # WS2812B driver (SPI DMA, based on ch32v003fun example)
    tmp102.h/.c         # TMP102 I2C driver (read temperature register)
    breathing.h/.c      # Breathing effect (sine table, brightness modulation)
```

## Application Logic

### Main Loop

```
1. Init system (clock 48MHz, I2C1, SPI1, SysTick)
2. Infinite loop:
   a. Every 2s: read TMP102 via I2C
   b. Map temperature to base color (with smooth interpolation at edges)
   c. Every ~20ms: update brightness via breathing effect
   d. Send final color to WS2812B via SPI DMA
```

### Temperature to Color Mapping (Beer Theme)

| Range    | Color  | RGB           | Meaning        |
|----------|--------|---------------|----------------|
| < 5C     | Blue   | (0, 0, 255)   | Cold           |
| 5-8C     | Blue->Green | interpolated | Transition |
| 8-15C    | Green  | (0, 255, 0)   | Room temp      |
| 12-15C   | Green->Red | interpolated | Transition |
| > 15C    | Red    | (255, 0, 0)   | Warm           |

Interpolation is linear RGB between edge values to avoid abrupt color changes.

### Breathing Effect

- 64-step sine lookup table (values 0-255)
- Full breathing cycle: ~2 seconds (64 steps * 20ms = 1.28s, adjusted to ~2s)
- Applied as: `final_color = base_color * brightness / 255` per channel

### TMP102 Driver

- I2C address: 0x48 (ADD0 to GND)
- Read register 0x00: 2 bytes, 12-bit temperature (MSB first)
- Resolution: 0.0625C per LSB
- Conversion: `temp_c = raw_value * 0.0625` (using fixed-point integer math)

### WS2812B Driver

- SPI1 MOSI (PC6) with DMA
- Based on ch32v003fun WS2812B SPI DMA example
- Encodes each WS2812B bit as SPI byte pattern to match NeoPixel timing
- Single LED: 24 bits (GRB order) = 24 SPI bytes

## Build System

```makefile
TARGET = badge-cerveza
CH32V003FUN = ch32v003fun/ch32v003fun
SRCS = User/main.c User/ws2812b.c User/tmp102.c User/breathing.c
include $(CH32V003FUN)/../ch32v003fun.mk
```

### Targets

- `make` - Build ELF/BIN/HEX
- `make flash` - Program via WCH-LinkE (minichlink)
- `make clean` - Clean build artifacts

### Toolchain

- `riscv-none-elf-gcc` (or `riscv64-unknown-elf-gcc`)

## Resource Estimates

- Flash: ~4-6KB of 16KB
- RAM: ~200-400 bytes of 2KB

## Future Considerations (Not in v1)

- PD0 ALERT pin for over-temperature notification
- Additional LED effects (pulse, blink patterns)
- Low-power sleep between readings
