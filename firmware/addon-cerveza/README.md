# Addon-Cerveza Firmware

Firmware for the CH32V003F4U6 on the Addon-Cerveza board. Reads beer temperature
from a TMP102 sensor and shows it as a breathing WS2812B LED:

- **Blue** (< 8C): Cold beer
- **Green** (8-15C): Room temperature
- **Red** (> 15C): Warm beer

Smooth color transitions between ranges.

## Requirements

- RISC-V GCC toolchain (`riscv-none-elf-gcc` or `riscv64-unknown-elf-gcc`)
- WCH-LinkE programmer

## Build

```bash
cd firmware
git submodule update --init
make
```

## Flash

Connect WCH-LinkE to SWDIO (PD1) and SWCLK (PC5):

```bash
make flash
```

## Pin Mapping

| GPIO | Function     |
|------|-------------|
| PC1  | I2C SDA (TMP102) |
| PC2  | I2C SCL (TMP102) |
| PC6  | WS2812B DIN (SPI MOSI) |
| PD1  | SWDIO (debug) |
| PC5  | SWCLK (debug) |
