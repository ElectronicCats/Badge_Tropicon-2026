#pragma once

// ── Hardware Identity ─────────────────────────────────────────────────────
#define HW_VENDOR HardwareModel_PRIVATE_HW

// ── SX1262 LoRa — SPI Bus 1 ───────────────────────────────────────────────
// Meshtastic requiere el prefijo SX126X_* para inicializar el radio correctamente
#define USE_SX1262

#define SX126X_CS      38   // IO38 = CS0
#define SX126X_DIO1    48   // IO48 = DIO
#define SX126X_BUSY    39   // IO39 = BUSY
#define SX126X_RESET   47   // IO47 = RST

// SPI pins del bus del SX1262
#define LORA_SCK       37   // IO37
#define LORA_MISO      36   // IO36
#define LORA_MOSI      35   // IO35

// TCXO: requerido para SX1262 E22, evita reinicios continuos
#define SX126X_E22
// Alternativa explícita si SX126X_E22 no funciona:
// #define SX126X_DIO3_TCXO_VOLTAGE 1.8

// ── I2C Bus 1 — AHT30 sensor ──────────────────────────────────────────────
#define I2C_SDA1       42   // IO42
#define I2C_SCL1       41   // IO41

// ── Display TFT ER-TFT2.79-1 (controlador NV3007) — SPI Bus 2 ────────────
// El NV3007 usa interfaz SPI similar a ST7789; se configura vía TFT_eSPI
#define USE_ST7789      // NV3007 es compatible con comandos ST7789 en modo SPI

#define TFT_SPI_HOST   2    // HSPI (segundo bus SPI del ESP32-S3)
#define TFT_CS         10   // IO10 = CS1
#define TFT_DC         9    // IO09 = RS (Register Select = D/C)
#define TFT_RST        21   // IO21 = RSTB
#define TFT_MOSI       11   // IO11 = SDA (datos SPI del display)
#define TFT_SCLK       12   // IO12 = SCL (reloj SPI del display)
// IO13 = TE (Tearing Effect) — señal de sincronía vertical, opcional
#define TFT_TE         13   // IO13 = TE (puede dejarse sin conectar al inicio)

// Brillo por PWM (backlight)
#define TFT_BL         2    // IO02 = LCD_BRIGHT
#define TFT_BACKLIGHT_ON HIGH

// Resolución de la pantalla ER-TFT2.79-1
#define TFT_WIDTH      428
#define TFT_HEIGHT     142

// ── Botones de navegación ─────────────────────────────────────────────────
#define BUTTON_PIN     40   // IO40 = UP  (botón principal de Meshtastic)
#define BUTTON_PIN_ALT_UP    40  // IO40
#define BUTTON_PIN_ALT_DOWN  3   // TXD0 = DOWN (GPIO3 en ESP32-S3)
#define BUTTON_PIN_ALT_LEFT  1   // IO01 = LEFT
#define BUTTON_PIN_ALT_RIGHT 18  // IO18 = RIGHT

// ── Si4463 (segundo radio) — SPI Bus 2 ───────────────────────────────────
// Comparte bus SPI con el display, con CS diferente
// Pines mapeados de la tabla para referencia futura:
// IO14 = CS2, IO15 = SCK2, IO16 = MOSI2, IO17 = MISO2
// IO04 = SDN, IO05 = IO2, IO06 = IO0, IO07 = IO1, IO08 = IRQ
// Descomentar cuando se implemente soporte Si4463:
// #define SI4463_CS   14
// #define SI4463_SDN  4
// #define SI4463_IRQ  8