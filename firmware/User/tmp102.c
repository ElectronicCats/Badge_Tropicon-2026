#include "ch32fun.h"
#include "tmp102.h"

#define TMP102_ADDR       0x48
#define TMP102_REG_TEMP   0x00

#define I2C_TIMEOUT       10000

// I2C clock rates
#define I2C_PRERATE   1000000  // APB1 prescaler target (1MHz)
#define I2C_CLKRATE    100000  // I2C bus speed (100kHz standard mode)

static uint8_t i2c_error(void)
{
    // Generate STOP on error to release bus
    I2C1->CTLR1 |= I2C_CTLR1_STOP;
    return 1;
}

void tmp102_init(void)
{
    uint16_t tempreg;

    // Enable I2C1 and GPIOC clocks
    RCC->APB1PCENR |= RCC_APB1Periph_I2C1;
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO;

    // PC1 = SDA: alternate function open-drain 10MHz
    GPIOC->CFGLR &= ~(0xf << (4 * 1));
    GPIOC->CFGLR |= (GPIO_CFGLR_OUT_10Mhz_AF_OD) << (4 * 1);

    // PC2 = SCL: alternate function open-drain 10MHz
    GPIOC->CFGLR &= ~(0xf << (4 * 2));
    GPIOC->CFGLR |= (GPIO_CFGLR_OUT_10Mhz_AF_OD) << (4 * 2);

    // Reset I2C1
    RCC->APB1PRSTR |= RCC_APB1Periph_I2C1;
    RCC->APB1PRSTR &= ~RCC_APB1Periph_I2C1;

    // Set peripheral input clock frequency
    tempreg = I2C1->CTLR2;
    tempreg &= ~I2C_CTLR2_FREQ;
    tempreg |= (FUNCONF_SYSTEM_CORE_CLOCK / I2C_PRERATE) & I2C_CTLR2_FREQ;
    I2C1->CTLR2 = tempreg;

    // Set I2C clock speed (standard mode 100kHz)
    tempreg = (FUNCONF_SYSTEM_CORE_CLOCK / (2 * I2C_CLKRATE)) & I2C_CKCFGR_CCR;
    I2C1->CKCFGR = tempreg;

    // Enable I2C
    I2C1->CTLR1 |= I2C_CTLR1_PE;
    I2C1->CTLR1 |= I2C_CTLR1_ACK;
}

int16_t tmp102_read_raw(void)
{
    int32_t timeout;
    uint8_t msb, lsb;

    // --- Write phase: send register address ---

    // Wait for bus idle
    timeout = I2C_TIMEOUT;
    while ((I2C1->STAR2 & I2C_STAR2_BUSY) && (timeout--));
    if (timeout == -1) { i2c_error(); return 0; }

    // START
    I2C1->CTLR1 |= I2C_CTLR1_START;
    timeout = I2C_TIMEOUT;
    while (!(I2C1->STAR1 & I2C_STAR1_SB) && (timeout--));
    if (timeout == -1) { i2c_error(); return 0; }

    // Send address + write
    I2C1->DATAR = TMP102_ADDR << 1;
    timeout = I2C_TIMEOUT;
    while (!(I2C1->STAR1 & I2C_STAR1_ADDR) && (timeout--));
    if (timeout == -1) { i2c_error(); return 0; }
    (void)I2C1->STAR2; // Clear ADDR flag

    // Send register pointer
    I2C1->DATAR = TMP102_REG_TEMP;
    timeout = I2C_TIMEOUT;
    while (!(I2C1->STAR1 & I2C_STAR1_TXE) && (timeout--));
    if (timeout == -1) { i2c_error(); return 0; }

    // --- Read phase: repeated start, read 2 bytes ---

    // Repeated START
    I2C1->CTLR1 |= I2C_CTLR1_START;
    timeout = I2C_TIMEOUT;
    while (!(I2C1->STAR1 & I2C_STAR1_SB) && (timeout--));
    if (timeout == -1) { i2c_error(); return 0; }

    // Send address + read
    I2C1->DATAR = (TMP102_ADDR << 1) | 1;
    timeout = I2C_TIMEOUT;
    while (!(I2C1->STAR1 & I2C_STAR1_ADDR) && (timeout--));
    if (timeout == -1) { i2c_error(); return 0; }
    (void)I2C1->STAR2; // Clear ADDR flag

    // ACK for first byte
    I2C1->CTLR1 |= I2C_CTLR1_ACK;
    timeout = I2C_TIMEOUT;
    while (!(I2C1->STAR1 & I2C_STAR1_RXNE) && (timeout--));
    if (timeout == -1) { i2c_error(); return 0; }
    msb = I2C1->DATAR;

    // NACK for last byte
    I2C1->CTLR1 &= ~I2C_CTLR1_ACK;
    I2C1->CTLR1 |= I2C_CTLR1_STOP;
    timeout = I2C_TIMEOUT;
    while (!(I2C1->STAR1 & I2C_STAR1_RXNE) && (timeout--));
    if (timeout == -1) { i2c_error(); return 0; }
    lsb = I2C1->DATAR;

    // TMP102: 12-bit result, MSB[7:0] + LSB[7:4], right-shift LSB by 4
    int16_t raw = ((int16_t)msb << 4) | (lsb >> 4);

    // Handle negative temperatures (sign extend from 12 bits)
    if (raw & 0x0800) {
        raw |= 0xF000;
    }

    return raw;
}
