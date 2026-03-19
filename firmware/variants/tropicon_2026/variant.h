#pragma once

// ══════════════════════════════════════════════════════════════════════════════
// variant.h — Custom hardware definition for ESP32-S3-WROOM-1-N8
// Board: Tropicon 2026
// Firmware: Meshtastic / PlatformIO
// ══════════════════════════════════════════════════════════════════════════════

// ── Hardware Identity ─────────────────────────────────────────────────────────
#define HW_VENDOR HardwareModel_PRIVATE_HW

// ── SX1262 LoRa — SPI Bus 1 ───────────────────────────────────────────────
// Meshtastic requires the SX126X_* prefix to properly initialize the radio,
// even if the module is an E22 variant.
#define USE_SX1262

#define SX126X_CS      38   // IO38 = CS0   (Chip Select)
#define SX126X_DIO1    48   // IO48 = DIO   (IRQ / DIO1)
#define SX126X_BUSY    39   // IO39 = BUSY  (Busy indicator)
#define SX126X_RESET   47   // IO47 = RST   (Reset, active low)

// SPI pins for SX1262 bus (VSPI / SPI Bus 1)
#define LORA_SCK       37   // IO37 = SCK
#define LORA_MISO      36   // IO36 = MISO
#define LORA_MOSI      35   // IO35 = MOSI

// TCXO: required for SX1262 E22 modules to prevent continuous reset loops.
#define SX126X_E22
// If SX126X_E22 causes issues, try setting the TCXO voltage directly instead:
// #define SX126X_DIO3_TCXO_VOLTAGE 1.8

// ── I2C Bus 1 — AHT30 Temperature & Humidity Sensor ──────────────────────────
#define I2C_SDA1       42   // IO42 = SDA1
#define I2C_SCL1       41   // IO41 = SCL1

// ── Display ER-TFT2.79-1 (controller: NV3007) — SPI Bus 2 (HSPI) ─────────────
//
// IMPORTANT — Interface clarification:
//   The NV3007 uses a 4-wire SPI interface exclusively (no I2C support).
//   The labels "SDA" and "SCL" in the module's FPC connector are the
//   manufacturer's own naming for the SPI data and clock lines respectively.
//   This is common practice in Chinese COG/COF display modules.
//
//   NV3007 pin  │ KiCad label │ ESP32-S3 pin │ SPI function
//   ────────────┼─────────────┼──────────────┼──────────────
//   SDA         │ SDA         │ IO11         │ MOSI (serial data in)
//   SCL         │ SCL         │ IO12         │ SCLK (serial clock)
//   D/CX        │ RS          │ IO09         │ Data/Command select
//   CSX         │ CS1         │ IO10         │ Chip Select (active low)
//   RESX        │ RSTB        │ IO21         │ Reset (active low)
//   TE          │ TE          │ IO13         │ Tearing Effect (frame sync)
//
//   There is NO MISO line in standard 4-wire SPI mode of the NV3007.
//
// NOTE on driver compatibility:
//   The NV3007 command set is NOT compatible with ST7789 / ILI9341.
//   A custom initialization sequence is required via TFT_eSPI User_Setup.h
//   or a dedicated NV3007 driver. Do NOT use USE_ST7789.

#define TFT_SPI_HOST   2    // HSPI (SPI Bus 2 of ESP32-S3)

#define TFT_CS         10   // IO10 = CSX   (Chip Select, active low)
#define TFT_DC         9    // IO09 = D/CX  (RS in KiCad — Data/Command select)
#define TFT_RST        21   // IO21 = RESX  (Reset, active low)
#define TFT_MOSI       11   // IO11 = SDA   (SPI MOSI — NV3007 naming for MOSI)
#define TFT_SCLK       12   // IO12 = SCL   (SPI SCLK — NV3007 naming for SCK)
// No TFT_MISO: NV3007 in 4-wire SPI mode has no dedicated MISO line.

// IO13 = TE (Tearing Effect) — vertical sync output from NV3007.
// Used to synchronize frame writes and prevent display tearing.
// Can be left unconnected during initial bringup.
#define TFT_TE         13   // IO13 = TE

// Backlight PWM control
#define TFT_BL         2    // IO02 = LCD_BRIGHT (PWM output)
#define TFT_BACKLIGHT_ON HIGH

// Native resolution of the ER-TFT2.79-1 / NV3007: 168 × 428 pixels.
// Swap WIDTH/HEIGHT here if mounting the display in landscape orientation.
#define TFT_WIDTH      168  // Short axis (columns)
#define TFT_HEIGHT     428  // Long axis  (rows)

// ── Navigation Buttons ────────────────────────────────────────────────────────
// IO40 (UP) is used as the primary Meshtastic button.
#define BUTTON_PIN              40   // IO40 = UP   (primary / main button)
#define BUTTON_PIN_ALT_UP       40   // IO40 = UP
#define BUTTON_PIN_ALT_DOWN     43   // TXD0 (GPIO43 in ESP32-S3) = DOWN
#define BUTTON_PIN_ALT_LEFT      1   // IO01 = LEFT
#define BUTTON_PIN_ALT_RIGHT    18   // IO18 = RIGHT

// ── Si4463 (second radio) — SPI Bus 2 — RESERVED ─────────────────────────────
// Pins are mapped here for reference. Uncomment when Si4463 support is added.
//
// #define SI4463_CS    14  // IO14 = CS2
// #define SI4463_SDN    4  // IO04 = SDN
// #define SI4463_IRQ    8  // IO08 = IRQ
// #define SI4463_SCK   15  // IO15 = SCK2
// #define SI4463_MOSI  16  // IO16 = MOSI2
// #define SI4463_MISO  17  // IO17 = MISO2